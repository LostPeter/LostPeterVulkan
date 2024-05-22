/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanObjectManager.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanObjectManager::RHIVulkanObjectManager(RHIVulkanDevice* pVulkanDevice)
        : RHIObjectManager(pVulkanDevice)
        , m_pVulkanDevice(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanObjectManager::RHIVulkanObjectManager")
    }   

    RHIVulkanObjectManager::~RHIVulkanObjectManager()
    {
        Destroy();
    }

    void RHIVulkanObjectManager::Destroy()
    {

    }
    
}; //LostPeterPluginRHIVulkan