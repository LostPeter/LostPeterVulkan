/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_PLUGIN_H_
#define _VULKAN_PLUGIN_H_

#include "VulkanPreDefine.h"

namespace LostPeter
{
    class lpVulkanExport VulkanPlugin : public FPlugin
    {
    public:
        VulkanPlugin();
        virtual ~VulkanPlugin();

    public:
		static const String ms_strPluginName;

	protected:
		VulkanRenderer* m_pRendererVulkan;

	public:
		virtual const String& GetName() const;
		virtual void Install();
		virtual void Uninstall();
		virtual void Initialize();
		virtual void Shutdown();
    };

}; //LostPeter

#endif