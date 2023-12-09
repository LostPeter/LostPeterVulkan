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

#include "../include/VulkanSemaphore.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanSemaphore::VulkanSemaphore(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
        , m_vkSemaphore(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanSemaphore::VulkanSemaphore")
        m_vkSemaphore = m_pDevice->CreateVkSemaphore();
        F_Assert(m_vkSemaphore != VK_NULL_HANDLE && "VulkanSemaphore::VulkanSemaphore")
    }

	VulkanSemaphore::~VulkanSemaphore()
    {
        Destroy();
    }

    void VulkanSemaphore::Destroy()
    {
        m_pDevice->DestroyVkSemaphore(m_vkSemaphore);
        m_vkSemaphore = VK_NULL_HANDLE;
    }

}; //LostPeterPluginRendererVulkan