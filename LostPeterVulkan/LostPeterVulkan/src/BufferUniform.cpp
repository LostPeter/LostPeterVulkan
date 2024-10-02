/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/BufferUniform.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    BufferUniform::BufferUniform(const String& nameUniformBuffer)
        : Base(nameUniformBuffer)
    
        , nCount(0)
        , nStride(0)

        , pBuffer_Uniform(nullptr)
        , poBuffer_Uniform(VK_NULL_HANDLE)
        , poBufferMemory_Uniform(VK_NULL_HANDLE)
    {

    }

    BufferUniform::BufferUniform(const String& nameUniformBuffer, int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT*/)
        : Base(nameUniformBuffer)

        , nCount(count)
        , nStride(stride)

        , pBuffer_Uniform(nullptr)
        , poBuffer_Uniform(VK_NULL_HANDLE)
        , poBufferMemory_Uniform(VK_NULL_HANDLE)
    {
        F_Assert(nCount > 0 && nStride > 0 && "BufferUniform::BufferUniform")

        Init(count, stride, usage);
    }
    
    BufferUniform::~BufferUniform()
    {
        Destroy();
    }

    void BufferUniform::Destroy()
    {
        F_DELETE_T(pBuffer_Uniform)
        if (this->poBuffer_Uniform != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_Uniform, this->poBufferMemory_Uniform);
        }
        this->poBuffer_Uniform = VK_NULL_HANDLE;
        this->poBufferMemory_Uniform = VK_NULL_HANDLE;
    }

    void BufferUniform::Init(int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT*/)
    {
        Destroy();

        this->pBuffer_Uniform = new uint8[count * stride];
        memset(this->pBuffer_Uniform, 0, count * stride);
        VkDeviceSize bufferSize = count * stride;
        Base::GetWindowPtr()->createVkBuffer(this->name, bufferSize, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_Uniform, this->poBufferMemory_Uniform);
        UpdateBuffer();
    }
    
    void BufferUniform::UpdateBuffer()
    {
        if (this->pBuffer_Uniform == nullptr ||
            this->poBufferMemory_Uniform == VK_NULL_HANDLE)
        {
            return;
        }
        Base::GetWindowPtr()->updateVKBuffer(0, (size_t)GetBufferSize(), this->pBuffer_Uniform, this->poBufferMemory_Uniform);
    }
    void BufferUniform::UpdateBuffer(size_t offset, size_t bufSize, void* pBuf)
    {
        size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "BufferUniform::UpdateBuffer")

        uint8* pBuffer = this->pBuffer_Uniform + offset;
        memcpy(pBuffer, pBuf, bufSize);
        UpdateBuffer();
    }

}; //LostPeterVulkan