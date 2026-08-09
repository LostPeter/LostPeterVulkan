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

        //PipelineGraphics-ShadowMapDepth
		DescriptorSetLayout* pDescriptorSetLayout_ShadowMapDepth;
		VKStatePipelineGraphics* poStatePipelineGraphics_ShadowMapDepth;

        //PipelineGraphics-ShadowMapDepthCull
		DescriptorSetLayout* pDescriptorSetLayout_ShadowMapDepthCull;
		VKStatePipelineGraphics* poStatePipelineGraphics_ShadowMapDepthCull;

        //ObjectConstants
        std::vector<ObjectConstants> objectWorldCBs;
		VKBufferUniform* poBuffer_ObjectWorldCB;
		
    public:
        void Destroy();

        virtual bool Init();

        virtual bool InitShadowMapDepth(DescriptorSetLayout* pDSL,
                                        VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);
        virtual bool InitShadowMapDepthCull(DescriptorSetLayout* pDSL,
                                            VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        virtual void destroyBufferObjectWorldCB();

        virtual bool createBufferObjectWorldCB();
        VKStatePipelineGraphics* createGraphicsPipeline(const String& nameGraphicsPipeline,
														DescriptorSetLayout* pDSL,
														VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    public:
        virtual void CleanupSwapChain();

        //ObjectWorld
        virtual void UpdateBuffer_ObjectWorld_Clear();
        virtual void UpdateBuffer_ObjectWorld_AddOne(const ObjectConstants& object);
        virtual void UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects);
		virtual void UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects, int count);
        virtual void UpdateBuffer_ObjectWorld_Update();

        virtual void UpdateDescriptorSet_ShadowMapDepth();

        //Cull Instance
        virtual void UpdateDescriptorSet_ShadowMapDepthCull(VkDescriptorSetVector* pDescriptorSets,
                                                            VKBufferUniform* pCB_CullInstance,
                                                            VKBufferCompute* pCB_CullObjectInstances,
                                                            VKBufferCompute* pCB_Result);


    protected:
        virtual void updateDescriptorSets(VKStatePipelineGraphics* pStatePipelineGraphics,
										  VkDescriptorSetVector& vkDescriptorSets,
                                          VKBufferUniform* pCB_ObjectWorld,
                                          VKBufferUniform* pCB_CullInstance,
                                          VKBufferCompute* pCB_CullObjectInstances,
                                          VKBufferCompute* pCB_Result);
    };  

}; //LostPeterVulkan

#endif