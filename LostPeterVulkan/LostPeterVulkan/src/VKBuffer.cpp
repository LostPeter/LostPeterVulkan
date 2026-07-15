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

#include "../include/VKBuffer.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKBuffer::VKBuffer(const String& nameBuffer)
        : Base(nameBuffer)

		, poBuffer(VK_NULL_HANDLE)
        , poBufferMemory(VK_NULL_HANDLE)
		, poBuffer_Staging(VK_NULL_HANDLE)
        , poBufferMemory_Staging(VK_NULL_HANDLE)
    {

    }
    VKBuffer::~VKBuffer()
    {
       
    }

	void VKBuffer::Destroy()
	{
		destroyVkBuffer();
	}

	void VKBuffer::destroyVkBuffer()
	{
		if (this->poBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer, this->poBufferMemory);
        }
        this->poBuffer = VK_NULL_HANDLE;
        this->poBufferMemory = VK_NULL_HANDLE;

		if (this->poBuffer_Staging != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_Staging, this->poBufferMemory_Staging);
        }
        this->poBuffer_Staging = VK_NULL_HANDLE;
        this->poBufferMemory_Staging = VK_NULL_HANDLE;
	}

	bool VKBuffer::createVkBuffer(const String& nameBuffer, 
								  VkDeviceSize bufferSize,
								  VkBufferUsageFlags usage, 
								  VkMemoryPropertyFlags properties)
	{
		if (!Base::GetWindowPtr()->createVkBuffer(nameBuffer,
												  bufferSize,
												  usage,
												  properties,
												  this->poBuffer,
												  this->poBufferMemory))
		{
			F_LogError("*********************** VKBuffer::createVkBuffer: Failed to create buffer: [%s] !", nameBuffer.c_str());
			return false;
		}
		return true;
	}

	void VKBuffer::updateVkBuffer(size_t offset, 
								  size_t bufSize, 
								  void* pBuf, 
								  VkMemoryMapFlags flags /*= 0*/)
	{
		if (pBuf == nullptr ||
            this->poBufferMemory == VK_NULL_HANDLE)
        {
            return;
        }

		Base::GetWindowPtr()->updateVKBuffer(offset, 
											 bufSize, 
											 pBuf, 
											 this->poBufferMemory);
	}

}; //LostPeterVulkan