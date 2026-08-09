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

#include "../include/VKStatePipelineCompute.h"
#include "../include/VulkanWindow.h"
#include "../include/VKShader.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKTexture.h"

namespace LostPeterVulkan
{
	VKStatePipelineCompute::VKStatePipelineCompute(const String& nameState)
		: Base(nameState)

		, pDescriptorSetLayout(nullptr)
		, nameDescriptorSetLayout("")

		, poShaderCompute(nullptr)

		, poFlags(0)
		, poSpecializationInfo(nullptr)

		, poDescriptorSetLayout(VK_NULL_HANDLE)
		, poDescriptorSet(VK_NULL_HANDLE)
		, poPipelineLayout(VK_NULL_HANDLE)

		, poPipelineCompute(VK_NULL_HANDLE)

	{
		
	}
    VKStatePipelineCompute::~VKStatePipelineCompute()
	{
		Destroy();
	}

	void VKStatePipelineCompute::Destroy()
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		if (this->poPipelineCompute != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipeline(this->poPipelineCompute);
		}
		this->poPipelineCompute = VK_NULL_HANDLE;

		if (this->poPipelineCompute != VK_NULL_HANDLE)
		{
			pWindow->destroyVkPipelineLayout(this->poPipelineLayout);
		}
		this->poPipelineCompute = VK_NULL_HANDLE;

		if (this->poPipelineCompute != VK_NULL_HANDLE)
		{
			pWindow->destroyVkDescriptorSet(this->poDescriptorSet);
		}
		this->poPipelineCompute = VK_NULL_HANDLE;

		if (this->poDescriptorSets.size() > 0)
		{
			Base::GetWindowPtr()->destroyVkDescriptorSets(this->poDescriptorSets);
		}
		this->poDescriptorSets.clear();
		
		if (this->poDescriptorSetLayout != VK_NULL_HANDLE)
		{
			pWindow->destroyVkDescriptorSetLayout(this->poDescriptorSetLayout);
		}
		this->poDescriptorSetLayout = VK_NULL_HANDLE;
	}

	bool VKStatePipelineCompute::Init(DescriptorSetLayout* pDSL,
				  					  VKShader* pShaderCompute,
									  bool isDescriptorSets,
									  VkPipelineCreateFlags flags /*= 0*/,
                  					  VkSpecializationInfo* pSpecializationInfo /*= nullptr*/)
	{
		this->pDescriptorSetLayout = pDSL;
		this->poShaderCompute = pShaderCompute;
		this->poFlags = flags;
		this->poSpecializationInfo = pSpecializationInfo;

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
				String msg = "*********************** VKStatePipelineCompute::Init: Failed to create descriptor set layout, name: " + nameDescriptorSetLayout;
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
				String msg = "*********************** VKStatePipelineCompute::Init: Failed to create pipeline layout, name: " + namePipelineLayout;
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

		//3> DescriptorSet
		if (isDescriptorSets)
		{
			if (!pDSL->HasDescriptorSets())
			{
				String nameDescriptorSets = "DescriptorSets-" + this->name;
				pWindow->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, this->poDescriptorSets);
			}
		}
		else
		{
			if (!pDSL->HasDescriptorSet())
			{
				String nameDescriptorSet = "DescriptorSet-" + this->name;
				pWindow->createVkDescriptorSet(nameDescriptorSet, vkDescriptorSetLayout, this->poDescriptorSet);
			}
		}
		
		//4> Pipeline
		String namePipelineCompute = "PipelineCompute-" + this->name;
		this->poPipelineCompute = pWindow->createVkComputePipeline(namePipelineCompute,
																   pShaderCompute,
																   vkPipelineLayout,
																   flags,
																   pSpecializationInfo);
		if (this->poPipelineCompute == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineCompute::Init: Failed to create pipeline, name: " + namePipelineCompute;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}														

		return true;
	}

	bool VKStatePipelineCompute::Init(DescriptorSetLayout* pDSL,
									  const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
									  bool isDescriptorSets,
									  VkPipelineCreateFlags flags /*= 0*/)
	{
		this->pDescriptorSetLayout = pDSL;
		this->poShaderStageCreateInfo = shaderStageCreateInfo;
		this->poFlags = flags;

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
				String msg = "*********************** VKStatePipelineCompute::Init: Failed to create descriptor set layout, name: " + nameDescriptorSetLayout;
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
				String msg = "*********************** VKStatePipelineCompute::Init: Failed to create pipeline layout, name: " + namePipelineLayout;
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
		if (isDescriptorSets)
		{
			if (!pDSL->HasDescriptorSets())
			{
				String nameDescriptorSets = "DescriptorSets-" + this->name;
				pWindow->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, this->poDescriptorSets);
			}
		}
		else
		{
			if (!pDSL->HasDescriptorSet())
			{
				String nameDescriptorSet = "DescriptorSet-" + this->name;
				pWindow->createVkDescriptorSet(nameDescriptorSet, vkDescriptorSetLayout, this->poDescriptorSet);
			}
		}

		//4> Pipeline
		String namePipelineCompute = "PipelineCompute-" + this->name;
		this->poPipelineCompute = pWindow->createVkComputePipeline(namePipelineCompute,
																   shaderStageCreateInfo,
																   vkPipelineLayout,
																   flags);
		if (this->poPipelineCompute == VK_NULL_HANDLE)
		{
			String msg = "*********************** VKStatePipelineCompute::Init: Failed to create pipeline, name: " + namePipelineCompute;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
		}			

		return true;
	}

	void VKStatePipelineCompute::BindState(VkCommandBuffer& commandBuffer)
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		pWindow->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->poPipelineCompute);

		VkPipelineLayout vkPipelineLayout = this->poPipelineLayout;
		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			vkPipelineLayout = this->pDescriptorSetLayout->poPipelineLayout;
		}

		VkDescriptorSet* pDescriptorSet = nullptr;
		if (this->poDescriptorSet != VK_NULL_HANDLE)
		{
			pDescriptorSet = &this->poDescriptorSet;
		}
		else if (this->poDescriptorSets.size() > 0)
		{
			pDescriptorSet = &this->poDescriptorSets[pWindow->poSwapChainImageIndex];
		}
		else if (this->pDescriptorSetLayout->poDescriptorSet != VK_NULL_HANDLE)
		{
			pDescriptorSet = &this->pDescriptorSetLayout->poDescriptorSets[pWindow->poSwapChainImageIndex];
		}

		if (pDescriptorSet != nullptr &&
		    vkPipelineLayout != VK_NULL_HANDLE)
		{
			pWindow->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, vkPipelineLayout, 0, 1, pDescriptorSet, 0, nullptr);
		}
	}
	void VKStatePipelineCompute::UnBindState(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineCompute::BindShader(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineCompute::BindBufferUniforms(VkCommandBuffer& commandBuffer)
	{

	}
	void VKStatePipelineCompute::BindTextures(VkCommandBuffer& commandBuffer)
	{

	}

}; //LostPeterVulkan