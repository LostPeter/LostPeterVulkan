/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Engine.h"
#include "../include/VulkanWindow.h"
#include "../include/EngineConfig.h"
#include "../include/RenderEngine.h"


template<> LostPeter::Engine* LostPeterFoundation::FSingleton<LostPeter::Engine>::ms_Singleton = nullptr;

namespace LostPeter
{
    Engine* Engine::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	Engine& Engine::GetSingleton()
	{  
		F_Assert(ms_Singleton && "Engine::GetSingleton")
		return (*ms_Singleton);     
	}

#if LP_PLATFORM == LP_PLATFORM_WIN32      
	FPlatformType Engine::ms_typePlatform = F_Platform_Windows;
    String Engine::ms_strNamePlatform = F_GetPlatformTypeName(F_Platform_Windows);
#elif LP_PLATFORM == LP_PLATFORM_MAC
    FPlatformType Engine::ms_typePlatform = F_Platform_MacOS;
	String Engine::ms_strNamePlatform = F_GetPlatformTypeName(F_Platform_MacOS);
#elif LP_PLATFORM == LP_PLATFORM_LINUX
    FPlatformType Engine::ms_typePlatform = F_Platform_Linux;
	String Engine::ms_strNamePlatform = F_GetPlatformTypeName(F_Platform_Linux);                       
#elif LP_PLATFORM == LP_PLATFORM_ANDROID
    FPlatformType Engine::ms_typePlatform = F_Platform_Android;
	String Engine::ms_strNamePlatform = F_GetPlatformTypeName(F_Platform_Android);                     
#elif LP_PLATFORM == LP_PLATFORM_IOS
    FPlatformType Engine::ms_typePlatform = F_Platform_iOS;
	String Engine::ms_strNamePlatform = F_GetPlatformTypeName(F_Platform_iOS);
#else 
    #pragma error "UnKnown platform, Engine.cpp Abort! Abort!"
#endif

    bool Engine::ms_bUseBinaryResource = false;
    String Engine::ms_strPathCfgEngine = "Assets/config/cfg_engine.xml";
	String Engine::ms_strPathCfgPlugin = "Assets/config/cfg_plugin.xml";

    Engine::Engine()
        : Base("Engine")
        , m_pPathManager(nullptr)
        , m_pFileManager(nullptr)
        , m_pPluginManager(nullptr)
        , m_pRenderEngine(nullptr)

        , m_nFrameCurrent(0)
        , m_nFrameCount(0)
        , m_nTimeLast(0)
        , m_bRenderAutoStop(false)

        , m_pathCfg_Engine("")
        , m_pathCfg_Plugin("")
        , m_folder_Plugin("")
        , m_bEngineIsInit(false)
    {

    }
    
    Engine::~Engine()
    {
        Destroy();
    }

    void Engine::Destroy()
    {
        RemoveEngineListenerAll();
        if (m_pPluginManager != nullptr)
        {
            m_pPluginManager->UnloadPlugins();
        }
        F_DELETE(m_pPluginManager)

        F_DELETE(m_pRenderEngine)

        F_DELETE(m_pFileManager)
		F_DELETE(m_pPathManager)
		m_bEngineIsInit = false;
    }

    bool Engine::Init(const String& workFolder, bool bAutoCreateWindow)
    {
		m_pathCfg_Engine = workFolder + ms_strPathCfgEngine;
		m_pathCfg_Plugin = workFolder + ms_strPathCfgPlugin;
		m_folder_Plugin = workFolder + "Assets/Plugins/" + ms_strNamePlatform + "/";
		initCommonCfgItems();

        //1, PreCommon 
        {

        }

        //2, All Engines
        {

        }

        //3, Plugin
        {

        }

        //4, Initialize 
        {

        }

        //5, PostCommon
        {

        }

        m_bEngineIsInit = true;
		return true;
    }   
    void Engine::initCommonCfgItems()
	{
		m_mapEngineCommonCfgItem.clear();

		//<1> multi thread
		ConfigItem cfgItem;
		cfgItem.strName	= Util_GetEngineConfigTypeName(Vulkan_EngineConfig_Common_MultiThread);
		String strDefault("1");
		cfgItem.aPossibleValues.push_back(strDefault);
		cfgItem.strCurValue = strDefault;
		cfgItem.bImmutable	= false;
		m_mapEngineCommonCfgItem[cfgItem.strName] = cfgItem;


	}
    bool Engine::loadConfigEngine(const String& strPath)
	{
		if (strPath.empty())
			return false;

		EngineConfig cfg;
		if (!cfg.Load(strPath, this))
			return false;

		return true;
	}

    void Engine::RenderAuto()
    {

    }

    void Engine::RenderStop()
	{
		m_bRenderAutoStop = true;
	}

	bool Engine::RenderOneFrame()
    {
        return true;
    }


    void Engine::clearEventTimes()
    {

    }

    bool Engine::fireFrameStarted(RenderFrameEvent& event)
    {
        EngineListenerPtrMultiSet::iterator it, itEnd;
		itEnd = m_setRemovedEngineListeners.end();
		for (it = m_setRemovedEngineListeners.begin(); it != itEnd; ++it)
		{
			m_setEngineListeners.erase(*it);
		}
		m_setRemovedEngineListeners.clear();

		//Listener Event
		itEnd = m_setEngineListeners.end();
		for (it = m_setEngineListeners.begin(); it != itEnd; ++it)
		{
			if (!(*it)->RenderFrameStarted(event))
				return false;
		}

		return true;
    }

    bool Engine::fireFrameRenderingQueued(RenderFrameEvent& event)
    {
        ++ m_nFrameCurrent;
		EngineListenerPtrMultiSet::iterator it, itEnd;
		itEnd = m_setRemovedEngineListeners.end();
		for (it = m_setRemovedEngineListeners.begin(); it != itEnd; ++it)
		{
			m_setEngineListeners.erase(*it);
		}
		m_setRemovedEngineListeners.clear();

		//Listener Event
		itEnd = m_setEngineListeners.end();
		for (it = m_setEngineListeners.begin(); it != itEnd; ++it)
		{
			if (!(*it)->RenderFrameRenderingQueued(event))
				return false;
		}

		return true;
    }

    bool Engine::fireFrameEnded(RenderFrameEvent& event)
    {
        bool bRet = true;
		EngineListenerPtrMultiSet::iterator it,itEnd;
		itEnd = m_setRemovedEngineListeners.end();
		for (it = m_setRemovedEngineListeners.begin(); it != itEnd; ++it)
		{
			m_setEngineListeners.erase(*it);
		}
		m_setRemovedEngineListeners.clear();

		//Listener Event
		itEnd = m_setEngineListeners.end();
		for (it = m_setEngineListeners.begin(); it != itEnd; ++it)
		{
			if (!(*it)->RenderFrameEnded(event))
			{
				bRet = false;
				break;
			}
		}

		return bRet;
    }

    bool Engine::SaveConfig()
    {
        return true;
    }

    bool Engine::RestoreConfig()
    {
        return true;
    }

    void Engine::AddEngineListener(EngineListener* pEngineListener)
    {
        EngineListenerPtrMultiSet::iterator itFind = m_setRemovedEngineListeners.find(pEngineListener);
		if (itFind != m_setRemovedEngineListeners.end())
			m_setRemovedEngineListeners.erase(*itFind);
		else
			m_setEngineListeners.insert(pEngineListener);
    }
    void Engine::RemoveEngineListener(EngineListener* pEngineListener)
    {
        m_setRemovedEngineListeners.insert(pEngineListener);
    }
    void Engine::RemoveEngineListenerAll()
    {
        m_setEngineListeners.clear();
		m_setRemovedEngineListeners.clear();
    }

}; //LostPeter