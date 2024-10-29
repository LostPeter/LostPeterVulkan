/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-10-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineGraphicsDepthHiz.h"
#include "../include/VKRenderPassCull.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsDepthHiz::VKPipelineGraphicsDepthHiz(const String& namePipelineGraphics, VKRenderPassCull* pVKRenderPassCull)
        : Base(namePipelineGraphics)
        , m_pVKRenderPassCull(pVKRenderPassCull)

        //PipelineGraphics-HizDepth
        , nameDescriptorSetLayout_HizDepth("")
        , poDescriptorSetLayoutNames_HizDepth(nullptr)
        , poDescriptorSetLayout_HizDepth(VK_NULL_HANDLE)
        , poPipelineLayout_HizDepth(VK_NULL_HANDLE)
        , poPipeline_HizDepth(VK_NULL_HANDLE)

        , pMesh(nullptr)

    {

    }   

    VKPipelineGraphicsDepthHiz::~VKPipelineGraphicsDepthHiz()
    {
        Destroy();
    }

    void VKPipelineGraphicsDepthHiz::Destroy()
    {
        CleanupSwapChain();
    }

    bool VKPipelineGraphicsDepthHiz::Init(Mesh* pMesh)
    {
        F_Assert(pMesh != nullptr && "VKPipelineGraphicsDepthHiz::Init")
        this->pMesh = pMesh;
        
        return true;
    }
    bool VKPipelineGraphicsDepthHiz::InitHizDepth(const String& descriptorSetLayout,
                                                  StringVector* pDescriptorSetLayoutNames,
                                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                  const VkPipelineLayout& vkPipelineLayout,
                                                  const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->nameDescriptorSetLayout_HizDepth = descriptorSetLayout;
        this->poDescriptorSetLayoutNames_HizDepth = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout_HizDepth = vkDescriptorSetLayout;
        this->poPipelineLayout_HizDepth = vkPipelineLayout;

        if (!createVkGraphicsPipeline("PipelineGraphics-HizDepth-" + this->name,
                                      descriptorSetLayout,
                                      "DescriptorSets-" + this->name,
                                      pDescriptorSetLayoutNames,
                                      vkDescriptorSetLayout,
                                      vkPipelineLayout,
                                      aShaderStageCreateInfos,
                                      this->poPipeline_HizDepth,
                                      &this->poDescriptorSets_HizDepth))
        {
            F_LogError("*********************** VKPipelineGraphicsDepthHiz::InitHizDepth: createVkGraphicsPipeline failed !");
            return false;
        }
        UpdateDescriptorSet_HizDepth();

        return true;
    }
        bool VKPipelineGraphicsDepthHiz::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                  const String& descriptorSetLayout,
                                                                  const String& nameDescriptorSets,
                                                                  StringVector* pDescriptorSetLayoutNames,
                                                                  const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                                  const VkPipelineLayout& vkPipelineLayout,
                                                                  const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                  VkPipeline& vkPipeline,
                                                                  VkDescriptorSetVector* pDescriptorSets)
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
                };

                VkPipelineColorBlendAttachmentStateVector aColorBlendAttachmentState;
                aColorBlendAttachmentState.push_back(Util_PipelineColorBlendAttachmentState(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, false));
                vkPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(nameGraphicsPipeline,
                                                                            aShaderStageCreateInfos,
                                                                            false, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                            this->m_pVKRenderPassCull->poRenderPass, vkPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                            VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_TRUE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                            VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                            VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                            aColorBlendAttachmentState);
                if (vkPipeline == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VKPipelineGraphicsDepthHiz::createVkGraphicsPipeline: Failed to create pipeline graphics for: " + nameGraphicsPipeline;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("VKPipelineGraphicsDepthHiz::createVkGraphicsPipeline: [%s] Create pipeline graphics success !", nameGraphicsPipeline.c_str());
            }

            //2> VkDescriptorSets
            if (pDescriptorSets != nullptr)
            {
                Base::GetWindowPtr()->createVkDescriptorSets(nameDescriptorSets, vkDescriptorSetLayout, *pDescriptorSets);
                if (pDescriptorSets->empty())
                {
                    F_LogError("*********************** VKPipelineGraphicsDepthHiz::createVkGraphicsPipeline: createVkDescriptorSets failed !");
                    return false;
                }
            }
            
            return true;
        }

    void VKPipelineGraphicsDepthHiz::CleanupSwapChain()
    {
        //PipelineGraphics-HizDepth
        this->poDescriptorSetLayoutNames_HizDepth = nullptr;
        this->poDescriptorSetLayout_HizDepth = VK_NULL_HANDLE;
        this->poPipelineLayout_HizDepth = VK_NULL_HANDLE;
        if (this->poPipeline_HizDepth != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_HizDepth);
        }
        this->poPipeline_HizDepth = VK_NULL_HANDLE;
        this->poDescriptorSets_HizDepth.clear();
    }  


    void VKPipelineGraphicsDepthHiz::UpdateDescriptorSet_HizDepth()
    {
        updateDescriptorSets(this->poDescriptorSets_HizDepth, 
                             this->poDescriptorSetLayoutNames_HizDepth,
                             this->m_pVKRenderPassCull->poBuffer_HizDepthCB,
                             Base::GetWindowPtr()->poDepthImageView,
                             this->m_pVKRenderPassCull->poHizDepthSampler);
    }
    void VKPipelineGraphicsDepthHiz::updateDescriptorSets(VkDescriptorSetVector& vkDescriptorSets,
                                                          StringVector* poDescriptorSetLayoutNames,
                                                          VkBuffer vkBuffer_HizDepthCB,
                                                          VkImageView vkImageView,
                                                          VkSampler vkSampler)
    {
        uint32_t count_descriptorsets = (uint32_t)vkDescriptorSets.size();
        for (uint32_t i = 0; i < count_descriptorsets; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            uint32_t count_name = (uint32_t)poDescriptorSetLayoutNames->size();
            for (uint32_t j = 0; j < count_name; j++)
            {
                String& nameDescriptorSet = poDescriptorSetLayoutNames->at(j);

                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_HizDepth)) //HizDepth
                {
                    VkDescriptorBufferInfo bufferInfo_HizDepth = {};
                    bufferInfo_HizDepth.buffer = vkBuffer_HizDepthCB;
                    bufferInfo_HizDepth.offset = 0;
                    bufferInfo_HizDepth.range = sizeof(HizDepthConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      vkDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_HizDepth);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
                {
                    VkDescriptorImageInfo imageInfo;
                    imageInfo.sampler = vkSampler;
                    imageInfo.imageView = vkImageView;
                    imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
                    Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                    vkDescriptorSets[i],
                                                                    j,
                                                                    0,
                                                                    1,
                                                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                    imageInfo);
                }
                else
                {
                    String msg = "*********************** VKPipelineGraphicsDepthHiz::updateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

}; //LostPeterVulkan