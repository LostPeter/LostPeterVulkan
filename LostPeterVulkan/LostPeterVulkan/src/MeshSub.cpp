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
#include "../include/VKBufferVertex.h"
#include "../include/VKBufferVertexIndex.h"

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

		//VKBufferVertex/VKBufferVertexIndex
        , pBufferVertex(nullptr)
        , pBufferVertexIndex(nullptr)

        //Vertex
        , poTypeVertex(_poTypeVertex)
        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)

        //Index
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)

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
		F_DELETE(pBufferVertex)
        F_DELETE(pBufferVertexIndex)
    }

	const VkBuffer& MeshSub::GetVKBufferVertex() const 
	{ 
		if (this->pBufferVertexIndex != nullptr)
		{
			return this->pBufferVertexIndex->GetVKBufferVertex();
		}
		return this->pBufferVertex->GetVKBufferVertex();
	}
	const VkDeviceMemory& MeshSub::GetVKBufferVertexMemory() const 
	{ 
		if (this->pBufferVertexIndex != nullptr)
		{
			return this->pBufferVertexIndex->GetVKBufferVertexMemory();
		}
		return this->pBufferVertex->GetVKBufferVertexMemory();
	}
	const VkBuffer& MeshSub::GetVKBufferIndex() const 
	{ 
		return this->pBufferVertexIndex->GetVKBufferIndex(); 
	}
	const VkDeviceMemory& MeshSub::GetVKBufferIndexMemory() const 
	{ 
		return this->pBufferVertexIndex->GetVKBufferIndexMemory(); 
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
        this->aabb = meshDataPC.aabb;
        if (isTransformLocal)
        {
            this->aabb.TransformAffine(matTransformLocal);
        }
        this->aabb.MakeSphere(this->sphere);

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

		//2> createBufferVertexIndex or createBufferVertex
        if (this->poIndexBuffer_Size > 0 &&
            this->poIndexBuffer_Data != nullptr)
        {
			this->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + this->nameMeshSub,
                                                                                     this->poTypeVertex,
                                                                                     this->poVertexBuffer_Size, 
                                                                                     (uint8*)this->poVertexBuffer_Data, 
                                                                                     false,
                                                                                     this->poIndexBuffer_Size, 
                                                                                     (uint8*)this->poIndexBuffer_Data, 
                                                                                     false,
																					 this->isNeedUpdate_VertexBuffer);
			if (this->pBufferVertexIndex == nullptr)
            {
                F_LogError("*********************** MeshSub::CreateMeshSub: create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
            }
        }
		else
		{
			this->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + this->nameMeshSub,
                                                                           this->poTypeVertex,
                                                                           this->poVertexBuffer_Size, 
                                                                           (uint8*)this->poVertexBuffer_Data, 
                                                                           false,
																		   this->isNeedUpdate_VertexBuffer);
            if (this->pBufferVertex == nullptr)
            {
                F_LogError("*********************** MeshSub::CreateMeshSub: create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
            }
		}

        return true;
    }
    bool MeshSub::CreateMeshSub(FMeshData& meshData, bool isTransformLocal, const FMatrix4& matTransformLocal)
    {
        //1> Vertex/Index Data
        this->aabb = meshData.aabb;
        if (isTransformLocal)
        {
            this->aabb.TransformAffine(matTransformLocal);
        }
        this->aabb.MakeSphere(this->sphere);

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

        //2> createBufferVertexIndex or createBufferVertex
        if (this->poIndexBuffer_Size > 0 &&
            this->poIndexBuffer_Data != nullptr)
        {
			this->pBufferVertexIndex = Base::GetWindowPtr()->createBufferVertexIndex("VertexIndex-" + this->nameMeshSub,
                                                                                     this->poTypeVertex,
                                                                                     this->poVertexBuffer_Size, 
                                                                                     (uint8*)this->poVertexBuffer_Data, 
                                                                                     false,
                                                                                     this->poIndexBuffer_Size, 
                                                                                     (uint8*)this->poIndexBuffer_Data, 
                                                                                     false,
																					 this->isNeedUpdate_VertexBuffer);
			if (this->pBufferVertexIndex == nullptr)
            {
                F_LogError("*********************** MeshSub::CreateMeshSub: create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
            }
        }
		else
		{
			this->pBufferVertex = Base::GetWindowPtr()->createBufferVertex("Vertex-" + this->nameMeshSub,
                                                                           this->poTypeVertex,
                                                                           this->poVertexBuffer_Size, 
                                                                           (uint8*)this->poVertexBuffer_Data, 
                                                                           false,
																		   this->isNeedUpdate_VertexBuffer);
            if (this->pBufferVertex == nullptr)
            {
                F_LogError("*********************** MeshSub::CreateMeshSub: create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
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
		if (this->pBufferVertexIndex != nullptr)
        {
            this->pBufferVertexIndex->UpdateVertex(0,
                                             	   this->poVertexBuffer_Size, 
                                             	   (uint8*)this->poVertexBuffer_Data);
        }
        else if (this->pBufferVertex != nullptr)
        {
            this->pBufferVertex->UpdateVertex(0,
											  this->poVertexBuffer_Size, 
											  (uint8*)this->poVertexBuffer_Data);
        }
    }
    void MeshSub::UpdateIndexBuffer()
    {
        if (this->pBufferVertexIndex != nullptr)
        {
            this->pBufferVertexIndex->UpdateIndex(0,
                                             	  this->poIndexBuffer_Size, 
                                                  (uint8*)this->poIndexBuffer_Data);
        }
    }

}; //LostPeterVulkan