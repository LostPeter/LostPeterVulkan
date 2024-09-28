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

namespace LostPeterVulkan
{
    VKPipelineGraphicsDepthShadowMap::VKPipelineGraphicsDepthShadowMap(const String& namePipelineGraphics, VKRenderPassShadowMap* pVKRenderPassShadowMap)
        : Base(namePipelineGraphics)

        , m_pVKRenderPassShadowMap(pVKRenderPassShadowMap)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        

    {

    }   

    VKPipelineGraphicsDepthShadowMap::~VKPipelineGraphicsDepthShadowMap()
    {
        Destroy();
    }

    void VKPipelineGraphicsDepthShadowMap::Destroy()
    {
        CleanupSwapChain();
    }

    bool VKPipelineGraphicsDepthShadowMap::Init(const String& descriptorSetLayout,
                                                StringVector* pDescriptorSetLayoutNames,
                                                const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                const VkPipelineLayout& vkPipelineLayout,
                                                const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->nameDescriptorSetLayout = descriptorSetLayout;
        this->poDescriptorSetLayoutNames = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout = vkDescriptorSetLayout;
        this->poPipelineLayout = vkPipelineLayout;

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

            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline("GraphicsPipeline-" + this->name,
                                                                              aShaderStageCreateInfos,
                                                                              false, 0, 3,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2), 
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                              this->m_pVKRenderPassShadowMap->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                              VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_TRUE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                              VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                              VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                              aColorBlendAttachmentState);
            if (this->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsDepthShadowMap::Init: Failed to create pipeline graphics for [PipelineGraphics_DepthShadowMap] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsDepthShadowMap::Init: [PipelineGraphics_DepthShadowMap] Create pipeline graphics success !");
        }

        //2> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-" + this->name, this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::Init: createVkDescriptorSets failed !");
            return false;
        }
        UpdateDescriptorSets(Base::GetWindowPtr()->poBuffers_ObjectWorldCB);

        return true;
    }

    void VKPipelineGraphicsDepthShadowMap::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
        this->poPipelineLayout = VK_NULL_HANDLE;
        if (this->poPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline);
        }
        this->poPipeline = VK_NULL_HANDLE;
        this->poDescriptorSets.clear();
    }  
    
    void VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSets(const VkBufferVector& poBuffersObject)
    {
        StringVector* pDescriptorSetLayoutNames = this->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSets")
        uint32_t count_ds = (uint32_t)this->poDescriptorSets.size();
        F_Assert(poBuffersObject.size() == count_ds && "VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSets")
        for (uint32_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            uint32_t count_names = (uint32_t)pDescriptorSetLayoutNames->size();
            for (uint32_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Pass);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
                {
                    VkDescriptorBufferInfo bufferInfo_Object = {};
                    bufferInfo_Object.buffer = poBuffersObject[i];
                    bufferInfo_Object.offset = 0;
                    bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_WORLD_COUNT;
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Object);
                }
                else
                {
                    String msg = "*********************** VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

}; //LostPeterVulkan