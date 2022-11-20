#include "../include/preinclude.h"
#include "../include/meshloader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>

namespace LibUtil
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
            aiVector3D normal = meshTransformation * (pMesh->mNormals ? pMesh->mNormals[i] : aiVector3D(0.0f, 0.0f, 0.0f));
            aiVector3D tangent = pMesh->mTangents ? meshTransformation * pMesh->mTangents[i] : aiVector3D(0.0f, 0.0f, 0.0f);
            aiVector3D texCoords = pMesh->mTextureCoords[0] ? pMesh->mTextureCoords[0][i] : aiVector3D(0.0f, 0.0f, 0.0f);
            
            MeshVertex vertex(position.x, position.y, position.z,
                              color.r, color.g, color.b,
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
    static void s_ProcessNode_MeshDatas(void* pTransform, struct aiNode* pNode, const struct aiScene* pScene, std::vector<MeshData>& aMeshDatas)
    {
        aiMatrix4x4 nodeTransformation = *reinterpret_cast<aiMatrix4x4*>(pTransform) * pNode->mTransformation;

        //1> Meshes
        for (unsigned int i = 0; i < pNode->mNumMeshes; ++i)
        {
            aiMesh* pMesh = pScene->mMeshes[pNode->mMeshes[i]];
            MeshData meshData;
            if (s_ProcessMeshData(&nodeTransformation, pMesh, pScene, meshData))
                aMeshDatas.push_back(meshData);
        }

        //2> Nodes Children
        for (unsigned int i = 0; i < pNode->mNumChildren; ++i)
        {
            s_ProcessNode_MeshDatas(&nodeTransformation, pNode->mChildren[i], pScene, aMeshDatas);
        }
    }


    bool MeshLoader::LoadMeshData(const std::string& pathMesh, MeshData& meshData, unsigned int eMeshParserFlags)
    {
        return LoadMeshData(pathMesh.c_str(), meshData, eMeshParserFlags);
    }
    bool MeshLoader::LoadMeshData(const char* szPathMesh, MeshData& meshData, unsigned int eMeshParserFlags)
    {
        //1> LoadAssetFileContent
        std::vector<char> content;
        if (!VulkanUtil::LoadAssetFileContent(szPathMesh, content))
        {
            std::cout<< "*********************** VulkanMeshLoader::LoadMeshData: VulkanUtil::LoadAssetFileContent failed, mesh path: " << szPathMesh << std::endl;
            return false;
        }

        //2> Assimp import
        Assimp::Importer import;
        const void* pBuffer = (const void*)content.data();
        const aiScene* pScene = import.ReadFileFromMemory(pBuffer, content.size(), static_cast<unsigned int>(eMeshParserFlags));
        if (pScene == nullptr)
        {
            std::cout<< "*********************** VulkanMeshLoader::LoadMeshData: import.ReadFileFromMemory failed, mesh path: " << szPathMesh << std::endl;
            return false;
        }

        //3> Process
        aiMatrix4x4 identity;
        if (!s_ProcessMesh(&identity, pScene->mRootNode, pScene, meshData))
        {
            std::cout<< "*********************** VulkanMeshLoader::LoadMeshData: s_ProcessMesh failed, mesh path: " << szPathMesh << std::endl;
            return false;
        }

        return true;
    }

    bool MeshLoader::LoadMeshDatas(const std::string& pathMesh, std::vector<std::string>& aMaterials,  std::vector<MeshData>& aMeshDatas, unsigned int eMeshParserFlags)
    {
        return true;
    }
    bool MeshLoader::LoadMeshDatas(const char* szPathMesh, std::vector<std::string>& aMaterials,  std::vector<MeshData>& aMeshDatas, unsigned int eMeshParserFlags)
    {
        return true;
    }

}; //LibUtil