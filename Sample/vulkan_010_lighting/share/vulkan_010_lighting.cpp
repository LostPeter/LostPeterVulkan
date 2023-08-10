/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_010_lighting.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const String c_strVert = ".vert.spv";
const String c_strFrag = ".frag.spv";

static const int g_ShaderCount = 2;
static const char* g_pathShaderModules[2 * g_ShaderCount] = 
{
    "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit
    "Assets/Shader/standard_mesh_transparent_lit.vert.spv", "Assets/Shader/standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit
};


static const int g_CountLen = 3;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "plane",            "Assets/Model/Fbx/plane.fbx",                       "Assets/Texture/terrain.png", //plane
    "viking_room",      "Assets/Model/Obj/viking_room/viking_room.obj",     "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Model/Obj/bunny/bunny.obj",                 "Assets/Texture/white.bmp", //bunny  
};

static const char* g_pathModelShaderModules[g_CountLen] = 
{
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //plane 
    "Assets/Shader/standard_mesh_transparent_lit", //viking_room
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //bunny 
};

static float g_instanceGap = 4.0f;

static int g_instanceExtCount[] =
{
    0, //plane
    5, //viking_room
    5, //bunny
};

static FVector3 g_tranformModels[3 * g_CountLen] = 
{   
    FVector3(   0,   0,    0),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //plane
    FVector3(   0,   0,    5),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   0,   0,    0),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny
};

static bool g_isTranformLocalModels[g_CountLen] = 
{
    false, //plane
    true, //viking_room
    false, //bunny
};

static bool g_isFlipYModels[g_CountLen] = 
{
    true, //plane
    false, //viking_room
    false, //bunny
};

static bool g_isTransparentModels[g_CountLen] = 
{
    false, //plane
    true, //viking_room
    false, //bunny
};

static bool g_isRotateModels[] =
{
    false, //plane
    true, //viking_room
    true, //bunny
};



Vulkan_010_Lighting::Vulkan_010_Lighting(int width, int height, String name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;

    this->poTypeVertex = F_MeshVertex_Pos3Color4Normal3Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/standard_mesh_opaque.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/standard_mesh_opaque.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture2d.jpg";

    this->cfg_cameraPos = FVector3(0.0f, 15.0f, -20.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
}

void Vulkan_010_Lighting::createCamera()
{
    this->pCamera = new FCamera();
    cameraReset();
}

void Vulkan_010_Lighting::loadModel_Custom()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->nameModel = g_pathModels[3 * i + 0];
        pModelObject->pathModel = g_pathModels[3 * i + 1];
        pModelObject->pathTexture = g_pathModels[3 * i + 2];

        bool isTransformLocal = g_isTranformLocalModels[i];
        bool isFlipY = g_isFlipYModels[i];
        pModelObject->isTransparent = g_isTransparentModels[i];

        pModelObject->isRotate = g_isRotateModels[i];
        pModelObject->countInstanceExt = g_instanceExtCount[i];
        pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;

        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTransformLocal, g_tranformLocalModels[i]))
        {
            String msg = "Vulkan_010_Lighting::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "Vulkan_010_Lighting::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        m_aModelObjects.push_back(pModelObject);
        if (pModelObject->isTransparent)
            m_aModelObjects_Render.push_back(pModelObject);
        else 
            m_aModelObjects_Render.insert(m_aModelObjects_Render.begin(), pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
}
bool Vulkan_010_Lighting::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("Vulkan_010_Lighting::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        FMeshVertex& vertex = meshData.vertices[i];
        FVertex_Pos3Color4Normal3Tex2 v;
        v.pos = vertex.pos;
        v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
        v.normal = vertex.normal;
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
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    F_LogInfo("Vulkan_010_Lighting::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
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
bool Vulkan_010_Lighting::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        createTexture2D(pModelObject->pathTexture, pModelObject->poMipMapCount, pModelObject->poTextureImage, pModelObject->poTextureImageMemory);
        createVkImageView(pModelObject->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipMapCount, 1, pModelObject->poTextureImageView);
        createVkSampler(pModelObject->poMipMapCount, pModelObject->poTextureSampler);

        F_LogInfo("Vulkan_010_Lighting::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void Vulkan_010_Lighting::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_010_Lighting::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->instanceMatWorld.clear();
        pModelObject->objectCBs.clear();
        pModelObject->materialCBs.clear();
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                           g_tranformModels[i * 3 + 1],
                                                           g_tranformModels[i * 3 + 2]);
            pModelObject->objectCBs.push_back(objectConstants);
            pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = FMath::RandomColor(false);
            materialConstants.factorDiffuse = FMath::RandomColor(false);
            materialConstants.factorSpecular = FMath::RandomColor(false);
            materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
            materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
            pModelObject->materialCBs.push_back(materialConstants);
        }
        
        if (isCreateVkBuffer)
        {
            //ObjectConstants
            bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
            pModelObject->poBuffers_ObjectCB.resize(count_sci);
            pModelObject->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pModelObject->poBuffers_materialCB.resize(count_sci);
            pModelObject->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_materialCB[j], pModelObject->poBuffersMemory_materialCB[j]);
            }
        }
    }
}

void Vulkan_010_Lighting::createCustomBeforePipeline()
{
    //1> Shader
    createShaderModules();
}  
void Vulkan_010_Lighting::createGraphicsPipeline_Custom()
{
    //1> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //2> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        String pathVertShaderBase = g_pathModelShaderModules[i] + c_strVert;
        String pathFragShaderBase = g_pathModelShaderModules[i] + c_strFrag;
        VkShaderModule vertShaderBase = findShaderModule(pathVertShaderBase);
        VkShaderModule fragShaderBase = findShaderModule(pathFragShaderBase);

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkGraphicsPipeline(vertShaderBase, "main",
                                                                              fragShaderBase, "main",
                                                                              Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex),
                                                                              Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                              this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                              pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits, this->cfg_LineWidth,
                                                                              pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                              pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                              pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                              pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                              pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_010_Lighting::createGraphicsPipeline_Custom: Failed to create pipeline wire frame !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics
        VkBool32 isDepthTestEnable = pModelObject->cfg_isDepthTest;
        VkBool32 isDepthWriteEnable = pModelObject->cfg_isDepthWrite;
        VkBool32 isBlend = pModelObject->cfg_isBlend;
        VkBlendFactor blendColorFactorSrc = pModelObject->cfg_BlendColorFactorSrc; 
        VkBlendFactor blendColorFactorDst = pModelObject->cfg_BlendColorFactorDst; 
        if (pModelObject->isTransparent)
        {
            isDepthTestEnable = VK_FALSE;
            isDepthWriteEnable = VK_FALSE;

            isBlend = VK_TRUE;
            blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
            blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        }
        pModelObject->poPipelineGraphics = createVkGraphicsPipeline(vertShaderBase, "main",
                                                                    fragShaderBase, "main",
                                                                    Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                    Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                    this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                    pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_LineWidth,
                                                                    isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                                    pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                    isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                    pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                    pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_010_Lighting::createGraphicsPipeline_Custom: Failed to create pipeline !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
}

void Vulkan_010_Lighting::destroyShaderModules()
{   
    size_t count = this->m_aVkShaderModules.size();
    for (size_t i = 0; i < count; i++)
    {
        VkShaderModule& vkShaderModule= this->m_aVkShaderModules[i];
        destroyVkShaderModule(vkShaderModule);
    }
    this->m_aVkShaderModules.clear();
    this->m_mapVkShaderModules.clear();
}
void Vulkan_010_Lighting::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String pathVert = g_pathShaderModules[2 * i + 0];
        String pathFrag = g_pathShaderModules[2 * i + 1];

        //vert
        VkShaderModule vertShaderModule = createVkShaderModule("VertexShader: ", pathVert);
        this->m_aVkShaderModules.push_back(vertShaderModule);
        this->m_mapVkShaderModules[pathVert] = vertShaderModule;
        F_LogInfo("Vulkan_010_Lighting::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        VkShaderModule fragShaderModule = createVkShaderModule("FragmentShader: ", pathFrag);
        this->m_aVkShaderModules.push_back(fragShaderModule);
        this->m_mapVkShaderModules[pathFrag] = fragShaderModule;
        F_LogInfo("Vulkan_010_Lighting::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
VkShaderModule Vulkan_010_Lighting::findShaderModule(const String& pathShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   

void Vulkan_010_Lighting::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createVkDescriptorSets(this->poDescriptorSetLayout, pModelObject->poDescriptorSets);
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
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
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
                bufferInfo_Material.buffer = pModelObject->poBuffers_materialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
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

void Vulkan_010_Lighting::updateCBs_Custom()
{
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            if (pModelObject->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pModelObject->instanceMatWorld[j], 
                                                  time * glm::radians(90.0f), 
                                                  FVector3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pModelObject->materialCBs[j];
        }

        //ObjectConstants
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pModelObject->objectCBs.data(), memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pModelObject->materialCBs.data(), memory);
        }
    }
}



bool Vulkan_010_Lighting::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_010_Lighting", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();

        //2> Light
        lightConfig();

        //3> PassConstants
        passConstantsConfig();

        //4> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void Vulkan_010_Lighting::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        float fGap = g_instanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 5.0f))
        {
            g_instanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count = this->m_aModelObjects.size();
        for (size_t i = 0; i < count; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];

            String nameModel = FUtilString::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                String nameIsShow = "Is Show - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                
                String nameInstances = "Instance - " + pModelObject->nameModel;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 10);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);
                
                String nameWorld = "Model Object - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        String nameModelInstance = nameModel + " - " + FUtilString::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            String nameObject = FUtilString::SaveInt(j) + " - Object - " + pModelObject->nameModel;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
                                const FMatrix4& mat4World = obj.g_MatWorld;
                                String nameTable = FUtilString::SaveInt(j) + " - matWorld - " + pModelObject->nameModel;
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
                            
                            //MaterialConstants
                            String nameMaterial = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameModel;
                            if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                            {
                                //factorAmbient
                                String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                {

                                }
                                ImGui::Spacing();

                                //factorDiffuse
                                String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                {

                                }
                                ImGui::Spacing();

                                //factorSpecular
                                String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                {

                                }
                                ImGui::Spacing();

                                //shininess
                                String nameShininess = "Shininess - " + FUtilString::SaveInt(j);
                                if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //alpha
                                String nameAlpha = "Alpha - " + FUtilString::SaveInt(j);
                                if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //lighting
                                String nameLighting = "Lighting - " + FUtilString::SaveInt(j);
                                bool isLighting = mat.lighting == 1.0f ? true : false;
                                if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                {
                                    mat.lighting = isLighting ? 1.0f : 0.0f;
                                }
                                ImGui::Spacing();
                            }
                        }
                    }
                }
            }
        }
    }
}

void Vulkan_010_Lighting::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_010_Lighting::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
{   
    size_t count = this->m_aModelObjects_Render.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects_Render[i];
        if (!pModelObject->isShow)
            continue;

        VkBuffer vertexBuffers[] = { pModelObject->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pModelObject->poIndexBuffer != nullptr)
        {
            bindIndexBuffer(commandBuffer, pModelObject->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

        if (pModelObject->isWireFrame || this->cfg_isWireFrame)
        {
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_WireFrame);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        else
        {
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        
    }
}
void Vulkan_010_Lighting::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
{
    if (pModelObject->poIndexBuffer != nullptr)
    {
        drawIndexed(commandBuffer, pModelObject->poIndexCount, pModelObject->countInstance, 0, 0, 0);
    }
    else
    {
        draw(commandBuffer, pModelObject->poVertexCount, pModelObject->countInstance, 0, 0);
    }
}

void Vulkan_010_Lighting::cleanupCustom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        F_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_aModelObjects_Render.clear();
    this->m_mapModelObjects.clear();
}

void Vulkan_010_Lighting::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }

    destroyShaderModules();
}

void Vulkan_010_Lighting::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}