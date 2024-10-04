/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/BufferCompute.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    BufferCompute::BufferCompute(const String& nameComputeBuffer)
        : Base(nameComputeBuffer)
    
        , nCount(0)
        , nStride(0)

        , pBuffer_Compute(nullptr)
        , poBuffer_Compute(VK_NULL_HANDLE)
        , poBufferMemory_Compute(VK_NULL_HANDLE)
    {

    }

    BufferCompute::BufferCompute(const String& nameComputeBuffer, int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
        : Base(nameComputeBuffer)

        , nCount(count)
        , nStride(stride)

        , pBuffer_Compute(nullptr)
        , poBuffer_Compute(VK_NULL_HANDLE)
        , poBufferMemory_Compute(VK_NULL_HANDLE)
    {
        F_Assert(nCount > 0 && nStride > 0 && "BufferCompute::BufferCompute")

        Init(count, stride, usage);
    }
    
    BufferCompute::~BufferCompute()
    {
        Destroy();
    }

    void BufferCompute::Destroy()
    {
        F_DELETE_T(pBuffer_Compute)
        if (this->poBuffer_Compute != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_Compute, this->poBufferMemory_Compute);
        }
        this->poBuffer_Compute = VK_NULL_HANDLE;
        this->poBufferMemory_Compute = VK_NULL_HANDLE;
    }

    void BufferCompute::Init(int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
    {
        Destroy();

        this->pBuffer_Compute = new uint8[count * stride];
        memset(this->pBuffer_Compute, 0, count * stride);
        VkDeviceSize bufferSize = count * stride;
        Base::GetWindowPtr()->createVkBuffer(this->name, bufferSize, usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_Compute, this->poBufferMemory_Compute);
        UpdateBuffer();
    }
    
    void BufferCompute::UpdateBuffer()
    {
        if (this->pBuffer_Compute == nullptr ||
            this->poBufferMemory_Compute == VK_NULL_HANDLE)
        {
            return;
        }
        Base::GetWindowPtr()->updateVKBuffer(0, (size_t)GetBufferSize(), this->pBuffer_Compute, this->poBufferMemory_Compute);
    }
    void BufferCompute::UpdateBuffer(size_t offset, size_t bufSize, void* pBuf)
    {
        size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "BufferCompute::UpdateBuffer")

        uint8* pBuffer = this->pBuffer_Compute + offset;
        memcpy(pBuffer, pBuf, bufSize);
        UpdateBuffer();
    }

}; //LostPeterVulkan