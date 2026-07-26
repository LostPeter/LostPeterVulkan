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

		, poShaderVertex(nullptr)
		, poShaderTESC(nullptr)
		, poShaderTESE(nullptr)
		, poShaderGeom(nullptr)
		, poShaderFrag(nullptr)

		, poTypeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
		, poTessellationIsUsed(false)
		, poTessellationFlags(0)
		, poTessellationPatchControlPoints(3)

		, poRenderPass(VK_NULL_HANDLE)

		, poPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
		, poFrontFace(VK_FRONT_FACE_CLOCKWISE)
		, poPolygonMode(VK_POLYGON_MODE_FILL)
		, poCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
		, poDepthBiasEnable(VK_FALSE)
		, poDepthBiasConstantFactor(0.0f)
		, poDepthBiasClamp(0.0f)
		, poDepthBiasSlopeFactor(0.0f)
		, poLineWidth(1.0f)

		, poDepthEnabled(VK_TRUE)
		, poDepthTest(VK_TRUE)
		, poDepthWrite(VK_TRUE)
		, poDepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL)

		, poStencilEnabled(VK_FALSE)

		, poBlendEnabled(VK_FALSE)
		, poBlendColorFactorSrc(VK_BLEND_FACTOR_ONE)
		, poBlendColorFactorDst(VK_BLEND_FACTOR_ZERO)
		, poBlendColorOp(VK_BLEND_OP_ADD)
		, poBlendAlphaFactorSrc(VK_BLEND_FACTOR_ONE)
		, poBlendAlphaFactorDst(VK_BLEND_FACTOR_ZERO)
		, poBlendAlphaOp(VK_BLEND_OP_ADD)

		, poColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)

		, poSubpass(0)

		, poPipelineLayout(VK_NULL_HANDLE)
		, poPipelineGraphics(VK_NULL_HANDLE)
		, poPipelineGraphics_WireFrame(VK_NULL_HANDLE)

	{
		this->poDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        this->poDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

        this->poStencilOpFront.failOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpFront.passOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpFront.depthFailOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpFront.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        this->poStencilOpFront.compareMask = 0;
        this->poStencilOpFront.writeMask = 0;
        this->poStencilOpFront.reference = 0;

        this->poStencilOpBack.failOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpBack.passOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpBack.depthFailOp = VK_STENCIL_OP_KEEP;
        this->poStencilOpBack.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        this->poStencilOpBack.compareMask = 0;
        this->poStencilOpBack.writeMask = 0;
        this->poStencilOpBack.reference = 0;
	}
    VKStatePipelineGraphics::~VKStatePipelineGraphics()
	{
		Destroy();
	}

	void VKStatePipelineGraphics::Destroy()
	{

	}

	bool VKStatePipelineGraphics::Init(DescriptorSetLayout* pDescriptorSetLayout,
									   VKShader* pShaderVertex,
									   VKShader* pShaderTESC,
									   VKShader* pShaderTESE,
									   VKShader* pShaderGeom,
									   VKShader* pShaderFrag,
									   FMeshVertexType typeVertex,
									   bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
									   VkRenderPass renderPass, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
									   VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
									   VkBool32 bDepthEnabled, VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
									   VkBool32 bStencilEnabled, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
									   VkBool32 bBlendEnabled, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
									   VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
									   VkColorComponentFlags colorWriteMask, uint32_t subpass /*= 0*/)
	{
		this->poDescriptorSetLayout = pDescriptorSetLayout;

		this->poShaderVertex = pShaderVertex;
		this->poShaderTESC = pShaderTESC;
		this->poShaderTESE = pShaderTESE;
		this->poShaderGeom = pShaderGeom;
		this->poShaderFrag = pShaderFrag;

		this->poTypeVertex = typeVertex;
		this->poTessellationIsUsed = tessellationIsUsed;
		this->poTessellationFlags = tessellationFlags;
		this->poTessellationPatchControlPoints = tessellationPatchControlPoints;

		this->poRenderPass = renderPass;
		this->poViewports = aViewports;
		this->poScissors = aScissors;
		this->poDynamicStates = aDynamicStates;

        this->poPrimitiveTopology = primitiveTopology;
        this->poFrontFace = frontFace;
        this->poPolygonMode = polygonMode;
        this->poCullModeFlagBits = cullMode;
        this->poDepthBiasEnable = depthBiasEnable;
        this->poDepthBiasConstantFactor = depthBiasConstantFactor;
        this->poDepthBiasClamp = depthBiasClamp;
        this->poDepthBiasSlopeFactor = depthBiasSlopeFactor;
        this->poLineWidth = lineWidth;

		this->poDepthEnabled = bDepthEnabled;
        this->poDepthTest = bDepthTest;
        this->poDepthWrite = bDepthWrite;
        this->poDepthCompareOp = depthCompareOp;

        this->poStencilEnabled = bStencilEnabled;
        this->poStencilOpFront = stencilOpFront;
        this->poStencilOpBack = stencilOpBack;

        this->poBlendEnabled = bBlendEnabled;
        this->poBlendColorFactorSrc = blendColorFactorSrc;
        this->poBlendColorFactorDst = blendColorFactorDst;
        this->poBlendColorOp = blendColorOp;
        this->poBlendAlphaFactorSrc = blendAlphaFactorSrc;
        this->poBlendAlphaFactorDst = blendAlphaFactorDst;
        this->poBlendAlphaOp = blendAlphaOp;

        this->poColorWriteMask = colorWriteMask;

		this->poSubpass = subpass;
		

		//1> PipelineLayout 


		//2> Pipeline



		return true;
	}

	void VKStatePipelineGraphics::CleanupSwapChain()
	{

	}
	void VKStatePipelineGraphics::RecreateSwapChain()
	{
		
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