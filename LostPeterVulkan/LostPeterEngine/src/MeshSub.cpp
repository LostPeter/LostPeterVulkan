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

namespace LostPeter
{
    MeshSub::MeshSub(Mesh* _pMesh, 
                     const String& _nameOriginal,
                     const String& _nameMeshSub,
                     int _indexMeshSub,
                     FMeshVertexType _poTypeVertex,
                     bool isUpdateVertexBuffer,
                     bool isUpdateIndexBuffer)
        : Base(_nameOriginal)
        , pMesh(_pMesh)
        , nameMeshSub(_nameMeshSub)
        , indexMeshSub(_indexMeshSub)
        , isNeedUpdate_VertexBuffer(isUpdateVertexBuffer)
        , isNeedUpdate_IndexBuffer(isUpdateIndexBuffer)

        //Vertex
        , poTypeVertex(_poTypeVertex)
        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)
        , poVertexBuffer(VK_NULL_HANDLE)
        , poVertexBufferMemory(VK_NULL_HANDLE)
        , poVertexBuffer_Staging(VK_NULL_HANDLE)
        , poVertexBufferMemory_Staging(VK_NULL_HANDLE)

        //Index
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)
        , poIndexBuffer(VK_NULL_HANDLE)
        , poIndexBufferMemory(VK_NULL_HANDLE)
        , poIndexBuffer_Staging(VK_NULL_HANDLE)
        , poIndexBufferMemory_Staging(VK_NULL_HANDLE)

        //Instance
        , instanceCount(1)
    {

    }
    MeshSub::~MeshSub()
    {
        Destroy();
    }
    void MeshSub::Destroy()
    {
        
    }
    uint32_t MeshSub::GetVertexSize() 
    {
        if (this->vertices_Pos3Color4Tex2.size() > 0)
            return sizeof(FVertex_Pos3Color4Tex2);
        else if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
            return sizeof(FVertex_Pos3Color4Normal3Tex2);
        else if(this->vertices_Pos3Color4Normal3Tex4.size() > 0)
            return sizeof(FVertex_Pos3Color4Normal3Tex4);
        else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
            return sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        else if (this->vertices_Pos3Color4Normal3Tangent3Tex4.size() > 0)
            return sizeof(FVertex_Pos3Color4Normal3Tangent3Tex4);
        else
        {
            F_Assert(false && "MeshSub::GetVertexSize: wrong vertex type !")
            return 0;
        }
    }
    uint32_t MeshSub::GetIndexSize()
    {
        return sizeof(uint32_t);
    }
    bool MeshSub::CreateMeshSub(FMeshDataPC& meshDataPC, bool isTransformLocal, const FMatrix4& matTransformLocal)
    {
        

        return true;
    }
    bool MeshSub::CreateMeshSub(FMeshData& meshData, bool isTransformLocal, const FMatrix4& matTransformLocal)
    {
       

        return true;
    }
    void MeshSub::WriteVertexData(std::vector<FVertex_Pos3Color4Normal3Tex2>& aPos3Color4Normal3Tex2,
                                  std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2>& aPos3Color4Normal3Tangent3Tex2)
    {
        size_t count = 0;
        if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
        {
            count = this->vertices_Pos3Color4Normal3Tex2.size();
            for (size_t i = 0; i < count; i++)
            {
                FVertex_Pos3Color4Normal3Tex2& vSrc = this->vertices_Pos3Color4Normal3Tex2[i];
                aPos3Color4Normal3Tex2.push_back(vSrc);
            }
        }
        else if (this->vertices_Pos3Color4Normal3Tex4.size() > 0)
        {
            count = this->vertices_Pos3Color4Normal3Tex4.size();
            for (size_t i = 0; i < count; i++)
            {
                FVertex_Pos3Color4Normal3Tex4& vSrc = this->vertices_Pos3Color4Normal3Tex4[i];
                aPos3Color4Normal3Tex2.push_back(FVertex_Pos3Color4Normal3Tex2(vSrc.pos,
                                                vSrc.color,
                                                vSrc.normal,
                                                FVector2(vSrc.texCoord.x, vSrc.texCoord.y)));
            }
        }   
        else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
        {
            count = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
            for (size_t i = 0; i < count; i++)
            {
                FVertex_Pos3Color4Normal3Tangent3Tex2& vSrc = this->vertices_Pos3Color4Normal3Tangent3Tex2[i];
                aPos3Color4Normal3Tangent3Tex2.push_back(vSrc);
            }
        }   
        else if (this->vertices_Pos3Color4Normal3Tangent3Tex4.size() > 0)
        {
            count = this->vertices_Pos3Color4Normal3Tangent3Tex4.size();
            for (size_t i = 0; i < count; i++)
            {
                FVertex_Pos3Color4Normal3Tangent3Tex4& vSrc = this->vertices_Pos3Color4Normal3Tangent3Tex4[i];
                aPos3Color4Normal3Tangent3Tex2.push_back(FVertex_Pos3Color4Normal3Tangent3Tex2(vSrc.pos,
                                                        vSrc.color,
                                                        vSrc.normal,
                                                        vSrc.tangent,
                                                        FVector2(vSrc.texCoord.x, vSrc.texCoord.y)));
            }
        }
    }
    void MeshSub::WriteIndexData(std::vector<uint32_t>& indexData)
    {
        indexData.insert(indexData.end(), indices.begin(), indices.end());
    }

    void MeshSub::UpdateVertexBuffer()
    {
        
    }
    void MeshSub::UpdateIndexBuffer()
    {
        
    }

}; //LostPeter