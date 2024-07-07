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

#ifndef _VK_RENDER_PASS_TERRAIN_H_
#define _VK_RENDER_PASS_TERRAIN_H_

#include "Base.h"
#include "VKFrameBufferAttachment.h"

namespace LostPeterVulkan
{
    class vulkanExport VKRenderPassTerrain : public Base
    {
    public:
        VKRenderPassTerrain(const String& _nameRenderPass);
        virtual ~VKRenderPassTerrain();

    public:
        //Common
        String cfg_terrain_Path;
        String cfg_terrainTextureDiffuse_Path;
        String cfg_terrainTextureNormal_Path;
        String cfg_terrainTextureControl_Path;

        //Terrain
        uint8* poTerrainHeightMapData;
        float* poTerrainHeightMapDataFloat;
        int32 poTerrainHeightMapDataSize;
        int32 poTerrainHeightMapSize;
        int32 poTerrainGridVertexCount;
        int32 poTerrainGridInstanceVertexCount;
        int32 poTerrainGridInstanceCount;
        bool poTerrainIsDrawInstance;

        std::vector<FVertex_Pos3Normal3Tex2> poTerrain_Pos3Normal3Tex2;
        uint32_t poTerrainVertexCount;
        size_t poTerrainVertexBuffer_Size;
        void* poTerrainVertexBuffer_Data;
        VkBuffer poTerrainVertexBuffer;
        VkDeviceMemory poTerrainVertexBufferMemory;
        std::vector<uint32_t> poTerrain_Indices;
        uint32_t poTerrainIndexCount;
        size_t poTerrainIndexBuffer_Size;
        void* poTerrainIndexBuffer_Data;
        VkBuffer poTerrainIndexBuffer;
        VkDeviceMemory poTerrainIndexBufferMemory;

        //HeightMap
        VkImage poTerrainHeightMapImage;
        VkDeviceMemory poTerrainHeightMapImageMemory;
        VkImageView poTerrainHeightMapImageView;
        VkDescriptorImageInfo poTerrainHeightMapImageInfo;
        //NormalMap
        VkImage poTerrainNormalMapImage;
        VkDeviceMemory poTerrainNormalMapImageMemory;
        VkImageView poTerrainNormalMapImageView;
        VkDescriptorImageInfo poTerrainNormalMapImageInfo;
        VkSampler poTerrainImageSampler;

        //Diffuse
        VkImage poTerrainDiffuseImage;
        VkDeviceMemory poTerrainDiffuseImageMemory;
        VkImageView poTerrainDiffuseImageView;
        VkSampler poTerrainDiffuseImageSampler;
        VkDescriptorImageInfo poTerrainDiffuseImageInfo;
        //Normal
        VkImage poTerrainNormalImage;
        VkDeviceMemory poTerrainNormalImageMemory;
        VkImageView poTerrainNormalImageView;
        VkSampler poTerrainNormalImageSampler;
        VkDescriptorImageInfo poTerrainNormalImageInfo;
        //Control
        VkImage poTerrainControlImage;
        VkDeviceMemory poTerrainControlImageMemory;
        VkImageView poTerrainControlImageView;
        VkSampler poTerrainControlImageSampler;
        VkDescriptorImageInfo poTerrainControlImageInfo;


    public:
        void Destroy();

        virtual bool Init();

    protected:
        
        virtual bool loadTerrainData(); 
        virtual void setupTerrainGeometry();
        virtual void setupTerrainTexture();
    
    public:
        void CleanupSwapChain();
        void RecreateSwapChain();
    };

}; //LostPeterVulkan

#endif