/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-12-20
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_009_instancing.h"
#include "VulkanMeshLoader.h"
#include "VulkanCamera.h"
#include "VulkanTimer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const std::string c_strVert = ".vert.spv";
const std::string c_strFrag = ".frag.spv";

static const int g_ShaderCount = 3;
static const char* g_pathShaderModules[2 * g_ShaderCount] = 
{
    "Assets/Shader/standard_mesh_opaque.vert.spv", "Assets/Shader/standard_mesh_opaque.frag.spv", //standard_mesh_opaque
    "Assets/Shader/standard_mesh_transparent.vert.spv", "Assets/Shader/standard_mesh_transparent.frag.spv", //standard_mesh_transparent
    "Assets/Shader/standard_mesh_outline.vert.spv", "Assets/Shader/standard_mesh_outline.frag.spv", //standard_mesh_outline
};


static const int g_CountLen = 2;
static const char* g_pathModels[3 * g_CountLen] = 
{
    "viking_room",      "Assets/Model/Obj/viking_room/viking_room.obj",     "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Model/Obj/bunny/bunny.obj",                 "Assets/Texture/white.bmp", //bunny  
};

static const char* g_pathModelShaderModules[2 * g_CountLen] = 
{
    "Assets/Shader/standard_mesh_transparent", "Assets/Shader/standard_mesh_outline", //viking_room
    "Assets/Shader/standard_mesh_opaque", "Assets/Shader/standard_mesh_outline", //bunny 
};

static float g_instanceGap = 1.5f;

static glm::vec3 g_tranformModels[3 * g_CountLen] = 
{   
    glm::vec3(   0,   0,    1),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //viking_room
    glm::vec3(   0,   0,    0),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //bunny
};

static glm::mat4 g_tranformLocalModels[g_CountLen] = 
{
    MathUtil::RotateX(-90.0f), //viking_room
    MathUtil::ms_mat4Unit, //bunny
};

static bool g_isTranformLocalModels[g_CountLen] = 
{
    true, //viking_room
    false, //bunny
};

static bool g_isFlipYModels[g_CountLen] = 
{
    false, //viking_room
    false, //bunny
};

static bool g_isTransparentModels[g_CountLen] = 
{
    true, //viking_room
    false, //bunny
};

static float g_TransparentAlpha[g_CountLen] =
{
    0.5f, //viking_room
    1.0f, //bunny
};

static glm::vec4 g_OutlineWidth[g_CountLen] = 
{
    glm::vec4(0.02f,0.02f,0.02f,0.02f), //viking_room
    glm::vec4(0.02f,0.02f,0.02f,0.02f), //bunny
};

static glm::vec4 g_OutlineColor[g_CountLen] = 
{
    glm::vec4(0,1,0,1), //viking_room
    glm::vec4(1,0,0,1), //bunny
};



Vulkan_009_Instancing::Vulkan_009_Instancing(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
    , poPipelineLayout_Outline(VK_NULL_HANDLE)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = Vulkan_Vertex_Pos3Color4Normal3Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/standard_mesh_opaque.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/standard_mesh_opaque.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture2d.jpg";

    this->cfg_cameraPos = glm::vec3(0.0f, 3.0f, -4.0f);
}

void Vulkan_009_Instancing::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
}

void Vulkan_009_Instancing::loadModel_Custom()
{
    for (int i = 0; i < g_CountLen; i++)
    {
        ModelObject* pModelObject = new ModelObject(this->poDevice);
        pModelObject->nameModel = g_pathModels[3 * i + 0];
        pModelObject->pathModel = g_pathModels[3 * i + 1];
        pModelObject->pathTexture = g_pathModels[3 * i + 2];

        bool isTranformLocal = g_isTranformLocalModels[i];
        bool isFlipY = g_isFlipYModels[i];
        pModelObject->isTransparent = g_isTransparentModels[i];

        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTranformLocal, g_tranformLocalModels[i]))
        {
            std::string msg = "Vulkan_009_Instancing::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            std::string msg = "Vulkan_009_Instancing::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
            Util_LogError(msg.c_str());
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
bool Vulkan_009_Instancing::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load 
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_009_Instancing::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    pModelObject->vertices.clear();
    pModelObject->vertices.reserve(count_vertex);
    for (int i = 0; i < count_vertex; i++)
    {
        MeshVertex& vertex = meshData.vertices[i];
        Vertex_Pos3Color4Normal3Tex2 v;
        v.pos = vertex.pos;
        v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        v.normal = vertex.normal;
        v.texCoord = vertex.texCoord;

        if (isTranformLocal)
        {
            v.pos = MathUtil::Transform(matTransformLocal, v.pos);
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
    pModelObject->poVertexBuffer_Size = pModelObject->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tex2);
    pModelObject->poVertexBuffer_Data = &pModelObject->vertices[0];
    pModelObject->poIndexCount = (uint32_t)pModelObject->indices.size();
    pModelObject->poIndexBuffer_Size = pModelObject->poIndexCount * sizeof(uint32_t);
    pModelObject->poIndexBuffer_Data = &pModelObject->indices[0];

    Util_LogInfo("Vulkan_009_Instancing::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
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
bool Vulkan_009_Instancing::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        createTexture2D(pModelObject->pathTexture, pModelObject->poMipMapCount, pModelObject->poTextureImage, pModelObject->poTextureImageMemory);
        createImageView(pModelObject->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipMapCount, 1, pModelObject->poTextureImageView);
        createSampler(pModelObject->poMipMapCount, pModelObject->poTextureSampler);

        Util_LogInfo("Vulkan_009_Instancing::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void Vulkan_009_Instancing::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_009_Instancing::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        //1> Stencil
        pModelObject->instanceMatWorld.clear();
        pModelObject->objectCBs.clear();
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = MathUtil::FromTRS(g_tranformModels[i * 3 + 0] + glm::vec3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                           g_tranformModels[i * 3 + 1],
                                                           g_tranformModels[i * 3 + 2]);
            pModelObject->objectCBs.push_back(objectConstants);
            pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);
        }
        
        if (isCreateVkBuffer)
        {
            bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
            pModelObject->poBuffers_ObjectCB.resize(count_sci);
            pModelObject->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
            }
        }

        //2> Transparent
        if (pModelObject->isTransparent)
        {
            pModelObject->materialCBs.clear();
            for (int j = 0; j < pModelObject->countInstance; j++)
            {
                MaterialConstants materialConstants;
                materialConstants.alpha = g_TransparentAlpha[i];
                pModelObject->materialCBs.push_back(materialConstants);
            }

            if (isCreateVkBuffer)
            {
                bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                pModelObject->poBuffers_materialCB.resize(count_sci);
                pModelObject->poBuffersMemory_materialCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_materialCB[j], pModelObject->poBuffersMemory_materialCB[j]);
                }
            }
        }

        //3> Outline
        pModelObject->objectCBs_Outline.clear();
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            ObjectConstants_Outline objectConstants_Outline;
            objectConstants_Outline.g_MatWorld = pModelObject->instanceMatWorld[j];
            objectConstants_Outline.g_OutlineColor = MathUtil::RandomColor(false);
            objectConstants_Outline.g_OutlineWidth = g_OutlineWidth[i];
            pModelObject->objectCBs_Outline.push_back(objectConstants_Outline);
        }
        
        if (isCreateVkBuffer)
        {
            bufferSize = sizeof(ObjectConstants_Outline) * 128;
            pModelObject->poBuffers_ObjectCB_Outline.resize(count_sci);
            pModelObject->poBuffersMemory_ObjectCB_Outline.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB_Outline[j], pModelObject->poBuffersMemory_ObjectCB_Outline[j]);
            }
        }
    }
}

void Vulkan_009_Instancing::createPipeline_Custom()
{
    //1> Shader
    createShaderModules();

    //2> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //3> PipelineLayout
    createPipelineLayout_Outline();

    //4> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        std::string pathVertShaderBase = g_pathModelShaderModules[2 * i + 0] + c_strVert;
        std::string pathFragShaderBase = g_pathModelShaderModules[2 * i + 0] + c_strFrag;
        VkShaderModule vertShaderBase = findShaderModule(pathVertShaderBase);
        VkShaderModule fragShaderBase = findShaderModule(pathFragShaderBase);

        std::string pathVertShaderOutline = g_pathModelShaderModules[2 * i + 1] + c_strVert;
        std::string pathFragShaderOutline = g_pathModelShaderModules[2 * i + 1] + c_strFrag;
        VkShaderModule vertShaderOutline = findShaderModule(pathVertShaderOutline);
        VkShaderModule fragShaderOutline = findShaderModule(pathFragShaderOutline);

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkPipeline(vertShaderBase, "main",
                                                                      fragShaderBase, "main",
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
            std::string msg = "Vulkan_009_Instancing::createPipeline_Custom: Failed to create pipeline wire frame !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_Stencil
        VkStencilOpState back = {};
        back.compareOp = VK_COMPARE_OP_ALWAYS;
        back.failOp = VK_STENCIL_OP_REPLACE;
        back.depthFailOp = VK_STENCIL_OP_REPLACE;
        back.passOp = VK_STENCIL_OP_REPLACE;
        back.compareMask = 0xFF;
        back.writeMask = 0xFF;
        back.reference = 1;
        VkStencilOpState front = back;

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
        pModelObject->poPipelineGraphics_Stencil = createVkPipeline(vertShaderBase, "main",
                                                                    fragShaderBase, "main",
                                                                    Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                    Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                    this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                    pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE,
                                                                    isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                                    VK_TRUE, front, back, 
                                                                    isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                    pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                    pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_Stencil == VK_NULL_HANDLE)
        {
            std::string msg = "Vulkan_009_Instancing::createPipeline_Custom: Failed to create pipeline stencil !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_Outline
        back.compareOp = VK_COMPARE_OP_NOT_EQUAL;
        back.failOp = VK_STENCIL_OP_KEEP;
		back.depthFailOp = VK_STENCIL_OP_KEEP;
		back.passOp = VK_STENCIL_OP_REPLACE;
		front = back;
        pModelObject->poPipelineGraphics_Outline = createVkPipeline(vertShaderOutline, "main",
                                                                    fragShaderOutline, "main",
                                                                    Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                    Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                    this->poRenderPass, this->poPipelineLayout_Outline, aViewports, aScissors,
                                                                    pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE,
                                                                    VK_FALSE, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                    VK_TRUE, front, back,
                                                                    pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                    pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                    pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_Outline == VK_NULL_HANDLE)
        {
            std::string msg = "Vulkan_009_Instancing::createPipeline_Custom: Failed to create pipeline outline !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }

}
void Vulkan_009_Instancing::createPipelineLayout_Outline()
{
    VkDescriptorSetLayoutVector aDescriptorSetLayout;
    aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
    this->poPipelineLayout_Outline = createVkPipelineLayout(aDescriptorSetLayout);
    if (this->poPipelineLayout_Outline == VK_NULL_HANDLE)
    {
        std::string msg = "Vulkan_009_Instancing::createPipelineLayout_Outline: createVkPipelineLayout failed !";
        Util_LogError(msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
}
void Vulkan_009_Instancing::destroyShaderModules()
{   
    size_t count = this->m_aVkShaderModules.size();
    for (size_t i = 0; i < count; i++)
    {
        VkShaderModule& vkShaderModule= this->m_aVkShaderModules[i];
        vkDestroyShaderModule(this->poDevice, vkShaderModule, nullptr);
    }
    this->m_aVkShaderModules.clear();
    this->m_mapVkShaderModules.clear();
}
void Vulkan_009_Instancing::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        std::string pathVert = g_pathShaderModules[2 * i + 0];
        std::string pathFrag = g_pathShaderModules[2 * i + 1];

        //vert
        VkShaderModule vertShaderModule = createShaderModule("VertexShader: ", pathVert);
        this->m_aVkShaderModules.push_back(vertShaderModule);
        this->m_mapVkShaderModules[pathVert] = vertShaderModule;
        Util_LogInfo("Vulkan_009_Instancing::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        VkShaderModule fragShaderModule = createShaderModule("FragmentShader: ", pathFrag);
        this->m_aVkShaderModules.push_back(fragShaderModule);
        this->m_mapVkShaderModules[pathFrag] = fragShaderModule;
        Util_LogInfo("Vulkan_009_Instancing::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
VkShaderModule Vulkan_009_Instancing::findShaderModule(const std::string& pathShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   

void Vulkan_009_Instancing::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createDescriptorSets(pModelObject->poDescriptorSets, this->poDescriptorSetLayout);
        createDescriptorSets(pModelObject->poDescriptorSets_Outline, this->poDescriptorSetLayout);
        for (size_t j = 0; j < count_sci; j++)
        {
            //1> Stencil
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);

                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;

                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = pModelObject->isTransparent ? pModelObject->poBuffers_materialCB[j] : this->poBuffers_MaterialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT; 

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

            //2> Outline
            {
                VkDescriptorBufferInfo bufferInfo_Pass_Outline  = {};
                bufferInfo_Pass_Outline.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass_Outline.offset = 0;
                bufferInfo_Pass_Outline.range = sizeof(PassConstants);

                VkDescriptorBufferInfo bufferInfo_Object_Outline = {};
                bufferInfo_Object_Outline.buffer = pModelObject->poBuffers_ObjectCB_Outline[j];
                bufferInfo_Object_Outline.offset = 0;
                bufferInfo_Object_Outline.range = sizeof(ObjectConstants_Outline) * 128; 

                VkDescriptorBufferInfo bufferInfo_Material_Outline = {};
                bufferInfo_Material_Outline.buffer = this->poBuffers_MaterialCB[j];
                bufferInfo_Material_Outline.offset = 0;
                bufferInfo_Material_Outline.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;

                VkDescriptorBufferInfo bufferInfo_Instance_Outline = {};
                bufferInfo_Instance_Outline.buffer = this->poBuffers_InstanceCB[j];
                bufferInfo_Instance_Outline.offset = 0;
                bufferInfo_Instance_Outline.range = sizeof(InstanceConstants) * this->instanceCBs.size();

                VkDescriptorImageInfo imageInfo_Outline = {};
                imageInfo_Outline.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo_Outline.imageView = pModelObject->poTextureImageView;
                imageInfo_Outline.sampler = pModelObject->poTextureSampler;

                std::vector<VkWriteDescriptorSet> descriptorWrites_Outline;

                //0
                VkWriteDescriptorSet ds0_Outline  = {};
                ds0_Outline.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds0_Outline.dstSet = pModelObject->poDescriptorSets_Outline[j];
                ds0_Outline.dstBinding = 0;
                ds0_Outline.dstArrayElement = 0;
                ds0_Outline.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds0_Outline.descriptorCount = 1;
                ds0_Outline.pBufferInfo = &bufferInfo_Pass_Outline;
                descriptorWrites_Outline.push_back(ds0_Outline);

                //1
                VkWriteDescriptorSet ds1_Outline = {};
                ds1_Outline.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds1_Outline.dstSet = pModelObject->poDescriptorSets_Outline[j];
                ds1_Outline.dstBinding = 1;
                ds1_Outline.dstArrayElement = 0;
                ds1_Outline.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds1_Outline.descriptorCount = 1;
                ds1_Outline.pBufferInfo = &bufferInfo_Object_Outline;
                descriptorWrites_Outline.push_back(ds1_Outline);

                //2
                VkWriteDescriptorSet ds2_Outline = {};
                ds2_Outline.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds2_Outline.dstSet = pModelObject->poDescriptorSets_Outline[j];
                ds2_Outline.dstBinding = 2;
                ds2_Outline.dstArrayElement = 0;
                ds2_Outline.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds2_Outline.descriptorCount = 1;
                ds2_Outline.pBufferInfo = &bufferInfo_Material_Outline;
                descriptorWrites_Outline.push_back(ds2_Outline);

                //3
                VkWriteDescriptorSet ds3_Outline = {};
                ds3_Outline.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds3_Outline.dstSet = pModelObject->poDescriptorSets_Outline[j];
                ds3_Outline.dstBinding = 3;
                ds3_Outline.dstArrayElement = 0;
                ds3_Outline.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds3_Outline.descriptorCount = 1;
                ds3_Outline.pBufferInfo = &bufferInfo_Instance_Outline;
                descriptorWrites_Outline.push_back(ds3_Outline);
                
                //4
                VkWriteDescriptorSet ds4_Outline = {};
                ds4_Outline.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds4_Outline.dstSet = pModelObject->poDescriptorSets_Outline[j];
                ds4_Outline.dstBinding = 4;
                ds4_Outline.dstArrayElement = 0;
                ds4_Outline.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds4_Outline.descriptorCount = 1;
                ds4_Outline.pImageInfo = &imageInfo_Outline;
                descriptorWrites_Outline.push_back(ds4_Outline);

                vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites_Outline.size()), descriptorWrites_Outline.data(), 0, nullptr);
            }
        }
    }
}

void Vulkan_009_Instancing::updateCBs_Custom()
{
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        assert(pModelObject->objectCBs.size() == pModelObject->objectCBs_Outline.size() && "Vulkan_009_Instancing::updateCBs_Custom");
        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            ObjectConstants_Outline& objectCB_Outline = pModelObject->objectCBs_Outline[j];
            if (pModelObject->isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pModelObject->instanceMatWorld[j], 
                                                  time * glm::radians(90.0f), 
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->instanceMatWorld[j];
            }
            objectCB_Outline.g_MatWorld = objectCB.g_MatWorld;

            if (pModelObject->isTransparent)
            {
                MaterialConstants& materialCB = pModelObject->materialCBs[j];
                materialCB.alpha = pModelObject->alpha;
            }
        }
        //Stencil
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->objectCBs.data(), sizeof(ObjectConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
        //Transparent
        if (pModelObject->isTransparent)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(MaterialConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->materialCBs.data(), sizeof(MaterialConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
        //Outline
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB_Outline[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants_Outline) * count_object, 0, &data);
                memcpy(data, pModelObject->objectCBs_Outline.data(), sizeof(ObjectConstants_Outline) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
    }
}



bool Vulkan_009_Instancing::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_009_Instancing", &windowOpened, 0);
    {
        ImGui::Text("Frametime: %f", this->fFPS);
        ImGui::Separator();

        //1> Model
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

            std::string nameModel = StringUtil::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                std::string nameIsShow = "Is Show - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                std::string nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                std::string nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                std::string nameIsOutline = "Is Outline - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsOutline.c_str(), &pModelObject->isOutline);   
                std::string nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                if (pModelObject->isTransparent)
                {
                    //alpha
                    std::string nameAlpha = "Alpha - " + pModelObject->nameModel;
                    float fAlpha = pModelObject->alpha;
                    if (ImGui::DragFloat(nameAlpha.c_str(), &fAlpha, 0.001f, 0.0f, 1.0f))
                    {
                        pModelObject->alpha = fAlpha;
                    }
                }
                std::string nameInstances = "Instance - " + pModelObject->nameModel;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 63);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);
                
                std::string nameWorld = "Model Object - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        ObjectConstants_Outline& obj = pModelObject->objectCBs_Outline[j];
                        //Mat
                        const glm::mat4& mat4World = obj.g_MatWorld;
                        std::string nameTable = StringUtil::SaveInt(j) + " - matWorld - " + pModelObject->nameModel;
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
                        //OutlineWidth
                        std::string nameOutlineWidth = "Outline Width - " + pModelObject->nameModel;
                        float fOutlineWidth = obj.g_OutlineWidth.x;
                        if (ImGui::DragFloat(nameOutlineWidth.c_str(), &fOutlineWidth, 0.01f, 0.01f, 1.0f))
                        {
                            obj.g_OutlineWidth = glm::vec4(fOutlineWidth,fOutlineWidth,fOutlineWidth,fOutlineWidth);
                        }
                        //OutlineColor
                        std::string nameOutlineColor = "Outline Color - " + pModelObject->nameModel;
                        if (ImGui::ColorEdit4(nameOutlineColor.c_str(), (float*)&(obj.g_OutlineColor)))
                        {

                        }
                    }
                }
            }
        }
        
        ImGui::Separator();
        ImGui::Spacing();

        //2> Camera
        cameraConfig();
    }
    ImGui::End();

    return true;
}

void Vulkan_009_Instancing::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_009_Instancing::drawMesh_Custom(VkCommandBuffer& commandBuffer)
{   
    size_t count = this->m_aModelObjects_Render.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects_Render[i];
        if (!pModelObject->isShow)
            continue;

        VkBuffer vertexBuffers[] = { pModelObject->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pModelObject->poIndexBuffer != nullptr)
        {
            vkCmdBindIndexBuffer(commandBuffer, pModelObject->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

        if (pModelObject->isWireFrame)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_WireFrame);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        else
        {
            //1> Stencil Pass
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_Stencil);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);

            //2> Outline Pass
            if (pModelObject->isOutline)
            {
                vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_Outline);
                if (pModelObject->poDescriptorSets_Outline.size() > 0)
                {
                    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout_Outline, 0, 1, &pModelObject->poDescriptorSets_Outline[this->poSwapChainImageIndex], 0, nullptr);
                }
                drawModelObject(commandBuffer, pModelObject);
            }
        }
        
    }
}
void Vulkan_009_Instancing::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
{
    if (pModelObject->poIndexBuffer != nullptr)
    {
        vkCmdDrawIndexed(commandBuffer, pModelObject->poIndexCount, pModelObject->countInstance, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, pModelObject->poVertexCount, pModelObject->countInstance, 0, 0);
    }
}

void Vulkan_009_Instancing::cleanupCustom()
{
    if (this->poPipelineLayout_Outline != nullptr)
    {
        vkDestroyPipelineLayout(this->poDevice, this->poPipelineLayout_Outline, nullptr);
    }
    this->poPipelineLayout_Outline = VK_NULL_HANDLE;
    
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        UTIL_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_aModelObjects_Render.clear();
    this->m_mapModelObjects.clear();
}

void Vulkan_009_Instancing::cleanupSwapChain_Custom()
{
    destroyShaderModules();

    if (this->poPipelineLayout_Outline != nullptr)
    {
        vkDestroyPipelineLayout(this->poDevice, this->poPipelineLayout_Outline, nullptr);
    }
    this->poPipelineLayout_Outline = VK_NULL_HANDLE;

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->cleanupSwapChain();
    }
}

void Vulkan_009_Instancing::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}