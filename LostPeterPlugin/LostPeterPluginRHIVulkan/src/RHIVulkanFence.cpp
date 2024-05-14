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

#include "../include/RHIVulkanFence.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanFence::RHIVulkanFence(RHIDevice& device)
        : RHIFence(device)
    {

    }

    RHIVulkanFence::~RHIVulkanFence()
    {

    }

    void RHIVulkanFence::Destroy()
    {

    }

    RHIFenceStatusType RHIVulkanFence::GetStatus()
    {
        return RHIFenceStatusType::RHI_FenceStatus_Signaled;
    }

    void RHIVulkanFence::Reset()
    {

    }

    void RHIVulkanFence::Wait()
    {

    }
    
}; //LostPeterPluginRHIVulkan