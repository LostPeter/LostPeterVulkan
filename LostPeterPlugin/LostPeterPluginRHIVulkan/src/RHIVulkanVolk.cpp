/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-14
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanVolk.h"
#include "../include/volk.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanVolk::RHIVulkanVolk()
    {

    }

    RHIVulkanVolk::~RHIVulkanVolk()
    {

    }

    bool RHIVulkanVolk::VolkInitialize()
    {
        if (VK_SUCCESS != volkInitialize())
        {
            F_LogError("*********************** RHIVulkanVolk::VolkInitialize: Failed to call volkInitialize !");
            return false;
        }

        F_LogInfo("RHIVulkanVolk::VolkInitialize: Success to call volkInitialize !");
        return true;
    }

    void RHIVulkanVolk::VolkLoadInstanceOnly(VkInstance vkInstance)
    {
        volkLoadInstanceOnly(vkInstance);
        F_LogInfo("RHIVulkanVolk::VolkLoadInstanceOnly: Success to call volkLoadInstanceOnly !");
    }

    void RHIVulkanVolk::VolkLoadDevice(VkDevice vkDevice)
    {
        volkLoadDevice(vkDevice);
        F_LogInfo("RHIVulkanVolk::VolkLoadDevice: Success to call volkLoadDevice !");
    }

}; //LostPeterPluginRHIVulkan