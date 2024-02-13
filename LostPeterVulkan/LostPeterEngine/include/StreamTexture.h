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

#ifndef _STREAM_TEXTURE_H_
#define _STREAM_TEXTURE_H_

#include "Stream.h"

namespace LostPeterEngine
{
    class engineExport StreamTexture : public Stream
    {
    public:
        StreamTexture(uint32 nWidth, 
					  uint32 nHeight, 
					  uint32 nDepth,
			          FPixelFormatType ePixelFormat, 
					  EStreamUsageType eStreamUsage, 
			          bool bIsUseSystemMemory, 
					  bool bIsUseShadowStream);
        virtual ~StreamTexture();

    public:
    protected:
		uint32 m_nWidth;
		uint32 m_nHeight;
		uint32 m_nDepth;

		uint32 m_nRowPitch;		
		uint32 m_nSlicePitch;		

		FPixelFormatType m_ePixelFormat;		
		FPixelBox m_CurrentLock;

	public:
		F_FORCEINLINE uint32 GetWidth() const { return m_nWidth; }
		F_FORCEINLINE uint32 GetHeight() const { return m_nHeight; }
		F_FORCEINLINE uint32 GetDepth() const { return m_nDepth; }

		F_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		
		const FPixelBox& GetCurrentLock();

	public:
		virtual void* Lock(uint32 nOffset, uint32 nLength, EStreamLockType eLockType);
		virtual const FPixelBox& Lock(const FBox& lockBox, EStreamLockType eLockType);

		virtual void ReadData(uint32 nOffset, uint32 nLength, void* pDest);
		virtual void WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream = false);

		virtual void Blit(StreamTexture* pSrc, const FBox& srcBox, const FBox& dstBox);
		void Blit(StreamTexture* pSrc);

		virtual void BlitFromMemory(const FPixelBox& src, const FBox& dstBox) = 0;
		void BlitFromMemory(const FPixelBox& src)
		{
			BlitFromMemory(src, FBox(0, 0, 0, m_nWidth, m_nHeight, m_nDepth));
		}

		virtual void BlitToMemory(const FBox& srcBox, const FPixelBox& dst) = 0;
		void BlitToMemory(const FPixelBox& dst)
		{
			BlitToMemory(FBox(0, 0, 0, m_nWidth, m_nHeight, m_nDepth), dst);
		}

		virtual RenderTexture* GetRenderTarget(uint32 nSlice = 0);
		virtual void ClearSliceRTT(uint32 zoffset);

	protected:
		virtual void* lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eLockType);
		virtual FPixelBox lockImpl(const FBox lockBox, EStreamLockType eLockType) = 0;
    };

}; //LostPeterEngine

#endif