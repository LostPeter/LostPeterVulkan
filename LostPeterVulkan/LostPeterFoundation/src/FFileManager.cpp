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

#include "../include/FFileManager.h"
#include "../include/FFileIO.h"
#include "../include/FFileMemory.h"

namespace LostPeterFoundation
{
    FFileMemoryPtr::FFileMemoryPtr()
		: m_pFileMemory(nullptr)
	{
		
	}

	FFileMemoryPtr::FFileMemoryPtr(FFileMemory* pFM)
		: m_pFileMemory(pFM)
	{
		F_Assert(m_pFileMemory && "FFileMemoryPtr::FFileMemoryPtr")
	}

	FFileMemoryPtr::~FFileMemoryPtr()
	{
		if (m_pFileMemory)
		{
			FFileManager::GetSingleton().DeleteFileMemory(m_pFileMemory);
			m_pFileMemory = 0;
		}
	}

	uint8* FFileMemoryPtr::GetBuffer()
	{
		if (!m_pFileMemory)
			return nullptr;
		return m_pFileMemory->GetBuffer();
	}

	uint32 FFileMemoryPtr::GetSize()
	{
		if (!m_pFileMemory)
			return 0;
		return (uint32)m_pFileMemory->Size();
	} 


    template<> FFileManager* LostPeterFoundation::FSingleton<FFileManager>::ms_Singleton = nullptr;
    FFileManager* FFileManager::GetSingletonPtr()
    {
        return ms_Singleton;
    }
    FFileManager& FFileManager::GetSingleton()
    {  
        F_Assert(ms_Singleton && "FFileManager::GetSingleton")
        return (*ms_Singleton);  
    }

    FFileManager::FFileManager()
    {

    }

    FFileManager::~FFileManager()
    {
        Destroy();
    }

    void FFileManager::Destroy()
	{
		//file io
		for (FFileIOPtrMap::iterator it = m_mapFileIO.begin();
			it != m_mapFileIO.end(); ++it)
		{
			delete it->first;
		}
		m_mapFileIO.clear();
			
		//file memory
		for (FFileMemoryPtrMap::iterator it = m_mapFileMemory.begin();
			it != m_mapFileMemory.end(); ++it)
		{
			delete it->first;
		}
		m_mapFileMemory.clear();
	}

	FFileIO* FFileManager::CreateFileIO()
	{
		FFileIO* pFIO = new FFileIO();
		m_mapFileIO[pFIO] = int(1);
		return pFIO;
	}

	void FFileManager::DeleteFileIO(FFileIO* pFIO)
	{
		FFileIOPtrMap::iterator itFind = m_mapFileIO.find(pFIO);
		if (itFind != m_mapFileIO.end())
			m_mapFileIO.erase(itFind);
		delete pFIO;
	}
	
	FFileMemory* FFileManager::CreateFileMemory(uint32 nSize)
	{
		uint8* pBuffer = new uint8[nSize];
		return CreateFileMemory(pBuffer, nSize);
	}

	FFileMemory* FFileManager::CreateFileMemory(uint8* pBuffer, uint32 nSize)
	{
		FFileMemory* pFM = new FFileMemory(pBuffer, nSize);
		m_mapFileMemory[pFM] = int(1);
		return pFM;
	}

	FFileMemory* FFileManager::CreateFileMemory(const String& strPath)
	{
		FFileIO file;
		if (!file.Open(strPath.c_str(), "rb"))
		{
			F_LogError("*********************** FFileManager::CreateFileMemory: Open file: [%s] failed !", strPath.c_str());
			return 0;
		}
		long nSize = file.Size();
		FFileMemory* pFM = CreateFileMemory(nSize);
		if (!pFM)
			return 0;
		file.Read(pFM->GetBuffer(), nSize, 1);
		file.Close();
		return pFM;
	}
	
	void FFileManager::DeleteFileMemory(FFileMemory* pFM)
	{
		FFileMemoryPtrMap::iterator itFind = m_mapFileMemory.find(pFM);
		if (itFind != m_mapFileMemory.end())
			m_mapFileMemory.erase(itFind);
		delete pFM;
	}
    
}; //LostPeterFoundation