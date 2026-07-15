/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-08
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKBufferStorage.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBufferStorage::VKBufferStorage(const String& nameStorageBuffer)
        : VKBuffer(nameStorageBuffer)
    
        , nCount(0)
        , nStride(0)
        , pBuffer_Storage(nullptr)
    {

    }

    VKBufferStorage::VKBufferStorage(const String& nameStorageBuffer, int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
        : VKBuffer(nameStorageBuffer)

        , nCount(count)
        , nStride(stride)
        , pBuffer_Storage(nullptr)
    {
        F_Assert(nCount > 0 && nStride > 0 && "VKBufferStorage::VKBufferStorage")

        Init(count, stride, usage);
    }
    
    VKBufferStorage::~VKBufferStorage()
    {
        Destroy();
    }

    void VKBufferStorage::Destroy()
    {
		VKBuffer::Destroy();

        F_DELETE_T(pBuffer_Storage)
    }

    bool VKBufferStorage::Init(int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
    {
        Destroy();
		if (count <= 0)
        {
			F_LogError("*********************** VKBufferStorage::Init: Empty buffer: [%s] !", this->name.c_str());
			return false;
		}

        this->pBuffer_Storage = new uint8[count * stride];
        memset(this->pBuffer_Storage, 0, count * stride);
        VkDeviceSize bufferSize = count * stride;

		if (!createVkBuffer(this->name, 
							bufferSize, 
							usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
							VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))
		{
			F_LogError("*********************** VKBufferStorage::Init: Failed to create buffer: [%s] !", this->name.c_str());
			return false;
		}
		UpdateBuffer();

		return true;
    }
    
    void VKBufferStorage::UpdateBuffer()
    {
		if (this->pBuffer_Storage == nullptr)
			return;

		updateVkBuffer(0, 
					   (size_t)GetBufferSize(), 
					   this->pBuffer_Storage);
    }
    void VKBufferStorage::UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf)
    {
        size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "VKBufferStorage::UpdateBuffer")

        CopyBuffer(offset, bufSize, pBuf);
        UpdateBuffer();
    }

    void VKBufferStorage::CopyBuffer(size_t offset, size_t bufSize, void* pBuf)
    {
        uint8* pBuffer = this->pBuffer_Storage + offset;
        memcpy(pBuffer, pBuf, bufSize);
    }
    void VKBufferStorage::RemoveBufferGap(size_t current, size_t start, size_t end, bool isUpdateBuffer)
    {
        size_t total = (size_t)GetBufferSize();
        size_t count = current - end;
        uint8* pBufferSrc = this->pBuffer_Storage + start;
        uint8* pBufferDst = this->pBuffer_Storage + end;
        for (size_t i = 0; i < count; i++)
        {
            *pBufferSrc = *pBufferDst;

            ++pBufferSrc;
            ++pBufferDst;
        }

        if (isUpdateBuffer)
        {
            UpdateBuffer();
        }   
    }

}; //LostPeterVulkan