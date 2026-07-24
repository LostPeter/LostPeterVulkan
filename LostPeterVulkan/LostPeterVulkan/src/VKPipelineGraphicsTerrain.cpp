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
#include "../include/VKBufferUniform.h"

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

        , poBuffer_TerrainObjectCB(nullptr)
        , poBuffer_MaterialCB(nullptr)
        , poBuffer_TerrainCB(nullptr)
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
        destroyBufferMaterial();
        destroyBufferTerrain();
    }
        void VKPipelineGraphicsTerrain::destroyBufferTerrainObject()
        {
            this->terrainObjectCBs.clear();
			F_DELETE(this->poBuffer_TerrainObjectCB)
        }
        void VKPipelineGraphicsTerrain::destroyBufferMaterial()
        {
            this->materialCBs.clear();
			F_DELETE(this->poBuffer_MaterialCB)
        }
        void VKPipelineGraphicsTerrain::destroyBufferTerrain()
        {
			F_DELETE(this->poBuffer_TerrainCB)
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

        //1> Buffer
        if (this->poBuffer_TerrainObjectCB == nullptr)
        {
            if (!createBufferTerrainObject())
            {
                F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createBufferTerrainObject failed !");
                return false;
            }
        }
        if (this->poBuffer_MaterialCB == nullptr)
        {
            if (!createBufferMaterial())
            {
                F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createBufferMaterial failed !");
                return false;
            }
        }
        if (this->poBuffer_TerrainCB == nullptr)
        {
            if (!createBufferTerrain())
            {
                F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createBufferTerrain failed !");
                return false;
            }
        }
         

        //2> Pipeline
        {
            VkViewportVector aViewports;
            aViewports.push_back(Base::GetWindowPtr()->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(Base::GetWindowPtr()->poScissor);
            VkDynamicStateVector aDynamicStates =
            {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };

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

            this->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-" + this->name,
                                                                              aShaderStageCreateInfos,
                                                                              false, 0, 0,
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                              Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
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
            
            this->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline("PipelineGraphics-Wire-" + this->name,
                                                                                        aShaderStageCreateInfos,
                                                                                        false, 0, 0,
                                                                                        Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                                        Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Normal3Tex2),
                                                                                        Base::GetWindowPtr()->poRenderPass, this->poPipelineLayout, aViewports, aScissors, aDynamicStates,
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

        //3> DescriptorSet
        Base::GetWindowPtr()->createVkDescriptorSets("DescriptorSets-" + this->name, this->poDescriptorSetLayout, this->poDescriptorSets);
        if (this->poDescriptorSets.empty())
        {
            F_LogError("*********************** VKPipelineGraphicsTerrain::Init: createVkDescriptorSets failed !");
            return false;
        }    
        UpdateDescriptorSets();

        return true;
    }
        bool VKPipelineGraphicsTerrain::createBufferTerrainObject()
        {
            this->terrainObjectCBs.clear();
            TerrainObjectConstants toWhole;
            this->terrainObjectCBs.push_back(toWhole);
            float fTerrainSize = (float)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize - 1.0f);
            float fTerrainSizeHalf = fTerrainSize / 2.0f;
            float fTerrainInstanceSize = (float)(VKRenderPassTerrain::c_nInstanceGridVertexCount - 1.0f);
            float fTerrainInstanceSizeHalf = fTerrainInstanceSize / 2.0f;
            for (int i = 0; i < this->m_pVKRenderPassTerrain->poTerrainInstanceCount; i++)
            {
                for (int j = 0; j < this->m_pVKRenderPassTerrain->poTerrainInstanceCount; j++)
                {
                    TerrainObjectConstants toInstance;
                    toInstance.offsetX = j * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf;
                    toInstance.offsetZ = i * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf;
                    this->terrainObjectCBs.push_back(toInstance);
                }
            }
            F_Assert(this->terrainObjectCBs.size() < MAX_OBJECT_TERRAIN_COUNT && "VKPipelineGraphicsTerrain::createBufferTerrainObject")
            VkDeviceSize bufferSize = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
			String nameBuffer = "TerrainObjectConstants-" + this->name;
			this->poBuffer_TerrainObjectCB = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																					   sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size(), 
																					   (uint8*)this->terrainObjectCBs.data(),
																					   false);
			if (!this->poBuffer_TerrainObjectCB)
			{
				String msg = "*********************** VKPipelineGraphicsTerrain::createBufferTerrainObject: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
        }
        bool VKPipelineGraphicsTerrain::createBufferMaterial()
        {
            this->materialCBs.clear();
            for (int i = 0; i < MAX_MATERIAL_COUNT; i++)
            {
                MaterialConstants mcWhole;
                this->materialCBs.push_back(mcWhole);
            }
			String nameBuffer = "MaterialConstants-" + this->name;
			this->poBuffer_MaterialCB = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																				  sizeof(MaterialConstants) * this->materialCBs.size(), 
																				  (uint8*)this->materialCBs.data(),
																				  false);
			if (!this->poBuffer_MaterialCB)
			{
				String msg = "*********************** VKPipelineGraphicsTerrain::createBufferMaterial: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
            return true;
        }
        bool VKPipelineGraphicsTerrain::createBufferTerrain()
        {
            this->terrainCB.textureX = (float)this->m_pVKRenderPassTerrain->poTerrainHeightMapSize;
            this->terrainCB.textureZ = (float)this->m_pVKRenderPassTerrain->poTerrainHeightMapSize;
            this->terrainCB.textureX_Inverse = 1.0f / (this->terrainCB.textureX - 1.0f);
            this->terrainCB.textureZ_Inverse = 1.0f / (this->terrainCB.textureZ - 1.0f);
            this->terrainCB.heightStart = Base::GetWindowPtr()->cfg_terrainHeightStart;
            this->terrainCB.heightMax = Base::GetWindowPtr()->cfg_terrainHeightMax;
            this->terrainCB.terrainSizeX = (float)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize - 1.0f);
            this->terrainCB.terrainSizeZ = (float)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize - 1.0f);

			String nameBuffer = "TerrainConstants-" + this->name;
			this->poBuffer_TerrainCB = Base::GetWindowPtr()->createBufferUniform(nameBuffer,
																				 sizeof(TerrainConstants), 
																				 (uint8*)&this->terrainCB,
																				 false);
			if (!this->poBuffer_TerrainCB)
			{
				String msg = "*********************** VKPipelineGraphicsTerrain::createBufferTerrain: create buffer uniform: [" + nameBuffer + "] failed !";
				F_LogError(msg.c_str());
				throw std::runtime_error(msg);
			}
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
            //<0> PassConstants
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i]->GetVkBuffer();
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  0,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Pass);
            }
            //<1> TerrainObjectConstants
            {
                VkDescriptorBufferInfo bufferInfo_TerrainObject = {};
                bufferInfo_TerrainObject.buffer = this->poBuffer_TerrainObjectCB->GetVkBuffer();
                bufferInfo_TerrainObject.offset = 0;
                bufferInfo_TerrainObject.range = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  1,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_TerrainObject);
            }
            //<2> MaterialConstants
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = this->poBuffer_MaterialCB->GetVkBuffer();
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  2,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Material);
            }
            //<3> InstanceConstants
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
            //<4> Terrain
            {
                VkDescriptorBufferInfo bufferInfo_Terrain = {};
                bufferInfo_Terrain.buffer = this->poBuffer_TerrainCB->GetVkBuffer();
                bufferInfo_Terrain.offset = 0;
                bufferInfo_Terrain.range = sizeof(TerrainConstants);
                Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                  this->poDescriptorSets[i],
                                                                  4,
                                                                  0,
                                                                  1,
                                                                  bufferInfo_Terrain);
            }
            //<5> poTerrainHeightMapImage
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                5,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                                                                this->m_pVKRenderPassTerrain->poTerrainHeightMapImageInfo_NoSampler);
            }
            //<6> poTerrainNormalMapImage
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                6,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                                                                this->m_pVKRenderPassTerrain->poTerrainNormalMapImageInfo_NoSampler);
            }
            //<7> poTerrainDiffuseImage
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                7,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainDiffuseImageInfo);
            }
            //<8> poTerrainNormalImage
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                8,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainNormalImageInfo);
            }
            //<9> poTerrainControlImage
            {
                Base::GetWindowPtr()->pushVkDescriptorSet_Image(descriptorWrites,
                                                                this->poDescriptorSets[i],
                                                                9,
                                                                0,
                                                                1,
                                                                VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                                this->m_pVKRenderPassTerrain->poTerrainControlImageInfo);
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }

    void VKPipelineGraphicsTerrain::UpdateBufferTerrain()
    {
		this->poBuffer_TerrainCB->UpdateBuffer(0,
											   sizeof(TerrainConstants), 
											   (uint8*)&this->terrainCB);
    }

    float VKPipelineGraphicsTerrain::GetTerrainHeight(int x, int z)
    {
        return this->m_pVKRenderPassTerrain->GetTerrainHeight(x, z, this->terrainCB.heightStart, this->terrainCB.heightMax);
    }
    float VKPipelineGraphicsTerrain::GetTerrainHeight(const FVector3& vPos)
    {
        return GetTerrainHeight(vPos.x, vPos.z);
    }
    float VKPipelineGraphicsTerrain::GetTerrainHeight(float x, float z)
    {
        return this->m_pVKRenderPassTerrain->GetTerrainHeight(x, z, this->terrainCB.heightStart, this->terrainCB.heightMax);
    }

    bool VKPipelineGraphicsTerrain::RaytraceTerrain(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FVector3& vPos)
    {
        vPos = FMath::ms_v3Zero;

        FRay ray;
        pCamera->ConvertScreenPos2ToWorldRay(screenX, screenY, vViewport, &ray);

        FPlane plane(FMath::ms_v3UnitY, 0.0f);
        std::pair<bool, float> ret = FMath::Intersects_RayPlane(ray, plane, vPos);
        if (!ret.first)
        {
            return false;
        }
        float fHeight = GetTerrainHeight(vPos.x, vPos.z);
        vPos.y = fHeight;

        return true;
    }

}; //LostPeterVulkan