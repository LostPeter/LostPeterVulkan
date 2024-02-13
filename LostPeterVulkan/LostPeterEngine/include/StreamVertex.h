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

#ifndef _STREAM_VERTEX_H_
#define _STREAM_VERTEX_H_

#include "Stream.h"

namespace LostPeterEngine
{
    class engineExport StreamVertex : public Stream
    {
    public:
        StreamVertex(uint32 nVertexSize, 
					 uint32 nVertexCount, 
					 EStreamUsageType eStreamUsage,
					 bool bIsUseSystemMemory, 
					 bool bIsUseShadowStream);
		StreamVertex(uint32 nSizeInBytes, 
					 EStreamUsageType eStreamUsage,
					 bool bIsUseSystemMemory, 
					 bool bIsUseShadowStream);
		virtual ~StreamVertex();

	public:
	protected:
		uint32 m_nStreamVertexSize;	
		uint32 m_nStreamVertexCount;

		bool m_bSingleFormat;

	public:
		F_FORCEINLINE uint32 GetStreamVertexSize() const { return m_nStreamVertexSize; }
		void SetStreamVertexSize(uint32 nVertexSize);
		F_FORCEINLINE uint32 GetStreamVertexCount() const { return m_nStreamVertexCount; }
    };

}; //LostPeterEngine

#endif