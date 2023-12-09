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

#ifndef _F_DYNAMIC_LIB_MANAGER_H_
#define _F_DYNAMIC_LIB_MANAGER_H_

#include "FPreDefine.h"
#include "FSingleton.h"
#include "FDynamicLib.h"

namespace LostPeterFoundation
{
    class foundationExport FDynamicLibManager : public FSingleton<FDynamicLibManager>
	{
	public:
		FDynamicLibManager();
		~FDynamicLibManager();
    
    public:
	private:
		FDynamicLibPtrMap m_mapDynamicLib;

	public:
		static FDynamicLibManager& GetSingleton();
		static FDynamicLibManager* GetSingletonPtr();
		
		FDynamicLib* GetDynamicLib(const String& strFileName);

	public:
		FDynamicLib* LoadDynamicLib(const String& strFileName, const String& strFilePath);
		bool UnloadDynamicLib(FDynamicLib* pLib);
		void UnloadDynamicLibAll();
	};

}; //LostPeterFoundation

#endif