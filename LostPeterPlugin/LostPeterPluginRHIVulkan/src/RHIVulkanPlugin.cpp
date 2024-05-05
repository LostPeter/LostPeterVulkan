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

#include "../include/RHIVulkanPlugin.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPlugin* g_pPlugin_RHIVulkan = nullptr;
	extern "C" void rhiVulkanExport dllStartPlugin() throw()
	{
		g_pPlugin_RHIVulkan = new RHIVulkanPlugin();
		FPluginManager::GetSingleton().InstallPlugin(g_pPlugin_RHIVulkan);
	}
	extern "C" void rhiVulkanExport dllStopPlugin()
	{
		FPluginManager::GetSingleton().UninstallPlugin(g_pPlugin_RHIVulkan);
		F_DELETE(g_pPlugin_RHIVulkan)
	}




    static const String s_strPluginName = "LostPeterPluginRHIVulkan";
    RHIVulkanPlugin::RHIVulkanPlugin()
    {

    }

    RHIVulkanPlugin::~RHIVulkanPlugin()
    {

    }

    const String& RHIVulkanPlugin::GetName() const
    {
        return s_strPluginName;
    }

    void RHIVulkanPlugin::Install()
    {

    }

    void RHIVulkanPlugin::Uninstall()
    {
        
    }

    void RHIVulkanPlugin::Initialize()
    {

    }

    void RHIVulkanPlugin::Shutdown()
    {
        
    }
    

}; //LostPeterPluginRHIVulkan