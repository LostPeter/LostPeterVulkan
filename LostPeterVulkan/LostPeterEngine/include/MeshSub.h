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

#ifndef _MESH_SUB_H_
#define _MESH_SUB_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport MeshSub : public Base
    {
        friend class Mesh;

    public:
        MeshSub(const String& nameMeshSub);
        virtual ~MeshSub();

    public:
        Mesh* m_pMesh;
        int32 m_nMeshSubIndex;

        FRenderPrimitiveType m_eRenderPrimitive;
        DataVertex* m_pDataVertex;
        DataIndex* m_pDataIndex;
        bool m_bIsUseSharedVertex;

        FAABB m_boundAABB;
        FSphere m_boundSphere;

    public:
        F_FORCEINLINE Mesh* GetMesh() const { return m_pMesh; }
		F_FORCEINLINE void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }
        F_FORCEINLINE int GetMeshSubIndex() const { return m_nMeshSubIndex; }
		F_FORCEINLINE void SetMeshSubIndex(int nMeshSubIndex) { m_nMeshSubIndex = nMeshSubIndex; }

        F_FORCEINLINE FRenderPrimitiveType GetRenderPrimitiveType() const { return m_eRenderPrimitive; }
		F_FORCEINLINE void SetRenderPrimitiveType(FRenderPrimitiveType eRenderPrimitive) { m_eRenderPrimitive = eRenderPrimitive; }
		F_FORCEINLINE DataVertex* GetDataVertex() const { return m_pDataVertex; }
		F_FORCEINLINE void SetDataVertex(DataVertex* pDataVertex) { m_pDataVertex = pDataVertex; }
		F_FORCEINLINE DataIndex* GetDataIndex() const { return m_pDataIndex; }
		F_FORCEINLINE void SetDataIndex(DataIndex* pDataIndex) { m_pDataIndex = pDataIndex; } 
        F_FORCEINLINE bool GetIsUseSharedVertex() const { return m_bIsUseSharedVertex; }
		F_FORCEINLINE void SetIsUseSharedVertex(bool bIsUseSharedVertex) { m_bIsUseSharedVertex = bIsUseSharedVertex; } 

        F_FORCEINLINE const FAABB& GetBoundAABB() const { return m_boundAABB; }
		F_FORCEINLINE void SetBoundAABB(const FAABB& boundAABB)	{ m_boundAABB = boundAABB; }
        F_FORCEINLINE const FSphere& GetBoundSphere() const { return m_boundSphere; }
		F_FORCEINLINE void SetBoundSphere(const FSphere& boundSphere) { m_boundSphere = boundSphere; }

    public:
        void Destroy();
        

    public:
        bool GetDataVertexIndex(DataVertexIndex& dataVI, uint32 nIndexLOD = 0);


    };

}; //LostPeterEngine

#endif