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

    uint32 VertexDeclarationManager::ms_nVertexDeclarationIncrementCount = 10;
    VertexDeclarationManager::VertexDeclarationManager()
        : Base("VertexDeclarationManager")
        , m_nCount(0)
    {

    }

    VertexDeclarationManager::~VertexDeclarationManager()
    {
		Destroy();
    }

	void VertexDeclarationManager::Destroy()
	{
		DestroyVertexDeclarationAll();
	}

	bool VertexDeclarationManager::Init()
	{
		if (!createVertexDeclarationDefault())
		{
			F_LogError("*********************** VertexDeclarationManager::Init: Failed to init VertexDeclaration Default !");
            return false;
		}
		return true;
	}
		bool VertexDeclarationManager::createVertexDeclarationDefault()
		{
			m_mapVertexDeclarationDefault.clear();

			//0:	F_MeshVertex_Pos2Color4
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float2);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 2, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos2Color4] = pVertexDeclaration;
			}
			//1:	F_MeshVertex_Pos3Color4
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4] = pVertexDeclaration;
			}
			//2:	F_MeshVertex_Pos3Normal3
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Normal3] = pVertexDeclaration;
			}
			//3:	F_MeshVertex_Pos3Normal3Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 6, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Normal3Tex2] = pVertexDeclaration;
			}
			//4:	F_MeshVertex_Pos2Color4Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float2);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 2, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 6, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos2Color4Tex2] = pVertexDeclaration;
			}
			//5:	F_MeshVertex_Pos3Color4Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Tex2] = pVertexDeclaration;
			}
			//6:	F_MeshVertex_Pos3Color4Normal3Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(3, sizeof(float) * 10, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Normal3Tex2] = pVertexDeclaration;
			}
			//7:	F_MeshVertex_Pos3Color4Normal3Tex4
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(3, sizeof(float) * 10, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float4);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Normal3Tex4] = pVertexDeclaration;
			}
			//8:	F_MeshVertex_Pos3Color4Normal3Tangent3Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(3, sizeof(float) * 10, F_VertexElementSemantic_Tangent, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(4, sizeof(float) * 13, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Normal3Tangent3Tex2] = pVertexDeclaration;
			}
			//9:	F_MeshVertex_Pos3Color4Normal3Tangent3Tex4
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(3, sizeof(float) * 10, F_VertexElementSemantic_Tangent, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(4, sizeof(float) * 13, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float4);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Normal3Tangent3Tex4] = pVertexDeclaration;
			}
			//10:	F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 6, F_VertexElementSemantic_Tangent, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(3, sizeof(float) * 9, F_VertexElementSemantic_BlendWeights, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(4, sizeof(float) * 13, F_VertexElementSemantic_BlendIndices, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(5, sizeof(float) * 17, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Normal3Tangent3BlendWI8Tex2] = pVertexDeclaration;
			}
			//11:	F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2
			{
				VertexDeclaration* pVertexDeclaration = CreateVertexDeclaration();
				pVertexDeclaration->AddVertexElement(0, 0, F_VertexElementSemantic_Position, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(1, sizeof(float) * 3, F_VertexElementSemantic_Diffuse, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(2, sizeof(float) * 7, F_VertexElementSemantic_Normal, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(4, sizeof(float) * 10, F_VertexElementSemantic_Tangent, F_VertexElementData_Float3);
				pVertexDeclaration->AddVertexElement(5, sizeof(float) * 13, F_VertexElementSemantic_BlendWeights, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(6, sizeof(float) * 17, F_VertexElementSemantic_BlendIndices, F_VertexElementData_Float4);
				pVertexDeclaration->AddVertexElement(7, sizeof(float) * 21, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float2);
				pVertexDeclaration = GetSameVertexDeclaration(pVertexDeclaration);
				F_Assert(pVertexDeclaration && "VertexDeclarationManager::createVertexDeclarationDefault")
				m_mapVertexDeclarationDefault[F_MeshVertex_Pos3Color4Normal3Tangent3BlendWI8Tex2] = pVertexDeclaration;
			}

			return true;
		}

	VertexDeclaration* VertexDeclarationManager::GetVertexDeclaration(FMeshVertexType eMeshVertex)
	{
		MeshVertexType2VertexDeclarationPtrMap::iterator itFind = m_mapVertexDeclarationDefault.find(eMeshVertex);
		F_Assert(itFind != m_mapVertexDeclarationDefault.end() && "VertexDeclarationManager::GetVertexDeclaration")
		return itFind->second;
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