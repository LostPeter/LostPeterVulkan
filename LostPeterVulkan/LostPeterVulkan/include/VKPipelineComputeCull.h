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
		DescriptorSetLayout* pDescriptorSetLayout_CullClearArgs;
		VKStatePipelineCompute* poStatePipelineCompute_CullClearArgs;

        //PipelineCompute-CullFrustum
		DescriptorSetLayout* pDescriptorSetLayout_CullFrustum;
		VKStatePipelineCompute* poStatePipelineCompute_CullFrustum;

        //PipelineCompute-CullFrustumDepthHiz
		DescriptorSetLayout* pDescriptorSetLayout_CullFrustumDepthHiz;
		VKStatePipelineCompute* poStatePipelineCompute_CullFrustumDepthHiz;

        //PipelineCompute-CullFrustumDepthHizClip
		DescriptorSetLayout* pDescriptorSetLayout_CullFrustumDepthHizClip;
		VKStatePipelineCompute* poStatePipelineCompute_CullFrustumDepthHizClip;

        //PipelineCompute-HizDepthGenerate
		DescriptorSetLayout* pDescriptorSetLayout_HizDepthGenerate;
		VKStatePipelineCompute* poStatePipelineCompute_HizDepthGenerate;

        ////////////////////////// Buffer ///////////////////////////////////
        //CullConstants
        CullConstants cullCB;
        VKBufferUniform* poBuffer_CullCB;  

        //Camera Param
        FMatrix4 mat4VPLast;
        FVector3 aWorldFrustumCorners[MAX_FRUSTUM_CORNER_COUNT];
        FPlane aWorldFrustumPlanes[MAX_FRUSTUM_PLANE_COUNT];


    public:
        void Destroy();
        virtual bool Init();

        virtual bool InitCullClearArgs(DescriptorSetLayout* pDSL,
                          			   VKShader* pShader);
        virtual bool InitCullFrustum(DescriptorSetLayout* pDSL,
                          			 VKShader* pShader);
        virtual bool InitCullFrustumDepthHiz(DescriptorSetLayout* pDSL,
                          					 VKShader* pShader);
        virtual bool InitCullFrustumDepthHizClip(DescriptorSetLayout* pDSL,
                          						 VKShader* pShader);

        virtual bool InitHizDepthGenerate(DescriptorSetLayout* pDSL,
                          				  VKShader* pShader);

    protected:
        void destroyBufferCull();
        void destroyVkComputePipeline(VkPipeline& poPipeline);
        
        bool createBufferCull();

    public:
        virtual void CleanupSwapChain();

        virtual void Dispatch_Cull(VkCommandBuffer& commandBuffer);
        virtual void Dispatch_HizDepthGenerate(VkCommandBuffer& commandBuffer);

    public:
        virtual void UpdateMatrixVP();
        virtual void UpdateBuffer_Cull();

    public:
        virtual void UpdateDescriptorSet_CullClearArgs(VKBufferIndirectCommand* pCB_RenderArgs);
        virtual void UpdateDescriptorSet_CullFrustum(VKBufferCompute* pCB_CullObjects,
                                                     VKBufferIndirectCommand* pCB_RenderArgs,
                                                     VKBufferCompute* pCB_LodArgs,
                                                     VKBufferCompute* pCB_Result);
        virtual void UpdateDescriptorSet_CullFrustumDepthHiz(VKBufferCompute* pCB_CullObjects,
                                                             VKBufferIndirectCommand* pCB_RenderArgs,
                                                             VKBufferCompute* pCB_LodArgs,
                                                             VKBufferCompute* pCB_Result);
        virtual void UpdateDescriptorSet_CullFrustumDepthHizClip(VKBufferCompute* pCB_CullObjects,
                                                                 VKBufferIndirectCommand* pCB_RenderArgs,
                                                                 VKBufferCompute* pCB_LodArgs,
                                                                 VKBufferCompute* pCB_Result,
                                                                 VKBufferCompute* pCB_Clip);
        virtual void UpdateDescriptorSet_HizDepthGenerate(int mipmap0, int mipmap1);

    protected:
        virtual void updateDescriptorSet(VkDescriptorSet& descriptorSet,
										 VKStatePipelineCompute* pStatePipelineCompute,
                                         VKBufferCompute* pCB_CullObjects,
                                         VKBufferIndirectCommand* pCB_RenderArgs,
                                         VKBufferCompute* pCB_LodArgs,
                                         VKBufferCompute* pCB_Result,
                                         VKBufferCompute* pCB_Clip,
                                         int mipmap0 = 0,
                                         int mipmap1 = 1);
    };

}; //LostPeterVulkan

#endif