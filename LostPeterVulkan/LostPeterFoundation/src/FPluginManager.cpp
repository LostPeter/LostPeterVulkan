/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPluginManager.h"
#include "../include/FDynamicLibManager.h"
#include "../include/FFileXMLTinyConfig.h"

#include "tinyxml.h"
#include "tinystr.h"

namespace LostPeterFoundation
{
    typedef void (*DLL_START_PLUGIN)();
	typedef void (*DLL_STOP_PLUGIN)();

	template<> FPluginManager* FSingleton<FPluginManager>::ms_Singleton = nullptr;
	FPluginManager* FPluginManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}

	FPluginManager& FPluginManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "FPluginManager::GetSingleton")
		return (*ms_Singleton);  
	}

	FPluginManager::FPluginManager()
		: m_strFolderPlugin("")
	{
		m_pDynamicLibManager = FDynamicLibManager::GetSingletonPtr();
		if (m_pDynamicLibManager == nullptr)
		{
			m_pDynamicLibManager = new FDynamicLibManager;
		}
	}

	FPluginManager::~FPluginManager()
	{
        delete m_pDynamicLibManager;
	}
	
	bool FPluginManager::LoadPlugins(const String& strPluginsCfgPath, const String& strPluginsFolder)
	{
		FFileXMLTinyConfig xmlCfg;
		if (!xmlCfg.Load(strPluginsCfgPath.c_str()))
		{
			F_LogError("*********************** FPluginManager::LoadPlugins: Load plugin cfg file: [%s] failed !", strPluginsCfgPath.c_str());
			return false;
		}
		m_strFolderPlugin = strPluginsFolder;

		//1>, get all plugin names
		StringVector aPluginNames;
		TiXmlElement* pRoot = xmlCfg.GetRoot();
		TiXmlElement *pElement = pRoot->FirstChildElement();
		while (pElement)
		{
			String strPluginName;
			xmlCfg.GetString(pElement, "name", strPluginName);

#if LP_PLATFORM == LP_PLATFORM_WIN32
		#if LP_DEBUG == 1
			strPluginName += "_d.dll";
		#else
			strPluginName += ".dll";
		#endif 

#elif LP_PLATFORM == LP_PLATFORM_LINUX || LP_PLATFORM == LP_PLATFORM_ANDROID
		#if LP_DEBUG == 1
			strPluginName += "_d.so";
		#else
			strPluginName += ".so";
		#endif
			strPluginName = "lib" + strPluginName;
#elif LP_PLATFORM == LP_PLATFORM_MAC || LP_PLATFORM == LP_PLATFORM_IOS
		#if LP_DEBUG == 1
			strPluginName += "_d.dylib";
		#else
			strPluginName += ".dylib";
		#endif
			strPluginName = "lib" + strPluginName;
#else 
	#pragma error "UnKnown platform, FPluginManager.cpp Abort! Abort!"
#endif
			aPluginNames.push_back(strPluginName);
		
			pElement = pElement->NextSiblingElement();
		}
		
		//2, load all plugins
		for (StringVector::iterator it = aPluginNames.begin();
			 it != aPluginNames.end(); ++it)
		{
			String& strPluginName = (*it);
			String strPluginPath = strPluginsFolder + strPluginName;
			F_LogInfo("State: [%d], plugin name: [%s], plugin path: [%s]", LP_DEBUG, strPluginName.c_str(), strPluginPath.c_str());
			if (!LoadPlugin(strPluginName, strPluginPath))
				return false;
		}

		return true;
	}

	bool FPluginManager::UnloadPlugins()
	{
		FPluginPtrMap mapPlugins = m_mapPlugin;
		for (FPluginPtrMap::iterator it = mapPlugins.begin();
			 it != mapPlugins.end(); ++it)
		{
			UnloadPlugin(it->first);
		}
		m_mapPlugin.clear();

		return true;
	}

	bool FPluginManager::LoadPlugin(const String& strPluginName, const String& strPluginPath)
	{
		FPluginPtrMap::iterator itFind = m_mapPlugin.find(strPluginName);
		if (itFind != m_mapPlugin.end())
			return true;
		
		FDynamicLib* pDynlib = m_pDynamicLibManager->LoadDynamicLib(strPluginName, strPluginPath);
		if (!pDynlib)
		{
			F_LogError("*********************** FPluginManager::LoadPlugin: Load plugin [%s] failed !", strPluginName.c_str());
			return false;
		}

		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)pDynlib->GetSymbol("dllStartPlugin");
		if (!pFunc)
		{
			F_LogError("*********************** FPluginManager::LoadPlugin: Cannot find symbol 'dllStartPlugin' in library [%s] !", strPluginName.c_str());
			return false;
		}

		// must call InstallPlugin
		pFunc();
		
		F_LogInfo("FPluginManager::LoadPlugin: Load plugin [%s] success !", strPluginName.c_str());
		return true;
	}

	bool FPluginManager::UnloadPlugin(const String& strPluginName)
	{
		FPluginPtrMap::iterator itFind = m_mapPlugin.find(strPluginName);
		if (itFind == m_mapPlugin.end())
			return true;
		
		String strDynLibName(strPluginName);
#if LP_PLATFORM == LP_PLATFORM_WIN32
	#if LP_DEBUG == 1
		strDynLibName += "_d.dll";
	#else
		strDynLibName += ".dll";
	#endif 
#elif LP_PLATFORM == LP_PLATFORM_LINUX || LP_PLATFORM == LP_PLATFORM_ANDROID
	#if LP_DEBUG == 1
		strDynLibName += "_d.so";
	#else
		strDynLibName += ".so";
	#endif 
	strDynLibName = "lib" + strDynLibName;
#elif LP_PLATFORM == LP_PLATFORM_MAC || LP_PLATFORM == LP_PLATFORM_IOS
	#if LP_DEBUG == 1
		strDynLibName += "_d.dylib";
	#else
		strDynLibName += ".dylib";
	#endif
		strDynLibName = "lib" + strDynLibName;
#else 
	#pragma error "UnKnown platform, FPluginManager.cpp Abort! Abort!"
#endif

		FDynamicLib* pLib = m_pDynamicLibManager->GetDynamicLib(strDynLibName);
		if (!pLib)
			return true;

		DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)pLib->GetSymbol("dllStopPlugin");
		if (!pFunc)
		{
			F_LogError("*********************** FPluginManager::UnloadPlugin: Cannot find symbol 'dllStopPlugin' in library [%s] !", strDynLibName.c_str());
			return false;
		}

		// must call UninstallPlugin
		pFunc();

		bool bRet = m_pDynamicLibManager->UnloadDynamicLib(pLib);
		if (bRet)
		{
			F_LogInfo("FPluginManager::UnloadPlugin: Unload plugin [%s] success !", strPluginName.c_str());
		}
		return bRet;
	}

	bool FPluginManager::InstallPlugin(FPlugin* pPlugin)
	{
		const String& namePlugin = pPlugin->GetName();
		F_LogInfo("FPluginManager::InstallPlugin: Installing plugin: [%s] ...", namePlugin.c_str());
		{
			m_mapPlugin[namePlugin] = pPlugin;
			pPlugin->Install();
			pPlugin->Initialize();
		}
		F_LogInfo("FPluginManager::InstallPlugin: Install plugin: [%s] success !", namePlugin.c_str());
		return true;
	}

	bool FPluginManager::UninstallPlugin(FPlugin* pPlugin)
	{
		const String& namePlugin = pPlugin->GetName();
		F_LogInfo("FPluginManager::UninstallPlugin: Uninstalling plugin: [%s] ...", namePlugin.c_str());
		{
			FPluginPtrMap::iterator itFind = m_mapPlugin.find(namePlugin);
			if (itFind != m_mapPlugin.end())
				m_mapPlugin.erase(itFind);
			pPlugin->Shutdown();
			pPlugin->Uninstall();
		}
		F_LogInfo("FPluginManager::UninstallPlugin: Uninstall plugin: [%s] success !", namePlugin.c_str());
		return true;
	}	
    
}; //LostPeterFoundation