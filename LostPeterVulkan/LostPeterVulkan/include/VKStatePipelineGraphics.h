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
		DescriptorSetLayout* pDescriptorSetLayout;

		VKShader* poShaderVertex;
		VKShader* poShaderTESC;
		VKShader* poShaderTESE;
		VKShader* poShaderGeom;
		VKShader* poShaderFrag;
		VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;

		FMeshVertexType poTypeVertex;
		bool poTessellationIsUsed;
		VkPipelineTessellationStateCreateFlags poTessellationFlags;
		uint32_t poTessellationPatchControlPoints;

		VkRenderPass poRenderPass;
		VkViewportVector poViewports;
		VkRect2DVector poScissors;
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

		VkPipelineColorBlendAttachmentStateVector poColorBlendAttachmentState;

		uint32_t poSubpass;

		VkDescriptorSetLayout poDescriptorSetLayout;
		VkDescriptorSetVector poDescriptorSets;
		VkPipelineLayout poPipelineLayout;

        VkPipeline poPipelineGraphics;
        VkPipeline poPipelineGraphics_WireFrame;
		VkPipeline poPipelineGraphics_WireFrame2;

	public:
		void Destroy();
		bool Init(DescriptorSetLayout* pDSL,
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
				  VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);
		bool Init(DescriptorSetLayout* pDSL,
				  VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
				  FMeshVertexType typeVertex,
				  bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
				  VkRenderPass renderPass, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
				  VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
				  VkBool32 bDepthEnabled, VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
				  VkBool32 bStencilEnabled, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
				  VkBool32 bBlendEnabled, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
				  VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
				  VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);
		bool Init(DescriptorSetLayout* pDSL,
				  VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
				  FMeshVertexType typeVertex,
				  bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
				  VkRenderPass renderPass, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
				  VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
				  VkBool32 bDepthEnabled, VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
				  VkBool32 bStencilEnabled, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
				  const VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentState, uint32_t subpass = 0);

		virtual void CleanupSwapChain();
		virtual void RecreateSwapChain(); 

	public:
		F_FORCEINLINE const VkPipelineLayout& GetVkPipelineLayout() const { return this->poPipelineLayout; }	
		F_FORCEINLINE const VkPipeline& GetVkPipeline() const { return this->poPipelineGraphics; }	
		F_FORCEINLINE const VkPipeline& GetVkPipeline_WireFrame() const { return this->poPipelineGraphics_WireFrame; }	

	public:
		void BindState(VkCommandBuffer& commandBuffer, bool isWireFrame);
		void BindState(VkCommandBuffer& commandBuffer, const VkDescriptorSetVector& vkDescriptorSetVector, bool isWireFrame);
		void BindState(VkCommandBuffer& commandBuffer, const VkDescriptorSet* pDescriptorSet, bool isWireFrame);
		void UnBindState(VkCommandBuffer& commandBuffer);
		void BindShader(VkCommandBuffer& commandBuffer);
		void BindBufferUniforms(VkCommandBuffer& commandBuffer);
		void BindTextures(VkCommandBuffer& commandBuffer);

	public:
		void BindPipeline(VkCommandBuffer& commandBuffer, bool isWireFrame);
		void BindPipeline(VkCommandBuffer& commandBuffer, VkPipeline vkPipeline);

		void BindDescriptorSet(VkCommandBuffer& commandBuffer);
		void BindDescriptorSet(VkCommandBuffer& commandBuffer, const VkDescriptorSet* pDescriptorSet);
		void BindDescriptorSet(VkCommandBuffer& commandBuffer, VkPipelineLayout& vkPipelineLayout, const VkDescriptorSet* pDescriptorSet);
	};

}; //LostPeterVulkan

#endif