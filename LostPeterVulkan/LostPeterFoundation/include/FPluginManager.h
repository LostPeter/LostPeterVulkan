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

#ifndef _F_PLUGIN_MANAGER_H_
#define _F_PLUGIN_MANAGER_H_

#include "FPreDefine.h"
#include "FSingleton.h"
#include "FPlugin.h"

namespace LostPeterFoundation
{
    class foundationExport FPluginManager : public FSingleton<FPluginManager>
	{
	public:
		FPluginManager();
		~FPluginManager();
	
	public:
	private:
		FDynamicLibManager* m_pDynamicLibManager;
		FPluginPtrMap m_mapPlugin;
		String m_strFolderPlugin;

	public:
		static FPluginManager& GetSingleton();
		static FPluginManager* GetSingletonPtr();

	public:
		bool LoadPlugins(const String& strPluginsCfgPath, const String& strPluginsFolder);
		bool UnloadPlugins();

		bool LoadPlugin(const String& strPluginName, const String& strPluginPath);
		bool UnloadPlugin(const String& strPluginName);

		bool InstallPlugin(FPlugin* pPlugin);
		bool UninstallPlugin(FPlugin* pPlugin);
	};

}; //LostPeterFoundation

#endif