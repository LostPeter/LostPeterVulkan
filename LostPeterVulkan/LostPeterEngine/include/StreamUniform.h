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

#ifndef _STREAM_UNIFORM_H_
#define _STREAM_UNIFORM_H_

#include "Stream.h"

namespace LostPeterEngine
{
    class engineExport StreamUniform : public Stream
    {
    public:
        StreamUniform(uint32 nUniformSize, 
					  uint32 nUniformCount, 
					  EStreamUsageType eStreamUsage,
					  bool bIsUseSystemMemory, 
					  bool bIsUseShadowStream);
		StreamUniform(uint32 nSizeInBytes, 
					  EStreamUsageType eStreamUsage,
					  bool bIsUseSystemMemory, 
					  bool bIsUseShadowStream);
		virtual ~StreamUniform();

	public:
	protected:
		uint32 m_nStreamUniformSize;	
		uint32 m_nStreamUniformCount;

	public:
		F_FORCEINLINE uint32 GetStreamUniformSize() const { return m_nStreamUniformSize; }
		void SetStreamUniformSize(uint32 nUniformSize);
		F_FORCEINLINE uint32 GetStreamUniformCount() const { return m_nStreamUniformCount; }
    };

}; //LostPeterEngine

#endif