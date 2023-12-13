/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Engine.h"
#include "../include/EngineConfig.h"
#include "../include/RenderEngine.h"
#include "../include/SystemCapabilities.h"
#include "../include/EngineFrameProfiler.h"
#include "../include/ImageCodecDXT.h"

template<> LostPeterEngine::Engine* LostPeterFoundation::FSingleton<LostPeterEngine::Engine>::ms_Singleton = nullptr;

namespace LostPeterEngine
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

#if F_PLATFORM == F_PLATFORM_WINDOW      
	FPlatformType Engine::ms_ePlatform = F_Platform_Windows;
#elif F_PLATFORM == F_PLATFORM_MAC
    FPlatformType Engine::ms_ePlatform = F_Platform_MacOS;
#elif F_PLATFORM == F_PLATFORM_LINUX
    FPlatformType Engine::ms_ePlatform = F_Platform_Linux;                    
#elif F_PLATFORM == F_PLATFORM_ANDROID
    FPlatformType Engine::ms_ePlatform = F_Platform_Android;                  
#elif F_PLATFORM == F_PLATFORM_IOS
    FPlatformType Engine::ms_ePlatform = F_Platform_iOS;
#else 
    #pragma error "UnKnown platform, Engine.cpp Abort! Abort!"
#endif

    bool Engine::ms_bUseBinaryResource = false;
    String Engine::ms_strPathCfgEngine = "Assets/Config/Cfg_Engine.xml";
	String Engine::ms_strPathCfgPlugin = "Assets/Config/Cfg_Plugin.xml";

    Engine::Engine()
        : Base("Engine")
        , m_pSystemCapabilities(nullptr)
        , m_pPathManager(nullptr)
        , m_pFileManager(nullptr)
        , m_pCodecManager(nullptr)
        , m_pPluginManager(nullptr)
        , m_pRenderEngine(nullptr)

        , m_pTimer(nullptr)
        , m_pEngineFrameProfiler(nullptr)
        , m_nFrameCurrent(0)
        , m_nFrameCount(0)
        , m_nTimeLast(0)
        , m_bRenderAutoStop(false)

        , m_pathWorkFolder("")
        , m_folder_Plugin("")
        , m_bEngineIsInit(false)
        , m_bCodecIsInit(false)
    {

    }
    
    Engine::~Engine()
    {
        Destroy();
    }

    void Engine::Destroy()
    {
        RemoveEngineListenerAll();
        if (m_pRenderEngine)
        {
            m_pRenderEngine->Destroy();
        }
        if (m_pPluginManager != nullptr)
        {
            m_pPluginManager->UnloadPlugins();
        }
        F_DELETE(m_pPluginManager)
        F_DELETE(m_pRenderEngine)

        F_DELETE(m_pTimer)
        if (m_bCodecIsInit)
		{
            ImageCodecDXT::Shutdown();
			m_bCodecIsInit = false;
		}
        F_DELETE(m_pCodecManager)
        F_DELETE(m_pFileManager)
		F_DELETE(m_pPathManager)
        F_DELETE(m_pSystemCapabilities)
		m_bEngineIsInit = false;
        F_LogInfo("Engine::Destroy: Engine destroy success !");
    }

    bool Engine::Init(const String& strWorkFolder, bool bAutoCreateWindow)
    {
        m_strNamePlatform = F_GetPlatformTypeName(ms_ePlatform);
        m_pathWorkFolder = strWorkFolder;
		m_folder_Plugin = strWorkFolder + "Assets/Plugins/" + m_strNamePlatform + "/";
		initCommonCfgItems();

        //1, PreCommon 
        {
            //<1> SystemCapabilities
			m_pSystemCapabilities = new SystemCapabilities;
			m_pSystemCapabilities->Init();

            //<2> PathManager
			m_pPathManager = new FPathManager;
			if (!m_pPathManager->Init(strWorkFolder))
			{
                F_LogError("*********************** Engine::Init: PathManager init failed, path: [%s] !", strWorkFolder.c_str());
				return false;
			}
            F_LogInfo("Engine::Init: [1] FPathManager init success !");

            //<3> FileManager
			m_pFileManager  = new FFileManager;

            //<4> CodecManager
            m_pCodecManager = new FCodecManager;
            ImageCodecDXT::Startup();
            m_bCodecIsInit = true;

            //<5> ControllerManager
        }

        //2, All Engines
        {
            //<1> RenderEngine
            m_pRenderEngine = new RenderEngine;

            //<2> AudioEngine

            //<3> PhysicsEngine

            //<4> InputEngine

            //<5> Load Engine Config
            if (!loadEngineConfig(ms_strPathCfgEngine))
			{
				F_LogError("*********************** Engine::Init: Load engine config file: [%s] failed !", ms_strPathCfgEngine.c_str());
				return false;
			}
            F_LogInfo("Engine::Init: [2] loadEngineConfig success !");
        }

        //3, Plugin
        {
            m_pPluginManager = new FPluginManager;
			if (!m_pPluginManager->LoadPlugins(ms_strPathCfgPlugin, m_folder_Plugin))
			{
                F_LogError("*********************** Engine::Init: Load plugins failed, path config: [%s], path plugins: [%s] !", ms_strPathCfgPlugin.c_str(), m_folder_Plugin.c_str());
				return false;
			}
            F_LogInfo("Engine::Init: [3] LoadPlugins success !");
        }

        //4, Initialize 
        {
            //<1> render engine init
			if (!m_pRenderEngine->Init(bAutoCreateWindow))
			{
                F_LogError("*********************** Engine::Init: RenderEngine init failed !");
				return false;
			}
            F_LogInfo("Engine::Init: [4] RenderEngine init success !");
        }

        //5, PostCommon
        {
            m_pTimer = new FTimer();
        }

        m_pTimer->Reset();
		resetStatistics();
        m_bEngineIsInit = true;

        F_LogInfo("Engine::Init: Engine init success !");
		return true;
    }   
    void Engine::initCommonCfgItems()
	{
		m_mapEngineCommonCfgItem.clear();

		//<1> multi thread
        {
            ConfigItem cfgItem;
            cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Common_MultiThread);
            String strDefault("1");
            cfgItem.aPossibleValues.push_back(strDefault);
            cfgItem.strCurValue = strDefault;
            cfgItem.bImmutable	= false;
            m_mapEngineCommonCfgItem[cfgItem.strName] = cfgItem;
        }
		


	}
    bool Engine::loadEngineConfig(const String& strPath)
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

    void Engine::updateStats()
    {

    }

    void Engine::resetStatistics()
    {

    }

}; //LostPeterEngine