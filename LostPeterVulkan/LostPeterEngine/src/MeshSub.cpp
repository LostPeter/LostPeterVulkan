/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MeshSub.h"
#include "../include/Mesh.h"
#include "../include/DataVertex.h"
#include "../include/DataIndex.h"
#include "../include/DataVertexIndex.h"

namespace LostPeterEngine
{
    MeshSub::MeshSub(const String& nameMeshSub)
        : Base(nameMeshSub)
        , m_pMesh(nullptr)
        , m_nMeshSubIndex(0)
        , m_eRenderPrimitive(F_RenderPrimitive_TriangleList)
        , m_pDataVertex(nullptr)
        , m_pDataIndex(nullptr)
        , m_bIsUseSharedVertex(false)
    {

    }

    MeshSub::~MeshSub()
    {
        Destroy();
    }

    void MeshSub::Destroy()
    {
        F_DELETE(m_pDataVertex)
		F_DELETE(m_pDataIndex)
    }

    bool MeshSub::GetDataVertexIndex(DataVertexIndex& dataVI, uint32 nIndexLOD /*= 0*/)
    {
        if (m_pMesh == nullptr)
            return false;

        dataVI.m_eRenderPrimitive = m_eRenderPrimitive;
        dataVI.m_pDataVertex = m_bIsUseSharedVertex ? m_pMesh->GetDataVertexShared() : m_pDataVertex;
		if (nIndexLOD > 0)
		{
			
		}
		else
		{
			dataVI.m_pDataIndex = m_pDataIndex;
		}
         dataVI.m_bIsUseIndex = m_pDataIndex != nullptr ? true : false;

		F_Assert(dataVI.m_pDataVertex && "MeshSub::GetDataVertexIndex")
		return true;
    }
    

}; //LostPeterEngine