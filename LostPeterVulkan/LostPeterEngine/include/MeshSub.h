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
    public:
        MeshSub(Mesh* _pMesh, 
                const String& _nameOriginal,
                const String& _nameMeshSub,
                int _indexMeshSub,
                FMeshVertexType _poTypeVertex,
                bool isUpdateVertexBuffer,
                bool isUpdateIndexBuffer);
        virtual ~MeshSub();

    public:
        Mesh* pMesh;
        String nameMeshSub;
        int indexMeshSub;
        bool isNeedUpdate_VertexBuffer;
        bool isNeedUpdate_IndexBuffer;
        FSphere sphere;
        FAABB aabb;

        //Vertex
        FMeshVertexType poTypeVertex;
        FMeshVertexPCVector vertices_Pos3Color4;
        std::vector<FVertex_Pos3Color4Tex2> vertices_Pos3Color4Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tex4> vertices_Pos3Color4Normal3Tex4;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex4> vertices_Pos3Color4Normal3Tangent3Tex4;
        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;

        //Index
        std::vector<uint32_t> indices;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;

        //Instance
        uint32_t instanceCount;

    public:
        void Destroy();
        uint32_t GetVertexSize();
        uint32_t GetIndexSize();

        virtual bool CreateMeshSub(FMeshDataPC& meshDataPC, bool isTransformLocal, const FMatrix4& matTransformLocal);
        virtual bool CreateMeshSub(FMeshData& meshData, bool isTransformLocal, const FMatrix4& matTransformLocal);
        virtual void WriteVertexData(std::vector<FVertex_Pos3Color4Normal3Tex2>& aPos3Color4Normal3Tex2,
                                        std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2>& aPos3Color4Normal3Tangent3Tex2);
        virtual void WriteIndexData(std::vector<uint32_t>& indexData);

        virtual void UpdateVertexBuffer();
        virtual void UpdateIndexBuffer();
    };

}; //LostPeterEngine

#endif