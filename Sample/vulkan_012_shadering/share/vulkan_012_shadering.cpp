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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>



/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 5;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "geo_triangle",     "Pos3Color4Normal3Tex2",                "geometry",         "EntityTriangle",           "", //geo_triangle

    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Common/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Common/cube.obj", //cube
    "sphere",           "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Common/sphere.fbx", //sphere
    "bunny",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/bunny/bunny.obj", //bunny

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
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //geo_triangle

    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube
    FMath::ms_mat4Unit, //sphere
    FMath::ms_mat4Unit, //bunny

};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 11;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "default_white",                    "2D",            "false",                  "false",                           "Assets/Texture/Common/default_white.bmp", //default_white  
    "default_blackwhite",               "2D",            "false",                  "false",                           "Assets/Texture/Common/default_blackwhite.png", //default_blackwhite
    "bricks_diffuse",                   "2D",            "false",                  "false",                           "Assets/Texture/Common/bricks_diffuse.png", //bricks_diffuse
    "terrain",                          "2D",            "false",                  "false",                           "Assets/Texture/Common/terrain.png", //terrain
    "texture2d",                        "2D",            "false",                  "false",                           "Assets/Texture/Common/texture2d.jpg", //texture2d
    
    "texturecubemap",                   "CubeMap",       "false",                  "false",                           "Assets/Texture/Sky/texturecubemap_x_right.png;Assets/Texture/Sky/texturecubemap_x_left.png;Assets/Texture/Sky/texturecubemap_y_up.png;Assets/Texture/Sky/texturecubemap_y_down.png;Assets/Texture/Sky/texturecubemap_z_front.png;Assets/Texture/Sky/texturecubemap_z_back.png", //texturecubemap

    "texture_terrain_diffuse",          "2DArray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png", //texture_terrain_diffuse
    "texture_terrain_normal",           "2DArray",       "false",                  "false",                           "Assets/Texture/Terrain/shore_sand_norm.png;Assets/Texture/Terrain/moss_norm.tga;Assets/Texture/Terrain/rock_cliff_norm.tga;Assets/Texture/Terrain/cliff_norm.png", //texture_terrain_normal
    "texture_terrain_control",          "2DArray",       "false",                  "false",                           "Assets/Texture/Terrain/terrain_control.png", //texture_terrain_control

    "texture_rt_compute_copy_tex",      "2D",            "true",                   "true",                            "", //texture_rt_compute_copy_tex
    "texture_rt_compute_copy_texarray", "2D",            "true",                   "true",                            "", //texture_rt_compute_copy_texarray

};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white
    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite
    F_TexturePixelFormat_R8G8B8A8_SRGB, //bricks_diffuse
    F_TexturePixelFormat_R8G8B8A8_SRGB, //terrain
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturecubemap

    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture_terrain_diffuse
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_terrain_normal
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_terrain_control

    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_rt_compute_copy_tex
    F_TexturePixelFormat_R8G8B8A8_UNORM, //texture_rt_compute_copy_texarray

};
static FTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    F_TextureFilter_Bilinear, //default_white
    F_TextureFilter_Bilinear, //default_blackwhite
    F_TextureFilter_Bilinear, //bricks_diffuse
    F_TextureFilter_Bilinear, //terrain
    F_TextureFilter_Bilinear, //texture2d

    F_TextureFilter_Bilinear, //texturecubemap

    F_TextureFilter_Bilinear, //texture_terrain_diffuse
    F_TextureFilter_Bilinear, //texture_terrain_normal
    F_TextureFilter_Bilinear, //texture_terrain_control

    F_TextureFilter_Bilinear, //texture_rt_compute_copy_tex
    F_TextureFilter_Bilinear, //texture_rt_compute_copy_texarray

};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //default_white
    F_TextureAddressing_Clamp, //default_blackwhite
    F_TextureAddressing_Clamp, //bricks_diffuse
    F_TextureAddressing_Clamp, //terrain
    F_TextureAddressing_Clamp, //texture2d

    F_TextureAddressing_Wrap, //texturecubemap

    F_TextureAddressing_Clamp, //texture_terrain_diffuse
    F_TextureAddressing_Clamp, //texture_terrain_normal
    F_TextureAddressing_Clamp, //texture_terrain_control

    F_TextureAddressing_Clamp, //texture_rt_compute_copy_tex
    F_TextureAddressing_Clamp, //texture_rt_compute_copy_texarray

};
static FTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    F_TextureBorderColor_OpaqueBlack, //default_white
    F_TextureBorderColor_OpaqueBlack, //default_blackwhite
    F_TextureBorderColor_OpaqueBlack, //bricks_diffuse
    F_TextureBorderColor_OpaqueBlack, //terrain
    F_TextureBorderColor_OpaqueBlack, //texture2d

    F_TextureBorderColor_OpaqueBlack, //texturecubemap

    F_TextureBorderColor_OpaqueBlack, //texture_terrain_diffuse
    F_TextureBorderColor_OpaqueBlack, //texture_terrain_normal
    F_TextureBorderColor_OpaqueBlack, //texture_terrain_control

    F_TextureBorderColor_OpaqueBlack, //texture_rt_compute_copy_tex
    F_TextureBorderColor_OpaqueBlack, //texture_rt_compute_copy_texarray

};
static int g_TextureSizes[3 * g_TextureCount] = 
{
     64,     64,    1, //default_white
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
    0,    0, //default_white
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

    "geometry_show",                            "bunny",            "",                     "default_white",                                                                        "", //geometry_show 
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
static FVector3 g_ObjectTranforms[3 * g_ObjectCount] = 
{   
    FVector3(   0,    0,   0),     FVector3(     0,  0,  0),    FVector3( 100.0f,  100.0f,  100.0f), //textureCubeMap_SkyBox
    FVector3(-2.0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainDiffuse
    FVector3(   0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainNormal
    FVector3( 2.0,  1.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //texture2Darray_TerrainControl

    FVector3(   0,  2.2,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //compute_CopyTexture
    FVector3(   0,  3.4,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //compute_CopyTextureArray

    FVector3(   0,  4.6,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //tessellation_passthrough
    FVector3(-2.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_integer
    FVector3(-1.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_even
    FVector3(   0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_fractional_odd
    FVector3( 1.0,  5.8,   0),     FVector3(     0,  0,  0),    FVector3(  1.0f,    1.0f,    1.0f), //tessellation_triangle_pow2
    FVector3(   0,  7.0,   0),     FVector3(   -90,  0,  0),    FVector3( 0.01f,   0.01f,   0.01f), //tessellation_pntriangles

    FVector3(   0,   0,  -10),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //geometry_show
    FVector3(   0,   0,  -10),     FVector3(     0, 180, 0),    FVector3( 1.0f,   1.0f,   1.0f), //geometry_normal

    FVector3(   0, -0.1,   0),     FVector3(     0,  0,  0),    FVector3( 1.0f,   1.0f,   1.0f), //terrain

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
bool Vulkan_012_Shadering::ModelMesh::LoadMesh(bool isFlipY, bool isTransformLocal, const FMatrix4& matTransformLocal)
{
    //1> Load
    FMeshDataPC meshDataPC;
    FMeshData meshData;
    meshData.bIsFlipY = isFlipY;
    if (this->typeMesh == F_Mesh_File)
    {
        unsigned int eMeshParserFlags = aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices;
        if (!FMeshDataLoader::LoadMeshData(this->pathMesh, meshData, eMeshParserFlags))
        {
            F_LogError("*********************** Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh failed: [%s] !", this->pathMesh.c_str());
            return false; 
        }
    }
    else if (this->typeMesh == F_Mesh_Geometry)
    {
        if (!FMeshGeometry::CreateGeometry(&meshDataPC, &meshData, this->typeGeometryType))
        {
            F_LogError("*********************** Vulkan_012_Shadering::ModelMesh::LoadMesh: create geometry mesh failed: typeGeometry: [%s] !", F_GetMeshGeometryTypeName(this->typeGeometryType).c_str());
            return false; 
        }
    }
    else
    {
        F_Assert(false && "Vulkan_012_Shadering::ModelMesh::LoadMesh: Wrong typeMesh !")
        return false;
    }

    int count_vertex = (int)meshData.vertices.size();
    if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tex2.reserve(count_vertex);
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
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        F_LogInfo("Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                  this->nameMesh.c_str(),
                  (int)this->vertices_Pos3Color4Normal3Tex2.size(), 
                  (int)this->indices.size());
    }
    else if (this->poTypeVertex == F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
    {
        this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
        this->vertices_Pos3Color4Normal3Tangent3Tex2.reserve(count_vertex);
        for (int i = 0; i < count_vertex; i++)
        {
            FMeshVertex& vertex = meshData.vertices[i];
            FVertex_Pos3Color4Normal3Tangent3Tex2 v;
            v.pos = vertex.pos;
            v.color = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
            v.normal = vertex.normal;
            v.tangent = vertex.tangent;
            v.texCoord = vertex.texCoord;
            if (isTransformLocal)
            {
                v.pos = FMath::Transform(matTransformLocal, v.pos);
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
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
        this->poIndexCount = (uint32_t)this->indices.size();
        this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
        this->poIndexBuffer_Data = &this->indices[0];

        F_LogInfo("Vulkan_012_Shadering::ModelMesh::LoadMesh: load mesh [%s] success, [Pos3Color4Normal3Tangent3Tex2]: Vertex count: [%d], Index count: [%d] !", 
                  this->nameMesh.c_str(),
                  (int)this->vertices_Pos3Color4Normal3Tangent3Tex2.size(), 
                  (int)this->indices.size());
    }

    //2> createVertexBuffer
    this->pWindow->createVertexBuffer("Vertex-" + this->nameMesh, this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pWindow->createIndexBuffer("Index-" + this->nameMesh, this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }

    return true;
}



/////////////////////////// ModelObject /////////////////////////



Vulkan_012_Shadering::Vulkan_012_Shadering(int width, int height, String name)
    : VulkanWindow(width, height, name)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = false;
    this->cfg_isUseComputeShader = true;
    this->cfg_isCreateRenderComputeSycSemaphore = true;

    this->cfg_cameraPos = FVector3(-2.5f, 2.0f, -20.0f);
    this->cfg_cameraLookTarget = FVector3(-2.5f, 5.0f, 0.0f);
    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector3(0, -1, 0); //y-
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
        F_LogError("*********************** Vulkan_012_Shadering::setUpEnabledFeatures: tessellationShader is not supported !");
    }

}

void Vulkan_012_Shadering::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_012_Shadering::createCamera()
{
    VulkanWindow::createCamera();
    
    cameraReset();
}
void Vulkan_012_Shadering::cameraReset()
{
    VulkanWindow::cameraReset();
}

void Vulkan_012_Shadering::loadModel_Custom()
{
    createMeshes();
    createTextures();

    for (int i = 0; i < g_ObjectCount; i++)
    {
        ModelObject* pModelObject = new ModelObject(this);

        pModelObject->indexModel = i;
        pModelObject->nameObject = g_ObjectConfigs[5 * i + 0]; //Object Name     
        pModelObject->nameMesh = g_ObjectConfigs[5 * i + 1]; //Mesh Name

        //Mesh
        {
            ModelMesh* pMesh = this->findMesh(pModelObject->nameMesh);
            F_Assert(pMesh != nullptr && "Vulkan_012_Shadering::loadModel_Custom")
            pModelObject->SetMesh(pMesh);
        }

        //Texture VS
        {
            String nameTextureVS = g_ObjectConfigs[5 * i + 2]; //Texture VS
            if (!nameTextureVS.empty())
            {
                StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
                size_t count_tex = aTextureVS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureVS[j];
                    Texture* pTextureVS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
                }
            }
        }
        //Texture FS
        {
            String nameTextureFS = g_ObjectConfigs[5 * i + 3]; //Texture FS
            if (!nameTextureFS.empty())
            {
                StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
                size_t count_tex = aTextureFS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureFS[j];
                    Texture* pTextureFS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
                }
            }
        }
        //Texture CS
        {
            String nameTextureCS = g_ObjectConfigs[5 * i + 4]; //Texture CS
            if (!nameTextureCS.empty())
            {
                StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
                size_t count_tex = aTextureCS.size();
                for (size_t j = 0; j < count_tex; j++)
                {
                    String nameTex = aTextureCS[j];
                    Texture* pTextureCS = this->findTexture(nameTex);
                    pModelObject->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
                }
            }
        }

        //Tessellation
        String nameShaderTesc = g_ObjectNameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectNameShaderModules[6 * i + 2];
        if (!nameShaderTesc.empty() || !nameShaderTese.empty())
        {
            pModelObject->isUsedTessellation = true;
            if (g_ObjectIsTopologyPatchLists[i])
            {
                pModelObject->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;
            }
        }

        //Geometry
        String nameShaderGeom = g_ObjectNameShaderModules[6 * i + 3];
        if (!nameShaderGeom.empty())
        {
            pModelObject->isUsedGeometry = true;
        }

        //Pipeline Graphics - DescriptorSetLayout
        pModelObject->pPipelineGraphics->nameDescriptorSetLayout = g_ObjectNameDescriptorSetLayouts[2 * i + 0];

        //Pipeline Computes - DescriptorSetLayout
        String nameDescriptorSetLayout = g_ObjectNameDescriptorSetLayouts[2 * i + 1];
        if (!nameDescriptorSetLayout.empty())
        {
            StringVector aDescriptorSetLayout = FUtilString::Split(nameDescriptorSetLayout, ";");
            size_t count_dsl = aDescriptorSetLayout.size();
            for (size_t j = 0; j < count_dsl; j++)
            {
                const String& nameDescriptorSetLayout = aDescriptorSetLayout[j];
                VKPipelineCompute* pPipelineCompute = new VKPipelineCompute("PipelineC-Object");
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
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectTranforms[i * 3 + 0] + FVector3((j - pModelObject->countInstanceExt) * g_instanceGap , 0, 0),
                                                                 g_ObjectTranforms[i * 3 + 1],
                                                                 g_ObjectTranforms[i * 3 + 2]);
                pModelObject->objectCBs.push_back(objectConstants);
                pModelObject->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = g_ObjectIsLightings[i];
                //Texture VS
                {
                    TexturePtrVector* pTextureVSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    TexturePtrVector* pTextureFSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    TexturePtrVector* pTextureCSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
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
                String nameBuffer = "ObjectConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_ObjectCB[j], pModelObject->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pModelObject->poBuffers_materialCB.resize(count_sci);
            pModelObject->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "MaterialConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_materialCB[j], pModelObject->poBuffersMemory_materialCB[j]);
            }

            //TessellationConstants
            if (pModelObject->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pModelObject->poBuffers_tessellationCB.resize(count_sci);
                pModelObject->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    String nameBuffer = "TessellationConstants-" + FUtilString::SavePointI(FPointI(i,j));
                    createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_tessellationCB[j], pModelObject->poBuffersMemory_tessellationCB[j]);
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
                    String nameBuffer = "GeometryConstants-" + FUtilString::SavePointI(FPointI(i,j));
                    createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pModelObject->poBuffers_geometryCB[j], pModelObject->poBuffersMemory_geometryCB[j]);
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
        String nameShaderVert = g_ObjectNameShaderModules[6 * i + 0];
        String nameShaderTesc = g_ObjectNameShaderModules[6 * i + 1];
        String nameShaderTese = g_ObjectNameShaderModules[6 * i + 2];
        String nameShaderGeom = g_ObjectNameShaderModules[6 * i + 3];
        String nameShaderFrag = g_ObjectNameShaderModules[6 * i + 4];
        if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  m_mapVkShaderModules,
                                                  pModelObject->aShaderStageCreateInfos_Graphics))
        {
            String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pModelObject->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pModelObject->pPipelineGraphics->nameDescriptorSetLayout);
            if (pModelObject->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pModelObject->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            pModelObject->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline("PipelineGraphics-Wire-" + pModelObject->nameObject,
                                                                                             pModelObject->aShaderStageCreateInfos_Graphics,
                                                                                             pModelObject->isUsedTessellation, 0, 3,
                                                                                             Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                             Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                             this->poRenderPass, pModelObject->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                             pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pModelObject->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                             pModelObject->cfg_isDepthTest, pModelObject->cfg_isDepthWrite, pModelObject->cfg_DepthCompareOp,
                                                                                             pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                             pModelObject->cfg_isBlend, pModelObject->cfg_BlendColorFactorSrc, pModelObject->cfg_BlendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                             pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                             pModelObject->cfg_ColorWriteMask);
            if (pModelObject->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_012_Shadering::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pModelObject->nameObject.c_str());

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
            pModelObject->pPipelineGraphics->poPipeline = createVkGraphicsPipeline("PipelineGraphics-" + pModelObject->nameObject,
                                                                                   pModelObject->aShaderStageCreateInfos_Graphics,
                                                                                   pModelObject->isUsedTessellation, 0, 3,
                                                                                   Util_GetVkVertexInputBindingDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex), 
                                                                                   Util_GetVkVertexInputAttributeDescriptionVectorPtr(pModelObject->pMesh->poTypeVertex),
                                                                                   this->poRenderPass, pModelObject->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                   pModelObject->cfg_vkPrimitiveTopology, pModelObject->cfg_vkFrontFace, pModelObject->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                   isDepthTestEnable, isDepthWriteEnable, pModelObject->cfg_DepthCompareOp,
                                                                                   pModelObject->cfg_isStencilTest, pModelObject->cfg_StencilOpFront, pModelObject->cfg_StencilOpBack, 
                                                                                   isBlend, blendColorFactorSrc, blendColorFactorDst, pModelObject->cfg_BlendColorOp,
                                                                                   pModelObject->cfg_BlendAlphaFactorSrc, pModelObject->cfg_BlendAlphaFactorDst, pModelObject->cfg_BlendAlphaOp,
                                                                                   pModelObject->cfg_ColorWriteMask);
            if (pModelObject->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createGraphicsPipeline_Custom: Failed to create pipeline graphics !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_012_Shadering::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pModelObject->nameObject.c_str());
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
        String nameShaderComp = g_ObjectNameShaderModules[6 * i + 5];
        if (!CreatePipelineShaderStageCreateInfos(nameShaderComp,
                                                  m_mapVkShaderModules,
                                                  pModelObject->aShaderStageCreateInfos_Computes,
                                                  pModelObject->mapShaderStageCreateInfos_Computes))
        {
            String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pModelObject->aShaderStageCreateInfos_Computes.size())
        {
            String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            VKPipelineCompute* p = pModelObject->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pModelObject->aShaderStageCreateInfos_Computes[j];

            p->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline("PipelineCompute-" + p->nameDescriptorSetLayout, shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_012_Shadering::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}

void Vulkan_012_Shadering::destroyMeshes()
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
void Vulkan_012_Shadering::createMeshes()
{
    for (int i = 0; i < g_MeshCount; i++)
    {
        String nameMesh = g_MeshPaths[5 * i + 0];
        String nameVertexType = g_MeshPaths[5 * i + 1];
        String nameMeshType = g_MeshPaths[5 * i + 2];
        String nameGeometryType = g_MeshPaths[5 * i + 3];
        String pathMesh = g_MeshPaths[5 * i + 4];
        
        FMeshVertexType typeVertex = F_ParseMeshVertexType(nameVertexType); 
        FMeshType typeMesh = F_ParseMeshType(nameMeshType);
        FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
        if (!nameGeometryType.empty())
        {
            typeGeometryType = F_ParseMeshGeometryType(nameGeometryType);
        }

        ModelMesh* pMesh = new ModelMesh(this, 
                                         nameMesh,
                                         pathMesh,
                                         typeMesh,
                                         typeGeometryType,
                                         typeVertex);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i]))
        {
            String msg = "*********************** Vulkan_012_Shadering::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("Vulkan_012_Shadering::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), pathMesh.c_str());
    }
}
Vulkan_012_Shadering::ModelMesh* Vulkan_012_Shadering::findMesh(const String& nameMesh)
{
    ModelMeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_012_Shadering::destroyTextures()
{
    size_t count = this->m_aModelTexture.size();
    for (size_t i = 0; i < count; i++)
    {
        Texture* pTexture = this->m_aModelTexture[i];
        delete pTexture;
    }
    this->m_aModelTexture.clear();
    this->m_mapModelTexture.clear();
}
void Vulkan_012_Shadering::createTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        FTextureType typeTexture = F_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
        String nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
        String pathTextures = g_TexturePaths[5 * i + 4];

        StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
        Texture* pTexture = new Texture(0,
                                        nameTexture,
                                        aPathTexture,
                                        typeTexture,
                                        g_TextureFormats[i],
                                        g_TextureFilters[i],
                                        g_TextureAddressings[i],
                                        g_TextureBorderColors[i],
                                        isRenderTarget,
                                        isGraphicsComputeShared);
        pTexture->texChunkMaxX = g_TextureAnimChunks[i * 2 + 0];
        pTexture->texChunkMaxY = g_TextureAnimChunks[i * 2 + 1];
        if (pTexture->texChunkMaxX > 0 && 
            pTexture->texChunkMaxY > 0)
        {
            pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
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

        F_LogInfo("Vulkan_012_Shadering::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
Texture* Vulkan_012_Shadering::findTexture(const String& nameTexture)
{
    TexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
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
        String nameLayout(g_DescriptorSetLayoutNames[i]);
        StringVector aLayouts = FUtilString::Split(nameLayout, "-");
        VkDescriptorSetLayout vkDescriptorSetLayout = CreateDescriptorSetLayout(nameLayout, &aLayouts);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_012_Shadering::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        F_LogInfo("Vulkan_012_Shadering::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_012_Shadering::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
StringVector* Vulkan_012_Shadering::findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout)
{
    std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
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
        destroyVkShaderModule(vkShaderModule);
    }
    this->m_aVkShaderModules.clear();
    this->m_mapVkShaderModules.clear();
}
void Vulkan_012_Shadering::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createVkShaderModule(shaderName, shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        F_LogInfo("Vulkan_012_Shadering::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                  shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_012_Shadering::findShaderModule(const String& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
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
        String nameDescriptorSetLayout(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_012_Shadering::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(nameDescriptorSetLayout, aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_012_Shadering::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_012_Shadering::findPipelineLayout(const String& namePipelineLayout)
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
            StringVector* pDescriptorSetLayoutNames = pModelObject->pPipelineGraphics->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_012_Shadering::createDescriptorSets_Custom")
            createVkDescriptorSets("DescriptorSets-" + pModelObject->nameObject, pModelObject->pPipelineGraphics->poDescriptorSetLayout, pModelObject->pPipelineGraphics->poDescriptorSets);
            for (size_t j = 0; j < count_sci; j++)
            {   
                VkWriteDescriptorSetVector descriptorWrites;
                int nIndexTextureVS = 0;
                int nIndexTextureTESC = 0;
                int nIndexTextureTESE = 0;
                int nIndexTextureFS = 0;

                size_t count_names = pDescriptorSetLayoutNames->size();
                for (size_t p = 0; p < count_names; p++)
                {
                    String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
                    if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Pass);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = pModelObject->poBuffers_ObjectCB[j];
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Object);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
                    {
                        VkDescriptorBufferInfo bufferInfo_Material = {};
                        bufferInfo_Material.buffer = pModelObject->poBuffers_materialCB[j];
                        bufferInfo_Material.offset = 0;
                        bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Material);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance)) //Instance
                    {
                        VkDescriptorBufferInfo bufferInfo_Instance = {};
                        bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[j];
                        bufferInfo_Instance.offset = 0;
                        bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Instance);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Tessellation)) //Tessellation
                    {
                        VkDescriptorBufferInfo bufferInfo_Tessellation = {};
                        bufferInfo_Tessellation.buffer = pModelObject->poBuffers_tessellationCB[j];
                        bufferInfo_Tessellation.offset = 0;
                        bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Tessellation);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Geometry)) //Geometry
                    {
                        VkDescriptorBufferInfo bufferInfo_Geometry = {};
                        bufferInfo_Geometry.buffer = pModelObject->poBuffers_geometryCB[j];
                        bufferInfo_Geometry.offset = 0;
                        bufferInfo_Geometry.range = sizeof(GeometryConstants) * MAX_OBJECT_COUNT;
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                    p,
                                                    0,
                                                    1,
                                                    bufferInfo_Geometry);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureVS)) //TextureVS
                    {
                        Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Vertex), nIndexTextureVS);
                        nIndexTextureVS ++;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                  p,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  pTexture->poTextureImageInfo);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESC))//TextureTESC
                    {
                        Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_TessellationControl), nIndexTextureTESC);
                        nIndexTextureTESC ++;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                  p,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  pTexture->poTextureImageInfo);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESE))//TextureTESE
                    {
                        Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), nIndexTextureTESE);
                        nIndexTextureTESE ++;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                  p,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  pTexture->poTextureImageInfo);
                    }
                    else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
                    {
                        Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
                        nIndexTextureFS ++;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  pModelObject->pPipelineGraphics->poDescriptorSets[j],
                                                  p,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  pTexture->poTextureImageInfo);
                    }
                    else
                    {
                        String msg = "*********************** Vulkan_012_Shadering::createDescriptorSets_Custom: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                }
                updateVkDescriptorSets(descriptorWrites);
            }
        }   
        
        //Pipeline Computes
        size_t count_comp = pModelObject->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {       
            VKPipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];

            StringVector* pDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
            F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_012_Shadering::createDescriptorSets_Custom")
            String nameDS = "DescriptorSet-" + FUtilString::SavePointI(FPointI(i,j)) + "-" + pModelObject->nameObject;
            createVkDescriptorSet(nameDS, pPipelineCompute->poDescriptorSetLayout, pPipelineCompute->poDescriptorSet);

            VkWriteDescriptorSetVector descriptorWrites;
            int nIndexTextureCS = 0;
            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t p = 0; p < count_names; p++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[p];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCopy)) //TextureCopy
                {
                    pPipelineCompute->CreateTextureCopy();

                    VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
                    bufferInfo_TextureCopy.buffer = pPipelineCompute->poBuffer_TextureCopy;
                    bufferInfo_TextureCopy.offset = 0;
                    bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                pPipelineCompute->poDescriptorSet,
                                                p,
                                                0,
                                                1,
                                                bufferInfo_TextureCopy);
                }   
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
                {
                    Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
                    nIndexTextureCS ++;
                    pPipelineCompute->pTextureSource = pTexture;
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              pPipelineCompute->poDescriptorSet,
                                              p,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              pTexture->poTextureImageInfo);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
                {
                    Texture* pTexture = pModelObject->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
                    nIndexTextureCS ++;
                    pPipelineCompute->pTextureTarget = pTexture;
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              pPipelineCompute->poDescriptorSet,
                                              p,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                              pTexture->poTextureImageInfo);
                }
                else
                {
                    String msg = "*********************** Vulkan_012_Shadering::createDescriptorSets_Custom: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }  
            updateVkDescriptorSets(descriptorWrites);
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
            VKPipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];
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
                    pPipelineCompute->pTextureCopy->texOffset.x = FMath::Rand(0, 1) * pPipelineCompute->pTextureSource->width;
                    pPipelineCompute->pTextureCopy->texOffset.y = FMath::Rand(0, 1) * pPipelineCompute->pTextureSource->height;
                    pPipelineCompute->pTextureCopy->texOffset.z = 0;
                    pPipelineCompute->pTextureCopy->texOffset.w = 0;

                    int seed = FMath::Rand(0, 10000);
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
                updateVKBuffer(0, sizeof(TextureCopyConstants), pPipelineCompute->pTextureCopy, memory);

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
                                                  FVector3(0.0f, 1.0f, 0.0f));
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
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pModelObject->objectCBs.data(), memory);
        }

        //MaterialConstants
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pModelObject->materialCBs.data(), memory);
        }

        //TessellationConstants
        if (pModelObject->isUsedTessellation)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pModelObject->tessellationCBs.data(), memory);
        }

        //GeometryConstants
        if (pModelObject->isUsedGeometry)
        {
            VkDeviceMemory& memory = pModelObject->poBuffersMemory_geometryCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(GeometryConstants) * count_object, pModelObject->geometryCBs.data(), memory);
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
            VKPipelineCompute* pPipelineCompute = pModelObject->aPipelineComputes[j];
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

            String nameObject = FUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
            if (ImGui::CollapsingHeader(nameObject.c_str()))
            {
                String nameIsShow = "Is Show - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsShow.c_str(), &pModelObject->isShow);
                String nameIsWireFrame = "Is WireFrame - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsWireFrame.c_str(), &pModelObject->isWireFrame);
                String nameIsRotate = "Is Rotate - " + pModelObject->nameObject;
                ImGui::Checkbox(nameIsRotate.c_str(), &pModelObject->isRotate);
                String nameIsTransparent = "Is Transparent - " + pModelObject->nameObject;
                bool isTransparent = pModelObject->isTransparent;
                ImGui::Checkbox(nameIsTransparent.c_str(), &isTransparent);
                String nameIsLighting = "Is Lighting - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsLighting.c_str(), &pModelObject->isLighting))
                {
                    for (int j = 0; j < pModelObject->countInstance; j++)
                    {
                        MaterialConstants& mat = pModelObject->materialCBs[j];
                        mat.lighting = pModelObject->isLighting;
                    }
                }
                
                String nameInstances = "Instance - " + pModelObject->nameObject;
                int countInstanceExt = pModelObject->countInstanceExt;
                ImGui::DragInt(nameInstances.c_str(), &countInstanceExt, 1, 0, 10);
                if (countInstanceExt != pModelObject->countInstanceExt)
                {
                    pModelObject->countInstanceExt = countInstanceExt;
                    pModelObject->countInstance = countInstanceExt * 2 + 1;
                    rebuildInstanceCBs(false);
                }

                ImGui::Text("Vertex: [%d], Index: [%d]", (int)pModelObject->pMesh->poVertexCount, (int)pModelObject->pMesh->poIndexCount);
                
                String nameWorld = "Model Object - " + pModelObject->nameObject;
                if (ImGui::CollapsingHeader(nameWorld.c_str()))
                {
                    int count_instance = pModelObject->countInstance;
                    for (int j = 0; j < count_instance; j++)
                    {
                        ObjectConstants& obj = pModelObject->objectCBs[j];
                        MaterialConstants& mat = pModelObject->materialCBs[j];

                        String nameModelInstance = nameObject + " - " + FUtilString::SaveInt(j);
                        if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                        {
                            //ObjectConstants
                            String nameObject = FUtilString::SaveInt(j) + " - Object - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameObject.c_str()))
                            {
                                const FMatrix4& mat4World = obj.g_MatWorld;
                                String nameTable = FUtilString::SaveInt(j) + " - matWorld - " + pModelObject->nameObject;
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
                            String nameMaterial = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject;
                            if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                            {
                                //factorAmbient
                                String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                {

                                }
                                ImGui::Spacing();

                                //factorDiffuse
                                String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                {

                                }
                                ImGui::Spacing();

                                //factorSpecular
                                String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                {

                                }
                                ImGui::Spacing();

                                //shininess
                                String nameShininess = "Shininess - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //alpha
                                String nameAlpha = "Alpha - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //lighting
                                String nameLighting = "Lighting - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                bool isLighting = mat.lighting == 1.0f ? true : false;
                                if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                {
                                    mat.lighting = isLighting ? 1.0f : 0.0f;
                                }

                                //Texture VS
                                {
                                    TexturePtrVector* pTextureVSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                    if (pTextureVSs != nullptr)
                                    {

                                    }
                                }
                                //Texture FS
                                {
                                    TexturePtrVector* pTextureFSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                                    if (pTextureFSs != nullptr)
                                    {
                                        size_t count_texture = pTextureFSs->size();
                                        for (size_t p = 0; p < count_texture; p++)
                                        {
                                            Texture* pTexture = (*pTextureFSs)[p];

                                            String nameMaterial_Texture = FUtilString::SaveInt(j) + " - Material - " + pModelObject->nameObject + " - TextureFS - " + FUtilString::SaveInt(p);
                                            if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                            {
                                                //texWidth
                                                String nameWidth = "Width - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int width = pTexture->width;
                                                ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                //texHeight
                                                String nameHeight = "Height - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int height = pTexture->height;
                                                ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                //texDepth
                                                String nameDepth = "Depth - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                int depth = pTexture->depth;
                                                ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                //indexTextureArray
                                                String nameIndexTextureArray = "IndexTextureArray - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (pTexture->typeTexture == F_Texture_2DArray)
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
                                                String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }
                                                //texSpeedV
                                                String nameTexSpeedV = "texSpeedV - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }
                                                //texSpeedW
                                                String nameTexSpeedW = "texSpeedW - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                {
                                                    
                                                }

                                                //texChunkMaxX
                                                String nameTexChunkMaxX = "texChunkMaxX - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxX = mat.aTexLayers[p].texChunkMaxX;
                                                ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                //texChunkMaxY
                                                String nameTexChunkMaxY = "texChunkMaxY - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkMaxY = mat.aTexLayers[p].texChunkMaxY;
                                                ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                //texChunkIndexX
                                                String nameTexChunkIndexX = "texChunkIndexX - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexX = mat.aTexLayers[p].texChunkIndexX;
                                                ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                //texChunkIndexY
                                                String nameTexChunkIndexY = "texChunkIndexY - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + pModelObject->nameObject;
                                                float fTexChunkIndexY = mat.aTexLayers[p].texChunkIndexY;
                                                ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                            }
                                        }
                                    }
                                }
                                //Texture CS
                                {
                                    TexturePtrVector* pTextureCSs = pModelObject->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                                    if (pTextureCSs != nullptr)
                                    {

                                    }
                                }

                                ImGui::Spacing();
                            }

                            //TessellationConstants
                            if (pModelObject->isUsedTessellation)
                            {
                                String nameTessellation = FUtilString::SaveInt(j) + " - Tessellation - " + pModelObject->nameObject;
                                if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                {
                                    TessellationConstants& tess = pModelObject->tessellationCBs[j];
                                    //tessLevelOuter
                                    String nameTessLevelOuter = "tessLevelOuter - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                    {
                                        
                                    }
                                    //tessLevelInner
                                    String nameTessLevelInner = "tessLevelInner - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                    {
                                        
                                    }
                                    //tessAlpha
                                    String nameTessAlpha = "tessAlpha - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                    {
                                        
                                    }
                                    //tessStrength
                                    String nameTessStrength = "tessStrength - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameTessStrength.c_str(), &tess.tessStrength, 0.025f, 0.1f, 100.0f))
                                    {
                                        
                                    }
                                    
                                    ImGui::Spacing();
                                }
                            }
                           
                            //Geometry
                            if (pModelObject->isUsedGeometry)
                            {
                                String nameGeometry = FUtilString::SaveInt(j) + " - Geometry - " + pModelObject->nameObject;
                                if (ImGui::CollapsingHeader(nameGeometry.c_str()))
                                {
                                    GeometryConstants& geometry = pModelObject->geometryCBs[j];
                                    //width
                                    String nameGeometryWidth = "width - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameGeometryWidth.c_str(), &geometry.width, 0.001f, 0.001f, 10.0f))
                                    {
                                        
                                    }
                                    //height
                                    String nameGeometryHeight = "height - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
                                    if (ImGui::DragFloat(nameGeometryHeight.c_str(), &geometry.height, 0.001f, 0.001f, 10.0f))
                                    {
                                        
                                    }
                                    //length
                                    String nameGeometryLength = "length - " + FUtilString::SaveInt(j) + " - " + pModelObject->nameObject;
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
    destroyTextures();
    destroyMeshes();

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