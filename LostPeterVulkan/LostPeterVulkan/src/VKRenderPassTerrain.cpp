/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-07-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKRenderPassTerrain.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainManager.h"
#include "../include/VKBufferVertexIndex.h"
#include "../include/VKTexture.h"

namespace LostPeterVulkan
{
    const uint32 VKRenderPassTerrain::c_nMaxGridVertexCount = 513;
    const uint32 VKRenderPassTerrain::c_nInstanceGridVertexCount = 17;
    
    VKRenderPassTerrain::VKRenderPassTerrain(const String& nameRenderPass)
        //Window
        : Base(nameRenderPass)

        //Terrain
        , poTerrainHeightMapData(nullptr)
        , poTerrainHeightMapDataFloat(nullptr)
        , poTerrainHeightMapDataSize(0)
        , poTerrainHeightMapSize(0)
        , poTerrainInstanceIsDraw(false)
        , poTerrainInstanceCount(0)

        //Mesh Whole
        , poTerrainVertexCount(0)
        , poTerrainVertexBuffer_Size(0)
        , poTerrainVertexBuffer_Data(nullptr)
        , poTerrainIndexCount(0)
        , poTerrainIndexBuffer_Size(0)
        , poTerrainIndexBuffer_Data(nullptr)
        , poBufferVertexIndex_MeshWhole(nullptr)

        //Mesh Instance
        , poTerrainVertexCount_Instance(0)
        , poTerrainVertexBuffer_Size_Instance(0)
        , poTerrainVertexBuffer_Data_Instance(nullptr)
        , poTerrainIndexCount_Instance(0)
        , poTerrainIndexBuffer_Size_Instance(0)
        , poTerrainIndexBuffer_Data_Instance(nullptr)
        , poBufferVertexIndex_MeshInstance(nullptr)

		//HeightMap/NormalMap
		, pTexture_HeightMap(nullptr)
		, pTexture_NormalMap(nullptr)

		//Diffuse/Normal/Control
		, pTexture_Diffuse(nullptr)
		, pTexture_Normal(nullptr)
		, pTexture_Control(nullptr)

    {

    }
    VKRenderPassTerrain::~VKRenderPassTerrain()
    {
        Destroy();
    }   

    void VKRenderPassTerrain::Destroy()
    {
		destroyTerrainTexture();
		destroyTerrainManager();


        F_DELETE_T(this->poTerrainHeightMapData)
        F_DELETE_T(this->poTerrainHeightMapDataFloat)
        this->poTerrainHeightMapDataSize = 0;
        this->poTerrainHeightMapSize = 0;

        //Mesh Whole
        this->poTerrain_Pos3Color4Normal3Tex2.clear();
        this->poTerrainVertexCount = 0;
        this->poTerrainVertexBuffer_Size = 0;
        this->poTerrainVertexBuffer_Data = nullptr;
        this->poTerrain_Indices.clear();
        this->poTerrainIndexCount = 0;
        this->poTerrainIndexBuffer_Size = 0;
        this->poTerrainIndexBuffer_Data = nullptr;
		F_DELETE(this->poBufferVertexIndex_MeshWhole)

        //Mesh Instance
        this->poTerrain_Pos3Color4Normal3Tex2_Instance.clear();
        this->poTerrainVertexCount_Instance = 0;
        this->poTerrainVertexBuffer_Size_Instance = 0;
        this->poTerrainVertexBuffer_Data_Instance = nullptr;
        this->poTerrain_Indices_Instance.clear();
        this->poTerrainIndexCount_Instance = 0;
        this->poTerrainIndexBuffer_Size_Instance = 0;
        this->poTerrainIndexBuffer_Data_Instance = nullptr;
		F_DELETE(this->poBufferVertexIndex_MeshInstance)

        
    } 
		void VKRenderPassTerrain::destroyTerrainManager()
		{
			F_DELETE(this->pTerrainManager)
		}
		void VKRenderPassTerrain::destroyTerrainTexture()
		{
			F_DELETE(this->pTexture_Diffuse)
			F_DELETE(this->pTexture_Normal)
			F_DELETE(this->pTexture_Control)

			F_DELETE(this->pTexture_NormalMap)
			F_DELETE(this->pTexture_HeightMap)
		}


    bool VKRenderPassTerrain::Init()
    {
		createTerrainManager();

        if (loadTerrainData())
        {
            setupTerrainGeometryWhole();
            setupTerrainGeometryInstance();
            createTerrainTexture();

            return true;
        }
        return false;
    }
		void VKRenderPassTerrain::createTerrainManager()
		{
			if (this->pTerrainManager != nullptr)
				return;

			this->pTerrainManager = new TerrainManager();
		}

        bool VKRenderPassTerrain::loadTerrainData()
        {
            const String& pathTerrain = Base::GetWindowPtr()->cfg_terrain_Path; 
            if (pathTerrain.empty())
                return false;

            this->poTerrainHeightMapData = nullptr;
            this->poTerrainHeightMapDataFloat = nullptr;
            this->poTerrainHeightMapDataSize = 0;
            this->poTerrainHeightMapSize = 0;
            if (!FUtil::LoadAssetFileToBuffer(pathTerrain.c_str(), &this->poTerrainHeightMapData, this->poTerrainHeightMapDataSize, false))
            {
                F_LogError("*********************** VKRenderPassTerrain::loadTerrainData failed, path: [%s] !", pathTerrain.c_str());
                return false;
            }
            this->poTerrainHeightMapSize = (int32)(FMath::Sqrt((float)this->poTerrainHeightMapDataSize / 2.0f));

            int nSize = this->poTerrainHeightMapSize;
            this->poTerrainHeightMapDataFloat = new float[nSize * nSize];
            for (int i = 0; i < nSize; i++)
            {
                for (int j = 0; j < nSize; j++)
                {
                    uint8* pStart = this->poTerrainHeightMapData + i * nSize * 2 + j * 2;
                    uint16 v = *((uint16*)pStart);
					this->poTerrainHeightMapDataFloat[i * nSize + j] = (float)v / 0xFFFF;
                }
            }
            this->poTerrainInstanceCount = (nSize - 1) / (c_nInstanceGridVertexCount - 1);

            F_LogInfo("VKRenderPassTerrain::loadTerrainData: Load terrain data: [%s] success, heightmap data size: [%d], heightmap size: [%d] !", 
                      pathTerrain.c_str(), this->poTerrainHeightMapDataSize, this->poTerrainHeightMapSize);
            return true;
        }
        void VKRenderPassTerrain::setupTerrainGeometryWhole()
        {   
			VulkanWindow* pWindow = Base::GetWindowPtr();

            //1> Mesh Geometry
            FMeshData meshData;
            float fSize = (float)(this->poTerrainHeightMapSize - 1.0f);
            uint32 nSizeVertex = (uint32)(this->poTerrainHeightMapSize);
            uint32 nVertexCount = nSizeVertex;
            if (nVertexCount > c_nMaxGridVertexCount)
                nVertexCount = c_nMaxGridVertexCount;
            float* pHeight = this->poTerrainHeightMapDataFloat;
            uint32 heightDataGap = (nSizeVertex - 1) / (nVertexCount - 1);
            FMeshGeometry::CreateEntityTerrain(meshData,
                                               0.0f,
                                               0.0f,
                                               fSize,
                                               fSize,
                                               nVertexCount,
                                               nVertexCount,
                                               pHeight,
                                               heightDataGap,
                                               false,
                                               false);

            int count_vertex = (int)meshData.vertices.size();
            this->poTerrain_Pos3Color4Normal3Tex2.clear();
            this->poTerrain_Pos3Color4Normal3Tex2.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tex2 v;
                v.pos = vertex.pos;
                v.color = vertex.color;
                v.normal = vertex.normal;
                v.texCoord = vertex.texCoord;
                this->poTerrain_Pos3Color4Normal3Tex2.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->poTerrain_Indices.clear();
            this->poTerrain_Indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->poTerrain_Indices.push_back(meshData.indices32[i]);
            }

            this->poTerrainVertexCount = (uint32_t)this->poTerrain_Pos3Color4Normal3Tex2.size();
            this->poTerrainVertexBuffer_Size = this->poTerrainVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
            this->poTerrainVertexBuffer_Data = &this->poTerrain_Pos3Color4Normal3Tex2[0];
            this->poTerrainIndexCount = (uint32_t)this->poTerrain_Indices.size();
            this->poTerrainIndexBuffer_Size = this->poTerrainIndexCount * sizeof(uint32_t);
            this->poTerrainIndexBuffer_Data = &this->poTerrain_Indices[0];

            F_LogInfo("VKRenderPassTerrain::setupTerrainGeometryWhole: create terrain mesh: [Pos3Normal3Tex2]: Grid: [%d - %d], Vertex-Index: [%d - %d] success !", 
                      nVertexCount, nVertexCount,
                      (int)this->poTerrain_Pos3Color4Normal3Tex2.size(), 
                      (int)this->poTerrain_Indices.size());

            //2> createBufferVertexIndex
			this->poBufferVertexIndex_MeshWhole = pWindow->createBufferVertexIndex("Vertex-Whole-" + this->name,
																				   F_MeshVertex_Pos3Color4Normal3Tex2,
																				   this->poTerrainVertexBuffer_Size, 
																				   (uint8*)this->poTerrainVertexBuffer_Data, 
																				   false,
																				   this->poTerrainIndexBuffer_Size, 
																				   (uint8*)this->poTerrainIndexBuffer_Data, 
																				   false,
																				   false);
			if (this->poBufferVertexIndex_MeshWhole == nullptr)
			{
				String msg = "*********************** VKRenderPassTerrain::setupTerrainGeometryWhole: Failed to create terrain vertex whole !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg.c_str());
			}
        }
        void VKRenderPassTerrain::setupTerrainGeometryInstance()
        {
			VulkanWindow* pWindow = Base::GetWindowPtr();

            //1> Mesh Geometry
            FMeshData meshData;
            float fSize = (float)(c_nInstanceGridVertexCount - 1.0f);
            uint32 nVertexCount = c_nInstanceGridVertexCount;
            float* pHeight = nullptr;
            uint32 heightDataGap = 1;
            FMeshGeometry::CreateEntityTerrain(meshData,
                                               0.0f,
                                               0.0f,
                                               fSize,
                                               fSize,
                                               nVertexCount,
                                               nVertexCount,
                                               pHeight,
                                               heightDataGap,
                                               false,
                                               false);

            int count_vertex = (int)meshData.vertices.size();
            this->poTerrain_Pos3Color4Normal3Tex2_Instance.clear();
            this->poTerrain_Pos3Color4Normal3Tex2_Instance.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                FMeshVertex& vertex = meshData.vertices[i];
                FVertex_Pos3Color4Normal3Tex2 v;
                v.pos = vertex.pos;
                v.color = vertex.color;
                v.normal = vertex.normal;
                v.texCoord = vertex.texCoord;
                this->poTerrain_Pos3Color4Normal3Tex2_Instance.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->poTerrain_Indices_Instance.clear();
            this->poTerrain_Indices_Instance.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->poTerrain_Indices_Instance.push_back(meshData.indices32[i]);
            }

            this->poTerrainVertexCount_Instance = (uint32_t)this->poTerrain_Pos3Color4Normal3Tex2_Instance.size();
            this->poTerrainVertexBuffer_Size_Instance = this->poTerrainVertexCount_Instance * sizeof(FVertex_Pos3Color4Normal3Tex2);
            this->poTerrainVertexBuffer_Data_Instance = &this->poTerrain_Pos3Color4Normal3Tex2_Instance[0];
            this->poTerrainIndexCount_Instance = (uint32_t)this->poTerrain_Indices_Instance.size();
            this->poTerrainIndexBuffer_Size_Instance = this->poTerrainIndexCount_Instance * sizeof(uint32_t);
            this->poTerrainIndexBuffer_Data_Instance = &this->poTerrain_Indices[0];

            F_LogInfo("VKRenderPassTerrain::setupTerrainGeometryInstance: create terrain mesh: [Pos3Normal3Tex2]: Grid: [%d - %d], Vertex-Index: [%d - %d], InstanceMax-Grid: [%d - %d] success !", 
                      nVertexCount, nVertexCount,
                      (int)this->poTerrain_Pos3Color4Normal3Tex2_Instance.size(), 
                      (int)this->poTerrain_Indices_Instance.size(),
                      (int)this->poTerrainInstanceCount,
                      (int)c_nInstanceGridVertexCount);

            //2> createBufferVertexIndex
			this->poBufferVertexIndex_MeshInstance = pWindow->createBufferVertexIndex("Vertex-Whole-" + this->name,
																					  F_MeshVertex_Pos3Color4Normal3Tex2,
																					  this->poTerrainVertexBuffer_Size_Instance, 
																					  (uint8*)this->poTerrainVertexBuffer_Data_Instance, 
																					  false,
																					  this->poTerrainIndexBuffer_Size_Instance, 
																					  (uint8*)this->poTerrainIndexBuffer_Data_Instance, 
																					  false,
																					  false);
			if (this->poBufferVertexIndex_MeshInstance == nullptr)
			{
				String msg = "*********************** VKRenderPassTerrain::setupTerrainGeometryInstance: Failed to create terrain vertex instance !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg.c_str());
			}
        }
        void VKRenderPassTerrain::createTerrainTexture()
        {
			VulkanWindow* pWindow = Base::GetWindowPtr();

            //Compute
            {
                //1> TerrainHeightMap Texture
                {
                    String nameTexture = "Texture-TerrainHeightMap";
					StringVector aPathTextureHeightMap;
					this->pTexture_HeightMap = new VKTexture(0,
														     nameTexture,
														     aPathTextureHeightMap,
														     F_Texture_2D,
														     F_TexturePixelFormat_R16_UNORM,
														     F_TextureFilter_Bilinear,
														     F_TextureAddressing_Clamp,
														     F_TextureBorderColor_OpaqueBlack,
														     true,
														     true);
					this->pTexture_HeightMap->rtImageUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
					this->pTexture_HeightMap->poTextureImageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					this->pTexture_HeightMap->LoadTexture(this->poTerrainHeightMapSize, 
														  this->poTerrainHeightMapSize,
														  1,
														  2,
														  this->poTerrainHeightMapData);
					
                    F_LogInfo("VKRenderPassTerrain::createTerrainTexture: Compute: Create render texture [TerrainHeightMap] - [%d, %d] success !",
                              (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
                //2> TerrainNormalMap Texture
                {
                    String nameTexture = "Texture-TerrainNormalMap";
					StringVector aPathTextureNormalMap;
					this->pTexture_NormalMap = new VKTexture(0,
														     nameTexture,
														     aPathTextureNormalMap,
														     F_Texture_2D,
														     F_TexturePixelFormat_R8G8B8A8_UNORM,
														     F_TextureFilter_Bilinear,
														     F_TextureAddressing_Clamp,
														     F_TextureBorderColor_OpaqueBlack,
														     true,
														     true);
					this->pTexture_NormalMap->rtImageUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
					this->pTexture_NormalMap->poTextureImageLayout = VK_IMAGE_LAYOUT_GENERAL;
					this->pTexture_NormalMap->LoadTexture(this->poTerrainHeightMapSize, 
														  this->poTerrainHeightMapSize,
														  1,
														  4,
														  nullptr);

                    F_LogInfo("VKRenderPassTerrain::createTerrainTexture: Compute: Create render texture [TerrainNormalMap] - [%d, %d] success !",
                              (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
            }

            //Graphics
            {
                uint32_t mipMapCount = 1;
                //1> Terrain Diffuse
                {
                    String nameTexture = "Texture-Terrain-Diffuse";
                    StringVector aPathTextureDiffuse = FUtilString::Split(pWindow->cfg_terrainTextureDiffuse_Path, ";");
					this->pTexture_Diffuse = new VKTexture(0,
														   nameTexture,
														   aPathTextureDiffuse,
														   F_Texture_2DArray,
														   F_TexturePixelFormat_R8G8B8A8_SRGB,
														   F_TextureFilter_Bilinear,
														   F_TextureAddressing_Clamp,
														   F_TextureBorderColor_OpaqueBlack,
														   false,
														   false);
					this->pTexture_Diffuse->LoadTexture(1024, 
														1024,
														1);

                    F_LogInfo("VKRenderPassTerrain::createTerrainTexture: Graphics: Create terrain diffuse texture array: [%s] success !",
                              pWindow->cfg_terrainTextureDiffuse_Path.c_str());
                }
                //2> Terrain Normal
                {
                    String nameTexture = "Texture-Terrain-Normal";
                    StringVector aPathTextureNormal = FUtilString::Split(pWindow->cfg_terrainTextureNormal_Path, ";");
					this->pTexture_Normal = new VKTexture(0,
														  nameTexture,
														  aPathTextureNormal,
														  F_Texture_2DArray,
														  F_TexturePixelFormat_R8G8B8A8_UNORM,
														  F_TextureFilter_Bilinear,
														  F_TextureAddressing_Clamp,
														  F_TextureBorderColor_OpaqueBlack,
														  false,
														  false);
					this->pTexture_Normal->LoadTexture(1024, 
														1024,
														1);

                    F_LogInfo("VKRenderPassTerrain::createTerrainTexture: Graphics: Create terrain normal texture array: [%s] success !",
                              pWindow->cfg_terrainTextureNormal_Path.c_str());
                }
                //3> Terrain Control
                {
                    String nameTexture = "Texture-Terrain-Control";
                    StringVector aPathTextureControl = FUtilString::Split(pWindow->cfg_terrainTextureControl_Path, ";");
					this->pTexture_Control = new VKTexture(0,
														   nameTexture,
														   aPathTextureControl,
														   F_Texture_2DArray,
														   F_TexturePixelFormat_R8G8B8A8_UNORM,
														   F_TextureFilter_Bilinear,
														   F_TextureAddressing_Clamp,
														   F_TextureBorderColor_OpaqueBlack,
														   false,
														   false);
					this->pTexture_Control->LoadTexture(1024, 
														1024,
														1);

                    F_LogInfo("VKRenderPassTerrain::createTerrainTexture: Graphics: Create terrain control texture array: [%s] success !",
                              pWindow->cfg_terrainTextureControl_Path.c_str());
                }
            }
        }

    void VKRenderPassTerrain::CleanupSwapChain()
    {
        Destroy();
    }
    
    void VKRenderPassTerrain::RecreateSwapChain()
    {

    }

    float VKRenderPassTerrain::GetTerrainHeight(int x, int z, float heightStart, float heightMax)
    {
        int size = this->poTerrainHeightMapSize;
        x = FMath::Clamp(x, 0, size - 1);
        z = FMath::Clamp(z, 0, size - 1);
        float fPerf = poTerrainHeightMapDataFloat[x + z * size];
        return heightStart + heightMax * fPerf;
    }
    float VKRenderPassTerrain::GetTerrainHeight(const FVector3& vPos, float heightStart, float heightMax)
    {
        return GetTerrainHeight(vPos.x, vPos.z, heightStart, heightMax);
    }
    float VKRenderPassTerrain::GetTerrainHeight(float x, float z, float heightStart, float heightMax)
    {
        float sizeX = this->poTerrainHeightMapSize - 1.0f;
        float sizeZ = this->poTerrainHeightMapSize - 1.0f;
        float xCoord = FMath::Clamp((x + sizeX / 2.0f), 0.0f, sizeX);
        float zCoord = FMath::Clamp((sizeZ / 2.0f - z), 0.0f, sizeZ);

        float fPerf = FMath::BiLinear(xCoord, zCoord,
                                      this->poTerrainHeightMapSize, this->poTerrainHeightMapSize,
                                      this->poTerrainHeightMapDataFloat);
        float fHeight = heightStart + heightMax * fPerf;
        return fHeight;
    }

}; //LostPeterVulkan