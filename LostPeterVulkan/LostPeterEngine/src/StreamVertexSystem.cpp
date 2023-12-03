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
    StreamVertexSystem::StreamVertexSystem(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage)
		: StreamVertex(nVertexSize, nVertexNum, eStreamUsage, true, false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
    }

	StreamVertexSystem::StreamVertexSystem(size_t nSizeInBytes, EStreamUsageType eStreamUsage)
		: StreamVertex(nSizeInBytes,eStreamUsage,true,false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
	}

	StreamVertexSystem::~StreamVertexSystem()
	{
        F_DELETE_T(m_pData)
	}

	void* StreamVertexSystem::Lock(size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
	{
		m_bIsLocked = true;
		return m_pData + nOffset;
	}

	void StreamVertexSystem::Unlock()
	{
		m_bIsLocked = false;
	}

	void StreamVertexSystem::ReadData(size_t nOffset, size_t nLength, void* pDest)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamVertexSystem::ReadData !")
		memcpy(pDest, m_pData + nOffset, nLength);
	}

	void StreamVertexSystem::WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream /*= false*/)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamVertexSystem::WriteData !")
		memcpy(m_pData + nOffset, pSource, nLength);
	}

	void* StreamVertexSystem::lockImpl(size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
	{		
		return m_pData + nOffset;
	}

	void StreamVertexSystem::unlockImpl()
	{
	
	}

}; //LostPeterEngine