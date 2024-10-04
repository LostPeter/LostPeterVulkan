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
        String nameDescriptorSetLayout_ShadowMapDepth;
        StringVector* poDescriptorSetLayoutNames_ShadowMapDepth;
        VkDescriptorSetLayout poDescriptorSetLayout_ShadowMapDepth;
        VkPipelineLayout poPipelineLayout_ShadowMapDepth;
        VkPipeline poPipeline_ShadowMapDepth;
        VkDescriptorSetVector poDescriptorSets_ShadowMapDepth;

        //PipelineGraphics-ShadowMapDepthCull
        String nameDescriptorSetLayout_ShadowMapDepthCull;
        StringVector* poDescriptorSetLayoutNames_ShadowMapDepthCull;
        VkDescriptorSetLayout poDescriptorSetLayout_ShadowMapDepthCull;
        VkPipelineLayout poPipelineLayout_ShadowMapDepthCull;
        VkPipeline poPipeline_ShadowMapDepthCull;

        //ObjectConstants
        std::vector<ObjectConstants> objectWorldCBs;
        VkBuffer poBuffer_ObjectWorldCB;
        VkDeviceMemory poBufferMemory_ObjectWorldCB;
        

    public:
        void Destroy();

        virtual bool Init();

        virtual bool InitShadowMapDepth(const String& descriptorSetLayout,
                                        StringVector* pDescriptorSetLayoutNames,
                                        const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                        const VkPipelineLayout& vkPipelineLayout,
                                        const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);
        virtual bool InitShadowMapDepthCull(const String& descriptorSetLayout,
                                            StringVector* pDescriptorSetLayoutNames,
                                            const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                            const VkPipelineLayout& vkPipelineLayout,
                                            const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos);

    protected:
        virtual void destroyBufferObjectWorldCB();

        virtual bool createBufferObjectWorldCB();
        bool createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                      const String& descriptorSetLayout,
                                      const String& nameDescriptorSets,
                                      StringVector* pDescriptorSetLayoutNames,
                                      const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                      const VkPipelineLayout& vkPipelineLayout,
                                      const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                      VkPipeline& vkPipeline,
                                      VkDescriptorSetVector* pDescriptorSets);

    public:
        virtual void CleanupSwapChain();

        //ObjectWorld
        virtual void UpdateBuffer_ObjectWorld_Clear();
        virtual void UpdateBuffer_ObjectWorld_AddOne(const ObjectConstants& object);
        virtual void UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects);
        virtual void UpdateBuffer_ObjectWorld_Update();

        virtual void UpdateDescriptorSet_ShadowMapDepth();

        //Cull Instance
        virtual void CreateDescriptorSet_ShadowMapDepthCull(const String& nameDescriptorSets, VkDescriptorSetVector& vkDescriptorSets);
        virtual void UpdateDescriptorSet_ShadowMapDepthCull(VkDescriptorSetVector* pescriptorSets,
                                                            BufferUniform* pCB_CullInstance,
                                                            BufferCompute* pCB_CullObjectInstances,
                                                            BufferCompute* pCB_Result);


    protected:
        virtual void updateDescriptorSets(VkDescriptorSetVector& vkDescriptorSets,
                                          StringVector* poDescriptorSetLayoutNames,
                                          VkBuffer vkBuffer_ObjectWorldCB,
                                          BufferUniform* pCB_CullInstance,
                                          BufferCompute* pCB_CullObjectInstances,
                                          BufferCompute* pCB_Result);
    };  

}; //LostPeterVulkan

#endif