/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-07-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineComputeTerrain.h"
#include "../include/VKRenderPassTerrain.h"
#include "../include/VulkanWindow.h"
#include "../include/VKTexture.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKStatePipelineCompute.h"

namespace LostPeterVulkan
{
    VKPipelineComputeTerrain::VKPipelineComputeTerrain(const String& namePipelineCompute, VKRenderPassTerrain* pVKRenderPassTerrain)
        : Base(namePipelineCompute)

        , m_pVKRenderPassTerrain(pVKRenderPassTerrain)

        , pDescriptorSetLayout(nullptr)
        , poStatePipelineCompute(nullptr)
        
        , pTextureCopy(nullptr)
        , poBuffer_TextureCopy(nullptr)

        , isNormalUpdated(false)
        , isNormalUpdated_Sustained(true)
    {

    }

    VKPipelineComputeTerrain::~VKPipelineComputeTerrain()
    {
        Destroy();
    }

    void VKPipelineComputeTerrain::Destroy()
    {
        CleanupSwapChain();
        destroyBufferTextureCopy();
    }
        void VKPipelineComputeTerrain::destroyBufferTextureCopy()
        {
            F_DELETE(this->pTextureCopy)
			F_DELETE(this->poBuffer_TextureCopy)
        }

    bool VKPipelineComputeTerrain::Init(DescriptorSetLayout* pDSL,
                                        VKShader* pShader)
    {
        this->pDescriptorSetLayout = pDSL;

        //1> Buffer
        if (this->pTextureCopy == nullptr)
        {
            if (!createBufferTextureCopy(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize))
            {
                F_LogError("*********************** VKPipelineComputeTerrain::Init: createBufferTextureCopy failed !");
                return false;
            }
        }

        //2> Pipeline
		this->poStatePipelineCompute = Base::GetWindowPtr()->createStatePipelineCompute("PipelineCompute-Terrain",
																						pDSL,
																						pShader,
																						false);

        if (this->poStatePipelineCompute == nullptr)
        {
            F_LogError("*********************** VKPipelineComputeTerrain::Init: createStatePipelineCompute failed !");
            return false;
        }

        //3> DescriptorSet
        UpdateDescriptorSet();
        
        return true;
    }
        bool VKPipelineComputeTerrain::createBufferTextureCopy(int nHeightMapSize)
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
				String msg = "*********************** VKPipelineComputeTerrain::createBufferTextureCopy: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
        }

    void VKPipelineComputeTerrain::CleanupSwapChain()
    {
		F_DELETE(this->poStatePipelineCompute)
    }  

    void VKPipelineComputeTerrain::UpdateDescriptorSet()
    {
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
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poStatePipelineCompute->poDescriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_TextureCopy);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poStatePipelineCompute->poDescriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->pTexture_HeightMap->GetVkDescriptorImageInfo());
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poStatePipelineCompute->poDescriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassTerrain->pTexture_NormalMap->GetVkDescriptorImageInfo());
            }
            else
            {
                String msg = "*********************** VKPipelineComputeTerrain::UpdateDescriptorSet: Wrong DescriptorSetLayout type: " + nameDescriptor;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
    }

}; //LostPeterVulkan