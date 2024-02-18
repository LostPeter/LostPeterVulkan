/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/StreamUniform.h"
#include "../include/StreamUniformSystem.h"

namespace LostPeterEngine
{
    StreamUniform::StreamUniform(uint32 nUniformSize, 
							     uint32 nUniformCount, 
							     EStreamUsageType eStreamUsage,
		                         bool bIsUseSystemMemory, 
							     bool bIsUseShadowStream)
		: Stream(eStreamUsage, 
				 bIsUseSystemMemory, 
				 bIsUseShadowStream)
		, m_nStreamUniformSize(nUniformSize)
		, m_nStreamUniformCount(nUniformCount)
	{
		m_nStreamSizeInBytes = m_nStreamUniformSize * m_nStreamUniformCount;
		if (m_bIsUseShadowStream)
		{
			m_pStreamShadow = new StreamUniformSystem(m_nStreamUniformSize, m_nStreamUniformCount, E_StreamUsage_Dynamic);
		}
	}	

	StreamUniform::StreamUniform(uint32 nSizeInBytes, 
							     EStreamUsageType eStreamUsage,
		                         bool bIsUseSystemMemory, 
							     bool bIsUseShadowStream)
		: Stream(eStreamUsage, 
				 bIsUseSystemMemory, 
				 bIsUseShadowStream)
		, m_nStreamUniformSize(0)
		, m_nStreamUniformCount(1)
	{
		m_nStreamSizeInBytes = nSizeInBytes;
		if (m_bIsUseShadowStream)
		{
			m_pStreamShadow = new StreamUniformSystem(m_nStreamUniformSize, m_nStreamUniformCount, E_StreamUsage_Dynamic);
		}
	}

	StreamUniform::~StreamUniform()
	{
		F_DELETE(m_pStreamShadow)
	}

	void StreamUniform::SetStreamUniformSize(uint32 nUniformSize)
	{
		m_nStreamUniformSize = nUniformSize;
	}

}; //LostPeterEngine