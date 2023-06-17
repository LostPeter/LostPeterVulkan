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

#include "../include/FDynamicLibManager.h"

namespace LostPeterFoundation
{
     template<> FDynamicLibManager* FSingleton<FDynamicLibManager>::ms_Singleton = nullptr;
	FDynamicLibManager* FDynamicLibManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}

	FDynamicLibManager& FDynamicLibManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "FDynamicLibManager::GetSingleton")
		return (*ms_Singleton);  
	}

	FDynamicLibManager::FDynamicLibManager()
	{

	}

	FDynamicLibManager::~FDynamicLibManager()
	{
		UnloadDynamicLibAll();
	}	

	FDynamicLib* FDynamicLibManager::GetDynamicLib(const String& strFileName)
	{
		FDynamicLibPtrMap::iterator itFind = m_mapDynamicLib.find(strFileName);
		if (itFind != m_mapDynamicLib.end())
			return itFind->second;
		return nullptr;
	}

	FDynamicLib* FDynamicLibManager::LoadDynamicLib(const String& strFileName, const String& strFilePath)
	{
		FDynamicLib* pLib = GetDynamicLib(strFileName);
		if (pLib)
			return pLib;
		pLib = new FDynamicLib(strFileName, strFilePath);
		if (!pLib->Load())
			return nullptr;

		m_mapDynamicLib[strFileName] = pLib;
		return pLib;
	}

	bool FDynamicLibManager::UnloadDynamicLib(FDynamicLib* pLib)
	{
		FDynamicLibPtrMap::iterator itFind = m_mapDynamicLib.find(pLib->GetName());
		if (itFind != m_mapDynamicLib.end())
			m_mapDynamicLib.erase(itFind);
		bool bRet = pLib->Unload();
		delete pLib;

		return bRet;
	}
	
	void FDynamicLibManager::UnloadDynamicLibAll()
	{
		for (FDynamicLibPtrMap::iterator it = m_mapDynamicLib.begin(); 
			 it != m_mapDynamicLib.end(); ++it)
		{
			it->second->Unload();
			delete it->second;
		}
		m_mapDynamicLib.clear();
	}
    
}; //LostPeterFoundation