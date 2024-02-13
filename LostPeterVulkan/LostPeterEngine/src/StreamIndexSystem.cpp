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

#include "../include/StreamIndexSystem.h"

namespace LostPeterEngine
{
     StreamIndexSystem::StreamIndexSystem(EStreamIndexType eStreamIndex, 
	 									  uint32 nIndexCount, 
										  EStreamUsageType eStreamUsage)
		: StreamIndex(eStreamIndex, 
					  nIndexCount, 
					  eStreamUsage, 
					  true, 
					  false) 
	{
		m_pData = new uint8[m_nStreamSizeInBytes];
	}

	StreamIndexSystem::~StreamIndexSystem()
	{
		F_DELETE_T(m_pData)
	}

	void* StreamIndexSystem::Lock(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{
		m_bIsLocked = true;
		return m_pData + nOffset;
	}	

	void StreamIndexSystem::Unlock()
	{
		m_bIsLocked = false;
	}

	void StreamIndexSystem::ReadData(uint32 nOffset, uint32 nLength, void* pDest)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamIndexSystem::ReadData !")
		memcpy(pDest,m_pData + nOffset,nLength);
	}

	void StreamIndexSystem::WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream /*= false*/)
	{
		F_Assert((nOffset + nLength) <= m_nStreamSizeInBytes && "StreamIndexSystem::WriteData !")
		memcpy(m_pData + nOffset,pSource,nLength);
	}

	void* StreamIndexSystem::lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock)
	{
		return m_pData + nOffset;
	}

	void StreamIndexSystem::unlockImpl()
	{
        
	}

}; //LostPeterEngine