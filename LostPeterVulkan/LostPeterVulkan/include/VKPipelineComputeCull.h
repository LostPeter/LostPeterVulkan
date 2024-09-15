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
        VKPipelineComputeCull(const String& namePipelineCompute, VKRenderPassCull* pVKRenderPassCull);
        virtual ~VKPipelineComputeCull();

    public:
    

    public:
        VKRenderPassCull* m_pVKRenderPassCull;

        ////////////////////////// PipelineCompute //////////////////////////
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

        //PipelineCompute-HizDepthGenerate
        String nameDescriptorSetLayout_HizDepthGenerate;
        StringVector* poDescriptorSetLayoutNames_HizDepthGenerate;
        VkDescriptorSetLayout poDescriptorSetLayout_HizDepthGenerate;
        VkPipelineLayout poPipelineLayout_HizDepthGenerate;
        VkPipeline poPipeline_HizDepthGenerate;
        VkDescriptorSet poDescriptorSet_HizDepthGenerate;

        ////////////////////////// Buffer ///////////////////////////////////
        //CullConstants
        CullConstants cullCB;
        VkBuffer poBuffer_CullCB;  
        VkDeviceMemory poBufferMemory_CullCB;

        //CullObjectConstants
        CullObjectConstantsVector cullObjectCBs;
        VkBuffer poBuffer_CullObjectCB;
        VkDeviceMemory poBufferMemory_CullObjectCB;

        //HizDepthConstants
        HizDepthConstants hizDepthCB;
        VkBuffer poBuffer_HizDepthCB;  
        VkDeviceMemory poBufferMemory_HizDepthCB;

        //Args
        Uint32Vector aArgsCB;
        VkBuffer poBuffer_ArgsCB;  
        VkDeviceMemory poBufferMemory_ArgsCB;

        //Lod
        FloatVector aLodCB;
        VkBuffer poBuffer_LodCB;  
        VkDeviceMemory poBufferMemory_LodCB;

        //Result
        Uint32Vector aResultCB;
        VkBuffer poBuffer_ResultCB;  
        VkDeviceMemory poBufferMemory_ResultCB;

        //Clip
        Uint32Vector aClipCB;
        VkBuffer poBuffer_ClipCB;  
        VkDeviceMemory poBufferMemory_ClipCB;


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

        virtual bool InitHizDepthGenerate(const String& descriptorSetLayout,
                                          StringVector* pDescriptorSetLayoutNames,
                                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                          const VkPipelineLayout& vkPipelineLayout,
                                          const VkShaderModule& vkShaderModule);

    protected:
        void destroyBufferClip();
        void destroyBufferResult();
        void destroyBufferLod();
        void destroyBufferArgs();
        void destroyBufferHizDepth();
        void destroyBufferCullObject();
        void destroyBufferCull();
        void destroyVkComputePipeline(VkPipeline& poPipeline);
        
        bool createBufferCull();
        bool createBufferCullObject();
        bool createBufferHizDepth();
        bool createBufferArgs();
        bool createBufferLod();
        bool createBufferResult();
        bool createBufferClip();
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