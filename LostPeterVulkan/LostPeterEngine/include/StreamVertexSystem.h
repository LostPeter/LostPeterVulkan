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

#ifndef _STREAM_VERTEX_SYSTEM_H_
#define _STREAM_VERTEX_SYSTEM_H_

#include "StreamVertex.h"

namespace LostPeterEngine
{
    class engineExport StreamVertexSystem : public StreamVertex 
	{
	public:
		StreamVertexSystem(uint32 nVertexSize, uint32 nVertexCount, EStreamUsageType eStreamUsage);
		StreamVertexSystem(uint32 nSizeInBytes, EStreamUsageType eStreamUsage);
		virtual ~StreamVertexSystem();

	protected:
		uint8* m_pData;

	public:
		virtual void* Lock(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock);
		virtual void Unlock();

		virtual void ReadData(uint32 nOffset, uint32 nLength, void* pDest);
		virtual void WriteData(uint32 nOffset, uint32 nLength, const void* pSource, bool bIsDiscardWholeStream = false);

	protected:
		virtual void* lockImpl(uint32 nOffset, uint32 nLength, EStreamLockType eStreamLock);
		virtual void unlockImpl();
	};

}; //LostPeterEngine

#endif