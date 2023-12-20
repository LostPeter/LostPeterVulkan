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

#include "../include/VertexDeclaration.h"
#include "../include/VertexDeclarationManager.h"

namespace LostPeterEngine
{
    VertexDeclaration::VertexDeclaration()
		: m_nPoolIndex(-1)
		, m_bSharedVD(true)
	{

	}

	VertexDeclaration::~VertexDeclaration()
	{

	}

	void VertexDeclaration::GetVertexDeclKey(String& strKey)
	{
		VertexElementList::iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			VertexElement& ve = (*it);
			String strTemp;
			ve.GetElementKey(strTemp);
			strKey += strTemp;
			strKey += "_";
		}
	}

	const VertexElement* VertexDeclaration::GetVertexElement(uint16 nIndex)
	{
		F_Assert(nIndex >= 0 && nIndex < m_listElement.size() && "VertexDeclaration::GetVertexElement: Index out of bounds !")

		VertexElementList::iterator it = m_listElement.begin();
		for (uint16 n = 0; n < nIndex; ++n)
			++it;
		return &(*it);
	}

	bool VertexDeclaration::VertexElementLess(const VertexElement& e1, const VertexElement& e2)
	{
		if (e1.GetSourceIndex() < e2.GetSourceIndex())
		{
			return true;
		}
		else if (e1.GetSourceIndex() == e2.GetSourceIndex())
		{
			if (e1.GetVertexElementSemanticType() < e2.GetVertexElementSemanticType())
			{
				return true;
			}
			else if (e1.GetVertexElementSemanticType() == e2.GetVertexElementSemanticType())
			{
				if (e1.GetIndex() < e2.GetIndex())
				{
					return true;
				}
			}
		}
		return false;
	}

	void VertexDeclaration::Sort()
	{
		m_listElement.sort(VertexDeclaration::VertexElementLess);
	}
	
	void VertexDeclaration::Clear()
	{
		m_nPoolIndex = -1;
		RemoveVertexElementAll();
	}

	void VertexDeclaration::CloseGapsInSource()
	{
		if (m_listElement.empty())
			return;
		Sort();

		VertexElementList::iterator it,itEnd;
		itEnd = m_listElement.end();
		uint16 nTargetIndex = 0;
		uint16 nLastIndex = GetVertexElement(0)->GetSourceIndex();
		uint16 nCur = 0;
		for (it = m_listElement.begin(); it != itEnd; ++it,++nCur)
		{
			VertexElement& ve = *it;
			if (nLastIndex != ve.GetSourceIndex())
			{
				nTargetIndex++;
				nLastIndex = ve.GetSourceIndex();
			}
			if (nTargetIndex != ve.GetSourceIndex())
			{
				ModifyVertexElement(nCur, nTargetIndex, ve.GetOffset(), ve.GetVertexElementSemanticType(), ve.GetVertexElementDataType(), ve.GetIndex());
			}
		}
	}

	VertexDeclaration* VertexDeclaration::GetAutoOrganizedDeclaration(bool bSkeletalAnimation, bool bVertexAnimation)
	{
		VertexDeclaration* newDecl = this->Clone();
		const VertexElementList& elems = newDecl->GetVertexElementList();
		uint16 nCur = 0;
		VertexElementList::const_iterator it,itEnd;
		itEnd = elems.end();
		for (it = elems.begin(); it != itEnd; ++it,++nCur)
		{
			const VertexElement& elem = *it;
			newDecl->ModifyVertexElement(nCur, 0, 0, elem.GetVertexElementSemanticType(), elem.GetVertexElementDataType(), elem.GetIndex());
		}
		newDecl->Sort();
	
		size_t offset = 0;
		nCur = 0;
		uint16 buffer = 0;
		FVertexElementSemanticType prevSemantic = F_VertexElementSemantic_Position;
		itEnd = elems.end();
		for (it = elems.begin(); it != itEnd; ++it,++nCur)
		{
			const VertexElement& elem = *it;

			bool splitWithPrev = false;
			bool splitWithNext = false;
			switch ((int32)elem.GetVertexElementSemanticType())
			{
			case F_VertexElementSemantic_Position:
				splitWithPrev = bVertexAnimation;
				splitWithNext = bVertexAnimation;
				break;
			case F_VertexElementSemantic_Normal:
				splitWithPrev = (prevSemantic == F_VertexElementSemantic_BlendWeights || prevSemantic == F_VertexElementSemantic_BlendIndices);
				splitWithNext = (bSkeletalAnimation || bVertexAnimation);
				break;
			case F_VertexElementSemantic_BlendWeights:
				splitWithPrev = true;
				break;
			case F_VertexElementSemantic_BlendIndices:
				splitWithNext = true;
				break;
			}

			if (splitWithPrev && offset)
			{
				++buffer;
				offset = 0;
			}
			prevSemantic = elem.GetVertexElementSemanticType();
			newDecl->ModifyVertexElement(nCur, buffer, offset, elem.GetVertexElementSemanticType(), elem.GetVertexElementDataType(), elem.GetIndex());

			if (splitWithNext)
			{
				++buffer;
				offset = 0;
			}
			else
			{
				offset += elem.GetSize();
			}
		}
		return newDecl;
	}

	uint16 VertexDeclaration::GetMaxSource() const
	{
		uint16 nRet = 0;
		VertexElementList::const_iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			if (it->GetSourceIndex() > nRet)
			{
				nRet = it->GetSourceIndex();
			}
		}
		return nRet;
	}

	const VertexElement& VertexDeclaration::AddVertexElement(uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex /*= 0*/)
	{
		if (typeData == F_VertexElementData_Color)
		{
			typeData = VertexElement::GetBestColorVertexElementType();
		}
		m_listElement.push_back(VertexElement(nSourceIndex, nOffset, typeSemantic, typeData, nIndex));

		if (static_cast<size_t>(nSourceIndex + 1) > m_aVertexSize.size())
		{
			m_aVertexSize.resize(nSourceIndex + 1);
			m_aVertexSize[nSourceIndex] = VertexElement::GetTypeSize(typeData);
		}
		else
		{
			m_aVertexSize[nSourceIndex] += VertexElement::GetTypeSize(typeData);
		}

		return m_listElement.back();
	}

	const VertexElement& VertexDeclaration::InsertVertexElement(uint16 atPosition, uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex /*= 0*/)
	{
		if (atPosition >= m_listElement.size())
		{
			return AddVertexElement(nSourceIndex, nOffset, typeSemantic, typeData, nIndex);
		}
		VertexElementList::iterator it = m_listElement.begin();
		for (uint16 n = 0; n < atPosition; ++n)
			++it;
		it = m_listElement.insert(it,VertexElement(nSourceIndex, nOffset, typeSemantic, typeData, nIndex));
		
		if (static_cast<size_t>(nSourceIndex+1) > m_aVertexSize.size())
		{
			m_aVertexSize.resize(nSourceIndex+1);
			m_aVertexSize[nSourceIndex]  = VertexElement::GetTypeSize(typeData);
		}
		else
		{
			m_aVertexSize[nSourceIndex] += VertexElement::GetTypeSize(typeData);
		}

		return *it;
	}

	void VertexDeclaration::RemoveVertexElement(uint16 nIndex)
	{
		F_Assert(nIndex >= 0 && nIndex < m_listElement.size() && "VertexDeclaration::RemoveVertexElement: Index out of bounds !")
		VertexElementList::iterator it = m_listElement.begin();
		for (uint16 n = 0; n < nIndex; ++n)
			++it;

		m_aVertexSize[it->GetSourceIndex()] -= it->GetSize();
		m_listElement.erase(it);
	}

	void VertexDeclaration::RemoveVertexElement(FVertexElementSemanticType typeSemantic, uint16 nIndex /*= 0*/)
	{
		VertexElementList::const_iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			if (it->GetVertexElementSemanticType() == typeSemantic && it->GetIndex() == nIndex)
			{
				m_aVertexSize[it->GetSourceIndex()] -= it->GetSize();
				m_listElement.erase(it);
				break;
			}
		}
	}

	void VertexDeclaration::RemoveVertexElementAll()
	{	
		m_aVertexSize.clear();
		m_listElement.clear();
	}

	void VertexDeclaration::ModifyVertexElement(uint16 nElementIndex, uint16 nSourceIndex, size_t nOffset, FVertexElementSemanticType typeSemantic, FVertexElementDataType typeData, uint16 nIndex /*= 0*/)
	{
		F_Assert(nElementIndex < m_listElement.size() && "VertexDeclaration::ModifyVertexElement: Index out of bounds !")
		VertexElementList::iterator it = m_listElement.begin();
		std::advance(it, nElementIndex);

		m_aVertexSize[it->GetSourceIndex()] -= it->GetSize();

		(*it) = VertexElement(nSourceIndex, nOffset, typeSemantic, typeData, nIndex);

		if (static_cast<size_t>(nSourceIndex+1) > m_aVertexSize.size())
		{
			m_aVertexSize.resize(nSourceIndex+1);
			m_aVertexSize[nSourceIndex] = VertexElement::GetTypeSize(typeData);
		}
		else
		{
			m_aVertexSize[nSourceIndex] += VertexElement::GetTypeSize(typeData);
		}
	}

	const VertexElement* VertexDeclaration::FindVertexElementBySemantic(FVertexElementSemanticType typeSemantic, uint16 nIndex /*= 0*/)
	{
		VertexElementList::const_iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			if (it->GetVertexElementSemanticType() == typeSemantic && it->GetIndex() == nIndex)
			{
				return &(*it);
			}
		}
		return 0;
	}

	VertexElementList VertexDeclaration::FindVertexElementsBySource(uint16 nSourceIndex)
	{
		VertexElementList retList;
		VertexElementList::const_iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			if (it->GetSourceIndex() == nSourceIndex)
			{
				retList.push_back(*it);
			}
		}
		return retList;
	}

	size_t VertexDeclaration::GetVertexSize(uint16 nSourceIndex)
	{
		return m_aVertexSize[nSourceIndex];
	}

	VertexDeclaration* VertexDeclaration::Clone()
	{
		VertexDeclaration* pVD = VertexDeclarationManager::GetSingleton().CreateVertexDeclaration();
		VertexElementList::const_iterator it,itEnd;
		itEnd = m_listElement.end();
		for (it = m_listElement.begin(); it != itEnd; ++it)
		{
			pVD->AddVertexElement(it->GetSourceIndex(), it->GetOffset(), it->GetVertexElementSemanticType(), it->GetVertexElementDataType(), it->GetIndex());
		}
		return pVD;
	}

}; //LostPeterEngine