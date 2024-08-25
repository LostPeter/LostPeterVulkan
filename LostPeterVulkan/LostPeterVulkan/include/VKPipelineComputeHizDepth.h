/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_PIPELINE_COMPUTE_HIZ_DEPTH_H_
#define _VK_PIPELINE_COMPUTE_HIZ_DEPTH_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineComputeHizDepth : public Base
    {
    public:
        VKPipelineComputeHizDepth(const String& namePipelineCompute);
        virtual ~VKPipelineComputeHizDepth();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;
        
        

    public:
        void Destroy();
        virtual bool Init(const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkShaderModule& vkShaderModule);

    protected:
        void destroyBufferCull();
        
        bool createBufferCull();

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet();
    };

}; //LostPeterVulkan

#endif