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

#ifndef _VK_BUFFER_UNIFORM_H_
#define _VK_BUFFER_UNIFORM_H_

#include "VKBuffer.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBufferUniform : public VKBuffer
    {
    public:
        VKBufferUniform(const String& nameUniformBuffer);
        VKBufferUniform(const String& nameUniformBuffer, int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
        virtual ~VKBufferUniform();

    public:
        int nCount;
        int nStride;

        uint8* pBuffer_Uniform;
        VkBuffer poBuffer_Uniform;  
        VkDeviceMemory poBufferMemory_Uniform;

    public:
        F_FORCEINLINE int GetBufferSize() const { return nCount * nStride; }
        F_FORCEINLINE void* GetBuffer() const { return pBuffer_Uniform; }
        
    public:
        virtual void Destroy();
        void Init(int count, int stride, VkBufferUsageFlagBits usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    public:
        void UpdateBuffer();
        void UpdateBuffer(size_t offset, size_t bufSize, void* pBuf);

    };

}; //LostPeterVulkan

#endif