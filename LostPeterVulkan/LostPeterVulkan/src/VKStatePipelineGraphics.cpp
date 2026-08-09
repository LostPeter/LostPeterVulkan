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

		, pDescriptorSetLayout(nullptr)

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

		, poDescriptorSetLayout(VK_NULL_HANDLE)
		, poPipelineLayout(VK_NULL_HANDLE)

		, poPipelineGraphics(VK_NULL_HANDLE)
		, poPipelineGraphics_WireFrame(VK_NULL_HANDLE)
		, poPipelineGraphics_WireFrame2(VK_NULL_HANDLE)

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
		VulkanWindow* pWindow = Base::GetWindowPtr();

		if (this->poPipelineGraphics_WireFrame2 != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipeline(this->poPipelineGraphics_WireFrame2);
		}
		this->poPipelineGraphics_WireFrame2 = VK_NULL_HANDLE;

		if (this->poPipelineGraphics_WireFrame != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipeline(this->poPipelineGraphics_WireFrame);
		}
		this->poPipelineGraphics_WireFrame = VK_NULL_HANDLE;

		if (this->poPipelineGraphics != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipeline(this->poPipelineGraphics);
		}
		this->poPipelineGraphics = VK_NULL_HANDLE;

		if (this->poPipelineLayout != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipelineLayout(this->poPipelineLayout);
		}
		this->poPipelineLayout = VK_NULL_HANDLE;
		
		if (this->poDescriptorSets.size() > 0)
		{
			pWindow->destroyVkDescriptorSets(this->poDescriptorSets);
		}
		this->poDescriptorSets.clear();

		if (this->poDescriptorSetLayout != VK_NULL_HANDLE)
		{
			pWindow->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout);
		}	
		this->poDescriptorSetLayout = VK_NULL_HANDLE;
	}

	bool VKStatePipelineGraphics::Init(DescriptorSetLayout* pDSL,
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
		this->pDescriptorSetLayout = pDSL;

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
		
		VulkanWindow* pWindow = Base::GetWindowPtr();

		VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
		if (!pDSL->HasLayout())
		{
			//1> DescriptorSetLayout
			String nameDescriptorSetLayout = "DescriptorSetLayout-" + this->name;
			this->poDescriptorSetLayout = pWindow->CreateDescriptorSetLayout(nameDescriptorSetLayout, &pDescriptorSetLayout->aLayouts);
			if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create descriptor set layout, name: " + nameDescriptorSetLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkDescriptorSetLayout = this->poDescriptorSetLayout;

			//2> PipelineLayout
			VkDescriptorSetLayoutVector aDescriptorSetLayout;
			aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
			String namePipelineLayout = "PipelineLayout-" + this->name;
			this->poPipelineLayout = pWindow->createVkPipelineLayout(namePipelineLayout, aDescriptorSetLayout);
			if (this->poPipelineLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline layout, name: " + namePipelineLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkPipelineLayout = this->poPipelineLayout;
		}
		else
		{
			vkDescriptorSetLayout = pDSL->poDescriptorSetLayout;
			vkPipelineLayout = pDSL->poPipelineLayout;
		}

		//3> DescriptorSets
		if (!pDSL->HasDescriptorSets())
		{
			String nameDescriptorSets = "DescriptorSets-" + this->name;
			pWindow->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, this->poDescriptorSets);
		}

		//4> Pipeline
		String namePipelineGraphics = "PipelineGraphics-" + this->name;
		this->poPipelineGraphics = pWindow->createVkGraphicsPipeline(namePipelineGraphics,
																	 pShaderVertex,
																	 pShaderTESC,
																	 pShaderTESE,
																	 pShaderGeom,
																	 pShaderFrag,
																	 typeVertex,
																	 tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																	 renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																	 primitiveTopology, frontFace, polygonMode, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																	 bDepthTest, bDepthWrite, depthCompareOp,
																	 bStencilEnabled, stencilOpFront, stencilOpBack,
																	 bBlendEnabled, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
																	 blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
																	 colorWriteMask, subpass);
		if (this->poPipelineGraphics == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline, name: " + namePipelineGraphics;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}

		String namePipelineGraphics_WireFrame = "PipelineGraphics-WireFrame-" + this->name;
		this->poPipelineGraphics_WireFrame = pWindow->createVkGraphicsPipeline(namePipelineGraphics_WireFrame,
																			   pShaderVertex,
																			   pShaderTESC,
																			   pShaderTESE,
																			   pShaderGeom,
																			   pShaderFrag,
																			   typeVertex,
																			   tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																			   renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																			   primitiveTopology, frontFace, VK_POLYGON_MODE_LINE, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																			   bDepthTest, bDepthWrite, depthCompareOp,
																			   bStencilEnabled, stencilOpFront, stencilOpBack,
																			   bBlendEnabled, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
																			   blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
																			   colorWriteMask, subpass);
		if (this->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline wire frame, name: " + namePipelineGraphics_WireFrame;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}

		return true;
	}
	bool VKStatePipelineGraphics::Init(DescriptorSetLayout* pDSL,
									   VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
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
		this->pDescriptorSetLayout = pDSL;

		this->aShaderStageCreateInfos = aShaderStageCreateInfos;

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
		
		VulkanWindow* pWindow = Base::GetWindowPtr();

		VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
		if (!pDSL->HasLayout())
		{
			//1> DescriptorSetLayout
			String nameDescriptorSetLayout = "DescriptorSetLayout-" + this->name;
			this->poDescriptorSetLayout = pWindow->CreateDescriptorSetLayout(nameDescriptorSetLayout, &pDSL->aLayouts);
			if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create descriptor set layout, name: " + nameDescriptorSetLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkDescriptorSetLayout = this->poDescriptorSetLayout;

			//2> PipelineLayout
			VkDescriptorSetLayoutVector aDescriptorSetLayout;
			aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
			String namePipelineLayout = "PipelineLayout-" + this->name;
			this->poPipelineLayout = pWindow->createVkPipelineLayout(namePipelineLayout, aDescriptorSetLayout);
			if (this->poPipelineLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline layout, name: " + namePipelineLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkPipelineLayout = this->poPipelineLayout;
		}
		else
		{
			vkDescriptorSetLayout = pDSL->poDescriptorSetLayout;
			vkPipelineLayout = pDSL->poPipelineLayout;
		}

		//3> DescriptorSets
		if (!pDSL->HasDescriptorSets())
		{
			String nameDescriptorSets = "DescriptorSets-" + this->name;
			pWindow->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, this->poDescriptorSets);
		}

		//4> Pipeline
		String namePipelineGraphics = "PipelineGraphics-" + this->name;
		this->poPipelineGraphics = pWindow->createVkGraphicsPipeline(namePipelineGraphics,
																	 aShaderStageCreateInfos,
																	 typeVertex,
																	 tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																	 renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																	 primitiveTopology, frontFace, polygonMode, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																	 bDepthTest, bDepthWrite, depthCompareOp,
																	 bStencilEnabled, stencilOpFront, stencilOpBack,
																	 bBlendEnabled, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
																	 blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
																	 colorWriteMask, subpass);
		if (this->poPipelineGraphics == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline, name: " + namePipelineGraphics;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}

		String namePipelineGraphics_WireFrame = "PipelineGraphics-WireFrame-" + this->name;
		this->poPipelineGraphics_WireFrame = pWindow->createVkGraphicsPipeline(namePipelineGraphics_WireFrame,
																			   aShaderStageCreateInfos,
																			   typeVertex,
																			   tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																			   renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																			   primitiveTopology, frontFace, VK_POLYGON_MODE_LINE, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																			   bDepthTest, bDepthWrite, depthCompareOp,
																			   bStencilEnabled, stencilOpFront, stencilOpBack,
																			   bBlendEnabled, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
																			   blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
																			   colorWriteMask, subpass);
		if (this->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline wire frame, name: " + namePipelineGraphics_WireFrame;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}
		return true;
	}
	bool VKStatePipelineGraphics::Init(DescriptorSetLayout* pDSL,
									   VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
									   FMeshVertexType typeVertex,
									   bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
									   VkRenderPass renderPass, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
									   VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
									   VkBool32 bDepthEnabled, VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
									   VkBool32 bStencilEnabled, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
									   const VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentState, uint32_t subpass /*= 0*/)
	{
		this->pDescriptorSetLayout = pDSL;

		this->aShaderStageCreateInfos = aShaderStageCreateInfos;

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

        this->poColorBlendAttachmentState = aColorBlendAttachmentState;

		this->poSubpass = subpass;
		
		VulkanWindow* pWindow = Base::GetWindowPtr();

		VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
		VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
		if (!pDSL->HasLayout())
		{
			//1> DescriptorSetLayout
			String nameDescriptorSetLayout = "DescriptorSetLayout-" + this->name;
			this->poDescriptorSetLayout = pWindow->CreateDescriptorSetLayout(nameDescriptorSetLayout, &pDSL->aLayouts);
			if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create descriptor set layout, name: " + nameDescriptorSetLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkDescriptorSetLayout = this->poDescriptorSetLayout;

			//2> PipelineLayout
			VkDescriptorSetLayoutVector aDescriptorSetLayout;
			aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
			String namePipelineLayout = "PipelineLayout-" + this->name;
			this->poPipelineLayout = pWindow->createVkPipelineLayout(namePipelineLayout, aDescriptorSetLayout);
			if (this->poPipelineLayout == VK_NULL_HANDLE)
			{
				String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline layout, name: " + namePipelineLayout;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
			vkPipelineLayout = this->poPipelineLayout;
		}
		else
		{
			vkDescriptorSetLayout = pDSL->poDescriptorSetLayout;
			vkPipelineLayout = pDSL->poPipelineLayout;
		}

		//3> DescriptorSets
		if (!pDSL->HasDescriptorSets())
		{
			String nameDescriptorSets = "DescriptorSets-" + this->name;
			pWindow->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, this->poDescriptorSets);
		}

		//4> Pipeline
		String namePipelineGraphics = "PipelineGraphics-" + this->name;
		this->poPipelineGraphics = pWindow->createVkGraphicsPipeline(namePipelineGraphics,
																	 aShaderStageCreateInfos,
																	 tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																	 Util_GetVkVertexInputBindingDescriptionVectorPtr(typeVertex), 
                                                                     Util_GetVkVertexInputAttributeDescriptionVectorPtr(typeVertex),
																	 renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																	 primitiveTopology, frontFace, polygonMode, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																	 bDepthTest, bDepthWrite, depthCompareOp,
																	 bStencilEnabled, stencilOpFront, stencilOpBack,
																	 aColorBlendAttachmentState, subpass);
		if (this->poPipelineGraphics == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline, name: " + namePipelineGraphics;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}

		String namePipelineGraphics_WireFrame = "PipelineGraphics-WireFrame-" + this->name;
		this->poPipelineGraphics_WireFrame = pWindow->createVkGraphicsPipeline(namePipelineGraphics_WireFrame,
																			   aShaderStageCreateInfos,
																			   tessellationIsUsed, tessellationFlags, tessellationPatchControlPoints,
																			   Util_GetVkVertexInputBindingDescriptionVectorPtr(typeVertex), 
                                                                     		   Util_GetVkVertexInputAttributeDescriptionVectorPtr(typeVertex),
																			   renderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
																			   primitiveTopology, frontFace, VK_POLYGON_MODE_LINE, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
																			   bDepthTest, bDepthWrite, depthCompareOp,
																			   bStencilEnabled, stencilOpFront, stencilOpBack,
																			   aColorBlendAttachmentState, subpass);
		if (this->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineGraphics::Init: Failed to create pipeline wire frame, name: " + namePipelineGraphics_WireFrame;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}
		return true;
	}

	void VKStatePipelineGraphics::CleanupSwapChain()
	{

	}
	void VKStatePipelineGraphics::RecreateSwapChain()
	{
		
	}

	void VKStatePipelineGraphics::BindState(VkCommandBuffer& commandBuffer, bool isWireFrame)
	{
		BindPipeline(commandBuffer, isWireFrame);

		BindDescriptorSet(commandBuffer);
	}
	void VKStatePipelineGraphics::BindState(VkCommandBuffer& commandBuffer, const VkDescriptorSetVector& vkDescriptorSetVector,bool isWireFrame)
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		BindPipeline(commandBuffer, isWireFrame);

		VkPipelineLayout vkPipelineLayout = this->poPipelineLayout;
		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			vkPipelineLayout = this->pDescriptorSetLayout->poPipelineLayout;
		}
		const VkDescriptorSet* pDescriptorSet = &vkDescriptorSetVector[pWindow->poSwapChainImageIndex];
		BindDescriptorSet(commandBuffer, vkPipelineLayout, pDescriptorSet);
	}
	void VKStatePipelineGraphics::BindState(VkCommandBuffer& commandBuffer, const VkDescriptorSet* pDescriptorSet, bool isWireFrame)
	{
		BindPipeline(commandBuffer, isWireFrame);

		VkPipelineLayout vkPipelineLayout = this->poPipelineLayout;
		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			vkPipelineLayout = this->pDescriptorSetLayout->poPipelineLayout;
		}
		BindDescriptorSet(commandBuffer, vkPipelineLayout, pDescriptorSet);
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


	void VKStatePipelineGraphics::BindPipeline(VkCommandBuffer& commandBuffer, bool isWireFrame)
	{
		VkPipeline vkPipeline = this->poPipelineGraphics;
		if (isWireFrame)
			vkPipeline = this->poPipelineGraphics_WireFrame;
		Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
	}
	void VKStatePipelineGraphics::BindPipeline(VkCommandBuffer& commandBuffer, VkPipeline vkPipeline)
	{
		Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);
	}
	
	void VKStatePipelineGraphics::BindDescriptorSet(VkCommandBuffer& commandBuffer)
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		VkPipelineLayout vkPipelineLayout = this->poPipelineLayout;
		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			vkPipelineLayout = this->pDescriptorSetLayout->poPipelineLayout;
		}

		VkDescriptorSet* pDescriptorSet = nullptr;
		if (this->poDescriptorSets.size() > 0)
		{
			pDescriptorSet = &this->poDescriptorSets[pWindow->poSwapChainImageIndex];
		}
		else if (this->pDescriptorSetLayout->poDescriptorSets.size() > 0)
		{
			pDescriptorSet = &this->pDescriptorSetLayout->poDescriptorSets[pWindow->poSwapChainImageIndex];
		}

		if (pDescriptorSet != nullptr &&
		    vkPipelineLayout != VK_NULL_HANDLE)
		{
			BindDescriptorSet(commandBuffer, vkPipelineLayout, pDescriptorSet);
		}
	}
	void VKStatePipelineGraphics::BindDescriptorSet(VkCommandBuffer& commandBuffer, const VkDescriptorSet* pDescriptorSet)
	{
		VkPipelineLayout vkPipelineLayout = this->poPipelineLayout;
		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			vkPipelineLayout = this->pDescriptorSetLayout->poPipelineLayout;
		}
		BindDescriptorSet(commandBuffer, vkPipelineLayout, pDescriptorSet);
	}
	void VKStatePipelineGraphics::BindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& vkPipelineLayout, const VkDescriptorSet* pDescriptorSet)
	{
		Base::GetWindowPtr()->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, pDescriptorSet, 0, nullptr);
	}


}; //LostPeterVulkan