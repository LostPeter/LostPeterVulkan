/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_GRAPHICS_H_
#define _VK_PIPELINE_GRAPHICS_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineGraphics : public Base
    {
    public:
        VKPipelineGraphics(const String& namePipelineGraphics);
        virtual ~VKPipelineGraphics();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline_WireFrame;
        VkPipeline poPipeline;
        VkDescriptorSetVector poDescriptorSets;

        bool hasNextSubpass;
        bool isMultiView;
        String nameDescriptorSetLayout2;
        StringVector* poDescriptorSetLayoutNames2;
        VkDescriptorSetLayout poDescriptorSetLayout2;
        VkPipelineLayout poPipelineLayout2;
        VkPipeline poPipeline_WireFrame2;
        VkPipeline poPipeline2;
        VkDescriptorSetVector poDescriptorSets2;

        String nameDescriptorSetLayout_Cull;
        StringVector* poDescriptorSetLayoutNames_Cull;
        VkDescriptorSetLayout poDescriptorSetLayout_Cull;
        VkPipelineLayout poPipelineLayout_Cull;
        VkPipeline poPipeline_WireFrame_Cull;
        VkPipeline poPipeline_Cull;
        VkDescriptorSetVector poDescriptorSets_Cull;

        String nameRenderPass;
        VKMultiRenderPass* pRenderPass;

    public:
        void Destroy();

        virtual void CleanupSwapChain();
    };  

}; //LostPeterVulkan

#endif