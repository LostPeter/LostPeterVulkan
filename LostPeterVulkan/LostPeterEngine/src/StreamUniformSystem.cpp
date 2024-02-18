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

#include "../include/StreamUniformSystem.h"

namespace LostPeterEngine
{
    StreamUniformSystem::StreamUniformSystem(uint32 nUniformSize, uint32 nUniformCount, EStreamUsageType eStreamUsage)
		: StreamUniform(nUniformSize, nUniformCount, eStreamUsage, true, false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
    }

	StreamUniformSystem::StreamUniformSystem(uint32 nSizeInBytes, EStreamUsageType eStreamUsage)
		: StreamUniform(nSizeInBytes,eStreamUsage,true,false)
	{
        m_pData = new uint8[m_nStreamSizeInBytes];
	}

	StreamUniformSystem::~StreamUniformSystem()
	{
        F_DELETE_T(m_pData)
	}

	void* StreamUniformSystem::Lock(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{
		m_bIsLocked = true;
		return m_pData + nOffset;
	}

	void StreamUniformSystem::Unlock()
	{
		m_bIsLocked = false;
	}

	void StreamUniformSystem::ReadData(uint32 nOffset, uint32 nLength, void* pDest)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamUniformSystem::ReadData !")
		memcpy(pDest, m_pData + nOffset, nLength);
	}

	void StreamUniformSystem::WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream /*= false*/)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamUniformSystem::WriteData !")
		memcpy(m_pData + nOffset, pSource, nLength);
	}

	void* StreamUniformSystem::lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{		
		return m_pData + nOffset;
	}

	void StreamUniformSystem::unlockImpl()
	{
	
	}

}; //LostPeterEngine