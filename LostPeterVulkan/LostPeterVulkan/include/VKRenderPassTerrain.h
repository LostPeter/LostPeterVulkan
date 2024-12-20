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
        VKRenderPassTerrain(const String& nameRenderPass);
        virtual ~VKRenderPassTerrain();

    public:
        static const uint32 c_nMaxGridVertexCount;
        static const uint32 c_nInstanceGridVertexCount;

    public:
        //Terrain
        uint8* poTerrainHeightMapData;
        float* poTerrainHeightMapDataFloat;
        int32 poTerrainHeightMapDataSize;
        int32 poTerrainHeightMapSize;

        bool poTerrainInstanceIsDraw;
        int32 poTerrainInstanceCount;

        //Mesh Whole
        std::vector<FVertex_Pos3Color4Normal3Tex2> poTerrain_Pos3Color4Normal3Tex2;
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

        //Mesh Instance
        std::vector<FVertex_Pos3Color4Normal3Tex2> poTerrain_Pos3Color4Normal3Tex2_Instance;
        uint32_t poTerrainVertexCount_Instance;
        size_t poTerrainVertexBuffer_Size_Instance;
        void* poTerrainVertexBuffer_Data_Instance;
        VkBuffer poTerrainVertexBuffer_Instance;
        VkDeviceMemory poTerrainVertexBufferMemory_Instance;
        std::vector<uint32_t> poTerrain_Indices_Instance;
        uint32_t poTerrainIndexCount_Instance;
        size_t poTerrainIndexBuffer_Size_Instance;
        void* poTerrainIndexBuffer_Data_Instance;
        VkBuffer poTerrainIndexBuffer_Instance;
        VkDeviceMemory poTerrainIndexBufferMemory_Instance;

        //HeightMap
        VkImage poTerrainHeightMapImage;
        VkDeviceMemory poTerrainHeightMapImageMemory;
        VkImageView poTerrainHeightMapImageView;
        VkDescriptorImageInfo poTerrainHeightMapImageInfo_NoSampler;
        VkDescriptorImageInfo poTerrainHeightMapImageInfo_Sampler;
        //NormalMap
        VkImage poTerrainNormalMapImage;
        VkDeviceMemory poTerrainNormalMapImageMemory;
        VkImageView poTerrainNormalMapImageView;
        VkDescriptorImageInfo poTerrainNormalMapImageInfo_NoSampler;
        VkDescriptorImageInfo poTerrainNormalMapImageInfo_Sampler;
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
        virtual void setupTerrainGeometryWhole();
        virtual void setupTerrainGeometryInstance();
        virtual void setupTerrainTexture();

    public:
        void CleanupSwapChain();
        void RecreateSwapChain();

    public:
        float GetTerrainHeight(int x, int z, float heightStart, float heightMax);
        float GetTerrainHeight(const FVector3& vPos, float heightStart, float heightMax);
        float GetTerrainHeight(float x, float z, float heightStart, float heightMax);
    };

}; //LostPeterVulkan

#endif