/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-08
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_BUFFER_H_
#define _VK_BUFFER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBuffer : public Base
    {
    public:
        VKBuffer(const String& nameBuffer);
        virtual ~VKBuffer();

    public:
		VkBuffer poBuffer; 
        VkDeviceMemory poBufferMemory;
		VkBuffer poBuffer_Staging;
        VkDeviceMemory poBufferMemory_Staging;

	public:
		F_FORCEINLINE const VkBuffer& GetVkBuffer() const { return this->poBuffer; }
		F_FORCEINLINE const VkDeviceMemory& GetVkBufferMemory() const { return this->poBufferMemory; }
		F_FORCEINLINE const VkBuffer& GetVkBuffer_Staging() const { return this->poBuffer_Staging; }
		F_FORCEINLINE const VkDeviceMemory& GetVkBufferMemory_Staging() const { return this->poBufferMemory_Staging; }

    public:
        virtual void Destroy();


		virtual void UpdateBuffer() = 0;
        virtual void UpdateBuffer(size_t offset, size_t bufSize, uint8* pBuf) = 0;

	public:
		virtual void destroyVkBuffer();
		virtual bool createVkBuffer(const String& nameBuffer, 
									VkDeviceSize bufferSize,
								    VkBufferUsageFlags usage, 
                                    VkMemoryPropertyFlags properties);

		virtual void updateVkBuffer(size_t offset, 
									size_t bufSize, 
									void* pBuf, 
									VkMemoryMapFlags flags = 0);
    };

}; //LostPeterVulkan

#endif