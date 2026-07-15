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

#include "../include/VKBufferCompute.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferCompute::VKBufferCompute(const String& nameComputeBuffer)
        : VKBuffer(nameComputeBuffer)
    
        , nCount(0)
        , nStride(0)
        , pBuffer_Compute(nullptr)
    {

    }

    VKBufferCompute::VKBufferCompute(const String& nameComputeBuffer, int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
        : VKBuffer(nameComputeBuffer)

        , nCount(count)
        , nStride(stride)
        , pBuffer_Compute(nullptr)
    {
        F_Assert(nCount > 0 && nStride > 0 && "VKBufferCompute::VKBufferCompute")

        Init(count, stride, usage);
    }
    
    VKBufferCompute::~VKBufferCompute()
    {
        Destroy();
    }

    void VKBufferCompute::Destroy()
    {
		VKBuffer::Destroy();

        F_DELETE_T(pBuffer_Compute)
    }

    bool VKBufferCompute::Init(int count, 
							   int stride, 
							   VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
    {
        Destroy();
		if (count <= 0)
        {
			F_LogError("*********************** VKBufferCompute::Init: Empty buffer: [%s] !", this->name.c_str());
			return false;
		}

        this->pBuffer_Compute = new uint8[count * stride];
        memset(this->pBuffer_Compute, 0, count * stride);
        VkDeviceSize bufferSize = count * stride;

		if (!createVkBuffer(this->name, 
							bufferSize, 
							usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			F_LogError("*********************** VKBufferCompute::Init: Failed to create buffer: [%s] !", this->name.c_str());
			return false;
		}
        UpdateBuffer();

		return true;
    }
    
    void VKBufferCompute::UpdateBuffer()
    {	
		if (this->pBuffer_Compute == nullptr)
			return;

		updateVkBuffer(0, 
					   (size_t)GetBufferSize(), 
					   this->pBuffer_Compute);
    }

    void VKBufferCompute::UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf)
    {
        size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "VKBufferCompute::UpdateBuffer")

        uint8* pBuffer = this->pBuffer_Compute + offset;
        memcpy(pBuffer, pBuf, bufSize);
        UpdateBuffer();
    }

}; //LostPeterVulkan