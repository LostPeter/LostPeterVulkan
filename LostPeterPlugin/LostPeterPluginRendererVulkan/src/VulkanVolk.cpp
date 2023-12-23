/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanVolk.h"
#include "../include/volk.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanVolk::VulkanVolk()
    {

    }

    VulkanVolk::~VulkanVolk()
    {

    }

    bool VulkanVolk::VolkInitialize()
    {
        if (VK_SUCCESS != volkInitialize())
        {
            F_LogError("*********************** VulkanVolk::VolkInitialize: Failed to call volkInitialize !");
            return false;
        }

        F_LogInfo("VulkanVolk::VolkInitialize: Success to call volkInitialize !");
        return true;
    }

    void VulkanVolk::VolkLoadInstanceOnly(VkInstance vkInstance)
    {
        volkLoadInstanceOnly(vkInstance);
        F_LogInfo("VulkanVolk::VolkLoadInstanceOnly: Success to call volkLoadInstanceOnly !");
    }

    void VulkanVolk::VolkLoadDevice(VkDevice vkDevice)
    {
        volkLoadDevice(vkDevice);
        F_LogInfo("VulkanVolk::VolkLoadDevice: Success to call volkLoadDevice !");
    }

}; //LostPeterPluginRendererVulkan