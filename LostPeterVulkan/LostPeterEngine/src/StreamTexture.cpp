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

#include "../include/StreamTexture.h"
#include "../include/RenderTexture.h"
#include "../include/Image.h"

namespace LostPeterEngine
{
    StreamTexture::StreamTexture(size_t nWidth, size_t nHeight, size_t nDepth,
								 FPixelFormatType ePixelFormat, EStreamUsageType eStreamUsage, 
								 bool bSystemMemory, bool bUseShadowStream)
		: Stream(eStreamUsage, bSystemMemory, bUseShadowStream)
		, m_nWidth(nWidth)
		, m_nHeight(nHeight)
		, m_nDepth(nDepth)
		, m_ePixelFormat(ePixelFormat)
	{
		m_nRowPitch	= m_nWidth;
		m_nSlicePitch = m_nHeight * m_nWidth;
		m_nStreamSizeInBytes = m_nHeight * m_nWidth * FPixelFormat::GetPixelFormatElemBytes(m_ePixelFormat);
	}

	StreamTexture::~StreamTexture()
	{

	}

	const FPixelBox& StreamTexture::GetCurrentLock()
	{
		F_Assert(IsLocked() && "StreamTexture::GetCurrentLock: Stream not locked !")

		return m_CurrentLock; 
	}
	
	void* StreamTexture::Lock(size_t nOffset, size_t nLength, EStreamLockType eLockType)
	{
		F_Assert(!IsLocked() && "StreamTexture::Lock: Cannot lock this stream, it is already locked !")
		F_Assert(nOffset == 0 && nLength == m_nStreamSizeInBytes && "StreamTexture::Lock: Cannot lock memory region, most lock box or entire buffer !")

		FBox myBox(0, 0, 0, m_nWidth, m_nHeight, m_nDepth);
		const FPixelBox& rv = Lock(myBox, eLockType);
		return rv.m_pData;
	}

	const FPixelBox& StreamTexture::Lock(const FBox& lockBox, EStreamLockType eLockType)
	{
		if (m_bUseShadowStream)
		{
			if (eLockType != E_StreamLock_ReadOnly)
			{
				m_bShadowUpdated = true;
			}
			m_CurrentLock = static_cast<StreamTexture*>(m_pStreamShadow)->Lock(lockBox,eLockType);
		}
		else
		{
			m_CurrentLock = lockImpl(lockBox, eLockType);
			m_bIsLocked = true;
		}

		return m_CurrentLock;
	}

	void StreamTexture::ReadData(size_t nOffset, size_t nLength, void* pDest)
	{
		F_Assert(false && "StreamTexture::ReadData: Reading a byte range is not implemented. Use BlitToMemory !")
	}

	void StreamTexture::WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream /*= false*/)
	{	
		F_Assert(false && "StreamTexture::WriteData: Writing a byte range is not implemented. Use BlitFromMemory !")
	}

	void StreamTexture::Blit(StreamTexture* pSrc, const FBox& srcBox, const FBox& dstBox)
	{
		if (IsLocked() || pSrc->IsLocked())
		{
			F_Assert(false && "StreamTexture::Blit: Source or destination stream may not be locked !")
			return;
		}
		if (pSrc == this)
		{
			F_Assert(false && "StreamTexture::Blit: Source must not be the same object !")
			return;
		}
		const FPixelBox& srclock = pSrc->Lock(srcBox, E_StreamLock_ReadOnly);

		EStreamLockType eLockType = E_StreamLock_Normal;
		if (dstBox.m_nLeft == 0 && dstBox.m_nTop == 0 && dstBox.m_nFront == 0 &&
			dstBox.m_nRight == m_nWidth && dstBox.m_nBottom == m_nHeight &&
			dstBox.m_nBack == m_nDepth)
        {
            eLockType = E_StreamLock_Discard;
        }

		const FPixelBox &dstlock = Lock(dstBox, eLockType);
		if (dstlock.GetWidth() != srclock.GetWidth() ||
			dstlock.GetHeight() != srclock.GetHeight() ||
			dstlock.GetDepth() != srclock.GetDepth())
		{
			//Image::Scale(srclock, dstlock);
		}
		else
		{
			FPixelFormat::BulkPixelConversion(srclock, dstlock);
		}

		Unlock();
		pSrc->Unlock();
	}

	void StreamTexture::Blit(StreamTexture* pSrc)
	{
		Blit(pSrc, FBox(0, 0, 0, pSrc->GetWidth(), pSrc->GetHeight(), pSrc->GetDepth()), FBox(0, 0, 0, m_nWidth, m_nHeight, m_nDepth));
	}
	
	RenderTexture* StreamTexture::GetRenderTarget(size_t nSlice /*= 0*/)
	{
		F_Assert(false && "StreamTexture::GetRenderTarget: Not yet implemented !")
		return 0;
	}

	void StreamTexture::ClearSliceRTT(size_t zoffset)
	{
		
	}

	void* StreamTexture::lockImpl(size_t nOffset, size_t nLength, EStreamLockType eLockType)
	{
		F_Assert(false && "StreamTexture::lockImpl: must sub class called !")
		return 0;
	}

}; //LostPeterEngine