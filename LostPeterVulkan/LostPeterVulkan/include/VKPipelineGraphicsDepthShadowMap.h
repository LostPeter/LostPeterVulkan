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

#ifndef _VK_PIPELINE_GRAPHICS_DEPTH_SHADOW_H_
#define _VK_PIPELINE_GRAPHICS_DEPTH_SHADOW_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphicsDepthShadowMap : public Base
    {
    public:
        VKPipelineGraphicsDepthShadowMap(const String& namePipelineGraphics, VKRenderPassShadowMap* pVKRenderPassShadowMap);
        virtual ~VKPipelineGraphicsDepthShadowMap();

    public:
        VKRenderPassShadowMap* m_pVKRenderPassShadowMap;

        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;


    public:
        void Destroy();
        virtual bool Init(const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSets(const VkBufferVector& poBuffersObject);
    };  

}; //LostPeterVulkan

#endif