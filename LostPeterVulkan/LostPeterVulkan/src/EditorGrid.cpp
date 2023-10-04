/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/EditorGrid.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/PipelineGraphics.h"

namespace LostPeter
{
    const String EditorGrid::s_strNameShader_Grid_Vert = "vert_editor_grid";
    const String EditorGrid::s_strNameShader_Grid_Frag = "frag_editor_grid";
    EditorGrid::EditorGrid()
        : EditorBase("EditorGrid")

        , poBuffers_ObjectCB(VK_NULL_HANDLE)
        , poBuffersMemory_ObjectCB(VK_NULL_HANDLE)
        , isNeedUpdate(true)
    {

    }
    EditorGrid::~EditorGrid()
    {
        Destroy();
    }
    void EditorGrid::Destroy()
    {
        destroyPipelineGraphics();
        destroyBufferUniforms();
        destroyShaders();
        destroyMeshes();
    }
    void EditorGrid::Init()
    {
        EditorBase::Init();
    }
    void EditorGrid::UpdateCBs()
    {
        if (!IsNeedUpdate())
            return;
        SetIsNeedUpdate(false);

        Base::GetWindowPtr()->updateVKBuffer(0, sizeof(GridObjectConstants), &gridObjectCB, this->poBuffersMemory_ObjectCB);
    }
    void EditorGrid::initConfigs()
    {
        //1> Mesh
        {
            MeshInfo* pMI = new MeshInfo("EditorGrid",
                                         "",
                                         F_Mesh_Geometry,
                                         F_MeshVertex_Pos3Color4Tex2,
                                         F_MeshGeometry_EntityGrid,
                                         nullptr,
                                         false,
                                         true,
                                         FMath::RotateX(90.0f));
            this->aMeshInfos.push_back(pMI);
        }
        //2> Shader
        {
            //Vert
            ShaderModuleInfo siVert;
            siVert.nameShader = s_strNameShader_Grid_Vert;
            siVert.nameShaderType = "vert";
            siVert.pathShader = "Assets/Shader/editor_grid.vert.spv";
            this->aShaderModuleInfos.push_back(siVert);
            //Frag
            ShaderModuleInfo siFrag;
            siFrag.nameShader = s_strNameShader_Grid_Frag;
            siFrag.nameShaderType = "frag";
            siFrag.pathShader = "Assets/Shader/editor_grid.frag.spv";
            this->aShaderModuleInfos.push_back(siFrag);
        }
        //3> BufferUniform
        {
            this->gridObjectCB.g_MatWorld = FMath::FromTRS(FVector3(0.0f, 0.0f, 0.0f),
                                                           FVector3(0.0f, 0.0f, 0.0f),
                                                           FVector3(1024.0f, 1024.0f, 1024.0f));
        }
        //4> DescriptorSetLayout
        {
            this->nameDescriptorSetLayout = "Pass-ObjectGrid";
            this->aNameDescriptorSetLayouts = FUtilString::Split(this->nameDescriptorSetLayout, "-");
        }
    }
    void EditorGrid::initBufferUniforms()
    {
        VkDeviceSize bufferSize = sizeof(GridObjectConstants);
        Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);
        SetIsNeedUpdate(true);
    }
    void EditorGrid::initPipelineGraphics()
    {
        String namePipelineGraphics = "PipelineG-" + GetName();
        this->pPipelineGraphics = new PipelineGraphics(namePipelineGraphics);
        this->pPipelineGraphics->nameDescriptorSetLayout = this->nameDescriptorSetLayout;
        this->pPipelineGraphics->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts;
        //1> DescriptorSetLayout 
        this->pPipelineGraphics->poDescriptorSetLayout = this->poDescriptorSetLayout;
        //2> DescriptorSets
        Base::GetWindowPtr()->createVkDescriptorSets(this->pPipelineGraphics->poDescriptorSetLayout, this->pPipelineGraphics->poDescriptorSets);
        updateDescriptorSets_Graphics();
        //3> PipelineLayout
        this->pPipelineGraphics->poPipelineLayout = this->poPipelineLayout;
        //4> Pipeline
        {
            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
            if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_Grid_Vert,
                                                                            "",
                                                                            "",
                                                                            "",
                                                                            s_strNameShader_Grid_Frag,
                                                                            this->mapShaderModules,
                                                                            aShaderStageCreateInfos_Graphics))
            {
                String msg = "EditorGrid::initPipelineGraphics: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            VkViewportVector aViewports;
            aViewports.push_back(Base::GetWindowPtr()->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(Base::GetWindowPtr()->poScissor);

            VkStencilOpState stencilOpFront; 
            VkStencilOpState stencilOpBack; 

            //pPipelineGraphics->poPipeline
            this->pPipelineGraphics->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                 false, 0, 3,
                                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                 Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                                 VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, 1.0f,
                                                                                                 VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS,
                                                                                                 VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                 VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                 VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                 VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "EditorGrid::initPipelineGraphics: Failed to create pipeline graphics for [EditorGrid] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorGrid::initPipelineGraphics: [EditorGrid] Create pipeline graphics success !");

            //pPipelineGraphics->poPipeline_WireFrame
            this->pPipelineGraphics->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                           false, 0, 3,
                                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                           Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                                           VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, 1.0f,
                                                                                                           VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS,
                                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                           VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "EditorGrid::initPipelineGraphics: Failed to create pipeline graphics wire frame for [EditorGrid] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorGrid::initPipelineGraphics: [EditorGrid] Create pipeline graphics wire frame success !");
        }
    }
    void EditorGrid::updateDescriptorSets_Graphics()
    {
        StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorGrid::updateDescriptorSets_Graphics")
        size_t count_ds = this->pPipelineGraphics->poDescriptorSets.size();
        for (size_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->pPipelineGraphics->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Pass);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectGrid)) //ObjectGrid
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectGrid = {};
                    bufferInfo_ObjectGrid.buffer = this->poBuffers_ObjectCB;
                    bufferInfo_ObjectGrid.offset = 0;
                    bufferInfo_ObjectGrid.range = sizeof(GridObjectConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->pPipelineGraphics->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_ObjectGrid);
                }
                else
                {
                    String msg = "EditorGrid::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }
    void EditorGrid::destroyBufferUniforms()
    {
        if (this->poBuffers_ObjectCB != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);
        }
        this->poBuffers_ObjectCB = VK_NULL_HANDLE;
        this->poBuffersMemory_ObjectCB = VK_NULL_HANDLE;
    }
    void EditorGrid::destroyPipelineGraphics()
    {
        EditorBase::destroyPipelineGraphics();
    }

    void EditorGrid::CleanupSwapChain()
    {   
        EditorBase::CleanupSwapChain();

    }
    void EditorGrid::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();

    }

}; //LostPeter