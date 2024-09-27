/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MeshSub.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"

namespace LostPeterVulkan
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
        //Vertex
        if (this->poVertexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
        }
        this->poVertexBuffer = VK_NULL_HANDLE;
        this->poVertexBufferMemory = VK_NULL_HANDLE;

        if (this->poVertexBuffer_Staging != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poVertexBuffer_Staging, this->poVertexBufferMemory_Staging);
        }
        this->poVertexBuffer_Staging = VK_NULL_HANDLE;
        this->poVertexBufferMemory_Staging = VK_NULL_HANDLE;

        //Index
        if (this->poIndexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
        }
        this->poIndexBuffer = VK_NULL_HANDLE;
        this->poIndexBufferMemory = VK_NULL_HANDLE;

        if (this->poIndexBuffer_Staging != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poIndexBuffer_Staging, this->poIndexBufferMemory_Staging);
        }
        this->poIndexBuffer_Staging = VK_NULL_HANDLE;
        this->poIndexBufferMemory_Staging = VK_NULL_HANDLE;
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
        if (this->poTypeVertex != F_MeshVertex_Pos3Color4)
        {
            F_Assert("MeshSub::CreateMeshSub: Type vertex is not [F_MeshVertex_Pos3Color4] !");
            return false;
        }

        //1> Vertex/Index Data
        this->sphere = meshDataPC.sphere;
        this->aabb = meshDataPC.aabb;
        if (isTransformLocal)
        {
            this->sphere.Transform(matTransformLocal);
            this->aabb.TransformAffine(matTransformLocal);
        }

        int count_vertex = (int)meshDataPC.vertices.size();
        this->vertices_Pos3Color4.clear();
        this->vertices_Pos3Color4.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertexPC& vertex = meshDataPC.vertices[i];
            FVertex_Pos3Color4 v;
            v.pos = vertex.pos;
            v.color = vertex.color;
            if (isTransformLocal)
            {
                v.pos = FMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4.push_back(v);
        }

        int count_index = (int)meshDataPC.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshDataPC.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4]: Vertex count: [%d], Index count: [%d] !", 
                  this->nameMeshSub.c_str(),
                  this->name.c_str(),
                  (int)this->vertices_Pos3Color4.size(),
                  (int)this->indices.size());

        //2> createVertexBuffer
        if (!this->isNeedUpdate_VertexBuffer)
        {
            Base::GetWindowPtr()->createVertexBuffer("Vertex-" + this->nameMeshSub,
                                                     this->poVertexBuffer_Size, 
                                                     this->poVertexBuffer_Data, 
                                                     this->poVertexBuffer, 
                                                     this->poVertexBufferMemory);
        }
        else
        {
            Base::GetWindowPtr()->createVertexBuffer("Vertex-" + this->nameMeshSub,
                                                     this->poVertexBuffer_Size, 
                                                     this->poVertexBuffer_Data, 
                                                     this->poVertexBuffer, 
                                                     this->poVertexBufferMemory,
                                                     this->poVertexBuffer_Staging, 
                                                     this->poVertexBufferMemory_Staging);
        }

        //3> createIndexBuffer
        if (this->poIndexBuffer_Size > 0 &&
            this->poIndexBuffer_Data != nullptr)
        {
            if (!this->isNeedUpdate_VertexBuffer)
            {
                Base::GetWindowPtr()->createIndexBuffer("Index-" + this->nameMeshSub,
                                                        this->poIndexBuffer_Size, 
                                                        this->poIndexBuffer_Data, 
                                                        this->poIndexBuffer, 
                                                        this->poIndexBufferMemory);
            }   
            else
            {
                Base::GetWindowPtr()->createIndexBuffer("Index-" + this->nameMeshSub,
                                                        this->poIndexBuffer_Size, 
                                                        this->poIndexBuffer_Data, 
                                                        this->poIndexBuffer, 
                                                        this->poIndexBufferMemory,
                                                        this->poIndexBuffer_Staging, 
                                                        this->poIndexBufferMemory_Staging);
            }
        }

        return true;
    }
    bool MeshSub::CreateMeshSub(FMeshData& meshData, bool isTransformLocal, const FMatrix4& matTransformLocal)
    {
        //1> Vertex/Index Data
        this->sphere = meshData.sphere;
        this->aabb = meshData.aabb;
        if (isTransformLocal)
        {
            this->sphere.Transform(matTransformLocal);
            this->aabb.TransformAffine(matTransformLocal);
        }

        int count_vertex = (int)meshData.vertices.size();
        if (this->poTypeVertex == F_MeshVertex_Pos3Color4Tex2)
        {   
            this->vertices_Pos3Color4Tex2.clear();
            this->vertices_Pos3Color4Tex2.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Tex2 v;
                v.pos = vertex.pos;
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                v.texCoord = vertex.texCoord;
                if (isTransformLocal)
                {
                    v.pos = FMath::Transform(matTransformLocal, v.pos);
                }
                this->vertices_Pos3Color4Tex2.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->indices.clear();
            this->indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->indices.push_back(meshData.indices32[i]);
            }
            this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Tex2.size();
            this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Tex2);
            this->poVertexBuffer_Data = &this->vertices_Pos3Color4Tex2[0];
            this->poIndexCount = (uint32_t)this->indices.size();
            this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
            this->poIndexBuffer_Data = &this->indices[0];

            F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Tex2]: Vertex count: [%d], Index count: [%d] !", 
                      this->nameMeshSub.c_str(),
                      this->name.c_str(),
                      (int)this->vertices_Pos3Color4Tex2.size(), 
                      (int)this->indices.size());
        }
        else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tex2)
        {
            this->vertices_Pos3Color4Normal3Tex2.clear();
            this->vertices_Pos3Color4Normal3Tex2.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tex2 v;
                v.pos = vertex.pos;
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                v.normal = vertex.normal;
                v.texCoord = vertex.texCoord;
                if (isTransformLocal)
                {
                    v.pos = FMath::Transform(matTransformLocal, v.pos);
                }
                this->vertices_Pos3Color4Normal3Tex2.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->indices.clear();
            this->indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->indices.push_back(meshData.indices32[i]);
            }
            this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tex2.size();
            this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
            this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
            this->poIndexCount = (uint32_t)this->indices.size();
            this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
            this->poIndexBuffer_Data = &this->indices[0];

            F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                      this->nameMeshSub.c_str(),
                      this->name.c_str(),
                      (int)this->vertices_Pos3Color4Normal3Tex2.size(), 
                      (int)this->indices.size());
        }
        else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tex4)
        {
            this->vertices_Pos3Color4Normal3Tex4.clear();
            this->vertices_Pos3Color4Normal3Tex4.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tex4 v;
                v.pos = vertex.pos;
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                v.normal = vertex.normal;
                v.texCoord = FVector4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
                if (isTransformLocal)
                {
                    v.pos = FMath::Transform(matTransformLocal, v.pos);
                }
                this->vertices_Pos3Color4Normal3Tex4.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->indices.clear();
            this->indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->indices.push_back(meshData.indices32[i]);
            }
            this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tex4.size();
            this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex4);
            this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex4[0];
            this->poIndexCount = (uint32_t)this->indices.size();
            this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
            this->poIndexBuffer_Data = &this->indices[0];

            F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tex4]: Vertex count: [%d], Index count: [%d] !", 
                      this->nameMeshSub.c_str(),
                      this->name.c_str(),
                      (int)this->vertices_Pos3Color4Normal3Tex4.size(), 
                      (int)this->indices.size());
        }
        else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        {
            this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
            this->vertices_Pos3Color4Normal3Tangent3Tex2.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tangent3Tex2 v;
                v.pos = vertex.pos;
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                v.normal = vertex.normal;
                v.tangent = vertex.tangent;
                v.texCoord = vertex.texCoord;
                if (isTransformLocal)
                {
                    v.pos = FMath::Transform(matTransformLocal, v.pos);
                }
                this->vertices_Pos3Color4Normal3Tangent3Tex2.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->indices.clear();
            this->indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->indices.push_back(meshData.indices32[i]);
            }
            this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
            this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
            this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
            this->poIndexCount = (uint32_t)this->indices.size();
            this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
            this->poIndexBuffer_Data = &this->indices[0];

            F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                      this->nameMeshSub.c_str(),
                      this->name.c_str(),
                      (int)this->vertices_Pos3Color4Normal3Tangent3Tex2.size(), 
                      (int)this->indices.size());
        }
        else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex4)
        {
            this->vertices_Pos3Color4Normal3Tangent3Tex4.clear();
            this->vertices_Pos3Color4Normal3Tangent3Tex4.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tangent3Tex4 v;
                v.pos = vertex.pos;
                v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
                v.normal = vertex.normal;
                v.tangent = vertex.tangent;
                v.texCoord = FVector4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
                if (isTransformLocal)
                {
                    v.pos = FMath::Transform(matTransformLocal, v.pos);
                }
                this->vertices_Pos3Color4Normal3Tangent3Tex4.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->indices.clear();
            this->indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->indices.push_back(meshData.indices32[i]);
            }
            this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tangent3Tex4.size();
            this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex4);
            this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex4[0];
            this->poIndexCount = (uint32_t)this->indices.size();
            this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
            this->poIndexBuffer_Data = &this->indices[0];

            F_LogInfo("MeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tangent3Tex4]: Vertex count: [%d], Index count: [%d] !", 
                      this->nameMeshSub.c_str(),
                      this->name.c_str(),
                      (int)this->vertices_Pos3Color4Normal3Tangent3Tex4.size(), 
                      (int)this->indices.size());
        }
        else
        {
            F_LogError("*********************** MeshSub::CreateMeshSub: create mesh sub failed: [%s], wrong poTypeVertex !", this->nameMeshSub.c_str());
            return false; 
        }

        //2> createVertexBuffer
        if (!this->isNeedUpdate_VertexBuffer)
        {
            Base::GetWindowPtr()->createVertexBuffer("Vertex-" + this->nameMeshSub,
                                                     this->poVertexBuffer_Size, 
                                                     this->poVertexBuffer_Data, 
                                                     this->poVertexBuffer, 
                                                     this->poVertexBufferMemory);
        }
        else
        {
            Base::GetWindowPtr()->createVertexBuffer("Vertex-" + this->nameMeshSub,
                                                     this->poVertexBuffer_Size, 
                                                     this->poVertexBuffer_Data, 
                                                     this->poVertexBuffer, 
                                                     this->poVertexBufferMemory,
                                                     this->poVertexBuffer_Staging, 
                                                     this->poVertexBufferMemory_Staging);
        }

        //3> createIndexBuffer
        if (this->poIndexBuffer_Size > 0 &&
            this->poIndexBuffer_Data != nullptr)
        {
            if (!this->isNeedUpdate_VertexBuffer)
            {
                Base::GetWindowPtr()->createIndexBuffer("Index-" + this->nameMeshSub,
                                                        this->poIndexBuffer_Size, 
                                                        this->poIndexBuffer_Data, 
                                                        this->poIndexBuffer, 
                                                        this->poIndexBufferMemory);
            }   
            else
            {
                Base::GetWindowPtr()->createIndexBuffer("Index-" + this->nameMeshSub,
                                                        this->poIndexBuffer_Size, 
                                                        this->poIndexBuffer_Data, 
                                                        this->poIndexBuffer, 
                                                        this->poIndexBufferMemory,
                                                        this->poIndexBuffer_Staging, 
                                                        this->poIndexBufferMemory_Staging);
            }
        }

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
        if (!this->isNeedUpdate_VertexBuffer ||
            this->poVertexBuffer_Staging == VK_NULL_HANDLE ||
            this->poVertexBufferMemory_Staging == VK_NULL_HANDLE)
            return;
        
        Base::GetWindowPtr()->updateVKBuffer(0, this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBufferMemory_Staging);
        Base::GetWindowPtr()->copyVkBuffer(this->poVertexBuffer_Staging, this->poVertexBuffer, this->poVertexBuffer_Size);
    }
    void MeshSub::UpdateIndexBuffer()
    {
        if (!this->isNeedUpdate_IndexBuffer ||
            this->poIndexBuffer_Staging == VK_NULL_HANDLE ||
            this->poIndexBufferMemory_Staging == VK_NULL_HANDLE)
            return;

        Base::GetWindowPtr()->updateVKBuffer(0, this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBufferMemory_Staging);
        Base::GetWindowPtr()->copyVkBuffer(this->poIndexBuffer_Staging, this->poIndexBuffer, this->poIndexBuffer_Size);
    }

}; //LostPeterVulkan