/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanSemaphore.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanSemaphore::VulkanSemaphore(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
        , m_vkSemaphore(VK_NULL_HANDLE)
    {
        assert(m_pDevice && "VulkanSemaphore::VulkanSemaphore");
        m_vkSemaphore = m_pDevice->CreateVkSemaphore();
        assert(m_vkSemaphore != VK_NULL_HANDLE && "VulkanSemaphore::VulkanSemaphore");
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

}; //LostPeter