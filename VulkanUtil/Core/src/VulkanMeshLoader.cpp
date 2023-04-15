/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanMeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LostPeter
{
    //Materials
    static void s_ProcessMaterials(const struct aiScene* pScene, std::vector<std::string>& aMaterials)
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
    
    //MeshData
	static bool s_ProcessMeshData(void* pTransform, struct aiMesh* pMesh, const struct aiScene* pScene, MeshData& meshData)
    {
        if (pMesh->mNumVertices <= 0)
            return false;

        aiMatrix4x4 meshTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform);
        for (unsigned int i = 0; i < pMesh->mNumVertices; ++i)
        {
            aiVector3D position	= meshTransformation * pMesh->mVertices[i];
            aiColor4D color = pMesh->mColors[0] ? pMesh->mColors[0][i] : aiColor4D(1.0f, 1.0f, 1.0f, 1.0f);
            aiVector3D normal = meshTransformation * (pMesh->mNormals ? pMesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 1.0f));
            aiVector3D tangent = pMesh->mTangents ? meshTransformation * pMesh->mTangents[i] : aiVector3D(1.0f, 0.0f, 0.0f);
            aiVector3D texCoords = pMesh->mTextureCoords[0] ? pMesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
            
            MeshVertex vertex(position.x, position.y, position.z,
                              color.r, color.g, color.b, color.a,
                              normal.x, normal.y, normal.z,
                              tangent.x, tangent.y, tangent.z,
                              texCoords.x, meshData.bIsFlipY ? 1.0f - texCoords.y : texCoords.y);
            meshData.AddVertex(vertex);
        }

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
    static bool s_ProcessMesh(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, MeshData& meshData)
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
    static void s_ProcessNode_MeshDatas(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, std::vector<MeshData>& aMeshDatas, bool isFlipY)
    {
        aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform) * pNode->mTransformation;

        //1> Meshes
        for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
            MeshData meshData;
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


    bool VulkanMeshLoader::LoadMeshData(const std::string& pathMesh, MeshData& meshData, unsigned int eMeshParserFlags)
    {
        return LoadMeshData(pathMesh.c_str(), meshData, eMeshParserFlags);
    }
    bool VulkanMeshLoader::LoadMeshData(const char* szPathMesh, MeshData& meshData, unsigned int eMeshParserFlags)
    {
        //1> LoadAssetFileContent
        std::vector<char> content;
        if (!VulkanUtil::LoadAssetFileContent(szPathMesh, content))
        {
            Util_LogError("*********************** VulkanMeshLoader::LoadMeshData: 1> VulkanUtil::LoadAssetFileContent failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //2> Assimp import
        Assimp::Importer import;
        const void* pBuffer = (const void*)content.data();
        const aiScene* pScene = import.ReadFileFromMemory(pBuffer, content.size(), static_cast<unsigned int>(eMeshParserFlags));
        if (pScene == nullptr)
        {
            Util_LogError("*********************** VulkanMeshLoader::LoadMeshData: 2> import.ReadFileFromMemory failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //3> Process
        aiMatrix4x4 identity;
        if (!s_ProcessMesh(&identity, pScene->mRootNode, pScene, meshData))
        {
            Util_LogError("*********************** VulkanMeshLoader::LoadMeshData: 3> s_ProcessMesh failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        return true;
    }

    bool VulkanMeshLoader::LoadMeshDatas(const std::string& pathMesh, std::vector<MeshData>& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags)
    {
        return LoadMeshDatas(pathMesh.c_str(), aMeshDatas, isFlipY, eMeshParserFlags);
    }
    bool VulkanMeshLoader::LoadMeshDatas(const char* szPathMesh, std::vector<MeshData>& aMeshDatas, bool isFlipY, unsigned int eMeshParserFlags)
    {
        //1> LoadAssetFileContent
        std::vector<char> content;
        if (!VulkanUtil::LoadAssetFileContent(szPathMesh, content))
        {
            Util_LogError("*********************** VulkanMeshLoader::LoadMeshDatas: 1> VulkanUtil::LoadAssetFileContent failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //2> Assimp import
        Assimp::Importer import;
        const void* pBuffer = (const void*)content.data();
        const aiScene* pScene = import.ReadFileFromMemory(pBuffer, content.size(), static_cast<unsigned int>(eMeshParserFlags));
        if (pScene == nullptr)
        {
            Util_LogError("*********************** VulkanMeshLoader::LoadMeshDatas: 2> import.ReadFileFromMemory failed, mesh path: [%s] !", szPathMesh);
            return false;
        }

        //3> Process
        aiMatrix4x4 identity;
	    s_ProcessNode_MeshDatas(&identity, pScene->mRootNode, pScene, aMeshDatas, isFlipY);

        return true;
    }

}; //LostPeter