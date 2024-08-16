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

namespace LostPeterVulkan
{
    VKRenderPassTerrain::VKRenderPassTerrain(const String& _nameRenderPass)
        //Window
        : Base(_nameRenderPass)

        //Terrain
        , poTerrainHeightMapData(nullptr)
        , poTerrainHeightMapDataFloat(nullptr)
        , poTerrainHeightMapDataSize(0)
        , poTerrainHeightMapSize(0)
        , poTerrainGridVertexCount(129)
        , poTerrainGridInstanceVertexCount(17)
        , poTerrainGridInstanceCount(0)
        , poTerrainIsDrawInstance(false)

        , poTerrainVertexCount(0)
        , poTerrainVertexBuffer_Size(0)
        , poTerrainVertexBuffer_Data(nullptr)
        , poTerrainVertexBuffer(VK_NULL_HANDLE)
        , poTerrainVertexBufferMemory(VK_NULL_HANDLE)
        , poTerrainIndexCount(0)
        , poTerrainIndexBuffer_Size(0)
        , poTerrainIndexBuffer_Data(nullptr)
        , poTerrainIndexBuffer(VK_NULL_HANDLE)
        , poTerrainIndexBufferMemory(VK_NULL_HANDLE)

        //HeightMap
        , poTerrainHeightMapImage(VK_NULL_HANDLE)
        , poTerrainHeightMapImageMemory(VK_NULL_HANDLE)
        , poTerrainHeightMapImageView(VK_NULL_HANDLE)
        //NormalMap
        , poTerrainNormalMapImage(VK_NULL_HANDLE)
        , poTerrainNormalMapImageMemory(VK_NULL_HANDLE)
        , poTerrainNormalMapImageView(VK_NULL_HANDLE)
        , poTerrainImageSampler(VK_NULL_HANDLE)

        //Diffuse
        , poTerrainDiffuseImage(VK_NULL_HANDLE)
        , poTerrainDiffuseImageMemory(VK_NULL_HANDLE)
        , poTerrainDiffuseImageView(VK_NULL_HANDLE)
        , poTerrainDiffuseImageSampler(VK_NULL_HANDLE)
        //Normal
        , poTerrainNormalImage(VK_NULL_HANDLE)
        , poTerrainNormalImageMemory(VK_NULL_HANDLE)
        , poTerrainNormalImageView(VK_NULL_HANDLE)
        , poTerrainNormalImageSampler(VK_NULL_HANDLE)
        //Control
        , poTerrainControlImage(VK_NULL_HANDLE)
        , poTerrainControlImageMemory(VK_NULL_HANDLE)
        , poTerrainControlImageView(VK_NULL_HANDLE)
        , poTerrainControlImageSampler(VK_NULL_HANDLE)
    {

    }
    VKRenderPassTerrain::~VKRenderPassTerrain()
    {
        Destroy();
    }   

    void VKRenderPassTerrain::Destroy()
    {
        F_DELETE_T(this->poTerrainHeightMapData)
        F_DELETE_T(this->poTerrainHeightMapDataFloat)
        this->poTerrainHeightMapDataSize = 0;
        this->poTerrainHeightMapSize = 0;

        //Vertex/Index   
        this->poTerrain_Pos3Color4Normal3Tex2.clear();
        if (this->poTerrainVertexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poTerrainVertexBuffer, this->poTerrainVertexBufferMemory);
        }
        this->poTerrainVertexBuffer = VK_NULL_HANDLE;
        this->poTerrainVertexBufferMemory = VK_NULL_HANDLE;
        this->poTerrainVertexCount = 0;
        this->poTerrainVertexBuffer_Size = 0;
        this->poTerrainVertexBuffer_Data = nullptr;

        this->poTerrain_Indices.clear();
        if (this->poTerrainIndexBuffer != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poTerrainIndexBuffer, this->poTerrainIndexBufferMemory);
        }
        this->poTerrainIndexBuffer = VK_NULL_HANDLE;
        this->poTerrainIndexBufferMemory = VK_NULL_HANDLE;
        this->poTerrainIndexCount = 0;
        this->poTerrainIndexBuffer_Size = 0;
        this->poTerrainIndexBuffer_Data = nullptr;

        //Texture
        //TerrainHeightMapImage
        if (this->poTerrainHeightMapImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poTerrainHeightMapImage, this->poTerrainHeightMapImageMemory, this->poTerrainHeightMapImageView);
        }
        this->poTerrainHeightMapImage = VK_NULL_HANDLE;
        this->poTerrainHeightMapImageMemory = VK_NULL_HANDLE;
        this->poTerrainHeightMapImageView = VK_NULL_HANDLE;
        //TerrainNormalMapImage
        if (this->poTerrainNormalMapImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poTerrainNormalMapImage, this->poTerrainNormalMapImageMemory, this->poTerrainNormalMapImageView);
        }
        this->poTerrainNormalMapImage = VK_NULL_HANDLE;
        this->poTerrainNormalMapImageMemory = VK_NULL_HANDLE;
        this->poTerrainNormalMapImageView = VK_NULL_HANDLE;
        if (this->poTerrainImageSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poTerrainImageSampler);
        }
        this->poTerrainImageSampler = VK_NULL_HANDLE;

        //TerrainDiffuseImage
        if (this->poTerrainDiffuseImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poTerrainDiffuseImage, this->poTerrainDiffuseImageMemory, this->poTerrainDiffuseImageView);
        }
        this->poTerrainDiffuseImage = VK_NULL_HANDLE;
        this->poTerrainDiffuseImageMemory = VK_NULL_HANDLE;
        this->poTerrainDiffuseImageView = VK_NULL_HANDLE;
         if (this->poTerrainDiffuseImageSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poTerrainDiffuseImageSampler);
        }
        this->poTerrainDiffuseImageSampler = VK_NULL_HANDLE;

        //TerrainNormalImage
        if (this->poTerrainNormalImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poTerrainNormalImage, this->poTerrainNormalImageMemory, this->poTerrainNormalImageView);
        }
        this->poTerrainNormalImage = VK_NULL_HANDLE;
        this->poTerrainNormalImageMemory = VK_NULL_HANDLE;
        this->poTerrainNormalImageView = VK_NULL_HANDLE;
         if (this->poTerrainNormalImageSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poTerrainNormalImageSampler);
        }
        this->poTerrainNormalImageSampler = VK_NULL_HANDLE;

        //TerrainControlImage
        if (this->poTerrainControlImage != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImage(this->poTerrainControlImage, this->poTerrainControlImageMemory, this->poTerrainControlImageView);
        }
        this->poTerrainControlImage = VK_NULL_HANDLE;
        this->poTerrainControlImageMemory = VK_NULL_HANDLE;
        this->poTerrainControlImageView = VK_NULL_HANDLE;
         if (this->poTerrainControlImageSampler != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkImageSampler(this->poTerrainControlImageSampler);
        }
        this->poTerrainControlImageSampler = VK_NULL_HANDLE;
    } 

    bool VKRenderPassTerrain::Init()
    {
        if (loadTerrainData())
        {
            setupTerrainGeometry();
            setupTerrainTexture();

            return true;
        }
        return false;
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
            this->poTerrainGridInstanceCount = (nSize - 1) / (this->poTerrainGridInstanceVertexCount - 1);

            F_LogInfo("VKRenderPassTerrain::loadTerrainData: Load terrain data: [%s] success, heightmap data size: [%d], heightmap size: [%d] !", 
                      pathTerrain.c_str(), this->poTerrainHeightMapDataSize, this->poTerrainHeightMapSize);
            return true;
        }
        void VKRenderPassTerrain::setupTerrainGeometry()
        {   
            //1> Mesh Geometry
            FMeshData meshData;
            float fSize = (float)(this->poTerrainHeightMapSize - 1.0f);
            uint32 nSizeVertex = (uint32)(this->poTerrainHeightMapSize);
            uint32 nVertexCount = 0;
            float* pHeight = nullptr;
            uint32 heightDataGap = 1;
            if (this->poTerrainIsDrawInstance)
            {
                nVertexCount = (uint32)this->poTerrainGridInstanceVertexCount;
            }
            else
            {
                nVertexCount = nSizeVertex;
                if (nVertexCount > (uint32)this->poTerrainGridVertexCount)
                    nVertexCount = (uint32)this->poTerrainGridVertexCount;
                
                pHeight = this->poTerrainHeightMapDataFloat;
                heightDataGap = (nSizeVertex - 1) / (nVertexCount - 1);
            }
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

            F_LogInfo("VKRenderPassTerrain::setupTerrainGeometry: create terrain mesh: [Pos3Normal3Tex2]: Grid: [%d - %d], Vertex-Index: [%d - %d], Instance-Grid: [%d - %d] success !", 
                      nVertexCount, nVertexCount,
                      (int)this->poTerrain_Pos3Color4Normal3Tex2.size(), 
                      (int)this->poTerrain_Indices.size(),
                      (int)this->poTerrainGridInstanceCount,
                      (int)this->poTerrainGridInstanceVertexCount);

            //2> createVertexBuffer
            Base::GetWindowPtr()->createVertexBuffer(this->poTerrainVertexBuffer_Size, 
                                                     this->poTerrainVertexBuffer_Data, 
                                                     this->poTerrainVertexBuffer, 
                                                     this->poTerrainVertexBufferMemory);

            //3> createIndexBuffer
            Base::GetWindowPtr()->createIndexBuffer(this->poTerrainIndexBuffer_Size, 
                                                    this->poTerrainIndexBuffer_Data, 
                                                    this->poTerrainIndexBuffer, 
                                                    this->poTerrainIndexBufferMemory);
        }
        void VKRenderPassTerrain::setupTerrainTexture()
        {
            //Compute
            {
                //1> TerrainHeightMap Texture
                {
                    Base::GetWindowPtr()->createTextureRenderTarget2D(this->poTerrainHeightMapData,
                                                                      this->poTerrainHeightMapSize,
                                                                      this->poTerrainHeightMapSize,
                                                                      false,
                                                                      1,
                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                      VK_FORMAT_R16_UNORM,
                                                                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                                                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                                                      true,
                                                                      this->poTerrainHeightMapImage,
                                                                      this->poTerrainHeightMapImageMemory);
                    Base::GetWindowPtr()->createVkImageView(this->poTerrainHeightMapImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D, 
                                                            VK_FORMAT_R16_UNORM, 
                                                            VK_IMAGE_ASPECT_COLOR_BIT, 
                                                            1, 
                                                            1, 
                                                            this->poTerrainHeightMapImageView);
                    F_LogInfo("VKRenderPassTerrain::setupTerrainTexture: Compute: Create render texture [TerrainHeightMap] - [%d, %d] success !",
                              (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
                //2> TerrainNormalMap Texture
                {
                    Base::GetWindowPtr()->createTextureRenderTarget2D(nullptr,
                                                                      this->poTerrainHeightMapSize,
                                                                      this->poTerrainHeightMapSize,
                                                                      false,
                                                                      1,
                                                                      VK_SAMPLE_COUNT_1_BIT,
                                                                      VK_FORMAT_R8G8B8A8_UNORM,
                                                                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                                                      VK_IMAGE_LAYOUT_GENERAL,
                                                                      true,
                                                                      this->poTerrainNormalMapImage,
                                                                      this->poTerrainNormalMapImageMemory);
                    Base::GetWindowPtr()->createVkImageView(this->poTerrainNormalMapImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D, 
                                                            VK_FORMAT_R8G8B8A8_UNORM, 
                                                            VK_IMAGE_ASPECT_COLOR_BIT, 
                                                            1, 
                                                            1, 
                                                            this->poTerrainNormalMapImageView);
                    F_LogInfo("VKRenderPassTerrain::setupTerrainTexture: Compute: Create render texture [TerrainNormalMap] - [%d, %d] success !",
                              (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
                //3> Terrain ImageSampler
                {
                    Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                          F_TextureAddressing_Clamp,
                                                          F_TextureBorderColor_OpaqueBlack,
                                                          true,
                                                          Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                          0.0f,
                                                          1.0f,
                                                          0.0f,
                                                          this->poTerrainImageSampler);
                }
                //4> ImageInfo
                {
                    //TerrainHeightMap Texture
                    this->poTerrainHeightMapImageInfo_NoSampler = {};
                    this->poTerrainHeightMapImageInfo_NoSampler.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainHeightMapImageInfo_NoSampler.imageView = this->poTerrainHeightMapImageView;
                    this->poTerrainHeightMapImageInfo_NoSampler.sampler = nullptr;
                    this->poTerrainHeightMapImageInfo_Sampler = {};
                    this->poTerrainHeightMapImageInfo_Sampler.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainHeightMapImageInfo_Sampler.imageView = this->poTerrainHeightMapImageView;
                    this->poTerrainHeightMapImageInfo_Sampler.sampler = this->poTerrainImageSampler;

                    //TerrainNormalMap Texture
                    this->poTerrainNormalMapImageInfo_NoSampler = {};
                    this->poTerrainNormalMapImageInfo_NoSampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                    this->poTerrainNormalMapImageInfo_NoSampler.imageView = this->poTerrainNormalMapImageView;
                    this->poTerrainNormalMapImageInfo_NoSampler.sampler = nullptr;
                    this->poTerrainNormalMapImageInfo_Sampler = {};
                    this->poTerrainNormalMapImageInfo_Sampler.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                    this->poTerrainNormalMapImageInfo_Sampler.imageView = this->poTerrainNormalMapImageView;
                    this->poTerrainNormalMapImageInfo_Sampler.sampler = this->poTerrainImageSampler;
                }
            }

            //Graphics
            {
                uint32_t mipMapCount = 1;
                //1> Terrain Diffuse
                {
                    StringVector aPathTextureDiffuse = FUtilString::Split(Base::GetWindowPtr()->cfg_terrainTextureDiffuse_Path, ";");
                    Base::GetWindowPtr()->createTexture2DArray(aPathTextureDiffuse, 
                                                               VK_IMAGE_TYPE_2D,
                                                               VK_SAMPLE_COUNT_1_BIT, 
                                                               VK_FORMAT_R8G8B8A8_SRGB, 
                                                               true, 
                                                               mipMapCount, 
                                                               this->poTerrainDiffuseImage, 
                                                               this->poTerrainDiffuseImageMemory);
                    Base::GetWindowPtr()->createVkImageView(this->poTerrainDiffuseImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                            VK_FORMAT_R8G8B8A8_SRGB, 
                                                            VK_IMAGE_ASPECT_COLOR_BIT, 
                                                            mipMapCount, 
                                                            (int)aPathTextureDiffuse.size(), 
                                                            this->poTerrainDiffuseImageView);
                    Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                          F_TextureAddressing_Clamp,
                                                          F_TextureBorderColor_OpaqueBlack,
                                                          true,
                                                          Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                          0.0f,
                                                          static_cast<float>(mipMapCount),
                                                          0.0f,
                                                          this->poTerrainDiffuseImageSampler);

                    this->poTerrainDiffuseImageInfo = {};
                    this->poTerrainDiffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainDiffuseImageInfo.imageView = this->poTerrainDiffuseImageView;
                    this->poTerrainDiffuseImageInfo.sampler = this->poTerrainDiffuseImageSampler;

                    F_LogInfo("VKRenderPassTerrain::setupTerrainTexture: Graphics: Create terrain diffuse texture array: [%s] success !",
                              Base::GetWindowPtr()->cfg_terrainTextureDiffuse_Path.c_str());
                }
                //2> Terrain Normal
                {
                    StringVector aPathTextureNormal = FUtilString::Split(Base::GetWindowPtr()->cfg_terrainTextureNormal_Path, ";");
                    Base::GetWindowPtr()->createTexture2DArray(aPathTextureNormal, 
                                                               VK_IMAGE_TYPE_2D,
                                                               VK_SAMPLE_COUNT_1_BIT, 
                                                               VK_FORMAT_R8G8B8A8_UNORM, 
                                                               true, 
                                                               mipMapCount, 
                                                               this->poTerrainNormalImage, 
                                                               this->poTerrainNormalImageMemory);
                    Base::GetWindowPtr()->createVkImageView(this->poTerrainNormalImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                            VK_FORMAT_R8G8B8A8_UNORM, 
                                                            VK_IMAGE_ASPECT_COLOR_BIT, 
                                                            mipMapCount, 
                                                            (int)aPathTextureNormal.size(), 
                                                            this->poTerrainNormalImageView);
                    Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                          F_TextureAddressing_Clamp,
                                                          F_TextureBorderColor_OpaqueBlack,
                                                          true,
                                                          Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                          0.0f,
                                                          static_cast<float>(mipMapCount),
                                                          0.0f,
                                                          this->poTerrainNormalImageSampler);

                    this->poTerrainNormalImageInfo = {};
                    this->poTerrainNormalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainNormalImageInfo.imageView = this->poTerrainNormalImageView;
                    this->poTerrainNormalImageInfo.sampler = this->poTerrainNormalImageSampler;

                    F_LogInfo("VKRenderPassTerrain::setupTerrainTexture: Graphics: Create terrain normal texture array: [%s] success !",
                              Base::GetWindowPtr()->cfg_terrainTextureNormal_Path.c_str());
                }
                //3> Terrain Control
                {
                    StringVector aPathTextureControl = FUtilString::Split(Base::GetWindowPtr()->cfg_terrainTextureControl_Path, ";");
                    Base::GetWindowPtr()->createTexture2DArray(aPathTextureControl, 
                                                               VK_IMAGE_TYPE_2D,
                                                               VK_SAMPLE_COUNT_1_BIT, 
                                                               VK_FORMAT_R8G8B8A8_UNORM, 
                                                               true, 
                                                               mipMapCount, 
                                                               this->poTerrainControlImage, 
                                                               this->poTerrainControlImageMemory);
                    Base::GetWindowPtr()->createVkImageView(this->poTerrainControlImage, 
                                                            VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                                            VK_FORMAT_R8G8B8A8_UNORM, 
                                                            VK_IMAGE_ASPECT_COLOR_BIT, 
                                                            mipMapCount, 
                                                            (int)aPathTextureControl.size(), 
                                                            this->poTerrainControlImageView);
                    Base::GetWindowPtr()->createVkSampler(F_TextureFilter_Bilinear, 
                                                          F_TextureAddressing_Clamp,
                                                          F_TextureBorderColor_OpaqueBlack,
                                                          true,
                                                          Base::GetWindowPtr()->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                                          0.0f,
                                                          static_cast<float>(mipMapCount),
                                                          0.0f,
                                                          this->poTerrainControlImageSampler);

                    this->poTerrainControlImageInfo = {};
                    this->poTerrainControlImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainControlImageInfo.imageView = this->poTerrainControlImageView;
                    this->poTerrainControlImageInfo.sampler = this->poTerrainControlImageSampler;

                    F_LogInfo("VKRenderPassTerrain::setupTerrainTexture: Graphics: Create terrain control texture array: [%s] success !",
                              Base::GetWindowPtr()->cfg_terrainTextureControl_Path.c_str());
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

}; //LostPeterVulkan