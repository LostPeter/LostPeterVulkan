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
#include "../include/RHIVulkanInstance.h"

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
        : m_pInstance(nullptr)
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
        m_pInstance = new RHIVulkanInstance();
        if (!m_pInstance->Init())
        {
            F_LogError("*********************** RHIVulkanPlugin::Install: Failed to init RHIVulkanInstance !");
            return;
        }
        F_LogInfo("RHIVulkanPlugin::Install: Success to init RHIVulkanInstance !");
    }

    void RHIVulkanPlugin::Uninstall()
    {
        F_DELETE(m_pInstance)
        F_LogInfo("RHIVulkanPlugin::Uninstall: Success to delete RHIVulkanInstance !");
    }

    void RHIVulkanPlugin::Initialize()
    {

    }

    void RHIVulkanPlugin::Shutdown()
    {
        
    }
    

}; //LostPeterPluginRHIVulkan