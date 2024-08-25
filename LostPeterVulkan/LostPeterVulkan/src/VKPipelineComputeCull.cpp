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

#include "../include/VKPipelineComputeCull.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineComputeCull::VKPipelineComputeCull(const String& namePipelineCompute)
        : Base(namePipelineCompute)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , poBuffer_CullCB(VK_NULL_HANDLE)
        , poBufferMemory_CullCB(VK_NULL_HANDLE)

        , poBuffer_CullObjectCB(VK_NULL_HANDLE)
        , poBufferMemory_CullObjectCB(VK_NULL_HANDLE)

    {

    }

    VKPipelineComputeCull::~VKPipelineComputeCull()
    {
        Destroy();
    }

    void VKPipelineComputeCull::Destroy()
    {
        CleanupSwapChain();
        destroyBufferCullObject();
        destroyBufferCull();
    }
        void VKPipelineComputeCull::destroyBufferCullObject()
        {
            if (this->poBuffer_CullCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_CullCB, this->poBufferMemory_CullCB);
            }
            this->poBuffer_CullCB = VK_NULL_HANDLE;
            this->poBufferMemory_CullCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyBufferCull()
        {
            if (this->poBuffer_CullObjectCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_CullObjectCB, this->poBufferMemory_CullObjectCB);
            }
            this->poBuffer_CullObjectCB = VK_NULL_HANDLE;
            this->poBufferMemory_CullObjectCB = VK_NULL_HANDLE;
        }

    bool VKPipelineComputeCull::Init(const String& descriptorSetLayout,
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
        if (this->poBuffer_CullCB == VK_NULL_HANDLE)
        {
            if (!createBufferCull())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferCull failed !");
                return false;
            }
        }
        if (this->poBuffer_CullObjectCB == VK_NULL_HANDLE)
        {
            if (!createBufferCullObject())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferCullObject failed !");
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
            F_LogError("*********************** VKPipelineComputeCull::Init: createVkComputePipeline failed !");
            return false;
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSet(this->poDescriptorSetLayout, this->poDescriptorSet);
        if (this->poDescriptorSet == VK_NULL_HANDLE)
        {
            F_LogError("*********************** VKPipelineComputeCull::Init: createVkDescriptorSet failed !");
            return false;
        }
        UpdateDescriptorSet();
        
        return true;
    }
        bool VKPipelineComputeCull::createBufferCull()
        {
            destroyBufferCull();

            VkDeviceSize bufferSize = sizeof(CullConstants);
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_CullCB, this->poBufferMemory_CullCB);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CullConstants), &this->cullCB, this->poBufferMemory_CullCB);
            return true;
        }
        bool VKPipelineComputeCull::createBufferCullObject()
        {
            destroyBufferCullObject();

            VkDeviceSize bufferSize = sizeof(CullObjectConstants) * MAX_OBJECT_CULL_COUNT;
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_CullObjectCB, this->poBufferMemory_CullObjectCB);
            //Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CullConstants), &this->cullObjectCBs[0], this->poBufferMemory_CullObjectCB);
            return true;
        }


    void VKPipelineComputeCull::CleanupSwapChain()
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

    void VKPipelineComputeCull::UpdateDescriptorSet()
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