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

#include "../include/VKPipelineGraphicsCopyBlit.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsCopyBlit::VKPipelineGraphicsCopyBlit(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline_WireFrame(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , m_vkBuffer_CopyBlit(VK_NULL_HANDLE)
        , m_vkBuffersMemory_CopyBlit(VK_NULL_HANDLE)
    {

    }

    VKPipelineGraphicsCopyBlit::~VKPipelineGraphicsCopyBlit()
    {
        Destroy();
    }

    void VKPipelineGraphicsCopyBlit::Destroy()
    {
        CleanupSwapChain();
        destroyBufferCopyBlitObject();
    }
        void VKPipelineGraphicsCopyBlit::destroyBufferCopyBlitObject()
        {
            if (this->m_vkBuffer_CopyBlit != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->m_vkBuffer_CopyBlit, this->m_vkBuffersMemory_CopyBlit);
            }
            this->m_vkBuffer_CopyBlit = VK_NULL_HANDLE;
            this->m_vkBuffersMemory_CopyBlit = VK_NULL_HANDLE;
        }


    bool VKPipelineGraphicsCopyBlit::Init(const String& descriptorSetLayout,
                                          StringVector* pDescriptorSetLayoutNames,
                                          const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                          const VkPipelineLayout& vkPipelineLayout,
                                          const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->nameDescriptorSetLayout = descriptorSetLayout;
        this->poDescriptorSetLayoutNames = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout = vkDescriptorSetLayout;
        this->poPipelineLayout = vkPipelineLayout;

        //1> VkBuffer
        if (this->m_vkBuffer_CopyBlit == VK_NULL_HANDLE)
        {
            if (!createBufferCopyBlitObject())
            {
                F_LogError("*********************** VKPipelineGraphicsCopyBlit::Init: createBufferCopyBlitObject failed !");
                return false;
            }
        }

        //2> VkPipeline
        {
            VkStencilOpState stencilOpFront; 
            VkStencilOpState stencilOpBack;

            VkViewportVector aViewports;
            aViewports.push_back(Base::GetWindowPtr()->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(Base::GetWindowPtr()->poScissor);
            
            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                                              false, 0, 3,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                              Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                              VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, 1.0f,
                                                                              VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                              VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                              VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                              VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlit::Init: Failed to create pipeline graphics for [PipelineGraphics_CopyBlit] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlit::Init: [PipelineGraphics_CopyBlit] Create pipeline graphics success !");
            
            this->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                                                                false, 0, 3,
                                                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                                                VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, 1.0f,
                                                                                                VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                                VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlit::Init: Failed to create pipeline graphics wire frame for [PipelineGraphics_CopyBlit] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlit::Init: [PipelineGraphics_CopyBlit] Create pipeline graphics wire frame success !");
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets(this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsCopyBlit::Init: createVkDescriptorSets failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineGraphicsCopyBlit::createBufferCopyBlitObject()
        {
            this->m_objectCB_CopyBlit.offsetX = 0.0f;
            this->m_objectCB_CopyBlit.offsetY = 0.0f;
            this->m_objectCB_CopyBlit.scaleX = 2.0f;
            this->m_objectCB_CopyBlit.scaleY = 2.0f;
            VkDeviceSize bufferSize = sizeof(CopyBlitObjectConstants);
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->m_vkBuffer_CopyBlit, this->m_vkBuffersMemory_CopyBlit);
            UpdateBuffer(this->m_objectCB_CopyBlit);
            return true;
        }

    void VKPipelineGraphicsCopyBlit::CleanupSwapChain()
    {
        this->poDescriptorSetLayoutNames = nullptr;
        this->poDescriptorSetLayout = VK_NULL_HANDLE;
        this->poPipelineLayout = VK_NULL_HANDLE;
        if (this->poPipeline_WireFrame != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline_WireFrame);
        }
        this->poPipeline_WireFrame = VK_NULL_HANDLE;

        if (this->poPipeline != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkPipeline(this->poPipeline);
        }
        this->poPipeline = VK_NULL_HANDLE;
        this->poDescriptorSets.clear();
    }  

    void VKPipelineGraphicsCopyBlit::UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo)
    {
        StringVector* pDescriptorSetLayoutNames = this->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "VKPipelineGraphicsCopyBlit::UpdateDescriptorSets")
        size_t count_ds = this->poDescriptorSets.size();
        for (size_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectCopyBlit = {};
                    bufferInfo_ObjectCopyBlit.buffer = this->m_vkBuffer_CopyBlit;
                    bufferInfo_ObjectCopyBlit.offset = 0;
                    bufferInfo_ObjectCopyBlit.range = sizeof(CopyBlitObjectConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_ObjectCopyBlit);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameColor)) //TextureFrameColor
                {
                    Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                    this->poDescriptorSets[i],
                                                                    j,
                                                                    0,
                                                                    1,
                                                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                    const_cast<VkDescriptorImageInfo&>(imageInfo));
                }
                else
                {
                    String msg = "*********************** VKPipelineGraphicsCopyBlit::UpdateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

    void VKPipelineGraphicsCopyBlit::UpdateBuffer(const CopyBlitObjectConstants& object)
    {
        m_objectCB_CopyBlit = object;
        Base::GetWindowPtr()->updateVKBuffer(0, sizeof(CopyBlitObjectConstants), &this->m_objectCB_CopyBlit, this->m_vkBuffersMemory_CopyBlit);
    }

}; //LostPeterVulkan