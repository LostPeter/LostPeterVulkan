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

#include "../include/RHIVulkanShaderModule.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanShaderModule::RHIVulkanShaderModule(const RHIShaderModuleCreateInfo& createInfo)
        : RHIShaderModule(createInfo)
    {

    }

    RHIVulkanShaderModule::~RHIVulkanShaderModule()
    {
        Destroy();
    }

    void RHIVulkanShaderModule::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan