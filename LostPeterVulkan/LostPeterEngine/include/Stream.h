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

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Stream : public Base
							  , public FNonCopyable
	{
	public:
		Stream(EStreamUsageType eStreamUsage, 
			   bool bIsUseSystemMemory, 
			   bool bIsUseShadowStream);
		virtual ~Stream();

	public:
	protected:
		uint32 m_nStreamSizeInBytes;					
		EStreamUsageType m_eStreamUsage;	
		bool m_bIsLocked;					
		uint32 m_nLockStart;					
		uint32 m_nLockSize;					
		bool m_bIsUseSystemMemory;				
		bool m_bIsUseShadowStream;
		Stream* m_pStreamShadow;
		bool m_bIsShadowUpdated;	
		bool m_bIsSuppressUpdate;

		uint8* m_pResourceBuf;
	
	public:
		F_FORCEINLINE uint32 GetStreamSizeInBytes() const { return m_nStreamSizeInBytes; }
		F_FORCEINLINE EStreamUsageType GetStreamUsageType() const { return m_eStreamUsage; }
		F_FORCEINLINE bool IsUseSystemMemory() const { return m_bIsUseSystemMemory; }
		F_FORCEINLINE bool IsUseShadowStream() const { return m_bIsUseShadowStream; }
		F_FORCEINLINE bool IsLocked() const 
		{ 
			return m_bIsLocked || (m_bIsUseShadowStream && m_pStreamShadow->IsLocked()); 
		}

	public:
		virtual void* Lock(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock);
		virtual void* Lock(EStreamLockType eStreamLock);

		virtual void Unlock();

		virtual void CreateResourceBuf();

		virtual void ReadData(uint32 nOffset, uint32 nLength, void* pDest)	= 0;
		virtual void WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream = false) = 0;

		virtual void CopyData(Stream& streamSrc, uint32 nSrcOffset, uint32 nDstOffset, uint32 nLength, bool bIsDiscardWholeStream = false);
		virtual void CopyData(Stream& streamSrc);
		virtual void UpdateFromShadow();

		void SuppressHardwareUpdate(bool bSuppress);

	protected:
		virtual void* lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock) = 0;
		virtual void unlockImpl() = 0;
	};

}; //LostPeterEngine

#endif