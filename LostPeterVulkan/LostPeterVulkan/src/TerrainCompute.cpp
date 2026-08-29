/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-08-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/TerrainCompute.h"
#include "../include/VulkanWindow.h"
#include "../include/TerrainManager.h"
#include "../include/TerrainSetting.h"
#include "../include/TerrainChunked.h"
#include "../include/VKTexture.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKStatePipelineCompute.h"

namespace LostPeterVulkan
{
	const String TerrainCompute::s_nameDescriptorSetLayout = "TextureCopy-TextureCSR-TextureCSRW";
	const String TerrainCompute::s_nameShaderCompute = "comp_standard_compute_texgen_normalmap";
	DescriptorSetLayout* TerrainCompute::s_pDescriptorSetLayout = nullptr;
	VKShader* TerrainCompute::s_pShaderCompute = nullptr;

	void TerrainCompute::InitStatic()
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		//DescriptorSetLayout
		s_pDescriptorSetLayout = pWindow->FindDescriptorSetLayout_Internal(s_nameDescriptorSetLayout);
		if (s_pDescriptorSetLayout == nullptr)
		{
			F_LogError("*********************** TerrainCompute::InitStatic: Can not find DescriptorSetLayout: [%s] !", s_nameDescriptorSetLayout.c_str());
		}

		//Shader
		s_pShaderCompute = pWindow->FindShader_Internal(s_nameShaderCompute);
		if (s_pShaderCompute == nullptr)
		{
			F_LogError("*********************** TerrainCompute::InitStatic: Can not find Shader: [%s] !", s_nameShaderCompute.c_str());
		}
	}
	void TerrainCompute::DestroyStatic()
	{

	}


	TerrainCompute::TerrainCompute(const String& nameCompute)
		: Base(nameCompute)

		, pChunked(nullptr)

        , poStatePipelineCompute(nullptr)
        
        , pTextureCopy(nullptr)
        , poBuffer_TextureCopy(nullptr)

        , isNormalUpdated(false)
        , isNormalUpdated_Sustained(true)

	{

	}
    TerrainCompute::~TerrainCompute()
	{
		Destroy();
	}

	void TerrainCompute::Destroy()
	{
		CleanupSwapChain();
		destroyBufferTextureCopy();
	}
		void TerrainCompute::destroyBufferTextureCopy()
		{
			F_DELETE(this->pTextureCopy)
			F_DELETE(this->poBuffer_TextureCopy)
		}

	bool TerrainCompute::Init(TerrainChunked* pChunked)
	{
		this->pChunked = pChunked;

		//1> Buffer
        if (this->pTextureCopy == nullptr)
        {
			int nHeightMapSize = TerrainSetting::GetSingleton().GetResolution();
            if (!createBufferTextureCopy(nHeightMapSize))
            {
                F_LogError("*********************** TerrainCompute::Init: createBufferTextureCopy failed !");
                return false;
            }
        }

        //2> Pipeline
		String namePipelineCompute = "PipelineCompute-Terrain-" + this->name;
		this->poStatePipelineCompute = Base::GetWindowPtr()->createStatePipelineCompute(namePipelineCompute,
																						s_pDescriptorSetLayout,
																						s_pShaderCompute,
																						false);

        if (this->poStatePipelineCompute == nullptr)
        {
            F_LogError("*********************** TerrainCompute::Init: createStatePipelineCompute: [%s] failed !", namePipelineCompute.c_str());
            return false;
        }

        //3> DescriptorSet
        UpdateDescriptorSet();

		return true;
	}
		bool TerrainCompute::createBufferTextureCopy(int nHeightMapSize)
		{
			destroyBufferTextureCopy();
            this->pTextureCopy = new TextureCopyConstants();
            this->pTextureCopy->texInfo.x = (float)nHeightMapSize;
            this->pTextureCopy->texInfo.y = (float)nHeightMapSize;
            VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
			String nameBuffer = "TextureCopyConstants-" + this->name;
			this->poBuffer_TextureCopy = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																				   sizeof(TextureCopyConstants),
																				   (uint8*)this->pTextureCopy,
																				   false);
			if (!this->poBuffer_TextureCopy)
			{
				String msg = "*********************** TerrainCompute::createBufferTextureCopy: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
		}

	void TerrainCompute::CleanupSwapChain()
	{
		F_DELETE(this->poStatePipelineCompute)
	}
    void TerrainCompute::UpdateDescriptorSet()
	{
		VulkanWindow* pWindow = Base::GetWindowPtr();

		VkWriteDescriptorSetVector descriptorWrites;
        size_t count = this->poStatePipelineCompute->pDescriptorSetLayout->aLayouts.size();
        for (size_t i = 0; i < count; i++)
        {
            const String& nameDescriptor = this->poStatePipelineCompute->pDescriptorSetLayout->aLayouts[i];

            if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCopy)) //TextureCopy
            {
                VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
                bufferInfo_TextureCopy.buffer = this->poBuffer_TextureCopy->GetVkBuffer();
                bufferInfo_TextureCopy.offset = 0;
                bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);
                pWindow->pushVkDescriptorSet_Uniform(descriptorWrites,
													 this->poStatePipelineCompute->poDescriptorSet,
													 (uint32_t)i,
													 0,
													 1,
													 bufferInfo_TextureCopy);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineCompute->poDescriptorSet,
												   (uint32_t)i,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
												   this->pChunked->pTexture_HeightMap->GetVkDescriptorImageInfo());
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
            {
                pWindow->pushVkDescriptorSet_Image(descriptorWrites,
												   this->poStatePipelineCompute->poDescriptorSet,
												   (uint32_t)i,
												   0,
												   1,
												   VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
												   this->pChunked->pTexture_NormalMap->GetVkDescriptorImageInfo());
            }
            else
            {
                String msg = "*********************** TerrainCompute::UpdateDescriptorSet: Wrong DescriptorSetLayout type: " + nameDescriptor;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        pWindow->updateVkDescriptorSets(descriptorWrites);
	}

}; //LostPeterVulkan