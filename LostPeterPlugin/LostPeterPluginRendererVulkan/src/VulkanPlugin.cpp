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

#include "../include/VulkanPlugin.h"
#include "../include/VulkanRenderer.h"

namespace LostPeter
{
    VulkanPlugin* g_pPlugin_RendererVulkan = nullptr;
	extern "C" void vulkanExport dllStartPlugin() throw()
	{
		g_pPlugin_RendererVulkan = new VulkanPlugin();
		FPluginManager::GetSingleton().InstallPlugin(g_pPlugin_RendererVulkan);
	}
	extern "C" void vulkanExport dllStopPlugin()
	{
		FPluginManager::GetSingleton().UninstallPlugin(g_pPlugin_RendererVulkan);
		F_DELETE(g_pPlugin_RendererVulkan)
	}


    const String VulkanPlugin::ms_strPluginName = "LostPeterPluginRendererVulkan";
    VulkanPlugin::VulkanPlugin()
        : m_pRendererVulkan(nullptr)
    {

    }

	VulkanPlugin::~VulkanPlugin()
    {

    }

    const String& VulkanPlugin::GetName() const
    {
        return ms_strPluginName;
    }

	void VulkanPlugin::Install()
	{
        m_pRendererVulkan = new VulkanRenderer();
        if (RenderEngine::GetSingleton().AddRenderer(m_pRendererVulkan))
        {
            F_LogInfo("VulkanPlugin::Install: Add renderer vulkan to RenderEngine success !");
        }
        else
        {
            F_LogError("*********************** VulkanPlugin::Install: Add renderer vulkan to RenderEngine failed !");
        }
    }

    void VulkanPlugin::Uninstall()
	{
        RenderEngine::GetSingleton().RemoveRenderer(m_pRendererVulkan);
        F_LogInfo("VulkanPlugin::Uninstall: Remove renderer vulkan from RenderEngine success !");
        F_DELETE(m_pRendererVulkan)
    }

    void VulkanPlugin::Initialize()
    {

    }

    void VulkanPlugin::Shutdown()
    {

    }

}; //LostPeter