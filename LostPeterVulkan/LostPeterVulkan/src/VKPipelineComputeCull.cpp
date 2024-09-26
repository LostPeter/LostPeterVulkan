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
#include "../include/VKRenderPassCull.h"
#include "../include/VulkanWindow.h"
#include "../include/CullManager.h"

namespace LostPeterVulkan
{
    VKPipelineComputeCull::VKPipelineComputeCull(const String& namePipelineCompute, VKRenderPassCull* pVKRenderPassCull)
        : Base(namePipelineCompute)
        , m_pVKRenderPassCull(pVKRenderPassCull)
        , m_pCullManager(new CullManager())

        //PipelineCompute-CullClearArgs
        , nameDescriptorSetLayout_CullClearArgs("")
        , poDescriptorSetLayoutNames_CullClearArgs(nullptr)
        , poDescriptorSetLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipelineLayout_CullClearArgs(VK_NULL_HANDLE)
        , poPipeline_CullClearArgs(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustum
        , nameDescriptorSetLayout_CullFrustum("")
        , poDescriptorSetLayoutNames_CullFrustum(nullptr)
        , poDescriptorSetLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustum(VK_NULL_HANDLE)
        , poPipeline_CullFrustum(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustumDepthHiz
        , nameDescriptorSetLayout_CullFrustumDepthHiz("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHiz(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHiz(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHiz(VK_NULL_HANDLE)

        //PipelineCompute-CullFrustumDepthHizClip
        , nameDescriptorSetLayout_CullFrustumDepthHizClip("")
        , poDescriptorSetLayoutNames_CullFrustumDepthHizClip(nullptr)
        , poDescriptorSetLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipelineLayout_CullFrustumDepthHizClip(VK_NULL_HANDLE)
        , poPipeline_CullFrustumDepthHizClip(VK_NULL_HANDLE)

        //PipelineCompute-HizDepthGenerate
        , nameDescriptorSetLayout_HizDepthGenerate("")
        , poDescriptorSetLayoutNames_HizDepthGenerate(nullptr)
        , poDescriptorSetLayout_HizDepthGenerate(VK_NULL_HANDLE)
        , poPipelineLayout_HizDepthGenerate(VK_NULL_HANDLE)
        , poPipeline_HizDepthGenerate(VK_NULL_HANDLE)

        //CullConstants
        , poBuffer_CullCB(VK_NULL_HANDLE)
        , poBufferMemory_CullCB(VK_NULL_HANDLE)

        //CullObjectConstants
        , poBuffer_CullObjectCB(VK_NULL_HANDLE)
        , poBufferMemory_CullObjectCB(VK_NULL_HANDLE)

        //CullObjectConstants
        , poBuffer_HizDepthCB(VK_NULL_HANDLE)
        , poBufferMemory_HizDepthCB(VK_NULL_HANDLE)
        
        //Args
        , poBuffer_ArgsCB(VK_NULL_HANDLE)
        , poBufferMemory_ArgsCB(VK_NULL_HANDLE)

        //Lod
        , poBuffer_LodCB(VK_NULL_HANDLE)
        , poBufferMemory_LodCB(VK_NULL_HANDLE)

        //Result
        , poBuffer_ResultCB(VK_NULL_HANDLE)
        , poBufferMemory_ResultCB(VK_NULL_HANDLE)

        //Clip
        , poBuffer_ClipCB(VK_NULL_HANDLE)
        , poBufferMemory_ClipCB(VK_NULL_HANDLE)

        
    {

    }

    VKPipelineComputeCull::~VKPipelineComputeCull()
    {
        Destroy();
    }

    void VKPipelineComputeCull::Destroy()
    {
        F_DELETE(m_pCullManager)
        CleanupSwapChain();
        destroyBufferClip();
        destroyBufferResult();
        destroyBufferLod();
        destroyBufferArgs();
        destroyBufferHizDepth();
        destroyBufferCullObject();
        destroyBufferCull();
    }
        void VKPipelineComputeCull::destroyBufferClip()
        {
            if (this->poBuffer_ClipCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_ClipCB, this->poBufferMemory_ClipCB);
            }
            this->poBuffer_ClipCB = VK_NULL_HANDLE;
            this->poBufferMemory_ClipCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyBufferResult()
        {
            if (this->poBuffer_ResultCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_ResultCB, this->poBufferMemory_ResultCB);
            }
            this->poBuffer_ResultCB = VK_NULL_HANDLE;
            this->poBufferMemory_ResultCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyBufferLod()
        {
            if (this->poBuffer_LodCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_LodCB, this->poBufferMemory_LodCB);
            }
            this->poBuffer_LodCB = VK_NULL_HANDLE;
            this->poBufferMemory_LodCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyBufferArgs()
        {
            if (this->poBuffer_ArgsCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_ArgsCB, this->poBufferMemory_ArgsCB);
            }
            this->poBuffer_ArgsCB = VK_NULL_HANDLE;
            this->poBufferMemory_ArgsCB = VK_NULL_HANDLE;
        }
        void VKPipelineComputeCull::destroyBufferHizDepth()
        {
            if (this->poBuffer_HizDepthCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            }
            this->poBuffer_HizDepthCB = VK_NULL_HANDLE;
            this->poBufferMemory_HizDepthCB = VK_NULL_HANDLE;
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
        //CullConstants
        if (this->poBuffer_CullCB == VK_NULL_HANDLE)
        {
            if (!createBufferCull())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferCull failed !");
                return false;
            }
        }
        //CullObjectConstants
        if (this->poBuffer_CullObjectCB == VK_NULL_HANDLE)
        {
            if (!createBufferCullObject())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferCullObject failed !");
                return false;
            }
        }
        //HizDepthConstants
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferHizDepth())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferHizDepth failed !");
                return false;
            }
        }
        //Args
        if (this->poBuffer_ArgsCB == VK_NULL_HANDLE)
        {
            if (!createBufferArgs())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferArgs failed !");
                return false;
            }
        }
        //Lod
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferLod())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferLod failed !");
                return false;
            }
        }
        //Result
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferResult())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferResult failed !");
                return false;
            }
        }
        //Clip
        if (this->poBuffer_HizDepthCB == VK_NULL_HANDLE)
        {
            if (!createBufferClip())
            {
                F_LogError("*********************** VKPipelineComputeCull::Init: createBufferClip failed !");
                return false;
            }
        }
        //CullManager
        m_pCullManager->Init(this);

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

        if (!createVkComputePipeline("PipelineCompute-CullClearArgs",
                                     descriptorSetLayout,
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

        if (!createVkComputePipeline("PipelineCompute-CullFrustum",
                                     descriptorSetLayout,
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

        if (!createVkComputePipeline("PipelineCompute-CullFrustumDepthHiz",
                                     descriptorSetLayout,
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

        if (!createVkComputePipeline("PipelineCompute-CullFrustumDepthHizClip",
                                     descriptorSetLayout,
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
    bool VKPipelineComputeCull::InitHizDepthGenerate(const String& descriptorSetLayout,
                                                     StringVector* pDescriptorSetLayoutNames,
                                                     const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                     const VkPipelineLayout& vkPipelineLayout,
                                                     const VkShaderModule& vkShaderModule)
    {
        this->nameDescriptorSetLayout_HizDepthGenerate = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_HizDepthGenerate = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_HizDepthGenerate = vkDescriptorSetLayout;
        this->poPipelineLayout_HizDepthGenerate = vkPipelineLayout;

        if (!createVkComputePipeline("PipelineCompute-HizDepthGenerate",
                                     descriptorSetLayout,
                                     pDescriptorSetLayoutNames,
                                     vkDescriptorSetLayout,
                                     vkPipelineLayout,
                                     vkShaderModule,
                                     this->poPipeline_HizDepthGenerate,
                                     this->poDescriptorSet_HizDepthGenerate))
        {
            F_LogError("*********************** VKPipelineComputeCull::InitHizDepthGenerate: createVkComputePipeline failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineComputeCull::createBufferCull()
        {
            destroyBufferCull();

            VkDeviceSize bufferSize = sizeof(CullConstants);
            Base::GetWindowPtr()->createVkBuffer("CullConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_CullCB, this->poBufferMemory_CullCB);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CullConstants), &this->cullCB, this->poBufferMemory_CullCB);
            return true;
        }
        bool VKPipelineComputeCull::createBufferCullObject()
        {
            destroyBufferCullObject();

            VkDeviceSize bufferSize = sizeof(CullObjectConstants) * MAX_OBJECT_CULL_COUNT;
            Base::GetWindowPtr()->createVkBuffer("CullObjectConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_CullObjectCB, this->poBufferMemory_CullObjectCB);
            return true;
        }
        bool VKPipelineComputeCull::createBufferHizDepth()
        {
            destroyBufferHizDepth();

            VkDeviceSize bufferSize = sizeof(HizDepthConstants);
            Base::GetWindowPtr()->createVkBuffer("HizDepthConstants-" + this->name, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_HizDepthCB, this->poBufferMemory_HizDepthCB);
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(HizDepthConstants), &this->hizDepthCB, this->poBufferMemory_HizDepthCB);
            return true;
        } 
        bool VKPipelineComputeCull::createBufferArgs()
        {


            return true;
        }
        bool VKPipelineComputeCull::createBufferLod()
        {

            return true;
        }
        bool VKPipelineComputeCull::createBufferResult()
        {

            return true;
        }
        bool VKPipelineComputeCull::createBufferClip()
        {
            
            return true;
        }
        bool VKPipelineComputeCull::createVkComputePipeline(const String& nameComputePipeline,
                                                            const String& descriptorSetLayout,
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
            vkPipeline = Base::GetWindowPtr()->createVkComputePipeline(nameComputePipeline, shaderStageInfo, vkPipelineLayout);
            if (vkPipeline == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VKPipelineComputeCull::createVkComputePipeline: createVkComputePipeline failed !");
                return false;
            }

            //2> VkDescriptorSet
            Base::GetWindowPtr()->createVkDescriptorSet(descriptorSetLayout, vkDescriptorSetLayout, vkDescriptorSet);
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

        //PipelineCompute-CullFrustum
        this->poDescriptorSetLayoutNames_CullFrustum = nullptr;
        this->poDescriptorSetLayout_CullFrustum = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustum = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustum);  
        this->poPipeline_CullFrustum = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustum = VK_NULL_HANDLE;

        //PipelineCompute-CullFrustumDepthHiz
        this->poDescriptorSetLayoutNames_CullFrustumDepthHiz = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHiz = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHiz);  
        this->poPipeline_CullFrustumDepthHiz = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHiz = VK_NULL_HANDLE;

        //PipelineCompute-CullFrustumDepthHizClip
        this->poDescriptorSetLayoutNames_CullFrustumDepthHizClip = nullptr;
        this->poDescriptorSetLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poPipelineLayout_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_CullFrustumDepthHizClip);  
        this->poPipeline_CullFrustumDepthHizClip = VK_NULL_HANDLE;
        this->poDescriptorSet_CullFrustumDepthHizClip = VK_NULL_HANDLE;

        //PipelineCompute-HizDepthGenerate
        this->poDescriptorSetLayoutNames_HizDepthGenerate = nullptr;
        this->poDescriptorSetLayout_HizDepthGenerate = VK_NULL_HANDLE;
        this->poPipelineLayout_HizDepthGenerate = VK_NULL_HANDLE;
        destroyVkComputePipeline(this->poPipeline_HizDepthGenerate);  
        this->poPipeline_HizDepthGenerate = VK_NULL_HANDLE;
        this->poDescriptorSet_HizDepthGenerate = VK_NULL_HANDLE;


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
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_HizDepth)) //HizDepth
            {
                VkDescriptorBufferInfo bufferInfo_HizDepth = {};
                bufferInfo_HizDepth.buffer = this->poBuffer_HizDepthCB;
                bufferInfo_HizDepth.offset = 0;
                bufferInfo_HizDepth.range = sizeof(HizDepthConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  descriptorSet,
                                                                  (uint32_t)i,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_HizDepth);
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
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_Sampler);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWSrc)) //TextureCSRWSrc
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_NoSampler);
            }
            else if (nameDescriptor == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWDst)) //TextureCSRWDst
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                descriptorSet,
                                                                (uint32_t)i,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                                                this->m_pVKRenderPassCull->poHizDepthImageInfo_NoSampler);
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