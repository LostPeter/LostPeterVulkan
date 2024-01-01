/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FArchiveManager.h"
#include "../include/FArchive.h"

namespace LostPeterFoundation
{
    typedef void (*CreateFunc)(FArchive**, const String&);

	template<> FArchiveManager* FSingleton<FArchiveManager>::ms_Singleton = nullptr;
	FArchiveManager* FArchiveManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	FArchiveManager& FArchiveManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "FArchiveManager::GetSingleton")
		return (*ms_Singleton);  
	}


    FArchiveManager::FArchiveManager() 
    {
        
    }

	FArchiveManager::~FArchiveManager()
	{
        Destroy();
	}

    void FArchiveManager::Destroy()
    {
        for (FArchivePtrMap::iterator it = m_mapArchive.begin(); 
             it != m_mapArchive.end(); ++it)
		{
			FArchive* pArchive = it->second;
			pArchive->Unload();

			FArchiveFactoryPtrMap::iterator itFactory = m_mapArchiveFactory.find(pArchive->GetType());
			if (itFactory == m_mapArchiveFactory.end())
			{
                F_LogError("*********************** FArchiveManager::Destroy: Can not find an archive factory, nameFactory: [%s] !", pArchive->GetType().c_str());
				continue;
			}
            itFactory->second->DestroyInstance(pArchive);
		}
		m_mapArchive.clear();
    }


	FArchive* FArchiveManager::Load(const String& strFileName, const String& strArchiveType)
	{
		FArchivePtrMap::iterator itFind = m_mapArchive.find(strFileName);
		FArchive* pArchive = nullptr;
		if (itFind == m_mapArchive.end())
		{
			FArchiveFactoryPtrMap::iterator itFactory = m_mapArchiveFactory.find(strArchiveType);
			if (itFactory == m_mapArchiveFactory.end())
            {
                F_LogError("*********************** FArchiveManager::Load: Can not find an archive factory, nameFactory: [%s] !", strArchiveType.c_str());
                return nullptr;
            }
			pArchive = itFactory->second->CreateInstance(strFileName);
			pArchive->Load();
			m_mapArchive[strFileName] = pArchive;
		}
		else
		{
			pArchive = itFind->second;
		}   
		return pArchive;
	}

	void FArchiveManager::Unload(FArchive* pArchive)
	{
		Unload(pArchive->GetName());
	}

	void FArchiveManager::Unload(const String& strFileName)
	{
		FArchivePtrMap::iterator itFind = m_mapArchive.find(strFileName);
		if (itFind != m_mapArchive.end())
		{
			itFind->second->Unload();

			FArchiveFactoryPtrMap::iterator itFactory = m_mapArchiveFactory.find(itFind->second->GetType());
			if (itFactory == m_mapArchiveFactory.end())
			{
                F_LogError("*********************** FArchiveManager::Unload: Can not find an archive factory, nameFactory: [%s] !", itFind->second->GetType().c_str());
                return;
			}
			itFactory->second->DestroyInstance(itFind->second);
			m_mapArchive.erase(itFind);
		}
	}

	FArchivePtrMapIterator FArchiveManager::GetArchivePtrMapIterator()
	{
		return FArchivePtrMapIterator(m_mapArchive.begin(), m_mapArchive.end());
	}

	void FArchiveManager::AddArchiveFactory(FArchiveFactory* pArchiveFactory)
	{        
		m_mapArchiveFactory.insert(FArchiveFactoryPtrMap::value_type(pArchiveFactory->GetType(), pArchiveFactory));
        F_LogInfo("FArchiveManager::AddArchiveFactory: FArchiveFactory type: [%s] registered !", pArchiveFactory->GetType().c_str());
	}
    
}; //LostPeterFoundation