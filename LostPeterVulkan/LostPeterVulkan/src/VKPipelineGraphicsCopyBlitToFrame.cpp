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

#include "../include/VKPipelineGraphicsCopyBlitToFrame.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/VKBufferUniform.h"
#include "../include/VKStatePipelineGraphics.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsCopyBlitToFrame::VKPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)

        , pBuffer_CopyBlit(nullptr)

        , pMeshBlit(nullptr)
		, pDescriptorSetLayout(nullptr)
		, poStatePipelineGraphics(nullptr)
    {

    }

    VKPipelineGraphicsCopyBlitToFrame::~VKPipelineGraphicsCopyBlitToFrame()
    {
        Destroy();
    }

    void VKPipelineGraphicsCopyBlitToFrame::Destroy()
    {
        CleanupSwapChain();
        destroyBufferCopyBlitObject();
    }
        void VKPipelineGraphicsCopyBlitToFrame::destroyBufferCopyBlitObject()
        {
			F_DELETE(this->pBuffer_CopyBlit)
        }


    bool VKPipelineGraphicsCopyBlitToFrame::Init(Mesh* pMesh,
                                                 DescriptorSetLayout* pDSL,
                                                 VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
        this->pMeshBlit = pMesh;
		this->pDescriptorSetLayout = pDSL;

        //1> Buffer
        if (this->pBuffer_CopyBlit == nullptr)
        {
            if (!createBufferCopyBlitObject())
            {
                F_LogError("*********************** VKPipelineGraphicsCopyBlitToFrame::Init: createBufferCopyBlitObject failed !");
                return false;
            }
        }

        //2> Pipeline
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
			
			//poStatePipelineGraphics
            String nameStatePipelineGraphics = "PipelineGraphics-CopyBlitToFrame-" + this->name;
            this->poStatePipelineGraphics = Base::GetWindowPtr()->createStatePipelineGraphics(nameStatePipelineGraphics,
																						      this->pDescriptorSetLayout,
																							  aShaderStageCreateInfos,
																							  F_MeshVertex_Pos3Color4Tex2,
																							  false, 0, 3,
																							  Base::GetWindowPtr()->poRenderPass, aViewports, aScissors, aDynamicStates,
																							  VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
																							  VK_TRUE, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
																							  VK_FALSE, stencilOpFront, stencilOpBack, 
																							  VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
																							  VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
																							  VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poStatePipelineGraphics == nullptr)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlitToFrame::Init: Failed to create pipeline graphics for [" + nameStatePipelineGraphics + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlitToFrame::Init: [%s] Create pipeline graphics success !", nameStatePipelineGraphics.c_str());
        }

        return true;
    }
        bool VKPipelineGraphicsCopyBlitToFrame::createBufferCopyBlitObject()
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
				String msg = "*********************** VKPipelineGraphicsCopyBlitToFrame::createBufferCopyBlitObject: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
        }

    void VKPipelineGraphicsCopyBlitToFrame::CleanupSwapChain()
    {
		F_DELETE(this->poStatePipelineGraphics)
    }  

    void VKPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo)
    {
        uint32_t count_ds = (uint32_t)this->poStatePipelineGraphics->poDescriptorSets.size();
        for (uint32_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            uint32_t count_names = (uint32_t)this->poStatePipelineGraphics->pDescriptorSetLayout->aLayouts.size();
            for (uint32_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = this->poStatePipelineGraphics->pDescriptorSetLayout->aLayouts[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectCopyBlit = {};
                    bufferInfo_ObjectCopyBlit.buffer = this->pBuffer_CopyBlit->GetVkBuffer();
                    bufferInfo_ObjectCopyBlit.offset = 0;
                    bufferInfo_ObjectCopyBlit.range = sizeof(CopyBlitObjectConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->poStatePipelineGraphics->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_ObjectCopyBlit);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameColor)) //TextureFrameColor
                {
                    Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                    this->poStatePipelineGraphics->poDescriptorSets[i],
                                                                    j,
                                                                    0,
                                                                    1,
                                                                    VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                    const_cast<VkDescriptorImageInfo&>(imageInfo));
                }
                else
                {
                    String msg = "*********************** VKPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

    void VKPipelineGraphicsCopyBlitToFrame::UpdateBuffer(const CopyBlitObjectConstants& object)
    {
		if (this->pBuffer_CopyBlit == nullptr)
			return;
        this->objectCB_CopyBlit = object;
		this->pBuffer_CopyBlit->UpdateBuffer(0,				
											 sizeof(CopyBlitObjectConstants), 
											 (uint8*)&this->objectCB_CopyBlit);
    }

}; //LostPeterVulkan