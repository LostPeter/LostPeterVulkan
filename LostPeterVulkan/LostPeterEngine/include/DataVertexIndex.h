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

#ifndef _DATA_VERTEX_INDEX_H_
#define _DATA_VERTEX_INDEX_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport DataVertexIndex
	{
	public:
		DataVertexIndex();
        virtual ~DataVertexIndex();
		
	public:
	    FRenderPrimitiveType m_eRenderPrimitive;
		DataVertex* m_pDataVertex;
		DataIndex* m_pDataIndex;
		bool m_bIsUseIndex;
		const Renderable* m_pRenderable;

		//ERenderInstanceType m_eRenderInstance;
		uint32 m_nInstanceCount;
	};

}; //LostPeterEngine

#endif