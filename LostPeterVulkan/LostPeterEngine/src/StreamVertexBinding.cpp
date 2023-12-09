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

#include "../include/StreamVertexBinding.h"
#include "../include/StreamManager.h"

namespace LostPeterEngine
{
     StreamVertexBinding::StreamVertexBinding()
		: m_nMaxIndex(0)
		, m_pCreator(nullptr)
	{

	}

	StreamVertexBinding::~StreamVertexBinding()
	{
		Destroy();
	}

	void StreamVertexBinding::Destroy()
	{
		StreamManager* pSM = StreamManager::GetSingletonPtr();
		StreamVertexPtrMap::iterator it,itEnd;
		itEnd = m_mapBinding.end();
		for (it = m_mapBinding.begin(); it != itEnd; ++it)
		{
			pSM->DestroyStreamVertex(it->second);
		}
		m_mapBinding.clear();
		m_nMaxIndex = 0;
	}

	void StreamVertexBinding::SetBinding(uint16 nIndex, StreamVertex* pVertexStream)
	{
		m_mapBinding[nIndex] = pVertexStream;
		m_nMaxIndex = FMath::Max(m_nMaxIndex, (uint16)(nIndex + 1));
	}

	StreamVertex* StreamVertexBinding::UnsetBinding(uint16 nIndex)
	{
		StreamVertexPtrMap::iterator itFind = m_mapBinding.find(nIndex);
		if (itFind == m_mapBinding.end())
		{
			F_LogError("*********************** StreamVertexBinding::UnsetBinding: Can not find buffer binding from index: [%d] !", nIndex);
			return nullptr;
		}
		StreamVertex* pRet = itFind->second;
		m_mapBinding.erase(itFind);
		return pRet;
	}

	StreamVertexPtrVector StreamVertexBinding::UnsetAllBindings()
	{
		StreamVertexPtrVector aRet;
		for (StreamVertexPtrMap::iterator it = m_mapBinding.begin();
			 it != m_mapBinding.end(); ++it)
		{
			aRet.push_back(it->second);
		}
		m_mapBinding.clear();
		m_nMaxIndex = 0;
		return aRet;
	}

	StreamVertex* StreamVertexBinding::GetVertexStream(uint16 nIndex) const
	{
		StreamVertexPtrMap::const_iterator itFind = m_mapBinding.find(nIndex);
		if (itFind == m_mapBinding.end())
		{
			F_LogError("*********************** StreamVertexBinding::GetVertexStream: Can not find buffer binding from index: [%d] !", nIndex);
			return nullptr;
		}
		return itFind->second;
	}

	bool StreamVertexBinding::IsStreamBound(uint16 nIndex) const
	{
		return m_mapBinding.find(nIndex) != m_mapBinding.end();
	}

	uint16 StreamVertexBinding::GetLastBoundIndex() const
	{
		return m_mapBinding.empty() ? 0 : m_mapBinding.rbegin()->first + 1;
	}

	bool StreamVertexBinding::HasGaps() const
	{
		if (m_mapBinding.empty())
			return false;
		if (m_mapBinding.rbegin()->first + 1 == (int)m_mapBinding.size())
			return false;
		return true;
	}

	void StreamVertexBinding::CloseGaps(BindingIndexMap& bindingIndexMap)
	{
		bindingIndexMap.clear();
		StreamVertexPtrMap newBindingMap;

		uint16 targetIndex = 0;
		for (StreamVertexPtrMap::const_iterator it = m_mapBinding.begin();
			 it != m_mapBinding.end(); ++it, ++targetIndex)
		{
			bindingIndexMap[it->first] = targetIndex;
			newBindingMap[targetIndex] = it->second;
		}
		m_mapBinding.swap(newBindingMap);
		m_nMaxIndex = targetIndex;
	}

}; //LostPeterEngine