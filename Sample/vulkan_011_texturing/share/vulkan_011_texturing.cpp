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



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 6;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "viking_room",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/viking_room/viking_room.obj", //viking_room
    "bunny",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/bunny/bunny.obj", //bunny

    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane
    "plane_nt",         "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane_nt
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/cube/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/sphere.fbx", //sphere

};

static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    false, //viking_room
    false, //bunny

    true, //plane
    true, //plane_nt
    false, //cube
    false, //sphere

};

static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    true, //viking_room
    false, //bunny

    false, //plane  
    false, //plane_nt  
    false, //cube
    false, //sphere

};

static glm::mat4 g_MeshTranformLocals[g_MeshCount] = 
{
    MathUtil::RotateX(-90.0f), //viking_room
    MathUtil::ms_mat4Unit, //bunny

    MathUtil::ms_mat4Unit, //plane
    MathUtil::ms_mat4Unit, //plane_nt
    MathUtil::ms_mat4Unit, //cube
    MathUtil::ms_mat4Unit, //sphere

};


/////////////////////////// Texture /////////////////////////////
static const std::string g_TextureDefault = "default";
static const int g_TextureCount = 18;
static const char* g_TexturePaths[3 * g_TextureCount] = 
{
    "default",                      "2d",           "Assets/Texture/default_blackwhite.png", //default
    "terrain",                      "2d",           "Assets/Texture/terrain.png", //terrain
    "viking_room",                  "2d",           "Assets/Model/Obj/viking_room/viking_room.png", //viking_room
    "white",                        "2d",           "Assets/Texture/white.bmp", //white
    
////Basic-Level Texture Operation
    "texturesampler_wrap",          "2d",           "Assets/Texture/texture2d.jpg", //texturesampler_wrap
    "texturesampler_mirror",        "2d",           "Assets/Texture/texture2d.jpg", //texturesampler_mirror
    "texturesampler_clamp",         "2d",           "Assets/Texture/texture2d.jpg", //texturesampler_clamp
    "texturesampler_border",        "2d",           "Assets/Texture/texture2d.jpg", //texturesampler_border
    "texture1d",                    "1d",           "Assets/Texture/texture1d.tga", //texture1d
    "texture2d",                    "2d",           "Assets/Texture/texture2d.jpg", //texture2d
    "texture2darray",               "2darray",      "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture2darray
    "texture3d",                    "3d",           "", //texture3d
    "texturecubemap",               "cubemap",      "Assets/Texture/texturecubemap_x_right.png;Assets/Texture/texturecubemap_x_left.png;Assets/Texture/texturecubemap_y_up.png;Assets/Texture/texturecubemap_y_down.png;Assets/Texture/texturecubemap_z_front.png;Assets/Texture/texturecubemap_z_back.png", //texturecubemap
    "textureanimation_scroll",      "2darray",      "Assets/Texture/textureanimation1.png;Assets/Texture/textureanimation2.png", //textureanimation_scroll
    "textureanimation_chunk",       "2darray",      "Assets/Texture/textureanimation3.png", //textureanimation_chunk

////High-Level Texture Operation
    "texturebumpmap_diffuse",       "2d",           "Assets/Texture/bricks_diffuse.png", //texturebumpmap_diffuse
    "texturebumpmap_bumpmap",       "2d",           "Assets/Texture/bricks_bumpmap.png", //texturebumpmap_bumpmap
    "texturenormalmap_normalmap",   "2d",           "Assets/Texture/bricks_normalmap.png", //texturenormalmap_normalmap

};
static VkFormat g_TextureFormats[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //default
    VK_FORMAT_R8G8B8A8_SRGB, //terrain
    VK_FORMAT_R8G8B8A8_SRGB, //viking_room
    VK_FORMAT_R8G8B8A8_SRGB, //white

////Basic-Level Texture Operation
    VK_FORMAT_R8G8B8A8_SRGB, //texturesampler_wrap
    VK_FORMAT_R8G8B8A8_SRGB, //texturesampler_mirror
    VK_FORMAT_R8G8B8A8_SRGB, //texturesampler_clamp
    VK_FORMAT_R8G8B8A8_SRGB, //texturesampler_border
    VK_FORMAT_R8G8B8A8_SRGB, //texture1d
    VK_FORMAT_R8G8B8A8_SRGB, //texture2d
    VK_FORMAT_R8G8B8A8_SRGB, //texture2darray
    VK_FORMAT_R8_UNORM, //texture3d
    VK_FORMAT_R8G8B8A8_SRGB, //texturecubemap
    VK_FORMAT_R8G8B8A8_SRGB, //textureanimation_scroll
    VK_FORMAT_R8G8B8A8_SRGB, //textureanimation_chunk

////High-Level Texture Operation
    VK_FORMAT_R8G8B8A8_SRGB, //texturebumpmap_diffuse
    VK_FORMAT_R8G8B8A8_SRGB, //texturebumpmap_bumpmap
    VK_FORMAT_R8G8B8A8_UNORM, //texturenormalmap_normalmap

};
static VulkanTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //default
    Vulkan_TextureFilter_Bilinear, //terrain
    Vulkan_TextureFilter_Bilinear, //viking_room
    Vulkan_TextureFilter_Bilinear, //white

////Basic-Level Texture Operation
    Vulkan_TextureFilter_Bilinear, //texturesampler_wrap
    Vulkan_TextureFilter_Bilinear, //texturesampler_mirror
    Vulkan_TextureFilter_Bilinear, //texturesampler_clamp
    Vulkan_TextureFilter_Bilinear, //texturesampler_border
    Vulkan_TextureFilter_Bilinear, //texture1d
    Vulkan_TextureFilter_Bilinear, //texture2d
    Vulkan_TextureFilter_Bilinear, //texture2darray
    Vulkan_TextureFilter_Bilinear, //texture3d
    Vulkan_TextureFilter_Bilinear, //texturecubemap
    Vulkan_TextureFilter_Bilinear, //textureanimation_scroll
    Vulkan_TextureFilter_Bilinear, //textureanimation_chunk

////High-Level Texture Operation
    Vulkan_TextureFilter_Bilinear, //texturebumpmap_diffuse
    Vulkan_TextureFilter_Bilinear, //texturebumpmap_bumpmap
    Vulkan_TextureFilter_Bilinear, //texturenormalmap_normalmap

};
static VulkanTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //default
    Vulkan_TextureAddressing_Clamp, //terrain
    Vulkan_TextureAddressing_Clamp, //viking_room
    Vulkan_TextureAddressing_Clamp, //white

////Basic-Level Texture Operation
    Vulkan_TextureAddressing_Wrap, //texturesampler_wrap
    Vulkan_TextureAddressing_Mirror, //texturesampler_mirror
    Vulkan_TextureAddressing_Clamp, //texturesampler_clamp
    Vulkan_TextureAddressing_Border, //texturesampler_border
    Vulkan_TextureAddressing_Clamp, //texture1d
    Vulkan_TextureAddressing_Clamp, //texture2d
    Vulkan_TextureAddressing_Clamp, //texture2darray
    Vulkan_TextureAddressing_Clamp, //texture3d
    Vulkan_TextureAddressing_Wrap, //texturecubemap
    Vulkan_TextureAddressing_Wrap, //textureanimation_scroll
    Vulkan_TextureAddressing_Wrap, //textureanimation_chunk

////High-Level Texture Operation
    Vulkan_TextureAddressing_Clamp, //texturebumpmap_diffuse
    Vulkan_TextureAddressing_Clamp, //texturebumpmap_bumpmap
    Vulkan_TextureAddressing_Clamp, //texturenormalmap_normalmap

};
static VulkanTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //default
    Vulkan_TextureBorderColor_OpaqueBlack, //terrain
    Vulkan_TextureBorderColor_OpaqueBlack, //viking_room
    Vulkan_TextureBorderColor_OpaqueBlack, //white

////Basic-Level Texture Operation
    Vulkan_TextureBorderColor_OpaqueBlack, //texturesampler_wrap
    Vulkan_TextureBorderColor_OpaqueBlack, //texturesampler_mirror
    Vulkan_TextureBorderColor_OpaqueBlack, //texturesampler_clamp
    Vulkan_TextureBorderColor_OpaqueBlack, //texturesampler_border
    Vulkan_TextureBorderColor_OpaqueBlack, //texture1d
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2d
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2darray
    Vulkan_TextureBorderColor_OpaqueBlack, //texture3d
    Vulkan_TextureBorderColor_OpaqueBlack, //texturecubemap
    Vulkan_TextureBorderColor_OpaqueBlack, //textureanimation_scroll
    Vulkan_TextureBorderColor_OpaqueBlack, //textureanimation_chunk

////High-Level Texture Operation
    Vulkan_TextureBorderColor_OpaqueBlack, //texturebumpmap_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //texturebumpmap_bumpmap
    Vulkan_TextureBorderColor_OpaqueBlack, //texturenormalmap_normalmap

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //default
    512,    512,    1, //terrain
   1024,   1024,    1, //viking_room
     64,     64,    1, //white

////Basic-Level Texture Operation
    512,    512,    1, //texturesampler_wrap
    512,    512,    1, //texturesampler_mirror
    512,    512,    1, //texturesampler_clamp
    512,    512,    1, //texturesampler_border
     64,      1,    1, //texture1d
    512,    512,    1, //texture2d
   1024,   1024,    1, //texture2darray
    128,    128,    128, //texture3d
    512,    512,    1, //texturecubemap
     64,     64,    1, //textureanimation_scroll
    512,    512,    1, //textureanimation_chunk

////High-Level Texture Operation
    512,    512,    1, //texturebumpmap_diffuse
    512,    512,    1, //texturebumpmap_bumpmap
    512,    512,    1, //texturenormalmap_normalmap

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default
    0,    0, //terrain
    0,    0, //viking_room
    0,    0, //white

////Basic-Level Texture Operation
    0,    0, //texturesampler_wrap
    0,    0, //texturesampler_mirror
    0,    0, //texturesampler_clamp
    0,    0, //texturesampler_border
    0,    0, //texture1d
    0,    0, //texture2d
    0,    0, //texture2darray
    0,    0, //texture3d
    0,    0, //texturecubemap
    0,    0, //textureanimation_scroll
    4,    8, //textureanimation_chunk

////High-Level Texture Operation
    0,    0, //texturebumpmap_diffuse
    0,    0, //texturebumpmap_bumpmap
    0,    0, //texturenormalmap_normalmap

};


/////////////////////////// DescriptorSetLayout /////////////////
const std::string c_strLayout_Pass = "Pass";
const std::string c_strLayout_Object = "Object";
const std::string c_strLayout_Material = "Material";
const std::string c_strLayout_Instance = "Instance";
const std::string c_strLayout_TextureVS = "TextureVS";
const std::string c_strLayout_TextureFS = "TextureFS";
static const int g_DescriptorSetLayoutCount = 2;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS",
};


/////////////////////////// Shader //////////////////////////////
const std::string c_strVert = ".vert.spv";
const std::string c_strFrag = ".frag.spv";
static const int g_ShaderCount = 11;
static const char* g_ShaderModulePaths[2 * g_ShaderCount] = 
{
////Basic-Level Texture Operation
    "Assets/Shader/standard_mesh_opaque_texsampler_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texsampler_lit.frag.spv", //standard_mesh_opaque_texsampler_lit
    "Assets/Shader/standard_mesh_opaque_tex1d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex1d_lit.frag.spv", //standard_mesh_opaque_tex1d_lit
    "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit
    "Assets/Shader/standard_mesh_opaque_tex2darray_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit
    "Assets/Shader/standard_mesh_opaque_tex3d_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_tex3d_lit.frag.spv", //standard_mesh_opaque_tex3d_lit
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit
    "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.frag.spv", //standard_mesh_opaque_texanim_scroll_lit
    "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.frag.spv", //standard_mesh_opaque_texanim_chunk_lit
    
////High-Level Texture Operation
    "Assets/Shader/standard_mesh_opaque_texbumpmap_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texbumpmap_lit.frag.spv", //standard_mesh_opaque_texbumpmap_lit
    "Assets/Shader/standard_mesh_opaque_texnormalmap_lit.vert.spv", "Assets/Shader/standard_mesh_opaque_texnormalmap_lit.frag.spv", //standard_mesh_opaque_texnormalmap_lit


////Transparent
    "Assets/Shader/standard_mesh_transparent_lit.vert.spv", "Assets/Shader/standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit
};


/////////////////////////// Object //////////////////////////////
static const int g_ObjectCount = 18;
static const char* g_ObjectConfigs[5 * g_ObjectCount] = 
{
    //Object Name                       //Mesh Path                    //Texture One                           //Texture Two                    //Texture Three
    "ground",                           "plane",                       "terrain",                              "",                              "", //ground
    "viking_room",                      "viking_room",                 "viking_room",                          "",                              "", //viking_room
    "bunny",                            "bunny",                       "white",                                "",                              "", //bunny  

////Basic-Level Texture Operation
    "textureSampler_Wrap",              "plane",                       "texturesampler_wrap",                  "",                              "", //textureSampler_Wrap
    "textureSampler_Mirror",            "plane",                       "texturesampler_mirror",                "",                              "", //textureSampler_Mirror
    "textureSampler_Clamp",             "plane",                       "texturesampler_clamp",                 "",                              "", //textureSampler_Clamp
    "textureSampler_Border",            "plane",                       "texturesampler_border",                "",                              "", //textureSampler_Border
    "texture1D",                        "plane",                       "texture1d",                            "",                              "", //texture1D
    "texture2D",                        "plane",                       "texture2d",                            "",                              "", //texture2D
    "texture2Darray",                   "plane",                       "texture2darray",                       "",                              "", //texture2Darray
    "texture3D",                        "plane",                       "texture3d",                            "",                              "", //texture3D
    "textureCubeMap_SkyBox",            "cube",                        "texturecubemap",                       "",                              "", //textureCubeMap_SkyBox
    "textureCubeMap_Sphere",            "sphere",                      "texturecubemap",                       "",                              "", //textureCubeMap_Sphere
    "textureAnimation_Scroll",          "plane",                       "textureanimation_scroll",              "",                              "", //textureAnimation_Scroll
    "textureAnimation_Chunk",           "plane",                       "textureanimation_chunk",               "",                              "", //textureAnimation_Chunk

////High-Level Texture Operation
    "textureOriginal",                  "plane",                       "texturebumpmap_diffuse",               "",                              "", //textureOriginal    
    "textureBumpMap",                   "plane",                       "texturebumpmap_diffuse",               "texturebumpmap_bumpmap",        "", //textureBumpMap
    "textureNormalMap",                 "plane_nt",                    "texturebumpmap_diffuse",               "texturenormalmap_normalmap",    "", //textureNormalMap

};

static const std::string g_Object_Texture3D = "texture3D";
static const std::string g_Object_TextureAnimation_Scroll = "textureAnimation_Scroll";
static const std::string g_Object_TextureAnimation_Chunk = "textureAnimation_Chunk";
static const std::string g_Object_TextureOriginal = "textureOriginal";
static const std::string g_Object_TextureBumpMap = "textureBumpMap";
static const std::string g_Object_TextureNormalMap = "textureNormalMap";


static const char* g_ObjectPathShaderModules[g_ObjectCount] = 
{
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //ground 
    "Assets/Shader/standard_mesh_transparent_lit", //viking_room
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //bunny 

////Basic-Level Texture Operation
    "Assets/Shader/standard_mesh_opaque_texsampler_lit", //textureSampler_Wrap 
    "Assets/Shader/standard_mesh_opaque_texsampler_lit", //textureSampler_Mirror 
    "Assets/Shader/standard_mesh_opaque_texsampler_lit", //textureSampler_Clamp 
    "Assets/Shader/standard_mesh_opaque_texsampler_lit", //textureSampler_Border 
    "Assets/Shader/standard_mesh_opaque_tex1d_lit", //texture1D 
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //texture2D 
    "Assets/Shader/standard_mesh_opaque_tex2darray_lit", //texture2Darray
    "Assets/Shader/standard_mesh_opaque_tex3d_lit", //texture3D
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit", //textureCubeMap_SkyBox
    "Assets/Shader/standard_mesh_opaque_texcubemap_lit", //textureCubeMap_Sphere
    "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit", //textureAnimation_Scroll
    "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit", //textureAnimation_Chunk

////High-Level Texture Operation
    "Assets/Shader/standard_mesh_opaque_tex2d_lit", //textureOriginal 
    "Assets/Shader/standard_mesh_opaque_texbumpmap_lit", //textureBumpMap
    "Assets/Shader/standard_mesh_opaque_texnormalmap_lit", //textureNormalMap
    
};

static const char* g_ObjectNameDescriptorSetLayouts[g_ObjectCount] = 
{
    "Pass-Object-Material-Instance-TextureFS", //ground 
    "Pass-Object-Material-Instance-TextureFS", //viking_room
    "Pass-Object-Material-Instance-TextureFS", //bunny 

////Basic-Level Texture Operation
    "Pass-Object-Material-Instance-TextureFS", //textureSampler_Wrap 
    "Pass-Object-Material-Instance-TextureFS", //textureSampler_Mirror 
    "Pass-Object-Material-Instance-TextureFS", //textureSampler_Clamp 
    "Pass-Object-Material-Instance-TextureFS", //textureSampler_Border 
    "Pass-Object-Material-Instance-TextureFS", //texture1D 
    "Pass-Object-Material-Instance-TextureFS", //texture2D 
    "Pass-Object-Material-Instance-TextureFS", //texture2Darray
    "Pass-Object-Material-Instance-TextureFS", //texture3D
    "Pass-Object-Material-Instance-TextureFS", //textureCubeMap_SkyBox
    "Pass-Object-Material-Instance-TextureFS", //textureCubeMap_Sphere
    "Pass-Object-Material-Instance-TextureFS", //textureAnimation_Scroll
    "Pass-Object-Material-Instance-TextureFS", //textureAnimation_Chunk

////High-Level Texture Operation
    "Pass-Object-Material-Instance-TextureFS", //textureOriginal 
    "Pass-Object-Material-Instance-TextureFS-TextureFS", //textureBumpMap
    "Pass-Object-Material-Instance-TextureFS-TextureFS", //textureNormalMap

};


static float g_instanceGap = 1.2f;
static int g_ObjectInstanceExtCount[g_ObjectCount] =
{
    0, //ground
    0, //viking_room
    0, //bunny

////Basic-Level Texture Operation
    5, //textureSampler_Wrap 
    5, //textureSampler_Mirror 
    5, //textureSampler_Clamp 
    5, //textureSampler_Border 
    5, //texture1D 
    5, //texture2D 
    5, //texture2Darray 
    5, //texture3D 
    0, //textureCubeMap_SkyBox 
    5, //textureCubeMap_Sphere 
    5, //textureAnimation_Scroll
    5, //textureAnimation_Chunk

////High-Level Texture Operation
    5, //textureOriginal 
    5, //textureBumpMap 
    5, //textureNormalMap 

};

static glm::vec3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    glm::vec3(   0, -0.1,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //ground
    glm::vec3(   0,    0,   -2),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //viking_room
    glm::vec3(   0,    0,   -4),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //bunny

////Basic-Level Texture Operation
    glm::vec3(   0,  0.4,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureSampler_Wrap
    glm::vec3(   0,  1.5,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureSampler_Mirror
    glm::vec3(   0,  2.6,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureSampler_Clamp
    glm::vec3(   0,  3.7,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureSampler_Border
    glm::vec3(   0,  4.8,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture1D
    glm::vec3(   0,  5.9,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2D
    glm::vec3(   0,  7.0,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2Darray
    glm::vec3(   0,  8.1,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture3D
    glm::vec3(   0,    0,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    glm::vec3(   0,  9.2,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 0.005f,  0.005f,  0.005f), //textureCubeMap_Sphere
    glm::vec3(   0, 10.3,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureAnimation_Scroll
    glm::vec3(   0, 11.4,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureAnimation_Chunk

////High-Level Texture Operation
    glm::vec3(   0, 12.5,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureOriginal
    glm::vec3(   0, 13.6,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureBumpMap
    glm::vec3(   0, 14.7,    0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //textureNormalMap

};

static bool g_ObjectIsTransparents[g_ObjectCount] = 
{
    false, //ground
    true, //viking_room
    false, //bunny

////Basic-Level Texture Operation
    false, //textureSampler_Wrap
    false, //textureSampler_Mirror
    false, //textureSampler_Clamp
    false, //textureSampler_Border
    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere
    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk

////High-Level Texture Operation
    false, //textureOriginal
    false, //textureBumpMap
    false, //textureNormalMap

};

static bool g_ObjectIsShows[] = 
{
    true, //ground
    false, //viking_room
    false, //bunny

////Basic-Level Texture Operation
    true, //textureSampler_Wrap
    true, //textureSampler_Mirror
    true, //textureSampler_Clamp
    true, //textureSampler_Border
    true, //texture1D
    true, //texture2D
    true, //texture2Darray
    true, //texture3D
    true, //textureCubeMap_SkyBox
    true, //textureCubeMap_Sphere
    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk

////High-Level Texture Operation
    true, //textureOriginal
    true, //textureBumpMap
    true, //textureNormalMap

};

static bool g_ObjectIsRotates[g_ObjectCount] =
{
    false, //ground
    true, //viking_room
    true, //bunny

////Basic-Level Texture Operation
    false, //textureSampler_Wrap
    false, //textureSampler_Mirror
    false, //textureSampler_Clamp
    false, //textureSampler_Border
    false, //texture1D
    false, //texture2D
    false, //texture2Darray
    false, //texture3D
    false, //textureCubeMap_SkyBox
    false, //textureCubeMap_Sphere
    false, //textureAnimation_Scroll
    false, //textureAnimation_Chunk

////High-Level Texture Operation
    false, //textureOriginal
    false, //textureBumpMap
    false, //textureNormalMap

};

static bool g_ObjectIsLightings[g_ObjectCount] =
{
    true, //ground
    true, //viking_room
    true, //bunny

////Basic-Level Texture Operation
    true, //textureSampler_Wrap
    true, //textureSampler_Mirror
    true, //textureSampler_Clamp
    true, //textureSampler_Border
    true, //texture1D
    true, //texture2D
    true, //texture2Darray
    true, //texture3D
    true, //textureCubeMap_SkyBox
    true, //textureCubeMap_Sphere
    true, //textureAnimation_Scroll
    true, //textureAnimation_Chunk

////High-Level Texture Operation
    true, //textureOriginal
    true, //textureBumpMap
    true, //textureNormalMap

};



/////////////////////////// ModelMesh ///////////////////////////
bool Vulkan_011_Texturing::ModelMesh::LoadMesh(bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
    if (!VulkanMeshLoader::LoadMeshData(this->pathMesh, meshData, eMeshParserFlags))
    {
        Util_LogError("Vulkan_011_Texturing::ModelMesh::LoadMesh: load mesh failed: [%s] !", this->pathMesh.c_str());
        return false; 
    }

    int count_vertex = (int)meshData.vertices.size();
    if (this->poTypeVertex == Vulkan_Vertex_Pos3Color4Normal3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tex2.reserve(count_vertex);
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
            this->vertices_Pos3Color4Normal3Tex2.push_back(v);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        Util_LogInfo("Vulkan_011_Texturing::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMesh.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tex2.size(), 
                     (int)this->indices.size());
    }
    else if (this->poTypeVertex == Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tangent3Tex2.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            MeshVertex& vertex = meshData.vertices[i];
            Vertex_Pos3Color4Normal3Tangent3Tex2 v;
            v.pos = vertex.pos;
            v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = vertex.texCoord;
            if (isTranformLocal)
            {
                v.pos = MathUtil::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tangent3Tex2.push_back(v);

            // Util_LogInfo("Vulkan_011_Texturing::ModelMesh::LoadMesh: Normal: [%f, %f, %f], Tangent: [%f, %f, %f] !",
            //              v.normal.x, v.normal.y, v.normal.z,
            //              v.tangent.x, v.tangent.y, v.tangent.z);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        Util_LogInfo("Vulkan_011_Texturing::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMesh.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tangent3Tex2.size(), 
                     (int)this->indices.size());
    }

    //2> createVertexBuffer
    this->pWindow->createVertexBuffer(this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pWindow->createIndexBuffer(this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }

    return true;
}


/////////////////////////// ModelTexture ////////////////////////
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

    this->cfg_cameraPos = glm::vec3(-2.5f, 2.0f, -20.0f);
    this->cfg_cameraLookTarget = glm::vec3(-2.5f, 7.0f, 0.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = glm::vec3(0, 0, 1); //z+
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
    createModelMeshes();
    createModelTextures();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->indexModel = i;
        pModelObject->nameModel = g_ObjectConfigs[5 * i + 0];
        pModelObject->nameMesh = g_ObjectConfigs[5 * i + 1];
        pModelObject->nameDescriptorSetLayout = g_ObjectNameDescriptorSetLayouts[i];

        //Mesh
        {
            ModelMesh* pMesh = this->findModelMesh(pModelObject->nameMesh);
            assert(pMesh != nullptr && "Vulkan_011_Texturing::loadModel_Custom");
            pModelObject->SetMesh(pMesh);
        }

        int indexTex = 0;
        //Texture Channel 1
        {
            pModelObject->aTextureChannels.push_back(indexTex);
            std::string nameTexture1 = g_ObjectConfigs[5 * i + 2];
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
            std::string nameTexture2 = g_ObjectConfigs[5 * i + 3];
            if (!nameTexture2.empty())
            {
                ModelTexture* pTexture2 = this->findModelTexture(nameTexture2);
                pModelObject->AddTexture(pTexture2);
            }
            indexTex ++;
        }
        //Texture Chnnel 2
        {
            pModelObject->aTextureChannels.push_back(indexTex);
            std::string nameTexture3 = g_ObjectConfigs[5 * i + 4];
            if (!nameTexture3.empty())
            {
                ModelTexture* pTexture3 = this->findModelTexture(nameTexture3);
                pModelObject->AddTexture(pTexture3);
            }
            indexTex ++;
        }

        pModelObject->isTransparent = g_ObjectIsTransparents[i];
        pModelObject->isShow = g_ObjectIsShows[i];
        pModelObject->isRotate = g_ObjectIsRotates[i];
        pModelObject->countInstanceExt = g_ObjectInstanceExtCount[i];
        pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;

        m_aModelObjects.push_back(pModelObject);
        if (pModelObject->isTransparent)
            m_aModelObjects_Render.push_back(pModelObject);
        else 
            m_aModelObjects_Render.insert(m_aModelObjects_Render.begin(), pModelObject);
        m_mapModelObjects[pModelObject->nameModel] = pModelObject;
    }
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

        pModelObject->instanceMatWorld.clear();
        pModelObject->objectCBs.clear();
        pModelObject->materialCBs.clear();
        for (int j = 0; j < pModelObject->countInstance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = MathUtil::FromTRS(g_ObjectTranforms[i * 3 + 0] + glm::vec3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                           g_ObjectTranforms[i * 3 + 1],
                                                           g_ObjectTranforms[i * 3 + 2]);
            pModelObject->objectCBs.push_back(objectConstants);
            pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = MathUtil::RandomColor(false);
            materialConstants.factorDiffuse = MathUtil::RandomColor(false);
            materialConstants.factorSpecular = MathUtil::RandomColor(false);
            materialConstants.shininess = MathUtil::RandF(10.0f, 100.0f);
            materialConstants.alpha = MathUtil::RandF(0.2f, 0.9f);
            materialConstants.lighting = g_ObjectIsLightings[i];

            //Texture
            int count_texture = pModelObject->GetTextureCount();
            for (int p = 0; p < count_texture; p++)
            {
                ModelTexture* pTexture = pModelObject->GetTexture(p);
                materialConstants.aTexLayers[p].texWidth = pTexture->width;
                materialConstants.aTexLayers[p].texHeight = pTexture->height;
                materialConstants.aTexLayers[p].texDepth = pTexture->depth;

                if (pModelObject->nameModel == g_Object_Texture3D) //Texture3D
                {
                    materialConstants.aTexLayers[p].indexTextureArray = MathUtil::RandF(0.0f, 1.0f);
                }
                else if (pModelObject->nameModel == g_Object_TextureAnimation_Scroll) //TextureAnimation_Scroll
                {
                    if (pTexture->typeTexture == Vulkan_Texture_2DArray)
                    {
                        materialConstants.aTexLayers[p].indexTextureArray = pTexture->RandomTextureIndex();

                        if (materialConstants.aTexLayers[p].indexTextureArray > 0)
                        {
                            materialConstants.aTexLayers[p].texSpeedU = MathUtil::RandF(1.0f, 10.0f);
                        }
                        else 
                        {
                            materialConstants.aTexLayers[p].texSpeedV = MathUtil::RandF(1.0f, 10.0f);
                        }
                    }
                }
                else if (pModelObject->nameModel == g_Object_TextureAnimation_Chunk) //TextureAnimation_Chunk
                {
                    if (pTexture->texChunkMaxX > 0 &&
                        pTexture->texChunkMaxY > 0)
                    {
                        materialConstants.aTexLayers[p].texChunkMaxX = pTexture->texChunkMaxX;
                        materialConstants.aTexLayers[p].texChunkMaxY = pTexture->texChunkMaxY;
                        int indexX = pTexture->texChunkIndex % pTexture->texChunkMaxX;
                        int indexZ = pTexture->texChunkIndex / pTexture->texChunkMaxX;
                        materialConstants.aTexLayers[p].texChunkIndexX = indexX;
                        materialConstants.aTexLayers[p].texChunkIndexY = indexZ;
                    }
                }       
                else if (pModelObject->nameModel == g_Object_TextureOriginal) //TextureOriginal
                {
                    if (j == pModelObject->countInstance / 2)
                    {
                        materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                        materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                        materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                        materialConstants.lighting = 0.0f;
                    }
                }
                else if (pModelObject->nameModel == g_Object_TextureBumpMap) //TextureBumpMap
                {
                    if (j == pModelObject->countInstance / 2)
                    {
                        materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                        materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                        materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                    }

                    if (p == 1)
                    {
                        materialConstants.aTexLayers[p].indexTextureArray = MathUtil::RandF(2000.0f, 10000.0f);
                    }
                } 
                else if (pModelObject->nameModel == g_Object_TextureNormalMap) //TextureNormalMap
                {
                    if (j == pModelObject->countInstance / 2)
                    {
                        materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                        materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                        materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                    }

                    if (p == 1)
                    {
                        materialConstants.aTexLayers[p].indexTextureArray = 0;
                    }
                }

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
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();

    //4> Viewport
    std::vector<VkViewport> viewports;
    viewports.push_back(this->poViewport);
    std::vector<VkRect2D> scissors;
    scissors.push_back(this->poScissor);

    //5> Pipeline
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        std::string pathVertShaderBase = g_ObjectPathShaderModules[i] + c_strVert;
        std::string pathFragShaderBase = g_ObjectPathShaderModules[i] + c_strFrag;
        VkShaderModule vertShaderBase = findShaderModule(pathVertShaderBase);
        VkShaderModule fragShaderBase = findShaderModule(pathFragShaderBase);
        pModelObject->poPipelineLayout = findPipelineLayout(pModelObject->nameDescriptorSetLayout);
        if (pModelObject->poPipelineLayout  == VK_NULL_HANDLE)
        {
            std::string msg = "Vulkan_011_Texturing::createPipeline_Custom: Can not find PipelineLayout by name: " + pModelObject->nameDescriptorSetLayout;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //poPipelineGraphics_WireFrame
        pModelObject->poPipelineGraphics_WireFrame = createVkPipeline(vertShaderBase, "main",
                                                                      fragShaderBase, "main",
                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                      this->poRenderPass, pModelObject->poPipelineLayout, viewports, scissors,
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
                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex), 
                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                            this->poRenderPass, pModelObject->poPipelineLayout, viewports, scissors,
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

void Vulkan_011_Texturing::destroyModelMeshes()
{
    size_t count = this->m_aModelMesh.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelMesh* pMesh = this->m_aModelMesh[i];
        delete pMesh;
    }
    this->m_aModelMesh.clear();
    this->m_mapModelMesh.clear();
}
void Vulkan_011_Texturing::createModelMeshes()
{
    for (int i = 0; i < g_MeshCount; i++)
    {
        std::string nameMesh = g_MeshPaths[5 * i + 0];
        std::string nameVertexType = g_MeshPaths[5 * i + 1];
        std::string nameMeshType = g_MeshPaths[5 * i + 2];
        std::string nameGeometryType = g_MeshPaths[5 * i + 3];
        std::string pathMesh = g_MeshPaths[5 * i + 4];
        
        VulkanVertexType typeVertex = Util_ParseVertexType(nameVertexType); 
        VulkanMeshType typeMesh = Util_ParseMeshType(nameMeshType);
        VulkanMeshGeometryType typeGeometryType = Vulkan_MeshGeometry_Triangle;
        if (!nameGeometryType.empty())
        {
            typeGeometryType = Util_ParseMeshGeometryType(nameGeometryType);
        }

        ModelMesh* pMesh = new ModelMesh(this, 
                                         nameMesh,
                                         pathMesh,
                                         typeMesh,
                                         typeGeometryType,
                                         typeVertex);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTranformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTranformLocal, g_MeshTranformLocals[i]))
        {
            std::string msg = "Vulkan_011_Texturing::createModelMeshes: create mesh: [" + nameMesh + "] failed !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        Util_LogInfo("Vulkan_011_Texturing::createModelMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                     nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
Vulkan_011_Texturing::ModelMesh* Vulkan_011_Texturing::findModelMesh(const std::string& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
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
        std::string nameTexture = g_TexturePaths[3 * i + 0];
        std::string nameType = g_TexturePaths[3 * i + 1];
        VulkanTextureType typeTexture = Util_ParseTextureType(nameType);
        std::string pathTextures = g_TexturePaths[3 * i + 2];

        std::vector<std::string> aPathTexture = StringUtil::Split(pathTextures, ";");
        ModelTexture* pTexture = new ModelTexture(this, 
                                                  nameTexture,
                                                  typeTexture,
                                                  g_TextureFormats[i],
                                                  g_TextureFilters[i],
                                                  g_TextureAddressings[i],
                                                  g_TextureBorderColors[i],
                                                  aPathTexture);
        pTexture->texChunkMaxX = g_TextureAnimChunks[i * 2 + 0];
        pTexture->texChunkMaxY = g_TextureAnimChunks[i * 2 + 1];
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = MathUtil::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
        }
        pTexture->AddRef();

        int width = g_TextureSizes[3 * i + 0];
        int height = g_TextureSizes[3 * i + 1];
        int depth = g_TextureSizes[3 * i + 1];
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
    size_t count = this->m_aVkDescriptorSetLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        VkDescriptorSetLayout& vkDescriptorSetLayout = this->m_aVkDescriptorSetLayouts[i];
        vkDestroyDescriptorSetLayout(this->poDevice, vkDescriptorSetLayout, nullptr);
    }
    this->m_aVkDescriptorSetLayouts.clear();
    this->m_mapVkDescriptorSetLayout.clear();
    this->m_mapName2Layouts.clear();
}
void Vulkan_011_Texturing::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        std::string nameLayout(g_nameDescriptorSetLayouts[i]);
        std::vector<std::string> aLayouts = StringUtil::Split(nameLayout, "-");
        size_t count_layout = aLayouts.size();

        VkDescriptorSetLayout vkDescriptorSetLayout;
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        for (size_t j = 0; j < count_layout; j++)
        {
            std::string& strLayout = aLayouts[j];
            if (strLayout == c_strLayout_Pass) //Pass
            {
                VkDescriptorSetLayoutBinding passMainLayoutBinding = {};
                passMainLayoutBinding.binding = j;
                passMainLayoutBinding.descriptorCount = 1;
                passMainLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                passMainLayoutBinding.pImmutableSamplers = nullptr;
                passMainLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(passMainLayoutBinding);
            }
            else if (strLayout == c_strLayout_Object) //Object
            {
                VkDescriptorSetLayoutBinding objectLayoutBinding = {};
                objectLayoutBinding.binding = j;
                objectLayoutBinding.descriptorCount = 1;
                objectLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                objectLayoutBinding.pImmutableSamplers = nullptr;
                objectLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(objectLayoutBinding);
            }
            else if (strLayout == c_strLayout_Material) //Material
            {
                VkDescriptorSetLayoutBinding materialLayoutBinding = {};
                materialLayoutBinding.binding = j;
                materialLayoutBinding.descriptorCount = 1;
                materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                materialLayoutBinding.pImmutableSamplers = nullptr;
                materialLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(materialLayoutBinding);
            }
            else if (strLayout == c_strLayout_Instance) //Instance
            {
                VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
                instanceLayoutBinding.binding = j;
                instanceLayoutBinding.descriptorCount = 1;
                instanceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                instanceLayoutBinding.pImmutableSamplers = nullptr;
                instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(instanceLayoutBinding);
            }
            else if (strLayout == c_strLayout_TextureVS) //TextureVS
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

                bindings.push_back(samplerLayoutBinding);
            }
            else if (strLayout == c_strLayout_TextureFS) //TextureFS
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(samplerLayoutBinding);
            }
            else
            {
                std::string msg = "Vulkan_011_Texturing::createDescriptorSetLayouts: Wrong DescriptorSetLayout type: " + strLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();
        if (vkCreateDescriptorSetLayout(this->poDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS) 
        {
            std::string msg = "VulkanWindow::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        Util_LogInfo("Vulkan_011_Texturing::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_011_Texturing::findDescriptorSetLayout(const std::string& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
std::vector<std::string>* Vulkan_011_Texturing::findDescriptorSetLayoutNames(const std::string& nameDescriptorSetLayout)
{
    std::map<std::string, std::vector<std::string>>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
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
        std::string pathVert = g_ShaderModulePaths[2 * i + 0];
        std::string pathFrag = g_ShaderModulePaths[2 * i + 1];

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


void Vulkan_011_Texturing::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        VkPipelineLayout& vkPipelineLayout = this->m_aVkPipelineLayouts[i];
        vkDestroyPipelineLayout(this->poDevice, vkPipelineLayout, nullptr);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_011_Texturing::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        std::string nameDescriptorSetLayout(g_nameDescriptorSetLayouts[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_011_Texturing::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(m_aVkDescriptorSetLayouts[i]);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_011_Texturing::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_011_Texturing::findPipelineLayout(const std::string& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
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
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(pModelObject->nameDescriptorSetLayout);
        std::vector<std::string>* pDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pModelObject->nameDescriptorSetLayout);

        assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_011_Texturing::createDescriptorSets_Custom");
        createDescriptorSets(pModelObject->poDescriptorSets, vkDescriptorSetLayout);
        for (size_t j = 0; j < count_sci; j++)
        {   
            std::vector<VkWriteDescriptorSet> descriptorWrites;
            int nIndexTexture = 0;

            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t p = 0; p < count_names; p++)
            {
                std::string& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
                if (nameDescriptorSet == c_strLayout_Pass) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);

                    VkWriteDescriptorSet ds0 = {};
                    ds0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds0.dstSet = pModelObject->poDescriptorSets[j];
                    ds0.dstBinding = p;
                    ds0.dstArrayElement = 0;
                    ds0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ds0.descriptorCount = 1;
                    ds0.pBufferInfo = &bufferInfo_Pass;
                    descriptorWrites.push_back(ds0);
                }
                else if (nameDescriptorSet == c_strLayout_Object) //Object
                {
                    VkDescriptorBufferInfo bufferInfo_Object = {};
                    bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
                    bufferInfo_Object.offset = 0;
                    bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;

                    VkWriteDescriptorSet ds1 = {};
                    ds1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds1.dstSet = pModelObject->poDescriptorSets[j];
                    ds1.dstBinding = p;
                    ds1.dstArrayElement = 0;
                    ds1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ds1.descriptorCount = 1;
                    ds1.pBufferInfo = &bufferInfo_Object;
                    descriptorWrites.push_back(ds1);
                }
                else if (nameDescriptorSet == c_strLayout_Material) //Material
                {
                    VkDescriptorBufferInfo bufferInfo_Material = {};
                    bufferInfo_Material.buffer = pModelObject->poBuffers_materialCB[j];
                    bufferInfo_Material.offset = 0;
                    bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;

                    VkWriteDescriptorSet ds2 = {};
                    ds2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds2.dstSet = pModelObject->poDescriptorSets[j];
                    ds2.dstBinding = p;
                    ds2.dstArrayElement = 0;
                    ds2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ds2.descriptorCount = 1;
                    ds2.pBufferInfo = &bufferInfo_Material;
                    descriptorWrites.push_back(ds2);
                }
                else if (nameDescriptorSet == c_strLayout_Instance) //Instance
                {
                    VkDescriptorBufferInfo bufferInfo_Instance = {};
                    bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
                    bufferInfo_Instance.offset = 0;
                    bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();

                    VkWriteDescriptorSet ds3 = {};
                    ds3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds3.dstSet = pModelObject->poDescriptorSets[j];
                    ds3.dstBinding = p;
                    ds3.dstArrayElement = 0;
                    ds3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ds3.descriptorCount = 1;
                    ds3.pBufferInfo = &bufferInfo_Instance;
                    descriptorWrites.push_back(ds3);
                }
                else if (nameDescriptorSet == c_strLayout_TextureVS || //TextureVS
                         nameDescriptorSet == c_strLayout_TextureFS) //TextureFS
                {
                    ModelTexture* pTexture = pModelObject->GetTexture(nIndexTexture);
                    nIndexTexture ++;

                    VkWriteDescriptorSet ds4 = {};
                    ds4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds4.dstSet = pModelObject->poDescriptorSets[j];
                    ds4.dstBinding = p;
                    ds4.dstArrayElement = 0;
                    ds4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    ds4.descriptorCount = 1;
                    ds4.pImageInfo = &pTexture->poTextureImageInfo;
                    descriptorWrites.push_back(ds4);
                }
                else
                {
                    std::string msg = "Vulkan_011_Texturing::createDescriptorSets_Custom: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        }
    }
}

void Vulkan_011_Texturing::updateCBs_Custom()
{
    float timeSinceStart = this->pTimer->GetTimeSinceStart();
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        size_t count_object = pModelObject->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pModelObject->objectCBs[j];
            if (pModelObject->isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pModelObject->instanceMatWorld[j], 
                                                  timeSinceStart * glm::radians(90.0f), 
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pModelObject->materialCBs[j];
            if (pModelObject->nameModel == g_Object_TextureAnimation_Chunk)
            {
                ModelTexture* pTexture = pModelObject->GetTexture(0);
                if (pTexture->texChunkMaxX > 0 &&
                    pTexture->texChunkMaxY > 0)
                {
                    if (++ pTexture->frameCurrent >= 30)
                    {
                        pTexture->frameCurrent = 0;
                        pTexture->texChunkIndex ++;
                        if (pTexture->texChunkIndex >= pTexture->texChunkMaxX * pTexture->texChunkMaxY)
                        {
                            pTexture->texChunkIndex = 0;
                        }
                        int indexX = pTexture->texChunkIndex % pTexture->texChunkMaxX;
                        int indexZ = pTexture->texChunkIndex / pTexture->texChunkMaxX;
                        materialCB.aTexLayers[0].texChunkIndexX = indexX;
                        materialCB.aTexLayers[0].texChunkIndexY = indexZ;
                    }   
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

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->pMesh->poVertexCount, (int)pModelObject->pMesh->poIndexCount);
                
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

                                //Texture
                                int count_texture = pModelObject->GetTextureCount();
                                for (int p = 0; p < count_texture; p ++)
                                {
                                    ModelTexture* pTexture = pModelObject->GetTexture(p);

                                    std::string nameMaterial_Texture = StringUtil::SaveInt(j) + " - Material - " + pModelObject->nameModel + " - Texture - " + StringUtil::SaveInt(p);
                                    if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                    {
                                        //texWidth
                                        std::string nameWidth = "Width - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        int width = pTexture->width;
                                        ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                        //texHeight
                                        std::string nameHeight = "Height - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        int height = pTexture->height;
                                        ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                        //texDepth
                                        std::string nameDepth = "Depth - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        int depth = pTexture->depth;
                                        ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                        //indexTextureArray
                                        std::string nameIndexTextureArray = "IndexTextureArray - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        if (pTexture->typeTexture == Vulkan_Texture_2DArray)
                                        {
                                            int count_tex = (int)pTexture->aPathTexture.size();
                                            int indexTextureArray = (int)mat.aTexLayers[p].indexTextureArray;
                                            if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex-1))
                                            {
                                                mat.aTexLayers[p].indexTextureArray = indexTextureArray;
                                            }
                                        }
                                        else 
                                        {
                                            if (pModelObject->nameModel == g_Object_TextureBumpMap) //TextureBumpMap
                                            {
                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].indexTextureArray, 0.5f, 0.0f, 10000.0f))
                                                {
                                                    
                                                }
                                            }
                                            else if (pModelObject->nameModel == g_Object_TextureNormalMap) //TextureNormalMap
                                            {
                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].indexTextureArray, 0.5f, 0.0f, 10000.0f))
                                                {
                                                    
                                                }
                                            }
                                            else
                                            {
                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                {

                                                }
                                            }
                                        }

                                        //texSpeedU
                                        std::string nameTexSpeedU = "TexSpeedU - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                        {
                                            
                                        }
                                        //texSpeedV
                                        std::string nameTexSpeedV = "texSpeedV - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                        {
                                            
                                        }
                                        //texSpeedW
                                        std::string nameTexSpeedW = "texSpeedW - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                        {
                                            
                                        }

                                        //texChunkMaxX
                                        std::string nameTexChunkMaxX = "texChunkMaxX - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        float fTexChunkMaxX = mat.aTexLayers[p].texChunkMaxX;
                                        ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                        //texChunkMaxY
                                        std::string nameTexChunkMaxY = "texChunkMaxY - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        float fTexChunkMaxY = mat.aTexLayers[p].texChunkMaxY;
                                        ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                        //texChunkIndexX
                                        std::string nameTexChunkIndexX = "texChunkIndexX - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        float fTexChunkIndexX = mat.aTexLayers[p].texChunkIndexX;
                                        ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                        //texChunkIndexY
                                        std::string nameTexChunkIndexY = "texChunkIndexY - " + StringUtil::SaveInt(j) + " - " + StringUtil::SaveInt(p);
                                        float fTexChunkIndexY = mat.aTexLayers[p].texChunkIndexY;
                                        ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                    }
                                }
                            
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
        ModelMesh* pMesh = pModelObject->pMesh;

        VkBuffer vertexBuffers[] = { pMesh->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pMesh->poIndexBuffer != nullptr)
        {
            vkCmdBindIndexBuffer(commandBuffer, pMesh->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

        if (pModelObject->isWireFrame)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics_WireFrame);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineGraphics);
            if (pModelObject->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->poPipelineLayout, 0, 1, &pModelObject->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        
    }
}
void Vulkan_011_Texturing::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
{
    if (pModelObject->pMesh->poIndexBuffer != nullptr)
    {
        vkCmdDrawIndexed(commandBuffer, pModelObject->pMesh->poIndexCount, pModelObject->countInstance, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, pModelObject->pMesh->poVertexCount, pModelObject->countInstance, 0, 0);
    }
}

void Vulkan_011_Texturing::cleanupCustom()
{   
    destroyPipelineLayouts();
    destroyDescriptorSetLayouts();
    destroyModelTextures();
    destroyModelMeshes();

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