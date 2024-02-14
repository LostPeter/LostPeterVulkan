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
                               , public FNonCopyable
    {
        friend class Mesh;

    private:
        MeshSub(const String& nameMeshSub);
    public:
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

    protected:
        std::vector<FVertex_Pos3Color4> m_aVertices_Pos3Color4;
        std::vector<FVertex_Pos3Color4Tex2> m_aVertices_Pos3Color4Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tex2> m_aVertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tex4> m_aVertices_Pos3Color4Normal3Tex4;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> m_aVertices_Pos3Color4Normal3Tangent3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex4> m_aVertices_Pos3Color4Normal3Tangent3Tex4;
        uint32 m_nVertexCount;
        uint32 m_nVertexBuffer_Size;
        void* m_pVertexBuffer_Data;
        
        std::vector<uint32> m_aIndices;
        uint32 m_nIndexCount;
        uint32 m_nIndexBuffer_Size;
        void* m_pIndexBuffer_Data;

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

        F_FORCEINLINE uint32 GetVertexCount() const { return m_nVertexCount; }
        F_FORCEINLINE uint32 GetVertexBufferSize() const { return m_nIndexBuffer_Size; }
        F_FORCEINLINE uint32 GetIndexCount() const { return m_nIndexCount; }
        F_FORCEINLINE uint32 GetIndexBufferSize() const { return m_nIndexBuffer_Size; }

    public:
        void Destroy();

        bool Init(FMeshData& meshData, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);
        bool Init(FMeshDataPC& meshDataPC, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);

    public:
        bool GetDataVertexIndex(DataVertexIndex& dataVI, uint32 nIndexLOD = 0);

        void UpdateBound(const FAABB& aabb, const FSphere& sphere, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal);

    protected:
        virtual bool createDataVertex();
        virtual bool createDataIndex();

    private:
        virtual uint32 calculateSize() const;
    };

}; //LostPeterEngine

#endif