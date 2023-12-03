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

#ifndef _STREAM_H_
#define _STREAM_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport Stream
	{
	public:
		Stream(EStreamUsageType eStreamUsage, bool bSystemMemory, bool bUseShadowStream);
		virtual ~Stream();

	public:
	protected:
		size_t m_nStreamSizeInBytes;					
		EStreamUsageType m_eStreamUsage;	
		bool m_bIsLocked;					
		size_t m_nLockStart;					
		size_t m_nLockSize;					
		bool m_bSystemMemory;				
		bool m_bUseShadowStream;
		Stream* m_pStreamShadow;
		bool m_bShadowUpdated;	
		bool m_bSuppressUpdate;

		uint8* m_pResourceBuf;
	
	public:
		E_FORCEINLINE size_t GetStreamSizeInBytes() const { return m_nStreamSizeInBytes; }
		E_FORCEINLINE EStreamUsageType GetStreamUsageType() const { return m_eStreamUsage; }
		E_FORCEINLINE bool IsSystemMemory() const { return m_bSystemMemory; }
		E_FORCEINLINE bool HasShadowStream() const { return m_bUseShadowStream; }
		E_FORCEINLINE bool IsLocked() const 
		{ 
			return m_bIsLocked || (m_bUseShadowStream && m_pStreamShadow->IsLocked()); 
		}

	public:
		virtual void* Lock(size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
		virtual void* Lock(EStreamLockType eStreamLock);

		virtual void Unlock();

		virtual void CreateResourceBuf();

		virtual void ReadData(size_t nOffset, size_t nLength, void* pDest)	= 0;
		virtual void WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream = false) = 0;

		virtual void CopyData(Stream& streamSrc, size_t nSrcOffset, size_t nDstOffset, size_t nLength, bool bDiscardWholeStream = false);
		virtual void CopyData(Stream& streamSrc);
		virtual void UpdateFromShadow();

		void SuppressHardwareUpdate(bool bSuppress);

	protected:
		virtual void* lockImpl(size_t nOffset, size_t nLength, EStreamLockType eStreamLock) = 0;
		virtual void unlockImpl() = 0;
	};

}; //LostPeterEngine

#endif