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

#include "../include/DataVertex.h"
#include "../include/VertexElement.h"
#include "../include/VertexDeclaration.h"
#include "../include/VertexDeclarationManager.h"
#include "../include/StreamVertexBinding.h"
#include "../include/StreamVertexBindingManager.h"
#include "../include/StreamVertex.h"
#include "../include/StreamManager.h"

namespace LostPeterEngine
{
    DataVertex::DataVertex()
		: m_nVertexStart(0)
		, m_nVertexCount(0)
		//, m_nAnimDataItemsUsed(0)
	{
		m_pVertexDeclaration = VertexDeclarationManager::GetSingleton().CreateVertexDeclaration();
		m_pVertexStreamBinding = StreamVertexBindingManager::GetSingleton().CreateVertexStreamBinding();
		m_bDel_VertexDeclaration = true;
		m_bDel_VertexStreamBinding = true;
	}

	DataVertex::DataVertex(VertexDeclaration* pDecl, bool bDelVD, StreamVertexBinding* pBinding, bool bDelVB)
		: m_pVertexDeclaration(pDecl)
		, m_bDel_VertexDeclaration(bDelVD)
		, m_pVertexStreamBinding(pBinding)
		, m_bDel_VertexStreamBinding(bDelVB)
		, m_nVertexStart(0)
		, m_nVertexCount(0)
		//, m_nAnimDataItemsUsed(0)
	{
		F_Assert(m_pVertexDeclaration && m_pVertexStreamBinding && "DataVertex::DataVertex")
	}

	DataVertex::~DataVertex()
	{
		Destroy();
	}

	void DataVertex::Destroy()
	{
		if (m_bDel_VertexDeclaration)
		{
			if (m_pVertexDeclaration)
			{
				VertexDeclarationManager::GetSingleton().DestroyVertexDeclaration(m_pVertexDeclaration);
				m_pVertexDeclaration = nullptr;
			}
			m_bDel_VertexDeclaration = false;
		}
		if (m_bDel_VertexStreamBinding)
		{
			if (m_pVertexStreamBinding)
			{
				StreamVertexBindingManager::GetSingleton().DestroyVertexStreamBinding(m_pVertexStreamBinding);
				m_pVertexStreamBinding = nullptr;
			}
			m_bDel_VertexStreamBinding = false;
		}
	}

	DataVertex* DataVertex::Clone(bool bCopyData /*= true*/, bool bSharedVD /*= false*/) const
	{
		DataVertex* pDest = new DataVertex;
		pDest->SetVertexStart(m_nVertexStart);
		pDest->SetVertexCount(m_nVertexCount);

		//1> copy vertex buffers in turn
		const StreamVertexPtrMap& bindings = m_pVertexStreamBinding->GetBindings();
		for (StreamVertexPtrMap::const_iterator it = bindings.begin(); 
			it != bindings.end(); ++it)
		{
			StreamVertex* pSrcStream = it->second;
			StreamVertex* pDstStream = StreamManager::GetSingleton().CreateStreamVertex(pSrcStream->GetStreamVertexSize(),
									                                                    pSrcStream->GetStreamVertexNum(),
                                                                                        pSrcStream->GetStreamUsageType(),
                                                                                        pSrcStream->HasShadowStream());
			if (bCopyData)
			{
				pDstStream->CopyData(*pSrcStream, 0, 0, pSrcStream->GetStreamSizeInBytes(), true);
			}
			pDest->GetVertexStreamBinding()->SetBinding(it->first, pDstStream);
		}

		//2> copy elements
		const VertexElementList& listElements = m_pVertexDeclaration->GetVertexElementList();
		for (VertexElementList::const_iterator it = listElements.begin(); 
			 it != listElements.end();++it)
		{
			pDest->GetVertexDeclaration()->AddVertexElement(it->GetSourceIndex(),
                                                            it->GetOffset(),
                                                            it->GetVertexElementSemanticType(),
                                                            it->GetVertexElementDataType(),
                                                            it->GetIndex());
		}
		
		// //3> anim data
		// pDest->m_aAnimationData = m_aAnimationData;
		// pDest->m_nAnimDataItemsUsed = m_nAnimDataItemsUsed;
		
		if(bSharedVD)
			pDest->UpdateVertexDeclarationPointer();
		else
			pDest->GetVertexDeclaration()->SetSharedVD(false);
		return pDest;
	}	

	bool DataVertex::UpdateVertexDeclarationPointer()
	{
		m_pVertexDeclaration = VertexDeclarationManager::GetSingleton().GetSameVertexDeclaration(m_pVertexDeclaration);
		F_Assert(m_pVertexDeclaration && "DataVertex::UpdateVertexDeclarationPointer")
		return m_pVertexDeclaration?true:false;
	}
	
	void DataVertex::CloseGapsInBindings()
	{
		if (!m_pVertexStreamBinding->HasGaps())
			return;

		// Check for error first
		const VertexElementList& listElements = m_pVertexDeclaration->GetVertexElementList();
		VertexElementList::const_iterator it,itEnd;
		itEnd = listElements.end();
		for (it = listElements.begin(); it != itEnd; ++it)
		{
			const VertexElement& elem = *it;
			if (!m_pVertexStreamBinding->IsStreamBound(elem.GetSourceIndex()))
			{
				F_LogError("*********************** DataVertex::CloseGapsInBindings: No stream is bound to that element source !");
				return;
			}
		}

		// Close gaps in the vertex buffer bindings
		StreamVertexBinding::BindingIndexMap bindingIndexMap;
		m_pVertexStreamBinding->CloseGaps(bindingIndexMap);

		// Modify vertex elements to reference to new buffer index
		uint16 elemIndex = 0;
		for (it = listElements.begin(); it != itEnd; ++it,++elemIndex)
		{
			const VertexElement& elem = *it;
			StreamVertexBinding::BindingIndexMap::const_iterator itFind = bindingIndexMap.find(elem.GetSourceIndex());
			F_Assert(itFind != bindingIndexMap.end() && "DataVertex::CloseGapsInBindings");
			uint16 nTargetSource = itFind->second;
			if (elem.GetSourceIndex() != nTargetSource)
			{
				m_pVertexDeclaration->ModifyVertexElement(elemIndex,
                                                          nTargetSource,
                                                          elem.GetOffset(),
                                                          elem.GetVertexElementSemanticType(),
                                                          elem.GetVertexElementDataType(),
                                                          elem.GetIndex());
			}
		}
	}

	// void DataVertex::AllocateAnimationElements(uint16 nCount)
	// {
	// 	uint16 nTexCoord = 0;
	// 	const VertexElementList& listElements = m_pVertexDeclaration->GetVertexElementList();
	// 	VertexElementList::const_iterator it,itEnd;
	// 	itEnd = listElements.end();
	// 	for (it = listElements.begin(); it != itEnd; ++it) 
	// 	{
	// 		const VertexElement& ele = *it;
	// 		if (ele.GetVertexElementSemanticType() == F_VertexElementSemantic_TextureCoordinates)
	// 		{
	// 			++nTexCoord;
	// 		}
	// 	}
	// 	F_Assert(nTexCoord <= C_CONFIG_MAX_TEXTURE_COORD_SETS_COUNT && "DataVertex::AllocateAnimationElements")

	// 	// for (size_t i = m_aAnimationData.size(); i < nCount; ++i)
	// 	// {
	// 	// 	AnimationData data;
	// 	// 	data.pTargetVertexElement = &(m_pVertexDeclaration->AddVertexElement(m_pVertexStreamBinding->GetNextIndex(), 0, F_VertexElementSemantic_TextureCoordinates, F_VertexElementData_Float3, nTexCoord++));
	// 	// 	m_aAnimationData.push_back(data);
	// 	// }
	// }

}; //LostPeterEngine