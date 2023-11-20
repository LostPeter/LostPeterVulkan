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

#include "../include/VulkanQueue.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanQueue::VulkanQueue()
        : m_vkQueue(VK_NULL_HANDLE)
        , m_nFamilyIndex(-1)
        , m_pDevice(nullptr)
    {

    }

    VulkanQueue::~VulkanQueue()
    {
        Destroy();
    }

    void VulkanQueue::Destroy()
    {
        if (m_vkQueue != VK_NULL_HANDLE)
        {

        }
        m_vkQueue = VK_NULL_HANDLE;
    }

    bool VulkanQueue::Init(VulkanDevice* pDevice, uint32 nFamilyIndex)
    {
        m_pDevice = pDevice;
        m_nFamilyIndex = nFamilyIndex;
        m_vkQueue = m_pDevice->GetVkQueue(m_nFamilyIndex, 0);
        if (m_vkQueue == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VulkanQueue::Init: GetVkQueue failed !");
            return false;
        }

        return true;
    }

}; //LostPeter