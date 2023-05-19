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
#include "VulkanMeshLoader.h"
#include "VulkanCamera.h"
#include "VulkanTimer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


static const int g_CountLen = 4;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "viking_room",      "Assets/Model/Obj/viking_room/viking_room.obj",     "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Model/Obj/bunny/bunny.obj",                 "Assets/Texture/white.bmp", //bunny

    "plane",            "Assets/Model/Fbx/plane.fbx",                       "Assets/Texture/texture2d.jpg", //plane
    "cube",             "Assets/Model/Fbx/cube.fbx",                        "Assets/Texture/texture2d.jpg", //cube
};

static glm::vec3 g_tranformModels[3 * g_CountLen] = 
{
    glm::vec3(  -1,   0,   -1),     glm::vec3(     0,   0, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //viking_room
    glm::vec3(   1,   0,   -1),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //bunny

    glm::vec3(  -1,   0,    1),     glm::vec3(   -90, 0, 0),      glm::vec3(0.02f,  0.02f,  0.02f), //plane
    glm::vec3(   1,   0,    1),     glm::vec3(     0, 0, 0),      glm::vec3(0.01f,  0.01f,  0.01f), //cube
};

static glm::mat4 g_tranformLocalModels[g_CountLen] = 
{
    VulkanMath::RotateX(-90.0f), //viking_room
    VulkanMath::ms_mat4Unit, //bunny

    VulkanMath::ms_mat4Unit, //plane
    VulkanMath::ms_mat4Unit, //cube
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

    this->poTypeVertex = Vulkan_Vertex_Pos3Color4Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/pos3_color4_tex2_ubo.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/pos3_color4_tex2_ubo.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture2d.jpg";

    this->cfg_cameraPos = glm::vec3(-0.65f, 2.5f, -4.0f);
}

void Vulkan_006_Depth::createCamera()
{
    this->pCamera = new VulkanCamera();
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
        bool isTranformLocal = g_isTranformLocalModels[i];
        
        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTranformLocal, g_tranformLocalModels[i]))
        {
            String msg = "Vulkan_006_Depth::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        pModelObject->poMatWorld = VulkanMath::FromTRS(g_tranformModels[i * 3 + 0],
                                                     g_tranformModels[i * 3 + 1],
                                                     g_tranformModels[i * 3 + 2]); 

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "Vulkan_006_Depth::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        m_aModelObjects.push_back(pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
}
bool Vulkan_006_Depth::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load 
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_006_Depth::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        MeshVertex& vertex = meshData.vertices[i];
        Vertex_Pos3Color4Tex2 v;
        v.pos = vertex.pos;
        v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        v.texCoord = vertex.texCoord;

        if (isTranformLocal)
        {
            v.pos = VulkanMath::Transform(matTransformLocal, v.pos);
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
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(Vertex_Pos3Color4Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    Util_LogInfo("Vulkan_006_Depth::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
                 pModelObject->nameModel.c_str(),
                 (int)pModelObject->vertices.size(), 
                 (int)pModelObject->indices.size());

    //2> createVertexBuffer
    createVertexBuffer(pModelObject->poVertexBuffer_Size, pModelObject->poVertexBuffer_Data, pModelObject->poVertexBuffer, pModelObject->poVertexBufferMemory);

    //3> createIndexBuffer
    if (pModelObject->poIndexBuffer_Size > 0 &&
        pModelObject->poIndexBuffer_Data != nullptr)
    {
        createIndexBuffer(pModelObject->poIndexBuffer_Size, pModelObject->poIndexBuffer_Data, pModelObject->poIndexBuffer, pModelObject->poIndexBufferMemory);
    }

    return true;
}
bool Vulkan_006_Depth::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        createTexture2D(pModelObject->pathTexture, pModelObject->poMipMapCount, pModelObject->poTextureImage, pModelObject->poTextureImageMemory);
        createImageView(pModelObject->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipMapCount, 1, pModelObject->poTextureImageView);
        createSampler(pModelObject->poMipMapCount, pModelObject->poTextureSampler);

        Util_LogInfo("Vulkan_006_Depth::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
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
            createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
        }
    }
}

void Vulkan_006_Depth::createGraphicsPipeline_Custom()
{
    //1> Shader
    VkShaderModule vertShaderModule;
    if (!this->cfg_shaderVertex_Path.empty())
    {
        vertShaderModule = createShaderModule("VertexShader: ", this->cfg_shaderVertex_Path);
    }
    VkShaderModule fragShaderModule;
    if (!this->cfg_shaderFragment_Path.empty())
    {
        fragShaderModule = createShaderModule("FragmentShader: ", this->cfg_shaderFragment_Path);
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
        pModelObject->poPipelineGraphics = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                    fragShaderModule, "main",
                                                                    Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                    Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                    this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                    pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits,
                                                                    pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                    pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                    pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                    pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                    pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                              fragShaderModule, "main",
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex),
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                              this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                              pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits,
                                                                              pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                              pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                              pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                              pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                              pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline wire frame !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthTest
        pModelObject->poPipelineGraphics_NoDepthTest = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                                fragShaderModule, "main",
                                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                                pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits,
                                                                                VK_FALSE, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                                pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthTest == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth test !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthWrite
        pModelObject->poPipelineGraphics_NoDepthWrite = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                                 fragShaderModule, "main",
                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                 this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                                 pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits,
                                                                                 pModelObject->cfg_isDepthTest, VK_FALSE, pModelObject->cfg_DepthCompareOp,
                                                                                 pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                 pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                 pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                 pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthWrite == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth write !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_NoDepthTestWrite
        pModelObject->poPipelineGraphics_NoDepthTestWrite = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                                     fragShaderModule, "main",
                                                                                     Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                                     Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                                     this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                                     pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, pModelObject->cfg_vkCullModeFlagBits,
                                                                                     VK_FALSE, VK_FALSE, pModelObject->cfg_DepthCompareOp,
                                                                                     pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                     pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                     pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                     pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_NoDepthTestWrite == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_006_Depth::createGraphicsPipeline_Custom: Failed to create pipeline no depth test and write !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }

    //4> Destroy Shader
    vkDestroyShaderModule(this->poDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(this->poDevice, vertShaderModule, nullptr);
}

void Vulkan_006_Depth::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createDescriptorSets(pModelObject->poDescriptorSets, this->poDescriptorSetLayout);
        for (size_t j = 0; j < count_sci; j++)
        {
            VkDescriptorBufferInfo bufferInfo_Pass = {};
            bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
            bufferInfo_Pass.offset = 0;
            bufferInfo_Pass.range = sizeof(PassConstants);

            VkDescriptorBufferInfo bufferInfo_Object = {};
            bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
            bufferInfo_Object.offset = 0;
            bufferInfo_Object.range = sizeof(ObjectConstants) * pModelObject->objectCBs.size();

            VkDescriptorBufferInfo bufferInfo_Material = {};
            bufferInfo_Material.buffer = this->poBuffers_MaterialCB[j];
            bufferInfo_Material.offset = 0;
            bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();

            VkDescriptorBufferInfo bufferInfo_Instance = {};
            bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
            bufferInfo_Instance.offset = 0;
            bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();

            VkDescriptorImageInfo imageInfo = {};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = pModelObject->poTextureImageView;
            imageInfo.sampler = pModelObject->poTextureSampler;
            
            std::vector<VkWriteDescriptorSet> descriptorWrites;

            //0
            VkWriteDescriptorSet ds0 = {};
            ds0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds0.dstSet = pModelObject->poDescriptorSets[j];
            ds0.dstBinding = 0;
            ds0.dstArrayElement = 0;
            ds0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds0.descriptorCount = 1;
            ds0.pBufferInfo = &bufferInfo_Pass;
            descriptorWrites.push_back(ds0);

            //1
            VkWriteDescriptorSet ds1 = {};
            ds1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds1.dstSet = pModelObject->poDescriptorSets[j];
            ds1.dstBinding = 1;
            ds1.dstArrayElement = 0;
            ds1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds1.descriptorCount = 1;
            ds1.pBufferInfo = &bufferInfo_Object;
            descriptorWrites.push_back(ds1);

            //2
            VkWriteDescriptorSet ds2 = {};
            ds2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds2.dstSet = pModelObject->poDescriptorSets[j];
            ds2.dstBinding = 2;
            ds2.dstArrayElement = 0;
            ds2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds2.descriptorCount = 1;
            ds2.pBufferInfo = &bufferInfo_Material;
            descriptorWrites.push_back(ds2);

            //3
            VkWriteDescriptorSet ds3 = {};
            ds3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds3.dstSet = pModelObject->poDescriptorSets[j];
            ds3.dstBinding = 3;
            ds3.dstArrayElement = 0;
            ds3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds3.descriptorCount = 1;
            ds3.pBufferInfo = &bufferInfo_Instance;
            descriptorWrites.push_back(ds3);
            
            //4
            VkWriteDescriptorSet ds4 = {};
            ds4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds4.dstSet = pModelObject->poDescriptorSets[j];
            ds4.dstBinding = 4;
            ds4.dstArrayElement = 0;
            ds4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            ds4.descriptorCount = 1;
            ds4.pImageInfo = &imageInfo;
            descriptorWrites.push_back(ds4);

            vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
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
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->poMatWorld;
            }
        }
        VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
        void* data;
        vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
            memcpy(data, pModelObject->objectCBs.data(), sizeof(ObjectConstants) * count_object);
        vkUnmapMemory(this->poDevice, memory);
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

            String nameModel = VulkanUtilString::SaveInt(i) + " - " + pModelObject->nameModel;
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
                    const glm::mat4& mat4World = pModelObject->objectCBs[0].g_MatWorld;
                    String nameTable = VulkanUtilString::SaveInt(i) + " - split_model_world";
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
        UTIL_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_mapModelObjects.clear();
}