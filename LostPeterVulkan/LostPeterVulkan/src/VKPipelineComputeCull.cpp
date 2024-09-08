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

        , nameDescriptorSetLayout_CullClearArgs("")
        , poDescriptorSetLayoutNames_CullClearArgs(nullptr)
        , poDescriptorSetLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipelineLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipeline_CullClearArgs(VK_NULL_HANDLE)

        , nameDescriptorSetLayout_CullFrustum("")
        , poDescriptorSetLayoutNames_CullFrustum(nullptr)
        , poDescriptorSetLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipeline_CullFrustum(VK_NULL_HANDLE)

        , nameDescriptorSetLayout_CullFrustumDepthHiz("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHiz(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHiz(VK_NULL_HANDLE)

        , nameDescriptorSetLayout_CullFrustumDepthHizClip("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHizClip(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHizClip(VK_NULL_HANDLE)

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
        void VKPipelineComputeCull::destroyVkComputePipeline(VkPipeline& poPipeline)
        {
            if (poPipeline != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkPipeline(poPipeline);
            }       
        }

    bool VKPipelineComputeCull::Init()
    {
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

        return true;
    }
    bool VKPipelineComputeCull::InitCullClearArgs(const String& descriptorSetLayout,
                                                  StringVector* pDescriptorSetLayoutNames,
                                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                  const VkPipelineLayout& vkPipelineLayout,
                                                  const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullClearArgs = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullClearArgs = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullClearArgs = vkDescriptorSetLayout;
        this->poPipelineLayout_CullClearArgs = vkPipelineLayout;

        if (!createVkComputePipeline(descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullClearArgs,
                                     this->poDescriptorSet_CullClearArgs))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullClearArgs: createVkComputePipeline failed !");
            return false;
        }
        
        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustum(const String& descriptorSetLayout,
                                                StringVector* pDescriptorSetLayoutNames,
                                                const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                const VkPipelineLayout& vkPipelineLayout,
                                                const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustum = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustum = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustum = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustum = vkPipelineLayout;

        if (!createVkComputePipeline(descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustum,
                                     this->poDescriptorSet_CullFrustum))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustum: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustumDepthHiz(const String& descriptorSetLayout,
                                                        StringVector* pDescriptorSetLayoutNames,
                                                        const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                        const VkPipelineLayout& vkPipelineLayout,
                                                        const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustumDepthHiz = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustumDepthHiz = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustumDepthHiz = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustumDepthHiz = vkPipelineLayout;

        if (!createVkComputePipeline(descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustumDepthHiz,
                                     this->poDescriptorSet_CullFrustumDepthHiz))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustumDepthHiz: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
    bool VKPipelineComputeCull::InitCullFrustumDepthHizClip(const String& descriptorSetLayout,
                                                            StringVector* pDescriptorSetLayoutNames,
                                                            const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                            const VkPipelineLayout& vkPipelineLayout,
                                                            const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_CullFrustumDepthHizClip = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_CullFrustumDepthHizClip = vkDescriptorSetLayout;
        this->poPipelineLayout_CullFrustumDepthHizClip = vkPipelineLayout;

        if (!createVkComputePipeline(descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_CullFrustumDepthHizClip,
                                     this->poDescriptorSet_CullFrustumDepthHizClip))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitCullFrustumDepthHizClip: createVkComputePipeline failed !");
            return false;
        }

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
            return true;
        }
        bool VKPipelineComputeCull::createVkComputePipeline(const String& descriptorSetLayout,
                                                            StringVector* pDescriptorSetLayoutNames,
                                                            const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                            const VkPipelineLayout& vkPipelineLayout,
                                                            const VkShaderModule& vkShaderModule,
                                                            VkPipeline& vkPipeline,
                                                            VkDescriptorSet& vkDescriptorSet)
        {
            //1> VkPipeline
            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
            shaderStageInfo.module = vkShaderModule;
            shaderStageInfo.pName = "main";
            vkPipeline = Base::GetWindowPtr()->createVkComputePipeline(shaderStageInfo, vkPipelineLayout);
            if (vkPipeline == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VKPipelineComputeCull::createVkComputePipeline: createVkComputePipeline failed !");
                return false;
            }

            //2> VkDescriptorSet
            Base::GetWindowPtr()->createVkDescriptorSet(vkDescriptorSetLayout, vkDescriptorSet);
            if (vkDescriptorSet == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VKPipelineComputeCull::createVkComputePipeline: createVkDescriptorSet failed !");
                return false;
            }
            UpdateDescriptorSet(vkDescriptorSet, pDescriptorSetLayoutNames);

            return true;
        }


    void VKPipelineComputeCull::CleanupSwapChain()
    {
        //PipelineCompute-CullClearArgs
        this->poDescriptorSetLayoutNames_CullClearArgs = nullptr;
        this->poDescriptorSetLayout_CullClearArgs = VK_NULL_HANDLE;
        this->poPipelineLayout_CullClearArgs = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullClearArgs);  
        this->poPipeline_CullClearArgs = VK_NULL_HANDLE;
        this->poDescriptorSet_CullClearArgs = VK_NULL_HANDLE;

        //PipelineCompute-CullClearArgs
        this->poDescriptorSetLayoutNames_CullFrustum = nullptr;
        this->poDescriptorSetLayout_CullFrustum = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustum = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustum);  
        this->poPipeline_CullFrustum = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustum = VK_NULL_HANDLE;

        //PipelineCompute-CullClearArgs
        this->poDescriptorSetLayoutNames_CullFrustumDepthHiz = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHiz);  
        this->poPipeline_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHiz = VK_NULL_HANDLE;

        //PipelineCompute-CullClearArgs
        this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHizClip);  
        this->poPipeline_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHizClip = VK_NULL_HANDLE;
    }  

    void VKPipelineComputeCull::UpdateDescriptorSet(VkDescriptorSet& descriptorSet,
                                                    StringVector* poDescriptorSetLayoutNames)
    {
        VkWriteDescriptorSetVector descriptorWrites;
        size_t count = poDescriptorSetLayoutNames->size();
        for (size_t i = 0; i < count; i++)
        {
            const String& nameDescriptor = poDescriptorSetLayoutNames->at(i);

            if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Cull)) //Cull
            {
                VkDescriptorBufferInfo bufferInfo_Cull = {};
                bufferInfo_Cull.buffer = this->poBuffer_CullCB;
                bufferInfo_Cull.offset = 0;
                bufferInfo_Cull.range = sizeof(CullConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  descriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Cull);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCull)) //ObjectCull
            {
                VkDescriptorBufferInfo bufferInfo_ObjectCull = {};
                bufferInfo_ObjectCull.buffer = this->poBuffer_CullObjectCB;
                bufferInfo_ObjectCull.offset = 0;
                bufferInfo_ObjectCull.range = sizeof(CullObjectConstants) * MAX_OBJECT_CULL_COUNT;
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  descriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_ObjectCull);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWArgsCB)) //BufferRWArgsCB
            {
                
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWLodCB)) //BufferRWLodCB
            {
                
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
            {
                
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWClipCB)) //BufferRWClipCB
            {
                
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                
            }
            else
            {
                String msg = "*********************** VKPipelineComputeCull::UpdateDescriptorSet: Wrong DescriptorSetLayout type: " + nameDescriptor;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
    }

}; //LostPeterVulkan