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

#include "../include/VKPipelineGraphicsCopyBlitFromFrame.h"
#include "../include/VulkanWindow.h"
#include "../include/VKRenderPassCopyBlitFromFrame.h"
#include "../include/Mesh.h"
#include "../include/VKBufferUniform.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsCopyBlitFromFrame::VKPipelineGraphicsCopyBlitFromFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)
        , pVKRenderPassCopyBlitFromFrame(nullptr)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , pBuffer_CopyBlit(nullptr)

        , pMeshBlit(nullptr)
    {

    }

    VKPipelineGraphicsCopyBlitFromFrame::~VKPipelineGraphicsCopyBlitFromFrame()
    {
        Destroy();
    }

    void VKPipelineGraphicsCopyBlitFromFrame::Destroy()
    {
        CleanupSwapChain();
        destroyBufferCopyBlitObject();
        F_DELETE(pVKRenderPassCopyBlitFromFrame)
    }
        void VKPipelineGraphicsCopyBlitFromFrame::destroyBufferCopyBlitObject()
        {
			F_DELETE(this->pBuffer_CopyBlit)
        }

    bool VKPipelineGraphicsCopyBlitFromFrame::Init(uint32_t width, 
                                                   uint32_t height,
                                                   VkFormat format,
                                                   bool isDepth,
                                                   Mesh* pMesh,
                                                   const String& descriptorSetLayout,
                                                   StringVector* pDescriptorSetLayoutNames,
                                                   const VkDescriptorSetLayout& vkDescriptorSetLayout,
                                                   const VkPipelineLayout& vkPipelineLayout,
                                                   const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->pMeshBlit = pMesh;
        this->nameDescriptorSetLayout = descriptorSetLayout;
        this->poDescriptorSetLayoutNames = pDescriptorSetLayoutNames;
        this->poDescriptorSetLayout = vkDescriptorSetLayout;
        this->poPipelineLayout = vkPipelineLayout;


        //0> VKRenderPassCopyBlitFromFrame
        String nameRenderPass = "RenderPass-CopyBlitFromFrame-";
        if (isDepth)
            nameRenderPass += "Depth";
        else
            nameRenderPass += "Color";
        this->pVKRenderPassCopyBlitFromFrame = new VKRenderPassCopyBlitFromFrame(nameRenderPass);
        if (!this->pVKRenderPassCopyBlitFromFrame->Init(width,
                                                        height,
                                                        format,
                                                        isDepth))
        {
            F_LogError("*********************** VKPipelineGraphicsCopyBlitFromFrame::Init: this->pVKRenderPassCopyBlitFromFrame->Init failed !");
            return false;
        }

        //1> Buffer
        if (this->pBuffer_CopyBlit == nullptr)
        {
            if (!createBufferCopyBlitObject())
            {
                F_LogError("*********************** VKPipelineGraphicsCopyBlitFromFrame::Init: createBufferCopyBlitObject failed !");
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
            VkDynamicStateVector aDynamicStates =
            {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };
            
            String namePipeline = "PipelineGraphics-CopyBlitFromFrame-" + this->name;
            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(namePipeline,
                                                                              aShaderStageCreateInfos,
                                                                              false, 0, 3,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                              this->pVKRenderPassCopyBlitFromFrame->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                              VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                              VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                              VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                              VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                              VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlitFromFrame::Init: Failed to create pipeline graphics for [" + namePipeline + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlitFromFrame::Init: [%s] Create pipeline graphics success !", namePipeline.c_str());
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-CopyBlitToFrame-" + this->name, this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsCopyBlitFromFrame::Init: createVkDescriptorSets failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineGraphicsCopyBlitFromFrame::createBufferCopyBlitObject()
        {
            this->objectCB_CopyBlit.offsetX = 0.0f;
            this->objectCB_CopyBlit.offsetY = 0.0f;
            this->objectCB_CopyBlit.scaleX = 2.0f;
            this->objectCB_CopyBlit.scaleY = 2.0f;
			String nameBuffer = "CopyBlitObjectConstants-" + this->name;
			this->pBuffer_CopyBlit = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																			   sizeof(CopyBlitObjectConstants), 
																		  	   (uint8*)&this->objectCB_CopyBlit,
																		  	   false);
			if (!this->pBuffer_CopyBlit)
			{
				String msg = "*********************** VKPipelineGraphicsCopyBlitFromFrame::createBufferCopyBlitObject: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
        }

    void VKPipelineGraphicsCopyBlitFromFrame::CleanupSwapChain()
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

    void VKPipelineGraphicsCopyBlitFromFrame::UpdateDescriptorSets(const VkImageView& imageView)
    {
        VkDescriptorImageInfo imageInfo = { };
        imageInfo.sampler = this->pVKRenderPassCopyBlitFromFrame->poSampler;
        imageInfo.imageView = imageView;
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

        UpdateDescriptorSets(imageInfo);
    }
    void VKPipelineGraphicsCopyBlitFromFrame::UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo)
    {
        StringVector* pDescriptorSetLayoutNames = this->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "VKPipelineGraphicsCopyBlitFromFrame::UpdateDescriptorSets")
        uint32_t count_ds = (uint32_t)this->poDescriptorSets.size();
        for (uint32_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            uint32_t count_names = (uint32_t)pDescriptorSetLayoutNames->size();
            for (uint32_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectCopyBlit = {};
                    bufferInfo_ObjectCopyBlit.buffer = this->pBuffer_CopyBlit->GetVkBuffer();
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
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameDepth)) //TextureFrameDepth
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
                    String msg = "*********************** VKPipelineGraphicsCopyBlitFromFrame::UpdateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

    void VKPipelineGraphicsCopyBlitFromFrame::UpdateBuffer(const CopyBlitObjectConstants& object)
    {
		if (this->pBuffer_CopyBlit == nullptr)
			return;
        this->objectCB_CopyBlit = object;
		this->pBuffer_CopyBlit->UpdateBuffer(0,				
											 sizeof(CopyBlitObjectConstants), 
											 (uint8*)&this->objectCB_CopyBlit);
    }

}; //LostPeterVulkan