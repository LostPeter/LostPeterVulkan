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

#ifndef _STREAM_LOCK_H_
#define _STREAM_LOCK_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport StreamLock
    {
    public:
        StreamLock();
        StreamLock(Stream* pStream, EStreamLockType eStreamLock);
        StreamLock(Stream* pStream, size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
        ~StreamLock();

    public:
        Stream* m_pStream;
        void* m_pData;
        
    public:
        void Lock(Stream* pStream, EStreamLockType eStreamLock);
        void Lock(Stream* pStream, size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
        void Unlock();
    };

}; //LostPeterEngine

#endif