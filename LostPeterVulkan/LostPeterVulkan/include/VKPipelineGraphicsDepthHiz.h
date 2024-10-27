/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_GRAPHICS_DEPTH_HIZ_H_
#define _VK_PIPELINE_GRAPHICS_DEPTH_HIZ_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsDepthHiz : public Base
    {
    public:
        VKPipelineGraphicsDepthHiz(const String& namePipelineGraphics, VKRenderPassCull* pVKRenderPassCull);
        virtual ~VKPipelineGraphicsDepthHiz();

    public:
        VKRenderPassCull* m_pVKRenderPassCull;

        //PipelineGraphics-HizDepth
        String nameDescriptorSetLayout_HizDepth;
        StringVector* poDescriptorSetLayoutNames_HizDepth;
        VkDescriptorSetLayout poDescriptorSetLayout_HizDepth;
        VkPipelineLayout poPipelineLayout_HizDepth;
        VkPipeline poPipeline_HizDepth;
        VkDescriptorSetVector poDescriptorSets_HizDepth;
        
        VkDescriptorImageInfo imageInfo;

    public:
        void Destroy();

        virtual bool Init();

        virtual bool InitHizDepth(const String& descriptorSetLayout,
                                  StringVector* pDescriptorSetLayoutNames,
                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                  const VkPipelineLayout& vkPipelineLayout,
                                  const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        bool createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                      const String& descriptorSetLayout,
                                      const String& nameDescriptorSets,
                                      StringVector* pDescriptorSetLayoutNames,
                                      const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                      const VkPipelineLayout& vkPipelineLayout,
                                      const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                      VkPipeline& vkPipeline,
                                      VkDescriptorSetVector* pDescriptorSets);

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet_HizDepth();

    protected:
        virtual void updateDescriptorSets(VkDescriptorSetVector& vkDescriptorSets,
                                          StringVector* poDescriptorSetLayoutNames,
                                          VkBuffer vkBuffer_HizDepthCB,
                                          VkImageView vkImageView,
                                          VkSampler vkSampler);
    };  

}; //LostPeterVulkan

#endif