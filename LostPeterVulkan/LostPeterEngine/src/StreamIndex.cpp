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

#include "../include/StreamIndex.h"
#include "../include/StreamIndexSystem.h"

namespace LostPeterEngine
{
    StreamIndex::StreamIndex(EStreamIndexType eStreamIndex, size_t nIndexNum, 
                             EStreamUsageType eStreamUsage, bool bUseSystemMemory, bool bUseShadowStream)
		: Stream(eStreamUsage, bUseSystemMemory, bUseShadowStream)
		, m_eStreamIndex(eStreamIndex)
		, m_eStreamIndexNum(nIndexNum)
	{
		switch ((int32)m_eStreamIndex)
		{
		case E_StreamIndex_16Bit:
			m_eStreamIndexTypeSize = sizeof(uint16);
			break;
		case E_StreamIndex_32Bit:
			m_eStreamIndexTypeSize = sizeof(uint32);
			break;
		}
		m_nStreamSizeInBytes = m_eStreamIndexTypeSize * m_eStreamIndexNum;

		if (m_bUseShadowStream)
		{
			m_pStreamShadow = new StreamIndexSystem(m_eStreamIndex, m_eStreamIndexNum, E_StreamUsage_Dynamic);
		}
	}

	StreamIndex::~StreamIndex()
	{
		F_DELETE(m_pStreamShadow)
	}

}; //LostPeterEngine