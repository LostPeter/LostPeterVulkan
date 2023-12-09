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
        StreamTexture(size_t nWidth, size_t nHeight, size_t nDepth,
			          FPixelFormatType ePixelFormat, EStreamUsageType eStreamUsage, 
			          bool bSystemMemory, bool bUseShadowStream);
        virtual ~StreamTexture();

    public:
    protected:
		size_t m_nWidth;
		size_t m_nHeight;
		size_t m_nDepth;

		size_t m_nRowPitch;		
		size_t m_nSlicePitch;		

		FPixelFormatType m_ePixelFormat;		
		FPixelBox m_CurrentLock;

	public:
		F_FORCEINLINE size_t GetWidth() const { return m_nWidth; }
		F_FORCEINLINE size_t GetHeight() const { return m_nHeight; }
		F_FORCEINLINE size_t GetDepth() const { return m_nDepth; }

		F_FORCEINLINE FPixelFormatType GetPixelFormat() const { return m_ePixelFormat; }
		
		const FPixelBox& GetCurrentLock();

	public:
		virtual void* Lock(size_t nOffset, size_t nLength, EStreamLockType eLockType);
		virtual const FPixelBox& Lock(const FBox& lockBox, EStreamLockType eLockType);

		virtual void ReadData(size_t nOffset, size_t nLength, void* pDest);
		virtual void WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream = false);

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

		virtual RenderTexture* GetRenderTarget(size_t nSlice = 0);
		virtual void ClearSliceRTT(size_t zoffset);

	protected:
		virtual void* lockImpl(size_t nOffset, size_t nLength, EStreamLockType eLockType);
		virtual FPixelBox lockImpl(const FBox lockBox, EStreamLockType eLockType) = 0;
    };

}; //LostPeterEngine

#endif