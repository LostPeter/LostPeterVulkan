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

#ifndef _VK_PIPELINE_COMPUTE_TERRAIN_H_
#define _VK_PIPELINE_COMPUTE_TERRAIN_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineComputeTerrain : public Base
    {
    public:
        VKPipelineComputeTerrain(const String& namePipelineCompute, VKRenderPassTerrain* pRenderPassTerrain);
        virtual ~VKPipelineComputeTerrain();

    public:
        VKRenderPassTerrain* m_pRenderPassTerrain;

        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;
        
        TextureCopyConstants* pTextureCopy;
        VkBuffer poBuffer_TextureCopy;  
        VkDeviceMemory poBufferMemory_TextureCopy;

    public:
        void Destroy();
        virtual bool Init(const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkShaderModule& vkShaderModule);

    protected:
        void destroyBufferTextureCopy();
        
        bool createBufferTextureCopy(int nHeightMapSize);

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet(VkDescriptorImageInfo& imageInfoSrc,
                                         VkDescriptorImageInfo& imageInfoDst);
    };

}; //LostPeterVulkan

#endif