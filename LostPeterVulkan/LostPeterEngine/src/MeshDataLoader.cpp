/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/MeshManager.h"
#include "../include/DataVertex.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LostPeterEngine
{
	void Mesh::destroyInternalResourcesImpl()
	{

	}

	static void s_CreateMesh_FromMeshDataVector(Mesh* pMesh, FMeshDataVector& aMeshDatas)
	{
		const String& nameMesh = pMesh->GetName();

		int count_mesh_sub = (int)aMeshDatas.size();
        for (int i = 0; i < count_mesh_sub; i++)
        {
            FMeshData& meshData = aMeshDatas[i];
            
            String nameMeshSub = nameMesh + "-" + FUtilString::SaveInt(i);
            MeshSub* pMeshSub = pMesh->CreateMeshSub(nameMeshSub);
			if (!pMeshSub)
			{
				F_LogError("*********************** s_CreateMesh_FromMeshDataVector: Create mesh sub failed: [%s] !", nameMeshSub.c_str());
				continue;
			}
			if (!pMeshSub->Init(meshData, false, FMath::ms_mat4Unit))
			{
				pMesh->DeleteMeshSub(pMeshSub);
				F_LogError("*********************** s_CreateMesh_FromMeshDataVector: Mesh sub init failed: [%s] !", nameMeshSub.c_str());
				continue;
			}
        }
	}
	static void s_CreateMesh_FromMeshDataPCVector(Mesh* pMesh, FMeshDataPCVector& aMeshDataPCs)
	{
		const String& nameMesh = pMesh->GetName();

		int count_mesh_sub = (int)aMeshDataPCs.size();
        for (int i = 0; i < count_mesh_sub; i++)
        {
            FMeshDataPC& meshDataPC = aMeshDataPCs[i];
            
            String nameMeshSub = nameMesh + "-" + FUtilString::SaveInt(i);
            MeshSub* pMeshSub = pMesh->CreateMeshSub(nameMeshSub);
			if (!pMeshSub)
			{
				F_LogError("*********************** s_CreateMesh_FromMeshDataPCVector: Create mesh sub failed: [%s] !", nameMeshSub.c_str());
				continue;
			}
			if (!pMeshSub->Init(meshDataPC, false, FMath::ms_mat4Unit))
			{
				pMesh->DeleteMeshSub(pMeshSub);
				F_LogError("*********************** s_CreateMesh_FromMeshDataPCVector: Mesh sub init failed: [%s] !", nameMeshSub.c_str());
				continue;
			}
        }
	}

	static void s_CreateMesh_FromFile(Mesh* pMesh)
	{
		F_Assert(pMesh->GetMeshType() == F_Mesh_File && "s_CreateMesh_FromFile")
		const String& pathMesh = pMesh->GetPath();
		bool bIsFlipY = pMesh->GetIsFlipY();
		
		//1> Load MeshData from file
		FMeshDataVector aMeshDatas;
		unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
		if (!FMeshDataLoader::LoadMeshDatas(pathMesh, aMeshDatas, bIsFlipY, eMeshParserFlags))
		{
			F_LogError("*********************** s_CreateMesh_FromFile: load meshes failed, path: [%s] !", pathMesh.c_str());
			return; 
		}

		//2> Create MeshSub from MeshData
        s_CreateMesh_FromMeshDataVector(pMesh, aMeshDatas);
	}
	static void s_CreateMesh_FromGeometry(Mesh* pMesh)
	{
		F_Assert(pMesh->GetMeshType() == F_Mesh_Geometry && "s_CreateMesh_FromGeometry")

		const String& nameMesh = pMesh->GetName();
		FMeshGeometryType eMeshGeometry = pMesh->GetMeshGeometryType();
		bool bIsFlipY = pMesh->GetIsFlipY();
		FMeshCreateParam* pMeshCreateParam = pMesh->GetMeshCreateParam();

		FMeshDataVector aMeshDatas;
		FMeshDataPCVector aMeshDataPCs;

		//1> Create MeshData/MeshDataPC from Geometry
		FMeshDataPC meshDataPC;
		FMeshData meshData;
		meshData.bIsFlipY = bIsFlipY;
		if (!FMeshGeometry::CreateGeometryWithParam(&meshDataPC, &meshData, eMeshGeometry, pMeshCreateParam))
		{
			F_LogError("*********************** s_CreateMesh_FromGeometry: create geometry mesh failed: typeGeometry: [%s] !", F_GetMeshGeometryTypeName(eMeshGeometry).c_str());
			return;
		}

		//2> Create MeshSub from MeshData/MeshDataPC
		if (meshData.GetVertexCount() > 0)
		{
			aMeshDatas.push_back(meshData);
			s_CreateMesh_FromMeshDataVector(pMesh, aMeshDatas);
		}
		if (meshDataPC.GetVertexCount() > 0)
		{   
			aMeshDataPCs.push_back(meshDataPC);
			s_CreateMesh_FromMeshDataPCVector(pMesh, aMeshDataPCs);
		}
	}
    
	void Mesh::createInternalResourcesImpl()
	{
		if (this->m_eMesh == F_Mesh_File)
        {
            s_CreateMesh_FromFile(this);
        }
		else if (this->m_eMesh == F_Mesh_Geometry)
        {
			s_CreateMesh_FromGeometry(this);
        }
        else
        {
            F_Assert(false && "Mesh::createInternalResourcesImpl: Wrong type mesh !")
            return;
        }
	}
    
}; //LostPeterEngine