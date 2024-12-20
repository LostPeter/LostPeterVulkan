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
        VKRenderPassCull* m_pVKRenderPassCull;
        CullManager* m_pCullManager;

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
        VkDescriptorSetVector poDescriptorSet_HizDepthGenerates;

        ////////////////////////// Buffer ///////////////////////////////////
        //CullConstants
        CullConstants cullCB;
        VkBuffer poBuffer_CullCB;  
        VkDeviceMemory poBufferMemory_CullCB;

        //Camera Param
        FMatrix4 mat4VPLast;
        FVector3 aWorldFrustumCorners[MAX_FRUSTUM_CORNER_COUNT];
        FPlane aWorldFrustumPlanes[MAX_FRUSTUM_PLANE_COUNT];


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
        void destroyBufferCull();
        void destroyVkComputePipeline(VkPipeline& poPipeline);
        
        bool createBufferCull();
        bool createVkComputePipeline(const String& nameComputePipeline,
                                     const String& descriptorSetLayout,
                                     StringVector* pDescriptorSetLayoutNames,
                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                     const VkPipelineLayout& vkPipelineLayout,
                                     const VkShaderModule& vkShaderModule,
                                     VkPipeline& vkPipeline,
                                     VkDescriptorSet& vkDescriptorSet);
        bool createVkComputePipeline(const String& nameComputePipeline,
                                     const String& descriptorSetLayout,
                                     StringVector* pDescriptorSetLayoutNames,
                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                     const VkPipelineLayout& vkPipelineLayout,
                                     const VkShaderModule& vkShaderModule,
                                     VkPipeline& vkPipeline,
                                     int countDescriptorSets,
                                     VkDescriptorSetVector& vkDescriptorSets);

    public:
        virtual void CleanupSwapChain();

        virtual void Dispatch_Cull(VkCommandBuffer& commandBuffer);
        virtual void Dispatch_HizDepthGenerate(VkCommandBuffer& commandBuffer);

    public:
        virtual void UpdateMatrixVP();
        virtual void UpdateBuffer_Cull();

    public:
        virtual void UpdateDescriptorSet_CullClearArgs(BufferIndirectCommand* pCB_RenderArgs);
        virtual void UpdateDescriptorSet_CullFrustum(BufferCompute* pCB_CullObjects,
                                                     BufferIndirectCommand* pCB_RenderArgs,
                                                     BufferCompute* pCB_LodArgs,
                                                     BufferCompute* pCB_Result);
        virtual void UpdateDescriptorSet_CullFrustumDepthHiz(BufferCompute* pCB_CullObjects,
                                                             BufferIndirectCommand* pCB_RenderArgs,
                                                             BufferCompute* pCB_LodArgs,
                                                             BufferCompute* pCB_Result);
        virtual void UpdateDescriptorSet_CullFrustumDepthHizClip(BufferCompute* pCB_CullObjects,
                                                                 BufferIndirectCommand* pCB_RenderArgs,
                                                                 BufferCompute* pCB_LodArgs,
                                                                 BufferCompute* pCB_Result,
                                                                 BufferCompute* pCB_Clip);
        virtual void UpdateDescriptorSet_HizDepthGenerate(int mipmap0, int mipmap1);

    protected:
        virtual void updateDescriptorSet(VkDescriptorSet& descriptorSet,
                                         StringVector* poDescriptorSetLayoutNames,
                                         BufferCompute* pCB_CullObjects,
                                         BufferIndirectCommand* pCB_RenderArgs,
                                         BufferCompute* pCB_LodArgs,
                                         BufferCompute* pCB_Result,
                                         BufferCompute* pCB_Clip,
                                         int mipmap0 = 0,
                                         int mipmap1 = 1);
    };

}; //LostPeterVulkan

#endif