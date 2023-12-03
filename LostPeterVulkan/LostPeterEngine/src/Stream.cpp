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

#include "../include/Stream.h"

namespace LostPeterEngine
{
    Stream::Stream(EStreamUsageType eStreamUsage, bool bSystemMemory, bool bUseShadowStream)
        : m_eStreamUsage(eStreamUsage)
        , m_bIsLocked(false)
        , m_bSystemMemory(bSystemMemory)
        , m_bUseShadowStream(bUseShadowStream)
        , m_pStreamShadow(nullptr)
        , m_bShadowUpdated(false)
        , m_bSuppressUpdate(false)
        , m_pResourceBuf(nullptr)
    {
        if (bUseShadowStream && eStreamUsage == E_StreamUsage_Dynamic)
        {
            m_eStreamUsage = E_StreamUsage_WriteOnly;
        }
        else if (bUseShadowStream && eStreamUsage == E_StreamUsage_Static)
        {
            m_eStreamUsage = E_StreamUsage_StaticWriteOnly;
        }
    }

    Stream::~Stream()
    {
        F_DELETE_T(m_pResourceBuf)
    }

    void* Stream::Lock(size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
    {
        F_Assert(!IsLocked() && "Stream::Lock: Cannot lock this stream, it is already locked !")
        void* pRet = nullptr;
        if (m_bUseShadowStream)
        {
            if (E_StreamLock_ReadOnly != eStreamLock)
                m_bShadowUpdated = true;

            pRet = m_pStreamShadow->Lock(nOffset, nLength, eStreamLock);
        }
        else
        {
            pRet = lockImpl(nOffset, nLength, eStreamLock);
            m_bIsLocked = true;
        }

        m_nLockStart = nOffset;
        m_nLockSize = nLength;
        return pRet;
    }

    void* Stream::Lock(EStreamLockType eStreamLock)
    {
        return this->Lock(0, m_nStreamSizeInBytes, eStreamLock);
    }

    void Stream::Unlock()
    {
        F_Assert(IsLocked() && "Stream::Lock: Cannot unlock this stream, it is not locked !")

        if (m_bUseShadowStream && m_pStreamShadow->IsLocked())
        {
            m_pStreamShadow->Unlock();
            UpdateFromShadow();
        }
        else
        {
            unlockImpl();
            m_bIsLocked = false;
        }
    }

    void Stream::CreateResourceBuf()
    {
        F_DELETE_T(m_pResourceBuf)
		m_pResourceBuf = new uint8[m_nStreamSizeInBytes];
    }

    void Stream::CopyData(Stream& streamSrc, size_t nSrcOffset, size_t nDstOffset, size_t nLength, bool bDiscardWholeStream /*= false*/)
    {
        const void* pDataSrc = streamSrc.Lock(nSrcOffset, nLength, E_StreamLock_ReadOnly);
        this->WriteData(nDstOffset, nLength, pDataSrc, bDiscardWholeStream);
        streamSrc.Unlock();
    }

    void Stream::CopyData(Stream& streamSrc)
    {
        size_t size = FMath::Min(GetStreamSizeInBytes(), streamSrc.GetStreamSizeInBytes()); 
        CopyData(streamSrc, 0, 0, size, true);
    }

    void Stream::UpdateFromShadow()
    {
        if (m_bUseShadowStream && m_bShadowUpdated && !m_bSuppressUpdate)
        {
            const void* pDataSrc = m_pStreamShadow->lockImpl(m_nLockStart, m_nLockSize, E_StreamLock_ReadOnly);
            
            EStreamLockType eStreamLock;
            if (m_nLockStart == 0 && m_nLockSize == m_nStreamSizeInBytes)
                eStreamLock = E_StreamLock_Discard;
            else
                eStreamLock = E_StreamLock_Normal;

            void* pDataDst = this->lockImpl(m_nLockStart, m_nLockSize, eStreamLock);
            memcpy(pDataDst, pDataSrc, m_nLockSize);
            this->unlockImpl();
            m_pStreamShadow->unlockImpl();
            m_bShadowUpdated = false;
        }
    }

    void Stream::SuppressHardwareUpdate(bool bSuppress) 
    {
        m_bSuppressUpdate = bSuppress;
        if (!bSuppress)
        {
            UpdateFromShadow();
        }
    }

}; //LostPeterEngine