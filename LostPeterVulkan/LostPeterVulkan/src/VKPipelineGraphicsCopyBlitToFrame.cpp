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

namespace LostPeterVulkan
{
    VKPipelineGraphicsCopyBlitToFrame::VKPipelineGraphicsCopyBlitToFrame(const String& namePipelineGraphics)
        : Base(namePipelineGraphics)

        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline_WireFrame(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)
        
        , pBuffer_CopyBlit(nullptr)

        , pMeshBlit(nullptr)
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
            
            String namePipeline = "PipelineGraphics-CopyBlitToFrame-" + this->name;
            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(namePipeline,
                                                                              aShaderStageCreateInfos,
                                                                              false, 0, 3,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                              Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                              VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                              VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                              VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                              VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                              VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                              VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlitToFrame::Init: Failed to create pipeline graphics for [" + namePipeline + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlitToFrame::Init: [%s] Create pipeline graphics success !", namePipeline.c_str());
            
            String namePipelineWire = "PipelineGraphics-CopyBlitToFrame-Wire-" + this->name;
            this->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(namePipelineWire,
                                                                                        aShaderStageCreateInfos,
                                                                                        false, 0, 3,
                                                                                        Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                        Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                        Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
                                                                                        VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, VK_FALSE, 0.0f, 0.0f, 0.0f, 1.0f,
                                                                                        VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
                                                                                        VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                        VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                        VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsCopyBlitToFrame::Init: Failed to create pipeline graphics wire frame for [" + namePipelineWire + "] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("VKPipelineGraphicsCopyBlitToFrame::Init: [%s] Create pipeline graphics wire frame success !", namePipelineWire.c_str());
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-CopyBlitToFrame-" + this->name, this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsCopyBlitToFrame::Init: createVkDescriptorSets failed !");
            return false;
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

    void VKPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets(const VkDescriptorImageInfo& imageInfo)
    {
        StringVector* pDescriptorSetLayoutNames = this->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "VKPipelineGraphicsCopyBlitToFrame::UpdateDescriptorSets")
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