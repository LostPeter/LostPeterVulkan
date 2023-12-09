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
        StreamVertex(size_t nVertexSize, size_t nVertexNum, EStreamUsageType eStreamUsage,
					 bool bSystemMemory, bool bUseShadowStream);
		StreamVertex(size_t nSizeInBytes, EStreamUsageType eStreamUsage,
					 bool bSystemMemory, bool bUseShadowStream);
		virtual ~StreamVertex();

	public:
	protected:
		size_t m_nStreamVertexSize;	
		size_t m_nStreamVertexNum;

		bool m_bSingleFormat;

	public:
		F_FORCEINLINE size_t GetStreamVertexSize() const { return m_nStreamVertexSize; }
		void SetStreamVertexSize(size_t nVertexSize);
		F_FORCEINLINE size_t GetStreamVertexNum() const { return m_nStreamVertexNum; }
    };

}; //LostPeterEngine

#endif