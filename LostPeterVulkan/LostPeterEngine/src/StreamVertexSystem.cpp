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

#include "../include/StreamVertexSystem.h"

namespace LostPeterEngine
{
    StreamVertexSystem::StreamVertexSystem(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage)
		: StreamVertex(nVertexSize, nVertexCount, eStreamUsage, true, false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
    }

	StreamVertexSystem::StreamVertexSystem(uint32 nSizeInBytes, EStreamUsageType eStreamUsage)
		: StreamVertex(nSizeInBytes,eStreamUsage,true,false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
	}

	StreamVertexSystem::~StreamVertexSystem()
	{
        F_DELETE_T(m_pData)
	}

	void* StreamVertexSystem::Lock(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{
		m_bIsLocked = true;
		return m_pData + nOffset;
	}

	void StreamVertexSystem::Unlock()
	{
		m_bIsLocked = false;
	}

	void StreamVertexSystem::ReadData(uint32 nOffset, uint32 nLength, void* pDest)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamVertexSystem::ReadData !")
		memcpy(pDest, m_pData + nOffset, nLength);
	}

	void StreamVertexSystem::WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream /*= false*/)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamVertexSystem::WriteData !")
		memcpy(m_pData + nOffset, pSource, nLength);
	}

	void* StreamVertexSystem::lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{		
		return m_pData + nOffset;
	}

	void StreamVertexSystem::unlockImpl()
	{
	
	}

}; //LostPeterEngine