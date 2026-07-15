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

#ifndef _VK_BUFFER_COMPUTE_H_
#define _VK_BUFFER_COMPUTE_H_

#include "VKBuffer.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBufferCompute : public VKBuffer
    {
    public:
        VKBufferCompute(const String& nameComputeBuffer);
        VKBufferCompute(const String& nameComputeBuffer, int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
        virtual ~VKBufferCompute();

    public:
        int nCount;
        int nStride;
        uint8* pBuffer_Compute;

    public:
        F_FORCEINLINE int GetBufferSize() const { return nCount * nStride; }
        F_FORCEINLINE void* GetBuffer() const { return pBuffer_Compute; }
        
    public:
        virtual void Destroy();
        bool Init(int count, 
				  int stride, 
				  VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    public:
        virtual void UpdateBuffer();
        virtual void UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf);

    };

}; //LostPeterVulkan

#endif