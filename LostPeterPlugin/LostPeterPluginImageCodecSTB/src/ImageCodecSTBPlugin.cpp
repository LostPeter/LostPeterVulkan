/****************************************************************************
* LostPeterPluginImageCodecSTB - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ImageCodecSTBInclude.h"
#include "../include/ImageCodecSTBPlugin.h"
#include "../include/ImageCodecSTB.h"


namespace LostPeter
{
    ImageCodecSTBPlugin* g_pPlugin_ImageCodecSTB = nullptr;
	extern "C" void imageCodecSTBExport dllStartPlugin() throw()
	{
		g_pPlugin_ImageCodecSTB = new ImageCodecSTBPlugin();
		FPluginManager::GetSingleton().InstallPlugin(g_pPlugin_ImageCodecSTB);
	}
	extern "C" void imageCodecSTBExport dllStopPlugin()
	{
		FPluginManager::GetSingleton().UninstallPlugin(g_pPlugin_ImageCodecSTB);
		F_DELETE(g_pPlugin_ImageCodecSTB)
	}




    static const String s_strPluginName = "LostPeterPluginImageCodecSTB";
    ImageCodecSTBPlugin::ImageCodecSTBPlugin()
    {

    }

    ImageCodecSTBPlugin::~ImageCodecSTBPlugin()
    {

    }

    const String& ImageCodecSTBPlugin::GetName() const
    {
        return s_strPluginName;
    }

    void ImageCodecSTBPlugin::Install()
    {
        ImageCodecSTB::Startup();
    }

    void ImageCodecSTBPlugin::Uninstall()
    {
        ImageCodecSTB::Shutdown();
    }

    void ImageCodecSTBPlugin::Initialize()
    {

    }

    void ImageCodecSTBPlugin::Shutdown()
    {
        
    }

}; //LostPeter