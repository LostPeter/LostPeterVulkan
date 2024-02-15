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

#ifndef _VULKAN_PIPELINE_H_
#define _VULKAN_PIPELINE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanPipeline : public Base 
                         , public FNonCopyable
    {
        friend class VulkanPipelineManager;

    private:
        VulkanPipeline(const String& namePipeline, VulkanDevice* pDevice);
    public:
        ~VulkanPipeline();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkPipeline m_vkPipeline;

        bool m_bIsPipelineGraphics;
        VulkanShaderModulePtrVector m_aShaderModule;
        VulkanPipelineLayout* m_pPipelineLayout;

    public:
        F_FORCEINLINE VkPipeline GetVkPipeline() const { return m_vkPipeline; }

    public:
        void Destroy();

        bool InitPipelineGraphics(VulkanRenderWindow* pRenderWindow,
                                  VulkanRenderPass* pRenderPass,
                                  VulkanShaderModulePtrVector& aShaderModule,
                                  VulkanPipelineLayout* pPipelineLayout,
                                  FMeshVertexType eMeshVertex,
                                  FRenderPrimitiveType eRenderPrimitive,
                                  RenderState* pRenderState);
        bool InitPipelineGraphics(VulkanRenderWindow* pRenderWindow,
                                  VulkanRenderPass* pRenderPass,
                                  VulkanShaderModulePtrVector& aShaderModule,
                                  VulkanPipelineLayout* pPipelineLayout,
                                  VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                  VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                  FRenderPrimitiveType eRenderPrimitive,
                                  RenderState* pRenderState);
        bool InitPipelineGraphics(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
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
                                

        bool InitPipelineCompute(VulkanShaderModule* pShaderModule,
                                 VulkanPipelineLayout* pPipelineLayout,
                                 VkPipelineCreateFlags flags);
    };

}; //LostPeterPluginRendererVulkan

#endif