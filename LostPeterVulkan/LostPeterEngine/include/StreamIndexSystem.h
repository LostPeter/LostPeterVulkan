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

#ifndef _STREAM_INDEX_SYSTEM_H_
#define _STREAM_INDEX_SYSTEM_H_

#include "StreamIndex.h"

namespace LostPeterEngine
{
    class engineExport StreamIndexSystem : public StreamIndex
    {
    public:
		StreamIndexSystem(EStreamIndexType eStreamIndex, size_t nIndexNum, EStreamUsageType eStreamUsage);
		virtual ~StreamIndexSystem();

	protected:
		uint8* m_pData;

	public:
		virtual void* Lock(size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
		virtual void Unlock();

		virtual void ReadData(size_t nOffset, size_t nLength, void* pDest);
		virtual void WriteData(size_t nOffset, size_t nLength, const void* pSource, bool bDiscardWholeStream = false);

	protected:
		virtual void* lockImpl(size_t nOffset, size_t nLength, EStreamLockType eStreamLock);
		virtual void unlockImpl();
    };

}; //LostPeterEngine

#endif