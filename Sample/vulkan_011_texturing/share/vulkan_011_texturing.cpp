/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2023-01-02
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_011_texturing.h"
#include "VulkanMeshLoader.h"
#include "VulkanCamera.h"
#include "VulkanTimer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


const std::string c_strVert = ".vert.spv";
const std::string c_strFrag = ".frag.spv";

static const int g_ShaderCount = 8;
static const char* g_pathShaderModules[2 * g_ShaderCount] = 
{
    "Assets/Shader/standard_mesh_opaque_tex1d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex1d_lit.frag.spv", //standard_mesh_opaque_tex1d_lit
    "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit
    "Assets/Shader/standard_mesh_opaque_tex2darray_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit
    "Assets/Shader/standard_mesh_opaque_tex3d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex3d_lit.frag.spv", //standard_mesh_opaque_tex3d_lit
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit
    "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.frag.spv", //standard_mesh_opaque_texanim_scroll_lit
    "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.frag.spv", //standard_mesh_opaque_texanim_chunk_lit
    
    "Assets/Shader/standard_mesh_transparent_lit.vert.spv", "Assets/Shader/standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit
};

static const int g_DescriptorSetLayoutCount = 2;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-Texture",
    "",
};

static const std::string g_TextureDefault = "default";
static const int g_TextureCount = 11;
static const char* g_pathTextures[3 * g_TextureCount] = 
{
    "default",                      "2d",           "Assets/Texture/default_blackwhite.png", //default
    "terrain",                      "2d",           "Assets/Texture/terrain.png", //terrain
    "viking_room",                  "2d",           "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "white",                        "2d",           "Assets/Texture/white.bmp", //white
    
   //"textureSampler_"
    
    "texture1d",                    "1d",           "Assets/Texture/texture1d.tga", //texture1d
    "texture2d",                    "2d",           "Assets/Texture/texture2d.jpg", //texture2d
    "texture2darray",               "2darray",      "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture2darray
    "texture3d",                    "3d",           "", //texture3d
    "texturecubemap",               "cubemap",      "Assets/Texture/texturecubemap_x_right.png;Assets/Texture/texturecubemap_x_left.png;Assets/Texture/texturecubemap_y_up.png;Assets/Texture/texturecubemap_y_down.png;Assets/Texture/texturecubemap_z_front.png;Assets/Texture/texturecubemap_z_back.png", //texturecubemap

    "textureanimation_scroll",      "2darray",      "Assets/Texture/textureanimation1.png;Assets/Texture/textureanimation2.png", //textureanimation_scroll
    "textureanimation_chunk",       "2d",           "Assets/Texture/textureanimation3.png", //textureanimation_chunk
};
static VkFormat g_formatTextures[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //default
    VK_FORMAT_R8G8B8A8_SRGB, //terrain
    VK_FORMAT_R8G8B8A8_SRGB, //viking_room
    VK_FORMAT_R8G8B8A8_SRGB, //white

    VK_FORMAT_R8G8B8A8_SRGB, //texture1d
    VK_FORMAT_R8G8B8A8_SRGB, //texture2d
    VK_FORMAT_R8G8B8A8_SRGB, //texture2darray
    VK_FORMAT_R8_UNORM, //texture3d
    VK_FORMAT_R8G8B8A8_SRGB, //texturecubemap

    VK_FORMAT_R8G8B8A8_SRGB, //textureanimation_scroll
    VK_FORMAT_R8G8B8A8_SRGB, //textureanimation_chunk
};
static VulkanTextureFilterType g_filterTextures[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //default
    Vulkan_TextureFilter_Bilinear, //terrain
    Vulkan_TextureFilter_Bilinear, //viking_room
    Vulkan_TextureFilter_Bilinear, //white

    Vulkan_TextureFilter_Bilinear, //texture1d
    Vulkan_TextureFilter_Bilinear, //texture2d
    Vulkan_TextureFilter_Bilinear, //texture2darray
    Vulkan_TextureFilter_Bilinear, //texture3d
    Vulkan_TextureFilter_Bilinear, //texturecubemap

    Vulkan_TextureFilter_Bilinear, //textureanimation_scroll
    Vulkan_TextureFilter_Bilinear, //textureanimation_chunk
};
static VulkanTextureAddressingType g_addressingTextures[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //default
    Vulkan_TextureAddressing_Clamp, //terrain
    Vulkan_TextureAddressing_Clamp, //viking_room
    Vulkan_TextureAddressing_Clamp, //white

    Vulkan_TextureAddressing_Clamp, //texture1d
    Vulkan_TextureAddressing_Clamp, //texture2d
    Vulkan_TextureAddressing_Clamp, //texture2darray
    Vulkan_TextureAddressing_Clamp, //texture3d
    Vulkan_TextureAddressing_Wrap, //texturecubemap

    Vulkan_TextureAddressing_Wrap, //textureanimation_scroll
    Vulkan_TextureAddressing_Wrap, //textureanimation_chunk
};
static VulkanTextureBorderColorType g_borderColorTextures[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //default
    Vulkan_TextureBorderColor_OpaqueBlack, //terrain
    Vulkan_TextureBorderColor_OpaqueBlack, //viking_room
    Vulkan_TextureBorderColor_OpaqueBlack, //white

    Vulkan_TextureBorderColor_OpaqueBlack, //texture1d
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2d
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2darray
    Vulkan_TextureBorderColor_OpaqueBlack, //texture3d
    Vulkan_TextureBorderColor_OpaqueBlack, //texturecubemap

    Vulkan_TextureBorderColor_OpaqueBlack, //textureanimation_scroll
    Vulkan_TextureBorderColor_OpaqueBlack, //textureanimation_chunk
};
static int g_sizeTextures[3 * g_TextureCount] = 
{
    512,    512,    1, //default
    512,    512,    1, //terrain
    512,    512,    1, //viking_room
    512,    512,    1, //white

    512,    512,    1, //texture1d
    512,    512,    1, //texture2d
    512,    512,    1, //texture2darray
    128,    128,    128, //texture3d
    512,    512,    1, //texturecubemap

    512,    512,    1, //textureanimation_scroll
    512,    512,    1, //textureanimation_chunk
};
static float g_animChunkTextures[2 * g_TextureCount] = 
{
    0,    0, //default
    0,    0, //terrain
    0,    0, //viking_room
    0,    0, //white

    0,    0, //texture1d
    0,    0, //texture2d
    0,    0, //texture2darray
    0,    0, //texture3d
    0,    0, //texturecubemap

    0,    0, //textureanimation_scroll
    4,    8, //textureanimation_chunk
};



static const int g_ModelCount = 11;
static const char* g_pathModels[4 * g_ModelCount] = 
{
    //Model Name                    //Model Path                                        //Texture One                           //Texture Two
    "ground",                       "Assets/Model/Fbx/plane.fbx",                       "terrain",                              "", //ground
    "viking_room",                  "Assets/Model/Obj/viking_room/viking_room.obj",     "viking_room",                          "", //viking_room
    "bunny",                        "Assets/Model/Obj/bunny/bunny.obj",                 "white",                                "", //bunny  

    "texture1D",                    "Assets/Model/Fbx/plane.fbx",                       "texture1d",                            "", //texture1D
    "texture2D",                    "Assets/Model/Fbx/plane.fbx",                       "texture2d",                            "", //texture2D
    "texture2Darray",               "Assets/Model/Fbx/plane.fbx",                       "texture2darray",                       "", //texture2Darray
    "texture3D",                    "Assets/Model/Fbx/plane.fbx",                       "texture3d",                            "", //texture3D
    "textureCubeMap_SkyBox",        "Assets/Model/Obj/cube/cube.obj",                   "texturecubemap",                       "", //textureCubeMap_SkyBox
    "textureCubeMap_Sphere",        "Assets/Model/Fbx/sphere.fbx",                      "texturecubemap",                       "", //textureCubeMap_Sphere

    "textureAnimation_Scroll",      "Assets/Model/Fbx/plane.fbx",                       "textureanimation_scroll",              "", //textureAnimation_Scroll
    "textureAnimation_Chunk",       "Assets/Model/Fbx/plane.fbx",                       "textureanimation_chunk",               "", //textureAnimation_Chunk
};

static const VulkanTextureType g_ModelsTextureTypes[2 * g_ModelCount] =
{
    Vulkan_Texture_2D, Vulkan_Texture_2D, //ground 
    Vulkan_Texture_2D, Vulkan_Texture_2D, //viking_room
    Vulkan_Texture_2D, Vulkan_Texture_2D, //bunny 

    Vulkan_Texture_1D, Vulkan_Texture_1D, //texture1D 
    Vulkan_Texture_2D, Vulkan_Texture_2D, //texture2D 
    Vulkan_Texture_2DArray, Vulkan_Texture_2DArray, //texture2Darray 
    Vulkan_Texture_3D, Vulkan_Texture_3D, //texture3D
    Vulkan_Texture_CubeMap, Vulkan_Texture_CubeMap, //textureCubeMap_SkyBox
    Vulkan_Texture_CubeMap, Vulkan_Texture_CubeMap, //textureCubeMap_Sphere

    Vulkan_Texture_2DArray, Vulkan_Texture_2DArray, //textureAnimation_Scroll
    Vulkan_Texture_2D, Vulkan_Texture_2D, //textureAnimation_Chunk 
};

static const char* g_pathModelShaderModules[g_ModelCount] = 
{
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //ground 
    "Assets/Shader/standard_mesh_transparent_lit", //viking_room
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //bunny 

    "Assets/Shader/standard_mesh_opaque_tex1d_lit", //texture1D 
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //texture2D 
    "Assets/Shader/standard_mesh_opaque_tex2darray_lit", //texture2Darray
    "Assets/Shader/standard_mesh_opaque_tex3d_lit", //texture3D
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit", //textureCubeMap_SkyBox
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit", //textureCubeMap_Sphere

    "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit", //textureAnimation_Scroll
    "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit", //textureAnimation_Chunk
};

static float g_instanceGap = 1.5f;

static int g_instanceExtCount[] =
{
    0, //ground
    0, //viking_room
    0, //bunny

    5, //texture1D 
    5, //texture2D 
    5, //texture2Darray 
    5, //texture3D 
    0, //textureCubeMap_SkyBox 
    5, //textureCubeMap_Sphere 

    5, //textureAnimation_Scroll
    5, //textureAnimation_Chunk
};

static glm::vec3 g_tranformModels[3 * g_ModelCount] = 
{   
    glm::vec3(   0,   0,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //ground
    glm::vec3(   0,   0,   -2),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //viking_room
    glm::vec3(   0,   0,   -4),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //bunny

    glm::vec3(   0, 0.5,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture1D
    glm::vec3(   0, 2.0,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2D
    glm::vec3(   0, 3.5,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2Darray
    glm::vec3(   0, 5.0,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture3D
    glm::vec3(   0,   0,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    glm::vec3(   0, 6.5,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 0.005f,  0.005f,  0.005f), //textureCubeMap_Sphere

    glm::vec3(   0, 8.0,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureAnimation_Scroll
    glm::vec3(   0, 9.5,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureAnimation_Chunk
};

static glm::mat4 g_tranformLocalModels[g_ModelCount] = 
{
    MathUtil::ms_mat4Unit, //ground
    MathUtil::RotateX(-90.0f), //viking_room
    MathUtil::ms_mat4Unit, //bunny

    MathUtil::ms_mat4Unit, //texture1D
    MathUtil::ms_mat4Unit, //texture2D
    MathUtil::ms_mat4Unit, //texture2Darray
    MathUtil::ms_mat4Unit, //texture3D
    MathUtil::ms_mat4Unit, //textureCubeMap_SkyBox
    MathUtil::ms_mat4Unit, //textureCubeMap_Sphere

    MathUtil::ms_mat4Unit, //textureAnimation_Scroll
    MathUtil::ms_mat4Unit, //textureAnimation_Chunk
};

static bool g_isTranformLocalModels[g_ModelCount] = 
{
    false, //ground
    true, //viking_room
    false, //bunny

    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox  
    false, //textureCubeMap_Sphere  

    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk
};

static bool g_isFlipYModels[g_ModelCount] = 
{
    true, //ground
    false, //viking_room
    false, //bunny

    false, //texture1D
    true, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere

    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk
};

static bool g_isTransparentModels[g_ModelCount] = 
{
    false, //ground
    true, //viking_room
    false, //bunny

    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere

    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk
};

static bool g_isRotateModels[g_ModelCount] =
{
    false, //ground
    true, //viking_room
    true, //bunny

    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere

    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk
};

static bool g_isLightingModels[g_ModelCount] =
{
    true, //ground
    true, //viking_room
    true, //bunny

    true, //texture1D
    true, //texture2D
    true, //texture2Darray
    true, //texture3D
    true, //textureCubeMap_SkyBox
    true, //textureCubeMap_Sphere

    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk
};


void Vulkan_011_Texturing::ModelTexture::UpdateTexture()
{
    if (this->typeTexture == Vulkan_Texture_3D)
    {
        updateNoiseTexture();
    }
}
void Vulkan_011_Texturing::ModelTexture::updateNoiseTextureData()
{
    // Perlin noise
    noise::module::Perlin modulePerlin;
    for (int z = 0; z < this->depth; z++)
    {
        for (int y = 0; y < this->height; y++)
        {
            for (int x = 0; x < this->width; x++)
            {
                float nx = (float)x / (float)this->width;
                float ny = (float)y / (float)this->height;
                float nz = (float)z / (float)this->depth;

                float n = 20.0f * modulePerlin.GetValue(nx, ny, nz);
                n = n - floor(n);
                this->pDataRGBA[x + y * this->width + z * this->width * this->height] = static_cast<uint8>(floor(n * 255));
            }
        }
    }
}
void Vulkan_011_Texturing::ModelTexture::updateNoiseTexture()
{
    //1> updateNoiseTextureData
    updateNoiseTextureData();

    //2> MapData to stagingBuffer
    VkDeviceSize bufSize = this->width * this->height * this->depth;
    void* data;
    vkMapMemory(this->pWindow->poDevice, this->stagingBufferMemory, 0, bufSize, 0, &data);
        memcpy(data, this->pDataRGBA, bufSize);
    vkUnmapMemory(this->pWindow->poDevice, this->stagingBufferMemory);

    //3> CopyToImage
    VkCommandBuffer cmdBuffer = this->pWindow->beginSingleTimeCommands();
    {   
        this->pWindow->copyBufferToImage(cmdBuffer,
                                         this->stagingBuffer, 
                                         this->poTextureImage, 
                                         static_cast<uint32_t>(this->width), 
                                         static_cast<uint32_t>(this->height),
                                         static_cast<uint32_t>(this->depth), 
                                         1);
    }
    this->pWindow->endSingleTimeCommands(cmdBuffer);
}



Vulkan_011_Texturing::Vulkan_011_Texturing(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->poTypeVertex = Vulkan_VertexType_Pos3Color4Normal3Tex2;
    this->cfg_shaderVertex_Path = "Assets/Shader/standard_mesh_opaque.vert.spv";
    this->cfg_shaderFragment_Path = "Assets/Shader/standard_mesh_opaque.frag.spv";
    this->cfg_texture_Path = "Assets/Texture/texture2d.jpg";

    this->cfg_cameraPos = glm::vec3(-2.5f, 2.0f, -20.0f);
    this->cfg_cameraLookTarget = glm::vec3(-2.5f, 5.0f, 0.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
}

void Vulkan_011_Texturing::createDescriptorSetLayout_Custom()
{


    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_011_Texturing::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
}

void Vulkan_011_Texturing::loadModel_Custom()
{
    createModelTextures();

    for (int i = 0; i < g_ModelCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->indexModel = i;
        pModelObject->nameModel = g_pathModels[4 * i + 0];
        pModelObject->pathModel = g_pathModels[4 * i + 1];

        int indexTex = 0;
        //Texture Channel 1
        {
            pModelObject->aTextureChannels.push_back(indexTex);
            std::string nameTexture1 = g_pathModels[4 * i + 2];
            if (!nameTexture1.empty())
            {
                ModelTexture* pTexture1 = this->findModelTexture(nameTexture1);
                pModelObject->AddTexture(pTexture1);
            }
            indexTex ++;
        }
        //Texture Channel 2
        {
            pModelObject->aTextureChannels.push_back(indexTex);
            std::string nameTexture2 = g_pathModels[4 * i + 3];
            if (!nameTexture2.empty())
            {
                ModelTexture* pTexture2 = this->findModelTexture(nameTexture2);
                pModelObject->AddTexture(pTexture2);
            }
            indexTex ++;
        }

        bool isTranformLocal = g_isTranformLocalModels[i];
        bool isFlipY = g_isFlipYModels[i];
        pModelObject->isTransparent = g_isTransparentModels[i];

        pModelObject->isRotate = g_isRotateModels[i];
        pModelObject->countInstanceExt = g_instanceExtCount[i];
        pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;

        //Model
        if (!loadModel_VertexIndex(pModelObject, isFlipY, isTranformLocal, g_tranformLocalModels[i]))
        {
            std::string msg = "Vulkan_011_Texturing::loadModel_Custom: Failed to load model: " + pModelObject->pathModel;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //Texture
        if (!loadModel_Texture(pModelObject))
        {   
            std::string msg = "Vulkan_011_Texturing::loadModel_Custom: Failed to load textures !";
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
bool Vulkan_011_Texturing::loadModel_VertexIndex(ModelObject* pModelObject, bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load 
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(pModelObject->pathModel, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_011_Texturing::loadModel_VertexIndex load model failed: [%s] !", pModelObject->pathModel.c_str());
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

    Util_LogInfo("Vulkan_011_Texturing::loadModel_VertexIndex: load model [%s] success, Vertex count: [%d], Index count: [%d] !", 
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
bool Vulkan_011_Texturing::loadModel_Texture(ModelObject* pModelObject)
{
    

    return true;
}

void Vulkan_011_Texturing::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_011_Texturing::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        ModelTexture* pTexture1 = pModelObject->GetTexture(0);

        pModelObject->instanceMatWorld.clear();
        pModelObject->objectCBs.clear();
        pModelObject->materialCBs.clear();
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = MathUtil::FromTRS(g_tranformModels[i * 3 + 0] + glm::vec3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                           g_tranformModels[i * 3 + 1],
                                                           g_tranformModels[i * 3 + 2]);
            pModelObject->objectCBs.push_back(objectConstants);
            pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = MathUtil::RandomColor(false);
            materialConstants.factorDiffuse = MathUtil::RandomColor(false);
            materialConstants.factorSpecular = MathUtil::RandomColor(false);
            materialConstants.shininess = MathUtil::RandF(10.0f, 100.0f);
            materialConstants.alpha = MathUtil::RandF(0.2f, 0.9f);
            materialConstants.lighting = g_isLightingModels[i];
            if (pTexture1->typeTexture == Vulkan_Texture_2DArray)
            {
                materialConstants.indexTextureArray = pTexture1->RandomTextureIndex();

                if (materialConstants.indexTextureArray > 0)
                {
                    materialConstants.texSpeedU = MathUtil::RandF(1.0f, 10.0f);
                }
                else 
                {
                    materialConstants.texSpeedV = MathUtil::RandF(1.0f, 10.0f);
                }
            }
            else
            {
                materialConstants.indexTextureArray = MathUtil::RandF(0.0f, 1.0f);
            }

            if (pTexture1->texChunkMaxX > 0 &&
                pTexture1->texChunkMaxY > 0)
            {
                materialConstants.texChunkMaxX = pTexture1->texChunkMaxX;
                materialConstants.texChunkMaxY = pTexture1->texChunkMaxY;
                int indexX = pTexture1->texChunkIndex % pTexture1->texChunkMaxX;
                int indexZ = pTexture1->texChunkIndex / pTexture1->texChunkMaxX;
                materialConstants.texChunkIndexX = indexX;
                materialConstants.texChunkIndexY = indexZ;
            }
            
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
                createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pModelObject->poBuffers_materialCB.resize(count_sci);
            pModelObject->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_materialCB[j], pModelObject->poBuffersMemory_materialCB[j]);
            }
        }
    }
}

void Vulkan_011_Texturing::createPipeline_Custom()
{
    //1> Shader
    createShaderModules();

    //2> Viewport
    std::vector<VkViewport> viewports;
    viewports.push_back(this->poViewport);
    std::vector<VkRect2D> scissors;
    scissors.push_back(this->poScissor);

    //3> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        std::string pathVertShaderBase = g_pathModelShaderModules[i] + c_strVert;
        std::string pathFragShaderBase = g_pathModelShaderModules[i] + c_strFrag;
        VkShaderModule vertShaderBase = findShaderModule(pathVertShaderBase);
        VkShaderModule fragShaderBase = findShaderModule(pathFragShaderBase);

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkPipeline(vertShaderBase, "main",
                                                                      fragShaderBase, "main",
                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex),
                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                      this->poRenderPass, this->poPipelineLayout, viewports, scissors,
                                                                      pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits,
                                                                      pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                      pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                      pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                      pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                      pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
        {
            std::string msg = "Vulkan_011_Texturing::createPipeline_Custom: Failed to create pipeline wire frame !";
            Util_LogError(msg.c_str());
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
        pModelObject->poPipelineGraphics = createVkPipeline(vertShaderBase, "main",
                                                            fragShaderBase, "main",
                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                            this->poRenderPass, this->poPipelineLayout, viewports, scissors,
                                                            pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE,
                                                            isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                            pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                            pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                            pModelObject->cfg_ColorWriteMask);
        if (pModelObject->poPipelineGraphics == VK_NULL_HANDLE)
        {
            std::string msg = "Vulkan_011_Texturing::createPipeline_Custom: Failed to create pipeline !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }

}

void Vulkan_011_Texturing::destroyModelTextures()
{
    size_t count = this->m_aModelTexture.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelTexture* pTexture = this->m_aModelTexture[i];
        delete pTexture;
    }
    this->m_aModelTexture.clear();
    this->m_mapModelTexture.clear();
}
void Vulkan_011_Texturing::createModelTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        std::string nameTexture = g_pathTextures[3 * i + 0];
        std::string nameType = g_pathTextures[3 * i + 1];
        VulkanTextureType typeTexture = Util_ParseTextureType(nameType);
        std::string pathTextures = g_pathTextures[3 * i + 2];

        std::vector<std::string> aPathTexture = StringUtil::Split(pathTextures, ";");
        ModelTexture* pTexture = new ModelTexture(this, 
                                                  nameTexture,
                                                  typeTexture,
                                                  g_formatTextures[i],
                                                  g_filterTextures[i],
                                                  g_addressingTextures[i],
                                                  g_borderColorTextures[i],
                                                  aPathTexture);
        pTexture->texChunkMaxX = g_animChunkTextures[i * 2 + 0];
        pTexture->texChunkMaxY = g_animChunkTextures[i * 2 + 1];
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = MathUtil::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
        }
        pTexture->AddRef();

        int width = g_sizeTextures[3 * i + 0];
        int height = g_sizeTextures[3 * i + 1];
        int depth = g_sizeTextures[3 * i + 1];
        pTexture->LoadTexture(width, 
                              height,
                              depth);

        this->m_aModelTexture.push_back(pTexture);
        this->m_mapModelTexture[nameTexture] = pTexture;

        Util_LogInfo("Vulkan_011_Texturing::createModelTextures: create texture: [%s], type: [%s], path: [%s] success !", nameTexture.c_str(), nameType.c_str(), pathTextures.c_str());
    }
}
Vulkan_011_Texturing::ModelTexture* Vulkan_011_Texturing::findModelTexture(const std::string& nameTexture)
{
    ModelTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}

void Vulkan_011_Texturing::destroyDescriptorSetLayouts()
{

}
void Vulkan_011_Texturing::createDescriptorSetLayouts()
{

}
VkDescriptorSetLayout Vulkan_011_Texturing::findDescriptorSetLayout(const std::string& nameDescriptorSetLayout)
{
    return VK_NULL_HANDLE;
}

void Vulkan_011_Texturing::destroyShaderModules()
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
void Vulkan_011_Texturing::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        std::string pathVert = g_pathShaderModules[2 * i + 0];
        std::string pathFrag = g_pathShaderModules[2 * i + 1];

        //vert
        VkShaderModule vertShaderModule = createShaderModule("VertexShader: ", pathVert);
        this->m_aVkShaderModules.push_back(vertShaderModule);
        this->m_mapVkShaderModules[pathVert] = vertShaderModule;
        Util_LogInfo("Vulkan_011_Texturing::createShaderModules: create shader [%s] success !", pathVert.c_str());

        //frag
        VkShaderModule fragShaderModule = createShaderModule("FragmentShader: ", pathFrag);
        this->m_aVkShaderModules.push_back(fragShaderModule);
        this->m_mapVkShaderModules[pathFrag] = fragShaderModule;
        Util_LogInfo("Vulkan_011_Texturing::createShaderModules: create shader [%s] success !", pathFrag.c_str());
    }
}
VkShaderModule Vulkan_011_Texturing::findShaderModule(const std::string& pathShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(pathShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   

void Vulkan_011_Texturing::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        createDescriptorSets(pModelObject->poDescriptorSets);
        for (size_t j = 0; j < count_sci; j++)
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
            bufferInfo_Material.buffer = pModelObject->poBuffers_materialCB[j];
            bufferInfo_Material.offset = 0;
            bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;

            VkDescriptorBufferInfo bufferInfo_Instance = {};
            bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
            bufferInfo_Instance.offset = 0;
            bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();

            VkDescriptorImageInfo imageInfo = {};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = pModelObject->GetTextureImageView(0);
            imageInfo.sampler = pModelObject->GetTextureSampler(0);
            
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

void Vulkan_011_Texturing::updateCBs_Custom()
{
    float time = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        ModelTexture* pTexture1 = pModelObject->GetTexture(0);

        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
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

            //MaterialConstants
            MaterialConstants& materialCB = pModelObject->materialCBs[j];
            if (pTexture1->texChunkMaxX > 0 &&
                pTexture1->texChunkMaxY > 0)
            {
                if (++ pTexture1->frameCurrent >= 30)
                {
                    pTexture1->frameCurrent = 0;
                    pTexture1->texChunkIndex ++;
                    if (pTexture1->texChunkIndex >= pTexture1->texChunkMaxX * pTexture1->texChunkMaxY)
                    {
                        pTexture1->texChunkIndex = 0;
                    }
                    int indexX = pTexture1->texChunkIndex % pTexture1->texChunkMaxX;
                    int indexZ = pTexture1->texChunkIndex / pTexture1->texChunkMaxX;
                    materialCB.texChunkIndexX = indexX;
                    materialCB.texChunkIndexY = indexZ;
                }   
            }
        }

        //ObjectConstants
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->objectCBs.data(), sizeof(ObjectConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(MaterialConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->materialCBs.data(), sizeof(MaterialConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
    }
}



bool Vulkan_011_Texturing::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_011_Texturing", &windowOpened, 0);
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
            ModelTexture* pTexture1 = pModelObject->GetTexture(0);

            std::string nameModel = StringUtil::SaveInt(i) + " - " + pModelObject->nameModel;
            if (ImGui::CollapsingHeader(nameModel.c_str()))
            {
                std::string nameIsShow = "Is Show - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                std::string nameIsWireFrame = "Is WireFrame - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                std::string nameIsRotate = "Is Rotate - " + pModelObject->nameModel;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                std::string nameIsTransparent = "Is Transparent - " + pModelObject->nameModel;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                std::string nameIsLighting = "Is Lighting - " + pModelObject->nameModel;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < pModelObject->countInstance; j++)
                    {
                        MaterialConstants& mat = pModelObject->materialCBs[j];
                        mat.lighting = pModelObject->isLighting;
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
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        std::string nameModelInstance = nameModel + " - " + StringUtil::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            std::string nameObject = StringUtil::SaveInt(j) + " - Object - " + pModelObject->nameModel;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
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
                            }
                            
                            //MaterialConstants
                            std::string nameMaterial = StringUtil::SaveInt(j) + " - Material - " + pModelObject->nameModel;
                            if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                            {
                                //factorAmbient
                                std::string nameFactorAmbient = "FactorAmbient - " + StringUtil::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                {

                                }
                                ImGui::Spacing();

                                //factorDiffuse
                                std::string nameFactorDiffuse = "FactorDiffuse - " + StringUtil::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                {

                                }
                                ImGui::Spacing();

                                //factorSpecular
                                std::string nameFactorSpecular = "FactorSpecular - " + StringUtil::SaveInt(j);
                                if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                {

                                }
                                ImGui::Spacing();

                                //shininess
                                std::string nameShininess = "Shininess - " + StringUtil::SaveInt(j);
                                if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //alpha
                                std::string nameAlpha = "Alpha - " + StringUtil::SaveInt(j);
                                if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //lighting
                                std::string nameLighting = "Lighting - " + StringUtil::SaveInt(j);
                                bool isLighting = mat.lighting == 1.0f ? true : false;
                                if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                {
                                    mat.lighting = isLighting ? 1.0f : 0.0f;
                                }

                                //indexTextureArray
                                std::string nameIndexTextureArray = "IndexTextureArray - " + StringUtil::SaveInt(j);
                                if (pTexture1->typeTexture == Vulkan_Texture_2DArray)
                                {
                                    int count_tex = (int)pTexture1->aPathTexture.size();
                                    int indexTextureArray = (int)mat.indexTextureArray;
                                    if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex-1))
                                    {
                                        mat.indexTextureArray = indexTextureArray;
                                    }
                                }
                                else 
                                {
                                    if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.indexTextureArray, 0.001f, 0.0f, 1.0f))
                                    {

                                    }
                                }

                                //texSpeedU
                                std::string nameTexSpeedU = "TexSpeedU - " + StringUtil::SaveInt(j);
                                if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.texSpeedU, 0.01f, 0.0f, 100.0f))
                                {
                                    
                                }

                                //texSpeedV
                                std::string nameTexSpeedV = "texSpeedV - " + StringUtil::SaveInt(j);
                                if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.texSpeedV, 0.01f, 0.0f, 100.0f))
                                {
                                    
                                }

                                //texSpeedW
                                std::string nameTexSpeedW = "texSpeedW - " + StringUtil::SaveInt(j);
                                if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.texSpeedW, 0.01f, 0.0f, 100.0f))
                                {
                                    
                                }

                                //texChunkMaxX
                                std::string nameTexChunkMaxX = "texChunkMaxX - " + StringUtil::SaveInt(j);
                                float fTexChunkMaxX = mat.texChunkMaxX;
                                ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);

                                //texChunkMaxY
                                std::string nameTexChunkMaxY = "texChunkMaxY - " + StringUtil::SaveInt(j);
                                float fTexChunkMaxY = mat.texChunkMaxY;
                                ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);

                                //texChunkIndexX
                                std::string nameTexChunkIndexX = "texChunkIndexX - " + StringUtil::SaveInt(j);
                                float fTexChunkIndexX = mat.texChunkIndexX;
                                ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);

                                //texChunkIndexY
                                std::string nameTexChunkIndexY = "texChunkIndexY - " + StringUtil::SaveInt(j);
                                float fTexChunkIndexY = mat.texChunkIndexY;
                                ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);


                                ImGui::Spacing();
                            }
                        }
                    }
                }
            }
        }
        
        ImGui::Separator();
        ImGui::Spacing();

        //2> PassConstants
        if (ImGui::CollapsingHeader("PassConstants Settings"))
        {
            passConstantsConfig();
        }
        //3> Camera
        if (ImGui::CollapsingHeader("Camera Settings"))
        {
            cameraConfig();
        }
        //4> Light
        if (ImGui::CollapsingHeader("Light Settings"))
        {
            lightConfig();
        }
    }
    ImGui::End();

    return true;
}

void Vulkan_011_Texturing::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_011_Texturing::drawMesh_Custom(VkCommandBuffer& commandBuffer)
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
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        
    }
}
void Vulkan_011_Texturing::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
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

void Vulkan_011_Texturing::cleanupCustom()
{   
    destroyModelTextures();

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        UTIL_DELETE(pModelObject)
    }
    this->m_aModelObjects.clear();
    this->m_aModelObjects_Render.clear();
    this->m_mapModelObjects.clear();

    destroyDescriptorSetLayouts();
}

void Vulkan_011_Texturing::cleanupSwapChain_Custom()
{
    destroyShaderModules();

    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->cleanupSwapChain();
    }
}

void Vulkan_011_Texturing::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}