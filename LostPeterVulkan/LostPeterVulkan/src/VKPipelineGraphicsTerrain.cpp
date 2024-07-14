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

#include "../include/VKPipelineGraphicsTerrain.h"
#include "../include/VKRenderPassTerrain.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    VKPipelineGraphicsTerrain::VKPipelineGraphicsTerrain(const String& namePipelineGraphics, VKRenderPassTerrain* pVKRenderPassTerrain)
        : Base(namePipelineGraphics)

        , m_pVKRenderPassTerrain(pVKRenderPassTerrain)
        , nameDescriptorSetLayout("")
        , poDescriptorSetLayoutNames(nullptr)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipeline_WireFrame(VK_NULL_HANDLE)
        , poPipeline(VK_NULL_HANDLE)

        , poBuffer_TerrainObjectCB(VK_NULL_HANDLE)
        , poBufferMemory_TerrainObjectCB(VK_NULL_HANDLE)
    {

    }

    VKPipelineGraphicsTerrain::~VKPipelineGraphicsTerrain()
    {
        Destroy();
    }

    void VKPipelineGraphicsTerrain::Destroy()
    {
        CleanupSwapChain();
        destroyBufferTerrainObject();
    }
        void VKPipelineGraphicsTerrain::destroyBufferTerrainObject()
        {
            this->terrainObjectCBs.clear();
            if (this->poBuffer_TerrainObjectCB != VK_NULL_HANDLE)
            {
                Base::GetWindowPtr()->destroyVkBuffer(this->poBuffer_TerrainObjectCB, this->poBufferMemory_TerrainObjectCB);
            }
            this->poBuffer_TerrainObjectCB = VK_NULL_HANDLE;
            this->poBufferMemory_TerrainObjectCB = VK_NULL_HANDLE;
        }

    bool VKPipelineGraphicsTerrain::Init(const String& descriptorSetLayout,
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
        if (this->poBuffer_TerrainObjectCB == VK_NULL_HANDLE)
        {
            if (!createBufferTerrainObject())
            {
                F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createBufferTerrainObject failed !");
                return false;
            }
        }

        //2> VkPipeline
        {
            VkViewportVector aViewports;
            aViewports.push_back(Base::GetWindowPtr()->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(Base::GetWindowPtr()->poScissor);

            VkPrimitiveTopology vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            VkFrontFace vkFrontFace = VK_FRONT_FACE_CLOCKWISE;
            VkPolygonMode vkPolygonMode = VK_POLYGON_MODE_FILL;
            VkCullModeFlagBits vkCullModeFlagBits = VK_CULL_MODE_BACK_BIT;
            VkBool32 depthBiasEnable = VK_FALSE;
            float depthBiasConstantFactor = 0.0f;
            float depthBiasClamp = 0.0f;
            float depthBiasSlopeFactor = 0.0f;
            float lineWidth = 1.0f;
            VkBool32 isDepthTest = VK_TRUE;
            VkBool32 isDepthWrite = VK_TRUE; 
            VkCompareOp vkDepthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; 
            VkBool32 isStencilTest = VK_FALSE;
            VkStencilOpState vkStencilOpFront; 
            VkStencilOpState vkStencilOpBack; 
            VkBool32 isBlend = VK_FALSE;
            VkBlendFactor vkBlendColorFactorSrc = VK_BLEND_FACTOR_ONE; 
            VkBlendFactor vkBlendColorFactorDst = VK_BLEND_FACTOR_ZERO; 
            VkBlendOp vkBlendColorOp = VK_BLEND_OP_ADD;
            VkBlendFactor vkBlendAlphaFactorSrc = VK_BLEND_FACTOR_ONE;
            VkBlendFactor vkBlendAlphaFactorDst = VK_BLEND_FACTOR_ZERO; 
            VkBlendOp vkBlendAlphaOp = VK_BLEND_OP_ADD;
            VkColorComponentFlags vkColorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                                              false, 0, 0,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                              Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                              vkPrimitiveTopology, vkFrontFace, vkPolygonMode, vkCullModeFlagBits, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
                                                                              isDepthTest, isDepthWrite, vkDepthCompareOp,
                                                                              isStencilTest, vkStencilOpFront, vkStencilOpBack, 
                                                                              isBlend, vkBlendColorFactorSrc, vkBlendColorFactorDst, vkBlendColorOp,
                                                                              vkBlendAlphaFactorSrc, vkBlendAlphaFactorDst, vkBlendAlphaOp,
                                                                              vkColorWriteMask);
            if (this->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsTerrain::Init: Failed to create pipeline graphics for [PipelineGraphics_Terrain] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            F_LogInfo("VKPipelineGraphicsTerrain::Init: [PipelineGraphics_Terrain] Create pipeline graphics success !");
            
            this->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                                                        false, 0, 0,
                                                                                        Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                                        Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                                        Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                                        vkPrimitiveTopology, vkFrontFace, VK_POLYGON_MODE_LINE, vkCullModeFlagBits, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
                                                                                        isDepthTest, isDepthWrite, vkDepthCompareOp,
                                                                                        isStencilTest, vkStencilOpFront, vkStencilOpBack, 
                                                                                        isBlend, vkBlendColorFactorSrc, vkBlendColorFactorDst, vkBlendColorOp,
                                                                                        vkBlendAlphaFactorSrc, vkBlendAlphaFactorDst, vkBlendAlphaOp,
                                                                                        vkColorWriteMask);
            if (this->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** VKPipelineGraphicsTerrain::Init: Failed to create pipeline graphics wire frame for [PipelineGraphics_Terrain] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            F_LogInfo("VKPipelineGraphicsTerrain::Init: [PipelineGraphics_Terrain] Create pipeline graphics wire frame success !");
        }

        //3> VkDescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets(this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createVkDescriptorSets failed !");
            return false;
        }

        return true;
    }
        bool VKPipelineGraphicsTerrain::createBufferTerrainObject()
        {
            this->terrainObjectCBs.clear();
            TerrainObjectConstants toWhole;
            this->terrainObjectCBs.push_back(toWhole);
            float fTerrainSize = (float)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize - 1.0f);
            float fTerrainSizeHalf = fTerrainSize / 2.0f;
            float fTerrainInstanceSize = (float)(this->m_pVKRenderPassTerrain->poTerrainGridInstanceVertexCount - 1.0f);
            float fTerrainInstanceSizeHalf = fTerrainInstanceSize / 2.0f;
            int nHalfInstanceCount = this->m_pVKRenderPassTerrain->poTerrainGridInstanceCount / 2;
            for (int i = 0; i < this->m_pVKRenderPassTerrain->poTerrainGridInstanceCount; i++)
            {
                for (int j = 0; j < this->m_pVKRenderPassTerrain->poTerrainGridInstanceCount; j++)
                {
                    FVector3 vPos = FVector3(j * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf,
                                                0.0f,
                                                i * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf);
                    TerrainObjectConstants toInstance;
                    toInstance.g_MatWorld = FMath::Translate(vPos);
                    this->terrainObjectCBs.push_back(toWhole);
                }
            }
            VkDeviceSize bufferSize = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TerrainObjectCB, this->poBufferMemory_TerrainObjectCB);
            Base::GetWindowPtr()->updateVKBuffer(0, bufferSize, &this->terrainObjectCBs[0], this->poBufferMemory_TerrainObjectCB);
            return true;
        }

    void VKPipelineGraphicsTerrain::CleanupSwapChain()
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

    void VKPipelineGraphicsTerrain::UpdateDescriptorSets()
    {
        size_t count = this->poDescriptorSets.size();
        for (size_t i = 0; i < count; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            //(0) PassConstants
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  0,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Pass);
            }
            //(1) TerrainObjectConstants
            {
                VkDescriptorBufferInfo bufferInfo_TerrainObject = {};
                bufferInfo_TerrainObject.buffer = this->poBuffer_TerrainObjectCB;
                bufferInfo_TerrainObject.offset = 0;
                bufferInfo_TerrainObject.range = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  1,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_TerrainObject);
            }
            //(2) MaterialConstants
            {
                // VkDescriptorBufferInfo bufferInfo_Material = {};
                // bufferInfo_Material.buffer = this->poBuffers_MaterialCB[i];
                // bufferInfo_Material.offset = 0;
                // bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                // Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                //                                                   this->poDescriptorSets[i],
                //                                                   2,
                //                                                   0,
                //                                                   1,
                //                                                   bufferInfo_Material);
            }
            //(3) InstanceConstants
            {
                // VkDescriptorBufferInfo bufferInfo_Instance = {};
                // bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
                // bufferInfo_Instance.offset = 0;
                // bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                // Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                //                                                   this->poDescriptorSets[i],
                //                                                   3,
                //                                                   0,
                //                                                   1,
                //                                                   bufferInfo_Instance);
            }
            //(4) texture2DArrayDiffuse
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                4,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainDiffuseImageInfo);
            }
            //(5) texture2DArrayNormal
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                5,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainNormalImageInfo);
            }
            //(6) texture2DArrayControl
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                6,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainControlImageInfo);
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

}; //LostPeterVulkan