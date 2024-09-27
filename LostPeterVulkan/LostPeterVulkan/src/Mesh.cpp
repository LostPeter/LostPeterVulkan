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

#include "../include/Mesh.h"
#include "../include/VulkanWindow.h"
#include "../include/MeshSub.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LostPeterVulkan
{
    Mesh::Mesh(uint32 _group,
               const String& _nameMesh,
               const String& _pathMesh,
               FMeshType _typeMesh,
               FMeshVertexType _typeVertex,
               FMeshGeometryType _typeGeometryType,
               FMeshCreateParam* _pMeshCreateParam)
        : Base(_group, _nameMesh)
        , pathMesh(_pathMesh)
        , typeMesh(_typeMesh)
        , typeVertex(_typeVertex)
        , typeGeometryType(_typeGeometryType)
        , pMeshCreateParam(_pMeshCreateParam)
    {

    }
    Mesh::~Mesh()
    {
        Destroy();
    }

    void Mesh::Destroy()
    {
        int count = (int)this->aMeshSubs.size();
        for (int i = 0; i < count; i++)
        {
            MeshSub* pMeshSub = this->aMeshSubs[i];
            pMeshSub->Destroy();
            delete pMeshSub;
        }
        this->aMeshSubs.clear();
        this->mapMeshSubs.clear();
    }

    int Mesh::GetMeshSubCount() const 
    { 
        return (int)aMeshSubs.size();
    }

    MeshSub* Mesh::GetMeshSub(int index) const
    {
        F_Assert(index >= 0 && index < (int)aMeshSubs.size() && "Mesh::GetMeshSub")
        return aMeshSubs[index];
    }

    bool Mesh::AddMeshSub(MeshSub* pMeshSub)
    {
        MeshSubPtrMap::iterator itFind = this->mapMeshSubs.find(pMeshSub->nameMeshSub);
        if (itFind != this->mapMeshSubs.end())
        {
            F_LogError("*********************** Mesh::AddMeshSub: Mesh sub is exist: [%s] !", pMeshSub->nameMeshSub.c_str());
            return false;
        }

        this->aMeshSubs.push_back(pMeshSub);
        this->mapMeshSubs[pMeshSub->nameMeshSub] = pMeshSub;
        return true;
    }   
    bool Mesh::LoadMesh(bool isFlipY, 
                        bool isTransformLocal, 
                        const FMatrix4& matTransformLocal,
                        bool isUpdateVertexBuffer /*= false*/,
                        bool isUpdateIndexBuffer /*= false*/)
    {
        //1> Load
        FMeshDataPCVector aMeshDataPCs;
        FMeshDataVector aMeshDatas;
        if (this->typeMesh == F_Mesh_File)
        {
            unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
            if (!FMeshDataLoader::LoadMeshDatas(this->pathMesh, aMeshDatas, isFlipY, eMeshParserFlags))
            {
                F_LogError("*********************** Mesh::LoadMesh: load meshes failed: [%s] !", this->pathMesh.c_str());
                return false; 
            }
        }
        else if (this->typeMesh == F_Mesh_Geometry)
        {
            FMeshDataPC meshDataPC;
            FMeshData meshData;
            meshData.bIsFlipY = isFlipY;
            if (!FMeshGeometry::CreateGeometryWithParam(&meshDataPC, &meshData, this->typeGeometryType, this->pMeshCreateParam))
            {
                F_LogError("*********************** Mesh::LoadMesh: create geometry mesh failed: typeGeometry: [%s] !", F_GetMeshGeometryTypeName(this->typeGeometryType).c_str());
                return false; 
            }

            if (meshData.GetVertexCount() > 0)
            {
                aMeshDatas.push_back(meshData);
            }
            if (meshDataPC.GetVertexCount() > 0)
            {   
                aMeshDataPCs.push_back(meshDataPC);
            }
        }
        else
        {
            F_Assert(false && "Mesh::LoadMesh: Wrong typeMesh !")
            return false;
        }

        const String& nameMesh = GetName();
        //MeshData
        int count_mesh_sub = (int)aMeshDatas.size();
        for (int i = 0; i < count_mesh_sub; i++)
        {
            FMeshData& meshData = aMeshDatas[i];
            
            String nameMeshSub = nameMesh + "-" + FUtilString::SaveInt(i);
            MeshSub* pMeshSub = new MeshSub(this,
                                            meshData.nameMesh,
                                            nameMeshSub,
                                            i,
                                            this->typeVertex,
                                            isUpdateVertexBuffer,
                                            isUpdateIndexBuffer);
            if (!pMeshSub->CreateMeshSub(meshData, isTransformLocal, matTransformLocal))
            {
                F_LogError("*********************** Mesh::LoadMesh: Create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
            }
            AddMeshSub(pMeshSub);
        }

        //MeshDataPC
        count_mesh_sub = (int)aMeshDataPCs.size();
        for (int i = 0; i < count_mesh_sub; i++)
        {
            FMeshDataPC& meshDataPC = aMeshDataPCs[i];
            
            String nameMeshSub = nameMesh + "-" + FUtilString::SaveInt(i);
            MeshSub* pMeshSub = new MeshSub(this,
                                            meshDataPC.nameMesh,
                                            nameMeshSub,
                                            i,
                                            this->typeVertex,
                                            isUpdateVertexBuffer,
                                            isUpdateIndexBuffer);
            if (!pMeshSub->CreateMeshSub(meshDataPC, isTransformLocal, matTransformLocal))
            {
                F_LogError("*********************** Mesh::LoadMesh: Create mesh sub failed: [%s] !", nameMeshSub.c_str());
                return false;
            }
            AddMeshSub(pMeshSub);
        }

        return true;
    }
    
}; //LostPeterVulkan