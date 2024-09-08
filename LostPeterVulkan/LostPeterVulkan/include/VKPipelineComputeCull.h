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
        //PipelineCompute-CullClearArgs
        String nameDescriptorSetLayout_CullClearArgs;
        StringVector* poDescriptorSetLayoutNames_CullClearArgs;
        VkDescriptorSetLayout poDescriptorSetLayout_CullClearArgs;
        VkPipelineLayout poPipelineLayout_CullClearArgs;
        VkPipeline poPipeline_CullClearArgs;
        VkDescriptorSet poDescriptorSet_CullClearArgs;

        //PipelineCompute-CullFrustum
        String nameDescriptorSetLayout_CullFrustum;
        StringVector* poDescriptorSetLayoutNames_CullFrustum;
        VkDescriptorSetLayout poDescriptorSetLayout_CullFrustum;
        VkPipelineLayout poPipelineLayout_CullFrustum;
        VkPipeline poPipeline_CullFrustum;
        VkDescriptorSet poDescriptorSet_CullFrustum;

        //PipelineCompute-CullFrustumDepthHiz
        String nameDescriptorSetLayout_CullFrustumDepthHiz;
        StringVector* poDescriptorSetLayoutNames_CullFrustumDepthHiz;
        VkDescriptorSetLayout poDescriptorSetLayout_CullFrustumDepthHiz;
        VkPipelineLayout poPipelineLayout_CullFrustumDepthHiz;
        VkPipeline poPipeline_CullFrustumDepthHiz;
        VkDescriptorSet poDescriptorSet_CullFrustumDepthHiz;

        //PipelineCompute-CullFrustumDepthHizClip
        String nameDescriptorSetLayout_CullFrustumDepthHizClip;
        StringVector* poDescriptorSetLayoutNames_CullFrustumDepthHizClip;
        VkDescriptorSetLayout poDescriptorSetLayout_CullFrustumDepthHizClip;
        VkPipelineLayout poPipelineLayout_CullFrustumDepthHizClip;
        VkPipeline poPipeline_CullFrustumDepthHizClip;
        VkDescriptorSet poDescriptorSet_CullFrustumDepthHizClip;

        
        //CullConstants
        CullConstants cullCB;
        VkBuffer poBuffer_CullCB;  
        VkDeviceMemory poBufferMemory_CullCB;

        //CullObjectConstants
        std::vector<CullObjectConstants> cullObjectCBs;
        VkBuffer poBuffer_CullObjectCB;
        VkDeviceMemory poBufferMemory_CullObjectCB;

        //


    public:
        void Destroy();
        virtual bool Init();

        virtual bool InitCullClearArgs(const String& descriptorSetLayout,
                                       StringVector* pDescriptorSetLayoutNames,
                                       const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                       const VkPipelineLayout& vkPipelineLayout,
                                       const VkShaderModule& vkShaderModule);
        virtual bool InitCullFrustum(const String& descriptorSetLayout,
                                     StringVector* pDescriptorSetLayoutNames,
                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                     const VkPipelineLayout& vkPipelineLayout,
                                     const VkShaderModule& vkShaderModule);
        virtual bool InitCullFrustumDepthHiz(const String& descriptorSetLayout,
                                             StringVector* pDescriptorSetLayoutNames,
                                             const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                             const VkPipelineLayout& vkPipelineLayout,
                                             const VkShaderModule& vkShaderModule);
        virtual bool InitCullFrustumDepthHizClip(const String& descriptorSetLayout,
                                                 StringVector* pDescriptorSetLayoutNames,
                                                 const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                 const VkPipelineLayout& vkPipelineLayout,
                                                 const VkShaderModule& vkShaderModule);

    protected:
        void destroyBufferCullObject();
        void destroyBufferCull();
        void destroyVkComputePipeline(VkPipeline& poPipeline);
        
        bool createBufferCull();
        bool createBufferCullObject();
        bool createVkComputePipeline(const String& descriptorSetLayout,
                                     StringVector* pDescriptorSetLayoutNames,
                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                     const VkPipelineLayout& vkPipelineLayout,
                                     const VkShaderModule& vkShaderModule,
                                     VkPipeline& vkPipeline,
                                     VkDescriptorSet& vkDescriptorSet);

    public:
        virtual void CleanupSwapChain();

        virtual void UpdateDescriptorSet(VkDescriptorSet& descriptorSet,
                                         StringVector* poDescriptorSetLayoutNames);

    };

}; //LostPeterVulkan

#endif