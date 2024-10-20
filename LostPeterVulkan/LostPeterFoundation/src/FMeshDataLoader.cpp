/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FMeshDataLoader.h"
#include "../include/FMath.h"
#include "../include/FUtil.h"


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LostPeterFoundation
{
    //Materials
    static void s_ProcessMaterials(const struct aiScene* pScene, StringVector& aMaterials)
    {
        for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
        {
            aiMaterial* pMaterial = pScene->mMaterials[i];
            if (pMaterial)
            {
                aiString name;
			    aiGetMaterialString(pMaterial, AI_MATKEY_NAME, &name);
			    aMaterials.push_back(name.C_Str());
            }
        }
    }
    
    //FMeshData
	static bool s_ProcessMeshData(void* pTransform, struct aiMesh* pMesh, const struct aiScene* pScene, FMeshData& meshData)
    {
        if (pMesh->mNumVertices <= 0)
            return false;
        
        meshData.nameMesh = pMesh->mName.C_Str();
        aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform);
        for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
        {
            aiVector3D position	= meshTransformation * pMesh->mVertices[i];
            aiColor4D color = pMesh->mColors[0] ? pMesh->mColors[0][i] : aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
            aiVector3D normal = meshTransformation * (pMesh->mNormals ? pMesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 1.0f));
            aiVector3D tangent = pMesh->mTangents ? meshTransformation * pMesh->mTangents[i] : aiVector3D(1.0f, 0.0f, 0.0f);
            aiVector3D texCoords = pMesh->mTextureCoords[0] ? pMesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
            
            FMeshVertex vertex(position.x, position.y, position.z,
                               color.r, color.g, color.b, color.a,
                               normal.x, normal.y, normal.z,
                               tangent.x, tangent.y, tangent.z,
                               texCoords.x, meshData.bIsFlipY ? 1.0f - texCoords.y : texCoords.y);
            meshData.AddVertex(vertex, true, false);
        }
        meshData.RefreshSphereByAABB();

        for (unsigned int i = 0; i < pMesh->mNumFaces; ++i)
        {
            auto& face = pMesh->mFaces[i];
            for (size_t j = 0; j < 3; ++j)
            {
                meshData.AddIndex((unsigned int)face.mIndices[j]);
            }
        }

        return true;
    }
    static bool s_ProcessMesh(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, FMeshData& meshData)
    {
        aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform) * pNode->mTransformation;

        //1> Mesh Data
        for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
            if (s_ProcessMeshData(&nodeTransformation, pMesh, pScene, meshData))
                return true;
        }

        //2> Nodes Children
        for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
        {
            if (s_ProcessMesh(&nodeTransformation, pNode->mChildren[i], pScene, meshData))
                return true;
        }

        return false;
    }
    static void s_ProcessNode_MeshDatas(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, FMeshDataVector& aMeshDatas, bool isFlipY)
    {
        aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform) * pNode->mTransformation;

        //1> Meshes
        for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
            FMeshData meshData;
            meshData.bIsFlipY = isFlipY;
            if (s_ProcessMeshData(&nodeTransformation, pMesh, pScene, meshData))
                aMeshDatas.push_back(meshData);
        }

        //2> Nodes Children
        for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
        {
            s_ProcessNode_MeshDatas(&nodeTransformation, pNode->mChildren[i], pScene, aMeshDatas, isFlipY);
        }
    }


    bool FMeshDataLoader::LoadMeshData(const String& pathMesh, FMeshData& meshData, unsigned int eMeshParserFlags)
    {
        return LoadMeshData(pathMesh.c_str(), meshData, eMeshParserFlags);
    }
    bool FMeshDataLoader::LoadMeshData(const char* szPathMesh, FMeshData& meshData, unsigned int eMeshParserFlags)
    {
        //1> LoadAssetFileContent
        CharVector content;
        if (!FUtil::LoadAssetFileContent(szPathMesh, content))
        {
            F_LogError("*********************** FMeshDataLoader::LoadMeshData: 1> FUtil::LoadAssetFileContent failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //2> Assimp import
        Assimp::Importer import;
        const void* pBuffer = (const void*)content.data();
        const aiScene* pScene = import.ReadFileFromMemory(pBuffer, content.size(), static_cast<unsigned int>(eMeshParserFlags));
        if (pScene == nullptr)
        {
            F_LogError("*********************** FMeshDataLoader::LoadMeshData: 2> import.ReadFileFromMemory failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //3> Process
        aiMatrix4x4 identity;
        if (!s_ProcessMesh(&identity, pScene->mRootNode, pScene, meshData))
        {
            F_LogError("*********************** FMeshDataLoader::LoadMeshData: 3> s_ProcessMesh failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        return true;
    }

    bool FMeshDataLoader::LoadMeshDatas(const String& pathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags)
    {
        return LoadMeshDatas(pathMesh.c_str(), aMeshDatas, isFlipY, eMeshParserFlags);
    }
    bool FMeshDataLoader::LoadMeshDatas(const char* szPathMesh, FMeshDataVector& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags)
    {
        //1> LoadAssetFileContent
        CharVector content;
        if (!FUtil::LoadAssetFileContent(szPathMesh, content))
        {
            F_LogError("*********************** FMeshDataLoader::LoadMeshDatas: 1> FUtil::LoadAssetFileContent failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //2> Assimp import
        Assimp::Importer import;
        const void* pBuffer = (const void*)content.data();
        const aiScene* pScene = import.ReadFileFromMemory(pBuffer, content.size(), static_cast<unsigned int>(eMeshParserFlags));
        if (pScene == nullptr)
        {
            F_LogError("*********************** FMeshDataLoader::LoadMeshDatas: 2> import.ReadFileFromMemory failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //3> Process
        aiMatrix4x4 identity;
	    s_ProcessNode_MeshDatas(&identity, pScene->mRootNode, pScene, aMeshDatas, isFlipY);

        return true;
    }


    //FMeshData
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshData.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.texCoord = vertex.texCoord;
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }
    }
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshData.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.texCoord = vertex.texCoord;
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }
    }
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tex4(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tex4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshData.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tex4 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.texCoord = FVector4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }
    }
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tangent3Tex2(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshData.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tangent3Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = vertex.texCoord;
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }
    }
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Normal3Tangent3Tex4(FMeshData& meshData, std::vector<FVertex_Pos3Color4Normal3Tangent3Tex4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshData.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tangent3Tex4 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = FVector4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }
    }
    void FMeshDataLoader::ExtractIndexData(FMeshData& meshData, std::vector<uint32_t>& aIndices)
    {
        int count_index = (int)meshData.indices32.size();
        aIndices.clear();
        aIndices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            aIndices.push_back(meshData.indices32[i]);
        }
    }


    //FMeshDataPC
    void FMeshDataLoader::ExtractVertexData_Pos3Color4Tex2(FMeshDataPC& meshDataPC, std::vector<FVertex_Pos3Color4>& aVertices, bool bIsTransformLocal, const FMatrix4& mat4TransformLocal)
    {
        int count_vertex = (int)meshDataPC.vertices.size();
        aVertices.clear();
        aVertices.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertexPC& vertex = meshDataPC.vertices[i];
            FVertex_Pos3Color4 v;
            v.pos = vertex.pos;
            v.color = vertex.color;
            if (bIsTransformLocal)
            {
                v.pos = FMath::Transform(mat4TransformLocal, v.pos);
            }
            aVertices.push_back(v);
        }

    }
    void FMeshDataLoader::ExtractIndexData(FMeshDataPC& meshDataPC, std::vector<uint32_t>& aIndices)
    {
        int count_index = (int)meshDataPC.indices32.size();
        aIndices.clear();
        aIndices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            aIndices.push_back(meshDataPC.indices32[i]);
        }
    }

}; //LostPeterFoundation