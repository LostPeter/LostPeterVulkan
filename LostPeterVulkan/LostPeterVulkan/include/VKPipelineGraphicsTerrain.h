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

#ifndef _VK_PIPELINE_GRAPHICS_TERRAIN_H_
#define _VK_PIPELINE_GRAPHICS_TERRAIN_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsTerrain : public Base
    {
    public:
        VKPipelineGraphicsTerrain(const String& namePipelineGraphics, VKRenderPassTerrain* pVKRenderPassTerrain);
        virtual ~VKPipelineGraphicsTerrain();

    public:
        VKRenderPassTerrain* m_pVKRenderPassTerrain;

        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;

        std::vector<TerrainObjectConstants> terrainObjectCBs;
        VkBuffer poBuffer_TerrainObjectCB;
        VkDeviceMemory poBufferMemory_TerrainObjectCB;

        std::vector<MaterialConstants> materialCBs;
        VkBuffer poBuffer_MaterialCB;
        VkDeviceMemory poBufferMemory_MaterialCB;

        TerrainConstants terrainCB;
        VkBuffer poBuffer_TerrainCB;
        VkDeviceMemory poBufferMemory_TerrainCB;

    public: 
        void Destroy();
        virtual bool Init(const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        void destroyBufferTerrainObject();
        void destroyBufferMaterial();
        void destroyBufferTerrain();
        
        bool createBufferTerrainObject();
        bool createBufferMaterial();
        bool createBufferTerrain();

    public: 
        virtual void CleanupSwapChain();
        
        virtual void UpdateDescriptorSets();
        virtual void UpdateBufferTerrain();
    };  

}; //LostPeterVulkan

#endif