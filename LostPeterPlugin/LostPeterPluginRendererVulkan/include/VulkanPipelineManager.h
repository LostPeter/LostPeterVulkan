/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_PIPELINE_MANAGER_H_
#define _VULKAN_PIPELINE_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanPipelineManager : public FSingleton<VulkanPipelineManager>
    {
    public:
        VulkanPipelineManager(VulkanDevice* pDevice);
        ~VulkanPipelineManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanPipelinePtrVector m_aPipeline;
        VulkanPipelinePtrMap m_mapPipeline;

    public:
        static VulkanPipelineManager& GetSingleton();
		static VulkanPipelineManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanPipelinePtrVector& GetPipelinePtrVector() const { return m_aPipeline; }
        F_FORCEINLINE VulkanPipelinePtrVector& GetPipelinePtrVector() { return m_aPipeline; }
        F_FORCEINLINE const VulkanPipelinePtrMap& GetPipelinePtrMap() const { return m_mapPipeline; }
        F_FORCEINLINE VulkanPipelinePtrMap& GetPipelinePtrMap() { return m_mapPipeline; }

    public:
        void Destroy();

    public:
        bool HasPipeline(const String& strName);
        VulkanPipeline* GetPipeline(const String& strName);
        bool AddPipeline(VulkanPipeline* pPipeline);

        VulkanPipeline* CreatePipelineGraphics(const String& namePipeline,
                                               VulkanRenderWindow* pRenderWindow,
                                               VulkanRenderPass* pRenderPass,
                                               VulkanShaderModulePtrVector& aShaderModule,
                                               VulkanPipelineLayout* pPipelineLayout,
                                               FMeshVertexType eMeshVertex,
                                               FRenderPrimitiveType eRenderPrimitive,
                                               RenderState* pRenderState);
        VulkanPipeline* CreatePipelineGraphics(const String& namePipeline,
                                               VulkanRenderWindow* pRenderWindow,
                                               VulkanRenderPass* pRenderPass,
                                               VulkanShaderModulePtrVector& aShaderModule,
                                               VulkanPipelineLayout* pPipelineLayout,
                                               VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                               VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                               FRenderPrimitiveType eRenderPrimitive,
                                               RenderState* pRenderState);
        VulkanPipeline* CreatePipelineGraphics(const String& namePipeline,
                                               const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                               bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                               VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                               VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                               VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                               VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                               VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                               VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                               VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                               VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                               VkColorComponentFlags colorWriteMask,
                                               VkSampleCountFlagBits msaaSamples);

        VulkanPipeline* CreatePipelineCompute(const String& namePipeline,
                                              VulkanShaderModule* pShaderModule,
                                              VulkanPipelineLayout* pPipelineLayout,
                                              VkPipelineCreateFlags flags);      

        void DeletePipeline(const String& strName);
        void DeletePipeline(VulkanPipeline* pPipeline);
        void DeletePipelineAll();
    };

}; //LostPeterPluginRendererVulkan

#endif