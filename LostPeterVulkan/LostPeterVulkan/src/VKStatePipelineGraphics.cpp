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

#include "../include/VKStatePipelineGraphics.h"
#include "../include/VulkanWindow.h"
#include "../include/VKShader.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKTexture.h"

namespace LostPeterVulkan
{
	VKStatePipelineGraphics::VKStatePipelineGraphics(const String& nameState)
		: Base(nameState)

		, poDescriptorSetLayout(nullptr)

		, poTypeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)


	{

	}
    VKStatePipelineGraphics::~VKStatePipelineGraphics()
	{
		Destroy();
	}

	void VKStatePipelineGraphics::Destroy()
	{

	}

	bool VKStatePipelineGraphics::Init(DescriptorSetLayout* pDescriptorSetLayout,
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
									   VkColorComponentFlags colorWriteMask, uint32_t subpass /*= 0*/)
	{


		return true;
	}


	void VKStatePipelineGraphics::BindState(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineGraphics::UnBindState(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineGraphics::BindShader(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineGraphics::BindBufferUniforms(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineGraphics::BindTextures(VkCommandBuffer& commandBuffer)
	{

	}



}; //LostPeterVulkan