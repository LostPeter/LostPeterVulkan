/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SEMAPHORE_H_
#define _VULKAN_SEMAPHORE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanSemaphore
    {
    public:
        VulkanSemaphore(VulkanDevice* pDevice);
	    virtual ~VulkanSemaphore();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkSemaphore m_vkSemaphore;

    public:
        F_FORCEINLINE VkSemaphore GetVkSemaphore() const { return m_vkSemaphore; }

    public:
        void Destroy();
    };

}; //LostPeterPluginRendererVulkan

#endif