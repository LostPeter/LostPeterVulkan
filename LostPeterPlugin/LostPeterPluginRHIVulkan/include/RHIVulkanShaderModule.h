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

#ifndef _RHI_VULKAN_SHADER_MODULE_H_
#define _RHI_VULKAN_SHADER_MODULE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanShaderModule : public RHIShaderModule
    {
    public:
        RHIVulkanShaderModule(const RHIShaderModuleCreateInfo& createInfo);
        virtual ~RHIVulkanShaderModule();

    public:
    protected:


    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIVulkan

#endif