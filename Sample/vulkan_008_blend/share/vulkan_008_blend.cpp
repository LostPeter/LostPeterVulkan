/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_008_blend.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const String c_strVert = ".vert.spv";
const String c_strFrag = ".frag.spv";

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
    "viking_room",      "Assets/Mesh/Model/viking_room/viking_room.obj",    "Assets/Texture/Model/viking_room/viking_room.png", //viking_room
    "bunny",            "Assets/Mesh/Model/bunny/bunny.obj",                "Assets/Texture/Common/default_white.bmp", //bunny  
};

static const char* g_pathModelShaderModules[2 * g_CountLen] = 
{
    "Assets/Shader/standard_mesh_transparent", "Assets/Shader/standard_mesh_outline", //viking_room
    "Assets/Shader/standard_mesh_opaque", "Assets/Shader/standard_mesh_outline", //bunny 
};

static FVector3 g_tranformModels[3 * g_CountLen] = 
{   
    FVector3(   0,   0,    1),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //viking_room
    FVector3(   0,   0,    0),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //bunny
};

static FMatrix4 g_tranformLocalModels[g_CountLen] = 
{
    FMath::RotateX(-90.0f), //viking_room
    FMath::ms_mat4Unit, //bunny
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

static float g_OutlineWidth[g_CountLen] = 
{
    0.02f, //viking_room
    0.02f, //bunny
};

static FVector4 g_OutlineColor[g_CountLen] = 
{
    FVector4(0,1,0,1), //viking_room
    FVector4(1,0,0,1), //bunny
};



Vulkan_008_Blend::Vulkan_008_Blend(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , poPipelineLayout_Outline(VK_NULL_HANDLE)
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
    this->cfg_texture_Path = "Assets/Texture/Common/texture2d.jpg";

    this->cfg_cameraPos = FVector3(0.0f, 3.0f, -4.0f);
}

void Vulkan_008_Blend::createCamera()
{
    VulkanWindow::createCamera();
    
    cameraReset();
}

void Vulkan_008_Blend::loadModel_Custom()
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

        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTransformLocal, g_tranformLocalModels[i]))
        {
            String msg = "*********************** Vulkan_008_Blend::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        pModelObject->poMatWorld = FMath::FromTRS(g_tranformModels[i * 3 + 0],
                                                     g_tranformModels[i * 3 + 1],
                                                     g_tranformModels[i * 3 + 2]); 

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            String msg = "*********************** Vulkan_008_Blend::loadModel_Custom: Failed to load texture: " + pModelObject->pathTexture;
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
bool Vulkan_008_Blend::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load 
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!FMeshDataLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        F_LogError("*********************** Vulkan_008_Blend::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
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

    F_LogInfo("Vulkan_008_Blend::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
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
bool Vulkan_008_Blend::loadModel_Texture(ModelObject* pModelObject)
{
    if (!pModelObject->pathTexture.empty())
    {
        String nameTexture;
        String pathBase;
        FUtilString::SplitFileName(pModelObject->pathTexture, nameTexture, pathBase);
        createTexture2D(nameTexture, pModelObject->pathTexture, pModelObject->poMipMapCount, pModelObject->poTextureImage, pModelObject->poTextureImageMemory);
        createVkImageView(nameTexture, pModelObject->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, pModelObject->poMipMapCount, 1, pModelObject->poTextureImageView);
        createVkSampler(nameTexture, pModelObject->poMipMapCount, pModelObject->poTextureSampler);

        F_LogInfo("Vulkan_008_Blend::loadModel_Texture: Load texture [%s] success !", pModelObject->pathTexture.c_str());
    }

    return true;
}

void Vulkan_008_Blend::createCustomCB()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        //1> Stencil
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

        //2> Transparent
        if (pModelObject->isTransparent)
        {
            MaterialConstants materialConstants;
            materialConstants.alpha = g_TransparentAlpha[i];
            pModelObject->materialCBs.push_back(materialConstants);

            bufferSize = sizeof(MaterialConstants) * pModelObject->materialCBs.size();
            pModelObject->poBuffers_materialCB.resize(count_sci);
            pModelObject->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "MaterialConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_materialCB[j], pModelObject->poBuffersMemory_materialCB[j]);
            }
        }

        //3> Outline
        ObjectConstants_Outline objectConstants_Outline;
        objectConstants_Outline.g_MatWorld = pModelObject->poMatWorld;
        objectConstants_Outline.g_OutlineColor = g_OutlineColor[i];
        objectConstants_Outline.g_OutlineWidth = g_OutlineWidth[i];
        pModelObject->objectCBs_Outline.push_back(objectConstants_Outline);

        bufferSize = sizeof(ObjectConstants_Outline) * pModelObject->objectCBs_Outline.size();
        pModelObject->poBuffers_ObjectCB_Outline.resize(count_sci);
        pModelObject->poBuffersMemory_ObjectCB_Outline.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            String nameBuffer = "ObjectConstants_Outline-" + FUtilString::SavePointI(FPointI(i,j));
            createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB_Outline[j], pModelObject->poBuffersMemory_ObjectCB_Outline[j]);
        }
    }
}

void Vulkan_008_Blend::createCustomBeforePipeline()
{
    //1> Shader
    createShaderModules();
} 
void Vulkan_008_Blend::createGraphicsPipeline_Custom()
{
    //1> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //2> PipelineLayout
    createPipelineLayout_Outline();

    //4> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        String pathVertShaderBase = g_pathModelShaderModules[2 * i + 0] + c_strVert;
        String pathFragShaderBase = g_pathModelShaderModules[2 * i + 0] + c_strFrag;
        VkShaderModule vertShaderBase = findShaderModule(pathVertShaderBase);
        VkShaderModule fragShaderBase = findShaderModule(pathFragShaderBase);

        String pathVertShaderOutline = g_pathModelShaderModules[2 * i + 1] + c_strVert;
        String pathFragShaderOutline = g_pathModelShaderModules[2 * i + 1] + c_strFrag;
        VkShaderModule vertShaderOutline = findShaderModule(pathVertShaderOutline);
        VkShaderModule fragShaderOutline = findShaderModule(pathFragShaderOutline);

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkGraphicsPipeline("GraphicsPipeline-Wire-" + pModelObject->nameModel,
                                                                              vertShaderBase, "main",
                                                                              fragShaderBase, "main",
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
            String msg = "*********************** Vulkan_008_Blend::createGraphicsPipeline_Custom: Failed to create pipeline wire frame !";
            F_LogError(msg.c_str());
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
        pModelObject->poPipelineGraphics_Stencil = createVkGraphicsPipeline("GraphicsPipeline-" + pModelObject->nameModel,
                                                                            vertShaderBase, "main",
                                                                            fragShaderBase, "main",
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                            this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                            pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                            isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                                            VK_TRUE, front, back, 
                                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                            pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                            pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_Stencil == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_008_Blend::createGraphicsPipeline_Custom: Failed to create pipeline stencil !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_Outline
        back.compareOp = VK_COMPARE_OP_NOT_EQUAL;
        back.failOp = VK_STENCIL_OP_KEEP;
		back.depthFailOp = VK_STENCIL_OP_KEEP;
		back.passOp = VK_STENCIL_OP_REPLACE;
		front = back;
        pModelObject->poPipelineGraphics_Outline = createVkGraphicsPipeline("GraphicsPipeline-Outline-" + pModelObject->nameModel,
                                                                            vertShaderOutline, "main",
                                                                            fragShaderOutline, "main",
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                            this->poRenderPass, this->poPipelineLayout_Outline, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                            pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                            VK_FALSE, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                            VK_TRUE, front, back,
                                                                            pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                            pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                            pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_Outline == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_008_Blend::createGraphicsPipeline_Custom: Failed to create pipeline outline !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }
}
void Vulkan_008_Blend::createPipelineLayout_Outline()
{
    VkDescriptorSetLayoutVector aDescriptorSetLayout;
    aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
    this->poPipelineLayout_Outline = createVkPipelineLayout("PipelineLayout-Outline", aDescriptorSetLayout);
    if (this->poPipelineLayout_Outline == VK_NULL_HANDLE)
    {
        String msg = "*********************** Vulkan_008_Blend::createPipelineLayout_Outline: createVkPipelineLayout failed !";
        F_LogError(msg.c_str());
        throw std::runtime_error(msg.c_str());
    }
}

void Vulkan_008_Blend::destroyShaderModules()
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
void Vulkan_008_Blend::createShaderModules()
{
    String nameVertexShader;
    String nameFragmentShader;
    String namePathBase;

    for (int i = 0; i < g_ShaderCount; i++)
    {
        String pathVert = g_pathShaderModules[2 * i + 0];
        String pathFrag = g_pathShaderModules[2 * i + 1];

        //vert
        FUtilString::SplitFileName(pathVert, nameVertexShader, namePathBase);
        VkShaderModule vertShaderModule = createVkShaderModule(nameVertexShader, "VertexShader: ", pathVert);
        this->m_aVkShaderModules.push_back(vertShaderModule);
        this->m_mapVkShaderModules[pathVert] = vertShaderModule;
        F_LogInfo("Vulkan_008_Blend::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        FUtilString::SplitFileName(pathFrag, nameFragmentShader, namePathBase);
        VkShaderModule fragShaderModule = createVkShaderModule(nameFragmentShader, "FragmentShader: ", pathFrag);
        this->m_aVkShaderModules.push_back(fragShaderModule);
        this->m_mapVkShaderModules[pathFrag] = fragShaderModule;
        F_LogInfo("Vulkan_008_Blend::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
VkShaderModule Vulkan_008_Blend::findShaderModule(const String& pathShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   

void Vulkan_008_Blend::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createVkDescriptorSets("DescriptorSets-" + pModelObject->nameModel, this->poDescriptorSetLayout, pModelObject->poDescriptorSets);
        createVkDescriptorSets("DescriptorSets-Outline-" + pModelObject->nameModel, this->poDescriptorSetLayout, pModelObject->poDescriptorSets_Outline);
        for (size_t j = 0; j < count_sci; j++)
        {
            //1> Stencil
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
                    bufferInfo_Material.buffer = pModelObject->isTransparent ? pModelObject->poBuffers_materialCB[j] : this->poBuffers_MaterialCB[j];
                    bufferInfo_Material.offset = 0;
                    bufferInfo_Material.range = sizeof(MaterialConstants) * (pModelObject->isTransparent ? pModelObject->materialCBs.size() : this->materialCBs.size());
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

            //2> Outline
            {
                VkWriteDescriptorSetVector descriptorWrites_Outline;
                //(0) PassConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Pass_Outline  = {};
                    bufferInfo_Pass_Outline.buffer = this->poBuffers_PassCB[j];
                    bufferInfo_Pass_Outline.offset = 0;
                    bufferInfo_Pass_Outline.range = sizeof(PassConstants);
                    pushVkDescriptorSet_Uniform(descriptorWrites_Outline,
                                                pModelObject->poDescriptorSets_Outline[j],
                                                0,
                                                0,
                                                1,
                                                bufferInfo_Pass_Outline);
                }
                //(1) ObjectConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Object_Outline = {};
                    bufferInfo_Object_Outline.buffer = pModelObject->poBuffers_ObjectCB_Outline[j];
                    bufferInfo_Object_Outline.offset = 0;
                    bufferInfo_Object_Outline.range = sizeof(ObjectConstants_Outline) * pModelObject->objectCBs_Outline.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites_Outline,
                                                pModelObject->poDescriptorSets_Outline[j],
                                                1,
                                                0,
                                                1,
                                                bufferInfo_Object_Outline);
                }
                //(2) MaterialConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Material_Outline = {};
                    bufferInfo_Material_Outline.buffer = this->poBuffers_MaterialCB[j];
                    bufferInfo_Material_Outline.offset = 0;
                    bufferInfo_Material_Outline.range = sizeof(MaterialConstants) * this->materialCBs.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites_Outline,
                                                pModelObject->poDescriptorSets_Outline[j],
                                                2,
                                                0,
                                                1,
                                                bufferInfo_Material_Outline);
                }
                //(3) InstanceConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Instance_Outline = {};
                    bufferInfo_Instance_Outline.buffer = this->poBuffers_InstanceCB[j];
                    bufferInfo_Instance_Outline.offset = 0;
                    bufferInfo_Instance_Outline.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites_Outline,
                                                pModelObject->poDescriptorSets_Outline[j],
                                                3,
                                                0,
                                                1,
                                                bufferInfo_Instance_Outline);
                }
                //(4) Image
                {
                    VkDescriptorImageInfo imageInfo_Outline = {};
                    imageInfo_Outline.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    imageInfo_Outline.imageView = pModelObject->poTextureImageView;
                    imageInfo_Outline.sampler = pModelObject->poTextureSampler;
                    pushVkDescriptorSet_Image(descriptorWrites_Outline,
                                              pModelObject->poDescriptorSets_Outline[j],
                                              4,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              imageInfo_Outline);
                }
                updateVkDescriptorSets(descriptorWrites_Outline);
            }
        }
    }
}

void Vulkan_008_Blend::updateCBs_Custom()
{
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        F_Assert(pModelObject->objectCBs.size() == pModelObject->objectCBs_Outline.size() && "Vulkan_008_Blend::updateCBs_Custom")
        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            ObjectConstants_Outline& objectCB_Outline = pModelObject->objectCBs_Outline[j];
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
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pModelObject->objectCBs.data(), memory);
        }
        //Transparent
        if (pModelObject->isTransparent)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pModelObject->materialCBs.data(), memory);
        }
        //Outline
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB_Outline[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(ObjectConstants_Outline) * count_object, pModelObject->objectCBs_Outline.data(), memory);
        }
    }
}



bool Vulkan_008_Blend::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_008_Blend", &windowOpened, 0);
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
void Vulkan_008_Blend::modelConfig()
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
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsOutline = "Is Outline - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsOutline.c_str(), &pModelObject->isOutline);   
                String nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                if (pModelObject->isTransparent)
                {
                    //alpha
                    String nameAlpha = "Alpha - " + pModelObject->nameModel;
                    float fAlpha = pModelObject->alpha;
                    if (ImGui::DragFloat(nameAlpha.c_str(), &fAlpha, 0.001f, 0.0f, 1.0f))
                    {
                        pModelObject->alpha = fAlpha;
                    }
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->poVertexCount, (int)pModelObject->poIndexCount);
                
                String nameWorld = "Model Object - " + pModelObject->nameModel;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    ObjectConstants_Outline& obj = pModelObject->objectCBs_Outline[0];
                    //Mat
                    const FMatrix4& mat4World = obj.g_MatWorld;
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
                    //OutlineWidth
                    String nameOutlineWidth = "Outline Width - " + pModelObject->nameModel;
                    float fOutlineWidth = obj.g_OutlineWidth;
                    if (ImGui::DragFloat(nameOutlineWidth.c_str(), &fOutlineWidth, 0.01f, 0.01f, 1.0f))
                    {
                        obj.g_OutlineWidth = fOutlineWidth;
                    }
                    //OutlineColor
                    String nameOutlineColor = "Outline Color - " + pModelObject->nameModel;
                    if (ImGui::ColorEdit4(nameOutlineColor.c_str(), (float*)&(obj.g_OutlineColor)))
                    {

                    }
                }
            }
        }
    }
}

void Vulkan_008_Blend::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_008_Blend::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
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
            //1> Stencil Pass
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_Stencil);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);

            //2> Outline Pass
            if (pModelObject->isOutline)
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_Outline);
                if (pModelObject->poDescriptorSets_Outline.size() > 0)
                {
                    bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout_Outline, 0, 1, &pModelObject->poDescriptorSets_Outline[this->poSwapChainImageIndex], 0, nullptr);
                }
                drawModelObject(commandBuffer, pModelObject);
            }
        }
        
    }
}
void Vulkan_008_Blend::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
{
    if (pModelObject->poIndexBuffer != nullptr)
    {
        drawIndexed(commandBuffer, pModelObject->poIndexCount, 1, 0, 0, 0);
    }
    else
    {
        draw(commandBuffer, pModelObject->poVertexCount, 1, 0, 0);
    }
}

void Vulkan_008_Blend::cleanupCustom()
{
    destroyVkPipelineLayout(this->poPipelineLayout_Outline);
    this->poPipelineLayout_Outline = VK_NULL_HANDLE;
    
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

void Vulkan_008_Blend::cleanupSwapChain_Custom()
{
    destroyShaderModules();

    destroyVkPipelineLayout(this->poPipelineLayout_Outline);
    this->poPipelineLayout_Outline = VK_NULL_HANDLE;

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }
}

void Vulkan_008_Blend::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}