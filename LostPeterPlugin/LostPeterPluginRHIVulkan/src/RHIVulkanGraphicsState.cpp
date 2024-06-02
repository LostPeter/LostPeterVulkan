/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanGraphicsState.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanGraphicsState::RHIVulkanGraphicsState(RHIVulkanDevice* pVulkanDevice)
        : RHIGraphicsState(pVulkanDevice)
        , RHIVulkanObject(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanGraphicsState::RHIVulkanGraphicsState")

    }   

    RHIVulkanGraphicsState::~RHIVulkanGraphicsState()
    {
        Destroy();
    }

    void RHIVulkanGraphicsState::Destroy()
    {
        
    }
    

}; //LostPeterPluginRHIVulkan