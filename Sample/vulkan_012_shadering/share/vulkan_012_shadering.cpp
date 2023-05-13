/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_012_shadering.h"
#include "VulkanMeshLoader.h"
#include "VulkanMeshGeometry.h"
#include "VulkanCamera.h"
#include "VulkanTimer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 5;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "geo_triangle",     "Pos3Color4Normal3Tex2",                "geometry",         "triangle",                 "", //geo_triangle

    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/cube/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/sphere.fbx", //sphere
    "bunny",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/bunny/bunny.obj", //bunny

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //geo_triangle


    true, //plane
    false, //cube
    false, //sphere
    false, //bunny

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //geo_triangle

    false, //plane  
    false, //cube
    false, //sphere
    false, //bunny

};
static glm::mat4 g_MeshTranformLocals[g_MeshCount] = 
{
    VulkanMath::ms_mat4Unit, //geo_triangle

    VulkanMath::ms_mat4Unit, //plane
    VulkanMath::ms_mat4Unit, //cube
    VulkanMath::ms_mat4Unit, //sphere
    VulkanMath::ms_mat4Unit, //bunny

};


/////////////////////////// Texture /////////////////////////////
static const std::string g_TextureDefault = "default_blackwhite";
static const int g_TextureCount = 11;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "white",                            "2d",            "false",                  "false",                           "Assets/Texture/white.bmp", //white  
    "default_blackwhite",               "2d",            "false",                  "false",                           "Assets/Texture/default_blackwhite.png", //default_blackwhite
    "bricks_diffuse",                   "2d",            "false",                  "false",                           "Assets/Texture/bricks_diffuse.png", //bricks_diffuse
    "terrain",                          "2d",            "false",                  "false",                           "Assets/Texture/terrain.png", //terrain
    "texture2d",                        "2d",            "false",                  "false",                           "Assets/Texture/texture2d.jpg", //texture2d
    
    "texturecubemap",                   "cubemap",       "false",                  "false",                           "Assets/Texture/texturecubemap_x_right.png;Assets/Texture/texturecubemap_x_left.png;Assets/Texture/texturecubemap_y_up.png;Assets/Texture/texturecubemap_y_down.png;Assets/Texture/texturecubemap_z_front.png;Assets/Texture/texturecubemap_z_back.png", //texturecubemap

    "texture_terrain_diffuse",          "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture_terrain_diffuse
    "texture_terrain_normal",           "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_norm.png;Assets/Texture/Terrain/moss_norm.tga;Assets/Texture/Terrain/rock_cliff_norm.tga;Assets/Texture/Terrain/cliff_norm.png", //texture_terrain_normal
    "texture_terrain_control",          "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/terrain_control.png", //texture_terrain_control

    "texture_rt_compute_copy_tex",      "2d",            "true",                   "true",                            "", //texture_rt_compute_copy_tex
    "texture_rt_compute_copy_texarray", "2d",            "true",                   "true",                            "", //texture_rt_compute_copy_texarray

};
static VkFormat g_TextureFormats[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //white
    VK_FORMAT_R8G8B8A8_SRGB, //default_blackwhite
    VK_FORMAT_R8G8B8A8_SRGB, //bricks_diffuse
    VK_FORMAT_R8G8B8A8_SRGB, //terrain
    VK_FORMAT_R8G8B8A8_SRGB, //texture2d

    VK_FORMAT_R8G8B8A8_SRGB, //texturecubemap

    VK_FORMAT_R8G8B8A8_SRGB, //texture_terrain_diffuse
    VK_FORMAT_R8G8B8A8_UNORM, //texture_terrain_normal
    VK_FORMAT_R8G8B8A8_UNORM, //texture_terrain_control

    VK_FORMAT_R8G8B8A8_UNORM, //texture_rt_compute_copy_tex
    VK_FORMAT_R8G8B8A8_UNORM, //texture_rt_compute_copy_texarray

};
static VulkanTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //white
    Vulkan_TextureFilter_Bilinear, //default_blackwhite
    Vulkan_TextureFilter_Bilinear, //bricks_diffuse
    Vulkan_TextureFilter_Bilinear, //terrain
    Vulkan_TextureFilter_Bilinear, //texture2d

    Vulkan_TextureFilter_Bilinear, //texturecubemap

    Vulkan_TextureFilter_Bilinear, //texture_terrain_diffuse
    Vulkan_TextureFilter_Bilinear, //texture_terrain_normal
    Vulkan_TextureFilter_Bilinear, //texture_terrain_control

    Vulkan_TextureFilter_Bilinear, //texture_rt_compute_copy_tex
    Vulkan_TextureFilter_Bilinear, //texture_rt_compute_copy_texarray

};
static VulkanTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //white
    Vulkan_TextureAddressing_Clamp, //default_blackwhite
    Vulkan_TextureAddressing_Clamp, //bricks_diffuse
    Vulkan_TextureAddressing_Clamp, //terrain
    Vulkan_TextureAddressing_Clamp, //texture2d

    Vulkan_TextureAddressing_Wrap, //texturecubemap

    Vulkan_TextureAddressing_Clamp, //texture_terrain_diffuse
    Vulkan_TextureAddressing_Clamp, //texture_terrain_normal
    Vulkan_TextureAddressing_Clamp, //texture_terrain_control

    Vulkan_TextureAddressing_Clamp, //texture_rt_compute_copy_tex
    Vulkan_TextureAddressing_Clamp, //texture_rt_compute_copy_texarray

};
static VulkanTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //white
    Vulkan_TextureBorderColor_OpaqueBlack, //default_blackwhite
    Vulkan_TextureBorderColor_OpaqueBlack, //bricks_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //terrain
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2d

    Vulkan_TextureBorderColor_OpaqueBlack, //texturecubemap

    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_normal
    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_control

    Vulkan_TextureBorderColor_OpaqueBlack, //texture_rt_compute_copy_tex
    Vulkan_TextureBorderColor_OpaqueBlack, //texture_rt_compute_copy_texarray

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
     64,     64,    1, //white
    512,    512,    1, //default_blackwhite
    512,    512,    1, //bricks_diffuse
    512,    512,    1, //terrain
    512,    512,    1, //texture2d

    512,    512,    1, //texturecubemap

   1024,   1024,    1, //texture_terrain_diffuse
   1024,   1024,    1, //texture_terrain_normal
    512,    512,    1, //texture_terrain_control

   1024,   1024,    1, //texture_rt_compute_copy_tex
   2048,   2048,    1, //texture_rt_compute_copy_texarray

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //white
    0,    0, //default_blackwhite
    0,    0, //bricks_diffuse
    0,    0, //terrain
    0,    0, //texture2d

    0,    0, //texturecubemap

    0,    0, //texture_terrain_diffuse
    0,    0, //texture_terrain_normal
    0,    0, //texture_terrain_control

    0,    0, //texture_rt_compute_copy_tex
    0,    0, //texture_rt_compute_copy_texarray

};


/////////////////////////// DescriptorSetLayout /////////////////
const std::string c_strLayout_Pass = "Pass";
const std::string c_strLayout_Object = "Object";
const std::string c_strLayout_Material = "Material";
const std::string c_strLayout_Instance = "Instance";
const std::string c_strLayout_TextureCopy = "TextureCopy";
const std::string c_strLayout_Tessellation = "Tessellation";
const std::string c_strLayout_Geometry = "Geometry";
const std::string c_strLayout_TextureVS = "TextureVS";
const std::string c_strLayout_TextureTESC = "TextureTESC";
const std::string c_strLayout_TextureTESE = "TextureTESE";
const std::string c_strLayout_TextureFS = "TextureFS";
const std::string c_strLayout_TextureCSR = "TextureCSR";
const std::string c_strLayout_TextureCSRW = "TextureCSRW";
static const int g_DescriptorSetLayoutCount = 7;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS-TextureFS",
    
    "Pass-Object-Material-Instance-TextureFS-Tessellation",

    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-Geometry",

    "TextureCopy-TextureCSR-TextureCSRW",

};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 26;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",              "Assets/Shader/standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert
    
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",        "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_tessellation_lit.vert.spv", //standard_mesh_opaque_tex2d_tessellation_lit vert

    "vert_standard_geometry_normal",                           "vert",              "Assets/Shader/standard_geometry_normal.vert.spv", //standard_geometry_normal vert

    "vert_standard_terrain_opaque_lit",                        "vert",              "Assets/Shader/standard_terrain_opaque_lit.vert.spv", //standard_terrain_opaque_lit vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////
    "tesc_standard_tessellation_passthrough",                  "tesc",              "Assets/Shader/standard_tessellation_passthrough.tesc.spv", //standard_tessellation_passthrough tesc
    "tesc_standard_tessellation_triangle_intger",              "tesc",              "Assets/Shader/standard_tessellation_triangle_intger.tesc.spv", //standard_tessellation_triangle_intger tesc
    "tesc_standard_tessellation_triangle_fractional_even",     "tesc",              "Assets/Shader/standard_tessellation_triangle_fractional_even.tesc.spv", //standard_tessellation_triangle_fractional_even tesc
    "tesc_standard_tessellation_triangle_fractional_odd",      "tesc",              "Assets/Shader/standard_tessellation_triangle_fractional_odd.tesc.spv", //standard_tessellation_triangle_fractional_odd tesc
    "tesc_standard_tessellation_triangle_pow2",                "tesc",              "Assets/Shader/standard_tessellation_triangle_pow2.tesc.spv", //standard_tessellation_triangle_pow2 tesc
    "tesc_standard_tessellation_pntriangles",                  "tesc",              "Assets/Shader/standard_tessellation_pntriangles.tesc.spv", //standard_tessellation_pntriangles tesc
    "tesc_standard_tessellation_terrain",                      "tesc",              "Assets/Shader/standard_tessellation_terrain.tesc.spv", //standard_tessellation_terrain tesc

    ///////////////////////////////////////// tese /////////////////////////////////////////
    "tese_standard_tessellation_passthrough",                  "tese",              "Assets/Shader/standard_tessellation_passthrough.tese.spv", //standard_tessellation_passthrough tese
    "tese_standard_tessellation_triangle",                     "tese",              "Assets/Shader/standard_tessellation_triangle.tese.spv", //standard_tessellation_triangle tese
    "tese_standard_tessellation_pntriangles",                  "tese",              "Assets/Shader/standard_tessellation_pntriangles.tese.spv", //standard_tessellation_pntriangles tese
    "tese_standard_tessellation_terrain",                      "tese",              "Assets/Shader/standard_tessellation_terrain.tese.spv", //standard_tessellation_terrain tese

    ///////////////////////////////////////// geom /////////////////////////////////////////
    "geom_standard_geometry_normal",                           "geom",              "Assets/Shader/standard_geometry_normal.geom.spv", //geom_standard_geometry_normal geom

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag

    "frag_standard_mesh_opaque_tex2d_tessellation_lit",        "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_tessellation_lit.frag.spv", //standard_mesh_opaque_tex2d_tessellation_lit frag

    "frag_standard_geometry_normal",                           "frag",              "Assets/Shader/standard_geometry_normal.frag.spv", //standard_geometry_normal frag

    "frag_standard_terrain_opaque_lit",                        "frag",              "Assets/Shader/standard_terrain_opaque_lit.frag.spv", //standard_terrain_opaque_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    "comp_standard_compute_texcopy_tex2d",                     "comp",              "Assets/Shader/standard_compute_texcopy_tex2d.comp.spv", //standard_compute_texcopy_tex2d comp
    "comp_standard_compute_texcopy_tex2darray",                "comp",              "Assets/Shader/standard_compute_texcopy_tex2darray.comp.spv", //standard_compute_texcopy_tex2darray comp

};


/////////////////////////// Object //////////////////////////////
static const int g_ObjectCount = 15;
static const char* g_ObjectConfigs[5 * g_ObjectCount] = 
{
    //Object Name                               //Mesh Name         //Texture VS            //Texture FS                                                                    //Texture CS
    "textureCubeMap_SkyBox",                    "cube",             "",                     "texturecubemap",                                                               "", //textureCubeMap_SkyBox
    "texture2Darray_TerrainDiffuse",            "plane",            "",                     "texture_terrain_diffuse",                                                      "", //texture2Darray_TerrainDiffuse
    "texture2Darray_TerrainNormal",             "plane",            "",                     "texture_terrain_normal",                                                       "", //texture2Darray_TerrainNormal
    "texture2Darray_TerrainControl",            "plane",            "",                     "texture_terrain_control",                                                      "", //texture2Darray_TerrainControl

    "compute_CopyTexture",                      "plane",            "",                     "texture_rt_compute_copy_tex",                                                  "default_blackwhite;texture_rt_compute_copy_tex", //compute_CopyTexture
    "compute_CopyTextureArray",                 "plane",            "",                     "texture_rt_compute_copy_texarray",                                             "texture_terrain_diffuse;texture_rt_compute_copy_texarray", //compute_CopyTextureArray

    "tessellation_passthrough",                 "plane",            "",                     "bricks_diffuse",                                                               "", //tessellation_passthrough
    "tessellation_triangle_integer",            "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_integer
    "tessellation_triangle_fractional_even",    "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_fractional_even
    "tessellation_triangle_fractional_odd",     "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_fractional_odd
    "tessellation_triangle_pow2",               "geo_triangle",     "",                     "bricks_diffuse",                                                               "", //tessellation_triangle_pow2
    "tessellation_pntriangles",                 "plane",            "",                     "bricks_diffuse",                                                               "", //tessellation_pntriangles

    "geometry_show",                            "bunny",            "",                     "white",                                                                        "", //geometry_show 
    "geometry_normal",                          "bunny",            "",                     "",                                                                             "", //geometry_normal         

    "terrain",                                  "plane",            "",                     "texture_terrain_diffuse;texture_terrain_normal;texture_terrain_control",       "", //terrain

};
static const char* g_ObjectNameShaderModules[6 * g_ObjectCount] = 
{
    //vert                                                  //tesc                                                  //tese                                          //geom                                  //frag                                                  //comp
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_SkyBox
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainDiffuse
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainNormal
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray_TerrainControl

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "comp_standard_compute_texcopy_tex2d", //compute_CopyTexture
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "comp_standard_compute_texcopy_tex2darray", //compute_CopyTextureArray

    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_passthrough",               "tese_standard_tessellation_passthrough",       "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_passthrough
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_triangle_intger",           "tese_standard_tessellation_triangle",          "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_triangle_integer
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_triangle_fractional_even",  "tese_standard_tessellation_triangle",          "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_triangle_fractional_even
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_triangle_fractional_odd",   "tese_standard_tessellation_triangle",          "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_triangle_fractional_odd
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_triangle_pow2",             "tese_standard_tessellation_triangle",          "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_triangle_pow2
    "vert_standard_mesh_opaque_tex2d_tessellation_lit",     "tesc_standard_tessellation_pntriangles",               "tese_standard_tessellation_pntriangles",       "",                                     "frag_standard_mesh_opaque_tex2d_tessellation_lit",     "", //tessellation_pntriangles

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                                     "",                                             "",                                     "frag_standard_mesh_opaque_tex2d_lit",                  "", //geometry_show
    "vert_standard_geometry_normal",                        "",                                                     "",                                             "geom_standard_geometry_normal",        "frag_standard_geometry_normal",                        "", //geometry_normal

    "vert_standard_terrain_opaque_lit",                     "",                                                     "",                                             "",                                     "frag_standard_terrain_opaque_lit",                     "", //terrain
    
};
static const char* g_ObjectNameDescriptorSetLayouts[2 * g_ObjectCount] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureCubeMap_SkyBox
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture2Darray_TerrainDiffuse
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture2Darray_TerrainNormal
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture2Darray_TerrainControl

    "Pass-Object-Material-Instance-TextureFS",                          "TextureCopy-TextureCSR-TextureCSRW", //compute_CopyTexture
    "Pass-Object-Material-Instance-TextureFS",                          "TextureCopy-TextureCSR-TextureCSRW", //compute_CopyTextureArray

    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_passthrough
    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_triangle_integer
    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_triangle_fractional_even
    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_triangle_fractional_odd
    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_triangle_pow2
    "Pass-Object-Material-Instance-TextureFS-Tessellation",             "", //tessellation_pntriangles

    "Pass-Object-Material-Instance-TextureFS",                          "", //geometry_show
    "Pass-Object-Material-Instance-Geometry",                           "", //geometry_normal

    "Pass-Object-Material-Instance-TextureFS-TextureFS-TextureFS",      "", //terrain
};
static float g_instanceGap = 1.5f;
static int g_ObjectInstanceExtCount[g_ObjectCount] =
{
    0, //textureCubeMap_SkyBox
    0, //texture2Darray_TerrainDiffuse 
    0, //texture2Darray_TerrainNormal 
    0, //texture2Darray_TerrainControl 

    0, //compute_CopyTexture 
    0, //compute_CopyTextureArray 

    0, //tessellation_passthrough 
    0, //tessellation_triangle_integer 
    0, //tessellation_triangle_fractional_even 
    0, //tessellation_triangle_fractional_odd 
    0, //tessellation_triangle_pow2 
    0, //tessellation_pntriangles 

    0, //geometry_show 
    0, //geometry_normal 

    0, //terrain 
};
static glm::vec3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    glm::vec3(   0,    0,   0),     glm::vec3(     0,  0,  0),    glm::vec3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    glm::vec3(-2.0,  1.0,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainDiffuse
    glm::vec3(   0,  1.0,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainNormal
    glm::vec3( 2.0,  1.0,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainControl

    glm::vec3(   0,  2.2,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //compute_CopyTexture
    glm::vec3(   0,  3.4,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //compute_CopyTextureArray

    glm::vec3(   0,  4.6,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //tessellation_passthrough
    glm::vec3(-2.0,  5.8,   0),     glm::vec3(     0,  0,  0),    glm::vec3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_integer
    glm::vec3(-1.0,  5.8,   0),     glm::vec3(     0,  0,  0),    glm::vec3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_even
    glm::vec3(   0,  5.8,   0),     glm::vec3(     0,  0,  0),    glm::vec3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_odd
    glm::vec3( 1.0,  5.8,   0),     glm::vec3(     0,  0,  0),    glm::vec3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_pow2
    glm::vec3(   0,  7.0,   0),     glm::vec3(   -90,  0,  0),    glm::vec3( 0.01f,   0.01f,   0.01f), //tessellation_pntriangles

    glm::vec3(   0,   0,  -10),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //geometry_show
    glm::vec3(   0,   0,  -10),     glm::vec3(     0, 180, 0),    glm::vec3( 1.0f,   1.0f,   1.0f), //geometry_normal

    glm::vec3(   0, -0.1,   0),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //terrain

};
static bool g_ObjectIsTransparents[g_ObjectCount] = 
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    false, //tessellation_passthrough
    false, //tessellation_triangle_integer
    false, //tessellation_triangle_fractional_even
    false, //tessellation_triangle_fractional_odd
    false, //tessellation_triangle_pow2
    false, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain
};
static bool g_ObjectIsShows[] = 
{
    true, //textureCubeMap_SkyBox
    true, //texture2Darray_TerrainDiffuse
    true, //texture2Darray_TerrainNormal
    true, //texture2Darray_TerrainControl

    true, //compute_CopyTexture
    true, //compute_CopyTextureArray

    true, //tessellation_passthrough
    true, //tessellation_triangle_integer
    true, //tessellation_triangle_fractional_even
    true, //tessellation_triangle_fractional_odd
    true, //tessellation_triangle_pow2
    true, //tessellation_pntriangles

    true, //geometry_show
    true, //geometry_normal

    true, //terrain
};
static bool g_ObjectIsRotates[g_ObjectCount] =
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    false, //tessellation_passthrough
    false, //tessellation_triangle_integer
    false, //tessellation_triangle_fractional_even
    false, //tessellation_triangle_fractional_odd
    false, //tessellation_triangle_pow2
    false, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain
};
static bool g_ObjectIsLightings[g_ObjectCount] =
{
    true, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    false, //tessellation_passthrough
    false, //tessellation_triangle_integer
    false, //tessellation_triangle_fractional_even
    false, //tessellation_triangle_fractional_odd
    false, //tessellation_triangle_pow2
    false, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    true, //terrain
};
static bool g_ObjectIsTopologyPatchLists[g_ObjectCount] =
{
    false, //textureCubeMap_SkyBox
    false, //texture2Darray_TerrainDiffuse
    false, //texture2Darray_TerrainNormal
    false, //texture2Darray_TerrainControl

    false, //compute_CopyTexture
    false, //compute_CopyTextureArray

    true, //tessellation_passthrough
    true, //tessellation_triangle_integer
    true, //tessellation_triangle_fractional_even
    true, //tessellation_triangle_fractional_odd
    true, //tessellation_triangle_pow2
    true, //tessellation_pntriangles

    false, //geometry_show
    false, //geometry_normal

    false, //terrain
};


/////////////////////////// ModelMesh ///////////////////////////
bool Vulkan_012_Shadering::ModelMesh::LoadMesh(bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load
    MeshData meshData;
    meshData.bIsFlipY = isFlipY;
    if (this->typeMesh == Vulkan_Mesh_File)
    {
        unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
        if (!VulkanMeshLoader::LoadMeshData(this->pathMesh, meshData, eMeshParserFlags))
        {
            Util_LogError("Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh failed: [%s] !", this->pathMesh.c_str());
            return false; 
        }
    }
    else if (this->typeMesh == Vulkan_Mesh_Geometry)
    {
        if (!VulkanMeshGeometry::CreateGeometry(meshData, this->typeGeometryType))
        {
            Util_LogError("Vulkan_012_Shadering::ModelMesh::LoadMesh: create geometry mesh failed: typeGeometry: [%s] !", Util_GetMeshGeometryTypeName(this->typeGeometryType).c_str());
            return false; 
        }
    }
    else
    {
        assert(false && "Vulkan_012_Shadering::ModelMesh::LoadMesh: Wrong typeMesh !");
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
                v.pos = VulkanMath::Transform(matTransformLocal, v.pos);
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

        Util_LogInfo("Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
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
                v.pos = VulkanMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tangent3Tex2.push_back(v);
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

        Util_LogInfo("Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
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
void Vulkan_012_Shadering::ModelTexture::UpdateTexture()
{
    if (this->typeTexture == Vulkan_Texture_3D)
    {
        updateNoiseTexture();
    }
}
void Vulkan_012_Shadering::ModelTexture::updateNoiseTextureData()
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
void Vulkan_012_Shadering::ModelTexture::updateNoiseTexture()
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


/////////////////////////// ModelObject /////////////////////////



Vulkan_012_Shadering::Vulkan_012_Shadering(int width, int height, std::string name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isUseComputeShader = true;
    this->cfg_isCreateRenderComputeSycSemaphore = true;

    this->cfg_cameraPos = glm::vec3(-2.5f, 2.0f, -20.0f);
    this->cfg_cameraLookTarget = glm::vec3(-2.5f, 5.0f, 0.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = glm::vec3(0, -1, 0); //y-
}

void Vulkan_012_Shadering::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    //Tessellation Enable
    if (this->poPhysicalDeviceFeatures.tessellationShader)
    {
        this->poPhysicalEnabledFeatures.tessellationShader = VK_TRUE;
    }
    else
    {
        Util_LogError("Vulkan_012_Shadering::setUpEnabledFeatures: tessellationShader is not supported !");
    }

}

void Vulkan_012_Shadering::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_012_Shadering::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
}
void Vulkan_012_Shadering::cameraReset()
{
    VulkanWindow::cameraReset();
}

void Vulkan_012_Shadering::loadModel_Custom()
{
    createModelMeshes();
    createModelTextures();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);

        pModelObject->indexModel = i;
        pModelObject->nameObject = g_ObjectConfigs[5 * i + 0]; //Object Name     
        pModelObject->nameMesh = g_ObjectConfigs[5 * i + 1]; //Mesh Name

        //Mesh
        {
            ModelMesh* pMesh = this->findModelMesh(pModelObject->nameMesh);
            assert(pMesh != nullptr && "Vulkan_012_Shadering::loadModel_Custom");
            pModelObject->SetMesh(pMesh);
        }

        //Texture VS
        {
            std::string nameTextureVS = g_ObjectConfigs[5 * i + 2]; //Texture VS
            if (!nameTextureVS.empty())
            {
                std::vector<std::string> aTextureVS = VulkanUtilString::Split(nameTextureVS, ";");
                size_t count_tex = aTextureVS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureVS[j];
                    ModelTexture* pTextureVS = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), pTextureVS);
                }
            }
        }
        //Texture FS
        {
            std::string nameTextureFS = g_ObjectConfigs[5 * i + 3]; //Texture FS
            if (!nameTextureFS.empty())
            {
                std::vector<std::string> aTextureFS = VulkanUtilString::Split(nameTextureFS, ";");
                size_t count_tex = aTextureFS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureFS[j];
                    ModelTexture* pTextureFS = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureFS);
                }
            }
        }
        //Texture CS
        {
            std::string nameTextureCS = g_ObjectConfigs[5 * i + 4]; //Texture CS
            if (!nameTextureCS.empty())
            {
                std::vector<std::string> aTextureCS = VulkanUtilString::Split(nameTextureCS, ";");
                size_t count_tex = aTextureCS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureCS[j];
                    ModelTexture* pTextureCS = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), pTextureCS);
                }
            }
        }

        //Tessellation
        std::string nameShaderTesc = g_ObjectNameShaderModules[6 * i + 1];
        std::string nameShaderTese = g_ObjectNameShaderModules[6 * i + 2];
        if (!nameShaderTesc.empty() || !nameShaderTese.empty())
        {
            pModelObject->isUsedTessellation = true;
            if (g_ObjectIsTopologyPatchLists[i])
            {
                pModelObject->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
            }
        }

        //Geometry
        std::string nameShaderGeom = g_ObjectNameShaderModules[6 * i + 3];
        if (!nameShaderGeom.empty())
        {
            pModelObject->isUsedGeometry = true;
        }

        //Pipeline Graphics - DescriptorSetLayout
        pModelObject->pPipelineGraphics->nameDescriptorSetLayout = g_ObjectNameDescriptorSetLayouts[2 * i + 0];

        //Pipeline Computes - DescriptorSetLayout
        std::string nameDescriptorSetLayout = g_ObjectNameDescriptorSetLayouts[2 * i + 1];
        if (!nameDescriptorSetLayout.empty())
        {
            std::vector<std::string> aDescriptorSetLayout = VulkanUtilString::Split(nameDescriptorSetLayout, ";");
            size_t count_dsl = aDescriptorSetLayout.size();
            for (size_t j = 0; j < count_dsl; j++)
            {
                const std::string& nameDescriptorSetLayout = aDescriptorSetLayout[j];
                PipelineCompute* pPipelineCompute = new PipelineCompute(this);
                pPipelineCompute->nameDescriptorSetLayout = nameDescriptorSetLayout;
                pModelObject->AddPipelineCompute(pPipelineCompute);
            }
        }

        //Common
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
        m_mapModelObjects[pModelObject->nameObject] = pModelObject;
    }
}

void Vulkan_012_Shadering::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_012_Shadering::rebuildInstanceCBs(bool isCreateVkBuffer)
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
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = VulkanMath::FromTRS(g_ObjectTranforms[i * 3 + 0] + glm::vec3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                                 g_ObjectTranforms[i * 3 + 1],
                                                                 g_ObjectTranforms[i * 3 + 2]);
                pModelObject->objectCBs.push_back(objectConstants);
                pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = VulkanMath::RandomColor(false);
                materialConstants.factorDiffuse = VulkanMath::RandomColor(false);
                materialConstants.factorSpecular = VulkanMath::RandomColor(false);
                materialConstants.shininess = VulkanMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = VulkanMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = g_ObjectIsLightings[i];
                //Texture VS
                {
                    ModelTexturePtrVector* pTextureVSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    ModelTexturePtrVector* pTextureFSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    ModelTexturePtrVector* pTextureCSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
                    if (pTextureCSs != nullptr)
                    {

                    }
                }
                pModelObject->materialCBs.push_back(materialConstants);
            }

            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 1.0f;
                tessellationConstants.tessLevelInner = 1.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pModelObject->tessellationCBs.push_back(tessellationConstants);
            }

            //GeometryConstants
            if (pModelObject->isUsedGeometry)
            {
                GeometryConstants geometryConstants;
                geometryConstants.width = 0.05f;
                geometryConstants.height = 3.0f;
                geometryConstants.length = 0.02f;
                pModelObject->geometryCBs.push_back(geometryConstants);
            }
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

            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pModelObject->poBuffers_tessellationCB.resize(count_sci);
                pModelObject->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_tessellationCB[j], pModelObject->poBuffersMemory_tessellationCB[j]);
                }
            }

            //GeometryConstants
            if (pModelObject->isUsedGeometry)
            {
                bufferSize = sizeof(GeometryConstants) * MAX_OBJECT_COUNT;
                pModelObject->poBuffers_geometryCB.resize(count_sci);
                pModelObject->poBuffersMemory_geometryCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_geometryCB[j], pModelObject->poBuffersMemory_geometryCB[j]);
                }
            }
        }
    }
}

void Vulkan_012_Shadering::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();
}   
void Vulkan_012_Shadering::createGraphicsPipeline_Custom()
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

        //[1] Shaders
        std::string nameShaderVert = g_ObjectNameShaderModules[6 * i + 0];
        std::string nameShaderTesc = g_ObjectNameShaderModules[6 * i + 1];
        std::string nameShaderTese = g_ObjectNameShaderModules[6 * i + 2];
        std::string nameShaderGeom = g_ObjectNameShaderModules[6 * i + 3];
        std::string nameShaderFrag = g_ObjectNameShaderModules[6 * i + 4];
        if (!createPipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  pModelObject->aShaderStageCreateInfos_Graphics))
        {
            std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            pModelObject->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline(pModelObject->aShaderStageCreateInfos_Graphics,
                                                                                             pModelObject->isUsedTessellation, 0, 3,
                                                                                             Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                             Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                             this->poRenderPass, pModelObject->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                             pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits,
                                                                                             pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                                             pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                             pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                             pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                             pModelObject->cfg_ColorWriteMask);
            if (pModelObject->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_012_Shadering::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pModelObject->nameObject.c_str());

            //pPipelineGraphics->poPipeline
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
            pModelObject->pPipelineGraphics->poPipeline = createVkGraphicsPipeline(pModelObject->aShaderStageCreateInfos_Graphics,
                                                                                   pModelObject->isUsedTessellation, 0, 3,
                                                                                   Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex), 
                                                                                   Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                   this->poRenderPass, pModelObject->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                   pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE,
                                                                                   isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                                                   pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                   isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                   pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                   pModelObject->cfg_ColorWriteMask);
            if (pModelObject->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createGraphicsPipeline_Custom: Failed to create pipeline graphics !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_012_Shadering::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pModelObject->nameObject.c_str());
        }
    }
}
void Vulkan_012_Shadering::createComputePipeline_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        size_t count_pipeline = pModelObject->aPipelineComputes.size();
        if (count_pipeline <= 0)
            continue;

        //[1] Shaders
        std::string nameShaderComp = g_ObjectNameShaderModules[6 * i + 5];
        if (!createPipelineShaderStageCreateInfos(nameShaderComp,
                                                  pModelObject->aShaderStageCreateInfos_Computes,
                                                  pModelObject->mapShaderStageCreateInfos_Computes))
        {
            std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pModelObject->aShaderStageCreateInfos_Computes.size())
        {
            std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            PipelineCompute* p = pModelObject->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pModelObject->aShaderStageCreateInfos_Computes[j];

            p->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayoutNames == nullptr)
            {
                std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline(shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_012_Shadering::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}

void Vulkan_012_Shadering::destroyModelMeshes()
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
void Vulkan_012_Shadering::createModelMeshes()
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
            std::string msg = "Vulkan_012_Shadering::createModelMeshes: create mesh: [" + nameMesh + "] failed !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        Util_LogInfo("Vulkan_012_Shadering::createModelMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                     nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
Vulkan_012_Shadering::ModelMesh* Vulkan_012_Shadering::findModelMesh(const std::string& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_012_Shadering::destroyModelTextures()
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
void Vulkan_012_Shadering::createModelTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        std::string nameTexture = g_TexturePaths[5 * i + 0];
        std::string nameType = g_TexturePaths[5 * i + 1];
        VulkanTextureType typeTexture = Util_ParseTextureType(nameType);
        std::string nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = VulkanUtilString::ParserBool(nameIsRenderTarget);
        std::string nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = VulkanUtilString::ParserBool(nameIsGraphicsComputeShared);
        std::string pathTextures = g_TexturePaths[5 * i + 4];

        std::vector<std::string> aPathTexture = VulkanUtilString::Split(pathTextures, ";");
        ModelTexture* pTexture = new ModelTexture(this, 
                                                  nameTexture,
                                                  typeTexture,
                                                  isRenderTarget,
                                                  isGraphicsComputeShared,
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
            pTexture->texChunkIndex = VulkanMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
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

        Util_LogInfo("Vulkan_012_Shadering::createModelTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                     nameTexture.c_str(), 
                     nameType.c_str(), 
                     isRenderTarget ? "true" : "false",
                     pathTextures.c_str());
    }
}
Vulkan_012_Shadering::ModelTexture* Vulkan_012_Shadering::findModelTexture(const std::string& nameTexture)
{
    ModelTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_012_Shadering::destroyDescriptorSetLayouts()
{
    size_t count = this->m_aVkDescriptorSetLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkDescriptorSetLayout(this->m_aVkDescriptorSetLayouts[i]);
    }
    this->m_aVkDescriptorSetLayouts.clear();
    this->m_mapVkDescriptorSetLayout.clear();
    this->m_mapName2Layouts.clear();
}
void Vulkan_012_Shadering::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        std::string nameLayout(g_DescriptorSetLayoutNames[i]);
        std::vector<std::string> aLayouts = VulkanUtilString::Split(nameLayout, "-");
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
                passMainLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(passMainLayoutBinding);
            }
            else if (strLayout == c_strLayout_Object) //Object
            {
                VkDescriptorSetLayoutBinding objectLayoutBinding = {};
                objectLayoutBinding.binding = j;
                objectLayoutBinding.descriptorCount = 1;
                objectLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                objectLayoutBinding.pImmutableSamplers = nullptr;
                objectLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

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
            else if (strLayout == c_strLayout_TextureCopy) //TextureCopy
            {
                VkDescriptorSetLayoutBinding textureCopyLayoutBinding = {};
                textureCopyLayoutBinding.binding = j;
                textureCopyLayoutBinding.descriptorCount = 1;
                textureCopyLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                textureCopyLayoutBinding.pImmutableSamplers = nullptr;
                textureCopyLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

                bindings.push_back(textureCopyLayoutBinding);
            }
            else if (strLayout == c_strLayout_Tessellation) //Tessellation
            {
                VkDescriptorSetLayoutBinding tessellationLayoutBinding = {};
                tessellationLayoutBinding.binding = j;
                tessellationLayoutBinding.descriptorCount = 1;
                tessellationLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                tessellationLayoutBinding.pImmutableSamplers = nullptr;
                tessellationLayoutBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

                bindings.push_back(tessellationLayoutBinding);
            }
            else if (strLayout == c_strLayout_Geometry) //Geometry
            {
                VkDescriptorSetLayoutBinding geometryLayoutBinding = {};
                geometryLayoutBinding.binding = j;
                geometryLayoutBinding.descriptorCount = 1;
                geometryLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                geometryLayoutBinding.pImmutableSamplers = nullptr;
                geometryLayoutBinding.stageFlags = VK_SHADER_STAGE_GEOMETRY_BIT;

                bindings.push_back(geometryLayoutBinding);
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
            else if (strLayout == c_strLayout_TextureTESC) //TextureTESC
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

                bindings.push_back(samplerLayoutBinding);
            }
            else if (strLayout == c_strLayout_TextureTESE) //TextureTESE
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

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
            else if (strLayout == c_strLayout_TextureCSR) //TextureCSR
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

                bindings.push_back(samplerLayoutBinding);
            }
            else if (strLayout == c_strLayout_TextureCSRW) //TextureCSRW
            {
                VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                samplerLayoutBinding.binding = j;
                samplerLayoutBinding.descriptorCount = 1;
                samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                samplerLayoutBinding.pImmutableSamplers = nullptr;
                samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

                bindings.push_back(samplerLayoutBinding);
            }
            else
            {
                std::string msg = "Vulkan_012_Shadering::createDescriptorSetLayouts: Wrong DescriptorSetLayout type: " + strLayout;
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

        Util_LogInfo("Vulkan_012_Shadering::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_012_Shadering::findDescriptorSetLayout(const std::string& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
std::vector<std::string>* Vulkan_012_Shadering::findDescriptorSetLayoutNames(const std::string& nameDescriptorSetLayout)
{
    std::map<std::string, std::vector<std::string>>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
}


void Vulkan_012_Shadering::destroyShaderModules()
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
void Vulkan_012_Shadering::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        std::string shaderName = g_ShaderModulePaths[3 * i + 0];
        std::string shaderType = g_ShaderModulePaths[3 * i + 1];
        std::string shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createShaderModule(shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        Util_LogInfo("Vulkan_012_Shadering::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                     shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_012_Shadering::findShaderModule(const std::string& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   
bool Vulkan_012_Shadering::createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
                                                                const std::string& nameShaderTesc,
                                                                const std::string& nameShaderTese,
                                                                const std::string& nameShaderGeom,
                                                                const std::string& nameShaderFrag,
                                                                const std::string& nameShaderComp,
                                                                VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics,
                                                                VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                                VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute)
{
    if (!createPipelineShaderStageCreateInfos(nameShaderVert,
                                              nameShaderTesc,
                                              nameShaderTese,
                                              nameShaderGeom,
                                              nameShaderFrag,
                                              aStageCreateInfos_Graphics))
    {
        return false;
    }

    if (!createPipelineShaderStageCreateInfos(nameShaderComp,
                                              aStageCreateInfos_Compute,
                                              mapStageCreateInfos_Compute))
    {
        return false;
    }

    return true;
}
bool Vulkan_012_Shadering::createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
                                                                const std::string& nameShaderTesc,
                                                                const std::string& nameShaderTese,
                                                                const std::string& nameShaderGeom,
                                                                const std::string& nameShaderFrag,
                                                                VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics)
{
    //vert
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderVert);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find vert shader module: [%s] !", nameShaderVert.c_str());
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        aStageCreateInfos_Graphics.push_back(shaderStageInfo);
    }
    //tesc
    if (!nameShaderTesc.empty())
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderTesc);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find tesc shader module: [%s] !", nameShaderTesc.c_str());
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        aStageCreateInfos_Graphics.push_back(shaderStageInfo);
    }
    //tese
    if (!nameShaderTese.empty())
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderTese);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find tese shader module: [%s] !", nameShaderTese.c_str());
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        aStageCreateInfos_Graphics.push_back(shaderStageInfo);
    }
    //geom
    if (!nameShaderGeom.empty())
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderGeom);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find geom shader module: [%s] !", nameShaderGeom.c_str());
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        aStageCreateInfos_Graphics.push_back(shaderStageInfo);
    }
    //frag
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderFrag);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find frag shader module: [%s] !", nameShaderFrag.c_str());
            return false;
        }

        VkPipelineShaderStageCreateInfo shaderStageInfo = {};
        shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        shaderStageInfo.module = shaderModule;
        shaderStageInfo.pName = "main";
        aStageCreateInfos_Graphics.push_back(shaderStageInfo);
    }

    return true;
}
bool Vulkan_012_Shadering::createPipelineShaderStageCreateInfos(const std::string& nameShaderComp,
                                                                VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                                VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute)
{
    //comp
    if (!nameShaderComp.empty())
    {
        std::vector<std::string> aShaderComps = VulkanUtilString::Split(nameShaderComp, ";");
        int count_comp = (int)aShaderComps.size();
        for (int i = 0; i < count_comp; i++)
        {
            std::string nameSC = aShaderComps[i];
            VkShaderModule shaderModule = findShaderModule(nameSC);
            if (shaderModule == VK_NULL_HANDLE)
            {
                Util_LogError("Vulkan_012_Shadering::createPipelineShaderStageCreateInfos: Can not find comp shader module: [%s] !", nameSC.c_str());
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
            shaderStageInfo.module = shaderModule;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Compute.push_back(shaderStageInfo);
            mapStageCreateInfos_Compute[nameSC] = shaderStageInfo;
        }
    }

    return true;
}


void Vulkan_012_Shadering::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkPipelineLayout(this->m_aVkPipelineLayouts[i]);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_012_Shadering::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        std::string nameDescriptorSetLayout(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_012_Shadering::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_012_Shadering::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_012_Shadering::findPipelineLayout(const std::string& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}



void Vulkan_012_Shadering::createDescriptorSets_Custom()
{
    size_t count_sci = this->poSwapChainImages.size();
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        //Pipeline Graphics
        {
            std::vector<std::string>* pDescriptorSetLayoutNames = pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames;
            assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_012_Shadering::createDescriptorSets_Custom");
            createDescriptorSets(pModelObject->pPipelineGraphics->poDescriptorSets, pModelObject->pPipelineGraphics->poDescriptorSetLayout);
            for (size_t j = 0; j < count_sci; j++)
            {   
                std::vector<VkWriteDescriptorSet> descriptorWrites;
                int nIndexTextureVS = 0;
                int nIndexTextureTESC = 0;
                int nIndexTextureTESE = 0;
                int nIndexTextureFS = 0;

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

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Pass;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_Object) //Object
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Object;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_Material) //Material
                    {
                        VkDescriptorBufferInfo bufferInfo_Material = {};
                        bufferInfo_Material.buffer = pModelObject->poBuffers_materialCB[j];
                        bufferInfo_Material.offset = 0;
                        bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Material;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_Instance) //Instance
                    {
                        VkDescriptorBufferInfo bufferInfo_Instance = {};
                        bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
                        bufferInfo_Instance.offset = 0;
                        bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Instance;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_Tessellation) //Tessellation
                    {
                        VkDescriptorBufferInfo bufferInfo_Tessellation = {};
                        bufferInfo_Tessellation.buffer = pModelObject->poBuffers_tessellationCB[j];
                        bufferInfo_Tessellation.offset = 0;
                        bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Tessellation;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_Geometry) //Geometry
                    {
                        VkDescriptorBufferInfo bufferInfo_Geometry = {};
                        bufferInfo_Geometry.buffer = pModelObject->poBuffers_geometryCB[j];
                        bufferInfo_Geometry.offset = 0;
                        bufferInfo_Geometry.range = sizeof(GeometryConstants) * MAX_OBJECT_COUNT;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                        ds.descriptorCount = 1;
                        ds.pBufferInfo = &bufferInfo_Geometry;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_TextureVS) //TextureVS
                    {
                        ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), nIndexTextureVS);
                        nIndexTextureVS ++;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        ds.descriptorCount = 1;
                        ds.pImageInfo = &pTexture->poTextureImageInfo;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_TextureTESC)//TextureTESC
                    {
                        ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), nIndexTextureTESC);
                        nIndexTextureTESC ++;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        ds.descriptorCount = 1;
                        ds.pImageInfo = &pTexture->poTextureImageInfo;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_TextureTESE)//TextureTESE
                    {
                        ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), nIndexTextureTESE);
                        nIndexTextureTESE ++;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        ds.descriptorCount = 1;
                        ds.pImageInfo = &pTexture->poTextureImageInfo;
                        descriptorWrites.push_back(ds);
                    }
                    else if (nameDescriptorSet == c_strLayout_TextureFS) //TextureFS
                    {
                        ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), nIndexTextureFS);
                        nIndexTextureFS ++;

                        VkWriteDescriptorSet ds = {};
                        ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                        ds.dstSet = pModelObject->pPipelineGraphics->poDescriptorSets[j];
                        ds.dstBinding = p;
                        ds.dstArrayElement = 0;
                        ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                        ds.descriptorCount = 1;
                        ds.pImageInfo = &pTexture->poTextureImageInfo;
                        descriptorWrites.push_back(ds);
                    }
                    else
                    {
                        std::string msg = "Vulkan_012_Shadering::createDescriptorSets_Custom: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        Util_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
                vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
            }
        }   
        
        //Pipeline Computes
        size_t count_comp = pModelObject->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {       
            PipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];

            std::vector<std::string>* pDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
            assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_012_Shadering::createDescriptorSets_Custom");
            createDescriptorSet(pPipelineCompute->poDescriptorSet, pPipelineCompute->poDescriptorSetLayout);

            std::vector<VkWriteDescriptorSet> descriptorWrites;
            int nIndexTextureCS = 0;
            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t p = 0; p < count_names; p++)
            {
                std::string& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
                if (nameDescriptorSet == c_strLayout_TextureCopy) //TextureCopy
                {
                    pPipelineCompute->CreateTextureCopy();

                    VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
                    bufferInfo_TextureCopy.buffer = pPipelineCompute->poBuffer_TextureCopy;
                    bufferInfo_TextureCopy.offset = 0;
                    bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);

                    VkWriteDescriptorSet ds = {};
                    ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds.dstSet = pPipelineCompute->poDescriptorSet;
                    ds.dstBinding = p;
                    ds.dstArrayElement = 0;
                    ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    ds.descriptorCount = 1;
                    ds.pBufferInfo = &bufferInfo_TextureCopy;
                    descriptorWrites.push_back(ds);
                }   
                else if (nameDescriptorSet == c_strLayout_TextureCSR) //TextureCSR
                {
                    ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
                    nIndexTextureCS ++;
                    pPipelineCompute->pTextureSource = pTexture;

                    VkWriteDescriptorSet ds = {};
                    ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds.dstSet = pPipelineCompute->poDescriptorSet;
                    ds.dstBinding = p;
                    ds.dstArrayElement = 0;
                    ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    ds.descriptorCount = 1;
                    ds.pImageInfo = &pTexture->poTextureImageInfo;
                    descriptorWrites.push_back(ds);
                }
                else if (nameDescriptorSet == c_strLayout_TextureCSRW) //TextureCSRW
                {
                    ModelTexture* pTexture = pModelObject->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
                    nIndexTextureCS ++;
                    pPipelineCompute->pTextureTarget = pTexture;

                    VkWriteDescriptorSet ds = {};
                    ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    ds.dstSet = pPipelineCompute->poDescriptorSet;
                    ds.dstBinding = p;
                    ds.dstArrayElement = 0;
                    ds.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                    ds.descriptorCount = 1;
                    ds.pImageInfo = &pTexture->poTextureImageInfo;
                    descriptorWrites.push_back(ds);
                }
                else
                {
                    std::string msg = "Vulkan_012_Shadering::createDescriptorSets_Custom: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }  
            vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }
}

void Vulkan_012_Shadering::updateCompute_Custom(VkCommandBuffer& commandBuffer)
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        size_t count_comp = pModelObject->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            PipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];
            if (pPipelineCompute->pTextureSource != nullptr &&
                pPipelineCompute->pTextureTarget != nullptr &&
                pPipelineCompute->pTextureCopy != nullptr)
            {
                bool isRand = false;
                if (++pPipelineCompute->frameRand > 30)
                {
                    isRand = true;
                    pPipelineCompute->frameRand = 0;
                }

                pPipelineCompute->pTextureCopy->texInfo.x = pPipelineCompute->pTextureSource->width;
                pPipelineCompute->pTextureCopy->texInfo.y = pPipelineCompute->pTextureSource->height;
                pPipelineCompute->pTextureCopy->texInfo.z = 0;
                pPipelineCompute->pTextureCopy->texInfo.w = 0;
                if (isRand)
                {
                    pPipelineCompute->pTextureCopy->texOffset.x = VulkanMath::Rand(0, 1) * pPipelineCompute->pTextureSource->width;
                    pPipelineCompute->pTextureCopy->texOffset.y = VulkanMath::Rand(0, 1) * pPipelineCompute->pTextureSource->height;
                    pPipelineCompute->pTextureCopy->texOffset.z = 0;
                    pPipelineCompute->pTextureCopy->texOffset.w = 0;

                    int seed = VulkanMath::Rand(0, 10000);
                    int start = seed % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.x = start;
                    pPipelineCompute->pTextureCopy->texIndexArray.y = ++start % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.z = ++start % 4;
                    pPipelineCompute->pTextureCopy->texIndexArray.w = ++start % 4;
                }
                pPipelineCompute->pTextureCopy->texClearColor.x = 0;
                pPipelineCompute->pTextureCopy->texClearColor.y = 0;
                pPipelineCompute->pTextureCopy->texClearColor.z = 0;
                pPipelineCompute->pTextureCopy->texClearColor.w = 1;

                VkDeviceMemory& memory = pPipelineCompute->poBufferMemory_TextureCopy;
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(TextureCopyConstants), 0, &data);
                    memcpy(data, pPipelineCompute->pTextureCopy, sizeof(TextureCopyConstants));
                vkUnmapMemory(this->poDevice, memory);

                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pPipelineCompute->poPipeline);
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, pPipelineCompute->poPipelineLayout, 0, 1, &pPipelineCompute->poDescriptorSet, 0, 0);
                
                uint32_t groupX = (uint32_t)(pPipelineCompute->pTextureTarget->width / 8);
                uint32_t groupY = (uint32_t)(pPipelineCompute->pTextureTarget->height / 8);
                dispatch(commandBuffer, groupX, groupY, 1);
            }
        }
    }
}

void Vulkan_012_Shadering::updateCBs_Custom()
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
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pModelObject->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pModelObject->materialCBs[j];
            
            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pModelObject->tessellationCBs[j];
            }
            
            //GeometryConstants
            if (pModelObject->isUsedGeometry)
            {
                GeometryConstants& geometryCB = pModelObject->geometryCBs[j];
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

        //TessellationConstants
        if (pModelObject->isUsedTessellation)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(TessellationConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->tessellationCBs.data(), sizeof(TessellationConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }

        //GeometryConstants
        if (pModelObject->isUsedGeometry)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_geometryCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(GeometryConstants) * count_object, 0, &data);
                memcpy(data, pModelObject->geometryCBs.data(), sizeof(GeometryConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
    }
}

void Vulkan_012_Shadering::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        size_t count_comp = pModelObject->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            PipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];
            if (pPipelineCompute->pTextureSource != nullptr &&
                pPipelineCompute->pTextureTarget != nullptr &&
                pPipelineCompute->pTextureCopy != nullptr)
            {
                VkImageMemoryBarrier imageMemoryBarrier = {};
                imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
                imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_GENERAL;
                imageMemoryBarrier.image = pPipelineCompute->pTextureTarget->poTextureImage;
                imageMemoryBarrier.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
                imageMemoryBarrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
                imageMemoryBarrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
                imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

                vkCmdPipelineBarrier(commandBuffer,
                                     VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                                     VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
                                     0,
                                     0,
                                     nullptr,
                                     0,
                                     nullptr,
                                     1,
                                     &imageMemoryBarrier);
            }
        }
    }
}

bool Vulkan_012_Shadering::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_012_Shadering", &windowOpened, 0);
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
void Vulkan_012_Shadering::modelConfig()
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

            std::string nameObject = VulkanUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameObject.c_str()))
            {
                std::string nameIsShow = "Is Show - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                std::string nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                std::string nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                std::string nameIsTransparent = "Is Transparent - " + pModelObject->nameObject;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                std::string nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < pModelObject->countInstance; j++)
                    {
                        MaterialConstants& mat = pModelObject->materialCBs[j];
                        mat.lighting = pModelObject->isLighting;
                    }
                }
                
                std::string nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 10);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->pMesh->poVertexCount, (int)pModelObject->pMesh->poIndexCount);
                
                std::string nameWorld = "Model Object - " + pModelObject->nameObject;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        std::string nameModelInstance = nameObject + " - " + VulkanUtilString::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            std::string nameObject = VulkanUtilString::SaveInt(j) + " - Object - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
                                const glm::mat4& mat4World = obj.g_MatWorld;
                                std::string nameTable = VulkanUtilString::SaveInt(j) + " - matWorld - " + pModelObject->nameObject;
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
                            std::string nameMaterial = VulkanUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                            {
                                //factorAmbient
                                std::string nameFactorAmbient = "FactorAmbient - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                {

                                }
                                ImGui::Spacing();

                                //factorDiffuse
                                std::string nameFactorDiffuse = "FactorDiffuse - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                {

                                }
                                ImGui::Spacing();

                                //factorSpecular
                                std::string nameFactorSpecular = "FactorSpecular - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                {

                                }
                                ImGui::Spacing();

                                //shininess
                                std::string nameShininess = "Shininess - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //alpha
                                std::string nameAlpha = "Alpha - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //lighting
                                std::string nameLighting = "Lighting - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                bool isLighting = mat.lighting == 1.0f ? true : false;
                                if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                {
                                    mat.lighting = isLighting ? 1.0f : 0.0f;
                                }

                                //Texture VS
                                {
                                    ModelTexturePtrVector* pTextureVSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                                    if (pTextureVSs != nullptr)
                                    {

                                    }
                                }
                                //Texture FS
                                {
                                    ModelTexturePtrVector* pTextureFSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                                    if (pTextureFSs != nullptr)
                                    {
                                        size_t count_texture = pTextureFSs->size();
                                        for (size_t p = 0; p < count_texture; p++)
                                        {
                                            ModelTexture* pTexture = (*pTextureFSs)[p];

                                            std::string nameMaterial_Texture = VulkanUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject + " - TextureFS - " + VulkanUtilString::SaveInt(p);
                                            if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                            {
                                                //texWidth
                                                std::string nameWidth = "Width - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int width = pTexture->width;
                                                ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                //texHeight
                                                std::string nameHeight = "Height - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int height = pTexture->height;
                                                ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                //texDepth
                                                std::string nameDepth = "Depth - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int depth = pTexture->depth;
                                                ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                //indexTextureArray
                                                std::string nameIndexTextureArray = "IndexTextureArray - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
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
                                                    if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                    {

                                                    }
                                                }

                                                //texSpeedU
                                                std::string nameTexSpeedU = "TexSpeedU - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }
                                                //texSpeedV
                                                std::string nameTexSpeedV = "texSpeedV - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }
                                                //texSpeedW
                                                std::string nameTexSpeedW = "texSpeedW - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }

                                                //texChunkMaxX
                                                std::string nameTexChunkMaxX = "texChunkMaxX - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxX = mat.aTexLayers[p].texChunkMaxX;
                                                ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                //texChunkMaxY
                                                std::string nameTexChunkMaxY = "texChunkMaxY - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxY = mat.aTexLayers[p].texChunkMaxY;
                                                ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                //texChunkIndexX
                                                std::string nameTexChunkIndexX = "texChunkIndexX - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexX = mat.aTexLayers[p].texChunkIndexX;
                                                ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                //texChunkIndexY
                                                std::string nameTexChunkIndexY = "texChunkIndexY - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexY = mat.aTexLayers[p].texChunkIndexY;
                                                ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                            }
                                        }
                                    }
                                }
                                //Texture CS
                                {
                                    ModelTexturePtrVector* pTextureCSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
                                    if (pTextureCSs != nullptr)
                                    {

                                    }
                                }

                                ImGui::Spacing();
                            }

                            //TessellationConstants
                            if (pModelObject->isUsedTessellation)
                            {
                                std::string nameTessellation = VulkanUtilString::SaveInt(j) + " - Tessellation - " + pModelObject->nameObject;
                                if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                {
                                    TessellationConstants& tess = pModelObject->tessellationCBs[j];
                                    //tessLevelOuter
                                    std::string nameTessLevelOuter = "tessLevelOuter - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                    {
                                        
                                    }
                                    //tessLevelInner
                                    std::string nameTessLevelInner = "tessLevelInner - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                    {
                                        
                                    }
                                    //tessAlpha
                                    std::string nameTessAlpha = "tessAlpha - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                    {
                                        
                                    }
                                    //tessStrength
                                    std::string nameTessStrength = "tessStrength - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
                                    {
                                        
                                    }
                                    
                                    ImGui::Spacing();
                                }
                            }
                           
                            //Geometry
                            if (pModelObject->isUsedGeometry)
                            {
                                std::string nameGeometry = VulkanUtilString::SaveInt(j) + " - Geometry - " + pModelObject->nameObject;
                                if (ImGui::CollapsingHeader(nameGeometry.c_str()))
                                {
                                    GeometryConstants& geometry = pModelObject->geometryCBs[j];
                                    //width
                                    std::string nameGeometryWidth = "width - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameGeometryWidth.c_str(), &geometry.width, 0.001f, 0.001f, 10.0f))
                                    {
                                        
                                    }
                                    //height
                                    std::string nameGeometryHeight = "height - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameGeometryHeight.c_str(), &geometry.height, 0.001f, 0.001f, 10.0f))
                                    {
                                        
                                    }
                                    //length
                                    std::string nameGeometryLength = "length - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameGeometryLength.c_str(), &geometry.length, 0.001f, 0.001f, 10.0f))
                                    {
                                        
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
}

void Vulkan_012_Shadering::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_012_Shadering::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
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
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pMesh->poIndexBuffer != nullptr)
        {
            bindIndexBuffer(commandBuffer, pMesh->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

        if (pModelObject->isWireFrame || this->cfg_isWireFrame)
        {
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipeline_WireFrame);
            if (pModelObject->pPipelineGraphics->poDescriptorSets.size() > 0)
            {
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipelineLayout, 0, 1, &pModelObject->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        else
        {
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipeline);
            if (pModelObject->pPipelineGraphics->poDescriptorSets.size() > 0)
            {
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipelineLayout, 0, 1, &pModelObject->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        
    }
}
void Vulkan_012_Shadering::drawModelObject(VkCommandBuffer& commandBuffer, ModelObject* pModelObject)
{
    if (pModelObject->pMesh->poIndexBuffer != nullptr)
    {
        drawIndexed(commandBuffer, pModelObject->pMesh->poIndexCount, pModelObject->countInstance, 0, 0, 0);
    }
    else
    {
        draw(commandBuffer, pModelObject->pMesh->poVertexCount, pModelObject->countInstance, 0, 0);
    }
}

void Vulkan_012_Shadering::cleanupCustom()
{   
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

void Vulkan_012_Shadering::cleanupSwapChain_Custom()
{
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->CleanupSwapChain();
    }

    destroyDescriptorSetLayouts();
    destroyPipelineLayouts();
    destroyShaderModules();
}

void Vulkan_012_Shadering::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}