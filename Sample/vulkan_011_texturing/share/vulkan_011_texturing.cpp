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


/////////////////////////// Type ////////////////////////////////
enum BumpMappingType
{
    BumpMapping_DiffuseColor = 0,       //0: DiffuseColor
    BumpMapping_BumpMapping,            //1: NormalMapping

};
struct EnumBumpMappingDesc { BumpMappingType Value; const char* Name; const char* Tooltip; };
static const EnumBumpMappingDesc s_aBumpMappingDescs[] =
{
    { BumpMapping_DiffuseColor,                   "DiffuseColor",                     "Diffuse Color" },
    { BumpMapping_BumpMapping,                    "BumpMapping",                      "Bump Mapping" },
};


enum NormalMappingType
{
    NormalMapping_DiffuseColor = 0,     //0: DiffuseColor
    NormalMapping_NormalColor,          //1: NormalColor
    NormalMapping_NormalMapping,        //2: NormalMapping

};
struct EnumNormalMappingDesc { NormalMappingType Value; const char* Name; const char* Tooltip; };
static const EnumNormalMappingDesc s_aNormalMappingDescs[] =
{
    { NormalMapping_DiffuseColor,                 "DiffuseColor",                     "Diffuse Color" },
    { NormalMapping_NormalColor,                  "NormalColor",                      "Normal Color" },
    { NormalMapping_NormalMapping,                "NormalMapping",                    "Normal Mapping" },
};


enum ParallaxMappingType
{
    ParallaxMapping_DiffuseColor = 0,           //0: DiffuseColor
    ParallaxMapping_NormalColor,                //1: NormalColor
    ParallaxMapping_NormalMapping,              //2: NormalMapping
    ParallaxMapping_CommonParallaxMapping,      //3: CommonParallaxMapping
    ParallaxMapping_SteepParallaxMapping,       //4: SteepParallaxMapping
    ParallaxMapping_OcclusionParallaxMapping,   //5: OcclusionParallaxMapping

};
struct EnumParallaxMappingDesc { ParallaxMappingType Value; const char* Name; const char* Tooltip; };
static const EnumParallaxMappingDesc s_aParallaxMappingDescs[] =
{
    { ParallaxMapping_DiffuseColor,                 "DiffuseColor",                     "Diffuse Color" },
    { ParallaxMapping_NormalColor,                  "NormalColor",                      "Normal Color" },
    { ParallaxMapping_NormalMapping,                "NormalMapping",                    "Normal Mapping" },
    { ParallaxMapping_CommonParallaxMapping,        "CommonParallaxMapping",            "Common Parallax Mapping" },
    { ParallaxMapping_SteepParallaxMapping,         "SteepParallaxMapping",             "Steep Parallax Mapping" },
    { ParallaxMapping_OcclusionParallaxMapping,     "OcclusionParallaxMapping",         "Occlusion Parallax Mapping" },
};



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 5;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane
    "plane_nt",         "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane_nt
    "plane_gltf",       "Pos3Color4Normal3Tangent3Tex2",        "file",             "",                         "Assets/Model/Fbx/plane.fbx", //plane_gltf
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Obj/cube/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Model/Fbx/sphere.fbx", //sphere

};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane
    true, //plane_nt
    true, //plane_gltf
    false, //cube
    false, //sphere

};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  
    false, //plane_nt  
    false, //plane_gltf  
    false, //cube
    false, //sphere
    
};
static glm::mat4 g_MeshTranformLocals[g_MeshCount] = 
{
    VulkanMath::ms_mat4Unit, //plane
    VulkanMath::ms_mat4Unit, //plane_nt
    VulkanMath::ms_mat4Unit, //plane_gltf
    VulkanMath::ms_mat4Unit, //cube
    VulkanMath::ms_mat4Unit, //sphere

};


/////////////////////////// Texture /////////////////////////////
static const std::string g_TextureDefault = "default_blackwhite";
static const int g_TextureCount = 20;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                  //Texture Type  //TextureIsRenderTarget  //TextureIsGraphicsComputeShared  //Texture Path
    "default_blackwhite",           "2d",           "false",                 "false",                          "Assets/Texture/default_blackwhite.png", //default_blackwhite
    "terrain",                      "2d",           "false",                 "false",                          "Assets/Texture/terrain.png", //terrain
    "white",                        "2d",           "false",                 "false",                          "Assets/Texture/white.bmp", //white
    
////Basic-Level Texture Operation
    "texturesampler_wrap",          "2d",           "false",                 "false",                          "Assets/Texture/texture2d.jpg", //texturesampler_wrap
    "texturesampler_mirror",        "2d",           "false",                 "false",                          "Assets/Texture/texture2d.jpg", //texturesampler_mirror
    "texturesampler_clamp",         "2d",           "false",                 "false",                          "Assets/Texture/texture2d.jpg", //texturesampler_clamp
    "texturesampler_border",        "2d",           "false",                 "false",                          "Assets/Texture/texture2d.jpg", //texturesampler_border
    "texture1d",                    "1d",           "false",                 "false",                          "Assets/Texture/texture1d.tga", //texture1d
    "texture2d",                    "2d",           "false",                 "false",                          "Assets/Texture/texture2d.jpg", //texture2d
    "texture2darray",               "2darray",      "false",                 "false",                          "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture2darray
    "texture3d",                    "3d",           "false",                 "false",                          "", //texture3d
    "texturecubemap",               "cubemap",      "false",                 "false",                          "Assets/Texture/texturecubemap_x_right.png;Assets/Texture/texturecubemap_x_left.png;Assets/Texture/texturecubemap_y_up.png;Assets/Texture/texturecubemap_y_down.png;Assets/Texture/texturecubemap_z_front.png;Assets/Texture/texturecubemap_z_back.png", //texturecubemap
    "textureanimation_scroll",      "2darray",      "false",                 "false",                          "Assets/Texture/textureanimation1.png;Assets/Texture/textureanimation2.png", //textureanimation_scroll
    "textureanimation_chunk",       "2darray",      "false",                 "false",                          "Assets/Texture/textureanimation3.png", //textureanimation_chunk

////High-Level Texture Operation
    "texturebumpmap_diffuse",       "2d",           "false",                 "false",                          "Assets/Texture/bricks_diffuse.png", //texturebumpmap_diffuse
    "texturebumpmap_bumpmap",       "2d",           "false",                 "false",                          "Assets/Texture/bricks_bumpmap.png", //texturebumpmap_bumpmap
    "texturenormalmap_normalmap",   "2d",           "false",                 "false",                          "Assets/Texture/bricks_normalmap.png", //texturenormalmap_normalmap

    "rocks_color",                  "2d",           "false",                 "false",                          "Assets/Texture/rocks_color.png", //rocks_color
    "rocks_normal_height",          "2d",           "false",                 "false",                          "Assets/Texture/rocks_normal_height.png", //rocks_normal_height

    "stonefloor_color_height",      "2d",           "false",                 "false",                          "Assets/Texture/stonefloor_color_height.png", //stonefloor_color_height

};
static VkFormat g_TextureFormats[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //default_blackwhite
    VK_FORMAT_R8G8B8A8_SRGB, //terrain
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

    VK_FORMAT_R8G8B8A8_SRGB, //rocks_color
    VK_FORMAT_R8G8B8A8_UNORM, //rocks_normal_height

    VK_FORMAT_R8G8B8A8_UNORM, //stonefloor_color_height

};
static VulkanTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //default_blackwhite
    Vulkan_TextureFilter_Bilinear, //terrain
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

    Vulkan_TextureFilter_Bilinear, //rocks_color
    Vulkan_TextureFilter_Bilinear, //rocks_normal_height

    Vulkan_TextureFilter_Bilinear, //stonefloor_color_height

};
static VulkanTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //default_blackwhite
    Vulkan_TextureAddressing_Clamp, //terrain
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

    Vulkan_TextureAddressing_Clamp, //rocks_color
    Vulkan_TextureAddressing_Clamp, //rocks_normal_height

    Vulkan_TextureAddressing_Clamp, //stonefloor_color_height

};
static VulkanTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //default_blackwhite
    Vulkan_TextureBorderColor_OpaqueBlack, //terrain
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

    Vulkan_TextureBorderColor_OpaqueBlack, //rocks_color
    Vulkan_TextureBorderColor_OpaqueBlack, //rocks_normal_height

    Vulkan_TextureBorderColor_OpaqueBlack, //stonefloor_color_height

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //default_blackwhite
    512,    512,    1, //terrain
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

    1024,  1024,    1, //rocks_color
    1024,  1024,    1, //rocks_normal_height

    1024,  1024,    1, //stonefloor_color_height

};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //default_blackwhite
    0,    0, //terrain
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

    0,    0, //rocks_color
    0,    0, //rocks_normal_height

    0,    0, //stonefloor_color_height

};


/////////////////////////// DescriptorSetLayout /////////////////
const std::string c_strLayout_Pass = "Pass";
const std::string c_strLayout_Object = "Object";
const std::string c_strLayout_Material = "Material";
const std::string c_strLayout_Instance = "Instance";
const std::string c_strLayout_TextureCopy = "TextureCopy";
const std::string c_strLayout_Tessellation = "Tessellation";
const std::string c_strLayout_TextureVS = "TextureVS";
const std::string c_strLayout_TextureTESC = "TextureTESC";
const std::string c_strLayout_TextureTESE = "TextureTESE";
const std::string c_strLayout_TextureFS = "TextureFS";
const std::string c_strLayout_TextureCSR = "TextureCSR";
const std::string c_strLayout_TextureCSRW = "TextureCSRW";
static const int g_DescriptorSetLayoutCount = 3;
static const char* g_nameDescriptorSetLayouts[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS",

    "Pass-Object-Material-Instance-TextureTESE-TextureFS-Tessellation",

};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 26;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type              //path

    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_texsampler_lit",                "vert",             "Assets/Shader/standard_mesh_opaque_texsampler_lit.vert.spv", //standard_mesh_opaque_texsampler_lit vert
    "vert_standard_mesh_opaque_tex1d_lit",                     "vert",             "Assets/Shader/standard_mesh_opaque_tex1d_lit.vert.spv", //standard_mesh_opaque_tex1d_lit vert
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",             "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_opaque_tex2darray_lit",                "vert",             "Assets/Shader/standard_mesh_opaque_tex2darray_lit.vert.spv", //standard_mesh_opaque_tex2darray_lit vert
    "vert_standard_mesh_opaque_tex3d_lit",                     "vert",             "Assets/Shader/standard_mesh_opaque_tex3d_lit.vert.spv", //standard_mesh_opaque_tex3d_lit vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",             "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_texanim_scroll_lit",            "vert",             "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.vert.spv", //standard_mesh_opaque_texanim_scroll_lit vert
    "vert_standard_mesh_opaque_texanim_chunk_lit",             "vert",             "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.vert.spv", //standard_mesh_opaque_texanim_chunk_lit vert

    "vert_standard_mesh_opaque_texbumpmap_lit",                "vert",             "Assets/Shader/standard_mesh_opaque_texbumpmap_lit.vert.spv", //standard_mesh_opaque_texbumpmap_lit vert
    "vert_standard_mesh_opaque_texnormalmap_lit",              "vert",             "Assets/Shader/standard_mesh_opaque_texnormalmap_lit.vert.spv", //standard_mesh_opaque_texnormalmap_lit vert   
    "vert_standard_mesh_opaque_texparallaxmap_lit",            "vert",             "Assets/Shader/standard_mesh_opaque_texparallaxmap_lit.vert.spv", //standard_mesh_opaque_texparallaxmap_lit vert
    "vert_standard_mesh_opaque_texdisplacementmap_lit",        "vert",             "Assets/Shader/standard_mesh_opaque_texdisplacementmap_lit.vert.spv", //standard_mesh_opaque_texdisplacementmap_lit vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////
    "tesc_standard_tessellation_displacement",                 "tesc",              "Assets/Shader/standard_tessellation_displacement.tesc.spv", //standard_tessellation_displacement tesc

    ///////////////////////////////////////// tese /////////////////////////////////////////
    "tese_standard_tessellation_displacement",                 "tese",              "Assets/Shader/standard_tessellation_displacement.tese.spv", //standard_tessellation_displacement tese

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_texsampler_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texsampler_lit.frag.spv", //standard_mesh_opaque_texsampler_lit frag
    "frag_standard_mesh_opaque_tex1d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex1d_lit.frag.spv", //standard_mesh_opaque_tex1d_lit frag
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_opaque_tex2darray_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_tex2darray_lit.frag.spv", //standard_mesh_opaque_tex2darray_lit frag
    "frag_standard_mesh_opaque_tex3d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex3d_lit.frag.spv", //standard_mesh_opaque_tex3d_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_texanim_scroll_lit",            "frag",              "Assets/Shader/standard_mesh_opaque_texanim_scroll_lit.frag.spv", //standard_mesh_opaque_texanim_scroll_lit frag
    "frag_standard_mesh_opaque_texanim_chunk_lit",             "frag",              "Assets/Shader/standard_mesh_opaque_texanim_chunk_lit.frag.spv", //standard_mesh_opaque_texanim_chunk_lit frag
         
    "frag_standard_mesh_opaque_texbumpmap_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texbumpmap_lit.frag.spv", //standard_mesh_opaque_texbumpmap_lit frag
    "frag_standard_mesh_opaque_texnormalmap_lit",              "frag",              "Assets/Shader/standard_mesh_opaque_texnormalmap_lit.frag.spv", //standard_mesh_opaque_texnormalmap_lit frag
    "frag_standard_mesh_opaque_texparallaxmap_lit",            "frag",              "Assets/Shader/standard_mesh_opaque_texparallaxmap_lit.frag.spv", //standard_mesh_opaque_texparallaxmap_lit frag
    "frag_standard_mesh_opaque_texdisplacementmap_lit",        "frag",              "Assets/Shader/standard_mesh_opaque_texdisplacementmap_lit.frag.spv", //standard_mesh_opaque_texdisplacementmap_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////


};


/////////////////////////// Object //////////////////////////////
static const int g_ObjectCount = 18;
static const char* g_ObjectConfigs[8 * g_ObjectCount] = 
{
    //Object Name                       //Mesh Path                    //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS           //Texture FS                                                         //Texture CS
    "ground",                           "plane",                       "",                     "",                              "",                         "",                   "terrain",                                                           "", //ground

////Basic-Level Texture Operation
    "textureSampler_Wrap",              "plane",                       "",                     "",                              "",                         "",                   "texturesampler_wrap",                                                "", //textureSampler_Wrap
    "textureSampler_Mirror",            "plane",                       "",                     "",                              "",                         "",                   "texturesampler_mirror",                                              "", //textureSampler_Mirror
    "textureSampler_Clamp",             "plane",                       "",                     "",                              "",                         "",                   "texturesampler_clamp",                                               "", //textureSampler_Clamp
    "textureSampler_Border",            "plane",                       "",                     "",                              "",                         "",                   "texturesampler_border",                                              "", //textureSampler_Border
    "texture1D",                        "plane",                       "",                     "",                              "",                         "",                   "texture1d",                                                          "", //texture1D
    "texture2D",                        "plane",                       "",                     "",                              "",                         "",                   "texture2d",                                                          "", //texture2D
    "texture2Darray",                   "plane",                       "",                     "",                              "",                         "",                   "texture2darray",                                                     "", //texture2Darray
    "texture3D",                        "plane",                       "",                     "",                              "",                         "",                   "texture3d",                                                          "", //texture3D
    "textureCubeMap_SkyBox",            "cube",                        "",                     "",                              "",                         "",                   "texturecubemap",                                                     "", //textureCubeMap_SkyBox
    "textureCubeMap_Sphere",            "sphere",                      "",                     "",                              "",                         "",                   "texturecubemap",                                                     "", //textureCubeMap_Sphere
    "textureAnimation_Scroll",          "plane",                       "",                     "",                              "",                         "",                   "textureanimation_scroll",                                            "", //textureAnimation_Scroll
    "textureAnimation_Chunk",           "plane",                       "",                     "",                              "",                         "",                   "textureanimation_chunk",                                             "", //textureAnimation_Chunk

////High-Level Texture Operation
    "textureOriginal",                  "plane",                       "",                     "",                              "",                         "",                   "texturebumpmap_diffuse",                                             "", //textureOriginal    
    "textureBumpMap",                   "plane",                       "",                     "",                              "",                         "",                   "texturebumpmap_diffuse;texturebumpmap_bumpmap",                      "", //textureBumpMap
    "textureNormalMap",                 "plane_nt",                    "",                     "",                              "",                         "",                   "texturebumpmap_diffuse;texturenormalmap_normalmap",                  "", //textureNormalMap
    "textureParallaxMap",               "plane_gltf",                  "",                     "",                              "",                         "",                   "rocks_color;rocks_normal_height",                                    "", //textureParallaxMap
    "textureDisplacementMap",           "plane_nt",                    "",                     "",                              "stonefloor_color_height",  "",                   "stonefloor_color_height",                                            "", //textureDisplacementMap

};

static const std::string g_Object_Texture3D = "texture3D";
static const std::string g_Object_TextureAnimation_Scroll = "textureAnimation_Scroll";
static const std::string g_Object_TextureAnimation_Chunk = "textureAnimation_Chunk";
static const std::string g_Object_TextureOriginal = "textureOriginal";
static const std::string g_Object_TextureBumpMap = "textureBumpMap";
static const std::string g_Object_TextureNormalMap = "textureNormalMap";
static const std::string g_Object_TextureParallaxMap = "textureParallaxMap";
static const std::string g_Object_TextureDisplacementMap = "textureDisplacementMap";

static const char* g_ObjectNameShaderModules[6 * g_ObjectCount] = 
{
    //vert                                                  //tesc                                          //tese                                          //geom                      //frag                                                  //comp
    
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //ground

////Basic-Level Texture Operation
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Wrap 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Mirror 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Clamp 
    "vert_standard_mesh_opaque_texsampler_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texsampler_lit",             "", //textureSampler_Border 
    "vert_standard_mesh_opaque_tex1d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex1d_lit",                  "", //texture1D 
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //texture2D 
    "vert_standard_mesh_opaque_tex2darray_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2darray_lit",             "", //texture2Darray 
    "vert_standard_mesh_opaque_tex3d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex3d_lit",                  "", //texture3D 
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_SkyBox 
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //textureCubeMap_Sphere 
    "vert_standard_mesh_opaque_texanim_scroll_lit",         "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texanim_scroll_lit",         "", //textureAnimation_Scroll 
    "vert_standard_mesh_opaque_texanim_chunk_lit",          "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texanim_chunk_lit",          "", //textureAnimation_Chunk 

////High-Level Texture Operation
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                             "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //textureOriginal 
    "vert_standard_mesh_opaque_texbumpmap_lit",             "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texbumpmap_lit",             "", //textureBumpMap 
    "vert_standard_mesh_opaque_texnormalmap_lit",           "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texnormalmap_lit",           "", //textureNormalMap 
    "vert_standard_mesh_opaque_texparallaxmap_lit",         "",                                             "",                                             "",                         "frag_standard_mesh_opaque_texparallaxmap_lit",         "", //textureParallaxMap 
    "vert_standard_mesh_opaque_texdisplacementmap_lit",     "tesc_standard_tessellation_displacement",      "tese_standard_tessellation_displacement",      "",                         "frag_standard_mesh_opaque_texdisplacementmap_lit",     "", //textureDisplacementMap 

};
static const char* g_ObjectNameDescriptorSetLayouts[2 * g_ObjectCount] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-Object-Material-Instance-TextureFS",                          "", //ground 

////Basic-Level Texture Operation
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureSampler_Wrap 
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureSampler_Mirror 
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureSampler_Clamp 
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureSampler_Border 
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture1D 
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture2D 
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture2Darray
    "Pass-Object-Material-Instance-TextureFS",                          "", //texture3D
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureCubeMap_SkyBox
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureCubeMap_Sphere
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureAnimation_Scroll
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureAnimation_Chunk

////High-Level Texture Operation
    "Pass-Object-Material-Instance-TextureFS",                          "", //textureOriginal 
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //textureBumpMap
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //textureNormalMap
    "Pass-Object-Material-Instance-TextureFS-TextureFS",                "", //textureParallaxMap
    "Pass-Object-Material-Instance-TextureTESE-TextureFS-Tessellation", "", //textureDisplacementMap

};
static float g_instanceGap = 1.2f;
static int g_ObjectInstanceExtCount[g_ObjectCount] =
{
    0, //ground

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
    5, //textureParallaxMap 
    5, //textureDisplacementMap 

};
static glm::vec3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    glm::vec3(   0, -0.1,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 1.0f,   1.0f,   1.0f), //ground

////Basic-Level Texture Operation
    glm::vec3(   0,  0.1,  0.4),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureSampler_Wrap
    glm::vec3(   0,  0.1,  1.5),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureSampler_Mirror
    glm::vec3(   0,  0.1,  2.6),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureSampler_Clamp
    glm::vec3(   0,  0.1,  3.7),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureSampler_Border
    glm::vec3(   0,  0.1,  4.8),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //texture1D
    glm::vec3(   0,  0.1,  5.9),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //texture2D
    glm::vec3(   0,  0.1,  7.0),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //texture2Darray
    glm::vec3(   0,  0.1,  8.1),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //texture3D
    glm::vec3(   0,    0,    0),     glm::vec3(     0,  0,  0),    glm::vec3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    glm::vec3(   0,  0.4,  9.2),     glm::vec3(     0,  0,  0),    glm::vec3( 0.005f,  0.005f,  0.005f), //textureCubeMap_Sphere
    glm::vec3(   0,  0.1, 10.3),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureAnimation_Scroll
    glm::vec3(   0,  0.1, 11.4),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureAnimation_Chunk

////High-Level Texture Operation
    glm::vec3(   0,  0.1, 12.5),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureOriginal
    glm::vec3(   0,  0.1, 13.6),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureBumpMap
    glm::vec3(   0,  0.1, 14.7),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureNormalMap
    glm::vec3(   0,  0.1, 15.8),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureParallaxMap
    glm::vec3(   0,  0.1, 16.9),     glm::vec3(     0,  0,  0),    glm::vec3( 0.01f,   0.01f,    0.01f), //textureDisplacementMap

};
static bool g_ObjectIsTransparents[g_ObjectCount] = 
{
    false, //ground

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
    false, //textureParallaxMap
    false, //textureDisplacementMap

};
static bool g_ObjectIsShows[] = 
{
    true, //ground

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
    true, //textureParallaxMap
    true, //textureDisplacementMap

};
static bool g_ObjectIsRotates[g_ObjectCount] =
{
    false, //ground

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
    false, //textureParallaxMap
    false, //textureDisplacementMap

};
static bool g_ObjectIsLightings[g_ObjectCount] =
{
    true, //ground

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
    true, //textureParallaxMap
    true, //textureDisplacementMap

};
static bool g_ObjectIsTopologyPatchLists[g_ObjectCount] =
{
    false, //ground

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
    false, //textureParallaxMap
    false, //textureDisplacementMap

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
    this->cfg_isUseComputeShader = true;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = glm::vec3(0, -1, 0); //y-
}

void Vulkan_011_Texturing::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    //Tessellation Enable
    if (this->poPhysicalDeviceFeatures.tessellationShader)
    {
        this->poPhysicalEnabledFeatures.tessellationShader = VK_TRUE;
    }
    else
    {
        Util_LogError("Vulkan_011_Texturing::setUpEnabledFeatures: tessellationShader is not supported !");
    }
    
}

void Vulkan_011_Texturing::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();

}

void Vulkan_011_Texturing::createCamera()
{
    this->pCamera = new VulkanCamera();
    cameraReset();
    this->pCamera->SetPos(glm::vec3(-4.0f, 24.0f, 3.6f));
    this->pCamera->SetEulerAngles(glm::vec3(80.0f, 0.0f, 0.0f));
}

void Vulkan_011_Texturing::loadModel_Custom()
{
    createModelMeshes();
    createModelTextures();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);
        pModelObject->indexModel = i;
        pModelObject->nameObject = g_ObjectConfigs[8 * i + 0];
        pModelObject->nameMesh = g_ObjectConfigs[8 * i + 1];

        //Mesh
        {
            ModelMesh* pMesh = this->findModelMesh(pModelObject->nameMesh);
            assert(pMesh != nullptr && "Vulkan_011_Texturing::loadModel_Custom");
            pModelObject->SetMesh(pMesh);
        }

        //Texture VS
        {
            std::string nameTextureVS = g_ObjectConfigs[8 * i + 2]; //Texture VS
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
        //Texture TESC
        {
            std::string nameTextureTESC = g_ObjectConfigs[8 * i + 3]; //Texture TESC
            if (!nameTextureTESC.empty())
            {
                std::vector<std::string> aTextureTESC = VulkanUtilString::Split(nameTextureTESC, ";");
                size_t count_tex = aTextureTESC.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureTESC[j];
                    ModelTexture* pTextureTESC = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), pTextureTESC);
                }
            }
        }
        //Texture TESE
        {
            std::string nameTextureTESE = g_ObjectConfigs[8 * i + 4]; //Texture TESE
            if (!nameTextureTESE.empty())
            {
                std::vector<std::string> aTextureTESE = VulkanUtilString::Split(nameTextureTESE, ";");
                size_t count_tex = aTextureTESE.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureTESE[j];
                    ModelTexture* pTextureTESE = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), pTextureTESE);
                }
            }
        }
        //Texture GS
        {
            std::string nameTextureGS = g_ObjectConfigs[8 * i + 5]; //Texture GS
            if (!nameTextureGS.empty())
            {
                std::vector<std::string> aTextureGS = VulkanUtilString::Split(nameTextureGS, ";");
                size_t count_tex = aTextureGS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    std::string nameTex = aTextureGS[j];
                    ModelTexture* pTextureGS = this->findModelTexture(nameTex);
                    pModelObject->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureGS);
                }
            }
        }
        //Texture FS
        {
            std::string nameTextureFS = g_ObjectConfigs[8 * i + 6]; //Texture FS
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
            std::string nameTextureCS = g_ObjectConfigs[8 * i + 7]; //Texture CS
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
                        size_t count_texture = pTextureFSs->size();
                        for (size_t p = 0; p < count_texture; p++)
                        {
                            ModelTexture* pTexture = (*pTextureFSs)[p];
                            materialConstants.aTexLayers[p].texWidth = pTexture->width;
                            materialConstants.aTexLayers[p].texHeight = pTexture->height;
                            materialConstants.aTexLayers[p].texDepth = pTexture->depth;

                            if (pModelObject->nameObject == g_Object_Texture3D) //Texture3D
                            {
                                materialConstants.aTexLayers[p].indexTextureArray = VulkanMath::RandF(0.0f, 1.0f);
                            }
                            else if (pModelObject->nameObject == g_Object_TextureAnimation_Scroll) //TextureAnimation_Scroll
                            {
                                if (pTexture->typeTexture == Vulkan_Texture_2DArray)
                                {
                                    materialConstants.aTexLayers[p].indexTextureArray = pTexture->RandomTextureIndex();

                                    if (materialConstants.aTexLayers[p].indexTextureArray > 0)
                                    {
                                        materialConstants.aTexLayers[p].texSpeedU = VulkanMath::RandF(1.0f, 10.0f);
                                    }
                                    else 
                                    {
                                        materialConstants.aTexLayers[p].texSpeedV = VulkanMath::RandF(1.0f, 10.0f);
                                    }
                                }
                            }
                            else if (pModelObject->nameObject == g_Object_TextureAnimation_Chunk) //TextureAnimation_Chunk
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
                            else if (pModelObject->nameObject == g_Object_TextureOriginal) //TextureOriginal
                            {
                                if (j == pModelObject->countInstance / 2)
                                {
                                    materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                                    materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                    materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                    materialConstants.lighting = 0.0f;
                                }
                            }
                            else if (pModelObject->nameObject == g_Object_TextureBumpMap) //TextureBumpMap
                            {
                                if (j == pModelObject->countInstance / 2)
                                {
                                    materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                                    materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                    materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                }
                                materialConstants.aTexLayers[p].indexTextureArray = 1; 
                                materialConstants.aTexLayers[p].texSpeedU = VulkanMath::RandF(20.0f, 1000.0f);
                            } 
                            else if (pModelObject->nameObject == g_Object_TextureNormalMap) //TextureNormalMap
                            {
                                if (j == pModelObject->countInstance / 2)
                                {
                                    materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                                    materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                    materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                }
                                materialConstants.aTexLayers[p].indexTextureArray = 2;
                            }
                            else if (pModelObject->nameObject == g_Object_TextureParallaxMap) //TextureParallaxMap
                            {
                                if (j == pModelObject->countInstance / 2)
                                {
                                    materialConstants.factorAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
                                    materialConstants.factorDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                    materialConstants.factorSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                                }
                                materialConstants.aTexLayers[p].indexTextureArray = 3;
                                materialConstants.aTexLayers[p].texSpeedU = 0.1f;
                                materialConstants.aTexLayers[p].texSpeedV = -0.02f;
                                materialConstants.aTexLayers[p].texSpeedW = 48.0f;
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
                pModelObject->materialCBs.push_back(materialConstants);
            }
            
            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevel = 0.1f;
                tessellationConstants.tessAlpha = 1.0f;
                pModelObject->tessellationCBs.push_back(tessellationConstants);
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

            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pModelObject->poBuffers_tessellationCB.resize(count_sci);
                pModelObject->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_tessellationCB[j], pModelObject->poBuffersMemory_tessellationCB[j]);
                }
            }
        }
    }
}

void Vulkan_011_Texturing::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();
}  
void Vulkan_011_Texturing::createGraphicsPipeline_Custom()
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
            std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
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
                std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Failed to create pipeline wire frame !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_011_Texturing::createGraphicsPipeline_Custom: Object: [%s] Create pipeline wireframe success !", pModelObject->nameObject.c_str());

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
                std::string msg = "Vulkan_011_Texturing::createGraphicsPipeline_Custom: Failed to create pipeline !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            Util_LogInfo("Vulkan_011_Texturing::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics success !", pModelObject->nameObject.c_str());
        }
    }   
}
void Vulkan_011_Texturing::createComputePipeline_Custom()
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
            std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Can not find shader used !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pModelObject->aShaderStageCreateInfos_Computes.size())
        {
            std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Pipeline count is not equal shader count !";
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
                std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline(shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                std::string msg = "Vulkan_011_Texturing::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
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

        Util_LogInfo("Vulkan_011_Texturing::createModelTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                     nameTexture.c_str(), 
                     nameType.c_str(), 
                     isRenderTarget ? "true" : "false",
                     pathTextures.c_str());
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
        std::string shaderName = g_ShaderModulePaths[3 * i + 0];
        std::string shaderType = g_ShaderModulePaths[3 * i + 1];
        std::string shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createShaderModule(shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        Util_LogInfo("Vulkan_011_Texturing::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                     shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_011_Texturing::findShaderModule(const std::string& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   
bool Vulkan_011_Texturing::createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
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
bool Vulkan_011_Texturing::createPipelineShaderStageCreateInfos(const std::string& nameShaderVert,
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
            Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find vert shader module: [%s] !", nameShaderVert.c_str());
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
            Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find tesc shader module: [%s] !", nameShaderTesc.c_str());
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
            Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find tese shader module: [%s] !", nameShaderTese.c_str());
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
            Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find geom shader module: [%s] !", nameShaderGeom.c_str());
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
            Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find frag shader module: [%s] !", nameShaderFrag.c_str());
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
bool Vulkan_011_Texturing::createPipelineShaderStageCreateInfos(const std::string& nameShaderComp,
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
                Util_LogError("Vulkan_011_Texturing::createPipelineShaderStageCreateInfos: Can not find comp shader module: [%s] !", nameSC.c_str());
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

        //Pipeline Graphics
        {
            std::vector<std::string>* pDescriptorSetLayoutNames = pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames;
            assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_011_Texturing::createDescriptorSets_Custom");
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
                    else if (nameDescriptorSet == c_strLayout_TextureVS)//TextureVS
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
                        std::string msg = "Vulkan_011_Texturing::createDescriptorSets_Custom: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
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
            assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_011_Texturing::createDescriptorSets_Custom");
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
                    std::string msg = "Vulkan_011_Texturing::createDescriptorSets_Custom: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }  
            vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }   
}

void Vulkan_011_Texturing::updateCompute_Custom(VkCommandBuffer& commandBuffer)
{

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
            if (pModelObject->nameObject == g_Object_TextureAnimation_Chunk)
            {
                ModelTexturePtrVector* pTextureFSs = pModelObject->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                if (pTextureFSs != nullptr)
                {
                    size_t count_texture = pTextureFSs->size();
                    for (size_t p = 0; p < count_texture; p++)
                    {
                        ModelTexture* pTexture = (*pTextureFSs)[p];

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
            }   

            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                TessellationConstants& tessellationCB = pModelObject->tessellationCBs[j];
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
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 63);
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
                                                    if (pModelObject->nameObject == g_Object_TextureBumpMap) //TextureBumpMap
                                                    {
                                                        //BumpMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aBumpMappingDescs); nIndex++)
                                                        {
                                                            if (s_aBumpMappingDescs[nIndex].Value == mat.aTexLayers[p].indexTextureArray)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aBumpMappingDescs[nIndex].Name;
                                                        std::string nameBumpMappingType = "BumpMappingType - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameBumpMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aBumpMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aBumpMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = (int)s_aBumpMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }

                                                        //Bump Scale
                                                        std::string nameBumpScale = "BumpScale - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameBumpScale.c_str(), &mat.aTexLayers[p].texSpeedU, 0.5f, 0.0f, 5000.0f))
                                                        {
                                                            
                                                        }
                                                    }
                                                    else if (pModelObject->nameObject == g_Object_TextureNormalMap) //TextureNormalMap
                                                    {
                                                        //NormalMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aNormalMappingDescs); nIndex++)
                                                        {
                                                            if (s_aNormalMappingDescs[nIndex].Value == mat.aTexLayers[p].indexTextureArray)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aNormalMappingDescs[nIndex].Name;
                                                        std::string nameNormalMappingType = "NormalMappingType - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameNormalMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aNormalMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aNormalMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = (int)s_aNormalMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }
                                                    }
                                                    else if (pModelObject->nameObject == g_Object_TextureParallaxMap) //TextureParallaxMap
                                                    {
                                                        //ParallaxMapping Type
                                                        int nIndex = 0;
                                                        for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aParallaxMappingDescs); nIndex++)
                                                        {
                                                            if (s_aParallaxMappingDescs[nIndex].Value == mat.aTexLayers[p].indexTextureArray)
                                                                break;
                                                        }
                                                        const char* preview_text = s_aParallaxMappingDescs[nIndex].Name;
                                                        std::string nameParallaxMappingType = "ParallaxMappingType - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::BeginCombo(nameParallaxMappingType.c_str(), preview_text))
                                                        {
                                                            for (int q = 0; q < IM_ARRAYSIZE(s_aParallaxMappingDescs); q++)
                                                            {
                                                                if (ImGui::Selectable(s_aParallaxMappingDescs[q].Name, nIndex == q))
                                                                {
                                                                    mat.aTexLayers[p].indexTextureArray = (int)s_aParallaxMappingDescs[q].Value;
                                                                    break;
                                                                }
                                                            }
                                                            ImGui::EndCombo();
                                                        }

                                                        //heightScale
                                                        std::string nameHeightScale = "HeightScale - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameHeightScale.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 10.0f))
                                                        {
                                                            
                                                        }
                                                        //parallaxBias
                                                        std::string nameParallaxBias = "ParallaxBias - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        if (ImGui::DragFloat(nameParallaxBias.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, -5.0f, 5.0f))
                                                        {
                                                            
                                                        }
                                                        //numLayers
                                                        std::string nameNumLayers = "NumLayers - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                        int numLayers = (int)mat.aTexLayers[p].texSpeedW;
                                                        if (ImGui::DragInt(nameNumLayers.c_str(), &numLayers, 1, 1, 100.0f))
                                                        {
                                                            mat.aTexLayers[p].texSpeedW = numLayers;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        if (ImGui::DragFloat(nameIndexTextureArray.c_str(), &mat.aTexLayers[p].indexTextureArray, 0.001f, 0.0f, 1.0f))
                                                        {

                                                        }
                                                    }
                                                }

                                                if (pModelObject->nameObject != g_Object_TextureBumpMap &&
                                                    pModelObject->nameObject != g_Object_TextureParallaxMap)
                                                {
                                                    //texSpeedU
                                                    std::string nameTexSpeedU = "TexSpeedU - " + VulkanUtilString::SaveInt(j) + " - " + VulkanUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                    if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                    {
                                                        
                                                    }
                                                }
                                                
                                                if (pModelObject->nameObject != g_Object_TextureParallaxMap)
                                                {
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
                                TessellationConstants& tess = pModelObject->tessellationCBs[j];
                                //tessLevel
                                std::string nameTessLevel = "tessLevel - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameTessLevel.c_str(), &tess.tessLevel, 0.025f, 0.1f, 3.0f))
                                {
                                    
                                }
                                //tessAlpha
                                std::string nameTessAlpha = "tessAlpha - " + VulkanUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                {
                                    
                                }
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
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipeline_WireFrame);
            if (pModelObject->pPipelineGraphics->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipelineLayout, 0, 1, &pModelObject->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
            }
            drawModelObject(commandBuffer, pModelObject);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipeline);
            if (pModelObject->pPipelineGraphics->poDescriptorSets.size() > 0)
            {
                vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pModelObject->pPipelineGraphics->poPipelineLayout, 0, 1, &pModelObject->pPipelineGraphics->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
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

void Vulkan_011_Texturing::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->recreateSwapChain();
    }
}