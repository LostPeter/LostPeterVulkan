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
    StreamVertex::StreamVertex(uint32 nVertexSize, 
							   uint32 nVertexCount, 
							   EStreamUsageType eStreamUsage,
		                       bool bIsUseSystemMemory, 
							   bool bIsUseShadowStream)
		: Stream(eStreamUsage, 
				 bIsUseSystemMemory, 
				 bIsUseShadowStream)
		, m_nStreamVertexSize(nVertexSize)
		, m_nStreamVertexCount(nVertexCount)
		, m_bSingleFormat(false)
	{
		m_nStreamSizeInBytes = m_nStreamVertexSize * m_nStreamVertexCount;
		if (m_bIsUseShadowStream)
		{
			m_pStreamShadow = new StreamVertexSystem(m_nStreamVertexSize, m_nStreamVertexCount, E_StreamUsage_Dynamic);
		}
	}	

	StreamVertex::StreamVertex(uint32 nSizeInBytes, 
							   EStreamUsageType eStreamUsage,
		                       bool bIsUseSystemMemory, 
							   bool bIsUseShadowStream)
		: Stream(eStreamUsage, 
				 bIsUseSystemMemory, 
				 bIsUseShadowStream)
		, m_nStreamVertexSize(0)
		, m_nStreamVertexCount(1)
		, m_bSingleFormat(true)
	{
		m_nStreamSizeInBytes = nSizeInBytes;
		if (m_bIsUseShadowStream)
		{
			m_pStreamShadow = new StreamVertexSystem(m_nStreamVertexSize, m_nStreamVertexCount, E_StreamUsage_Dynamic);
		}
	}

	StreamVertex::~StreamVertex()
	{
		F_DELETE(m_pStreamShadow)
	}

	void StreamVertex::SetStreamVertexSize(uint32 nVertexSize)
	{
		if (!m_bSingleFormat)
		{
			m_nStreamVertexSize = nVertexSize;
		}
	}

}; //LostPeterEngine