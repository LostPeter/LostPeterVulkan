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

#include "../include/DataVertexIndex.h"
#include "../include/DataVertex.h"
#include "../include/DataIndex.h"

namespace LostPeterEngine
{
    DataVertexIndex::DataVertexIndex()
		: m_eRenderPrimitive(F_RenderPrimitive_TriangleList)
		, m_pVertexData(nullptr)
		, m_pIndexData(nullptr)
		, m_bUseIndices(true)
		, m_pRenderable(nullptr)
		//, m_eRenderInstance(E_RenderInstance_None)
		, m_nInstanceCount(0)
	{
		
	}

    DataVertexIndex::~DataVertexIndex()
    {
        
    }

}; //LostPeterEngine