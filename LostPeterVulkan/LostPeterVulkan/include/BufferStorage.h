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

#ifndef _CULL_BUFFER_STORAGE_H_
#define _CULL_BUFFER_STORAGE_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport BufferStorage : public Base
    {
    public:
        BufferStorage(const String& nameStorageBuffer);
        BufferStorage(const String& nameStorageBuffer, int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        virtual ~BufferStorage();

    public:
        int nCount;
        int nStride;

        uint8* pBuffer_Storage;
        VkBuffer poBuffer_Storage;  
        VkDeviceMemory poBufferMemory_Storage;

    public:
        F_FORCEINLINE int GetBufferSize() const { return nCount * nStride; }
        F_FORCEINLINE void* GetBuffer() const { return pBuffer_Storage; }
        
    public:
        void Destroy();
        void Init(int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    public:
        void UpdateBuffer();
        void UpdateBuffer(size_t offset, size_t bufSize, void* pBuf);

    };

}; //LostPeterVulkan

#endif