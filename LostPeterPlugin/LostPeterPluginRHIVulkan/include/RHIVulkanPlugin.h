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

#ifndef _RHI_VULKAN_PLUGIN_H_
#define _RHI_VULKAN_PLUGIN_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class RHIVulkanPlugin : public FPlugin
    {
    public:
        RHIVulkanPlugin();
        virtual ~RHIVulkanPlugin();

    public:
    protected:
        RHIVulkanInstance* m_pVulkanInstance;

    public:
        virtual const String& GetName() const;
        virtual void Install();
        virtual void Uninstall();
        virtual void Initialize();
        virtual void Shutdown();
    };

}; //LostPeterPluginRHIVulkan

#endif