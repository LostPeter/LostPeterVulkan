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

#ifndef _VK_PIPELINE_COMPUTE_CULL_H_
#define _VK_PIPELINE_COMPUTE_CULL_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKPipelineComputeCull : public Base
    {
    public:
        VKPipelineComputeCull(const String& namePipelineCompute);
        virtual ~VKPipelineComputeCull();

    public:
        String nameDescriptorSetLayout;
        StringVector* poDescriptorSetLayoutNames;
        VkDescriptorSetLayout poDescriptorSetLayout;
        VkPipelineLayout poPipelineLayout;
        VkPipeline poPipeline;
        VkDescriptorSet poDescriptorSet;


        
        CullConstants cullCB;
        VkBuffer poBuffer_CullCB;  
        VkDeviceMemory poBufferMemory_CullCB;

        std::vector<CullObjectConstants> cullObjectCBs;
        VkBuffer poBuffer_CullObjectCB;
        VkDeviceMemory poBufferMemory_CullObjectCB;


    public:
        void Destroy();
        virtual bool Init(const String& descriptorSetLayout,
                          StringVector* pDescriptorSetLayoutNames,
                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                          const VkPipelineLayout& vkPipelineLayout,
                          const VkShaderModule& vkShaderModule);

    protected:
        void destroyBufferCullObject();
        void destroyBufferCull();
        
        bool createBufferCull();
        bool createBufferCullObject();

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet();

    };

}; //LostPeterVulkan

#endif