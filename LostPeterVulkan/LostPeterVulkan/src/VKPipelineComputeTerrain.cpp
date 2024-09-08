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

namespace LostPeterVulkan
{
    VKPipelineComputeTerrain::VKPipelineComputeTerrain(const String& namePipelineCompute, VKRenderPassTerrain* pVKRenderPassTerrain)
        : Base(namePipelineCompute)

        , m_pVKRenderPassTerrain(pVKRenderPassTerrain)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , pTextureCopy(nullptr)
        , poBuffer_TextureCopy(VK_NULL_HANDLE)
        , poBufferMemory_TextureCopy(VK_NULL_HANDLE)

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
            if (this->poBuffer_TextureCopy != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
            }
            this->poBuffer_TextureCopy = VK_NULL_HANDLE;
            this->poBufferMemory_TextureCopy = VK_NULL_HANDLE;
        }

    bool VKPipelineComputeTerrain::Init(const String& descriptorSetLayout,
                                        StringVector* pDescriptorSetLayoutNames,
                                        const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                        const VkPipelineLayout& vkPipelineLayout,
                                        const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout = descriptorSetLayout;
        this->poDescriptorSetLayoutNames = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout = vkDescriptorSetLayout;
        this->poPipelineLayout = vkPipelineLayout;

        //1> VkBuffer
        if (this->pTextureCopy == nullptr)
        {
            if (!createBufferTextureCopy(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize))
            {
                F_LogError("*********************** VKPipelineComputeTerrain::Init: createBufferTextureCopy failed !");
                return false;
            }
        }

        //2> VkPipeline
        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        shaderStageInfo.module = vkShaderModule;
        shaderStageInfo.pName = "main";
        this->poPipeline = Base::GetWindowPtr()->createVkComputePipeline(shaderStageInfo, vkPipelineLayout);
        if (this->poPipeline == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKPipelineComputeTerrain::Init: createVkComputePipeline failed !");
            return false;
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSet(this->poDescriptorSetLayout, this->poDescriptorSet);
        if (this->poDescriptorSet == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKPipelineComputeTerrain::Init: createVkDescriptorSet failed !");
            return false;
        }
        UpdateDescriptorSet();
        
        return true;
    }
        bool VKPipelineComputeTerrain::createBufferTextureCopy(int nHeightMapSize)
        {
            destroyBufferTextureCopy();
            this->pTextureCopy = new TextureCopyConstants();
            this->pTextureCopy->texInfo.x = nHeightMapSize;
            this->pTextureCopy->texInfo.y = nHeightMapSize;
            VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TextureCopy, this->poBufferMemory_TextureCopy);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(TextureCopyConstants), this->pTextureCopy, this->poBufferMemory_TextureCopy);
            return true;
        }

    void VKPipelineComputeTerrain::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
        this->poPipelineLayout = VK_NULL_HANDLE;
        if (this->poPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline);
        }       
        this->poPipeline = VK_NULL_HANDLE;
        this->poDescriptorSet = VK_NULL_HANDLE;
    }  

    void VKPipelineComputeTerrain::UpdateDescriptorSet()
    {
        VkWriteDescriptorSetVector descriptorWrites;
        size_t count = this->poDescriptorSetLayoutNames->size();
        for (size_t i = 0; i < count; i++)
        {
            const String& nameDescriptor = this->poDescriptorSetLayoutNames->at(i);

            if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCopy)) //TextureCopy
            {
                VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
                bufferInfo_TextureCopy.buffer = this->poBuffer_TextureCopy;
                bufferInfo_TextureCopy.offset = 0;
                bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_TextureCopy);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainHeightMapImageInfo_Sampler);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassTerrain->poTerrainNormalMapImageInfo_Sampler);
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