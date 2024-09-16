/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _CULL_COMPUTE_BUFFER_H_
#define _CULL_COMPUTE_BUFFER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport ComputeBuffer : public Base
    {
    public:
        ComputeBuffer(const String& nameComputeBuffer);
        ComputeBuffer(const String& nameComputeBuffer, int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        virtual ~ComputeBuffer();

    public:
        int nCount;
        int nStride;

        uint8* pBuffer_Compute;
        VkBuffer poBuffer_Compute;  
        VkDeviceMemory poBufferMemory_Compute;

    public:
        F_FORCEINLINE int GetBufferSize() const { return nCount * nStride; }

    public:
        void Destroy();
        void Init(int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    public:
        void UpdateBuffer();
        void UpdateBuffer(size_t offset, size_t bufSize, void* pBuf);

    };

}; //LostPeterVulkan

#endif