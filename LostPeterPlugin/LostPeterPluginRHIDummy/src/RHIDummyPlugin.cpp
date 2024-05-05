/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyPlugin.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPlugin* g_pPlugin_RHIDummy = nullptr;
	extern "C" void rhiDummyExport dllStartPlugin() throw()
	{
		g_pPlugin_RHIDummy = new RHIDummyPlugin();
		FPluginManager::GetSingleton().InstallPlugin(g_pPlugin_RHIDummy);
	}
	extern "C" void rhiDummyExport dllStopPlugin()
	{
		FPluginManager::GetSingleton().UninstallPlugin(g_pPlugin_RHIDummy);
		F_DELETE(g_pPlugin_RHIDummy)
	}




    static const String s_strPluginName = "LostPeterPluginRHIDummy";
    RHIDummyPlugin::RHIDummyPlugin()
    {

    }

    RHIDummyPlugin::~RHIDummyPlugin()
    {

    }

    const String& RHIDummyPlugin::GetName() const
    {
        return s_strPluginName;
    }

    void RHIDummyPlugin::Install()
    {

    }

    void RHIDummyPlugin::Uninstall()
    {
        
    }

    void RHIDummyPlugin::Initialize()
    {

    }

    void RHIDummyPlugin::Shutdown()
    {
        
    }
    

}; //LostPeterPluginRHIDummy