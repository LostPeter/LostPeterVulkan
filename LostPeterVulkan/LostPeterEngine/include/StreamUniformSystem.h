/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _STREAM_UNIFORM_SYSTEM_H_
#define _STREAM_UNIFORM_SYSTEM_H_

#include "StreamUniform.h"

namespace LostPeterEngine
{
    class engineExport StreamUniformSystem : public StreamUniform 
	{
	public:
		StreamUniformSystem(uint32 nUniformSize, uint32 nUniformCount, EStreamUsageType eStreamUsage);
		StreamUniformSystem(uint32 nSizeInBytes, EStreamUsageType eStreamUsage);
		virtual ~StreamUniformSystem();

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