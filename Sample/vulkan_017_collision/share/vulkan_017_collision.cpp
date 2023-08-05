/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-07-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_017_collision.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 31; 
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name                 //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "geo_line_line",            "Pos3Color4",                           "geometry",         "Line",                     "", //geo_line_line
    "geo_line_triangle",        "Pos3Color4",                           "geometry",         "LineTriangle",             "", //geo_line_triangle
    "geo_line_quad",            "Pos3Color4",                           "geometry",         "LineQuad",                 "", //geo_line_quad
    "geo_line_grid",            "Pos3Color4",                           "geometry",         "LineGrid",                 "", //geo_line_grid
    "geo_line_circle",          "Pos3Color4",                           "geometry",         "LineCircle",               "", //geo_line_circle
    "geo_line_aabb",            "Pos3Color4",                           "geometry",         "LineAABB",                 "", //geo_line_aabb
    "geo_line_sphere",          "Pos3Color4",                           "geometry",         "LineSphere",               "", //geo_line_sphere
    "geo_line_cylinder",        "Pos3Color4",                           "geometry",         "LineCylinder",             "", //geo_line_cylinder
    "geo_line_capsule",         "Pos3Color4",                           "geometry",         "LineCapsule",              "", //geo_line_capsule
    "geo_line_cone",            "Pos3Color4",                           "geometry",         "LineCone",                 "", //geo_line_cone
    "geo_line_torus",           "Pos3Color4",                           "geometry",         "LineTorus",                "", //geo_line_torus
    
    "geo_flat_triangle",        "Pos3Color4",                           "geometry",         "FlatTriangle",             "", //geo_flat_triangle
    "geo_flat_quad",            "Pos3Color4",                           "geometry",         "FlatQuad",                 "", //geo_flat_quad
    "geo_flat_circle",          "Pos3Color4",                           "geometry",         "FlatCircle",               "", //geo_flat_circle
    "geo_flat_aabb",            "Pos3Color4",                           "geometry",         "FlatAABB",                 "", //geo_flat_aabb
    "geo_flat_sphere",          "Pos3Color4",                           "geometry",         "FlatSphere",               "", //geo_flat_sphere
    "geo_flat_cylinder",        "Pos3Color4",                           "geometry",         "FlatCylinder",             "", //geo_flat_cylinder
    "geo_flat_capsule",         "Pos3Color4",                           "geometry",         "FlatCapsule",              "", //geo_flat_capsule
    "geo_flat_cone",            "Pos3Color4",                           "geometry",         "FlatCone",                 "", //geo_flat_cone
    "geo_flat_torus",           "Pos3Color4",                           "geometry",         "FlatTorus",                "", //geo_flat_torus

    "geo_entity_triangle",      "Pos3Color4Normal3Tex2",                "geometry",         "EntityTriangle",           "", //geo_entity_triangle
    "geo_entity_quad",          "Pos3Color4Normal3Tex2",                "geometry",         "EntityQuad",               "", //geo_entity_quad
    "geo_entity_grid",          "Pos3Color4Normal3Tex2",                "geometry",         "EntityGrid",               "", //geo_entity_grid
    "geo_entity_circle",        "Pos3Color4Normal3Tex2",                "geometry",         "EntityCircle",             "", //geo_entity_circle
    "geo_entity_aabb",          "Pos3Color4Normal3Tex2",                "geometry",         "EntityAABB",               "", //geo_entity_aabb
    "geo_entity_sphere",        "Pos3Color4Normal3Tex2",                "geometry",         "EntitySphere",             "", //geo_entity_sphere
    "geo_entity_geosphere",     "Pos3Color4Normal3Tex2",                "geometry",         "EntityGeoSphere",          "", //geo_entity_geosphere
    "geo_entity_cylinder",      "Pos3Color4Normal3Tex2",                "geometry",         "EntityCylinder",           "", //geo_entity_cylinder
    "geo_entity_capsule",       "Pos3Color4Normal3Tex2",                "geometry",         "EntityCapsule",            "", //geo_entity_capsule
    "geo_entity_cone",          "Pos3Color4Normal3Tex2",                "geometry",         "EntityCone",               "", //geo_entity_cone
    "geo_entity_torus",         "Pos3Color4Normal3Tex2",                "geometry",         "EntityTorus",              "", //geo_entity_torus
    //"geo_entity_skybox",       "Pos3Color4Normal3Tex2",                "geometry",         "EntitySkyBox",             "", //geo_entity_skybox
    //"geo_entity_skydome",      "Pos3Color4Normal3Tex2",                "geometry",         "EntitySkyDome",            "", //geo_entity_skydome
    //"geo_entity_terrain",      "Pos3Color4Normal3Tex2",                "geometry",         "EntityTerrain",            "", //geo_entity_terrain
};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    false, //geo_line_line
    false, //geo_line_triangle
    false, //geo_line_quad
    false, //geo_line_grid
    false, //geo_line_circle
    false, //geo_line_aabb
    false, //geo_line_sphere
    false, //geo_line_cylinder
    false, //geo_line_capsule
    false, //geo_line_cone
    false, //geo_line_torus

    false, //geo_flat_triangle
    false, //geo_flat_quad
    false, //geo_flat_circle
    false, //geo_flat_aabb
    false, //geo_flat_sphere
    false, //geo_flat_cylinder
    false, //geo_flat_capsule
    false, //geo_flat_cone
    false, //geo_flat_torus

    false, //geo_entity_triangle
    false, //geo_entity_quad
    false, //geo_entity_grid
    false, //geo_entity_circle
    false, //geo_entity_aabb
    false, //geo_entity_sphere
    false, //geo_entity_geosphere
    false, //geo_entity_cylinder
    false, //geo_entity_capsule
    false, //geo_entity_cone
    false, //geo_entity_torus
    //false, //geo_entity_skybox
    //false, //geo_entity_skydome
    //false, //geo_entity_terrain
};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //geo_line_line
    true, //geo_line_triangle
    true, //geo_line_quad
    true, //geo_line_grid
    true, //geo_line_circle
    false, //geo_line_aabb
    false, //geo_line_sphere
    false, //geo_line_cylinder
    false, //geo_line_capsule
    false, //geo_line_cone
    false, //geo_line_torus

    true, //geo_flat_triangle
    true, //geo_flat_quad
    true, //geo_flat_circle
    false, //geo_flat_aabb
    false, //geo_flat_sphere
    false, //geo_flat_cylinder
    false, //geo_flat_capsule
    false, //geo_flat_cone
    false, //geo_flat_torus

    true, //geo_entity_triangle
    true, //geo_entity_quad
    true, //geo_entity_grid
    true, //geo_entity_circle
    false, //geo_entity_aabb
    false, //geo_entity_sphere
    false, //geo_entity_geosphere
    false, //geo_entity_cylinder
    false, //geo_entity_capsule
    false, //geo_entity_cone
    false, //geo_entity_torus
    //false, //geo_entity_skybox
    //false, //geo_entity_skydome
    //false, //geo_entity_terrain
};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //geo_line_line
    FMath::RotateX(90.0f), //geo_line_triangle
    FMath::RotateX(90.0f), //geo_line_quad
    FMath::RotateX(90.0f), //geo_line_grid
    FMath::RotateX(90.0f), //geo_line_circle
    FMath::ms_mat4Unit, //geo_line_aabb
    FMath::ms_mat4Unit, //geo_line_sphere
    FMath::ms_mat4Unit, //geo_line_cylinder
    FMath::ms_mat4Unit, //geo_line_capsule
    FMath::ms_mat4Unit, //geo_line_cone
    FMath::ms_mat4Unit, //geo_line_torus

    FMath::RotateX(90.0f), //geo_flat_triangle
    FMath::RotateX(90.0f), //geo_flat_quad
    FMath::RotateX(90.0f), //geo_flat_circle
    FMath::ms_mat4Unit, //geo_flat_aabb
    FMath::ms_mat4Unit, //geo_flat_sphere
    FMath::ms_mat4Unit, //geo_flat_cylinder
    FMath::ms_mat4Unit, //geo_flat_capsule
    FMath::ms_mat4Unit, //geo_flat_cone
    FMath::ms_mat4Unit, //geo_flat_torus

    FMath::RotateX(90.0f), //geo_entity_triangle  
    FMath::RotateX(90.0f), //geo_entity_quad 
    FMath::RotateX(90.0f), //geo_entity_grid
    FMath::RotateX(90.0f), //geo_entity_circle
    FMath::ms_mat4Unit, //geo_entity_aabb
    FMath::ms_mat4Unit, //geo_entity_sphere
    FMath::ms_mat4Unit, //geo_entity_geosphere
    FMath::ms_mat4Unit, //geo_entity_cylinder
    FMath::ms_mat4Unit, //geo_entity_capsule
    FMath::ms_mat4Unit, //geo_entity_cone
    FMath::ms_mat4Unit, //geo_entity_torus
    //FMath::ms_mat4Unit, //geo_entity_skybox
    //FMath::ms_mat4Unit, //geo_entity_skydome
    //FMath::ms_mat4Unit, //geo_entity_terrain
};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 1;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "bricks_diffuse",                   "2d",            "false",                  "false",                           "Assets/Texture/bricks_diffuse.png", //bricks_diffuse

};
static VkFormat g_TextureFormats[g_TextureCount] = 
{
    VK_FORMAT_R8G8B8A8_SRGB, //bricks_diffuse
};
static VulkanTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    Vulkan_TextureFilter_Bilinear, //bricks_diffuse
};
static VulkanTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    Vulkan_TextureAddressing_Clamp, //bricks_diffuse
};
static VulkanTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    Vulkan_TextureBorderColor_OpaqueBlack, //bricks_diffuse
};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //bricks_diffuse
};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //bricks_diffuse
};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 2;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-ObjectLineFlat",
    "Pass-Object-Material-Instance-TextureFS",
};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 4;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_editor_line",                                        "vert",              "Assets/Shader/editor_line.vert.spv", //editor_line vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////
   

    ///////////////////////////////////////// tese /////////////////////////////////////////
   

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_editor_line",                                        "frag",              "Assets/Shader/editor_line.frag.spv", //editor_line frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    

};


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 31; 
static const char* g_Object_Configs[2 * g_Object_Count] = 
{
    //Object Name                           //Mesh Name        
    "object_geo_line_line",                 "geo_line_line", //object_geo_line_line
    "object_geo_line_triangle",             "geo_line_triangle", //object_geo_line_triangle
    "object_geo_line_quad",                 "geo_line_quad", //object_geo_line_quad
    "object_geo_line_grid",                 "geo_line_grid", //object_geo_line_grid
    "object_geo_line_circle",               "geo_line_circle", //object_geo_line_circle
    "object_geo_line_aabb",                 "geo_line_aabb", //object_geo_line_aabb
    "object_geo_line_sphere",               "geo_line_sphere", //object_geo_line_sphere
    "object_geo_line_cylinder",             "geo_line_cylinder", //object_geo_line_cylinder
    "object_geo_line_capsule",              "geo_line_capsule", //object_geo_line_capsule
    "object_geo_line_cone",                 "geo_line_cone", //object_geo_line_cone
    "object_geo_line_torus",                "geo_line_torus", //object_geo_line_torus

    "object_geo_flat_triangle",             "geo_flat_triangle", //object_geo_flat_triangle
    "object_geo_flat_quad",                 "geo_flat_quad", //object_geo_flat_quad
    "object_geo_flat_circle",               "geo_flat_circle", //object_geo_flat_circle
    "object_geo_flat_aabb",                 "geo_flat_aabb", //object_geo_flat_aabb
    "object_geo_flat_sphere",               "geo_flat_sphere", //object_geo_flat_sphere
    "object_geo_flat_cylinder",             "geo_flat_cylinder", //object_geo_flat_cylinder
    "object_geo_flat_capsule",              "geo_flat_capsule", //object_geo_flat_capsule
    "object_geo_flat_cone",                 "geo_flat_cone", //object_geo_flat_cone
    "object_geo_flat_torus",                "geo_flat_torus", //object_geo_flat_torus

    "object_geo_entity_triangle",           "geo_entity_triangle", //object_geo_entity_triangle
    "object_geo_entity_quad",               "geo_entity_quad", //object_geo_entity_quad
    "object_geo_entity_grid",               "geo_entity_grid", //object_geo_entity_grid
    "object_geo_entity_circle",             "geo_entity_circle", //object_geo_entity_circle
    "object_geo_entity_aabb",               "geo_entity_aabb", //object_geo_entity_aabb
    "object_geo_entity_sphere",             "geo_entity_sphere", //object_geo_entity_sphere
    "object_geo_entity_geosphere",          "geo_entity_geosphere", //object_geo_entity_geosphere
    "object_geo_entity_cylinder",           "geo_entity_cylinder", //object_geo_entity_cylinder
    "object_geo_entity_capsule",            "geo_entity_capsule", //object_geo_entity_capsule
    "object_geo_entity_cone",               "geo_entity_cone", //object_geo_entity_cone
    "object_geo_entity_torus",              "geo_entity_torus", //object_geo_entity_torus
    //"object_geo_entity_skybox",           "geo_entity_skybox", //object_geo_entity_skybox
    //"object_geo_entity_skydome",          "geo_entity_skydome", //object_geo_entity_skydome
    //"object_geo_entity_terrain",          "geo_entity_terrain", //object_geo_entity_terrain
};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{   
    "0", //object_geo_line_line
    "0", //object_geo_line_triangle
    "0", //object_geo_line_quad
    "0", //object_geo_line_grid
    "0", //object_geo_line_circle
    "0", //object_geo_line_aabb
    "0", //object_geo_line_sphere
    "0", //object_geo_line_cylinder
    "0", //object_geo_line_capsule
    "0", //object_geo_line_cone
    "0", //object_geo_line_torus

    "0", //object_geo_flat_triangle
    "0", //object_geo_flat_quad
    "0", //object_geo_flat_circle
    "0", //object_geo_flat_aabb
    "0", //object_geo_flat_sphere
    "0", //object_geo_flat_cylinder
    "0", //object_geo_flat_capsule
    "0", //object_geo_flat_cone
    "0", //object_geo_flat_torus

    "0", //object_geo_entity_triangle
    "0", //object_geo_entity_quad
    "0", //object_geo_entity_grid
    "0", //object_geo_entity_circle
    "0", //object_geo_entity_aabb
    "0", //object_geo_entity_sphere
    "0", //object_geo_entity_geosphere
    "0", //object_geo_entity_cylinder
    "0", //object_geo_entity_capsule
    "0", //object_geo_entity_cone
    "0", //object_geo_entity_torus
    //"0", //object_geo_entity_skybox
    //"0", //object_geo_entity_skydome
    //"0", //object_geo_entity_terrain
};  

static float g_Object_InstanceGap = 1.5f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    1, //object_geo_line_line
    1, //object_geo_line_triangle
    1, //object_geo_line_quad
    1, //object_geo_line_grid
    1, //object_geo_line_circle
    1, //object_geo_line_aabb
    1, //object_geo_line_sphere
    1, //object_geo_line_cylinder
    1, //object_geo_line_capsule
    1, //object_geo_line_cone
    1, //object_geo_line_torus

    1, //object_geo_flat_triangle
    1, //object_geo_flat_quad
    1, //object_geo_flat_circle
    1, //object_geo_flat_aabb
    1, //object_geo_flat_sphere
    1, //object_geo_flat_cylinder
    1, //object_geo_flat_capsule
    1, //object_geo_flat_cone
    1, //object_geo_flat_torus

    1, //object_geo_entity_triangle 
    1, //object_geo_entity_quad 
    1, //object_geo_entity_grid 
    1, //object_geo_entity_circle 
    1, //object_geo_entity_aabb 
    1, //object_geo_entity_sphere 
    1, //object_geo_entity_geosphere 
    1, //object_geo_entity_cylinder 
    1, //object_geo_entity_capsule 
    1, //object_geo_entity_cone 
    1, //object_geo_entity_torus 
    //1, //object_geo_entity_skybox 
    //1, //object_geo_entity_skydome 
    //1, //object_geo_entity_terrain 
};
static bool g_Object_IsShows[] = 
{   
    true, //object_geo_line_line
    true, //object_geo_line_triangle
    true, //object_geo_line_quad
    true, //object_geo_line_grid
    true, //object_geo_line_circle
    true, //object_geo_line_aabb
    true, //object_geo_line_sphere
    true, //object_geo_line_cylinder
    true, //object_geo_line_capsule
    true, //object_geo_line_cone
    true, //object_geo_line_torus

    true, //object_geo_flat_triangle
    true, //object_geo_flat_quad
    true, //object_geo_flat_circle
    true, //object_geo_flat_aabb
    true, //object_geo_flat_sphere
    true, //object_geo_flat_cylinder
    true, //object_geo_flat_capsule
    true, //object_geo_flat_cone
    true, //object_geo_flat_torus

    true, //object_geo_entity_triangle
    true, //object_geo_entity_quad
    true, //object_geo_entity_grid
    true, //object_geo_entity_circle
    true, //object_geo_entity_aabb
    true, //object_geo_entity_sphere
    true, //object_geo_entity_geosphere
    true, //object_geo_entity_cylinder
    true, //object_geo_entity_capsule
    true, //object_geo_entity_cone
    true, //object_geo_entity_torus
    //true, //object_geo_entity_skybox
    //true, //object_geo_entity_skydome
    //true, //object_geo_entity_terrain'
};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_geo_line_line
    false, //object_geo_line_triangle
    false, //object_geo_line_quad
    false, //object_geo_line_grid
    false, //object_geo_line_circle
    false, //object_geo_line_aabb
    false, //object_geo_line_sphere
    false, //object_geo_line_cylinder
    false, //object_geo_line_capsule
    false, //object_geo_line_cone
    false, //object_geo_line_torus

    false, //object_geo_flat_triangle
    false, //object_geo_flat_quad
    false, //object_geo_flat_circle
    false, //object_geo_flat_aabb
    false, //object_geo_flat_sphere
    false, //object_geo_flat_cylinder
    false, //object_geo_flat_capsule
    false, //object_geo_flat_cone
    false, //object_geo_flat_torus

    false, //object_geo_entity_triangle
    false, //object_geo_entity_quad
    false, //object_geo_entity_grid
    false, //object_geo_entity_circle
    false, //object_geo_entity_aabb
    false, //object_geo_entity_sphere
    false, //object_geo_entity_geosphere
    false, //object_geo_entity_cylinder
    false, //object_geo_entity_capsule
    false, //object_geo_entity_cone
    false, //object_geo_entity_torus
    //false, //object_geo_entity_skybox
    //false, //object_geo_entity_skydome
    //false, //object_geo_entity_terrain
};
static bool g_Object_IsLightings[g_Object_Count] =
{
    false, //object_geo_line_line
    false, //object_geo_line_triangle
    false, //object_geo_line_quad
    false, //object_geo_line_grid
    false, //object_geo_line_circle
    false, //object_geo_line_aabb
    false, //object_geo_line_sphere
    false, //object_geo_line_cylinder
    false, //object_geo_line_capsule
    false, //object_geo_line_cone
    false, //object_geo_line_torus

    false, //object_geo_flat_triangle
    false, //object_geo_flat_quad
    false, //object_geo_flat_circle
    false, //object_geo_flat_aabb
    false, //object_geo_flat_sphere
    false, //object_geo_flat_cylinder
    false, //object_geo_flat_capsule
    false, //object_geo_flat_cone
    false, //object_geo_flat_torus

    true, //object_geo_entity_triangle
    true, //object_geo_entity_quad
    true, //object_geo_entity_grid
    true, //object_geo_entity_circle
    true, //object_geo_entity_aabb
    true, //object_geo_entity_sphere
    true, //object_geo_entity_geosphere
    true, //object_geo_entity_cylinder
    true, //object_geo_entity_capsule
    true, //object_geo_entity_cone
    true, //object_geo_entity_torus
    //true, //object_geo_entity_skybox
    //true, //object_geo_entity_skydome
    //true, //object_geo_entity_terrain
};
static bool g_Object_IsIndirectDraw[g_Object_Count] =
{
    false, //object_geo_line_line
    false, //object_geo_line_triangle
    false, //object_geo_line_quad
    false, //object_geo_line_grid
    false, //object_geo_line_circle
    false, //object_geo_line_aabb
    false, //object_geo_line_sphere
    false, //object_geo_line_cylinder
    false, //object_geo_line_capsule
    false, //object_geo_line_cone
    false, //object_geo_line_torus

    false, //object_geo_flat_triangle
    false, //object_geo_flat_quad
    false, //object_geo_flat_circle
    false, //object_geo_flat_aabb
    false, //object_geo_flat_sphere
    false, //object_geo_flat_cylinder
    false, //object_geo_flat_capsule
    false, //object_geo_flat_cone
    false, //object_geo_flat_torus

    false, //object_geo_entity_triangle
    false, //object_geo_entity_quad
    false, //object_geo_entity_grid
    false, //object_geo_entity_circle
    false, //object_geo_entity_aabb
    false, //object_geo_entity_sphere
    false, //object_geo_entity_geosphere
    false, //object_geo_entity_cylinder
    false, //object_geo_entity_capsule
    false, //object_geo_entity_cone
    false, //object_geo_entity_torus
    //false, //object_geo_entity_skybox
    //false, //object_geo_entity_skydome
    //false, //object_geo_entity_terrain
};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 31; 
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_geo_line_line-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_line-1
    "object_geo_line_triangle-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_triangle-1
    "object_geo_line_quad-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad-1
    "object_geo_line_grid-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_grid-1
    "object_geo_line_circle-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_circle-1
    "object_geo_line_aabb-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_aabb-1
    "object_geo_line_sphere-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_sphere-1
    "object_geo_line_cylinder-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_cylinder-1
    "object_geo_line_capsule-1",           "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_capsule-1
    "object_geo_line_cone-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_cone-1
    "object_geo_line_torus-1",             "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_torus-1

    "object_geo_flat_triangle-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_triangle-1
    "object_geo_flat_quad-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad-1
    "object_geo_flat_circle-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_circle-1
    "object_geo_flat_aabb-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_aabb-1
    "object_geo_flat_sphere-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_sphere-1
    "object_geo_flat_cylinder-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_cylinder-1
    "object_geo_flat_capsule-1",           "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_capsule-1
    "object_geo_flat_cone-1",              "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_cone-1
    "object_geo_flat_torus-1",             "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_torus-1

    "object_geo_entity_triangle-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_triangle-1
    "object_geo_entity_quad-1",            "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_quad-1
    "object_geo_entity_grid-1",            "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_grid-1
    "object_geo_entity_circle-1",          "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_circle-1
    "object_geo_entity_aabb-1",            "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_aabb-1
    "object_geo_entity_sphere-1",          "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_sphere-1
    "object_geo_entity_geosphere-1",       "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_geosphere-1
    "object_geo_entity_cylinder-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_cylinder-1
    "object_geo_entity_capsule-1",         "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_capsule-1
    "object_geo_entity_cone-1",            "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_cone-1
    "object_geo_entity_torus-1",           "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_torus-1
    //"object_geo_entity_skybox-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_skybox-1
    //"object_geo_entity_skydome-1",       "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_skydome-1
    //"object_geo_entity_terrain-1",       "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_terrain-1
};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_line-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_triangle-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_quad-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_grid-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_circle-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_aabb-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_sphere-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_cylinder-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_capsule-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_cone-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_line_torus-1
    
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_triangle-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_quad-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_circle-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_aabb-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_sphere-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_cylinder-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_capsule-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_cone-1
    "vert_editor_line",                                     "",                                             "",                                         "",                         "frag_editor_line",                                     "", //object_geo_flat_torus-1
    
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_triangle-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_quad-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_grid-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_circle-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_aabb-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_sphere-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_geosphere-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_cylinder-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_capsule-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_cone-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_torus-1
    //"vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_skybox-1
    //"vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_skydome-1
    //"vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_geo_entity_terrain-1
};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_line-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_triangle-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_quad-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_grid-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_circle-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_aabb-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_sphere-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_cylinder-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_capsule-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_cone-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_line_torus-1

    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_triangle-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_quad-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_circle-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_aabb-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_sphere-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_cylinder-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_capsule-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_cone-1
    "Pass-ObjectLineFlat",                                              "", //object_geo_flat_torus-1
    
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_triangle-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_quad-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_grid-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_circle-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_aabb-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_sphere-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_geosphere-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_cylinder-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_capsule-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_cone-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_geo_entity_torus-1
    //"Pass-Object-Material-Instance-TextureFS",                       "", //object_geo_entity_skybox-1
    //"Pass-Object-Material-Instance-TextureFS",                       "", //object_geo_entity_skydome-1
    //"Pass-Object-Material-Instance-TextureFS",                       "", //object_geo_entity_terrain-1
};
static FVector3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    FVector3(  -5,  0.0,  -8.8),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_line-1
    FVector3(  -5,  0.0,  -8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_triangle-1
    FVector3(  -5,  0.0,  -6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad-1
    FVector3(  -5,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_grid-1
    FVector3(  -5,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_circle-1
    FVector3(  -5,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_aabb-1
    FVector3(  -5,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_sphere-1
    FVector3(  -5,  0.0,   4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_cylinder-1
    FVector3(  -5,  0.0,   6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_capsule-1
    FVector3(  -5,  0.0,   8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_cone-1
    FVector3(  -5,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_torus-1

    FVector3(  5,  0.0,   -8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_triangle-1
    FVector3(  5,  0.0,   -6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad-1
    FVector3(  5,  0.0,   -4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_circle-1
    FVector3(  5,  0.0,   -2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_aabb-1
    FVector3(  5,  0.0,    0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_sphere-1
    FVector3(  5,  0.0,    2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_cylinder-1
    FVector3(  5,  0.0,    4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_capsule-1
    FVector3(  5,  0.0,    6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_cone-1
    FVector3(  5,  0.0,    8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_torus-1

    FVector3(   0,  0.0,  -8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_triangle-1
    FVector3(   0,  0.0,  -6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_quad-1
    FVector3(   0,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_grid-1
    FVector3(   0,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_circle-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_aabb-1
    FVector3(   0,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_sphere-1
    FVector3(   0,  0.0,   4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_geosphere-1
    FVector3(   0,  0.0,   6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_cylinder-1
    FVector3(   0,  0.0,   8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_capsule-1
    FVector3(   0,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_cone-1
    FVector3(   0,  0.0,  12.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_torus-1
    //FVector3(   0,  0.0,  14.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_skybox-1
    //FVector3(   0,  0.0,  16.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_skydome-1
    //FVector3(   0,  0.0,  18.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_entity_terrain-1
};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_geo_line_line-1
    false, //object_geo_line_triangle-1
    false, //object_geo_line_quad-1
    false, //object_geo_line_grid-1
    false, //object_geo_line_circle-1
    false, //object_geo_line_aabb-1
    false, //object_geo_line_sphere-1
    false, //object_geo_line_cylinder-1
    false, //object_geo_line_capsule-1
    false, //object_geo_line_cone-1
    false, //object_geo_line_torus-1

    false, //object_geo_flat_triangle-1
    false, //object_geo_flat_quad-1
    false, //object_geo_flat_circle-1
    false, //object_geo_flat_aabb-1
    false, //object_geo_flat_sphere-1
    false, //object_geo_flat_cylinder-1
    false, //object_geo_flat_capsule-1
    false, //object_geo_flat_cone-1
    false, //object_geo_flat_torus-1

    false, //object_geo_entity_triangle-1
    false, //object_geo_entity_quad-1
    false, //object_geo_entity_grid-1
    false, //object_geo_entity_circle-1
    false, //object_geo_entity_aabb-1
    false, //object_geo_entity_sphere-1
    false, //object_geo_geosphere-1
    false, //object_geo_entity_cylinder-1
    false, //object_geo_entity_capsule-1
    false, //object_geo_entity_cone-1
    false, //object_geo_entity_torus-1
    //false, //object_geo_entity_skybox-1
    //false, //object_geo_entity_skydome-1
    //false, //object_geo_entity_terrain-1
};
static bool g_ObjectRend_IsTopologyPatchLists[g_ObjectRend_Count] =
{
    false, //object_geo_line_line-1
    false, //object_geo_line_triangle-1
    false, //object_geo_line_quad-1
    false, //object_geo_line_grid-1
    false, //object_geo_line_circle-1
    false, //object_geo_line_aabb-1
    false, //object_geo_line_sphere-1
    false, //object_geo_line_cylinder-1
    false, //object_geo_line_capsule-1
    false, //object_geo_line_cone-1
    false, //object_geo_line_torus-1

    false, //object_geo_flat_triangle-1
    false, //object_geo_flat_quad-1
    false, //object_geo_flat_circle-1
    false, //object_geo_flat_aabb-1
    false, //object_geo_flat_sphere-1
    false, //object_geo_flat_cylinder-1
    false, //object_geo_flat_capsule-1
    false, //object_geo_flat_cone-1
    false, //object_geo_flat_torus-1

    false, //object_geo_entity_triangle-1
    false, //object_geo_entity_quad-1
    false, //object_geo_entity_grid-1
    false, //object_geo_entity_circle-1
    false, //object_geo_entity_aabb-1
    false, //object_geo_entity_sphere-1
    false, //object_geo_entity_geosphere-1
    false, //object_geo_entity_cylinder-1
    false, //object_geo_entity_capsule-1
    false, //object_geo_entity_cone-1
    false, //object_geo_entity_torus-1
    //false, //object_geo_entity_skybox-1
    //false, //object_geo_entity_skydome-1
    //false, //object_geo_entity_terrain-1
};
static bool g_ObjectRend_IsLine[g_ObjectRend_Count] =
{
    true, //object_geo_line_line-1
    true, //object_geo_line_triangle-1
    true, //object_geo_line_quad-1
    true, //object_geo_line_grid-1
    true, //object_geo_line_circle-1
    true, //object_geo_line_aabb-1
    true, //object_geo_line_sphere-1
    true, //object_geo_line_cylinder-1
    true, //object_geo_line_capsule-1
    true, //object_geo_line_cone-1
    true, //object_geo_line_torus-1

    false, //object_geo_flat_triangle-1
    false, //object_geo_flat_quad-1
    false, //object_geo_flat_circle-1
    false, //object_geo_flat_aabb-1
    false, //object_geo_flat_sphere-1
    false, //object_geo_flat_cylinder-1
    false, //object_geo_flat_capsule-1
    false, //object_geo_flat_cone-1
    false, //object_geo_flat_torus-1

    false, //object_geo_entity_triangle-1
    false, //object_geo_entity_quad-1
    false, //object_geo_entity_grid-1
    false, //object_geo_entity_circle-1
    false, //object_geo_entity_aabb-1
    false, //object_geo_entity_sphere-1
    false, //object_geo_entity_geosphere-1
    false, //object_geo_entity_cylinder-1
    false, //object_geo_entity_capsule-1
    false, //object_geo_entity_cone-1
    false, //object_geo_entity_torus-1
    //false, //object_geo_entity_skybox-1
    //false, //object_geo_entity_skydome-1
    //false, //object_geo_entity_terrain-1
};
static bool g_ObjectRend_IsFlat[g_ObjectRend_Count] =
{
    false, //object_geo_line_line-1
    false, //object_geo_line_triangle-1
    false, //object_geo_line_quad-1
    false, //object_geo_line_grid-1
    false, //object_geo_line_circle-1
    false, //object_geo_line_aabb-1
    false, //object_geo_line_sphere-1
    false, //object_geo_line_cylinder-1
    false, //object_geo_line_capsule-1
    false, //object_geo_line_cone-1
    false, //object_geo_line_torus-1

    true, //object_geo_flat_triangle-1
    true, //object_geo_flat_quad-1
    true, //object_geo_flat_circle-1
    true, //object_geo_flat_aabb-1
    true, //object_geo_flat_sphere-1
    true, //object_geo_flat_cylinder-1
    true, //object_geo_flat_capsule-1
    true, //object_geo_flat_cone-1
    true, //object_geo_flat_torus-1

    false, //object_geo_entity_triangle-1
    false, //object_geo_entity_quad-1
    false, //object_geo_entity_grid-1
    false, //object_geo_entity_circle-1
    false, //object_geo_entity_aabb-1
    false, //object_geo_entity_sphere-1
    false, //object_geo_entity_geosphere-1
    false, //object_geo_entity_cylinder-1
    false, //object_geo_entity_capsule-1
    false, //object_geo_entity_cone-1
    false, //object_geo_entity_torus-1
    //false, //object_geo_entity_skybox-1
    //false, //object_geo_entity_skydome-1
    //false, //object_geo_entity_terrain-1
};


/////////////////////////// ModelObjectRend /////////////////////


/////////////////////////// ModelObjectRendIndirect /////////////
void Vulkan_017_Collision::ModelObjectRendIndirect::Destroy()
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

void Vulkan_017_Collision::ModelObjectRendIndirect::CleanupSwapChain()
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

    count = this->poBuffers_ObjectCB_LineFlat.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB_LineFlat[i], this->poBuffersMemory_ObjectCB_LineFlat[i]);
    }
    this->objectCBs_LineFlat.clear();
    this->poBuffers_ObjectCB_LineFlat.clear();
    this->poBuffersMemory_ObjectCB_LineFlat.clear();

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

void Vulkan_017_Collision::ModelObjectRendIndirect::SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends)
{
    F_Assert(_aRends.size() > 0 && "Vulkan_017_Collision::ModelObjectRendIndirect::SetupVertexIndexBuffer")
    this->aRends.clear();
    this->aRends = _aRends;
    this->pRend = _aRends[0];

    //1> Vertex/Index
    this->aMeshSubs.clear();
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        this->aMeshSubs.push_back(pMeshSub);
    }

    this->vertices_Pos3Color4Normal3Tex2.clear();
    this->vertices_Pos3Color4Normal3Tangent3Tex2.clear();
    this->indices.clear();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        pMeshSub->WriteVertexData(this->vertices_Pos3Color4Normal3Tex2, this->vertices_Pos3Color4Normal3Tangent3Tex2);
        pMeshSub->WriteIndexData(this->indices);
    }
    
    //Vertex
    if (this->vertices_Pos3Color4Normal3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tex2[0];
    }
    else if (this->vertices_Pos3Color4Normal3Tangent3Tex2.size() > 0)
    {
        this->poVertexCount = this->vertices_Pos3Color4Normal3Tangent3Tex2.size();
        this->poVertexBuffer_Size = this->poVertexCount * sizeof(FVertex_Pos3Color4Normal3Tangent3Tex2);
        this->poVertexBuffer_Data = &this->vertices_Pos3Color4Normal3Tangent3Tex2[0];
    }
    else
    {
        F_Assert(false && "Vulkan_017_Collision::ModelObjectRendIndirect::SetupVertexIndexBuffer: No vertex data !")
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

void Vulkan_017_Collision::ModelObjectRendIndirect::SetupUniformIndirectCommandBuffer()
{
    VkDeviceSize bufferSize;
    size_t count_sci = this->pRend->pModelObject->pWindow->poSwapChainImages.size();

    //1> Uniform Buffer
    {
        if (!this->pRend->isGeometryLine && !this->pRend->isGeometryFlat)
        {
            //ObjectConstants
            bufferSize = sizeof(ObjectConstants) * this->objectCBs.size();
            this->poBuffers_ObjectCB.resize(count_sci);
            this->poBuffersMemory_ObjectCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB[j], this->poBuffersMemory_ObjectCB[j]);
            }
        }
        else
        {
            //LineFlatObjectConstants
            bufferSize = sizeof(LineFlatObjectConstants) * this->objectCBs_LineFlat.size();
            this->poBuffers_ObjectCB_LineFlat.resize(count_sci);
            this->poBuffersMemory_ObjectCB_LineFlat.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB_LineFlat[j], this->poBuffersMemory_ObjectCB_LineFlat[j]);
            }
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

void Vulkan_017_Collision::ModelObjectRendIndirect::UpdateUniformBuffer()
{
    this->objectCBs.clear();
    this->objectCBs_LineFlat.clear();
    this->materialCBs.clear();
    this->tessellationCBs.clear();

    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        if (!pR->isGeometryLine && !pR->isGeometryFlat)
            this->objectCBs.insert(this->objectCBs.end(), pR->objectCBs.begin(), pR->objectCBs.end());
        else
            this->objectCBs_LineFlat.insert(this->objectCBs_LineFlat.end(), pR->objectCBs_LineFlat.begin(), pR->objectCBs_LineFlat.end());
        this->materialCBs.insert(this->materialCBs.end(), pR->materialCBs.begin(), pR->materialCBs.end());
        if (pRend->isUsedTessellation)
        {
            this->tessellationCBs.insert(this->tessellationCBs.end(), pR->tessellationCBs.begin(), pR->tessellationCBs.end());
        }
    }
}

void Vulkan_017_Collision::ModelObjectRendIndirect::UpdateIndirectCommandBuffer()
{
    this->indirectCommandCBs.clear();

    int32_t vertexOffset = 0;
    uint32_t indexOffset = 0;
    uint32_t instanceOffset = 0;
    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

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



Vulkan_017_Collision::Vulkan_017_Collision(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
{
    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = true;

    this->mainLight.common.x = 0; //Directional Type
    this->mainLight.common.y = 1.0f; //Enable
    this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
    this->mainLight.direction = FVector3(0, -1, 0); //y-
}

void Vulkan_017_Collision::OnMouseLeftDown(double x, double y)
{
    VulkanWindow::OnMouseLeftDown(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseLeftDown: Mouse Left Down !");
    
}
void Vulkan_017_Collision::OnMouseLeftUp(double x, double y)
{
    VulkanWindow::OnMouseLeftUp(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseLeftUp: Mouse Left Up !");

}
void Vulkan_017_Collision::OnMouseMove(int button, double x, double y)
{
    VulkanWindow::OnMouseMove(button, x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseMove: Mouse Move !");

}
void Vulkan_017_Collision::OnMouseHover(double x, double y)
{
    VulkanWindow::OnMouseHover(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseHover: Mouse Hover !");

}

void Vulkan_017_Collision::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    if (this->poPhysicalEnabledFeatures.multiDrawIndirect)
    {
        this->m_isDrawIndirectMulti = true;
    }
    else
    {
        this->m_isDrawIndirectMulti = false;
        F_LogError("Vulkan_017_Collision::setUpEnabledFeatures: multiDrawIndirect is not supported !");
    }
}

void Vulkan_017_Collision::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_017_Collision::createCamera()
{
    this->pCamera = new FCamera();
    cameraReset();
}
void Vulkan_017_Collision::cameraReset()
{
    VulkanWindow::cameraReset();

    this->pCamera->SetPos(FVector3(0.0f, 18.0f, -13.0f));
    this->pCamera->SetEulerAngles(FVector3(55.0f, 0.0f, 0.0f));
    this->pCamera->SetFarZ(100000.0f);
}

void Vulkan_017_Collision::loadModel_Custom()
{
    createMeshes();
    createTextures();

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
                Mesh* pMesh = this->findMesh(pModelObject->nameMesh);
                F_Assert(pMesh != nullptr && "Vulkan_017_Collision::loadModel_Custom")
                pModelObject->SetMesh(pMesh);
            }
            //MeshSub Used
            {
                String nameMeshSubUsed = g_Object_MeshSubsUsed[i];
                StringVector aMeshSubUsed = FUtilString::Split(nameMeshSubUsed, ";");
                pModelObject->aMeshSubUsed.clear();
                size_t count_mesh_sub_used = aMeshSubUsed.size();
                for (size_t j = 0; j < count_mesh_sub_used; j++)
                {
                    String& name = aMeshSubUsed[j];
                    int indexMeshSub = FUtilString::ParserInt(name);
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
                F_Assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "Vulkan_017_Collision::loadModel_Custom")

                MeshSub* pMeshSub = pModelObject->pMesh->aMeshSubs[indexMeshSub];
                String nameObjectRend = g_ObjectRend_Configs[7 * nIndexObjectRend + 0];
                ModelObjectRend* pRend = new ModelObjectRend(nameObjectRend, pModelObject, pMeshSub);

                //Texture VS
                {
                    String nameTextureVS = g_ObjectRend_Configs[7 * nIndexObjectRend + 1]; //Texture VS
                    if (!nameTextureVS.empty())
                    {
                        StringVector aTextureVS = FUtilString::Split(nameTextureVS, ";");
                        size_t count_tex = aTextureVS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureVS[p];
                            Texture* pTextureVS = this->findTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), pTextureVS);
                        }
                    }
                }
                //Texture TESC
                {
                    String nameTextureTESC = g_ObjectRend_Configs[7 * nIndexObjectRend + 2]; //Texture TESC
                    if (!nameTextureTESC.empty())
                    {
                        StringVector aTextureTESC = FUtilString::Split(nameTextureTESC, ";");
                        size_t count_tex = aTextureTESC.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESC[p];
                            Texture* pTextureTESC = this->findTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), pTextureTESC);
                        }
                    }
                }
                //Texture TESE
                {
                    String nameTextureTESE = g_ObjectRend_Configs[7 * nIndexObjectRend + 3]; //Texture TESE
                    if (!nameTextureTESE.empty())
                    {
                        StringVector aTextureTESE = FUtilString::Split(nameTextureTESE, ";");
                        size_t count_tex = aTextureTESE.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureTESE[p];
                            Texture* pTextureTESE = this->findTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), pTextureTESE);
                        }
                    }
                }
                //Texture GS
                {
                    String nameTextureGS = g_ObjectRend_Configs[7 * nIndexObjectRend + 4]; //Texture GS
                    if (!nameTextureGS.empty())
                    {
                        StringVector aTextureGS = FUtilString::Split(nameTextureGS, ";");
                        size_t count_tex = aTextureGS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureGS[p];
                            Texture* pTextureGS = this->findTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureGS);
                        }
                    }
                }
                //Texture FS
                {
                    String nameTextureFS = g_ObjectRend_Configs[7 * nIndexObjectRend + 5]; //Texture FS
                    if (!nameTextureFS.empty())
                    {
                        StringVector aTextureFS = FUtilString::Split(nameTextureFS, ";");
                        size_t count_tex = aTextureFS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureFS[p];
                            Texture* pTextureFS = this->findTexture(nameTex);
                            pRend->AddTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), pTextureFS);
                        }
                    }
                }
                //Texture CS
                {
                    String nameTextureCS = g_ObjectRend_Configs[7 * nIndexObjectRend + 6]; //Texture CS
                    if (!nameTextureCS.empty())
                    {
                        StringVector aTextureCS = FUtilString::Split(nameTextureCS, ";");
                        size_t count_tex = aTextureCS.size();
                        for (size_t p = 0; p < count_tex; p++)
                        {
                            String nameTex = aTextureCS[p];
                            Texture* pTextureCS = this->findTexture(nameTex);
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
                    StringVector aDescriptorSetLayout = FUtilString::Split(nameDescriptorSetLayout, ";");
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
                pRend->isGeometryLine = g_ObjectRend_IsLine[nIndexObjectRend];
                pRend->isGeometryFlat = g_ObjectRend_IsFlat[nIndexObjectRend];
                if (pRend->isGeometryLine)
                {
                    pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    pRend->cfg_vkPolygonMode = VK_POLYGON_MODE_LINE;
                    pRend->cfg_vkCullModeFlagBits = VK_CULL_MODE_NONE;
                    //pRend->cfg_LineWidth = 5.0f;
                }
                if (pRend->isGeometryFlat)
                {
                    // pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
                    // pRend->cfg_vkPolygonMode = VK_POLYGON_MODE_LINE;
                    pRend->cfg_vkCullModeFlagBits = VK_CULL_MODE_NONE;
                }

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
void Vulkan_017_Collision::createIndirectCommands()
{

}

void Vulkan_017_Collision::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_017_Collision::rebuildInstanceCBs(bool isCreateVkBuffer)
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
        pRend->objectCBs_LineFlat.clear();
        pRend->materialCBs.clear();
        for (int j = 0; j < count_instance; j++)
        {
            if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
            {
                //ObjectConstants
                ObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                            g_ObjectRend_Tranforms[3 * i + 1],
                                                            g_ObjectRend_Tranforms[3 * i + 2]);
                pRend->objectCBs.push_back(objectConstants);
                pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }
            else
            {
                //LineFlatObjectConstants
                LineFlatObjectConstants objectConstants;
                objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                            g_ObjectRend_Tranforms[3 * i + 1],
                                                            g_ObjectRend_Tranforms[3 * i + 2]);
                pRend->objectCBs_LineFlat.push_back(objectConstants);
                pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            }

            //MaterialConstants
            {
                MaterialConstants materialConstants;
                materialConstants.factorAmbient = FMath::RandomColor(false);
                materialConstants.factorDiffuse = FMath::RandomColor(false);
                materialConstants.factorSpecular = FMath::RandomColor(false);
                materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
                materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
                materialConstants.lighting = isObjectLighting;
                //Texture VS
                {
                    TexturePtrVector* pTextureVSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                    if (pTextureVSs != nullptr)
                    {

                    }
                }
                //Texture FS
                {
                    TexturePtrVector* pTextureFSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                    if (pTextureFSs != nullptr)
                    {

                    }
                }
                //Texture CS
                {
                    TexturePtrVector* pTextureCSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
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
            if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
            {
                bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                pRend->poBuffers_ObjectCB.resize(count_sci);
                pRend->poBuffersMemory_ObjectCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB[j], pRend->poBuffersMemory_ObjectCB[j]);
                }
            }
            else
            {
                bufferSize = sizeof(ObjectConstants) * MAX_OBJECT_LINEFLAT_COUNT;
                pRend->poBuffers_ObjectCB_LineFlat.resize(count_sci);
                pRend->poBuffersMemory_ObjectCB_LineFlat.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB_LineFlat[j], pRend->poBuffersMemory_ObjectCB_LineFlat[j]);
                }
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

void Vulkan_017_Collision::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();
}   
void Vulkan_017_Collision::createGraphicsPipeline_Custom()
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
        if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  m_mapVkShaderModules,
                                                  pRend->aShaderStageCreateInfos_Graphics))
        {
            String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pRend->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            pRend->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline(pRend->aShaderStageCreateInfos_Graphics,
                                                                                      pRend->isUsedTessellation, 0, 3,
                                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                      pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, this->cfg_LineWidth,
                                                                                      pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                      pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                      pRend->cfg_isBlend, pRend->cfg_BlendColorFactorSrc, pRend->cfg_BlendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                      pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                      pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_017_Collision::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

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
                                                                            pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, pRend->cfg_vkCullModeFlagBits, this->cfg_LineWidth,
                                                                            isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                            pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                            pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                            pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createGraphicsPipeline_Custom: Failed to create pipeline graphics: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_017_Collision::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pRend->nameObjectRend.c_str());
        }
    }
}
void Vulkan_017_Collision::createComputePipeline_Custom()
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
        if (!CreatePipelineShaderStageCreateInfos(nameShaderComp,
                                                  m_mapVkShaderModules,
                                                  pRend->aShaderStageCreateInfos_Computes,
                                                  pRend->mapShaderStageCreateInfos_Computes))
        {
            String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pRend->aShaderStageCreateInfos_Computes.size())
        {
            String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            PipelineCompute* p = pRend->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pRend->aShaderStageCreateInfos_Computes[j];

            p->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline(shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "Vulkan_017_Collision::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}

void Vulkan_017_Collision::destroyMeshes()
{
    size_t count = this->m_aModelMesh.size();
    for (size_t i = 0; i < count; i++)
    {
        Mesh* pMesh = this->m_aModelMesh[i];
        delete pMesh;
    }
    this->m_aModelMesh.clear();
    this->m_mapModelMesh.clear();
}
void Vulkan_017_Collision::createMeshes()
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

        Mesh* pMesh = new Mesh(this, 
                               nameMesh,
                               pathMesh,
                               typeMesh,
                               typeVertex,
                               typeGeometryType,
                               nullptr);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i]))
        {
            String msg = "Vulkan_017_Collision::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("Vulkan_017_Collision::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
    }
}
Vulkan_017_Collision::Mesh* Vulkan_017_Collision::findMesh(const String& nameMesh)
{
    MeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_017_Collision::destroyTextures()
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
void Vulkan_017_Collision::createTextures()
{
    for (int i = 0; i < g_TextureCount; i++)
    {
        String nameTexture = g_TexturePaths[5 * i + 0];
        String nameType = g_TexturePaths[5 * i + 1];
        VulkanTextureType typeTexture = Util_ParseTextureType(nameType);
        String nameIsRenderTarget = g_TexturePaths[5 * i + 2];
        bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
        String nameIsGraphicsComputeShared = g_TexturePaths[5 * i + 3];
        bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
        String pathTextures = g_TexturePaths[5 * i + 4];

        StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
        Texture* pTexture = new Texture(this, 
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

        F_LogInfo("Vulkan_017_Collision::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
Vulkan_017_Collision::Texture* Vulkan_017_Collision::findTexture(const String& nameTexture)
{
    TexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_017_Collision::destroyDescriptorSetLayouts()
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
void Vulkan_017_Collision::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameLayout(g_DescriptorSetLayoutNames[i]);
        StringVector aLayouts = FUtilString::Split(nameLayout, "-");
        VkDescriptorSetLayout vkDescriptorSetLayout = CreateDescriptorSetLayout(nameLayout, &aLayouts);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "Vulkan_017_Collision::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        F_LogInfo("Vulkan_017_Collision::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_017_Collision::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
StringVector* Vulkan_017_Collision::findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout)
{
    std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
}


void Vulkan_017_Collision::destroyShaderModules()
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
void Vulkan_017_Collision::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createVkShaderModule(shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        F_LogInfo("Vulkan_017_Collision::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                  shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_017_Collision::findShaderModule(const String& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   


void Vulkan_017_Collision::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkPipelineLayout(this->m_aVkPipelineLayouts[i]);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_017_Collision::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameDescriptorSetLayout(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_017_Collision::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_017_Collision::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_017_Collision::findPipelineLayout(const String& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}



void Vulkan_017_Collision::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //Pipeline Graphics
        {
            createVkDescriptorSets(pRend->pPipelineGraphics->poDescriptorSetLayout, pRend->pPipelineGraphics->poDescriptorSets);
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
            createVkDescriptorSets(pModelObject->pRendIndirect->pRend->pPipelineGraphics->poDescriptorSetLayout, pModelObject->pRendIndirect->poDescriptorSets);
            createDescriptorSets_Graphics(pModelObject->pRendIndirect->poDescriptorSets, pModelObject->pRendIndirect->pRend, pModelObject->pRendIndirect);
        }
    }
}
void Vulkan_017_Collision::createDescriptorSets_Graphics(VkDescriptorSetVector& poDescriptorSets, 
                                                        ModelObjectRend* pRend, 
                                                        ModelObjectRendIndirect* pRendIndirect)
{
    StringVector* pDescriptorSetLayoutNames = pRend->pPipelineGraphics->poDescriptorSetLayoutNames;
    F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_017_Collision::createDescriptorSets_Graphics")
    size_t count_ds = poDescriptorSets.size();
    for (size_t j = 0; j < count_ds; j++)
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
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Pass);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
            {
                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB[j] : pRend->poBuffers_ObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Object);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat)) //ObjectLineFlat
            {
                VkDescriptorBufferInfo bufferInfo_ObjectLineFlat = {};
                bufferInfo_ObjectLineFlat.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB_LineFlat[j] : pRend->poBuffers_ObjectCB_LineFlat[j];
                bufferInfo_ObjectLineFlat.offset = 0;
                bufferInfo_ObjectLineFlat.range = sizeof(LineFlatObjectConstants) * MAX_OBJECT_LINEFLAT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_ObjectLineFlat);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_materialCB[j] : pRend->poBuffers_materialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
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
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Instance);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Tessellation)) //Tessellation
            {
                VkDescriptorBufferInfo bufferInfo_Tessellation = {};
                bufferInfo_Tessellation.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_tessellationCB[j] : pRend->poBuffers_tessellationCB[j];
                bufferInfo_Tessellation.offset = 0;
                bufferInfo_Tessellation.range = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Tessellation);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureVS)) //TextureVS
            {
                Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Vertex), nIndexTextureVS);
                nIndexTextureVS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESC))//TextureTESC
            {
                Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationControl), nIndexTextureTESC);
                nIndexTextureTESC ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESE))//TextureTESE
            {
                Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_TessellationEvaluation), nIndexTextureTESE);
                nIndexTextureTESE ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
            {
                Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Fragment), nIndexTextureFS);
                nIndexTextureFS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else
            {
                String msg = "Vulkan_017_Collision::createDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        updateVkDescriptorSets(descriptorWrites);
    }
}
void Vulkan_017_Collision::createDescriptorSets_Compute(PipelineCompute* pPipelineCompute, 
                                                           ModelObjectRend* pRend)
{
    StringVector* pDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
    F_Assert(pDescriptorSetLayoutNames != nullptr && "Vulkan_017_Collision::createDescriptorSets_Compute")
    createVkDescriptorSet(pPipelineCompute->poDescriptorSetLayout, pPipelineCompute->poDescriptorSet);

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
            Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
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
            Texture* pTexture = pRend->GetTexture(Util_GetShaderTypeName(Vulkan_Shader_Compute), nIndexTextureCS);
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
            String msg = "Vulkan_017_Collision::createDescriptorSets_Compute: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }  
    updateVkDescriptorSets(descriptorWrites);
}

void Vulkan_017_Collision::updateCompute_Custom(VkCommandBuffer& commandBuffer)
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

void Vulkan_017_Collision::updateCBs_Custom()
{
    //1> Object Rend
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_object = 0;
        if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
            count_object = pRend->objectCBs.size();
        else
            count_object = pRend->objectCBs_LineFlat.size();
        for (size_t j = 0; j < count_object; j++)
        {
            if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
            {
                //ObjectConstants
                ObjectConstants& objectCB = pRend->objectCBs[j];
                if (pRend->pModelObject->isRotate || pRend->isRotate || this->cfg_isRotate)
                {
                    objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j], 
                                                    time * glm::radians(90.0f), 
                                                    FVector3(0.0f, 1.0f, 0.0f));
                }
                else
                {
                    objectCB.g_MatWorld = pRend->instanceMatWorld[j];
                }
            }
            else
            {
                //LineFlatObjectConstants
                LineFlatObjectConstants& objectCB = pRend->objectCBs_LineFlat[j];
                if (pRend->pModelObject->isRotate || pRend->isRotate || this->cfg_isRotate)
                {
                    objectCB.g_MatWorld = glm::rotate(pRend->instanceMatWorld[j], 
                                                    time * glm::radians(90.0f), 
                                                    FVector3(0.0f, 1.0f, 0.0f));
                }
                else
                {
                    objectCB.g_MatWorld = pRend->instanceMatWorld[j];
                }
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
        if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                memcpy(data, pRend->objectCBs.data(), sizeof(ObjectConstants) * count_object);
            vkUnmapMemory(this->poDevice, memory);
        }
        else
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB_LineFlat[this->poSwapChainImageIndex];
            void* data;
            vkMapMemory(this->poDevice, memory, 0, sizeof(LineFlatObjectConstants) * count_object, 0, &data);
                memcpy(data, pRend->objectCBs_LineFlat.data(), sizeof(LineFlatObjectConstants) * count_object);
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
            
            if (!pRendIndirect->pRend->isGeometryLine && !pRendIndirect->pRend->isGeometryFlat)
            {
                //ObjectConstants
                size_t count_object = pRendIndirect->objectCBs.size();
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count_object, 0, &data);
                    memcpy(data, pRendIndirect->objectCBs.data(), sizeof(ObjectConstants) * count_object);
                vkUnmapMemory(this->poDevice, memory);
            }
            else
            {
                //LineFlatObjectConstants
                size_t count_object = pRendIndirect->objectCBs_LineFlat.size();
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB_LineFlat[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(LineFlatObjectConstants) * count_object, 0, &data);
                    memcpy(data, pRendIndirect->objectCBs_LineFlat.data(), sizeof(LineFlatObjectConstants) * count_object);
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

void Vulkan_017_Collision::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
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

bool Vulkan_017_Collision::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_017_Collision", &windowOpened, 0);
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
void Vulkan_017_Collision::modelConfig()
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
            String nameObject = FUtilString::SaveInt(i) + " - " + pModelObject->nameObject;
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
                                    FMatrix4* pMat4World = nullptr;
                                    if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
                                    {
                                        ObjectConstants& obj = pRend->objectCBs[p];
                                        pMat4World = &obj.g_MatWorld;
                                    }
                                    else
                                    {
                                        LineFlatObjectConstants& obj = pRend->objectCBs_LineFlat[p];
                                        pMat4World = &obj.g_MatWorld;
                                    }
                                    
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const FMatrix4& mat4World = *pMat4World;
                                            String nameTable = FUtilString::SaveInt(p) + " - matWorld - " + nameObjectRend;
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
                                        String nameMaterial = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameMaterial.c_str()))
                                        {
                                            //factorAmbient
                                            String nameFactorAmbient = "FactorAmbient - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorAmbient.c_str(), (float*)&mat.factorAmbient))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorDiffuse
                                            String nameFactorDiffuse = "FactorDiffuse - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorDiffuse.c_str(), (float*)&mat.factorDiffuse))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //factorSpecular
                                            String nameFactorSpecular = "FactorSpecular - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::ColorEdit4(nameFactorSpecular.c_str(), (float*)&mat.factorSpecular))
                                            {

                                            }
                                            ImGui::Spacing();

                                            //shininess
                                            String nameShininess = "Shininess - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameShininess.c_str(), &mat.shininess, 0.01f, 0.01f, 100.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //alpha
                                            String nameAlpha = "Alpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            if (ImGui::DragFloat(nameAlpha.c_str(), &mat.alpha, 0.001f, 0.0f, 1.0f))
                                            {
                                                
                                            }
                                            ImGui::Spacing();

                                            //lighting
                                            String nameLighting = "Lighting - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isLighting = mat.lighting == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameLighting.c_str(), &isLighting))
                                            {
                                                mat.lighting = isLighting ? 1.0f : 0.0f;
                                            }

                                            //Texture VS
                                            {
                                                TexturePtrVector* pTextureVSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                TexturePtrVector* pTextureFSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Fragment));
                                                if (pTextureFSs != nullptr)
                                                {
                                                    size_t count_texture = pTextureFSs->size();
                                                    for (size_t q = 0; q < count_texture; q++)
                                                    {
                                                        Texture* pTexture = (*pTextureFSs)[q];

                                                        String nameMaterial_Texture = FUtilString::SaveInt(p) + " - Material - " + nameObjectRend + " - TextureFS - " + FUtilString::SaveInt(q);
                                                        if (ImGui::CollapsingHeader(nameMaterial_Texture.c_str()))
                                                        {
                                                            //texWidth
                                                            String nameWidth = "Width - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int width = pTexture->width;
                                                            ImGui::DragInt(nameWidth.c_str(), &width, 1, 0, 4096);

                                                            //texHeight
                                                            String nameHeight = "Height - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int height = pTexture->height;
                                                            ImGui::DragInt(nameHeight.c_str(), &height, 1, 0, 4096);

                                                            //texDepth
                                                            String nameDepth = "Depth - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            int depth = pTexture->depth;
                                                            ImGui::DragInt(nameDepth.c_str(), &depth, 1, 0, 4096);

                                                            //indexTextureArray
                                                            String nameIndexTextureArray = "IndexTextureArray - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
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
                                                            String nameTexSpeedU = "TexSpeedU - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedU.c_str(), &mat.aTexLayers[p].texSpeedU, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedV
                                                            String nameTexSpeedV = "texSpeedV - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedV.c_str(), &mat.aTexLayers[p].texSpeedV, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }
                                                            //texSpeedW
                                                            String nameTexSpeedW = "texSpeedW - " + FUtilString::SaveInt(j) + " - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                            if (ImGui::DragFloat(nameTexSpeedW.c_str(), &mat.aTexLayers[p].texSpeedW, 0.01f, 0.0f, 100.0f))
                                                            {
                                                                
                                                            }

                                                            //texChunkMaxX
                                                            String nameTexChunkMaxX = "texChunkMaxX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxX = mat.aTexLayers[q].texChunkMaxX;
                                                            ImGui::DragFloat(nameTexChunkMaxX.c_str(), &fTexChunkMaxX, 1.0f, 1.0f, 100.0f);
                                                            //texChunkMaxY
                                                            String nameTexChunkMaxY = "texChunkMaxY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkMaxY = mat.aTexLayers[q].texChunkMaxY;
                                                            ImGui::DragFloat(nameTexChunkMaxY.c_str(), &fTexChunkMaxY, 1.0f, 1.0f, 100.0f);
                                                            //texChunkIndexX
                                                            String nameTexChunkIndexX = "texChunkIndexX - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexX = mat.aTexLayers[q].texChunkIndexX;
                                                            ImGui::DragFloat(nameTexChunkIndexX.c_str(), &fTexChunkIndexX, 1.0f, 0.0f, 100.0f);
                                                            //texChunkIndexY
                                                            String nameTexChunkIndexY = "texChunkIndexY - " + FUtilString::SaveInt(p) + " - " + FUtilString::SaveInt(q) + " - " + nameObjectRend;
                                                            float fTexChunkIndexY = mat.aTexLayers[q].texChunkIndexY;
                                                            ImGui::DragFloat(nameTexChunkIndexY.c_str(), &fTexChunkIndexY, 1.0f, 0.0f, 100.0f);
                                                        }
                                                    }
                                                }
                                            }
                                            //Texture CS
                                            {
                                                TexturePtrVector* pTextureCSs = pRend->GetTextures(Util_GetShaderTypeName(Vulkan_Shader_Compute));
                                                if (pTextureCSs != nullptr)
                                                {

                                                }
                                            }

                                            ImGui::Spacing();
                                        }

                                        //TessellationConstants
                                        String nameTessellation = FUtilString::SaveInt(p) + " - Tessellation - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameTessellation.c_str()))
                                        {
                                            if (pRend->isUsedTessellation)
                                            {
                                                TessellationConstants& tess = pRend->tessellationCBs[j];
                                                //tessLevelOuter
                                                String nameTessLevelOuter = "tessLevelOuter - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelOuter.c_str(), &tess.tessLevelOuter, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessLevelInner
                                                String nameTessLevelInner = "tessLevelInner - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessLevelInner.c_str(), &tess.tessLevelInner, 0.1f, 0.1f, 500.0f))
                                                {
                                                    
                                                }
                                                //tessAlpha
                                                String nameTessAlpha = "tessAlpha - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::DragFloat(nameTessAlpha.c_str(), &tess.tessAlpha, 0.05f, 0.0f, 1.0f))
                                                {
                                                    
                                                }
                                                //tessStrength
                                                String nameTessStrength = "tessStrength - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
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

void Vulkan_017_Collision::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_017_Collision::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
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
void Vulkan_017_Collision::drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
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
void Vulkan_017_Collision::drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect)
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

void Vulkan_017_Collision::drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
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
void Vulkan_017_Collision::drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
{
    ModelObject* pModelObject = pRend->pModelObject;
    MeshSub* pMeshSub = pRend->pMeshSub;

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

void Vulkan_017_Collision::cleanupCustom()
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
    this->m_mapModelObjects.clear();
    this->m_aModelObjectRends_All.clear();
    this->m_aModelObjectRends_Opaque.clear();
    this->m_aModelObjectRends_Transparent.clear();
}

void Vulkan_017_Collision::cleanupSwapChain_Custom()
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

void Vulkan_017_Collision::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}