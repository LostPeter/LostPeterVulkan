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

    }
        void VKPipelineComputeHizDepth::destroyBufferCull()
        {
            
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

            
        }
        Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
    }

}; //LostPeterVulkan