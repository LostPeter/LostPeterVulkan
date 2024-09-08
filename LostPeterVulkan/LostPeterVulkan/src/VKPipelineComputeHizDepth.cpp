/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-25
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineComputeHizDepth.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineComputeHizDepth::VKPipelineComputeHizDepth(const String& namePipelineCompute)
        : Base(namePipelineCompute)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        

    {

    }

    VKPipelineComputeHizDepth::~VKPipelineComputeHizDepth()
    {
        Destroy();
    }

    void VKPipelineComputeHizDepth::Destroy()
    {
        CleanupSwapChain();
        destroyBufferHizDepth();
    }
        void VKPipelineComputeHizDepth::destroyBufferHizDepth()
        {
            if (this->poBuffer_HizDepthCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            }
            this->poBuffer_HizDepthCB = VK_NULL_HANDLE;
            this->poBufferMemory_HizDepthCB = VK_NULL_HANDLE;
        }

    bool VKPipelineComputeHizDepth::Init(const String& descriptorSetLayout,
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
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferHizDepth())
            {
                F_LogError("*********************** VKPipelineComputeHizDepth::Init: createBufferHizDepth failed !");
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
            F_LogError("*********************** VKPipelineComputeHizDepth::Init: createVkComputePipeline failed !");
            return false;
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSet(this->poDescriptorSetLayout, this->poDescriptorSet);
        if (this->poDescriptorSet == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKPipelineComputeHizDepth::Init: createVkDescriptorSet failed !");
            return false;
        }
        UpdateDescriptorSet();
        
        return true;
    }
        bool VKPipelineComputeHizDepth::createBufferHizDepth()
        {
            destroyBufferHizDepth();

            VkDeviceSize bufferSize = sizeof(HizDepthConstants);
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(HizDepthConstants), &this->hizDepthCB, this->poBufferMemory_HizDepthCB);
            return true;
        }   

    void VKPipelineComputeHizDepth::CleanupSwapChain()
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

    void VKPipelineComputeHizDepth::UpdateDescriptorSet()
    {
        VkWriteDescriptorSetVector descriptorWrites;
        size_t count = this->poDescriptorSetLayoutNames->size();
        for (size_t i = 0; i < count; i++)
        {
            const String& nameDescriptor = this->poDescriptorSetLayoutNames->at(i);

            if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_HizDepth)) //HizDepth
            {
                VkDescriptorBufferInfo bufferInfo_HizDepth = {};
                bufferInfo_HizDepth.buffer = this->poBuffer_HizDepthCB;
                bufferInfo_HizDepth.offset = 0;
                bufferInfo_HizDepth.range = sizeof(HizDepthConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_HizDepth);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
            {
                
            }
            else
            {
                String msg = "*********************** VKPipelineComputeHizDepth::UpdateDescriptorSet: Wrong DescriptorSetLayout type: " + nameDescriptor;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
    }

}; //LostPeterVulkan