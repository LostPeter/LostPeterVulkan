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

#ifndef _PIPELINE_GRAPHICS_H_
#define _PIPELINE_GRAPHICS_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport PipelineGraphics : public Base
    {
    public:
        PipelineGraphics(const String& namePipelineGraphics);
        virtual ~PipelineGraphics();

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
        MultiRenderPass* pRenderPass;

    public:
        void Destroy();

        virtual void CleanupSwapChain();
    };  

}; //LostPeter

#endif