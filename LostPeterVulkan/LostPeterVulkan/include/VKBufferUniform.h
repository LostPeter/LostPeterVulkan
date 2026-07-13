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
        virtual ~VKBufferUniform();

    public:
		size_t nBufferSize;
        uint8* pBuffer;
        bool bIsDelete;

        VkBuffer poBuffer_Uniform;  
        VkDeviceMemory poBufferMemory_Uniform;

    public:
        F_FORCEINLINE size_t GetBufferSize() const { return this->nBufferSize; }
        F_FORCEINLINE uint8* GetBuffer() const { return this->pBuffer; }
		F_FORCEINLINE const VkBuffer& GetVKBufferUniform() const { return this->poBuffer_Uniform; }
		F_FORCEINLINE const VkDeviceMemory& GetVKBufferUniformMemory() const { return this->poBufferMemory_Uniform; }
        
    public:
        virtual void Destroy();
        bool Init(size_t bufSize, 
				  uint8* pBuf,
				  bool isDelete);

        void UpdateBuffer();
        void UpdateBuffer(size_t offset, 
						  size_t bufSize, 
						  uint8* pBuf);

    };

}; //LostPeterVulkan

#endif