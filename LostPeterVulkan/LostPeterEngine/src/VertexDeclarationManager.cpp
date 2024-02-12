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

    uint32 VertexDeclarationManager::ms_nVertexDeclarationIncrementCount = 5;
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
		VertexDeclarationPtrMap::iterator itFind = m_mapVertexDeclaration.find(strKey);
		if (itFind == m_mapVertexDeclaration.end())
		{
			m_mapVertexDeclaration[strKey] = pVertexDeclaration;
			m_nCount = (uint32)m_mapVertexDeclaration.size();
			pVertexDeclaration->SetPoolIndex(m_nCount);
		}
		else
		{
			pRet = itFind->second;
			pRet->AddRef();
			DestroyVertexDeclaration(pVertexDeclaration);
		}
		return pRet;
	}

	VertexDeclaration* VertexDeclarationManager::CreateVertexDeclaration()
	{
		//ENGINE_LOCK_MUTEX(m_mutexVertexDecl)
		VertexDeclaration* pVertexDeclaration = nullptr;
		if (m_listVertexDeclarationFree.empty())
		{
			for (uint32 i = 0; i < ms_nVertexDeclarationIncrementCount; i++)
			{
				VertexDeclaration* p = createVertexDeclarationImpl();
				F_Assert(p && "VertexDeclarationManager::CreateVertexDeclaration")
				m_aVertexDeclarationPool.push_back(p);
				m_listVertexDeclarationFree.push_back(p);
			}
		}
		pVertexDeclaration = m_listVertexDeclarationFree.front();
		m_listVertexDeclarationFree.pop_front();
		pVertexDeclaration->AddRef();
		return pVertexDeclaration;
	}

	void VertexDeclarationManager::DestroyVertexDeclaration(VertexDeclaration* pVertexDeclaration)
	{
		pVertexDeclaration->DelRef();
		if (pVertexDeclaration->HasRef())
			return;
		//ENGINE_LOCK_MUTEX(m_mutexVertexDecl)
		pVertexDeclaration->ResetRef();
		pVertexDeclaration->Clear();
		m_listVertexDeclarationFree.push_back(pVertexDeclaration);
	}
	void VertexDeclarationManager::DestroyVertexDeclarationAll()
	{
		VertexDeclarationPtrVector::iterator it,itEnd;
		itEnd = m_aVertexDeclarationPool.end();
		for (it = m_aVertexDeclarationPool.begin(); it != itEnd; ++it)
		{
			destroyVertexDeclarationImpl(*it);
		}
		m_aVertexDeclarationPool.clear();
		m_listVertexDeclarationFree.clear();
		m_mapVertexDeclaration.clear();
	}	

	VertexDeclaration* VertexDeclarationManager::createVertexDeclarationImpl()
	{
		return new VertexDeclaration;
	}
	void VertexDeclarationManager::destroyVertexDeclarationImpl(VertexDeclaration* pVertexDeclaration)
	{
		F_DELETE(pVertexDeclaration)
	}

}; //LostPeterEngine