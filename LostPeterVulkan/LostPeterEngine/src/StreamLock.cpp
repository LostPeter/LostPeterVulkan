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

#include "../include/StreamLock.h"
#include "../include/Stream.h"

namespace LostPeterEngine
{
    StreamLock::StreamLock() 
        : m_pStream(nullptr)
        , m_pData(nullptr) 
    {

    }
    
    StreamLock::StreamLock(Stream* pStream, EStreamLockType eStreamLock)
        : m_pStream(nullptr)
        , m_pData(nullptr)
    { 
        Lock(pStream, eStreamLock);
    }
    
    StreamLock::StreamLock(Stream* pStream, size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
        : m_pStream(nullptr)
        , m_pData(nullptr) 
    { 
        Lock(pStream, nOffset, nLength, eStreamLock); 
    }
    
    StreamLock::~StreamLock() 
    { 
        Unlock(); 
    }

    void StreamLock::Lock(Stream* pStream, EStreamLockType eStreamLock)
    {
        F_Assert(pStream && "StreamLock::Lock !")
        Unlock();
        m_pStream = pStream;
        m_pData = m_pStream->Lock(eStreamLock);
    }
    
    void StreamLock::Lock(Stream* pStream, size_t nOffset, size_t nLength, EStreamLockType eStreamLock)
    {
        F_Assert(pStream && "StreamLock::Lock !")
        Unlock();
        m_pStream = pStream;
        m_pData = m_pStream->Lock(nOffset, nLength, eStreamLock);
    }

    void StreamLock::Unlock()
    {
        if (m_pStream)
        {
            m_pStream->Unlock();
            m_pStream = nullptr;
            m_pData = nullptr;
        }   
    }

}; //LostPeterEngine