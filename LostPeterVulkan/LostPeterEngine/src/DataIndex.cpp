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

#include "../include/DataIndex.h"
#include "../include/StreamIndex.h"
#include "../include/StreamManager.h"

namespace LostPeterEngine
{
    DataIndex::DataIndex()
		: m_pStreamIndex(nullptr)
		, m_bIsDelete(true)
		, m_nIndexStart(0)
		, m_nIndexCount(0)
		, m_nBatchCount(0)
		//, m_pBatchInstance(nullptr)
	{

	}

	DataIndex::DataIndex(StreamIndex* pStreamIndex, uint32 nStart, uint32 nCount, bool bDel)
		: m_pStreamIndex(pStreamIndex)
		, m_bIsDelete(bDel)
		, m_nIndexStart(nStart)
		, m_nIndexCount(nCount)
		, m_nBatchCount(0)
		//, m_pBatchInstance(nullptr)
	{
		F_Assert(m_pStreamIndex && m_nIndexStart >= 0 && m_nIndexCount > 0 && "DataIndex::DataIndex")
	}

	DataIndex::~DataIndex()
	{
		Destroy();
	}

	void DataIndex::Destroy()
	{
		if (m_bIsDelete)
		{
			if (m_pStreamIndex)
			{
				StreamManager::GetSingleton().DestroyStreamIndex(m_pStreamIndex);
				m_pStreamIndex = nullptr;
			}
			m_bIsDelete = false;
		}
	}

	DataIndex* DataIndex::Clone(bool bIsCopyData /*= true*/) const
	{
		StreamIndex* pStreamIndex = StreamManager::GetSingleton().CreateStreamIndex(m_pStreamIndex->GetStreamIndexType(),
				                                                                    m_pStreamIndex->GetStreamIndexCount(), 
                                                                                    m_pStreamIndex->GetStreamUsageType(), 
                                                                                    m_pStreamIndex->IsUseShadowStream());
		if (bIsCopyData)
		{
			pStreamIndex->CopyData(*m_pStreamIndex, 0, 0, m_pStreamIndex->GetStreamSizeInBytes(), true);
		}
		DataIndex* pDest = new DataIndex(pStreamIndex, m_nIndexStart, m_nIndexCount, true);
		return pDest;
	}

}; //LostPeterEngine