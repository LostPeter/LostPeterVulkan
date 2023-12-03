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

#include "../include/StreamVertex.h"
#include "../include/StreamVertexSystem.h"

namespace LostPeterEngine
{
    StreamVertex::StreamVertex(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage,
		                       bool bSystemMemory, bool bUseShadowStream)
		: Stream(eStreamUsage, bSystemMemory, bUseShadowStream)
		, m_nStreamVertexSize(nVertexSize)
		, m_nStreamVertexNum(nVertexNum)
		, m_bSingleFormat(false)
	{
		m_nStreamSizeInBytes = m_nStreamVertexSize * m_nStreamVertexNum;
		if (m_bUseShadowStream)
		{
			m_pStreamShadow = new StreamVertexSystem(m_nStreamVertexSize, m_nStreamVertexNum, E_StreamUsage_Dynamic);
		}
	}	

	StreamVertex::StreamVertex(size_t nSizeInBytes, EStreamUsageType eStreamUsage,
		                       bool bSystemMemory, bool bUseShadowStream)
		: Stream(eStreamUsage, bSystemMemory, bUseShadowStream)
		, m_nStreamVertexSize(0)
		, m_nStreamVertexNum(1)
		, m_bSingleFormat(true)
	{
		m_nStreamSizeInBytes = nSizeInBytes;
		if (m_bUseShadowStream)
		{
			m_pStreamShadow = new StreamVertexSystem(m_nStreamVertexSize, m_nStreamVertexNum, E_StreamUsage_Dynamic);
		}
	}

	StreamVertex::~StreamVertex()
	{
		F_DELETE(m_pStreamShadow)
	}

	void StreamVertex::SetStreamVertexSize(size_t nVertexSize)
	{
		if (!m_bSingleFormat)
		{
			m_nStreamVertexSize = nVertexSize;
		}
	}

}; //LostPeterEngine