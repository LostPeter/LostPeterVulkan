/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_006_depth.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


static const int g_CountLen = 4;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "viking_room",      "Assets/Mesh/Model/viking_room/viking_room.obj",    "Assets/Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Mesh/Model/bunny/bunny.obj",                "Assets/Texture/Common/default_white.bmp", //bunny

    "plane",            "Assets/Mesh/Common/plane.fbx",                     "Assets/Texture/Common/texture2d.jpg", //plane
    "cube",             "Assets/Mesh/Common/cube.fbx",                      "Assets/Texture/Common/texture2d.jpg", //cube
};

static FVector3 g_tranformModels[3 * g_CountLen] = 
{
    FVector3(  -1,   0,   -1),     FVector3(     0,   0, 0),    FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   1,   0,   -1),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny

    FVector3(  -1,   0,    1),     FVector3(   -90, 0, 0),      FVector3(0.02f,  0.02f,  0.02f), //plane
    FVector3(   1,   0,    1),     FVector3(     0, 0, 0),      FVector3(0.01f,  0.01f,  0.01f), //cube
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny

    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube
};

static bool g_isTranformLocalModels[g_CountLen] = 
{
    true, //viking_room
    false, //bunny

    false, //plane
    false, //cube
};

static bool g_isFlipYModels[g_CountLen] = 
{
    false, //viking_room
    false, //bunny

    true, //plane
    true, //cube
};


Vulkan_006_Depth::Vulkan_006_Depth(int width, int height, String name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->poTypeVertex = F_MeshVertex_Pos3Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/pos3_color4_tex2_ubo.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/pos3_color4_tex2_ubo.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/Common/texture2d.jpg";

    this->cfg_cameraPos = FVector3(-0.65f, 2.5f, -4.0f);
}

void Vulkan_006_Depth::createCamera()
{
    VulkanWindow::createCamera();
    
    cameraReset();
}

void Vulkan_006_Depth::loadModel_Custom()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->nameModel = g_pathModels[3 * i + 0];
        pModelObject->pathModel = g_pathModels[3 * i + 1];
        pModelObject->pathTexture = g_pathModels[3 * i + 2];

        bool isFlipY = g_isFlipYModels[i];
        bool isTransformLocal = g_isTranformLocalModels[i];
        
        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTransformLocal, g_tranformLocalModels[i]))
        {
            String msg = "*********************** Vulkan_006_Depth::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        pModelObject->poMatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0],
                                                     g_tranformModels[i * 3 + 1],
                                                     g_tranformModels[i * 3 + 2]); 

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "*********************** Vulkan_006_Depth::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        m_aModelObjects.push_back(pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
}
bool Vulkan_006_Depth::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** Vulkan_006_Depth::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        FMeshVertex& vertex = meshData.vertices[i];
        FVertex_Pos3Color4Tex2 v;
        v.pos = vertex.pos;
        v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
        v.texCoord = vertex.texCoord;

        if (isTransformLocal)
        {
            v.pos = FMath::Transform(matTransformLocal, v.pos);
        }

        pModelObject->vertices.push_back(v);
    }

    int count_index = (int)meshData.indices32.size();
    pModelObject->indices.clear();
    pModelObject->indices.reserve(count_index);
    for (int i = 0; i < count_index; i++)
    {
        pModelObject->indices.push_back(meshData.indices32[i]);
    }
    pModelObject->poVertexCount = (uint32_t)pModelObject->vertices.size();
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(FVertex_Pos3Color4Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    F_LogInfo("Vulkan_006_Depth::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
              pModelObject->nameModel.c_str(),
              (int)pModelObject->vertices.size(), 
              (int)pModelObject->indices.size());

    //2> createVertexBuffer
    createVertexBuffer("Vertex-" + pModelObject->nameModel, pModelObject->poVertexBuffer_Size, pModelObject->poVertexBuffer_Data, pModelObject->poVertexBuffer, pModelObject->poVertexBufferMemory);

    //3> createIndexBuffer
    if (pModelObject->poIndexBuffer_Size > 0 &&
        pModelObject->poIndexBuffer_Data != nullptr)
    {
        createIndexBuffer("Index-" + pModelObject->nameModel, pModelObject->poIndexBuffer_Size, pModelObject->poIndexBuffer_Data, pModelObject->poIndexBuffer, pModelObject->poIndexBufferMemory);
    }

    return true;
}
bool Vulkan_006_Depth::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        String nameTexture;
        String pathBase;
        FUtilString::SplitFileName(pModelObject->pathTexture, nameTexture, pathBase);
        createTexture2D(nameTexture, pModelObject->pathTexture, pModelObject->poMipMapCount, pModelObject->poTextureImage, pModelObject->poTextureImageMemory);
        createVkImageView(nameTexture, pModelObject->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipMapCount, 1, pModelObject->poTextureImageView);
        createVkSampler(nameTexture, pModelObject->poMipMapCount, pModelObject->poTextureSampler);

        F_LogInfo("Vulkan_006_Depth::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void Vulkan_006_Depth::createCustomCB()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        ObjectConstants objectConstants;
        objectConstants.g_MatWorld = pModelObject->poMatWorld;
        pModelObject->objectCBs.push_back(objectConstants);

        VkDeviceSize bufferSize = sizeof(ObjectConstants) * pModelObject->objectCBs.size();
        pModelObject->poBuffers_ObjectCB.resize(count_sci);
        pModelObject->poBuffersMemory_ObjectCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            String nameBuffer = "ObjectConstants-" + FUtilString::SavePointI(FPointI(i,j));
            createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
        }
    }
}

void Vulkan_006_Depth::createGraphicsPipeline_Custom()
{
    String namePathBase;

    //1> Shader
    VkShaderModule vertShaderModule;
    if (!this->cfg_shaderVertex_Path.empty())
    {
        String nameVertexShader;
        FUtilString::SplitFileName(this->cfg_shaderVertex_Path, nameVertexShader, namePathBase);
        vertShaderModule = createVkShaderModule(nameVertexShader, "VertexShader: ", this->cfg_shaderVertex_Path);
    }
    VkShaderModule fragShaderModule;
    if (!this->cfg_shaderFragment_Path.empty())
    {
        String nameFragmentShader;
        FUtilString::SplitFileName(this->cfg_shaderFragment_Path, nameFragmentShader, namePathBase);
        fragShaderModule = createVkShaderModule(nameFragmentShader, "FragmentShader: ", this->cfg_shaderFragment_Path);
    }

    //2> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //3> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        //poPipelineGraphics
        pModelObject->poPipelineGraphics = createVkGraphicsPipeline("PipelineGraphics-" + pModelObject->nameModel,
                                                                    vertShaderModule, "main",
                                                                    fragShaderModule, "main",
                                                                    Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                    Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                    this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                    pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                    pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                    pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                    pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                    pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                    pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkGraphicsPipeline("PipelineGraphics-Wire-" + pModelObject->nameModel,
                                                                              vertShaderModule, "main",
                                                                              fragShaderModule, "main",
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex),
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                              this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                              pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                              pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                              pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                              pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                              pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                              pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline wire frame !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthTest
        pModelObject->poPipelineGraphics_NoDepthTest = createVkGraphicsPipeline("PipelineGraphics-NoDepthTest-" + pModelObject->nameModel,
                                                                                vertShaderModule, "main",
                                                                                fragShaderModule, "main",
                                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                VK_FALSE, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                                pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthTest == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth test !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthWrite
        pModelObject->poPipelineGraphics_NoDepthWrite = createVkGraphicsPipeline("PipelineGraphics-NoDepthWrite-" + pModelObject->nameModel,
                                                                                 vertShaderModule, "main",
                                                                                 fragShaderModule, "main",
                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                 this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                 pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                 pModelObject->cfg_isDepthTest, VK_FALSE, pModelObject->cfg_DepthCompareOp,
                                                                                 pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                 pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                 pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                 pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthWrite == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth write !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthTestWrite
        pModelObject->poPipelineGraphics_NoDepthTestWrite = createVkGraphicsPipeline("PipelineGraphics-NoDepthTestWrite-" + pModelObject->nameModel,
                                                                                     vertShaderModule, "main",
                                                                                     fragShaderModule, "main",
                                                                                     Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                     Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                     this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                     pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                     VK_FALSE, VK_FALSE, pModelObject->cfg_DepthCompareOp,
                                                                                     pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                     pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                     pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                     pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthTestWrite == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth test and write !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }

    //4> Destroy Shader
    destroyVkShaderModule(fragShaderModule);
    destroyVkShaderModule(vertShaderModule);
}

void Vulkan_006_Depth::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createVkDescriptorSets("DescriptorSets-" + pModelObject->nameModel, this->poDescriptorSetLayout, pModelObject->poDescriptorSets);
        for (size_t j = 0; j < count_sci; j++)
        {
            VkWriteDescriptorSetVector descriptorWrites;
            //(0) PassConstants
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pModelObject->poDescriptorSets[j],
                                            0,
                                            0,
                                            1,
                                            bufferInfo_Pass);
            }
            //(1) ObjectConstants
            {
                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * pModelObject->objectCBs.size();
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pModelObject->poDescriptorSets[j],
                                            1,
                                            0,
                                            1,
                                            bufferInfo_Object);
            }
            //(2) MaterialConstants
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = this->poBuffers_MaterialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pModelObject->poDescriptorSets[j],
                                            2,
                                            0,
                                            1,
                                            bufferInfo_Material);
            }
            //(3) InstanceConstants
            {
                VkDescriptorBufferInfo bufferInfo_Instance = {};
                bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
                bufferInfo_Instance.offset = 0;
                bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pModelObject->poDescriptorSets[j],
                                            3,
                                            0,
                                            1,
                                            bufferInfo_Instance);
            }
            //(4) Image
            {
                VkDescriptorImageInfo imageInfo = {};
                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = pModelObject->poTextureImageView;
                imageInfo.sampler = pModelObject->poTextureSampler;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pModelObject->poDescriptorSets[j],
                                          4,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          imageInfo);
            }
            updateVkDescriptorSets(descriptorWrites);
        }
    }
}

void Vulkan_006_Depth::updateCBs_Custom()
{
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            if (pModelObject->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pModelObject->poMatWorld, 
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->poMatWorld;
            }
        }
        VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
        updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pModelObject->objectCBs.data(), memory);
    }
}



bool Vulkan_006_Depth::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_006_Depth", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();

        //2> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void Vulkan_006_Depth::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        int count = this->m_aModelObjects.size();
        for (int i = 0; i < count; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];

            String nameModel = FUtilString::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                String nameIsShow = "Is Show - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                String nameIsNoDepthTest = "Is NoDepthTest - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsNoDepthTest.c_str(), &pModelObject->isNoDepthTest);
                String nameIsNoDepthWrite = "Is NoDepthWrite - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsNoDepthWrite.c_str(), &pModelObject->isNoDepthWrite);
                String nameIsNoDepthTestWrite = "Is NoDepthTestWrite - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsNoDepthTestWrite.c_str(), &pModelObject->isNoDepthTestWrite);

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);

                String nameWorld = "Model World - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    const FMatrix4& mat4World = pModelObject->objectCBs[0].g_MatWorld;
                    String nameTable = FUtilString::SaveInt(i) + " - split_model_world";
                    if (ImGui::BeginTable(nameTable.c_str(), 4))
                    {
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][0]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][1]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][2]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][3]);

                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][0]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][1]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][2]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][3]);

                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][0]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][1]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][2]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][3]);

                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][0]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][1]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][2]);
                        ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][3]);

                        ImGui::EndTable();
                    }
                }
            }
        }
    }
}

void Vulkan_006_Depth::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_006_Depth::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (!pModelObject->isShow)
            continue;
        
        if (pModelObject->isWireFrame || this->cfg_isWireFrame)
        {   
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_WireFrame);
        }
        else 
        {
            if (pModelObject->isNoDepthTestWrite)
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_NoDepthTestWrite);
            }
            else if (pModelObject->isNoDepthWrite)
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_NoDepthWrite);
            }
            else if (pModelObject->isNoDepthTest)
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_NoDepthTest);
            }
            else
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics);
            }
        }
        
        VkBuffer vertexBuffers[] = { pModelObject->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pModelObject->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pModelObject->poIndexBuffer != nullptr)
        {
            bindIndexBuffer(commandBuffer, pModelObject->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            drawIndexed(commandBuffer, pModelObject->poIndexCount, 1, 0, 0, 0);
        }
        else
        {
            draw(commandBuffer, pModelObject->poVertexCount, 1, 0, 0);
        }
    }
}

void Vulkan_006_Depth::cleanupCustom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        F_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_mapModelObjects.clear();
}