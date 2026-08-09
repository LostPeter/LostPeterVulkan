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
#include "../include/VKBufferUniform.h"
#include "../include/VKBufferCompute.h"
#include "../include/VKStatePipelineGraphics.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsDepthShadowMap::VKPipelineGraphicsDepthShadowMap(const String& namePipelineGraphics, VKRenderPassShadowMap* pVKRenderPassShadowMap)
        : Base(namePipelineGraphics)
        , m_pVKRenderPassShadowMap(pVKRenderPassShadowMap)

        //PipelineGraphics-ShadowMapDepth
        , pDescriptorSetLayout_ShadowMapDepth(nullptr)
        , poStatePipelineGraphics_ShadowMapDepth(nullptr)

        //PipelineGraphics-ShadowMapDepthCull
        , pDescriptorSetLayout_ShadowMapDepthCull(nullptr)
        , poStatePipelineGraphics_ShadowMapDepthCull(nullptr)
        
        //ObjectConstants
        , poBuffer_ObjectWorldCB(nullptr)

    {

    }   

    VKPipelineGraphicsDepthShadowMap::~VKPipelineGraphicsDepthShadowMap()
    {
        Destroy();
    }

    void VKPipelineGraphicsDepthShadowMap::Destroy()
    {
        CleanupSwapChain();
        destroyBufferObjectWorldCB();
    }
        void VKPipelineGraphicsDepthShadowMap::destroyBufferObjectWorldCB()
        {
			F_DELETE(this->poBuffer_ObjectWorldCB)
            this->objectWorldCBs.clear();
        }

    bool VKPipelineGraphicsDepthShadowMap::Init()
    {
        //ObjectConstants
        if (this->poBuffer_ObjectWorldCB == nullptr)
        {
            if (!createBufferObjectWorldCB())
            {
                F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::Init: createBufferObjectWorldCB failed !");
                return false;
            }
        }
        
        return true;
    }
    bool VKPipelineGraphicsDepthShadowMap::InitShadowMapDepth(DescriptorSetLayout* pDSL,
                                                              VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
		this->pDescriptorSetLayout_ShadowMapDepth = pDSL;

		this->poStatePipelineGraphics_ShadowMapDepth = createGraphicsPipeline("PipelineGraphics-ShadowMapDepth-" + this->name,
																			  pDSL,
																			  aShaderStageCreateInfos);
        if (this->poStatePipelineGraphics_ShadowMapDepth == nullptr)
        {
            F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::InitShadowMapDepth: createGraphicsPipeline failed !");
            return false;
        }
        updateDescriptorSets(this->poStatePipelineGraphics_ShadowMapDepth, 
                             this->poStatePipelineGraphics_ShadowMapDepth->poDescriptorSets,
                             this->poBuffer_ObjectWorldCB,
                             nullptr,
                             nullptr,
                             nullptr);

        return true;
    }
    bool VKPipelineGraphicsDepthShadowMap::InitShadowMapDepthCull(DescriptorSetLayout* pDSL,
                                                                  VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
    {
		this->pDescriptorSetLayout_ShadowMapDepthCull = pDSL;

		this->poStatePipelineGraphics_ShadowMapDepthCull = createGraphicsPipeline("PipelineGraphics-ShadowMapDepth-Cull-" + this->name,
																				  pDSL,
																			      aShaderStageCreateInfos);
        if (this->poStatePipelineGraphics_ShadowMapDepthCull == nullptr)
        {
            F_LogError("*********************** VKPipelineGraphicsDepthShadowMap::InitShadowMapDepthCull: createGraphicsPipeline failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineGraphicsDepthShadowMap::createBufferObjectWorldCB()
        {
			String nameBuffer = "ObjectConstants-" + this->name;
			this->poBuffer_ObjectWorldCB = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																					 sizeof(ObjectConstants) * MAX_OBJECT_COUNT, 
																					 nullptr,
																					 false);
			if (!this->poBuffer_ObjectWorldCB)
			{
				String msg = "*********************** VKPipelineGraphicsDepthShadowMap::createBufferObjectWorldCB: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}												
            F_LogInfo("VKPipelineGraphicsDepthShadowMap::createBufferObjectWorldCB: Create Uniform ObjectWorld constant buffer success !");
            return true;
        }
        VKStatePipelineGraphics* VKPipelineGraphicsDepthShadowMap::createGraphicsPipeline(const String& nameGraphicsPipeline,
																						  DescriptorSetLayout* pDSL,
																						  VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos)
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
			VKStatePipelineGraphics* pStatePipelineGraphics = Base::GetWindowPtr()->createStatePipelineGraphics(nameGraphicsPipeline,
																												pDSL,
																												aShaderStageCreateInfos,
																												F_MeshVertex_Pos3Color4Normal3Tex2,
																												false, 0, 3,
																												this->m_pVKRenderPassShadowMap->poRenderPass, aViewports, aScissors, aDynamicStates,
																												VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_TRUE, 0.0f, 0.0f, 0.0f, 1.0f,
																												VK_TRUE, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL,
																												VK_FALSE, stencilOpFront, stencilOpBack, 
																												aColorBlendAttachmentState);
			if (pStatePipelineGraphics == nullptr)
			{
				String msg = "*********************** VKPipelineGraphicsDepthShadowMap::createGraphicsPipeline: Failed to create pipeline graphics for: " + nameGraphicsPipeline;
				F_LogError(msg.c_str());
				throw std::runtime_error(msg.c_str());
			}
			F_LogInfo("VKPipelineGraphicsDepthShadowMap::createGraphicsPipeline: [%s] Create pipeline graphics success !", nameGraphicsPipeline.c_str());
			
            return pStatePipelineGraphics;
        }

    void VKPipelineGraphicsDepthShadowMap::CleanupSwapChain()
    {
        //poStatePipelineGraphics_ShadowMapDepth
		F_DELETE(this->poStatePipelineGraphics_ShadowMapDepth)
        //poStatePipelineGraphics_ShadowMapDepthCull
		F_DELETE(this->poStatePipelineGraphics_ShadowMapDepthCull)
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
	void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects, int count)
	{
		size_t count_object = objects.size();
        for (size_t i = 0; i < count_object; i++)
        {
			if (i > count - 1)
				break;
            this->objectWorldCBs.push_back(objects[i]);
        }
	}
    void VKPipelineGraphicsDepthShadowMap::UpdateBuffer_ObjectWorld_Update()
    {
        if (this->objectWorldCBs.size() <= 0)
            return;

        if (this->poBuffer_ObjectWorldCB != nullptr)
        {
			this->poBuffer_ObjectWorldCB->UpdateBuffer(0,
													   sizeof(ObjectConstants) * this->objectWorldCBs.size(),
													   (uint8*)this->objectWorldCBs.data());
        }
    }

    void VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSet_ShadowMapDepth()
    {
        updateDescriptorSets(this->poStatePipelineGraphics_ShadowMapDepthCull, 
                             this->poStatePipelineGraphics_ShadowMapDepthCull->poDescriptorSets,
                             this->poBuffer_ObjectWorldCB,
                             nullptr,
                             nullptr,
                             nullptr);
    }

    void VKPipelineGraphicsDepthShadowMap::UpdateDescriptorSet_ShadowMapDepthCull(VkDescriptorSetVector* pDescriptorSets, 
                                                                                  VKBufferUniform* pCB_CullInstance,
                                                                                  VKBufferCompute* pCB_CullObjectInstances,
                                                                                  VKBufferCompute* pCB_Result)
    {
        updateDescriptorSets(this->poStatePipelineGraphics_ShadowMapDepthCull,
							 *pDescriptorSets,
                             nullptr,
                             pCB_CullInstance,
                             pCB_CullObjectInstances,
                             pCB_Result);
    }

    void VKPipelineGraphicsDepthShadowMap::updateDescriptorSets(VKStatePipelineGraphics* pStatePipelineGraphics,
																VkDescriptorSetVector& vkDescriptorSets,
                                                                VKBufferUniform* pCB_ObjectWorld,
                                                                VKBufferUniform* pCB_CullInstance,
                                                                VKBufferCompute* pCB_CullObjectInstances,
                                                                VKBufferCompute* pCB_Result)
    {
        uint32_t count_descriptorsets = (uint32_t)vkDescriptorSets.size();
        for (uint32_t i = 0; i < count_descriptorsets; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            uint32_t count_name = (uint32_t)pStatePipelineGraphics->pDescriptorSetLayout->aLayouts.size();
            for (uint32_t j = 0; j < count_name; j++)
            {
                String& nameDescriptorSet = pStatePipelineGraphics->pDescriptorSetLayout->aLayouts[j];

                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i]->GetVkBuffer();
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
                    if (pCB_ObjectWorld != nullptr)
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = pCB_ObjectWorld->GetVkBuffer();
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_Object);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_CullInstance)) //CullInstance
                {
                    if (pCB_CullInstance != VK_NULL_HANDLE)
                    {
                        VkDescriptorBufferInfo bufferInfo_CullInstance = {};
                        bufferInfo_CullInstance.buffer = pCB_CullInstance->GetVkBuffer();
                        bufferInfo_CullInstance.offset = 0;
                        bufferInfo_CullInstance.range = sizeof(CullInstanceConstants);
                        Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_CullInstance);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWObjectCullInstance)) //BufferRWObjectCullInstance
                {
                    if (pCB_CullObjectInstances != nullptr)
                    {
                        VkDescriptorBufferInfo bufferInfo_CullObjectInstance = {};
                        bufferInfo_CullObjectInstance.buffer = pCB_CullObjectInstances->GetVkBuffer();
                        bufferInfo_CullObjectInstance.offset = 0;
                        bufferInfo_CullObjectInstance.range = (VkDeviceSize)pCB_CullObjectInstances->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                          vkDescriptorSets[i],
                                                                          j,
                                                                          0,
                                                                          1,
                                                                          bufferInfo_CullObjectInstance);
                    }
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
                {
                    if (pCB_Result != nullptr)
                    {
                        VkDescriptorBufferInfo bufferInfo_Result = {};
                        bufferInfo_Result.buffer = pCB_Result->GetVkBuffer();
                        bufferInfo_Result.offset = 0;
                        bufferInfo_Result.range = (VkDeviceSize)pCB_Result->GetBufferSize();
                        Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
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