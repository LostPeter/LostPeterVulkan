/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_STATE_PIPELINE_GRAPHICS_H_
#define _VK_STATE_PIPELINE_GRAPHICS_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport VKStatePipelineGraphics : public Base
    {
    public:
        VKStatePipelineGraphics(const String& nameState);
        virtual ~VKStatePipelineGraphics();

	public:
		DescriptorSetLayout* poDescriptorSetLayout;

		FMeshVertexType poTypeVertex;

		VkDynamicStateVector poDynamicStates;
        VkPrimitiveTopology poPrimitiveTopology;
        VkFrontFace poFrontFace;
        VkPolygonMode poPolygonMode;
        VkCullModeFlagBits poCullModeFlagBits;
        VkBool32 poDepthBiasEnable;
        float poDepthBiasConstantFactor;
        float poDepthBiasClamp;
        float poDepthBiasSlopeFactor;
        float poLineWidth;
		VkBool32 poDepthEnabled;
        VkBool32 poDepthTest;
        VkBool32 poDepthWrite; 
        VkCompareOp poDepthCompareOp; 
        VkBool32 poStencilEnabled;
        VkStencilOpState poStencilOpFront; 
        VkStencilOpState poStencilOpBack; 
        VkBool32 poBlendEnabled;
        VkBlendFactor poBlendColorFactorSrc; 
        VkBlendFactor poBlendColorFactorDst; 
        VkBlendOp poBlendColorOp;
        VkBlendFactor poBlendAlphaFactorSrc;
        VkBlendFactor poBlendAlphaFactorDst; 
        VkBlendOp poBlendAlphaOp;
        VkColorComponentFlags poColorWriteMask;

		VkRenderPass poRenderPass;

		VkShaderModule vertShaderModule;
		String vertMain;
		VkShaderModule tescShaderModule;
		String tescMain;
		VkShaderModule teseShaderModule; 
		String teseMain;
		VkShaderModule fragShaderModule;
		String fragMain;

		VkPipelineLayout poPipelineLayout;
        VkPipeline poPipelineGraphics;
        VkPipeline poPipelineGraphics_WireFrame;

	public:
		void Destroy();
		bool Init(DescriptorSetLayout* pDescriptorSetLayout,
				  VkShaderModule vertShaderModule, const String& vertMain,
				  VkShaderModule tescShaderModule, const String& tescMain,
				  VkShaderModule teseShaderModule, const String& teseMain,
				  VkShaderModule fragShaderModule, const String& fragMain,
				  FMeshVertexType typeVertex,
				  VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
				  VkRenderPass renderPass, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
				  VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
				  VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
				  VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
				  VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
				  VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
				  VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);

		

	public:
		


	public:
		void BindState(VkCommandBuffer& commandBuffer);
		void UnBindState(VkCommandBuffer& commandBuffer);
		void BindShader(VkCommandBuffer& commandBuffer);
		void BindBufferUniforms(VkCommandBuffer& commandBuffer);
		void BindTextures(VkCommandBuffer& commandBuffer);


	};

}; //LostPeterVulkan

#endif