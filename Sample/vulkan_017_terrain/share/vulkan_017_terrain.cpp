/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_017_terrain.h"
#include "VulkanMeshLoader.h"
#include "VulkanMeshGeometry.h"
#include "VulkanCamera.h"
#include "VulkanTimer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 10;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/cube/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/sphere.fbx", //sphere

    "mountain",         "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Obj/mountain/mountain.obj", //mountain

    "rock",             "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Fbx/rock/rock.fbx", //rock
    "cliff",            "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Obj/cliff/cliff.obj", //cliff

    "tree",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/tree/tree.fbx", //tree
    "tree_spruce",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/tree_spruce/tree_spruce.fbx", //tree_spruce

    "grass",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/grass/grass.fbx", //grass
    "flower",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/flower/flower.fbx", //flower

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane
    false, //cube
    false, //sphere

    false, //mountain

    false, //rock
    false, //cliff

    false, //tree
    false, //tree_spruce

    false, //grass
    false, //flower

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  
    false, //cube
    false, //sphere

    false, //mountain

    false, //rock
    false, //cliff

    false, //tree
    false, //tree_spruce

    false, //grass
    false, //flower

};
static glm::mat4 g_MeshTranformLocals[g_MeshCount] = 
{
    VulkanMath::ms_mat4Unit, //plane
    VulkanMath::ms_mat4Unit, //cube
    VulkanMath::ms_mat4Unit, //sphere

    VulkanMath::ms_mat4Unit, //mountain

    VulkanMath::ms_mat4Unit, //rock
    VulkanMath::ms_mat4Unit, //cliff

    VulkanMath::ms_mat4Unit, //tree
    VulkanMath::ms_mat4Unit, //tree_spruce

    VulkanMath::ms_mat4Unit, //grass
    VulkanMath::ms_mat4Unit, //flower

};


/////////////////////////// Texture /////////////////////////////
static const String g_TextureDefault = "default_blackwhite";
static const int g_TextureCount = 24;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "default_blackwhite",               "2d",            "false",                  "false",                           "Assets/Texture/default_blackwhite.png", //default_blackwhite
    "bricks_diffuse",                   "2d",            "false",                  "false",                           "Assets/Texture/bricks_diffuse.png", //bricks_diffuse
    "terrain",                          "2d",            "false",                  "false",                           "Assets/Texture/terrain.png", //terrain
    "texture2d",                        "2d",            "false",                  "false",                           "Assets/Texture/texture2d.jpg", //texture2d
    
    "texturecubemap",                   "cubemap",       "false",                  "false",                           "Assets/Texture/texturecubemap_x_right.png;Assets/Texture/texturecubemap_x_left.png;Assets/Texture/texturecubemap_y_up.png;Assets/Texture/texturecubemap_y_down.png;Assets/Texture/texturecubemap_z_front.png;Assets/Texture/texturecubemap_z_back.png", //texturecubemap

    "texture_terrain_diffuse",          "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture_terrain_diffuse
    "texture_terrain_normal",           "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_norm.png;Assets/Texture/Terrain/moss_norm.tga;Assets/Texture/Terrain/rock_cliff_norm.tga;Assets/Texture/Terrain/cliff_norm.png", //texture_terrain_normal
    "texture_terrain_control",          "2darray",       "false",                  "false",                           "Assets/Texture/Terrain/terrain_control.png", //texture_terrain_control

    "mountain_diffuse",                 "2d",            "false",                  "false",                           "Assets/Model/Obj/mountain/mountain_diffuse.png", //mountain_diffuse
    "mountain_normal",                  "2d",            "false",                  "false",                           "Assets/Model/Obj/mountain/mountain_normal.png", //mountain_normal

    "rock_diffuse",                     "2d",            "false",                  "false",                           "Assets/Model/Fbx/rock/rock_diffuse.png", //rock_diffuse
    "rock_normal",                      "2d",            "false",                  "false",                           "Assets/Model/Fbx/rock/rock_normal.png", //rock_normal
    "cliff_diffuse",                    "2d",            "false",                  "false",                           "Assets/Model/Obj/cliff/cliff_diffuse.png", //cliff_diffuse
    "cliff_normal",                     "2d",            "false",                  "false",                           "Assets/Model/Obj/cliff/cliff_normal.png", //cliff_normal

    "tree_diffuse",                     "2d",            "false",                  "false",                           "Assets/Model/Fbx/tree/tree_diffuse.png", //tree_diffuse
    "tree_spruce_diffuse",              "2d",            "false",                  "false",                           "Assets/Model/Fbx/tree_spruce/tree_spruce_diffuse.png", //tree_spruce_diffuse

    "grass_alien",                      "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_alien.png", //grass_alien
    "grass_field",                      "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_field.png", //grass_field
    "grass_pixelated",                  "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_pixelated.png", //grass_pixelated
    "grass_tall",                       "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_tall.png", //grass_tall
    "grass_thick",                      "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_thick.png", //grass_thick
    "grass_thin",                       "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_thin.png", //grass_thin
    "grass_wheat",                      "2d",            "false",                  "false",                           "Assets/Model/Fbx/grass/grass_wheat.png", //grass_wheat
    
    "flower_atlas",                     "2d",            "false",                  "false",                           "Assets/Model/Fbx/flower/flower_atlas.png", //flower_atlas

};
static VkFormat g_TextureFormats[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //default_blackwhite
    VK_FORMAT_R8G8B8A8_SRGB, //bricks_diffuse
    VK_FORMAT_R8G8B8A8_SRGB, //terrain
    VK_FORMAT_R8G8B8A8_SRGB, //texture2d

    VK_FORMAT_R8G8B8A8_SRGB, //texturecubemap

    VK_FORMAT_R8G8B8A8_SRGB, //texture_terrain_diffuse
    VK_FORMAT_R8G8B8A8_UNORM, //texture_terrain_normal
    VK_FORMAT_R8G8B8A8_UNORM, //texture_terrain_control

    VK_FORMAT_R8G8B8A8_SRGB, //mountain_diffuse
    VK_FORMAT_R8G8B8A8_UNORM, //mountain_normal

    VK_FORMAT_R8G8B8A8_SRGB, //rock_diffuse
    VK_FORMAT_R8G8B8A8_UNORM, //rock_normal
    VK_FORMAT_R8G8B8A8_SRGB, //cliff_diffuse
    VK_FORMAT_R8G8B8A8_UNORM, //cliff_normal

    VK_FORMAT_R8G8B8A8_SRGB, //tree_diffuse
    VK_FORMAT_R8G8B8A8_SRGB, //tree_spruce_diffuse

    VK_FORMAT_R8G8B8A8_SRGB, //grass_alien
    VK_FORMAT_R8G8B8A8_SRGB, //grass_field
    VK_FORMAT_R8G8B8A8_SRGB, //grass_pixelated
    VK_FORMAT_R8G8B8A8_SRGB, //grass_tall
    VK_FORMAT_R8G8B8A8_SRGB, //grass_thick
    VK_FORMAT_R8G8B8A8_SRGB, //grass_thin
    VK_FORMAT_R8G8B8A8_SRGB, //grass_wheat

    VK_FORMAT_R8G8B8A8_SRGB, //flower_atlas

};
static VulkanTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //default_blackwhite
    Vulkan_TextureFilter_Bilinear, //bricks_diffuse
    Vulkan_TextureFilter_Bilinear, //terrain
    Vulkan_TextureFilter_Bilinear, //texture2d

    Vulkan_TextureFilter_Bilinear, //texturecubemap

    Vulkan_TextureFilter_Bilinear, //texture_terrain_diffuse
    Vulkan_TextureFilter_Bilinear, //texture_terrain_normal
    Vulkan_TextureFilter_Bilinear, //texture_terrain_control

    Vulkan_TextureFilter_Bilinear, //mountain_diffuse
    Vulkan_TextureFilter_Bilinear, //mountain_normal

    Vulkan_TextureFilter_Bilinear, //rock_diffuse
    Vulkan_TextureFilter_Bilinear, //rock_normal
    Vulkan_TextureFilter_Bilinear, //cliff_diffuse
    Vulkan_TextureFilter_Bilinear, //cliff_normal

    Vulkan_TextureFilter_Bilinear, //tree_diffuse
    Vulkan_TextureFilter_Bilinear, //tree_spruce_diffuse

    Vulkan_TextureFilter_Bilinear, //grass_alien
    Vulkan_TextureFilter_Bilinear, //grass_field
    Vulkan_TextureFilter_Bilinear, //grass_pixelated
    Vulkan_TextureFilter_Bilinear, //grass_tall
    Vulkan_TextureFilter_Bilinear, //grass_thick
    Vulkan_TextureFilter_Bilinear, //grass_thin
    Vulkan_TextureFilter_Bilinear, //grass_wheat

    Vulkan_TextureFilter_Bilinear, //flower_atlas

};
static VulkanTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //default_blackwhite
    Vulkan_TextureAddressing_Clamp, //bricks_diffuse
    Vulkan_TextureAddressing_Clamp, //terrain
    Vulkan_TextureAddressing_Clamp, //texture2d

    Vulkan_TextureAddressing_Wrap, //texturecubemap

    Vulkan_TextureAddressing_Clamp, //texture_terrain_diffuse
    Vulkan_TextureAddressing_Clamp, //texture_terrain_normal
    Vulkan_TextureAddressing_Clamp, //texture_terrain_control

    Vulkan_TextureAddressing_Clamp, //mountain_diffuse
    Vulkan_TextureAddressing_Clamp, //mountain_normal

    Vulkan_TextureAddressing_Clamp, //rock_diffuse
    Vulkan_TextureAddressing_Clamp, //rock_normal
    Vulkan_TextureAddressing_Clamp, //cliff_diffuse
    Vulkan_TextureAddressing_Clamp, //cliff_normal

    Vulkan_TextureAddressing_Clamp, //tree_diffuse
    Vulkan_TextureAddressing_Clamp, //tree_spruce_diffuse

    Vulkan_TextureAddressing_Clamp, //grass_alien
    Vulkan_TextureAddressing_Clamp, //grass_field
    Vulkan_TextureAddressing_Clamp, //grass_pixelated
    Vulkan_TextureAddressing_Clamp, //grass_tall
    Vulkan_TextureAddressing_Clamp, //grass_thick
    Vulkan_TextureAddressing_Clamp, //grass_thin
    Vulkan_TextureAddressing_Clamp, //grass_wheat

    Vulkan_TextureAddressing_Clamp, //flower_atlas

};
static VulkanTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //default_blackwhite
    Vulkan_TextureBorderColor_OpaqueBlack, //bricks_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //terrain
    Vulkan_TextureBorderColor_OpaqueBlack, //texture2d

    Vulkan_TextureBorderColor_OpaqueBlack, //texturecubemap

    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_normal
    Vulkan_TextureBorderColor_OpaqueBlack, //texture_terrain_control

    Vulkan_TextureBorderColor_OpaqueBlack, //mountain_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //mountain_normal

    Vulkan_TextureBorderColor_OpaqueBlack, //rock_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //rock_normal
    Vulkan_TextureBorderColor_OpaqueBlack, //cliff_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //cliff_normal

    Vulkan_TextureBorderColor_OpaqueBlack, //tree_diffuse
    Vulkan_TextureBorderColor_OpaqueBlack, //tree_spruce_diffuse

    Vulkan_TextureBorderColor_OpaqueBlack, //grass_alien
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_field
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_pixelated
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_tall
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_thick
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_thin
    Vulkan_TextureBorderColor_OpaqueBlack, //grass_wheat

    Vulkan_TextureBorderColor_OpaqueBlack, //flower_atlas

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //default_blackwhite
    512,    512,    1, //bricks_diffuse
    512,    512,    1, //terrain
    512,    512,    1, //texture2d

    512,    512,    1, //texturecubemap

   1024,   1024,    1, //texture_terrain_diffuse
   1024,   1024,    1, //texture_terrain_normal
    512,    512,    1, //texture_terrain_control

   1024,   1024,    1, //mountain_diffuse
   1024,   1024,    1, //mountain_normal

    512,    512,    1, //rock_diffuse
    512,    512,    1, //rock_normal
    512,    512,    1, //cliff_diffuse
   1024,   1024,    1, //cliff_normal

   1024,   1024,    1, //tree_diffuse
   1024,   1024,    1, //tree_spruce_diffuse

   1024,   1024,    1, //grass_alien
   1024,   1024,    1, //grass_field
    128,    128,    1, //grass_pixelated
   1024,   1024,    1, //grass_tall
   1024,   1024,    1, //grass_thick
   1024,   1024,    1, //grass_thin
    128,    512,    1, //grass_wheat

   1024,   1024,    1, //flower_atlas

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default_blackwhite
    0,    0, //bricks_diffuse
    0,    0, //terrain
    0,    0, //texture2d

    0,    0, //texturecubemap

    0,    0, //texture_terrain_diffuse
    0,    0, //texture_terrain_normal
    0,    0, //texture_terrain_control

    0,    0, //mountain_diffuse
    0,    0, //mountain_normal

    0,    0, //rock_diffuse
    0,    0, //rock_normal
    0,    0, //cliff_diffuse
    0,    0, //cliff_normal

    0,    0, //tree_diffuse
    0,    0, //tree_spruce_diffuse

    0,    0, //grass_alien
    0,    0, //grass_field
    0,    0, //grass_pixelated
    0,    0, //grass_tall
    0,    0, //grass_thick
    0,    0, //grass_thin
    0,    0, //grass_wheat

    0,    0, //flower_atlas

};


/////////////////////////// DescriptorSetLayout /////////////////
const String c_strLayout_Pass = "Pass";
const String c_strLayout_Object = "Object";
const String c_strLayout_Material = "Material";
const String c_strLayout_Instance = "Instance";
const String c_strLayout_TextureCopy = "TextureCopy";
const String c_strLayout_Tessellation = "Tessellation";
const String c_strLayout_TextureVS = "TextureVS";
const String c_strLayout_TextureTESC = "TextureTESC";
const String c_strLayout_TextureTESE = "TextureTESE";
const String c_strLayout_TextureFS = "TextureFS";
const String c_strLayout_TextureCSR = "TextureCSR";
const String c_strLayout_TextureCSRW = "TextureCSRW";
static const int g_DescriptorSetLayoutCount = 3;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS-TextureFS",

};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 18;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_transparent_lit",                      "vert",              "Assets/Shader/standard_mesh_transparent_lit.vert.spv", //standard_mesh_transparent_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",              "Assets/Shader/standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert
    
    "vert_standard_terrain_opaque_lit",                        "vert",              "Assets/Shader/standard_terrain_opaque_lit.vert.spv", //standard_terrain_opaque_lit vert

    "vert_standard_mesh_opaque_normalmap_lit",                 "vert",              "Assets/Shader/standard_mesh_opaque_normalmap_lit.vert.spv", //standard_mesh_opaque_normalmap_lit vert
    "vert_standard_mesh_transparent_tree_lit",                 "vert",              "Assets/Shader/standard_mesh_transparent_tree_lit.vert.spv", //standard_mesh_transparent_tree_lit vert  
    "vert_standard_mesh_opaque_tree_alphatest_lit",            "vert",              "Assets/Shader/standard_mesh_opaque_tree_alphatest_lit.vert.spv", //standard_mesh_opaque_tree_alphatest_lit vert
    "vert_standard_mesh_opaque_grass_alphatest_lit",           "vert",              "Assets/Shader/standard_mesh_opaque_grass_alphatest_lit.vert.spv", //standard_mesh_opaque_grass_alphatest_lit vert  

    ///////////////////////////////////////// tesc /////////////////////////////////////////
   

    ///////////////////////////////////////// tese /////////////////////////////////////////
   

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_transparent_lit",                      "frag",              "Assets/Shader/standard_mesh_transparent_lit.frag.spv", //standard_mesh_transparent_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag

    "frag_standard_terrain_opaque_lit",                        "frag",              "Assets/Shader/standard_terrain_opaque_lit.frag.spv", //standard_terrain_opaque_lit frag

    "frag_standard_mesh_opaque_normalmap_lit",                 "frag",              "Assets/Shader/standard_mesh_opaque_normalmap_lit.frag.spv", //standard_mesh_opaque_normalmap_lit frag
    "frag_standard_mesh_transparent_tree_lit",                 "frag",              "Assets/Shader/standard_mesh_transparent_tree_lit.frag.spv", //standard_mesh_transparent_tree_lit frag
    "frag_standard_mesh_opaque_tree_alphatest_lit",            "frag",              "Assets/Shader/standard_mesh_opaque_tree_alphatest_lit.frag.spv", //standard_mesh_opaque_tree_alphatest_lit frag
    "frag_standard_mesh_opaque_grass_alphatest_lit",           "frag",              "Assets/Shader/standard_mesh_opaque_grass_alphatest_lit.frag.spv", //standard_mesh_opaque_grass_alphatest_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    

};


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 8;
static const char* g_Object_Configs[2 * g_Object_Count] = 
{
    //Object Name                          //Mesh Name                                                                    
    "object_skybox",                       "cube", //object_skybox
    "object_mountain",                     "mountain", //object_mountain   

    "object_rock",                         "rock", //object_rock   
    "object_cliff",                        "cliff", //object_cliff   

    "object_tree",                         "tree", //object_tree        
    "object_tree_spruce",                  "tree_spruce", //object_tree_spruce
    
    "object_grass",                        "grass", //object_grass        
    "object_flower",                       "flower", //object_flower

};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{
    "0", //object_skybox
    "0", //object_mountain

    "0", //object_rock
    "0", //object_cliff

    "0;1", //object_tree        
    "0;1", //object_tree_spruce

    "1;4;6;9", //object_grass
    "0;2;4;6;8;9;10;11", //object_flower

};  

static float g_Object_InstanceGap = 3.0f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    0, //object_skybox
    0, //object_mountain 

    4, //object_rock 
    4, //object_cliff 

    4, //object_tree 
    4, //object_tree_spruce 

    4, //object_grass 
    4, //object_flower 

};
static bool g_Object_IsShows[] = 
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    false, //object_flower

};
static bool g_Object_IsLightings[g_Object_Count] =
{
    true, //object_skybox
    true, //object_mountain

    true, //object_rock
    true, //object_cliff

    true, //object_tree
    true, //object_tree_spruce

    true, //object_grass
    true, //object_flower

};
static bool g_Object_IsIndirectDraw[g_Object_Count] =
{
    false, //object_skybox
    false, //object_mountain

    false, //object_rock
    false, //object_cliff

    false, //object_tree
    false, //object_tree_spruce

    false, //object_grass
    true, //object_flower

};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 20;
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_skybox-1",                     "",                     "",                              "",                         "",                    "texturecubemap",                                                               "", //object_skybox-1
    "object_mountain-1",                   "",                     "",                              "",                         "",                    "mountain_diffuse;mountain_normal",                                             "", //object_mountain-1

    "object_rock-1",                       "",                     "",                              "",                         "",                    "rock_diffuse;rock_normal",                                                     "", //object_rock-1
    "object_cliff-1",                      "",                     "",                              "",                         "",                    "cliff_diffuse;cliff_normal",                                                   "", //object_cliff-1

    "object_tree-1",                       "",                     "",                              "",                         "",                    "tree_diffuse",                                                                 "", //object_tree-1
    "object_tree-2",                       "",                     "",                              "",                         "",                    "tree_diffuse",                                                                 "", //object_tree-2
    "object_tree_spruce-1",                "",                     "",                              "",                         "",                    "tree_spruce_diffuse",                                                          "", //object_tree_spruce-1
    "object_tree_spruce-2",                "",                     "",                              "",                         "",                    "tree_spruce_diffuse",                                                          "", //object_tree_spruce-2

    "object_grass-1",                      "",                     "",                              "",                         "",                    "grass_field",                                                                  "", //object_grass-1
    "object_grass-2",                      "",                     "",                              "",                         "",                    "grass_wheat",                                                                  "", //object_grass-2
    "object_grass-3",                      "",                     "",                              "",                         "",                    "grass_tall",                                                                   "", //object_grass-3
    "object_grass-4",                      "",                     "",                              "",                         "",                    "grass_field",                                                                  "", //object_grass-4
    "object_flower-1",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-1
    "object_flower-2",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-2
    "object_flower-3",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-3
    "object_flower-4",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-4
    "object_flower-5",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-5
    "object_flower-6",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-6
    "object_flower-7",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-7
    "object_flower-8",                     "",                     "",                              "",                         "",                    "flower_atlas",                                                                 "", //object_flower-8

};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                         "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //object_skybox-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_mountain-1
    
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_rock-1
    "vert_standard_mesh_opaque_normalmap_lit",              "",                                             "",                                         "",                         "frag_standard_mesh_opaque_normalmap_lit",              "", //object_cliff-1

    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree-2
    "vert_standard_mesh_opaque_tree_alphatest_lit",         "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tree_alphatest_lit",         "", //object_tree_spruce-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_tree_spruce-2

    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-1
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-2
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-3
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_grass-4
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-1
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-2
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-3
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-4
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-5
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-6
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-7
    "vert_standard_mesh_opaque_grass_alphatest_lit",        "",                                             "",                                         "",                         "frag_standard_mesh_opaque_grass_alphatest_lit",        "", //object_flower-8

};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_skybox-1
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_mountain-1

    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_rock-1
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //object_cliff-1

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree_spruce-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_tree_spruce-2

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-3
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass-4
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-2
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-3
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-4
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-5
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-6
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-7
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_flower-8

};
static glm::vec3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    glm::vec3(   0,  0.0,   0.0),    glm::vec3(     0,  0,  0),    glm::vec3(  500.0f,    500.0f,    500.0f), //object_skybox-1
    glm::vec3(   0,  0.0,   0.0),    glm::vec3(     0,  0,  0),    glm::vec3(    1.0f,      1.0f,      1.0f), //object_mountain-1
 
    glm::vec3(   0,  0.0,   1.5),    glm::vec3(     0,  0,  0),    glm::vec3(   10.0f,     10.0f,     10.0f), //object_rock-1
    glm::vec3(   0,  0.0,   0.0),    glm::vec3(     0,  0,  0),    glm::vec3(    0.1f,      0.1f,      0.1f), //object_cliff-1

    glm::vec3(   0,  0.0, -10.0),    glm::vec3(     0,  0,  0),    glm::vec3(   10.0f,     10.0f,     10.0f), //object_tree-1
    glm::vec3(   0,  0.0, -10.0),    glm::vec3(     0,  0,  0),    glm::vec3(   10.0f,     10.0f,     10.0f), //object_tree-2
    glm::vec3(   0,  0.0,  10.0),    glm::vec3(     0,  0,  0),    glm::vec3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-1
    glm::vec3(   0,  0.0,  10.0),    glm::vec3(     0,  0,  0),    glm::vec3(   10.0f,     10.0f,     10.0f), //object_tree_spruce-2

    glm::vec3(   0,  0.0,   2.0),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_grass-1
    glm::vec3(   0,  0.0,   2.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_grass-2
    glm::vec3(   0,  0.0,   5.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_grass-3
    glm::vec3(   0,  0.0,   5.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_grass-4
    glm::vec3(   0,  0.0,  -1.0),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-1
    glm::vec3(   0,  0.0,  -1.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-2
    glm::vec3(   0,  0.0,  -2.0),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-3
    glm::vec3(   0,  0.0,  -2.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-4
    glm::vec3(   0,  0.0,  -3.0),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-5
    glm::vec3(   0,  0.0,  -3.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-6
    glm::vec3(   0,  0.0,  -4.0),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-7
    glm::vec3(   0,  0.0,  -4.5),    glm::vec3(     0,  0,  0),    glm::vec3(   50.0f,     50.0f,     50.0f), //object_flower-8

};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_skybox-1
    false, //object_mountain-1

    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4
    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};
static bool g_ObjectRend_IsTopologyPatchLists[g_ObjectRend_Count] =
{
    false, //object_skybox-1
    false, //object_mountain-1
    
    false, //object_rock-1
    false, //object_cliff-1

    false, //object_tree-1
    false, //object_tree-2
    false, //object_tree_spruce-1
    false, //object_tree_spruce-2

    false, //object_grass-1
    false, //object_grass-2
    false, //object_grass-3
    false, //object_grass-4
    false, //object_flower-1
    false, //object_flower-2
    false, //object_flower-3
    false, //object_flower-4
    false, //object_flower-5
    false, //object_flower-6
    false, //object_flower-7
    false, //object_flower-8

};





/////////////////////////// ModelMeshSub ////////////////////////
void Vulkan_017_Terrain::ModelMeshSub::Destroy()
{
    //Vertex
    this->pMesh->pWindow->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
    this->poVertexBuffer = VK_NULL_HANDLE;
    this->poVertexBufferMemory = VK_NULL_HANDLE;

    //Index
    this->pMesh->pWindow->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
    this->poIndexBuffer = VK_NULL_HANDLE;
    this->poIndexBufferMemory = VK_NULL_HANDLE;
}
bool Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub(MeshData& meshData, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
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

        Util_LogInfo("Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMeshSub.c_str(),
                     this->nameOriginal.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tex2.size(), 
                     (int)this->indices.size());
    }
    else if (this->poTypeVertex == Vulkan_Vertex_Pos3Color4Normal3Tex4)
    {
        this->vertices_Pos3Color4Normal3Tex4.clear();
        this->vertices_Pos3Color4Normal3Tex4.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            MeshVertex& vertex = meshData.vertices[i];
            Vertex_Pos3Color4Normal3Tex4 v;
            v.pos = vertex.pos;
            v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.texCoord = glm::vec4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
            if (isTranformLocal)
            {
                v.pos = VulkanMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tex4.push_back(v);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tex4.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tex4);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex4[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        Util_LogInfo("Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tex4]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMeshSub.c_str(),
                     this->nameOriginal.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tex4.size(), 
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

        Util_LogInfo("Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMeshSub.c_str(),
                     this->nameOriginal.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tangent3Tex2.size(), 
                     (int)this->indices.size());
    }
    else if (this->poTypeVertex == Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex4)
    {
        this->vertices_Pos3Color4Normal3Tangent3Tex4.clear();
        this->vertices_Pos3Color4Normal3Tangent3Tex4.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            MeshVertex& vertex = meshData.vertices[i];
            Vertex_Pos3Color4Normal3Tangent3Tex4 v;
            v.pos = vertex.pos;
            v.color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = glm::vec4(vertex.texCoord.x, vertex.texCoord.y, 0, 0);
            if (isTranformLocal)
            {
                v.pos = VulkanMath::Transform(matTransformLocal, v.pos);
            }
            this->vertices_Pos3Color4Normal3Tangent3Tex4.push_back(v);
        }

        int count_index = (int)meshData.indices32.size();
        this->indices.clear();
        this->indices.reserve(count_index);
        for (int i = 0; i < count_index; i++)
        {
            this->indices.push_back(meshData.indices32[i]);
        }
        this->poVertexCount = (uint32_t)this->vertices_Pos3Color4Normal3Tangent3Tex4.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tangent3Tex4);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex4[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        Util_LogInfo("Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub: create mesh sub: [%s] - [%s] success, [Pos3Color4Normal3Tangent3Tex4]: Vertex count: [%d], Index count: [%d] !", 
                     this->nameMeshSub.c_str(),
                     this->nameOriginal.c_str(),
                     (int)this->vertices_Pos3Color4Normal3Tangent3Tex4.size(), 
                     (int)this->indices.size());
    }
    else
    {
        Util_LogError("Vulkan_017_Terrain::ModelMeshSub::CreateMeshSub: create mesh sub failed: [%s], wrong poTypeVertex !", this->nameMeshSub.c_str());
        return false; 
    }

    //2> createVertexBuffer
    this->pMesh->pWindow->createVertexBuffer(this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pMesh->pWindow->createIndexBuffer(this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }

    return true;
}

void Vulkan_017_Terrain::ModelMeshSub::WriteVertexData(std::vector<Vertex_Pos3Color4Normal3Tex2>& aPos3Color4Normal3Tex2,
                                                            std::vector<Vertex_Pos3Color4Normal3Tangent3Tex2>& aPos3Color4Normal3Tangent3Tex2)
{
    size_t count = 0;
    if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
    {
        count = this->vertices_Pos3Color4Normal3Tex2.size();
        for (size_t i = 0; i < count; i++)
        {
            Vertex_Pos3Color4Normal3Tex2& vSrc = this->vertices_Pos3Color4Normal3Tex2[i];
            aPos3Color4Normal3Tex2.push_back(vSrc);
        }
    }
    else if (this->vertices_Pos3Color4Normal3Tex4.size() > 0)
    {
        count = this->vertices_Pos3Color4Normal3Tex4.size();
        for (size_t i = 0; i < count; i++)
        {
            Vertex_Pos3Color4Normal3Tex4& vSrc = this->vertices_Pos3Color4Normal3Tex4[i];
            aPos3Color4Normal3Tex2.push_back(Vertex_Pos3Color4Normal3Tex2(vSrc.pos,
                                             vSrc.color,
                                             vSrc.normal,
                                             glm::vec2(vSrc.texCoord.x, vSrc.texCoord.y)));
        }
    }   
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
    {
        count = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        for (size_t i = 0; i < count; i++)
        {
            Vertex_Pos3Color4Normal3Tangent3Tex2& vSrc = this->vertices_Pos3Color4Normal3Tangent3Tex2[i];
            aPos3Color4Normal3Tangent3Tex2.push_back(vSrc);
        }
    }   
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex4.size() > 0)
    {
        count = this->vertices_Pos3Color4Normal3Tangent3Tex4.size();
        for (size_t i = 0; i < count; i++)
        {
            Vertex_Pos3Color4Normal3Tangent3Tex4& vSrc = this->vertices_Pos3Color4Normal3Tangent3Tex4[i];
            aPos3Color4Normal3Tangent3Tex2.push_back(Vertex_Pos3Color4Normal3Tangent3Tex2(vSrc.pos,
                                                     vSrc.color,
                                                     vSrc.normal,
                                                     vSrc.tangent,
                                                     glm::vec2(vSrc.texCoord.x, vSrc.texCoord.y)));
        }
    }
}

void Vulkan_017_Terrain::ModelMeshSub::WriteIndexData(std::vector<uint32_t>& indexData)
{
    indexData.insert(indexData.end(), indices.begin(), indices.end());
}

/////////////////////////// ModelMesh ///////////////////////////
bool Vulkan_017_Terrain::ModelMesh::AddMeshSub(ModelMeshSub* pMeshSub)
{
    ModelMeshSubPtrMap::iterator itFind = this->mapMeshSubs.find(pMeshSub->nameMeshSub);
    if (itFind != this->mapMeshSubs.end())
    {
        Util_LogError("Vulkan_017_Terrain::ModelMesh::AddMeshSub: Mesh sub is exist: [%s] !", pMeshSub->nameMeshSub.c_str());
        return false;
    }

    this->aMeshSubs.push_back(pMeshSub);
    this->mapMeshSubs[pMeshSub->nameMeshSub] = pMeshSub;
    return true;
}

bool Vulkan_017_Terrain::ModelMesh::LoadMesh(bool isFlipY, bool isTranformLocal, const glm::mat4& matTransformLocal)
{
    //1> Load
    std::vector<MeshData> aMeshDatas;
    if (this->typeMesh == Vulkan_Mesh_File)
    {
        unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
        if (!VulkanMeshLoader::LoadMeshDatas(this->pathMesh, aMeshDatas, isFlipY, eMeshParserFlags))
        {
            Util_LogError("Vulkan_017_Terrain::ModelMesh::LoadMesh: load meshes failed: [%s] !", this->pathMesh.c_str());
            return false; 
        }
    }
    else if (this->typeMesh == Vulkan_Mesh_Geometry)
    {
        MeshData meshData;
        meshData.bIsFlipY = isFlipY;
        if (!VulkanMeshGeometry::CreateGeometry(meshData, this->typeGeometryType))
        {
            Util_LogError("Vulkan_017_Terrain::ModelMesh::LoadMesh: create geometry mesh failed: typeGeometry: [%s] !", Util_GetMeshGeometryTypeName(this->typeGeometryType).c_str());
            return false; 
        }
        aMeshDatas.push_back(meshData);
    }
    else
    {
        assert(false && "Vulkan_017_Terrain::ModelMesh::LoadMesh: Wrong typeMesh !");
        return false;
    }

    int count_mesh_sub = (int)aMeshDatas.size();
    for (int i = 0; i < count_mesh_sub; i++)
    {
        MeshData& meshData = aMeshDatas[i];
        
        String nameMeshSub = this->nameMesh + "-" + VulkanUtilString::SaveInt(i);
        ModelMeshSub* pMeshSub = new ModelMeshSub(this,
                                                  nameMeshSub,
                                                  meshData.nameMesh,
                                                  i,
                                                  this->typeVertex);
        if (!pMeshSub->CreateMeshSub(meshData, isTranformLocal, matTransformLocal))
        {
            Util_LogError("Vulkan_017_Terrain::ModelMesh::LoadMesh: Create mesh sub failed: [%s] !", nameMeshSub.c_str());
            return false;
        }
        AddMeshSub(pMeshSub);
    }

    return true;
}


/////////////////////////// ModelTexture ////////////////////////
void Vulkan_017_Terrain::ModelTexture::UpdateTexture()
{
    if (this->typeTexture == Vulkan_Texture_3D)
    {
        updateNoiseTexture();
    }
}
void Vulkan_017_Terrain::ModelTexture::updateNoiseTextureData()
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
void Vulkan_017_Terrain::ModelTexture::updateNoiseTexture()
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


/////////////////////////// ModelObjectRend /////////////////////


/////////////////////////// ModelObjectRendIndirect /////////////
void Vulkan_017_Terrain::ModelObjectRendIndirect::Destroy()
{
    //Vertex
    this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
    this->poVertexBuffer = VK_NULL_HANDLE;
    this->poVertexBufferMemory = VK_NULL_HANDLE;

    //Index
    this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
    this->poIndexBuffer = VK_NULL_HANDLE;
    this->poIndexBufferMemory = VK_NULL_HANDLE;

    CleanupSwapChain();

    this->aRends.clear();
    this->aMeshSubs.clear();
    this->pRend = nullptr;
}

void Vulkan_017_Terrain::ModelObjectRendIndirect::CleanupSwapChain()
{
    size_t count = 0;

    //1> Uniform Buffer
    count = this->poBuffers_ObjectCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
    }
    this->objectCBs.clear();
    this->poBuffers_ObjectCB.clear();
    this->poBuffersMemory_ObjectCB.clear();

    count = this->poBuffers_materialCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_materialCB[i], this->poBuffersMemory_materialCB[i]);
    }
    this->materialCBs.clear();
    this->poBuffers_materialCB.clear();
    this->poBuffersMemory_materialCB.clear();

    count = this->poBuffers_tessellationCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_tessellationCB[i], this->poBuffersMemory_tessellationCB[i]);
    }
    this->tessellationCBs.clear();
    this->poBuffers_tessellationCB.clear();
    this->poBuffersMemory_tessellationCB.clear();

    //2> VkDescriptorSets
    this->poDescriptorSets.clear();

    //3> IndirectCommand Buffer
    if (this->poBuffer_indirectCommandCB != VK_NULL_HANDLE)
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffer_indirectCommandCB, this->poBuffersMemory_indirectCommandCB);
    }
    this->indirectCommandCBs.clear();
    this->poBuffer_indirectCommandCB = VK_NULL_HANDLE;
    this->poBuffersMemory_indirectCommandCB = VK_NULL_HANDLE;
}

void Vulkan_017_Terrain::ModelObjectRendIndirect::SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends)
{
    assert(_aRends.size() > 0 && "Vulkan_017_Terrain::ModelObjectRendIndirect::SetupVertexIndexBuffer");
    this->aRends.clear();
    this->aRends = _aRends;
    this->pRend = _aRends[0];

    //1> Vertex/Index
    this->aMeshSubs.clear();
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        ModelMeshSub* pMeshSub = pR->pMeshSub;

        this->aMeshSubs.push_back(pMeshSub);
    }

    this->vertices_Pos3Color4Normal3Tex2.clear();
    this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
    this->indices.clear();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        ModelMeshSub* pMeshSub = pR->pMeshSub;

        pMeshSub->WriteVertexData(this->vertices_Pos3Color4Normal3Tex2, this->vertices_Pos3Color4Normal3Tangent3Tex2);
        pMeshSub->WriteIndexData(this->indices);
    }
    
    //Vertex
    if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
    }
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(Vertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
    }
    else
    {
        assert(false && "Vulkan_017_Terrain::ModelObjectRendIndirect::SetupVertexIndexBuffer: No vertex data !");
    }
    this->poIndexCount = this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data =  &this->indices[0];

    //2> createVertexBuffer
    this->pRend->pModelObject->pWindow->createVertexBuffer(this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pRend->pModelObject->pWindow->createIndexBuffer(this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }
}

void Vulkan_017_Terrain::ModelObjectRendIndirect::SetupUniformIndirectCommandBuffer()
{
    VkDeviceSize bufferSize;
    size_t count_sci = this->pRend->pModelObject->pWindow->poSwapChainImages.size();

    //1> Uniform Buffer
    {
        //ObjectConstants
        bufferSize = sizeof(ObjectConstants) * this->objectCBs.size();
        this->poBuffers_ObjectCB.resize(count_sci);
        this->poBuffersMemory_ObjectCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB[j], this->poBuffersMemory_ObjectCB[j]);
        }

        //MaterialConstants
        bufferSize = sizeof(MaterialConstants) * this->materialCBs.size();
        this->poBuffers_materialCB.resize(count_sci);
        this->poBuffersMemory_materialCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_materialCB[j], this->poBuffersMemory_materialCB[j]);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            bufferSize = sizeof(TessellationConstants) * this->tessellationCBs.size();
            this->poBuffers_tessellationCB.resize(count_sci);
            this->poBuffersMemory_tessellationCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_tessellationCB[j], this->poBuffersMemory_tessellationCB[j]);
            }
        }
    }

    //2> IndirectCommand Buffer
    {
        bufferSize = sizeof(VkDrawIndexedIndirectCommand) * this->indirectCommandCBs.size();
        this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_indirectCommandCB, this->poBuffersMemory_indirectCommandCB);
    }
}

void Vulkan_017_Terrain::ModelObjectRendIndirect::UpdateUniformBuffer()
{
    this->objectCBs.clear();
    this->materialCBs.clear();
    this->tessellationCBs.clear();

    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        ModelMeshSub* pMeshSub = pR->pMeshSub;

        this->objectCBs.insert(this->objectCBs.end(), pR->objectCBs.begin(), pR->objectCBs.end());
        this->materialCBs.insert(this->materialCBs.end(), pR->materialCBs.begin(), pR->materialCBs.end());
        if (pRend->isUsedTessellation)
        {
            this->tessellationCBs.insert(this->tessellationCBs.end(), pR->tessellationCBs.begin(), pR->tessellationCBs.end());
        }
    }
}

void Vulkan_017_Terrain::ModelObjectRendIndirect::UpdateIndirectCommandBuffer()
{
    this->indirectCommandCBs.clear();

    int32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    uint32_t instanceOffset = 0;
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        ModelMeshSub* pMeshSub = pR->pMeshSub;

        VkDrawIndexedIndirectCommand indirectCommand = {};
        indirectCommand.indexCount = pMeshSub->poIndexCount;
        indirectCommand.instanceCount = pRend->pModelObject->countInstance;
        indirectCommand.firstIndex = indexOffset;
        indirectCommand.vertexOffset = vertexOffset;
        indirectCommand.firstInstance = instanceOffset;
        this->indirectCommandCBs.push_back(indirectCommand);

        indexOffset += pMeshSub->poIndexCount;
        vertexOffset += pMeshSub->poVertexCount;
        instanceOffset += pRend->pModelObject->countInstance;
    }
    this->countIndirectDraw = (uint32_t)this->indirectCommandCBs.size();
}


/////////////////////////// ModelObject /////////////////////////



Vulkan_017_Terrain::Vulkan_017_Terrain(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = glm::vec3(0, -1, 0); //y-

    //this->cfg_terrain_Path = "Assets/Terrain/terrain_1025_1025.raw";
}

void Vulkan_017_Terrain::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    if (this->poPhysicalEnabledFeatures.multiDrawIndirect)
    {
        this->m_isDrawIndirectMulti = true;
    }
    else
    {
        this->m_isDrawIndirectMulti = false;
        Util_LogError("Vulkan_017_Terrain::setUpEnabledFeatures: multiDrawIndirect is not supported !");
    }
}

void Vulkan_017_Terrain::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_017_Terrain::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
}
void Vulkan_017_Terrain::cameraReset()
{
    VulkanWindow::cameraReset();

    this->pCamera->SetPos(glm::vec3(-25.0f, 13.0f, 4.0f));
    this->pCamera->SetEulerAngles(glm::vec3(35.0f, 90.0f, 0.0f));
    this->pCamera->SetFarZ(100000.0f);
}

void Vulkan_017_Terrain::loadModel_Custom()
{
    createModelMeshes();
    createModelTextures();

    int nIndexObjectRend = 0;
    for (int i = 0; i < g_Object_Count; i++)
    {
        ModelObject* pModelObject = new ModelObject(this, i);

        //1> Object
        {
            pModelObject->indexModel = i;
            pModelObject->nameObject = g_Object_Configs[2 * i + 0];
            pModelObject->nameMesh = g_Object_Configs[2 * i + 1];
            //Mesh
            {
                ModelMesh* pMesh = this->findModelMesh(pModelObject->nameMesh);
                assert(pMesh != nullptr && "Vulkan_017_Terrain::loadModel_Custom");
                pModelObject->SetMesh(pMesh);
            }
            //MeshSub Used
            {
                String nameMeshSubUsed = g_Object_MeshSubsUsed[i];
                StringVector aMeshSubUsed = VulkanUtilString::Split(nameMeshSubUsed, ";");
                pModelObject->aMeshSubUsed.clear();
                size_t count_mesh_sub_used = aMeshSubUsed.size();
                for (size_t j = 0; j < count_mesh_sub_used; j++)
                {
                    String& name = aMeshSubUsed[j];
                    int indexMeshSub = VulkanUtilString::ParserInt(name);
                    pModelObject->aMeshSubUsed.push_back(indexMeshSub);
                }
            }
            pModelObject->isShow = g_Object_IsShows[i];
            pModelObject->isRotate = g_Object_IsRotates[i];
            pModelObject->isIndirectDraw = g_Object_IsIndirectDraw[i];
            pModelObject->countInstanceExt = g_Object_InstanceExtCount[i];
            pModelObject->countInstance = pModelObject->countInstanceExt * 2 + 1;
        }

        //2> ObjectRend
        {
            size_t count_mesh_sub = pModelObject->pMesh->aMeshSubs.size();
            size_t count_mesh_sub_used = pModelObject->aMeshSubUsed.size();
            for (size_t j = 0; j < count_mesh_sub_used; j++)
            {
                int indexMeshSub = pModelObject->aMeshSubUsed[j];
                assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "Vulkan_017_Terrain::loadModel_Custom");

                ModelMeshSub* pMeshSub = pModelObject->pMesh->aMeshSubs[indexMeshSub];
                String nameObjectRend = g_ObjectRend_Configs[7 * nIndexObjectRend + 0];
                ModelObjectRend* pRend = new ModelObjectRend(nameObjectRend, pModelObject, pMeshSub);

                //Texture VS
                {
                    String nameTextureVS = g_ObjectRend_Configs[7 * nIndexObjectRend + 1]; //Texture VS
                    if (!nameTextureVS.empty())
                    {
                        StringVector aTextureVS = VulkanUtilString::Split(nameTextureVS, ";");
                        size_t count_tex = aTextureVS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureVS[p];
                            ModelTexture* pTextureVS = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), pTextureVS);
                        }
                    }
                }
                //Texture TESC
                {
                    String nameTextureTESC = g_ObjectRend_Configs[7 * nIndexObjectRend + 2]; //Texture TESC
                    if (!nameTextureTESC.empty())
                    {
                        StringVector aTextureTESC = VulkanUtilString::Split(nameTextureTESC, ";");
                        size_t count_tex = aTextureTESC.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESC[p];
                            ModelTexture* pTextureTESC = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), pTextureTESC);
                        }
                    }
                }
                //Texture TESE
                {
                    String nameTextureTESE = g_ObjectRend_Configs[7 * nIndexObjectRend + 3]; //Texture TESE
                    if (!nameTextureTESE.empty())
                    {
                        StringVector aTextureTESE = VulkanUtilString::Split(nameTextureTESE, ";");
                        size_t count_tex = aTextureTESE.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESE[p];
                            ModelTexture* pTextureTESE = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), pTextureTESE);
                        }
                    }
                }
                //Texture GS
                {
                    String nameTextureGS = g_ObjectRend_Configs[7 * nIndexObjectRend + 4]; //Texture GS
                    if (!nameTextureGS.empty())
                    {
                        StringVector aTextureGS = VulkanUtilString::Split(nameTextureGS, ";");
                        size_t count_tex = aTextureGS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureGS[p];
                            ModelTexture* pTextureGS = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureGS);
                        }
                    }
                }
                //Texture FS
                {
                    String nameTextureFS = g_ObjectRend_Configs[7 * nIndexObjectRend + 5]; //Texture FS
                    if (!nameTextureFS.empty())
                    {
                        StringVector aTextureFS = VulkanUtilString::Split(nameTextureFS, ";");
                        size_t count_tex = aTextureFS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureFS[p];
                            ModelTexture* pTextureFS = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureFS);
                        }
                    }
                }
                //Texture CS
                {
                    String nameTextureCS = g_ObjectRend_Configs[7 * nIndexObjectRend + 6]; //Texture CS
                    if (!nameTextureCS.empty())
                    {
                        StringVector aTextureCS = VulkanUtilString::Split(nameTextureCS, ";");
                        size_t count_tex = aTextureCS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureCS[p];
                            ModelTexture* pTextureCS = this->findModelTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), pTextureCS);
                        }
                    }
                }

                String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 1];
                String nameShaderTese = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 2];
                if (!nameShaderTesc.empty() || !nameShaderTese.empty())
                {
                    pRend->isUsedTessellation = true;
                    if (g_ObjectRend_IsTopologyPatchLists[nIndexObjectRend])
                    {
                        pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
                    }
                }

                //Pipeline Graphics - DescriptorSetLayout
                pRend->pPipelineGraphics->nameDescriptorSetLayout = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 0];

                //Pipeline Computes - DescriptorSetLayout
                String nameDescriptorSetLayout = g_ObjectRend_NameDescriptorSetLayouts[2 * nIndexObjectRend + 1];
                if (!nameDescriptorSetLayout.empty())
                {
                    StringVector aDescriptorSetLayout = VulkanUtilString::Split(nameDescriptorSetLayout, ";");
                    size_t count_dsl = aDescriptorSetLayout.size();
                    for (size_t p = 0; p < count_dsl; p++)
                    {
                        const String& nameDescriptorSetLayout = aDescriptorSetLayout[p];
                        PipelineCompute* pPipelineCompute = new PipelineCompute(this);
                        pPipelineCompute->nameDescriptorSetLayout = nameDescriptorSetLayout;
                        pRend->AddPipelineCompute(pPipelineCompute);
                    }
                }

                //Common
                pRend->isTransparent = g_ObjectRend_IsTransparents[nIndexObjectRend];

                pModelObject->AddObjectRend(pRend);
                m_aModelObjectRends_All.push_back(pRend);
                if (pRend->isTransparent)
                    m_aModelObjectRends_Transparent.push_back(pRend);
                else 
                    m_aModelObjectRends_Opaque.push_back(pRend);

                nIndexObjectRend ++;
            }

            m_aModelObjects.push_back(pModelObject);
            m_mapModelObjects[pModelObject->nameObject] = pModelObject;
        }
        
        //3> ObjectRendIndirectDraw
        if (pModelObject->isIndirectDraw)
        {
            size_t count_object_rend = pModelObject->aRends.size();
            if (count_object_rend > 0)
            {
                ModelObjectRend* pRend = pModelObject->aRends[i];
                String nameObjectRendIndirect = pModelObject->nameObject + " - RendIndirect";
                pModelObject->pRendIndirect = new ModelObjectRendIndirect(nameObjectRendIndirect);
                pModelObject->pRendIndirect->SetupVertexIndexBuffer(pModelObject->aRends);
            }
        }

    }
}
void Vulkan_017_Terrain::createIndirectCommands()
{

}

void Vulkan_017_Terrain::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_017_Terrain::rebuildInstanceCBs(bool isCreateVkBuffer)
{   
    VkDeviceSize bufferSize;
    size_t count_sci = this->poSwapChainImages.size();

    //1> ObjectRends
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        int indexObject = pRend->pModelObject->index;
        int count_instance = pRend->pModelObject->countInstance;
        bool isObjectLighting = g_Object_IsLightings[indexObject];

        pRend->instanceMatWorld.clear();
        pRend->objectCBs.clear();
        pRend->materialCBs.clear();
        for (int j = 0; j < count_instance; j++)
        {
            //ObjectConstants
            {
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = VulkanMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + glm::vec3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                                 g_ObjectRend_Tranforms[3 * i + 1],
                                                                 g_ObjectRend_Tranforms[3 * i + 2]);
                pRend->objectCBs.push_back(objectConstants);
                pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = VulkanMath::RandomColor(false);
                materialConstants.factorDiffuse = VulkanMath::RandomColor(false);
                materialConstants.factorSpecular = VulkanMath::RandomColor(false);
                materialConstants.shininess = VulkanMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = VulkanMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = isObjectLighting;
                //Texture VS
                {
                    ModelTexturePtrVector* pTextureVSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    ModelTexturePtrVector* pTextureFSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    ModelTexturePtrVector* pTextureCSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
                    if (pTextureCSs != nullptr)
                    {

                    }
                }
                pRend->materialCBs.push_back(materialConstants);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 3.0f;
                tessellationConstants.tessLevelInner = 3.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pRend->tessellationCBs.push_back(tessellationConstants);
            }
        }
        
        if (isCreateVkBuffer)
        {
            //ObjectConstants
            bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
            pRend->poBuffers_ObjectCB.resize(count_sci);
            pRend->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB[j], pRend->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pRend->poBuffers_materialCB.resize(count_sci);
            pRend->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_materialCB[j], pRend->poBuffersMemory_materialCB[j]);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pRend->poBuffers_tessellationCB.resize(count_sci);
                pRend->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_tessellationCB[j], pRend->poBuffersMemory_tessellationCB[j]);
                }
            }
        }
    }

    //2> ObjectRendIndriect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        if (pModelObject->isIndirectDraw &&
            pModelObject->pRendIndirect != nullptr)
        {
            pModelObject->pRendIndirect->UpdateUniformBuffer();
            pModelObject->pRendIndirect->UpdateIndirectCommandBuffer();

            if (isCreateVkBuffer)
            {
                pModelObject->pRendIndirect->SetupUniformIndirectCommandBuffer();
            }
        }
    }
}

void Vulkan_017_Terrain::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();
}   
void Vulkan_017_Terrain::createGraphicsPipeline_Custom()
{
    //1> Viewport
    VkViewportVector aViewports;
    aViewports.push_back(this->poViewport);
    VkRect2DVector aScissors;
    aScissors.push_back(this->poScissor);

    //2> Pipeline
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //[1] Shaders
        String nameShaderVert = g_ObjectRend_NameShaderModules[6 * i + 0];
        String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectRend_NameShaderModules[6 * i + 2];
        String nameShaderGeom = g_ObjectRend_NameShaderModules[6 * i + 3];
        String nameShaderFrag = g_ObjectRend_NameShaderModules[6 * i + 4];
        if (!createPipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  pRend->aShaderStageCreateInfos_Graphics))
        {
            String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pRend->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            pRend->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline(pRend->aShaderStageCreateInfos_Graphics,
                                                                                      pRend->isUsedTessellation, 0, 3,
                                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                      pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits,
                                                                                      pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                      pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                      pRend->cfg_isBlend, pRend->cfg_BlendColorFactorSrc, pRend->cfg_BlendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                      pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                      pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_017_Terrain::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

            //pPipelineGraphics->poPipeline
            VkBool32 isDepthTestEnable = pRend->cfg_isDepthTest;
            VkBool32 isDepthWriteEnable = pRend->cfg_isDepthWrite;
            VkBool32 isBlend = pRend->cfg_isBlend;
            VkBlendFactor blendColorFactorSrc = pRend->cfg_BlendColorFactorSrc; 
            VkBlendFactor blendColorFactorDst = pRend->cfg_BlendColorFactorDst; 
            if (pRend->isTransparent)
            {
                isDepthTestEnable = VK_FALSE;
                isDepthWriteEnable = VK_FALSE;

                isBlend = VK_TRUE;
                blendColorFactorSrc = VK_BLEND_FACTOR_SRC_ALPHA;
                blendColorFactorDst = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            }
            pRend->pPipelineGraphics->poPipeline = createVkGraphicsPipeline(pRend->aShaderStageCreateInfos_Graphics,
                                                                            pRend->isUsedTessellation, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                            this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                            pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE,
                                                                            isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                            pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                            pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                            pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createGraphicsPipeline_Custom: Failed to create pipeline graphics: " + pRend->nameObjectRend;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_017_Terrain::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pRend->nameObjectRend.c_str());
        }
    }
}
void Vulkan_017_Terrain::createComputePipeline_Custom()
{
    size_t count_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        size_t count_pipeline = pRend->aPipelineComputes.size();
        if (count_pipeline <= 0)
            continue;

        //[1] Shaders
        String nameShaderComp = g_ObjectRend_NameShaderModules[6 * i + 5];
        if (!createPipelineShaderStageCreateInfos(nameShaderComp,
                                                  pRend->aShaderStageCreateInfos_Computes,
                                                  pRend->mapShaderStageCreateInfos_Computes))
        {
            String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pRend->aShaderStageCreateInfos_Computes.size())
        {
            String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            PipelineCompute* p = pRend->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pRend->aShaderStageCreateInfos_Computes[j];

            p->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline(shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Terrain::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}

void Vulkan_017_Terrain::destroyModelMeshes()
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
void Vulkan_017_Terrain::createModelMeshes()
{
    for (int i = 0; i < g_MeshCount; i++)
    {
        String nameMesh = g_MeshPaths[5 * i + 0];
        String nameVertexType = g_MeshPaths[5 * i + 1];
        String nameMeshType = g_MeshPaths[5 * i + 2];
        String nameGeometryType = g_MeshPaths[5 * i + 3];
        String pathMesh = g_MeshPaths[5 * i + 4];
        
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
            String msg = "Vulkan_017_Terrain::createModelMeshes: create mesh: [" + nameMesh + "] failed !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        Util_LogInfo("Vulkan_017_Terrain::createModelMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                     nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
    }
}
Vulkan_017_Terrain::ModelMesh* Vulkan_017_Terrain::findModelMesh(const String& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_017_Terrain::destroyModelTextures()
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
void Vulkan_017_Terrain::createModelTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        VulkanTextureType typeTexture = Util_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = VulkanUtilString::ParserBool(nameIsRenderTarget);
        String nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = VulkanUtilString::ParserBool(nameIsGraphicsComputeShared);
        String pathTextures = g_TexturePaths[5 * i + 4];

        StringVector aPathTexture = VulkanUtilString::Split(pathTextures, ";");
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

        Util_LogInfo("Vulkan_017_Terrain::createModelTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                     nameTexture.c_str(), 
                     nameType.c_str(), 
                     isRenderTarget ? "true" : "false",
                     pathTextures.c_str());
    }
}
Vulkan_017_Terrain::ModelTexture* Vulkan_017_Terrain::findModelTexture(const String& nameTexture)
{
    ModelTexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_017_Terrain::destroyDescriptorSetLayouts()
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
void Vulkan_017_Terrain::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameLayout(g_DescriptorSetLayoutNames[i]);
        StringVector aLayouts = VulkanUtilString::Split(nameLayout, "-");
        size_t count_layout = aLayouts.size();

        VkDescriptorSetLayout vkDescriptorSetLayout;
        std::vector<VkDescriptorSetLayoutBinding> bindings;
        for (size_t j = 0; j < count_layout; j++)
        {
            String& strLayout = aLayouts[j];
            if (strLayout == c_strLayout_Pass) //Pass
            {
                VkDescriptorSetLayoutBinding passMainLayoutBinding = {};
                passMainLayoutBinding.binding = j;
                passMainLayoutBinding.descriptorCount = 1;
                passMainLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                passMainLayoutBinding.pImmutableSamplers = nullptr;
                passMainLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

                bindings.push_back(passMainLayoutBinding);
            }
            else if (strLayout == c_strLayout_Object) //Object
            {
                VkDescriptorSetLayoutBinding objectLayoutBinding = {};
                objectLayoutBinding.binding = j;
                objectLayoutBinding.descriptorCount = 1;
                objectLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                objectLayoutBinding.pImmutableSamplers = nullptr;
                objectLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

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
                VkDescriptorSetLayoutBinding textureCopyLayoutBinding = {};
                textureCopyLayoutBinding.binding = j;
                textureCopyLayoutBinding.descriptorCount = 1;
                textureCopyLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                textureCopyLayoutBinding.pImmutableSamplers = nullptr;
                textureCopyLayoutBinding.stageFlags = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

                bindings.push_back(textureCopyLayoutBinding);
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
                String msg = "Vulkan_017_Terrain::createDescriptorSetLayouts: Wrong DescriptorSetLayout type: " + strLayout;
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
            String msg = "VulkanWindow::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        Util_LogInfo("Vulkan_017_Terrain::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_017_Terrain::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
StringVector* Vulkan_017_Terrain::findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout)
{
    std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
}


void Vulkan_017_Terrain::destroyShaderModules()
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
void Vulkan_017_Terrain::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createShaderModule(shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        Util_LogInfo("Vulkan_017_Terrain::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                     shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_017_Terrain::findShaderModule(const String& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   
bool Vulkan_017_Terrain::createPipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                                   const String& nameShaderTesc,
                                                                   const String& nameShaderTese,
                                                                   const String& nameShaderGeom,
                                                                   const String& nameShaderFrag,
                                                                   const String& nameShaderComp,
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
bool Vulkan_017_Terrain::createPipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                                   const String& nameShaderTesc,
                                                                   const String& nameShaderTese,
                                                                   const String& nameShaderGeom,
                                                                   const String& nameShaderFrag,
                                                                   VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics)
{
    //vert
    {
        VkShaderModule shaderModule = findShaderModule(nameShaderVert);
        if (shaderModule == VK_NULL_HANDLE)
        {
            Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find vert shader module: [%s] !", nameShaderVert.c_str());
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
            Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find tesc shader module: [%s] !", nameShaderTesc.c_str());
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
            Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find tese shader module: [%s] !", nameShaderTese.c_str());
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
            Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find geom shader module: [%s] !", nameShaderGeom.c_str());
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
            Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find frag shader module: [%s] !", nameShaderFrag.c_str());
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
bool Vulkan_017_Terrain::createPipelineShaderStageCreateInfos(const String& nameShaderComp,
                                                                   VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                                   VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute)
{
    //comp
    if (!nameShaderComp.empty())
    {
        StringVector aShaderComps = VulkanUtilString::Split(nameShaderComp, ";");
        int count_comp = (int)aShaderComps.size();
        for (int i = 0; i < count_comp; i++)
        {
            String nameSC = aShaderComps[i];
            VkShaderModule shaderModule = findShaderModule(nameSC);
            if (shaderModule == VK_NULL_HANDLE)
            {
                Util_LogError("Vulkan_017_Terrain::createPipelineShaderStageCreateInfos: Can not find comp shader module: [%s] !", nameSC.c_str());
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


void Vulkan_017_Terrain::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkPipelineLayout(this->m_aVkPipelineLayouts[i]);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_017_Terrain::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameDescriptorSetLayout(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_017_Terrain::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            Util_LogError("*********************** Vulkan_017_Terrain::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_017_Terrain::findPipelineLayout(const String& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}



void Vulkan_017_Terrain::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //Pipeline Graphics
        {
            createDescriptorSets(pRend->pPipelineGraphics->poDescriptorSets, pRend->pPipelineGraphics->poDescriptorSetLayout);
            createDescriptorSets_Graphics(pRend->pPipelineGraphics->poDescriptorSets, pRend, nullptr);
        }   
        
        //Pipeline Computes
        size_t count_comp_rend = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp_rend; j++)
        {       
            PipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
            createDescriptorSets_Compute(pPipelineCompute, pRend);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (pModelObject->pRendIndirect != nullptr)
        {
            createDescriptorSets(pModelObject->pRendIndirect->poDescriptorSets, pModelObject->pRendIndirect->pRend->pPipelineGraphics->poDescriptorSetLayout);
            createDescriptorSets_Graphics(pModelObject->pRendIndirect->poDescriptorSets, pModelObject->pRendIndirect->pRend, pModelObject->pRendIndirect);
        }
    }
}
void Vulkan_017_Terrain::createDescriptorSets_Graphics(std::vector<VkDescriptorSet>& poDescriptorSets, 
                                                            ModelObjectRend* pRend, 
                                                            ModelObjectRendIndirect* pRendIndirect)
{
    StringVector* pDescriptorSetLayoutNames = pRend->pPipelineGraphics->poDescriptorSetLayoutNames;
    assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_017_Terrain::createDescriptorSets_Graphics");
    size_t count_ds = poDescriptorSets.size();
    for (size_t j = 0; j < count_ds; j++)
    {   
        std::vector<VkWriteDescriptorSet> descriptorWrites;
        int nIndexTextureVS = 0;
        int nIndexTextureTESC = 0;
        int nIndexTextureTESE = 0;
        int nIndexTextureFS = 0;

        size_t count_names = pDescriptorSetLayoutNames->size();
        for (size_t p = 0; p < count_names; p++)
        {
            String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
            if (nameDescriptorSet == c_strLayout_Pass) //Pass
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
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
                bufferInfo_Object.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB[j] : pRend->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
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
                bufferInfo_Material.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_materialCB[j] : pRend->poBuffers_materialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
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
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
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
                bufferInfo_Tessellation.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_tessellationCB[j] : pRend->poBuffers_tessellationCB[j];
                bufferInfo_Tessellation.offset = 0;
                bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
                ds.dstBinding = p;
                ds.dstArrayElement = 0;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds.descriptorCount = 1;
                ds.pBufferInfo = &bufferInfo_Tessellation;
                descriptorWrites.push_back(ds);
            }
            else if (nameDescriptorSet == c_strLayout_TextureVS) //TextureVS
            {
                ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), nIndexTextureVS);
                nIndexTextureVS ++;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
                ds.dstBinding = p;
                ds.dstArrayElement = 0;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds.descriptorCount = 1;
                ds.pImageInfo = &pTexture->poTextureImageInfo;
                descriptorWrites.push_back(ds);
            }
            else if (nameDescriptorSet == c_strLayout_TextureTESC)//TextureTESC
            {
                ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), nIndexTextureTESC);
                nIndexTextureTESC ++;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
                ds.dstBinding = p;
                ds.dstArrayElement = 0;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds.descriptorCount = 1;
                ds.pImageInfo = &pTexture->poTextureImageInfo;
                descriptorWrites.push_back(ds);
            }
            else if (nameDescriptorSet == c_strLayout_TextureTESE)//TextureTESE
            {
                ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), nIndexTextureTESE);
                nIndexTextureTESE ++;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
                ds.dstBinding = p;
                ds.dstArrayElement = 0;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds.descriptorCount = 1;
                ds.pImageInfo = &pTexture->poTextureImageInfo;
                descriptorWrites.push_back(ds);
            }
            else if (nameDescriptorSet == c_strLayout_TextureFS) //TextureFS
            {
                ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), nIndexTextureFS);
                nIndexTextureFS ++;

                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j];
                ds.dstBinding = p;
                ds.dstArrayElement = 0;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds.descriptorCount = 1;
                ds.pImageInfo = &pTexture->poTextureImageInfo;
                descriptorWrites.push_back(ds);
            }
            else
            {
                String msg = "Vulkan_017_Terrain::createDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}
void Vulkan_017_Terrain::createDescriptorSets_Compute(PipelineCompute* pPipelineCompute, 
                                                           ModelObjectRend* pRend)
{
    StringVector* pDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
    assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_017_Terrain::createDescriptorSets_Compute");
    createDescriptorSet(pPipelineCompute->poDescriptorSet, pPipelineCompute->poDescriptorSetLayout);

    std::vector<VkWriteDescriptorSet> descriptorWrites;
    int nIndexTextureCS = 0;
    size_t count_names = pDescriptorSetLayoutNames->size();
    for (size_t p = 0; p < count_names; p++)
    {
        String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
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
            ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
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
            ModelTexture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
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
            String msg = "Vulkan_017_Terrain::createDescriptorSets_Compute: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }  
    vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
}

void Vulkan_017_Terrain::updateCompute_Custom(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            PipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
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

void Vulkan_017_Terrain::updateCBs_Custom()
{
    //1> Object Rend
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_object = pRend->objectCBs.size();
        for (size_t j = 0; j < count_object; j++)
        {
            //ObjectConstants
            ObjectConstants& objectCB = pRend->objectCBs[j];
            if (pRend->pModelObject->isRotate || pRend->isRotate || this->cfg_isRotate)
            {
                objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j], 
                                                  time * glm::radians(90.0f), 
                                                  glm::vec3(0.0f, 1.0f, 0.0f));
            }
            else
            {
                objectCB.g_MatWorld = pRend->instanceMatWorld[j];
            }

            //MaterialConstants
            MaterialConstants& materialCB = pRend->materialCBs[j];
            
            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pRend->tessellationCBs[j];
            }
        }

        //ObjectConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                memcpy(data, pRend->objectCBs.data(), sizeof(ObjectConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(MaterialConstants) * count_object, 0, &data);
                memcpy(data, pRend->materialCBs.data(), sizeof(MaterialConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(TessellationConstants) * count_object, 0, &data);
                memcpy(data, pRend->tessellationCBs.data(), sizeof(TessellationConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            pRendIndirect->UpdateUniformBuffer();
            size_t count_object = pRendIndirect->objectCBs.size();
            
            //ObjectConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                    memcpy(data, pRendIndirect->objectCBs.data(), sizeof(ObjectConstants) * count_object);
                vkUnmapMemory(this->poDevice, memory);
            }

            //MaterialConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(MaterialConstants) * count_object, 0, &data);
                    memcpy(data, pRendIndirect->materialCBs.data(), sizeof(MaterialConstants) * count_object);
                vkUnmapMemory(this->poDevice, memory);
            }

            //TessellationConstants
            if (pRendIndirect->pRend->isUsedTessellation)
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(TessellationConstants) * count_object, 0, &data);
                    memcpy(data, pRendIndirect->tessellationCBs.data(), sizeof(TessellationConstants) * count_object);
                vkUnmapMemory(this->poDevice, memory);
            }

            //IndirectCommand
            {
                size_t count_indirectcommand = pRendIndirect->indirectCommandCBs.size();
                void* data;
                vkMapMemory(this->poDevice, pRendIndirect->poBuffersMemory_indirectCommandCB, 0, sizeof(VkDrawIndexedIndirectCommand) * count_indirectcommand, 0, &data);
                    memcpy(data, pRendIndirect->indirectCommandCBs.data(), sizeof(VkDrawIndexedIndirectCommand) * count_indirectcommand);
                vkUnmapMemory(this->poDevice, pRendIndirect->poBuffersMemory_indirectCommandCB);
            }
        }
    }
}

void Vulkan_017_Terrain::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            PipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
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

bool Vulkan_017_Terrain::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_017_Terrain", &windowOpened, 0);
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
void Vulkan_017_Terrain::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        //m_isDrawIndirect
        if (ImGui::Checkbox("Is DrawIndirect", &this->m_isDrawIndirect))
        {
            
        }
        //m_isDrawIndirectMulti
        if (ImGui::Checkbox("Is DrawIndirectMulti", &this->m_isDrawIndirectMulti))
        {
            
        }

        float fGap = g_Object_InstanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 100.0f))
        {
            g_Object_InstanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        size_t count_object = this->m_aModelObjects.size();
        for (size_t i = 0; i < count_object; i++)
        {
            ModelObject* pModelObject = this->m_aModelObjects[i];
            ModelObjectRendIndirect* pRendIndirect = pModelObject->pRendIndirect;
            size_t count_object_rend = pModelObject->aRends.size();

            //1> ModelObject
            String nameObject = VulkanUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameObject.c_str()))
            {
                //isShow
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isShow = pModelObject->isShow;
                    }
                }
                //isWireFrame
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isWireFrame = pModelObject->isWireFrame;
                    }
                }
                //isRotate
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isRotate = pModelObject->isRotate;
                    }
                }
                //isLighting
                String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        for (int p = 0; p < pModelObject->countInstance; p++)
                        {
                            MaterialConstants& mat = pRend->materialCBs[p];
                            mat.lighting = pModelObject->isLighting;
                        }
                    }
                }

                String nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 3);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                //2> ModelObjectRend
                if (pRendIndirect != nullptr && this->m_isDrawIndirect)
                {
                    String& nameObjectRendIndirect = pRendIndirect->nameObjectRendIndirect;
                    if (ImGui::CollapsingHeader(nameObjectRendIndirect.c_str()))
                    {
                        ImGui::Text("Rend Count: [%d], Vertex: [%d], Index: [%d]", 
                            (int)pRendIndirect->aRends.size(), 
                            (int)pRendIndirect->poVertexCount,
                            (int)pRendIndirect->poIndexCount);
                        //isShow
                        String nameIsShowRend = "Is Show - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRendIndirect->isShow))
                        {
                            if (pRendIndirect->isShow)
                            {
                                pModelObject->isShow = true;
                            }
                        }
                        //isWireFrame
                        String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRendIndirect->isWireFrame))
                        {
                            if (!pRendIndirect->isWireFrame)
                            {
                                pModelObject->isWireFrame = false;
                            }
                        }
                        //isRotate
                        String nameIsRotateRend = "Is Rotate - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRendIndirect->isRotate))
                        {
                            
                        }
                        //isLighting
                        String nameIsLightingRend = "Is Lighting - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRendIndirect->isLighting))
                        {
                            if (pRendIndirect->isLighting)
                            {
                                pModelObject->isLighting = true;
                            }
                            for (size_t p = 0; p < pRendIndirect->materialCBs.size(); p++)
                            {
                                MaterialConstants& mat = pRendIndirect->materialCBs[p];
                                mat.lighting = pRendIndirect->isLighting;
                            }
                        }
                        //isTransparent
                        String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRendIndirect;
                        bool isTransparent = pRendIndirect->isTransparent;
                        ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                        //countIndirectDraw
                        String nameCountIndirectDraw = "Count IndirectDraw - " + nameObjectRendIndirect;
                        int countIndirectDraw = (int)pRendIndirect->countIndirectDraw;
                        if (ImGui::DragInt(nameCountIndirectDraw.c_str(), &countIndirectDraw, 1, 0, (int)pRendIndirect->indirectCommandCBs.size()))
                        {
                            pRendIndirect->countIndirectDraw = (uint32_t)countIndirectDraw;
                        }

                        //ObjectRend
                        size_t count_rend = pRendIndirect->aRends.size();
                        for (int j = 0; j < count_rend; j++)
                        {
                            ModelObjectRend* pRend = pRendIndirect->aRends[j];

                            ImGui::Text("[%d], Vertex: [%d], Index: [%d], - [%s]", 
                            j,
                            (int)pRend->pMeshSub->poVertexCount,
                            (int)pRend->pMeshSub->poIndexCount,
                            pRend->nameObjectRend.c_str());
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        String& nameObjectRend = pRend->nameObjectRend;
                        if (ImGui::CollapsingHeader(nameObjectRend.c_str()))
                        {
                            ImGui::Text("Vertex: [%d], Index: [%d]", (int)pRend->pMeshSub->poVertexCount, (int)pRend->pMeshSub->poIndexCount);
                            //isShow
                            String nameIsShowRend = "Is Show - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsShowRend.c_str(), &pRend->isShow))
                            {
                                if (pRend->isShow)
                                {
                                    pModelObject->isShow = true;
                                }
                            }
                            //isWireFrame
                            String nameIsWireFrameRend = "Is WireFrame - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsWireFrameRend.c_str(), &pRend->isWireFrame))
                            {
                                if (!pRend->isWireFrame)
                                {
                                    pModelObject->isWireFrame = false;
                                }
                            }
                            //isRotate
                            String nameIsRotateRend = "Is Rotate - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsRotateRend.c_str(), &pRend->isRotate))
                            {
                                
                            }
                            //isLighting
                            String nameIsLightingRend = "Is Lighting - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsLightingRend.c_str(), &pRend->isLighting))
                            {
                                if (pRend->isLighting)
                                {
                                    pModelObject->isLighting = true;
                                }
                                for (int p = 0; p < pModelObject->countInstance; p++)
                                {
                                    MaterialConstants& mat = pRend->materialCBs[p];
                                    mat.lighting = pModelObject->isLighting;
                                }
                            }
                            //isTransparent
                            String nameIsTransparent = "Is Transparent(Read Only) - " + nameObjectRend;
                            bool isTransparent = pRend->isTransparent;
                            ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);

                            String nameWorld = "Model Object - " + nameObjectRend;
                            if (ImGui::CollapsingHeader(nameWorld.c_str()))
                            {
                                int count_instance = pModelObject->countInstance;
                                for (int p = 0; p < count_instance; p++)
                                {
                                    ObjectConstants& obj = pRend->objectCBs[p];
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = VulkanUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const glm::mat4& mat4World = obj.g_MatWorld;
                                            String nameTable = VulkanUtilString::SaveInt(p) + " - matWorld - " + nameObjectRend;
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
                                        String nameMaterial = VulkanUtilString::SaveInt(p) + " - Material - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                                        {
                                            //factorAmbient
                                            String nameFactorAmbient = "FactorAmbient - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorDiffuse
                                            String nameFactorDiffuse = "FactorDiffuse - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorSpecular
                                            String nameFactorSpecular = "FactorSpecular - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //shininess
                                            String nameShininess = "Shininess - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //alpha
                                            String nameAlpha = "Alpha - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //lighting
                                            String nameLighting = "Lighting - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isLighting = mat.lighting == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                            {
                                                mat.lighting = isLighting ? 1.0f : 0.0f;
                                            }

                                            //Texture VS
                                            {
                                                ModelTexturePtrVector* pTextureVSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                ModelTexturePtrVector* pTextureFSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                                                if (pTextureFSs != nullptr)
                                                {
                                                    size_t count_texture = pTextureFSs->size();
                                                    for (size_t q = 0; q < count_texture; q++)
                                                    {
                                                        ModelTexture* pTexture = (*pTextureFSs)[q];

                                                        String nameMaterial_Texture = VulkanUtilString::SaveInt(p) + " - Material - " + nameObjectRend + " - TextureFS - " + VulkanUtilString::SaveInt(q);
                                                        if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                                        {
                                                            //texWidth
                                                            String nameWidth = "Width - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int width = pTexture->width;
                                                            ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                            //texHeight
                                                            String nameHeight = "Height - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int height = pTexture->height;
                                                            ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                            //texDepth
                                                            String nameDepth = "Depth - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int depth = pTexture->depth;
                                                            ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                            //indexTextureArray
                                                            String nameIndexTextureArray = "IndexTextureArray - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (pTexture->typeTexture == Vulkan_Texture_2DArray)
                                                            {
                                                                int count_tex = (int)pTexture->aPathTexture.size();
                                                                int indexTextureArray = (int)mat.aTexLayers[q].indexTextureArray;
                                                                if (ImGui::DragInt(nameIndexTextureArray.c_str(), &indexTextureArray, 1, 0, count_tex - 1))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = indexTextureArray;
                                                                }
                                                            }
                                                            else 
                                                            {
                                                                if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[q].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                                {

                                                                }
                                                            }

                                                            //texSpeedU
                                                            String nameTexSpeedU = "TexSpeedU - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedV
                                                            String nameTexSpeedV = "texSpeedV - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedW
                                                            String nameTexSpeedW = "texSpeedW - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }

                                                            //texChunkMaxX
                                                            String nameTexChunkMaxX = "texChunkMaxX - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxX = mat.aTexLayers[q].texChunkMaxX;
                                                            ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                            //texChunkMaxY
                                                            String nameTexChunkMaxY = "texChunkMaxY - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxY = mat.aTexLayers[q].texChunkMaxY;
                                                            ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                            //texChunkIndexX
                                                            String nameTexChunkIndexX = "texChunkIndexX - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexX = mat.aTexLayers[q].texChunkIndexX;
                                                            ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                            //texChunkIndexY
                                                            String nameTexChunkIndexY = "texChunkIndexY - " + VulkanUtilString::SaveInt(p) + " - " + VulkanUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexY = mat.aTexLayers[q].texChunkIndexY;
                                                            ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                                        }
                                                    }
                                                }
                                            }
                                            //Texture CS
                                            {
                                                ModelTexturePtrVector* pTextureCSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
                                                if (pTextureCSs != nullptr)
                                                {

                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                        //TessellationConstants
                                        String nameTessellation = VulkanUtilString::SaveInt(p) + " - Tessellation - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                        {
                                            if (pRend->isUsedTessellation)
                                            {
                                                TessellationConstants& tess = pRend->tessellationCBs[j];
                                                //tessLevelOuter
                                                String nameTessLevelOuter = "tessLevelOuter - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessLevelInner
                                                String nameTessLevelInner = "tessLevelInner - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessAlpha
                                                String nameTessAlpha = "tessAlpha - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                                {
                                                    
                                                }
                                                //tessStrength
                                                String nameTessStrength = "tessStrength - " + VulkanUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
                                                {
                                                    
                                                }
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
    }
}

void Vulkan_017_Terrain::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_017_Terrain::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
{   
    if (this->m_isDrawIndirect)
    {
        //1> Opaque
        {
            drawModelObjectRendIndirects(commandBuffer, this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Transparent);
        }
    }
    else
    {
        //1> Opaque
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Opaque);
        }
        //2> Transparent
        {
            drawModelObjectRends(commandBuffer, this->m_aModelObjectRends_Transparent);
        }
    }
}
void Vulkan_017_Terrain::drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
{
    ModelObjectRendIndirect* pRendIndirect_Last = nullptr;
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        ModelObjectRendIndirect* pRendIndirect = pRend->pModelObject->pRendIndirect;
        if (pRendIndirect != nullptr)
        {
            if (pRendIndirect_Last != nullptr && pRendIndirect_Last == pRendIndirect)
                continue;
            if (!pRendIndirect->isShow)
            {
                pRendIndirect_Last = nullptr;
                continue;
            }
            drawModelObjectRendIndirect(commandBuffer, pRendIndirect);
            pRendIndirect_Last = pRendIndirect;
        }
        else
        {
            if (!pRend->isShow)
                continue;
            drawModelObjectRend(commandBuffer, pRend);
        }
    }
}   
void Vulkan_017_Terrain::drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect)
{
    ModelObjectRend* pRend = pRendIndirect->pRend;
    ModelObject* pModelObject = pRend->pModelObject;

    VkBuffer vertexBuffers[] = { pRendIndirect->poVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
    if (pRendIndirect->poIndexBuffer != nullptr)
    {
        bindIndexBuffer(commandBuffer, pRendIndirect->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    if (pModelObject->isWireFrame || pRendIndirect->isWireFrame || this->cfg_isWireFrame)
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_WireFrame);
        if (pRendIndirect->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRendIndirect->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
    }
    else
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline);
        if (pRendIndirect->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRendIndirect->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
    }

    uint32_t drawCount = pRendIndirect->countIndirectDraw;
    if (m_isDrawIndirectMulti)
    {
        drawIndexedIndirect(commandBuffer, pRendIndirect->poBuffer_indirectCommandCB, 0, drawCount, sizeof(VkDrawIndexedIndirectCommand));
    }
    else
    {
        for (uint32_t i = 0; i < drawCount; i++)
        {
            drawIndexedIndirect(commandBuffer, pRendIndirect->poBuffer_indirectCommandCB, i * sizeof(VkDrawIndexedIndirectCommand), 1, sizeof(VkDrawIndexedIndirectCommand));
        }
    }
}

void Vulkan_017_Terrain::drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
{
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        if (!pRend->isShow)
            continue;
        drawModelObjectRend(commandBuffer, pRend);
    }
}
void Vulkan_017_Terrain::drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
{
    ModelObject* pModelObject = pRend->pModelObject;
    ModelMeshSub* pMeshSub = pRend->pMeshSub;

    VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
    VkDeviceSize offsets[] = { 0 };
    bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
    if (pMeshSub->poIndexBuffer != nullptr)
    {
        bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }

    if (pModelObject->isWireFrame || pRend->isWireFrame || this->cfg_isWireFrame)
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_WireFrame);
        if (pRend->pPipelineGraphics->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRend->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pMeshSub->poIndexBuffer != nullptr)
        {
            drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
        }
        else
        {
            draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
        }
    }
    else
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline);
        if (pRend->pPipelineGraphics->poDescriptorSets.size() > 0)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout, 0, 1, &pRend->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        }
        if (pMeshSub->poIndexBuffer != nullptr)
        {
            drawIndexed(commandBuffer, pMeshSub->poIndexCount, pModelObject->countInstance, 0, 0, 0);
        }
        else
        {
            draw(commandBuffer, pMeshSub->poVertexCount, pModelObject->countInstance, 0, 0);
        }
    }
}

void Vulkan_017_Terrain::cleanupCustom()
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
    this->m_mapModelObjects.clear();
    this->m_aModelObjectRends_All.clear();
    this->m_aModelObjectRends_Opaque.clear();
    this->m_aModelObjectRends_Transparent.clear();
}

void Vulkan_017_Terrain::cleanupSwapChain_Custom()
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

void Vulkan_017_Terrain::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}