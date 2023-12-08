/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VertexDeclarationManager.h"
#include "../include/VertexDeclaration.h"

template<> LostPeterEngine::VertexDeclarationManager* LostPeterFoundation::FSingleton<LostPeterEngine::VertexDeclarationManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    VertexDeclarationManager* VertexDeclarationManager::GetSingletonPtr()
    {
        return ms_Singleton;
    }
    VertexDeclarationManager& VertexDeclarationManager::GetSingleton()
    {  
        F_Assert(ms_Singleton && "VertexDeclarationManager::GetSingleton")
        return (*ms_Singleton);  
    }

    size_t VertexDeclarationManager::ms_nVDIncrementNum = 5;
    VertexDeclarationManager::VertexDeclarationManager()
        : Base("VertexDeclarationManager")
        , m_nCount(0)
    {

    }

    VertexDeclarationManager::~VertexDeclarationManager()
    {
        DestroyVertexDeclarationAll();
    }

    VertexDeclaration* VertexDeclarationManager::GetSameVertexDeclaration(VertexDeclaration* pVertexDeclaration)
	{
		if (pVertexDeclaration->GetPoolIndex() > 0)
			return pVertexDeclaration;
		String strKey;
		pVertexDeclaration->GetVertexDeclKey(strKey);

		//ENGINE_LOCK_MUTEX(m_mutexVertexDecl)
		VertexDeclaration* pRet = pVertexDeclaration;
		VertexDeclarationPtrMap::iterator itFind = m_mapVD.find(strKey);
		if (itFind == m_mapVD.end())
		{
			m_mapVD[strKey] = pVertexDeclaration;
			m_nCount = (uint32)m_mapVD.size();
			pVertexDeclaration->SetPoolIndex(m_nCount);
		}
		else
		{
			pRet = itFind->second;
			//pRet->AddRef();
			DestroyVertexDeclaration(pVertexDeclaration);
		}
		return pRet;
	}

	VertexDeclaration* VertexDeclarationManager::CreateVertexDeclaration()
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexDecl)
		VertexDeclaration* pVertexDeclaration = nullptr;
		if (m_listVDFree.empty())
		{
			for (size_t i = 0; i < ms_nVDIncrementNum; i++)
			{
				VertexDeclaration* p = createVertexDeclarationImpl();
				F_Assert(p && "VertexDeclarationManager::CreateVertexDeclaration")
				m_aVDPool.push_back(p);
				m_listVDFree.push_back(p);
			}
		}
		if (!m_listVDFree.empty())
		{
			pVertexDeclaration = m_listVDFree.front();
			m_listVDFree.pop_front();
		}
		
		//pVertexDeclaration->AddRef();
		return pVertexDeclaration;
	}

	void VertexDeclarationManager::DestroyVertexDeclaration(VertexDeclaration* pVertexDeclaration)
	{
		//pVertexDeclaration->DelRef();
		//if(pVertexDeclaration->HasRef())
		//	return;
		//ENGINE_LOCK_MUTEX(m_mutexVertexDecl)
		//pVertexDeclaration->Reset();
		pVertexDeclaration->Clear();
		m_listVDFree.push_back(pVertexDeclaration);
	}

	void VertexDeclarationManager::DestroyVertexDeclarationAll()
	{
		VertexDeclarationPtrVector::iterator it,itEnd;
		itEnd = m_aVDPool.end();
		for (it = m_aVDPool.begin(); it != itEnd; ++it)
		{
			destroyVertexDeclarationImpl(*it);
		}
		m_aVDPool.clear();
		m_listVDFree.clear();
		m_mapVD.clear();
	}	

	VertexDeclaration* VertexDeclarationManager::createVertexDeclarationImpl()
	{
		return new VertexDeclaration;
	}

	void VertexDeclarationManager::destroyVertexDeclarationImpl(VertexDeclaration* pVertexDeclaration)
	{
		delete pVertexDeclaration;
	}

}; //LostPeterEngine