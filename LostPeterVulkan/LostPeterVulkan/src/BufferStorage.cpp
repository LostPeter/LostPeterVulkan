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

#include "../include/BufferStorage.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    BufferStorage::BufferStorage(const String& nameStorageBuffer)
        : Base(nameStorageBuffer)
    
        , nCount(0)
        , nStride(0)

        , pBuffer_Storage(nullptr)
        , poBuffer_Storage(VK_NULL_HANDLE)
        , poBufferMemory_Storage(VK_NULL_HANDLE)
    {

    }

    BufferStorage::BufferStorage(const String& nameStorageBuffer, int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
        : Base(nameStorageBuffer)

        , nCount(count)
        , nStride(stride)

        , pBuffer_Storage(nullptr)
        , poBuffer_Storage(VK_NULL_HANDLE)
        , poBufferMemory_Storage(VK_NULL_HANDLE)
    {
        F_Assert(nCount > 0 && nStride > 0 && "BufferStorage::BufferStorage")

        Init(count, stride, usage);
    }
    
    BufferStorage::~BufferStorage()
    {
        Destroy();
    }

    void BufferStorage::Destroy()
    {
        F_DELETE_T(pBuffer_Storage)
        if (this->poBuffer_Storage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_Storage, this->poBufferMemory_Storage);
        }
        this->poBuffer_Storage = VK_NULL_HANDLE;
        this->poBufferMemory_Storage = VK_NULL_HANDLE;
    }

    void BufferStorage::Init(int count, int stride, VkBufferUsageFlagBits usage /*= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT*/)
    {
        Destroy();

        this->pBuffer_Storage = new uint8[count * stride];
        memset(this->pBuffer_Storage, 0, count * stride);
        VkDeviceSize bufferSize = count * stride;
        Base::GetWindowPtr()->createVkBuffer(this->name, bufferSize, usage | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_Storage, this->poBufferMemory_Storage);
        UpdateBuffer();
    }
    
    void BufferStorage::UpdateBuffer()
    {
        if (this->pBuffer_Storage == nullptr ||
            this->poBufferMemory_Storage == VK_NULL_HANDLE)
        {
            return;
        }
        Base::GetWindowPtr()->updateVKBuffer(0, (size_t)GetBufferSize(), this->pBuffer_Storage, this->poBufferMemory_Storage);
    }
    void BufferStorage::UpdateBuffer(size_t offset, size_t bufSize, void* pBuf)
    {
        size_t size = GetBufferSize();
        F_Assert(offset >= 0 && offset < size && bufSize <= size && "BufferStorage::UpdateBuffer")

        CopyBuffer(offset, bufSize, pBuf);
        UpdateBuffer();
    }

    void BufferStorage::CopyBuffer(size_t offset, size_t bufSize, void* pBuf)
    {
        uint8* pBuffer = this->pBuffer_Storage + offset;
        memcpy(pBuffer, pBuf, bufSize);
    }
    void BufferStorage::RemoveBufferGap(size_t current, size_t start, size_t end, bool isUpdateBuffer)
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