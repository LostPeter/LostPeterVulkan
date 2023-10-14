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

namespace LostPeter
{
    class utilExport VKPipelineGraphics : public Base
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

        bool isMultiView;
        VkPipeline poPipeline_WireFrame2;
        VkPipeline poPipeline2;

        String nameRenderPass;
        VKMultiRenderPass* pRenderPass;

    public:
        void Destroy();

        virtual void CleanupSwapChain();
    };  

}; //LostPeter

#endif