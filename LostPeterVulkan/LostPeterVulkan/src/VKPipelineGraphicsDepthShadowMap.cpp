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

#include "../include/VKPipelineGraphicsDepthShadowMap.h"
#include "../include/VKRenderPassShadowMap.h"
#include "../include/VulkanWindow.h"
#include "../include/ComputeBuffer.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsDepthShadowMap::VKPipelineGraphicsDepthShadowMap(const String& namePipelineGraphics, VKRenderPassShadowMap* pVKRenderPassShadowMap)
        : Base(namePipelineGraphics)
        , m_pVKRenderPassShadowMap(pVKRenderPassShadowMap)

        //PipelineGraphics-ShadowMapDepth
        , nameDescriptorSetLayout_ShadowMapDepth("")
        , poDescriptorSetLayoutNames_ShadowMapDepth(nullptr)
        , poDescriptorSetLayout_ShadowMapDepth(VK_NULL_HANDLE)
        , poPipelineLayout_ShadowMapDepth(VK_NULL_HANDLE)
        , poPipeline_ShadowMapDepth(VK_NULL_HANDLE)

        //PipelineGraphics-ShadowMapDepthCull
        , nameDescriptorSetLayout_ShadowMapDepthCull("")
        , poDescriptorSetLayoutNames_ShadowMapDepthCull(nullptr)
        , poDescriptorSetLayout_ShadowMapDepthCull(VK_NULL_HANDLE)
        , poPipelineLayout_ShadowMapDepthCull(VK_NULL_HANDLE)
        , poPipeline_ShadowMapDepthCull(VK_NULL_HANDLE)
        
        //ObjectConstants
        , poBuffer_ObjectWorldCB(VK_NULL_HANDLE)
        , poBufferMemory_ObjectWorldCB(VK_NULL_HANDLE)

        //InstanceConstants
        , poBuffer_InstanceCB(VK_NULL_HANDLE)
        , poBufferMemory_InstanceCB(VK_NULL_HANDLE)

    {

    }   

    VKPipelineGraphicsDepthShadowMap::~VKPipelineGraphicsDepthShadowMap()
    {
        Destroy();
    }

    void VKPipelineGraphicsDepthShadowMap::Destroy()
    {
        CleanupSwapChain();
        destroyBufferInstanceCB();
        destroyBufferObjectWorldCB();
    }
        void VKPipelineGraphicsDepthShadowMap::destroyBufferInstanceCB()
        {
            if (this->poBuffer_InstanceCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_InstanceCB, this->poBufferMemory_InstanceCB);
            }
            this->poBuffer_InstanceCB = VK_NULL_HANDLE;
            this->poBufferMemory_InstanceCB = VK_NULL_HANDLE;
            this->instanceCBs.clear();
        }
        void VKPipelineGraphicsDepthShadowMap::destroyBufferObjectWorldCB()
        {
            if (this->poBuffer_ObjectWorldCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_ObjectWorldCB, this->poBufferMemory_ObjectWorldCB);
            }
            this->poBuffer_ObjectWorldCB = VK_NULL_HANDLE;
            this->poBufferMemory_ObjectWorldCB = VK_NULL_HANDLE;
            this->objectWorldCBs.clear();
        }

    bool VKPipelineGraphicsDepthShadowMap::Init()
    {
        //ObjectConstants
        if (this->poBuffer_ObjectWorldCB == VK_NULL_HANDLE)
        {
            if (!createBufferObjectWorldCB())
            {
                F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::Init: createBufferObjectWorldCB failed !");
                return false;
            }
        }

        //InstanceConstants
        if (this->poBuffer_InstanceCB == VK_NULL_HANDLE)
        {
            if (!createBufferInstanceCB())
            {
                F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::Init: createBufferInstanceCB failed !");
                return false;
            }
        }
        
        return true;
    }
    bool VKPipelineGraphicsDepthShadowMap::InitShadowMapDepth(const String& descriptorSetLayout,
                                                              StringVector* pDescriptorSetLayoutNames,
                                                              const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                              const VkPipelineLayout& vkPipelineLayout,
                                                              const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->nameDescriptorSetLayout_ShadowMapDepth = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_ShadowMapDepth = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_ShadowMapDepth = vkDescriptorSetLayout;
        this->poPipelineLayout_ShadowMapDepth = vkPipelineLayout;

        if (!createVkGraphicsPipeline("PipelineGraphics-ShadowMapDepth",
                                      descriptorSetLayout,
                                      pDescriptorSetLayoutNames,
                                      vkDescriptorSetLayout,
                                      vkPipelineLayout,
                                      aShaderStageCreateInfos,
                                      this->poPipeline_ShadowMapDepth,
                                      this->poDescriptorSets_ShadowMapDepth))
        {
            F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::InitShadowMapDepth: createVkGraphicsPipeline failed !");
            return false;
        }
        updateDescriptorSets(this->poDescriptorSets_ShadowMapDepth, 
                             pDescriptorSetLayoutNames,
                             this->poBuffer_ObjectWorldCB,
                             VK_NULL_HANDLE,
                             nullptr,
                             nullptr);

        return true;
    }
    bool VKPipelineGraphicsDepthShadowMap::InitShadowMapDepthCull(const String& descriptorSetLayout,
                                                                  StringVector* pDescriptorSetLayoutNames,
                                                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                                  const VkPipelineLayout& vkPipelineLayout,
                                                                  const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->nameDescriptorSetLayout_ShadowMapDepthCull = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_ShadowMapDepthCull = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_ShadowMapDepthCull = vkDescriptorSetLayout;
        this->poPipelineLayout_ShadowMapDepthCull = vkPipelineLayout;

        if (!createVkGraphicsPipeline("PipelineGraphics-ShadowMapDepthCull",
                                      descriptorSetLayout,
                                      pDescriptorSetLayoutNames,
                                      vkDescriptorSetLayout,
                                      vkPipelineLayout,
                                      aShaderStageCreateInfos,
                                      this->poPipeline_ShadowMapDepthCull,
                                      this->poDescriptorSets_ShadowMapDepthCull))
        {
            F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::InitShadowMapDepthCull: createVkGraphicsPipeline failed !");
            return false;
        }
        updateDescriptorSets(this->poDescriptorSets_ShadowMapDepthCull, 
                             pDescriptorSetLayoutNames,
                             VK_NULL_HANDLE,
                             VK_NULL_HANDLE,
                             nullptr,
                             nullptr);

        return true;
    }
        bool VKPipelineGraphicsDepthShadowMap::createBufferObjectWorldCB()
        {
            Base::GetWindowPtr()->createVkBuffer("ObjectConstants-" + this->name,
                                                 sizeof(ObjectConstants) * MAX_OBJECT_WORLD_COUNT, 
                                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                                 this->poBuffer_ObjectWorldCB, 
                                                 this->poBufferMemory_ObjectWorldCB);
            F_LogInfo("VKPipelineGraphicsDepthShadowMap::createBufferObjectWorldCB: Create Uniform ObjectWorld constant buffer success !");
            return true;
        }
        bool VKPipelineGraphicsDepthShadowMap::createBufferInstanceCB()
        {
            Base::GetWindowPtr()->createVkBuffer("InstanceConstants-" + this->name,
                                                 sizeof(InstanceConstants) * MAX_OBJECT_INSTANCE_COUNT, 
                                                 VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                                                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                                                 this->poBuffer_InstanceCB, 
                                                 this->poBufferMemory_InstanceCB);
            F_LogInfo("VKPipelineGraphicsDepthShadowMap::createBufferInstanceCB: Create Uniform Instance constant buffer success !");
            return true;
        }
        bool VKPipelineGraphicsDepthShadowMap::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                        const String& descriptorSetLayout,
                                                                        StringVector* pDescriptorSetLayoutNames,
                                                                        const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                                        const VkPipelineLayout& vkPipelineLayout,
                                                                        const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                        VkPipeline& vkPipeline,
                                                                        VkDescriptorSetVector& vkDescriptorSets)
        {
            //1> VkPipeline
            {
                VkStencilOpState stencilOpFront; 
                VkStencilOpState stencilOpBack;

                VkViewportVector aViewports;
                aViewports.push_back(Base::GetWindowPtr()->poViewport);
                VkRect2DVector aScissors;
                aScissors.push_back(Base::GetWindowPtr()->poScissor);
                VkDynamicStateVector aDynamicStates =
                {
                    VK_DYNAMIC_STATE_VIEWPORT,
                    VK_DYNAMIC_STATE_SCISSOR,
                    VK_DYNAMIC_STATE_DEPTH_BIAS
                };

                VkPipelineColorBlendAttachmentStateVector aColorBlendAttachmentState;

                vkPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline("GraphicsPipeline-" + this->name,
                                                                            aShaderStageCreateInfos,
                                                                            false, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                            this->m_pVKRenderPassShadowMap->poRenderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_TRUE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                            VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                            VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                            aColorBlendAttachmentState);
                if (vkPipeline == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VKPipelineGraphicsDepthShadowMap::createVkGraphicsPipeline: Failed to create pipeline graphics for: " + nameGraphicsPipeline;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("VKPipelineGraphicsDepthShadowMap::createVkGraphicsPipeline: [%s] Create pipeline graphics success !", nameGraphicsPipeline.c_str());
            }

            //2> VkDescriptorSets
            Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-" + this->name, vkDescriptorSetLayout, vkDescriptorSets);
            if (vkDescriptorSets.empty())
            {
                F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::createVkGraphicsPipeline: createVkDescriptorSets failed !");
                return false;
            }

            return true;
        }

    void VKPipelineGraphicsDepthShadowMap::CleanupSwapChain()
    {
        //PipelineGraphics-ShadowMapDepth
        this->poDescriptorSetLayoutNames_ShadowMapDepth = nullptr;
        this->poDescriptorSetLayout_ShadowMapDepth = VK_NULL_HANDLE;
        this->poPipelineLayout_ShadowMapDepth = VK_NULL_HANDLE;
        if (this->poPipeline_ShadowMapDepth != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_ShadowMapDepth);
        }
        this->poPipeline_ShadowMapDepth = VK_NULL_HANDLE;
        this->poDescriptorSets_ShadowMapDepth.clear();

        //PipelineGraphics-ShadowMapDepthCull
        this->poDescriptorSetLayoutNames_ShadowMapDepthCull = nullptr;
        this->poDescriptorSetLayout_ShadowMapDepthCull = VK_NULL_HANDLE;
        this->poPipelineLayout_ShadowMapDepthCull = VK_NULL_HANDLE;
        if (this->poPipeline_ShadowMapDepthCull != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_ShadowMapDepthCull);
        }
        this->poPipeline_ShadowMapDepthCull = VK_NULL_HANDLE;
        this->poDescriptorSets_ShadowMapDepthCull.clear();
    }  

    void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_Clear()
    {
        this->objectWorldCBs.clear();
    }
    void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_AddOne(const ObjectConstants& object)
    {
        this->objectWorldCBs.push_back(object);
    }
    void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects)
    {
        size_t count = objects.size();
        for (size_t i = 0; i < count; i++)
        {
            this->objectWorldCBs.push_back(objects[i]);
        }
    }
    void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_Update()
    {
        if (this->objectWorldCBs.size() <= 0)
            return;

        if (this->poBufferMemory_ObjectWorldCB != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->updateVKBuffer(0, sizeof(ObjectConstants) * this->objectWorldCBs.size(), this->objectWorldCBs.data(), this->poBufferMemory_ObjectWorldCB);
        }
    }

    void VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSet_ShadowMapDepth()
    {
        updateDescriptorSets(this->poDescriptorSets_ShadowMapDepth, 
                             this->poDescriptorSetLayoutNames_ShadowMapDepth,
                             this->poBuffer_ObjectWorldCB,
                             VK_NULL_HANDLE,
                             nullptr,
                             nullptr);
    }
    void VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSet_ShadowMapDepthCull(ComputeBuffer* pCB_CullInstances,
                                                                                  ComputeBuffer* pCB_Result)
    {
        updateDescriptorSets(this->poDescriptorSets_ShadowMapDepthCull, 
                             poDescriptorSetLayoutNames_ShadowMapDepthCull,
                             VK_NULL_HANDLE,
                             this->poBuffer_InstanceCB,
                             pCB_CullInstances,
                             pCB_Result);
    }

    void VKPipelineGraphicsDepthShadowMap::updateDescriptorSets(VkDescriptorSetVector& vkDescriptorSets,
                                                                StringVector* poDescriptorSetLayoutNames,
                                                                VkBuffer vkBuffer_ObjectWorldCB,
                                                                VkBuffer vkBuffer_InstanceCB,
                                                                ComputeBuffer* pCB_CullInstances,
                                                                ComputeBuffer* pCB_Result)
    {
        size_t count_descriptorsets = vkDescriptorSets.size();
        for (size_t i = 0; i < count_descriptorsets; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            size_t count_name = poDescriptorSetLayoutNames->size();
            for (size_t j = 0; j < count_name; j++)
            {
                String& nameDescriptorSet = poDescriptorSetLayoutNames->at(j);

                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      vkDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Pass);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
                {
                    if (vkBuffer_ObjectWorldCB != VK_NULL_HANDLE)
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = vkBuffer_ObjectWorldCB;
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_WORLD_COUNT;
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Object);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance)) //Instance
                {
                    if (vkBuffer_InstanceCB != VK_NULL_HANDLE)
                    {
                        VkDescriptorBufferInfo bufferInfo_Instance = {};
                        bufferInfo_Instance.buffer = vkBuffer_InstanceCB;
                        bufferInfo_Instance.offset = 0;
                        bufferInfo_Instance.range = sizeof(InstanceConstants) * MAX_OBJECT_INSTANCE_COUNT;
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Instance);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWObjectCullInstance)) //BufferRWObjectCullInstance
                {
                    if (pCB_CullInstances != nullptr)
                    {
                        VkDescriptorBufferInfo bufferInfo_CullInstance = {};
                        bufferInfo_CullInstance.buffer = pCB_CullInstances->poBuffer_Compute;
                        bufferInfo_CullInstance.offset = 0;
                        bufferInfo_CullInstance.range = (VkDeviceSize)pCB_CullInstances->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_CullInstance);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
                {
                    if (pCB_Result != nullptr)
                    {
                        VkDescriptorBufferInfo bufferInfo_Result = {};
                        bufferInfo_Result.buffer = pCB_Result->poBuffer_Compute;
                        bufferInfo_Result.offset = 0;
                        bufferInfo_Result.range = (VkDeviceSize)pCB_Result->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Result);
                    }
                }
                else
                {
                    String msg = "*********************** VKPipelineGraphicsDepthShadowMap::updateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

}; //LostPeterVulkan