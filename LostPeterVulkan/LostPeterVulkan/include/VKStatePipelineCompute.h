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

#ifndef _VK_STATE_PIPELINE_COMPUTE_H_
#define _VK_STATE_PIPELINE_COMPUTE_H_

#include "Base.h"

namespace LostPeterVulkan
{
	class vulkanExport VKStatePipelineCompute : public Base
    {
    public:
        VKStatePipelineCompute(const String& nameState);
        virtual ~VKStatePipelineCompute();

	public:
		DescriptorSetLayout* pDescriptorSetLayout;
		String nameDescriptorSetLayout;
		
		VKShader* poShaderCompute;
		VkPipelineShaderStageCreateInfo poShaderStageCreateInfo;

		VkPipelineCreateFlags poFlags;
		VkSpecializationInfo* poSpecializationInfo;

		VkDescriptorSetLayout poDescriptorSetLayout;
		VkDescriptorSetVector poDescriptorSets;
		VkDescriptorSet poDescriptorSet;
		VkPipelineLayout poPipelineLayout;

		VkPipeline poPipelineCompute;

	public:
		void Destroy();
		bool Init(DescriptorSetLayout* pDSL,
				  VKShader* pShaderCompute,
				  bool isDescriptorSets,
				  VkPipelineCreateFlags flags = 0,
                  VkSpecializationInfo* pSpecializationInfo = nullptr);
		bool Init(DescriptorSetLayout* pDSL,
				  const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
				  bool isDescriptorSets,
				  VkPipelineCreateFlags flags = 0);

	public:
		F_FORCEINLINE const VkPipelineLayout& GetVkPipelineLayout() const { return this->poPipelineLayout; }	
		F_FORCEINLINE const VkPipeline& GetVkPipeline() const { return this->poPipelineCompute; }	

	public:
		void BindState(VkCommandBuffer& commandBuffer);
		void UnBindState(VkCommandBuffer& commandBuffer);
		void BindShader(VkCommandBuffer& commandBuffer);
		void BindBufferUniforms(VkCommandBuffer& commandBuffer);
		void BindTextures(VkCommandBuffer& commandBuffer);

	};

}; //LostPeterVulkan

#endif