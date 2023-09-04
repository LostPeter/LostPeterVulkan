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
static const int g_MeshCount = 47;
static const char* g_MeshPaths[7 * g_MeshCount] =
{
    //Mesh Name                     //Vertex Type                 //Mesh Type         //Mesh Path   //Mesh Geometry Type        //Mesh Geometry Param                                           //Mesh Geometry Vertex Is Update
    "geo_line_line_2d",             "Pos3Color4",                 "geometry",         "",           "LineLine2D",               "0.5;0.9;0.7;0.9;1;1;1;1",                                      "false", //geo_line_line_2d
    "geo_line_triangle_2d",         "Pos3Color4",                 "geometry",         "",           "LineTriangle2D",           "0.6;0.8;0.5;0.6;0.7;0.6;1;1;1;1",                              "false", //geo_line_triangle_2d
    "geo_line_quad_2d",             "Pos3Color4",                 "geometry",         "",           "LineQuad2D",               "0.5;0.5;0.5;0.3;0.7;0.3;0.7;0.5;1;1;1;1",                      "false", //geo_line_quad_2d
    "geo_line_grid_2d",             "Pos3Color4",                 "geometry",         "",           "LineGrid2D",               "0.5;0.2;0.5;0.0;0.7;0.0;0.7;0.2;10;10;1;1;1;1",                "false", //geo_line_grid_2d
    "geo_line_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",           "LineQuad2D",               "0.55;-0.1;0.5;-0.3;0.65;-0.3;0.7;-0.1;1;1;1;1",                "false", //geo_line_quad_convex_2d
    "geo_line_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",           "LineQuad2D",               "0.5;-0.4;0.5;-0.6;0.7;-0.6;0.55;-0.5;1;1;1;1",                  "false", //geo_line_quad_concave_2d
    "geo_line_circle_2d",           "Pos3Color4",                 "geometry",         "",           "LineCircle2D",             "0.6;-0.8;1.0;0.0;0.1;1280;720;50;true;1;1;1;1",                "true", //geo_line_circle_2d

    "geo_flat_triangle_2d",         "Pos3Color4",                 "geometry",         "",           "FlatTriangle2D",           "0.85;0.8;0.75;0.6;0.95;0.6;1;1;1;1",                           "false", //geo_flat_triangle_2d
    "geo_flat_quad_2d",             "Pos3Color4",                 "geometry",         "",           "FlatQuad2D",               "0.75;0.5;0.75;0.3;0.95;0.3;0.95;0.5;1;1;1;1",                  "false", //geo_flat_quad_2d
    "geo_flat_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",           "FlatQuad2D",               "0.8;0.2;0.75;0.0;0.9;0.0;0.95;0.2;1;1;1;1",                    "false", //geo_flat_quad_convex_2d
    "geo_flat_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",           "FlatQuad2D",               "0.75;-0.1;0.75;-0.3;0.95;-0.3;0.8;-0.2;1;1;1;1",               "false", //geo_flat_quad_concave_2d
    "geo_flat_circle_2d",           "Pos3Color4",                 "geometry",         "",           "FlatCircle2D",             "0.85;-0.5;1.0;0.0;0.1;1280;720;50;1;1;1;1",                    "true", //geo_flat_circle_2d

    "geo_line_line_3d",             "Pos3Color4",                 "geometry",         "",           "LineLine3D",               "0;0;0;1;0;0;1;1;1;1",                                          "false", //geo_line_line_3d
    "geo_line_triangle_3d",         "Pos3Color4",                 "geometry",         "",           "LineTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_line_triangle_3d
    "geo_line_quad_3d",             "Pos3Color4",                 "geometry",         "",           "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_line_quad_3d
    "geo_line_grid_3d",             "Pos3Color4",                 "geometry",         "",           "LineGrid3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;10;10;1;1;1;1",    "false", //geo_line_grid_3d
    "geo_line_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",           "LineQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_line_quad_convex_3d
    "geo_line_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",           "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_line_quad_concave_3d
    "geo_line_circle_3d",           "Pos3Color4",                 "geometry",         "",           "LineCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;true;1;1;1;1",                      "false", //geo_line_circle_3d
    "geo_line_aabb_3d",             "Pos3Color4",                 "geometry",         "",           "LineAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_line_aabb_3d
    "geo_line_sphere_3d",           "Pos3Color4",                 "geometry",         "",           "LineSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_line_sphere_3d
    "geo_line_cylinder_3d",         "Pos3Color4",                 "geometry",         "",           "LineCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;true;1;1;1;1",                        "false", //geo_line_cylinder_3d
    "geo_line_capsule_3d",          "Pos3Color4",                 "geometry",         "",           "LineCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;1;1;1;1",                              "false", //geo_line_capsule_3d
    "geo_line_cone_3d",             "Pos3Color4",                 "geometry",         "",           "LineCone3D",               "0;0;0;0;1;0;0.5;1;50;1;1;1;1",                                 "false", //geo_line_cone_3d
    "geo_line_torus_3d",            "Pos3Color4",                 "geometry",         "",           "LineTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_line_torus_3d
    
    "geo_flat_triangle_3d",         "Pos3Color4",                 "geometry",         "",           "FlatTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_flat_triangle_3d
    "geo_flat_quad_3d",             "Pos3Color4",                 "geometry",         "",           "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_flat_quad_3d
    "geo_flat_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",           "FlatQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_flat_quad_convex_3d
    "geo_flat_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",           "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_flat_quad_concave_3d
    "geo_flat_circle_3d",           "Pos3Color4",                 "geometry",         "",           "FlatCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;1;1;1;1",                           "false", //geo_flat_circle_3d
    "geo_flat_aabb_3d",             "Pos3Color4",                 "geometry",         "",           "FlatAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_flat_aabb_3d
    "geo_flat_sphere_3d",           "Pos3Color4",                 "geometry",         "",           "FlatSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_flat_sphere_3d
    "geo_flat_cylinder_3d",         "Pos3Color4",                 "geometry",         "",           "FlatCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;30;1;1;1;1",                          "false", //geo_flat_cylinder_3d
    "geo_flat_capsule_3d",          "Pos3Color4",                 "geometry",         "",           "FlatCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;30;1;1;1;1",                           "false", //geo_flat_capsule_3d
    "geo_flat_cone_3d",             "Pos3Color4",                 "geometry",         "",           "FlatCone3D",               "0;0;0;0;1;0;0.5;1;50;30;1;1;1;1",                              "false", //geo_flat_cone_3d
    "geo_flat_torus_3d",            "Pos3Color4",                 "geometry",         "",           "FlatTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_flat_torus_3d

    "geo_entity_triangle",          "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityTriangle",           "false;false",                                                  "false", //geo_entity_triangle
    "geo_entity_quad",              "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityQuad",               "false;false;0;0;1;1;0",                                        "false", //geo_entity_quad
    "geo_entity_grid",              "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityGrid",               "false;false;1;1;10;10",                                        "false", //geo_entity_grid
    "geo_entity_circle",            "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityCircle",             "false;false;0.5;100",                                          "false", //geo_entity_circle
    "geo_entity_aabb",              "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityAABB",               "false;false;1;1;1;0",                                          "false", //geo_entity_aabb
    "geo_entity_sphere",            "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntitySphere",             "false;false;0.5;30;30",                                        "false", //geo_entity_sphere
    "geo_entity_geosphere",         "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityGeoSphere",          "false;false;0.5;5",                                            "false", //geo_entity_geosphere
    "geo_entity_cylinder",          "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityCylinder",           "false;false;0.5;0.5;1;0;50;30",                                "false", //geo_entity_cylinder
    "geo_entity_capsule",           "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityCapsule",            "false;false;0.5;1;0;10;50;30",                                 "false", //geo_entity_capsule
    "geo_entity_cone",              "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityCone",               "false;false;0.5;1;0;50;30",                                    "false", //geo_entity_cone
    "geo_entity_torus",             "Pos3Color4Normal3Tex2",      "geometry",         "",           "EntityTorus",              "false;false;0.5;0.2;50;20",                                    "false", //geo_entity_torus
    //"geo_entity_skybox",        "Pos3Color4Normal3Tex2",      "geometry",         "",          "EntitySkyBox",             "false;false;",                                                 "false", //geo_entity_skybox
    //"geo_entity_skydome",       "Pos3Color4Normal3Tex2",      "geometry",         "",          "EntitySkyDome",            "false;false;",                                                 "false", //geo_entity_skydome
    //"geo_entity_terrain",       "Pos3Color4Normal3Tex2",      "geometry",         "",          "EntityTerrain",            "false;false;",                                                 "false", //geo_entity_terrain
};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    false, //geo_line_line_2d
    false, //geo_line_triangle_2d
    false, //geo_line_quad_2d
    false, //geo_line_grid_2d
    false, //geo_line_quad_convex_2d
    false, //geo_line_quad_concave_2d
    false, //geo_line_circle_2d

    false, //geo_flat_triangle_2d
    false, //geo_flat_quad_2d
    false, //geo_flat_quad_convex_2d
    false, //geo_flat_quad_concave_2d
    false, //geo_flat_circle_2d

    false, //geo_line_line_3d
    false, //geo_line_triangle_3d
    false, //geo_line_quad_3d
    false, //geo_line_grid_3d
    false, //geo_line_quad_convex_3d
    false, //geo_line_quad_concave_3d
    false, //geo_line_circle_3d
    false, //geo_line_aabb_3d
    false, //geo_line_sphere_3d
    false, //geo_line_cylinder_3d
    false, //geo_line_capsule_3d
    false, //geo_line_cone_3d
    false, //geo_line_torus_3d

    false, //geo_flat_triangle_3d
    false, //geo_flat_quad_3d
    false, //geo_flat_quad_convex_3d
    false, //geo_flat_quad_concave_3d
    false, //geo_flat_circle_3d
    false, //geo_flat_aabb_3d
    false, //geo_flat_sphere_3d
    false, //geo_flat_cylinder_3d
    false, //geo_flat_capsule_3d
    false, //geo_flat_cone_3d
    false, //geo_flat_torus_3d

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
    false, //geo_line_line_2d
    false, //geo_line_triangle_2d
    false, //geo_line_quad_2d
    false, //geo_line_grid_2d
    false, //geo_line_quad_convex_2d
    false, //geo_line_quad_concave_2d
    false, //geo_line_circle_2d

    false, //geo_flat_triangle_2d
    false, //geo_flat_quad_2d
    false, //geo_flat_quad_convex_2d
    false, //geo_flat_quad_concave_2d
    false, //geo_flat_circle_2d

    false, //geo_line_line_3d
    true, //geo_line_triangle_3d
    true, //geo_line_quad_3d
    true, //geo_line_grid_3d
    true, //geo_line_quad_convex_3d
    true, //geo_line_quad_concave_3d
    true, //geo_line_circle_3d
    false, //geo_line_aabb_3d
    false, //geo_line_sphere_3d
    false, //geo_line_cylinder_3d
    false, //geo_line_capsule_3d
    false, //geo_line_cone_3d
    false, //geo_line_torus_3d

    true, //geo_flat_triangle_3d
    true, //geo_flat_quad_3d
    true, //geo_flat_quad_convex_3d
    true, //geo_flat_quad_concave_3d
    true, //geo_flat_circle_3d
    false, //geo_flat_aabb_3d
    false, //geo_flat_sphere_3d
    false, //geo_flat_cylinder_3d
    false, //geo_flat_capsule_3d
    false, //geo_flat_cone_3d
    false, //geo_flat_torus_3d

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
    FMath::ms_mat4Unit, //geo_line_line_2d
    FMath::ms_mat4Unit, //geo_line_triangle_2d
    FMath::ms_mat4Unit, //geo_line_quad_2d
    FMath::ms_mat4Unit, //geo_line_grid_2d
    FMath::ms_mat4Unit, //geo_line_quad_convex_2d
    FMath::ms_mat4Unit, //geo_line_quad_concave_2d
    FMath::ms_mat4Unit, //geo_line_circle_2d

    FMath::ms_mat4Unit, //geo_flat_triangle_2d
    FMath::ms_mat4Unit, //geo_flat_quad_2d
    FMath::ms_mat4Unit, //geo_flat_quad_convex_2d
    FMath::ms_mat4Unit, //geo_flat_quad_concave_2d
    FMath::ms_mat4Unit, //geo_flat_circle_2d

    FMath::ms_mat4Unit, //geo_line_line_3d
    FMath::RotateX(90.0f), //geo_line_triangle_3d
    FMath::RotateX(90.0f), //geo_line_quad_3d
    FMath::RotateX(90.0f), //geo_line_grid_3d
    FMath::RotateX(90.0f), //geo_line_quad_convex_3d
    FMath::RotateX(90.0f), //geo_line_quad_concave_3d
    FMath::RotateX(90.0f), //geo_line_circle_3d
    FMath::ms_mat4Unit, //geo_line_aabb_3d
    FMath::ms_mat4Unit, //geo_line_sphere_3d
    FMath::ms_mat4Unit, //geo_line_cylinder_3d
    FMath::ms_mat4Unit, //geo_line_capsule_3d
    FMath::ms_mat4Unit, //geo_line_cone_3d
    FMath::ms_mat4Unit, //geo_line_torus_3d

    FMath::RotateX(90.0f), //geo_flat_triangle_3d
    FMath::RotateX(90.0f), //geo_flat_quad_3d
    FMath::RotateX(90.0f), //geo_flat_quad_convex_3d
    FMath::RotateX(90.0f), //geo_flat_quad_concave_3d
    FMath::RotateX(90.0f), //geo_flat_circle_3d
    FMath::ms_mat4Unit, //geo_flat_aabb_3d
    FMath::ms_mat4Unit, //geo_flat_sphere_3d
    FMath::ms_mat4Unit, //geo_flat_cylinder_3d
    FMath::ms_mat4Unit, //geo_flat_capsule_3d
    FMath::ms_mat4Unit, //geo_flat_cone_3d
    FMath::ms_mat4Unit, //geo_flat_torus_3d

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
static const int g_DescriptorSetLayoutCount = 3;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-ObjectLineFlat2D",
    "Pass-ObjectLineFlat3D",
    "Pass-Object-Material-Instance-TextureFS",
};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 6;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_editor_line_2d",                                     "vert",              "Assets/Shader/editor_line_2d.vert.spv", //editor_line_2d vert
    "vert_editor_line_3d",                                     "vert",              "Assets/Shader/editor_line_3d.vert.spv", //editor_line_3d vert

    ///////////////////////////////////////// tesc /////////////////////////////////////////
   

    ///////////////////////////////////////// tese /////////////////////////////////////////
   

    ///////////////////////////////////////// geom /////////////////////////////////////////

    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_editor_line_2d",                                     "frag",              "Assets/Shader/editor_line_2d.frag.spv", //editor_line_2d frag
    "frag_editor_line_3d",                                     "frag",              "Assets/Shader/editor_line_3d.frag.spv", //editor_line_3d frag

    ///////////////////////////////////////// comp /////////////////////////////////////////
    

};


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 47; 
static const char* g_Object_Configs[2 * g_Object_Count] = 
{
    //Object Name                           //Mesh Name        
    "object_geo_line_line_2d",              "geo_line_line_2d", //object_geo_line_line_2d
    "object_geo_line_triangle_2d",          "geo_line_triangle_2d", //object_geo_line_triangle_2d
    "object_geo_line_quad_2d",              "geo_line_quad_2d", //object_geo_line_quad_2d
    "object_geo_line_grid_2d",              "geo_line_grid_2d", //object_geo_line_grid_2d
    "object_geo_line_quad_convex_2d",       "geo_line_quad_convex_2d", //object_geo_line_quad_convex_2d
    "object_geo_line_quad_concave_2d",      "geo_line_quad_concave_2d", //object_geo_line_quad_concave_2d
    "object_geo_line_circle_2d",            "geo_line_circle_2d", //object_geo_line_circle_2d

    "object_geo_flat_triangle_2d",          "geo_flat_triangle_2d", //object_geo_flat_triangle_2d
    "object_geo_flat_quad_2d",              "geo_flat_quad_2d", //object_geo_flat_quad_2d
    "object_geo_flat_quad_convex_2d",       "geo_flat_quad_convex_2d", //object_geo_flat_quad_convex_2d
    "object_geo_flat_quad_concave_2d",      "geo_flat_quad_concave_2d", //object_geo_flat_quad_concave_2d
    "object_geo_flat_circle_2d",            "geo_flat_circle_2d", //object_geo_flat_circle_2d

    "object_geo_line_line_3d",              "geo_line_line_3d", //object_geo_line_line_3d
    "object_geo_line_triangle_3d",          "geo_line_triangle_3d", //object_geo_line_triangle_3d
    "object_geo_line_quad_3d",              "geo_line_quad_3d", //object_geo_line_quad_3d
    "object_geo_line_grid_3d",              "geo_line_grid_3d", //object_geo_line_grid_3d
    "object_geo_line_quad_convex_3d",       "geo_line_quad_convex_3d", //object_geo_line_quad_convex_3d
    "object_geo_line_quad_concave_3d",      "geo_line_quad_concave_3d", //object_geo_line_quad_concave_3d
    "object_geo_line_circle_3d",            "geo_line_circle_3d", //object_geo_line_circle_3d
    "object_geo_line_aabb_3d",              "geo_line_aabb_3d", //object_geo_line_aabb_3d
    "object_geo_line_sphere_3d",            "geo_line_sphere_3d", //object_geo_line_sphere_3d
    "object_geo_line_cylinder_3d",          "geo_line_cylinder_3d", //object_geo_line_cylinder_3d
    "object_geo_line_capsule_3d",           "geo_line_capsule_3d", //object_geo_line_capsule_3d
    "object_geo_line_cone_3d",              "geo_line_cone_3d", //object_geo_line_cone_3d
    "object_geo_line_torus_3d",             "geo_line_torus_3d", //object_geo_line_torus_3d

    "object_geo_flat_triangle_3d",          "geo_flat_triangle_3d", //object_geo_flat_triangle_3d
    "object_geo_flat_quad_3d",              "geo_flat_quad_3d", //object_geo_flat_quad_3d
    "object_geo_flat_quad_convex_3d",       "geo_flat_quad_convex_3d", //object_geo_flat_quad_convex_3d
    "object_geo_flat_quad_concave_3d",      "geo_flat_quad_concave_3d", //object_geo_flat_quad_concave_3d
    "object_geo_flat_circle_3d",            "geo_flat_circle_3d", //object_geo_flat_circle_3d
    "object_geo_flat_aabb_3d",              "geo_flat_aabb_3d", //object_geo_flat_aabb_3d
    "object_geo_flat_sphere_3d",            "geo_flat_sphere_3d", //object_geo_flat_sphere_3d
    "object_geo_flat_cylinder_3d",          "geo_flat_cylinder_3d", //object_geo_flat_cylinder_3d
    "object_geo_flat_capsule_3d",           "geo_flat_capsule_3d", //object_geo_flat_capsule_3d
    "object_geo_flat_cone_3d",              "geo_flat_cone_3d", //object_geo_flat_cone_3d
    "object_geo_flat_torus_3d",             "geo_flat_torus_3d", //object_geo_flat_torus_3d

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
    "0", //object_geo_line_line_2d
    "0", //object_geo_line_triangle_2d
    "0", //object_geo_line_quad_2d
    "0", //object_geo_line_grid_2d
    "0", //object_geo_line_quad_convex_2d
    "0", //object_geo_line_quad_concave_2d
    "0", //object_geo_line_circle_2d

    "0", //object_geo_flat_triangle_2d
    "0", //object_geo_flat_quad_2d
    "0", //object_geo_flat_quad_convex_2d
    "0", //object_geo_flat_quad_concave_2d
    "0", //object_geo_flat_circle_2d

    "0", //object_geo_line_line_3d
    "0", //object_geo_line_triangle_3d
    "0", //object_geo_line_quad_3d
    "0", //object_geo_line_grid_3d
    "0", //object_geo_line_quad_convex_3d
    "0", //object_geo_line_quad_concave_3d
    "0", //object_geo_line_circle_3d
    "0", //object_geo_line_aabb_3d
    "0", //object_geo_line_sphere_3d
    "0", //object_geo_line_cylinder_3d
    "0", //object_geo_line_capsule_3d
    "0", //object_geo_line_cone_3d
    "0", //object_geo_line_torus_3d

    "0", //object_geo_flat_triangle_3d
    "0", //object_geo_flat_quad_3d
    "0", //object_geo_flat_quad_convex_3d
    "0", //object_geo_flat_quad_concave_3d
    "0", //object_geo_flat_circle_3d
    "0", //object_geo_flat_aabb_3d
    "0", //object_geo_flat_sphere_3d
    "0", //object_geo_flat_cylinder_3d
    "0", //object_geo_flat_capsule_3d
    "0", //object_geo_flat_cone_3d
    "0", //object_geo_flat_torus_3d

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
    0, //object_geo_line_line_2d
    0, //object_geo_line_triangle_2d
    0, //object_geo_line_quad_2d
    0, //object_geo_line_grid_2d
    0, //object_geo_line_quad_convex_2d
    0, //object_geo_line_quad_concave_2d
    0, //object_geo_line_circle_2d

    0, //object_geo_flat_triangle_2d
    0, //object_geo_flat_quad_2d
    0, //object_geo_flat_quad_convex_2d
    0, //object_geo_flat_quad_concave_2d
    0, //object_geo_flat_circle_2d

    1, //object_geo_line_line_3d
    1, //object_geo_line_triangle_3d
    1, //object_geo_line_quad_3d
    1, //object_geo_line_grid_3d
    1, //object_geo_line_quad_convex_3d
    1, //object_geo_line_quad_concave_3d
    1, //object_geo_line_circle_3d
    1, //object_geo_line_aabb_3d
    1, //object_geo_line_sphere_3d
    1, //object_geo_line_cylinder_3d
    1, //object_geo_line_capsule_3d
    1, //object_geo_line_cone_3d
    1, //object_geo_line_torus_3d

    1, //object_geo_flat_triangle_3d
    1, //object_geo_flat_quad_3d
    1, //object_geo_flat_quad_convex_3d
    1, //object_geo_flat_quad_concave_3d
    1, //object_geo_flat_circle_3d
    1, //object_geo_flat_aabb_3d
    1, //object_geo_flat_sphere_3d
    1, //object_geo_flat_cylinder_3d
    1, //object_geo_flat_capsule_3d
    1, //object_geo_flat_cone_3d
    1, //object_geo_flat_torus_3d

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
    true, //object_geo_line_line_2d
    true, //object_geo_line_triangle_2d
    true, //object_geo_line_quad_2d
    true, //object_geo_line_grid_2d
    true, //object_geo_line_quad_convex_2d
    true, //object_geo_line_quad_concave_2d
    true, //object_geo_line_circle_2d

    true, //object_geo_flat_triangle_2d
    true, //object_geo_flat_quad_2d
    true, //object_geo_flat_quad_convex_2d
    true, //object_geo_flat_quad_concave_2d
    true, //object_geo_flat_circle_2d

    true, //object_geo_line_line_3d
    true, //object_geo_line_triangle_3d
    true, //object_geo_line_quad_3d
    true, //object_geo_line_grid_3d
    true, //object_geo_line_quad_convex_3d
    true, //object_geo_line_quad_concave_3d
    true, //object_geo_line_circle_3d
    true, //object_geo_line_aabb_3d
    true, //object_geo_line_sphere_3d
    true, //object_geo_line_cylinder_3d
    true, //object_geo_line_capsule_3d
    true, //object_geo_line_cone_3d
    true, //object_geo_line_torus_3d

    true, //object_geo_flat_triangle_3d
    true, //object_geo_flat_quad_3d
    true, //object_geo_flat_quad_convex_3d
    true, //object_geo_flat_quad_concave_3d
    true, //object_geo_flat_circle_3d
    true, //object_geo_flat_aabb_3d
    true, //object_geo_flat_sphere_3d
    true, //object_geo_flat_cylinder_3d
    true, //object_geo_flat_capsule_3d
    true, //object_geo_flat_cone_3d
    true, //object_geo_flat_torus_3d

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
    false, //object_geo_line_line_2d
    false, //object_geo_line_triangle_2d
    false, //object_geo_line_quad_2d
    false, //object_geo_line_grid_2d
    false, //object_geo_line_quad_convex_2d
    false, //object_geo_line_quad_concave_2d
    false, //object_geo_line_circle_2d

    false, //object_geo_flat_triangle_2d
    false, //object_geo_flat_quad_2d
    false, //object_geo_flat_quad_convex_2d
    false, //object_geo_flat_quad_concave_2d
    false, //object_geo_flat_circle_2d

    false, //object_geo_line_line_3d
    false, //object_geo_line_triangle_3d
    false, //object_geo_line_quad_3d
    false, //object_geo_line_grid_3d
    false, //object_geo_line_quad_convex_3d
    false, //object_geo_line_quad_concave_3d
    false, //object_geo_line_circle_3d
    false, //object_geo_line_aabb_3d
    false, //object_geo_line_sphere_3d
    false, //object_geo_line_cylinder_3d
    false, //object_geo_line_capsule_3d
    false, //object_geo_line_cone_3d
    false, //object_geo_line_torus_3d

    false, //object_geo_flat_triangle_3d
    false, //object_geo_flat_quad_3d
    false, //object_geo_flat_quad_convex_3d
    false, //object_geo_flat_quad_concave_3d
    false, //object_geo_flat_circle_3d
    false, //object_geo_flat_aabb_3d
    false, //object_geo_flat_sphere_3d
    false, //object_geo_flat_cylinder_3d
    false, //object_geo_flat_capsule_3d
    false, //object_geo_flat_cone_3d
    false, //object_geo_flat_torus_3d

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
    false, //object_geo_line_line_2d
    false, //object_geo_line_triangle_2d
    false, //object_geo_line_quad_2d
    false, //object_geo_line_grid_2d
    false, //object_geo_line_quad_convex_2d
    false, //object_geo_line_quad_concave_2d
    false, //object_geo_line_circle_2d

    false, //object_geo_flat_triangle_2d
    false, //object_geo_flat_quad_2d
    false, //object_geo_flat_quad_convex_2d
    false, //object_geo_flat_quad_concave_2d
    false, //object_geo_flat_circle_2d

    false, //object_geo_line_line_3d
    false, //object_geo_line_triangle_3d
    false, //object_geo_line_quad_3d
    false, //object_geo_line_grid_3d
    false, //object_geo_line_quad_convex_3d
    false, //object_geo_line_quad_concave_3d
    false, //object_geo_line_circle_3d
    false, //object_geo_line_aabb_3d
    false, //object_geo_line_sphere_3d
    false, //object_geo_line_cylinder_3d
    false, //object_geo_line_capsule_3d
    false, //object_geo_line_cone_3d
    false, //object_geo_line_torus_3d

    false, //object_geo_flat_triangle_3d
    false, //object_geo_flat_quad_3d
    false, //object_geo_flat_quad_convex_3d
    false, //object_geo_flat_quad_concave_3d
    false, //object_geo_flat_circle_3d
    false, //object_geo_flat_aabb_3d
    false, //object_geo_flat_sphere_3d
    false, //object_geo_flat_cylinder_3d
    false, //object_geo_flat_capsule_3d
    false, //object_geo_flat_cone_3d
    false, //object_geo_flat_torus_3d

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
    false, //object_geo_line_line_2d
    false, //object_geo_line_triangle_2d
    false, //object_geo_line_quad_2d
    false, //object_geo_line_grid_2d
    false, //object_geo_line_quad_convex_2d
    false, //object_geo_line_quad_concave_2d
    false, //object_geo_line_circle_2d

    false, //object_geo_flat_triangle_2d
    false, //object_geo_flat_quad_2d
    false, //object_geo_flat_quad_convex_2d
    false, //object_geo_flat_quad_concave_2d
    false, //object_geo_flat_circle_2d

    false, //object_geo_line_line_3d
    false, //object_geo_line_triangle_3d
    false, //object_geo_line_quad_3d
    false, //object_geo_line_grid_3d
    false, //object_geo_line_quad_convex_3d
    false, //object_geo_line_quad_concave_3d
    false, //object_geo_line_circle_3d
    false, //object_geo_line_aabb_3d
    false, //object_geo_line_sphere_3d
    false, //object_geo_line_cylinder_3d
    false, //object_geo_line_capsule_3d
    false, //object_geo_line_cone_3d
    false, //object_geo_line_torus_3d

    false, //object_geo_flat_triangle_3d
    false, //object_geo_flat_quad_3d
    false, //object_geo_flat_quad_convex_3d
    false, //object_geo_flat_quad_concave_3d
    false, //object_geo_flat_circle_3d
    false, //object_geo_flat_aabb_3d
    false, //object_geo_flat_sphere_3d
    false, //object_geo_flat_cylinder_3d
    false, //object_geo_flat_capsule_3d
    false, //object_geo_flat_cone_3d
    false, //object_geo_flat_torus_3d

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
static const int g_ObjectRend_Count = 47; 
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                      //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_geo_line_line_2d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_line_2d-1
    "object_geo_line_triangle_2d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_triangle_2d-1
    "object_geo_line_quad_2d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_2d-1
    "object_geo_line_grid_2d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_grid_2d-1
    "object_geo_line_quad_convex_2d-1",     "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_convex_2d-1
    "object_geo_line_quad_concave_2d-1",    "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_concave_2d-1
    "object_geo_line_circle_2d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_circle_2d-1

    "object_geo_flat_triangle_2d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_triangle_2d-1
    "object_geo_flat_quad_2d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_2d-1
    "object_geo_flat_quad_convex_2d-1",     "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_convex_2d-1
    "object_geo_flat_quad_concave_2d-1",    "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_concave_2d-1
    "object_geo_flat_circle_2d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_circle_2d-1
    
    "object_geo_line_line_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_line_3d-1
    "object_geo_line_triangle_3d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_triangle_3d-1
    "object_geo_line_quad_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_3d-1
    "object_geo_line_grid_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_grid_3d-1
    "object_geo_line_quad_convex_3d-1",     "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_convex_3d-1
    "object_geo_line_quad_concave_3d-1",    "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_quad_concave_3d-1
    "object_geo_line_circle_3d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_circle_3d-1
    "object_geo_line_aabb_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_aabb_3d-1
    "object_geo_line_sphere_3d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_sphere_3d-1
    "object_geo_line_cylinder_3d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_cylinder_3d-1
    "object_geo_line_capsule_3d-1",         "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_capsule_3d-1
    "object_geo_line_cone_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_cone_3d-1
    "object_geo_line_torus_3d-1",           "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_line_torus_3d-1

    "object_geo_flat_triangle_3d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_triangle_3d-1
    "object_geo_flat_quad_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_3d-1
    "object_geo_flat_quad_convex_3d-1",     "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_convex_3d-1
    "object_geo_flat_quad_concave_3d-1",    "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_quad_concave_3d-1
    "object_geo_flat_circle_3d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_circle_3d-1
    "object_geo_flat_aabb_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_aabb_3d-1
    "object_geo_flat_sphere_3d-1",          "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_sphere_3d-1
    "object_geo_flat_cylinder_3d-1",        "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_cylinder_3d-1
    "object_geo_flat_capsule_3d-1",         "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_capsule_3d-1
    "object_geo_flat_cone_3d-1",            "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_cone_3d-1
    "object_geo_flat_torus_3d-1",           "",                     "",                              "",                         "",                    "",                                                                             "", //object_geo_flat_torus_3d-1

    "object_geo_entity_triangle-1",         "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_triangle-1
    "object_geo_entity_quad-1",             "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_quad-1
    "object_geo_entity_grid-1",             "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_grid-1
    "object_geo_entity_circle-1",           "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_circle-1
    "object_geo_entity_aabb-1",             "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_aabb-1
    "object_geo_entity_sphere-1",           "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_sphere-1
    "object_geo_entity_geosphere-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_geosphere-1
    "object_geo_entity_cylinder-1",         "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_cylinder-1
    "object_geo_entity_capsule-1",          "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_capsule-1
    "object_geo_entity_cone-1",             "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_cone-1
    "object_geo_entity_torus-1",            "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_torus-1
    //"object_geo_entity_skybox-1",         "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_skybox-1
    //"object_geo_entity_skydome-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_skydome-1
    //"object_geo_entity_terrain-1",        "",                     "",                              "",                         "",                    "bricks_diffuse",                                                               "", //object_geo_entity_terrain-1
};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_line_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_triangle_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_quad_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_grid_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_quad_convex_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_quad_concave_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_line_circle_2d-1

    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_flat_triangle_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_flat_quad_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_flat_quad_convex_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_flat_quad_concave_2d-1
    "vert_editor_line_2d",                                  "",                                             "",                                         "",                         "frag_editor_line_2d",                                  "", //object_geo_flat_circle_2d-1
    
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_line_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_triangle_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_quad_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_grid_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_quad_convex_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_quad_concave_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_circle_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_aabb_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_sphere_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_cylinder_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_capsule_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_cone_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_line_torus_3d-1
    
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_triangle_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_quad_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_quad_convex_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_quad_concave_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_circle_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_aabb_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_sphere_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_cylinder_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_capsule_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_cone_3d-1
    "vert_editor_line_3d",                                  "",                                             "",                                         "",                         "frag_editor_line_3d",                                  "", //object_geo_flat_torus_3d-1
    
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
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_line_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_triangle_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_quad_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_grid_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_quad_convex_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_quad_concave_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_line_circle_2d-1

    "Pass-ObjectLineFlat2D",                                            "", //object_geo_flat_triangle_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_flat_quad_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_flat_quad_convex_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_flat_quad_concave_2d-1
    "Pass-ObjectLineFlat2D",                                            "", //object_geo_flat_circle_2d-1
    
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_line_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_triangle_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_quad_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_grid_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_quad_convex_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_quad_concave_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_circle_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_aabb_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_sphere_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_cylinder_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_capsule_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_cone_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_line_torus_3d-1

    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_triangle_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_quad_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_quad_convex_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_quad_concave_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_circle_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_aabb_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_sphere_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_cylinder_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_capsule_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_cone_3d-1
    "Pass-ObjectLineFlat3D",                                            "", //object_geo_flat_torus_3d-1
    
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
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_line_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_triangle_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_grid_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_convex_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_concave_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_circle_2d-1

    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_triangle_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_convex_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_concave_2d-1
    FVector3(   0,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_circle_2d-1

    FVector3(  -5,  0.0,  -8.8),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_line_3d-1
    FVector3(  -5,  0.0,  -8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_triangle_3d-1
    FVector3(  -5,  0.0,  -6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_3d-1
    FVector3(  -5,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_grid_3d-1
    FVector3(  -5,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_convex_3d-1
    FVector3(  -5,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_quad_concave_3d-1
    FVector3(  -5,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_circle_3d-1
    FVector3(  -5,  0.0,   4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_aabb_3d-1
    FVector3(  -5,  0.0,   6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_sphere_3d-1
    FVector3(  -5,  0.0,   8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_cylinder_3d-1
    FVector3(  -5,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_capsule_3d-1
    FVector3(  -5,  0.0,  12.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_cone_3d-1
    FVector3(  -5,  0.0,  14.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_line_torus_3d-1

    FVector3(   5,  0.0,  -8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_triangle_3d-1
    FVector3(   5,  0.0,  -6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_3d-1
    FVector3(   5,  0.0,  -4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_convex_3d-1
    FVector3(   5,  0.0,  -2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_quad_concave_3d-1
    FVector3(   5,  0.0,   0.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_circle_3d-1
    FVector3(   5,  0.0,   2.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_aabb_3d-1
    FVector3(   5,  0.0,   4.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_sphere_3d-1
    FVector3(   5,  0.0,   6.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_cylinder_3d-1
    FVector3(   5,  0.0,   8.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_capsule_3d-1
    FVector3(   5,  0.0,  10.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_cone_3d-1
    FVector3(   5,  0.0,  12.0),    FVector3(     0,  0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_geo_flat_torus_3d-1

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
    false, //object_geo_line_line_2d-1
    false, //object_geo_line_triangle_2d-1
    false, //object_geo_line_quad_2d-1
    false, //object_geo_line_grid_2d-1
    false, //object_geo_line_quad_convex_2d-1
    false, //object_geo_line_quad_concave_2d-1
    false, //object_geo_line_circle_2d-1

    false, //object_geo_flat_triangle_2d-1
    false, //object_geo_flat_quad_2d-1
    false, //object_geo_flat_quad_convex_2d-1
    false, //object_geo_flat_quad_concave_2d-1
    false, //object_geo_flat_circle_2d-1

    false, //object_geo_line_line_3d-1
    false, //object_geo_line_triangle_3d-1
    false, //object_geo_line_quad_3d-1
    false, //object_geo_line_grid_3d-1
    false, //object_geo_line_quad_convex_3d-1
    false, //object_geo_line_quad_concave_3d-1
    false, //object_geo_line_circle_3d-1
    false, //object_geo_line_aabb_3d-1
    false, //object_geo_line_sphere_3d-1
    false, //object_geo_line_cylinder_3d-1
    false, //object_geo_line_capsule_3d-1
    false, //object_geo_line_cone_3d-1
    false, //object_geo_line_torus_3d-1

    false, //object_geo_flat_triangle_3d-1
    false, //object_geo_flat_quad_3d-1
    false, //object_geo_flat_quad_convex_3d-1
    false, //object_geo_flat_quad_concave_3d-1
    false, //object_geo_flat_circle_3d-1
    false, //object_geo_flat_aabb_3d-1
    false, //object_geo_flat_sphere_3d-1
    false, //object_geo_flat_cylinder_3d-1
    false, //object_geo_flat_capsule_3d-1
    false, //object_geo_flat_cone_3d-1
    false, //object_geo_flat_torus_3d-1

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
    false, //object_geo_line_line_2d-1
    false, //object_geo_line_triangle_2d-1
    false, //object_geo_line_quad_2d-1
    false, //object_geo_line_grid_2d-1
    false, //object_geo_line_quad_convex_2d-1
    false, //object_geo_line_quad_concave_2d-1
    false, //object_geo_line_circle_2d-1

    false, //object_geo_flat_triangle_2d-1
    false, //object_geo_flat_quad_2d-1
    false, //object_geo_flat_quad_convex_2d-1
    false, //object_geo_flat_quad_concave_2d-1
    false, //object_geo_flat_circle_2d-1

    false, //object_geo_line_line_3d-1
    false, //object_geo_line_triangle_3d-1
    false, //object_geo_line_quad_3d-1
    false, //object_geo_line_grid_3d-1
    false, //object_geo_line_quad_convex_3d-1
    false, //object_geo_line_quad_concave_3d-1
    false, //object_geo_line_circle_3d-1
    false, //object_geo_line_aabb_3d-1
    false, //object_geo_line_sphere_3d-1
    false, //object_geo_line_cylinder_3d-1
    false, //object_geo_line_capsule_3d-1
    false, //object_geo_line_cone_3d-1
    false, //object_geo_line_torus_3d-1

    false, //object_geo_flat_triangle_3d-1
    false, //object_geo_flat_quad_3d-1
    false, //object_geo_flat_quad_convex_3d-1
    false, //object_geo_flat_quad_concave_3d-1
    false, //object_geo_flat_circle_3d-1
    false, //object_geo_flat_aabb_3d-1
    false, //object_geo_flat_sphere_3d-1
    false, //object_geo_flat_cylinder_3d-1
    false, //object_geo_flat_capsule_3d-1
    false, //object_geo_flat_cone_3d-1
    false, //object_geo_flat_torus_3d-1

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
static bool g_ObjectRend_Is3D[g_ObjectRend_Count] =
{
    false, //object_geo_line_line_2d-1
    false, //object_geo_line_triangle_2d-1
    false, //object_geo_line_quad_2d-1
    false, //object_geo_line_grid_2d-1
    false, //object_geo_line_quad_convex_2d-1
    false, //object_geo_line_quad_concave_2d-1
    false, //object_geo_line_circle_2d-1

    false, //object_geo_flat_triangle_2d-1
    false, //object_geo_flat_quad_2d-1
    false, //object_geo_flat_quad_convex_2d-1
    false, //object_geo_flat_quad_concave_2d-1
    false, //object_geo_flat_circle_2d-1

    true, //object_geo_line_line_3d-1
    true, //object_geo_line_triangle_3d-1
    true, //object_geo_line_quad_3d-1
    true, //object_geo_line_grid_3d-1
    true, //object_geo_line_quad_convex_3d-1
    true, //object_geo_line_quad_concave_3d-1
    true, //object_geo_line_circle_3d-1
    true, //object_geo_line_aabb_3d-1
    true, //object_geo_line_sphere_3d-1
    true, //object_geo_line_cylinder_3d-1
    true, //object_geo_line_capsule_3d-1
    true, //object_geo_line_cone_3d-1
    true, //object_geo_line_torus_3d-1

    true, //object_geo_flat_triangle_3d-1
    true, //object_geo_flat_quad_3d-1
    true, //object_geo_flat_quad_convex_3d-1
    true, //object_geo_flat_quad_concave_3d-1
    true, //object_geo_flat_circle_3d-1
    true, //object_geo_flat_aabb_3d-1
    true, //object_geo_flat_sphere_3d-1
    true, //object_geo_flat_cylinder_3d-1
    true, //object_geo_flat_capsule_3d-1
    true, //object_geo_flat_cone_3d-1
    true, //object_geo_flat_torus_3d-1

    true, //object_geo_entity_triangle-1
    true, //object_geo_entity_quad-1
    true, //object_geo_entity_grid-1
    true, //object_geo_entity_circle-1
    true, //object_geo_entity_aabb-1
    true, //object_geo_entity_sphere-1
    true, //object_geo_entity_geosphere-1
    true, //object_geo_entity_cylinder-1
    true, //object_geo_entity_capsule-1
    true, //object_geo_entity_cone-1
    true, //object_geo_entity_torus-1
    //true, //object_geo_entity_skybox-1
    //true, //object_geo_entity_skydome-1
    //true, //object_geo_entity_terrain-1
};
static bool g_ObjectRend_IsLine[g_ObjectRend_Count] =
{
    true, //object_geo_line_line_2d-1
    true, //object_geo_line_triangle_2d-1
    true, //object_geo_line_quad_2d-1
    true, //object_geo_line_grid_2d-1
    true, //object_geo_line_quad_convex_2d-1
    true, //object_geo_line_quad_concave_2d-1
    true, //object_geo_line_circle_2d-1

    false, //object_geo_flat_triangle_2d-1
    false, //object_geo_flat_quad_2d-1
    false, //object_geo_flat_quad_convex_2d-1
    false, //object_geo_flat_quad_concave_2d-1
    false, //object_geo_flat_circle_2d-1

    true, //object_geo_line_line_3d-1
    true, //object_geo_line_triangle_3d-1
    true, //object_geo_line_quad_3d-1
    true, //object_geo_line_grid_3d-1
    true, //object_geo_line_quad_convex_3d-1
    true, //object_geo_line_quad_concave_3d-1
    true, //object_geo_line_circle_3d-1
    true, //object_geo_line_aabb_3d-1
    true, //object_geo_line_sphere_3d-1
    true, //object_geo_line_cylinder_3d-1
    true, //object_geo_line_capsule_3d-1
    true, //object_geo_line_cone_3d-1
    true, //object_geo_line_torus_3d-1

    false, //object_geo_flat_triangle_3d-1
    false, //object_geo_flat_quad_3d-1
    false, //object_geo_flat_quad_convex_3d-1
    false, //object_geo_flat_quad_concave_3d-1
    false, //object_geo_flat_circle_3d-1
    false, //object_geo_flat_aabb_3d-1
    false, //object_geo_flat_sphere_3d-1
    false, //object_geo_flat_cylinder_3d-1
    false, //object_geo_flat_capsule_3d-1
    false, //object_geo_flat_cone_3d-1
    false, //object_geo_flat_torus_3d-1

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
    false, //object_geo_line_line_2d-1
    false, //object_geo_line_triangle_2d-1
    false, //object_geo_line_quad_2d-1
    false, //object_geo_line_grid_2d-1
    false, //object_geo_line_quad_convex_2d-1
    false, //object_geo_line_quad_concave_2d-1
    false, //object_geo_line_circle_2d-1

    true, //object_geo_flat_triangle_2d-1
    true, //object_geo_flat_quad_2d-1
    true, //object_geo_flat_quad_convex_2d-1
    true, //object_geo_flat_quad_concave_2d-1
    true, //object_geo_flat_circle_2d-1

    false, //object_geo_line_line_3d-1
    false, //object_geo_line_triangle_3d-1
    false, //object_geo_line_quad_3d-1
    false, //object_geo_line_grid_3d-1
    false, //object_geo_line_quad_convex_3d-1
    false, //object_geo_line_quad_concave_3d-1
    false, //object_geo_line_circle_3d-1
    false, //object_geo_line_aabb_3d-1
    false, //object_geo_line_sphere_3d-1
    false, //object_geo_line_cylinder_3d-1
    false, //object_geo_line_capsule_3d-1
    false, //object_geo_line_cone_3d-1
    false, //object_geo_line_torus_3d-1

    true, //object_geo_flat_triangle_3d-1
    true, //object_geo_flat_quad_3d-1
    true, //object_geo_flat_quad_convex_3d-1
    true, //object_geo_flat_quad_concave_3d-1
    true, //object_geo_flat_circle_3d-1
    true, //object_geo_flat_aabb_3d-1
    true, //object_geo_flat_sphere_3d-1
    true, //object_geo_flat_cylinder_3d-1
    true, //object_geo_flat_capsule_3d-1
    true, //object_geo_flat_cone_3d-1
    true, //object_geo_flat_torus_3d-1

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
void Vulkan_017_Collision::ModelObjectRend::RecreateSwapChain()
{
    if (this->pMeshSub == nullptr)
        return;
    if (this->pMeshSub->pMesh->typeMesh != F_Mesh_Geometry)
        return;
    
    if (this->pMeshSub->pMesh->typeGeometryType == F_MeshGeometry_LineCircle2D)
    {
        FMeshCreateParam_LineCircle2D* pLineCircle2D = (FMeshCreateParam_LineCircle2D*)(this->pMeshSub->pMesh->pMeshCreateParam);
        pLineCircle2D->viewWidth = this->pModelObject->pWindow->poViewport.width;
        pLineCircle2D->viewHeight = this->pModelObject->pWindow->poViewport.height;
        FMeshGeometry::UpdateLineCircle2D(pMeshSub->vertices_Pos3Color4, pLineCircle2D);
        pMeshSub->UpdateVertexBuffer();
    }
    else if (this->pMeshSub->pMesh->typeGeometryType == F_MeshGeometry_FlatCircle2D)
    {
        FMeshCreateParam_FlatCircle2D* pFlatCircle2D = (FMeshCreateParam_FlatCircle2D*)(this->pMeshSub->pMesh->pMeshCreateParam);
        pFlatCircle2D->viewWidth = this->pModelObject->pWindow->poViewport.width;
        pFlatCircle2D->viewHeight = this->pModelObject->pWindow->poViewport.height;
        FMeshGeometry::UpdateFlatCircle2D(pMeshSub->vertices_Pos3Color4, pFlatCircle2D);
        pMeshSub->UpdateVertexBuffer();
    }
}   


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
    //Object
    count = this->poBuffers_ObjectCB.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
    }
    this->objectCBs.clear();
    this->poBuffers_ObjectCB.clear();
    this->poBuffersMemory_ObjectCB.clear();

    //ObjectLineFlat2D
    count = this->poBuffers_ObjectCB_LineFlat2D.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB_LineFlat2D[i], this->poBuffersMemory_ObjectCB_LineFlat2D[i]);
    }
    this->objectCBs_LineFlat2D.clear();
    this->poBuffers_ObjectCB_LineFlat2D.clear();
    this->poBuffersMemory_ObjectCB_LineFlat2D.clear();

    //ObjectLineFlat3D
    count = this->poBuffers_ObjectCB_LineFlat3D.size();
    for (size_t i = 0; i < count; i++) 
    {
        this->pRend->pModelObject->pWindow->destroyVkBuffer(this->poBuffers_ObjectCB_LineFlat3D[i], this->poBuffersMemory_ObjectCB_LineFlat3D[i]);
    }
    this->objectCBs_LineFlat3D.clear();
    this->poBuffers_ObjectCB_LineFlat3D.clear();
    this->poBuffersMemory_ObjectCB_LineFlat3D.clear();

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
        if (this->pRend->isGeometry3D)
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
                //ObjectLineFlat3D
                bufferSize = sizeof(LineFlat3DObjectConstants) * this->objectCBs_LineFlat3D.size();
                this->poBuffers_ObjectCB_LineFlat3D.resize(count_sci);
                this->poBuffersMemory_ObjectCB_LineFlat3D.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB_LineFlat3D[j], this->poBuffersMemory_ObjectCB_LineFlat3D[j]);
                }
            }
        }
        else
        {
            //ObjectLineFlat2D
            bufferSize = sizeof(LineFlat2DObjectConstants) * this->objectCBs_LineFlat2D.size();
            this->poBuffers_ObjectCB_LineFlat2D.resize(count_sci);
            this->poBuffersMemory_ObjectCB_LineFlat2D.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                this->pRend->pModelObject->pWindow->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB_LineFlat2D[j], this->poBuffersMemory_ObjectCB_LineFlat2D[j]);
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
    this->objectCBs_LineFlat2D.clear();
    this->objectCBs_LineFlat3D.clear();
    this->materialCBs.clear();
    this->tessellationCBs.clear();

    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        if (pR->isGeometry3D)
        {
            if (!pR->isGeometryLine && !pR->isGeometryFlat)
                this->objectCBs.insert(this->objectCBs.end(), pR->objectCBs.begin(), pR->objectCBs.end());
            else
                this->objectCBs_LineFlat3D.insert(this->objectCBs_LineFlat3D.end(), pR->objectCBs_LineFlat3D.begin(), pR->objectCBs_LineFlat3D.end());
        }
        else
        {
            this->objectCBs_LineFlat2D.insert(this->objectCBs_LineFlat2D.end(), pR->objectCBs_LineFlat2D.begin(), pR->objectCBs_LineFlat2D.end());
        }
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


/////////////////////////// Vulkan_017_Collision ////////////////
FColor Vulkan_017_Collision::s_color_Select = FColor(1, 0, 0, 1);
FColor Vulkan_017_Collision::s_color_Hover = FColor(1, 1, 0, 1);
//Line2D
FColor Vulkan_017_Collision::s_color_LineLine2D = FColor(0, 0, 1, 1);               //LineLine2D
FColor Vulkan_017_Collision::s_color_LineTriangle2D = FColor(0, 1, 0, 1);           //LineTriangle2D
FColor Vulkan_017_Collision::s_color_LineQuad2D = FColor(0, 1, 1, 1);               //LineQuad2D
FColor Vulkan_017_Collision::s_color_LineGrid2D = FColor(0, 1, 1, 1);               //LineGrid2D
FColor Vulkan_017_Collision::s_color_LineQuadConvex2D = FColor(0, 1, 1, 1);         //LineQuad2D Convex
FColor Vulkan_017_Collision::s_color_LineQuadConcave2D = FColor(0, 1, 1, 1);        //LineQuad2D Concave
FColor Vulkan_017_Collision::s_color_LineCircle2D = FColor(1, 0, 1, 1);             //LineCircle2D
//Flat2D
FColor Vulkan_017_Collision::s_color_FlatTriangle2D = FColor(0, 1, 0, 1);           //FlatTriangle2D
FColor Vulkan_017_Collision::s_color_FlatQuad2D = FColor(0, 1, 1, 1);               //FlatQuad2D
FColor Vulkan_017_Collision::s_color_FlatQuadConvex2D = FColor(0, 1, 1, 1);         //FlatQuad2D Convex
FColor Vulkan_017_Collision::s_color_FlatQuadConcave2D = FColor(0, 1, 1, 1);        //FlatQuad2D Concave
FColor Vulkan_017_Collision::s_color_FlatCircle2D = FColor(1, 0, 1, 1);             //FlatCircle2D
//Line3D
FColor Vulkan_017_Collision::s_color_LineLine3D = FColor(1, 1, 1, 1);               //LineLine3D
FColor Vulkan_017_Collision::s_color_LineTriangle3D = FColor(1, 1, 1, 1);           //LineTriangle3D
FColor Vulkan_017_Collision::s_color_LineQuad3D = FColor(1, 1, 1, 1);               //LineQuad3D
FColor Vulkan_017_Collision::s_color_LineGrid3D = FColor(1, 1, 1, 1);               //LineGrid3D
FColor Vulkan_017_Collision::s_color_LineQuadConvex3D = FColor(1, 1, 1, 1);         //LineQuad3D Convex
FColor Vulkan_017_Collision::s_color_LineQuadConcave3D = FColor(1, 1, 1, 1);        //LineQuad3D Concave
FColor Vulkan_017_Collision::s_color_LineCircle3D = FColor(1, 1, 1, 1);             //LineCircle3D
FColor Vulkan_017_Collision::s_color_LineAABB3D = FColor(1, 1, 1, 1);               //LineAABB3D
FColor Vulkan_017_Collision::s_color_LineSphere3D = FColor(1, 1, 1, 1);             //LineSphere3D
FColor Vulkan_017_Collision::s_color_LineCylinder3D = FColor(1, 1, 1, 1);           //LineCylinder3D
FColor Vulkan_017_Collision::s_color_LineCapsule3D = FColor(1, 1, 1, 1);            //LineCapsule3D
FColor Vulkan_017_Collision::s_color_LineCone3D = FColor(1, 1, 1, 1);               //LineCone3D
FColor Vulkan_017_Collision::s_color_LineTorus3D = FColor(1, 1, 1, 1);              //LineTorus3D
//Flat3D
FColor Vulkan_017_Collision::s_color_FlatTriangle3D = FColor(1, 1, 1, 1);           //FlatTriangle3D
FColor Vulkan_017_Collision::s_color_FlatQuad3D = FColor(1, 1, 1, 1);               //FlatQuad3D
FColor Vulkan_017_Collision::s_color_FlatQuadConvex3D = FColor(1, 1, 1, 1);         //FlatQuad3D Convex
FColor Vulkan_017_Collision::s_color_FlatQuadConcave3D = FColor(1, 1, 1, 1);        //FlatQuad3D Concave
FColor Vulkan_017_Collision::s_color_FlatCircle3D = FColor(1, 1, 1, 1);             //FlatCircle3D
FColor Vulkan_017_Collision::s_color_FlatAABB3D = FColor(1, 1, 1, 1);               //FlatAABB3D
FColor Vulkan_017_Collision::s_color_FlatSphere3D = FColor(1, 1, 1, 1);             //FlatSphere3D
FColor Vulkan_017_Collision::s_color_FlatCylinder3D = FColor(1, 1, 1, 1);           //FlatCylinder3D
FColor Vulkan_017_Collision::s_color_FlatCapsule3D = FColor(1, 1, 1, 1);            //FlatCapsule3D
FColor Vulkan_017_Collision::s_color_FlatCone3D = FColor(1, 1, 1, 1);               //FlatCone3D
FColor Vulkan_017_Collision::s_color_FlatTorus3D = FColor(1, 1, 1, 1);              //FlatTorus3D
//Entity
FColor Vulkan_017_Collision::s_color_EntityTriangle = FColor(1, 1, 1, 1);           //EntityTriangle
FColor Vulkan_017_Collision::s_color_EntityQuad = FColor(1, 1, 1, 1);               //EntityQuad
FColor Vulkan_017_Collision::s_color_EntityGrid = FColor(1, 1, 1, 1);               //EntityGrid
FColor Vulkan_017_Collision::s_color_EntityCircle = FColor(1, 1, 1, 1);             //EntityCircle
FColor Vulkan_017_Collision::s_color_EntityAABB = FColor(1, 1, 1, 1);               //EntityAABB
FColor Vulkan_017_Collision::s_color_EntitySphere = FColor(1, 1, 1, 1);             //EntitySphere
FColor Vulkan_017_Collision::s_color_EntityGeoSphere = FColor(1, 1, 1, 1);          //EntityGeoSphere
FColor Vulkan_017_Collision::s_color_EntityCylinder = FColor(1, 1, 1, 1);           //EntityCylinder
FColor Vulkan_017_Collision::s_color_EntityCapsule = FColor(1, 1, 1, 1);            //EntityCapsule
FColor Vulkan_017_Collision::s_color_EntityCone = FColor(1, 1, 1, 1);               //EntityCone
FColor Vulkan_017_Collision::s_color_EntityTorus = FColor(1, 1, 1, 1);              //EntityTorus
FColor Vulkan_017_Collision::s_color_EntitySkyBox = FColor(1, 1, 1, 1);             //EntitySkyBox
FColor Vulkan_017_Collision::s_color_EntitySkyDome = FColor(1, 1, 1, 1);            //EntitySkyDome
FColor Vulkan_017_Collision::s_color_EntityTerrain = FColor(1, 1, 1, 1);            //EntityTerrain


Vulkan_017_Collision::Vulkan_017_Collision(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
    //Line2D
    , pRend_LineLine2D(nullptr)
    , pRend_LineTriangle2D(nullptr)
    , pRend_LineQuad2D(nullptr)
    , pRend_LineGrid2D(nullptr)
    , pRend_LineQuadConvex2D(nullptr)
    , pRend_LineQuadConcave2D(nullptr)
    , pRend_LineCircle2D(nullptr)
    //Flat2D
    , pRend_FlatTriangle2D(nullptr)
    , pRend_FlatQuad2D(nullptr)
    , pRend_FlatQuadConvex2D(nullptr)
    , pRend_FlatQuadConcave2D(nullptr)
    , pRend_FlatCircle2D(nullptr)
    //Line3D
    , pRend_LineLine3D(nullptr)
    , pRend_LineTriangle3D(nullptr)
    , pRend_LineQuad3D(nullptr)
    , pRend_LineGrid3D(nullptr)
    , pRend_LineQuadConvex3D(nullptr)
    , pRend_LineQuadConcave3D(nullptr)
    , pRend_LineCircle3D(nullptr)
    , pRend_LineAABB3D(nullptr)
    , pRend_LineSphere3D(nullptr)
    , pRend_LineCylinder3D(nullptr)
    , pRend_LineCapsule3D(nullptr)
    , pRend_LineCone3D(nullptr)
    , pRend_LineTorus3D(nullptr)
    //Flat3D
    , pRend_FlatTriangle3D(nullptr)
    , pRend_FlatQuad3D(nullptr)
    , pRend_FlatQuadConvex3D(nullptr)
    , pRend_FlatQuadConcave3D(nullptr)
    , pRend_FlatCircle3D(nullptr)
    , pRend_FlatAABB3D(nullptr)
    , pRend_FlatSphere3D(nullptr)
    , pRend_FlatCylinder3D(nullptr)
    , pRend_FlatCapsule3D(nullptr)
    , pRend_FlatCone3D(nullptr)
    , pRend_FlatTorus3D(nullptr)
    //Entity
    , pRend_EntityTriangle(nullptr)
    , pRend_EntityQuad(nullptr)
    , pRend_EntityGrid(nullptr)
    , pRend_EntityCircle(nullptr)
    , pRend_EntityAABB(nullptr)
    , pRend_EntitySphere(nullptr)
    , pRend_EntityGeoSphere(nullptr)
    , pRend_EntityCylinder(nullptr)
    , pRend_EntityCapsule(nullptr)
    , pRend_EntityCone(nullptr)
    , pRend_EntityTorus(nullptr)
    , pRend_EntitySkyBox(nullptr)
    , pRend_EntitySkyDome(nullptr)
    , pRend_EntityTerrain(nullptr)
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

FPointI Vulkan_017_Collision::Convert2PointI(double x, double y)
{
    FVector2 vSceen = ConvertNDC2Screen((float)x, (float)y);
    return FPointI((int32)vSceen.x, (int32)vSceen.y);
}
FPointF Vulkan_017_Collision::Convert2PointF(double x, double y)
{
    FVector2 vSceen = ConvertNDC2Screen((float)x, (float)y);
    return FPointF(vSceen.x, vSceen.y);
}
void Vulkan_017_Collision::IsCollision(double x, double y, bool isHover)
{
    FColor color;
    if (isHover)
    {
        color = s_color_Hover;
    }
    else
    {
        color = s_color_Select;
    }

    FRay ray;
    FVector4 vViewport = GetViewportVector4();
    this->pCamera->ConvertScreenPos2ToWorldRay((float)x, (float)y, vViewport, &ray);

    //Line2D
    IsCollision_LineLine2D(x, y, this->pRend_LineLine2D, color, isHover);
    IsCollision_LineTriangle2D(x, y, this->pRend_LineTriangle2D, color, isHover);
    IsCollision_LineQuad2D(x, y, this->pRend_LineQuad2D, color, isHover);
    IsCollision_LineGrid2D(x, y, this->pRend_LineGrid2D, color, isHover);
    IsCollision_LineQuadConvex2D(x, y, this->pRend_LineQuadConvex2D, color, isHover);
    IsCollision_LineQuadConcave2D(x, y, this->pRend_LineQuadConcave2D, color, isHover);
    IsCollision_LineCircle2D(x, y, this->pRend_LineCircle2D, color, isHover);
    
    //Flat2D
    IsCollision_FlatTriangle2D(x, y, this->pRend_FlatTriangle2D, color, isHover);
    IsCollision_FlatQuad2D(x, y, this->pRend_FlatQuad2D, color, isHover);
    IsCollision_FlatQuadConvex2D(x, y, this->pRend_FlatQuadConvex2D, color, isHover);
    IsCollision_FlatQuadConcave2D(x, y, this->pRend_FlatQuadConcave2D, color, isHover);
    IsCollision_FlatCircle2D(x, y, this->pRend_FlatCircle2D, color, isHover);

    //Line3D
    IsCollision_LineLine3D(x, y, ray, this->pRend_LineLine3D, color, isHover);
    IsCollision_LineTriangle3D(x, y, ray, this->pRend_LineTriangle3D, color, isHover);
    IsCollision_LineQuad3D(x, y, ray, this->pRend_LineQuad3D, color, isHover);
    IsCollision_LineGrid3D(x, y, ray, this->pRend_LineGrid3D, color, isHover);
    IsCollision_LineQuadConvex3D(x, y, ray, this->pRend_LineQuadConvex3D, color, isHover);
    IsCollision_LineQuadConcave3D(x, y, ray, this->pRend_LineQuadConcave3D, color, isHover);
    IsCollision_LineCircle3D(x, y, ray, this->pRend_LineCircle3D, color, isHover);
    IsCollision_LineAABB3D(x, y, ray, this->pRend_LineAABB3D, color, isHover);
    IsCollision_LineSphere3D(x, y, ray, this->pRend_LineSphere3D, color, isHover);
    IsCollision_LineCylinder3D(x, y, ray, this->pRend_LineCylinder3D, color, isHover);
    IsCollision_LineCapsule3D(x, y, ray, this->pRend_LineCapsule3D, color, isHover);
    IsCollision_LineCone3D(x, y, ray, this->pRend_LineCone3D, color, isHover);
    IsCollision_LineTorus3D(x, y, ray, this->pRend_LineTorus3D, color, isHover);

    //Flat3D
    IsCollision_FlatTriangle3D(x, y, ray, this->pRend_FlatTriangle3D, color, isHover);
    IsCollision_FlatQuad3D(x, y, ray, this->pRend_FlatQuad3D, color, isHover);
    IsCollision_FlatQuadConvex3D(x, y, ray, this->pRend_FlatQuadConvex3D, color, isHover);
    IsCollision_FlatQuadConcave3D(x, y, ray, this->pRend_FlatQuadConcave3D, color, isHover);
    IsCollision_FlatCircle3D(x, y, ray, this->pRend_FlatCircle3D, color, isHover);
    IsCollision_FlatAABB3D(x, y, ray, this->pRend_FlatAABB3D, color, isHover);
    IsCollision_FlatSphere3D(x, y, ray, this->pRend_FlatSphere3D, color, isHover);
    IsCollision_FlatCylinder3D(x, y, ray, this->pRend_FlatCylinder3D, color, isHover);
    IsCollision_FlatCapsule3D(x, y, ray, this->pRend_FlatCapsule3D, color, isHover);
    IsCollision_FlatCone3D(x, y, ray, this->pRend_FlatCone3D, color, isHover);
    IsCollision_FlatTorus3D(x, y, ray, this->pRend_FlatTorus3D, color, isHover);

    //Entity
    IsCollision_EntityTriangle(x, y, ray, this->pRend_EntityTriangle, color, isHover);
    IsCollision_EntityQuad(x, y, ray, this->pRend_EntityQuad, color, isHover);
    IsCollision_EntityGrid(x, y, ray, this->pRend_EntityGrid, color, isHover);
    IsCollision_EntityCircle(x, y, ray, this->pRend_EntityCircle, color, isHover);
    IsCollision_EntityAABB(x, y, ray, this->pRend_EntityAABB, color, isHover);
    IsCollision_EntitySphere(x, y, ray, this->pRend_EntitySphere, color, isHover);
    IsCollision_EntityGeoSphere(x, y, ray, this->pRend_EntityGeoSphere, color, isHover);
    IsCollision_EntityCylinder(x, y, ray, this->pRend_EntityCylinder, color, isHover);
    IsCollision_EntityCapsule(x, y, ray, this->pRend_EntityCapsule, color, isHover);
    IsCollision_EntityCone(x, y, ray, this->pRend_EntityCone, color, isHover);
    IsCollision_EntityTorus(x, y, ray, this->pRend_EntityTorus, color, isHover);
    IsCollision_EntitySkyBox(x, y, ray, this->pRend_EntitySkyBox, color, isHover);
    IsCollision_EntitySkyDome(x, y, ray, this->pRend_EntitySkyDome, color, isHover);
    IsCollision_EntityTerrain(x, y, ray, this->pRend_EntityTerrain, color, isHover);
}

//Line2D
bool Vulkan_017_Collision::IsCollision_LineLine2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineLine2D* pLineLine2D = (FMeshCreateParam_LineLine2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptLine_Start = ConvertNDC2ScreenPointI(pLineLine2D->vStart);
    FPointI ptLine_End = ConvertNDC2ScreenPointI(pLineLine2D->vEnd);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInLine2DI(ptMouse, FLineI(ptLine_Start, ptLine_End)))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineLine2D: Mouse In LineLine 2D !");
        return true;
    }
    obj.color = s_color_LineLine2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineTriangle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineTriangle2D* pLineTriangle2D = (FMeshCreateParam_LineTriangle2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptTriangle_Top = ConvertNDC2ScreenPointI(pLineTriangle2D->vTop);
    FPointI ptTriangle_Left = ConvertNDC2ScreenPointI(pLineTriangle2D->vLeft);
    FPointI ptTriangle_Right = ConvertNDC2ScreenPointI(pLineTriangle2D->vRight);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInTriangle2DI(ptMouse, ptTriangle_Top, ptTriangle_Left, ptTriangle_Right))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineTriangle2D: Mouse In LineTriangle 2D !");
        return true;
    }
    obj.color = s_color_LineTriangle2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineQuad2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineQuad2D* pLineQuad2D = (FMeshCreateParam_LineQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_LeftTop = ConvertNDC2ScreenPointI(pLineQuad2D->vLeftTop);
    FPointI ptQuad_RightBottom = ConvertNDC2ScreenPointI(pLineQuad2D->vRightBottom);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, FRectI(ptQuad_LeftTop.x, ptQuad_LeftTop.y, ptQuad_RightBottom.x, ptQuad_RightBottom.y)))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuad2D: Mouse In LineQuad 2D !");
        return true;
    }
    obj.color = s_color_LineQuad2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineGrid2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineGrid2D* pLineGrid2D = (FMeshCreateParam_LineGrid2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptGrid_LeftTop = ConvertNDC2ScreenPointI(pLineGrid2D->vLeftTop);
    FPointI ptGrid_RightBottom = ConvertNDC2ScreenPointI(pLineGrid2D->vRightBottom);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, FRectI(ptGrid_LeftTop.x, ptGrid_LeftTop.y, ptGrid_RightBottom.x, ptGrid_RightBottom.y)))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineGrid2D: Mouse In LineGrid 2D !");
        return true;
    }
    obj.color = s_color_LineGrid2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineQuadConvex2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineQuad2D* pLineQuadConvex2D = (FMeshCreateParam_LineQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_a = ConvertNDC2ScreenPointI(pLineQuadConvex2D->vLeftTop);
    FPointI ptQuad_b = ConvertNDC2ScreenPointI(pLineQuadConvex2D->vLeftBottom);
    FPointI ptQuad_c = ConvertNDC2ScreenPointI(pLineQuadConvex2D->vRightBottom);
    FPointI ptQuad_d = ConvertNDC2ScreenPointI(pLineQuadConvex2D->vRightTop);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, ptQuad_a, ptQuad_b, ptQuad_c, ptQuad_d))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuadConvex2D: Mouse In LineQuad Convex 2D !");
        return true;
    }
    obj.color = s_color_LineQuadConvex2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineQuadConcave2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineQuad2D* pLineQuadConcave2D = (FMeshCreateParam_LineQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_a = ConvertNDC2ScreenPointI(pLineQuadConcave2D->vLeftTop);
    FPointI ptQuad_b = ConvertNDC2ScreenPointI(pLineQuadConcave2D->vLeftBottom);
    FPointI ptQuad_c = ConvertNDC2ScreenPointI(pLineQuadConcave2D->vRightBottom);
    FPointI ptQuad_d = ConvertNDC2ScreenPointI(pLineQuadConcave2D->vRightTop);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, ptQuad_a, ptQuad_b, ptQuad_c, ptQuad_d))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuadConcave2D: Mouse In LineQuad Concave 2D !");
        return true;
    }
    obj.color = s_color_LineQuadConcave2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineCircle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_LineCircle2D* pLineCircle2D = (FMeshCreateParam_LineCircle2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptCircle_Center = ConvertNDC2ScreenPointI(pLineCircle2D->vCenter);
    int32 radius = pLineCircle2D->GetRadiusI(this->poWindowContentScale);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInCircle2DI(ptMouse, ptCircle_Center, radius))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_LineCircle2D: Mouse In LineCircle 2D !");
        return true;
    }
    obj.color = s_color_LineCircle2D;
    return false;
}

//Flat2D
bool Vulkan_017_Collision::IsCollision_FlatTriangle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{   
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_FlatTriangle2D* pFlatTriangle2D = (FMeshCreateParam_FlatTriangle2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptTriangle_Top = ConvertNDC2ScreenPointI(pFlatTriangle2D->vTop);
    FPointI ptTriangle_Left = ConvertNDC2ScreenPointI(pFlatTriangle2D->vLeft);
    FPointI ptTriangle_Right = ConvertNDC2ScreenPointI(pFlatTriangle2D->vRight);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInTriangle2DI(ptMouse, ptTriangle_Top, ptTriangle_Left, ptTriangle_Right))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatTriangle2D: Mouse In FlatTriangle 2D !");
        return true;
    }
    obj.color = s_color_FlatTriangle2D;
    return false;
}
bool Vulkan_017_Collision::IsCollision_FlatQuad2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_FlatQuad2D* pFlatQuad2D = (FMeshCreateParam_FlatQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_LeftTop = ConvertNDC2ScreenPointI(pFlatQuad2D->vLeftTop);
    FPointI ptQuad_RightBottom = ConvertNDC2ScreenPointI(pFlatQuad2D->vRightBottom);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, FRectI(ptQuad_LeftTop.x, ptQuad_LeftTop.y, ptQuad_RightBottom.x, ptQuad_RightBottom.y)))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuad2D: Mouse In FlatQuad 2D !");
        return true;
    }
    obj.color = s_color_FlatQuad2D;
    return false;   
}
bool Vulkan_017_Collision::IsCollision_FlatQuadConvex2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_FlatQuad2D* pFlatQuadConvex2D = (FMeshCreateParam_FlatQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_a = ConvertNDC2ScreenPointI(pFlatQuadConvex2D->vLeftTop);
    FPointI ptQuad_b = ConvertNDC2ScreenPointI(pFlatQuadConvex2D->vLeftBottom);
    FPointI ptQuad_c = ConvertNDC2ScreenPointI(pFlatQuadConvex2D->vRightBottom);
    FPointI ptQuad_d = ConvertNDC2ScreenPointI(pFlatQuadConvex2D->vRightTop);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, ptQuad_a, ptQuad_b, ptQuad_c, ptQuad_d))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuadConvex2D: Mouse In FlatQuad Convex 2D !");
        return true;
    }
    obj.color = s_color_FlatQuadConvex2D;
    return false; 
}
bool Vulkan_017_Collision::IsCollision_FlatQuadConcave2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_FlatQuad2D* pFlatQuadConcave2D = (FMeshCreateParam_FlatQuad2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptQuad_a = ConvertNDC2ScreenPointI(pFlatQuadConcave2D->vLeftTop);
    FPointI ptQuad_b = ConvertNDC2ScreenPointI(pFlatQuadConcave2D->vLeftBottom);
    FPointI ptQuad_c = ConvertNDC2ScreenPointI(pFlatQuadConcave2D->vRightBottom);
    FPointI ptQuad_d = ConvertNDC2ScreenPointI(pFlatQuadConcave2D->vRightTop);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInRect2DI(ptMouse, ptQuad_a, ptQuad_b, ptQuad_c, ptQuad_d))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuadConcave2D: Mouse In FlatQuad Concave 2D !");
        return true;
    }
    obj.color = s_color_FlatQuadConcave2D;
    return false;   
}
bool Vulkan_017_Collision::IsCollision_FlatCircle2D(double x, double y, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    FPointI ptMouse((int32)x, (int32)y);
    FMeshCreateParam_FlatCircle2D* pFlatCircle2D = (FMeshCreateParam_FlatCircle2D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    FPointI ptCircle_Center = ConvertNDC2ScreenPointI(pFlatCircle2D->vCenter);
    int32 radius = pFlatCircle2D->GetRadiusI(this->poWindowContentScale);
    LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[0];
    if (FMath::Intersects_PointInCircle2DI(ptMouse, ptCircle_Center, radius))
    {
        obj.color = color;
        //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatCircle2D: Mouse In FlatCircle 2D !");
        return true;
    }
    obj.color = s_color_FlatCircle2D;
    return false;
}

//Line3D
bool Vulkan_017_Collision::IsCollision_LineLine3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    if (!isHover)
    {
        isCollision = false;
    }
    FMeshCreateParam_LineLine3D* pLineLine3D = (FMeshCreateParam_LineLine3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vStart = FMath::Transform(obj.g_MatWorld, pLineLine3D->vStart);
        FVector3 vEnd = FMath::Transform(obj.g_MatWorld, pLineLine3D->vEnd);
        if (FMath::Intersects_RaySegment_Test(ray, vStart, vEnd, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineLine3D: Ray trace in LineLine 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineLine3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineTriangle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineTriangle3D* pLineTriangle3D = (FMeshCreateParam_LineTriangle3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vTop = FMath::Transform(obj.g_MatWorld, pLineTriangle3D->vTop);
        FVector3 vLeft = FMath::Transform(obj.g_MatWorld, pLineTriangle3D->vLeft);
        FVector3 vRight = FMath::Transform(obj.g_MatWorld, pLineTriangle3D->vRight);
        if (FMath::Intersects_RayTriangle_Test(ray, vTop, vRight, vLeft, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineTriangle3D: Ray trace in LineTriangle 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineTriangle3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineQuad3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineQuad3D* pLineQuad3D = (FMeshCreateParam_LineQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pLineQuad3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pLineQuad3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pLineQuad3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pLineQuad3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuad3D: Ray trace in LineQuad 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineQuad3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineGrid3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineGrid3D* pLineGrid3D = (FMeshCreateParam_LineGrid3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pLineGrid3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pLineGrid3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pLineGrid3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pLineGrid3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineGrid3D: Ray trace in LineGrid 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineGrid3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineQuadConvex3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineQuad3D* pLineQuadConvex3D = (FMeshCreateParam_LineQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pLineQuadConvex3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pLineQuadConvex3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pLineQuadConvex3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pLineQuadConvex3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuadConvex3D: Ray trace in LineQuad Convex 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineQuadConvex3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineQuadConcave3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineQuad3D* pLineQuadConcave3D = (FMeshCreateParam_LineQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pLineQuadConcave3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pLineQuadConcave3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pLineQuadConcave3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pLineQuadConcave3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineQuadConcave3D: Ray trace in LineQuad Concave 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineQuadConcave3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineCircle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_LineCircle3D* pLineCircle3D = (FMeshCreateParam_LineCircle3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vCenter = FMath::Transform(obj.g_MatWorld, pLineCircle3D->vCenter);
        FVector3 vUp = FMath::Transform(obj.g_MatWorld, pLineCircle3D->vUp);
        FPlane plane(vUp, vCenter);
        if (FMath::Intersects_RayCircle_Test(ray, plane, vCenter, pLineCircle3D->radius, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineCircle3D: Ray trace in LineCircle 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineCircle3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineAABB3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FAABB& aabb = pRend->pMeshSub->aabb;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FAABB aabbWorld(aabb);
        aabbWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RayAABB_Test(ray, aabbWorld))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineAABB3D: Ray trace in LineAABB 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineAABB3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineSphere3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FSphere& sphere = pRend->pMeshSub->sphere;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FSphere sphereWorld(sphere);
        sphereWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RaySphere_Test(ray, sphereWorld, false))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_LineSphere3D: Ray trace in LineSphere 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_LineSphere3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_LineCylinder3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineCapsule3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineCone3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_LineTorus3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}

//Flat3D
bool Vulkan_017_Collision::IsCollision_FlatTriangle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_FlatTriangle3D* pFlatTriangle3D = (FMeshCreateParam_FlatTriangle3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vTop = FMath::Transform(obj.g_MatWorld, pFlatTriangle3D->vTop);
        FVector3 vLeft = FMath::Transform(obj.g_MatWorld, pFlatTriangle3D->vLeft);
        FVector3 vRight = FMath::Transform(obj.g_MatWorld, pFlatTriangle3D->vRight);
        if (FMath::Intersects_RayTriangle_Test(ray, vTop, vRight, vLeft, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatTriangle3D: Ray trace in FlatTriangle 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatTriangle3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatQuad3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_FlatQuad3D* pFlatQuad3D = (FMeshCreateParam_FlatQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pFlatQuad3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pFlatQuad3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pFlatQuad3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pFlatQuad3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuad3D: Ray trace in FlatQuad 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatQuad3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatQuadConvex3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_FlatQuad3D* pFlatQuadConvex3D = (FMeshCreateParam_FlatQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pFlatQuadConvex3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pFlatQuadConvex3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pFlatQuadConvex3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pFlatQuadConvex3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuadConvex3D: Ray trace in FlatQuad Convex 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatQuadConvex3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatQuadConcave3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_FlatQuad3D* pFlatQuadConcave3D = (FMeshCreateParam_FlatQuad3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pFlatQuadConcave3D->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pFlatQuadConcave3D->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pFlatQuadConcave3D->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pFlatQuadConcave3D->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatQuadConcave3D: Ray trace in FlatQuad Concave 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatQuadConcave3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatCircle3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_FlatCircle3D* pFlatCircle3D = (FMeshCreateParam_FlatCircle3D*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FVector3 vCenter = FMath::Transform(obj.g_MatWorld, pFlatCircle3D->vCenter);
        FVector3 vUp = FMath::Transform(obj.g_MatWorld, pFlatCircle3D->vUp);
        FPlane plane(vUp, vCenter);
        if (FMath::Intersects_RayCircle_Test(ray, plane, vCenter, pFlatCircle3D->radius, 0.005f))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatCircle3D: Ray trace in LineCircle 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatCircle3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatAABB3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FAABB& aabb = pRend->pMeshSub->aabb;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FAABB aabbWorld(aabb);
        aabbWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RayAABB_Test(ray, aabbWorld))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatAABB3D: Ray trace in FlatAABB 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatAABB3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatSphere3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FSphere& sphere = pRend->pMeshSub->sphere;
    size_t count = pRend->objectCBs_LineFlat3D.size();
    for (size_t i = 0; i < count; i++)
    {
        LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[i];
        FSphere sphereWorld(sphere);
        sphereWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RaySphere_Test(ray, sphereWorld, false))
        {
            obj.color = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_FlatSphere3D: Ray trace in FlatSphere 3D !");
            isCollision = true;
        }
        else
        {
            obj.color = s_color_FlatSphere3D;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_FlatCylinder3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_FlatCapsule3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_FlatCone3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_FlatTorus3D(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}

//Entity
bool Vulkan_017_Collision::IsCollision_EntityTriangle(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_EntityTriangle* pEntityTriangle = (FMeshCreateParam_EntityTriangle*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FVector3 vTop = FMath::Transform(obj.g_MatWorld, pEntityTriangle->vTop);
        FVector3 vLeft = FMath::Transform(obj.g_MatWorld, pEntityTriangle->vLeft);
        FVector3 vRight = FMath::Transform(obj.g_MatWorld, pEntityTriangle->vRight);
        if (FMath::Intersects_RayTriangle_Test(ray, vTop, vRight, vLeft, 0.005f))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityTriangle: Ray trace in EntityTriangle !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityTriangle;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityQuad(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_EntityQuad* pEntityQuad = (FMeshCreateParam_EntityQuad*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pEntityQuad->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pEntityQuad->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pEntityQuad->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pEntityQuad->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityQuad: Ray trace in EntityQuad !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityQuad;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityGrid(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_EntityGrid* pEntityGrid = (FMeshCreateParam_EntityGrid*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FVector3 vLeftTop = FMath::Transform(obj.g_MatWorld, pEntityGrid->vLeftTop);
        FVector3 vRightTop = FMath::Transform(obj.g_MatWorld, pEntityGrid->vRightTop);
        FVector3 vRightBottom = FMath::Transform(obj.g_MatWorld, pEntityGrid->vRightBottom);
        FVector3 vLeftBottom = FMath::Transform(obj.g_MatWorld, pEntityGrid->vLeftBottom);
        if (FMath::Intersects_RayQuad_Test(ray, vLeftTop, vRightTop, vRightBottom, vLeftBottom, 0.005f))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityGrid: Ray trace in EntityGrid !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityGrid;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityCircle(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FMeshCreateParam_EntityCircle* pEntityCircle = (FMeshCreateParam_EntityCircle*)pRend->pMeshSub->pMesh->pMeshCreateParam;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FVector3 vCenter = FMath::Transform(obj.g_MatWorld, pEntityCircle->vCenter);
        FVector3 vUp = FMath::Transform(obj.g_MatWorld, pEntityCircle->vUp);
        FPlane plane(vUp, vCenter);
        if (FMath::Intersects_RayCircle_Test(ray, plane, vCenter, pEntityCircle->radius, 0.005f))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityCircle: Ray trace in EntityCircle !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityCircle;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityAABB(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FAABB& aabb = pRend->pMeshSub->aabb;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FAABB aabbWorld(aabb);
        aabbWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RayAABB_Test(ray, aabbWorld))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityAABB: Ray trace in EntityAABB !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityAABB;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntitySphere(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FSphere& sphere = pRend->pMeshSub->sphere;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FSphere sphereWorld(sphere);
        sphereWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RaySphere_Test(ray, sphereWorld, false))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntitySphere: Ray trace in EntitySphere !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntitySphere;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityGeoSphere(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    bool isCollision = false;
    FSphere& sphere = pRend->pMeshSub->sphere;
    size_t count = pRend->objectCBs.size();
    for (size_t i = 0; i < count; i++)
    {
        ObjectConstants& obj = pRend->objectCBs[i];
        MaterialConstants& mat = pRend->materialCBs[i];
        FSphere sphereWorld(sphere);
        sphereWorld.Transform(obj.g_MatWorld);
        if (FMath::Intersects_RaySphere_Test(ray, sphereWorld, false))
        {
            mat.factorAmbient = color;
            //F_LogInfo("Vulkan_017_Collision::IsCollision_EntityGeoSphere: Ray trace in EntityGeoSphere !");
            isCollision = true;
        }
        else
        {
            mat.factorAmbient = s_color_EntityGeoSphere;
        }
    }
    return isCollision;
}
bool Vulkan_017_Collision::IsCollision_EntityCylinder(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntityCapsule(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntityCone(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntityTorus(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntitySkyBox(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntitySkyDome(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}
bool Vulkan_017_Collision::IsCollision_EntityTerrain(double x, double y, const FRay& ray, ModelObjectRend* pRend, const FColor& color, bool isHover)
{
    return false;
}

void Vulkan_017_Collision::OnMouseLeftDown(double x, double y)
{
    VulkanWindow::OnMouseLeftDown(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseLeftDown: Mouse Left Down !");
    
    IsCollision(x, y, false);
}
void Vulkan_017_Collision::OnMouseLeftUp(double x, double y)
{
    VulkanWindow::OnMouseLeftUp(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseLeftUp: Mouse Left Up !");

    IsCollision(x, y, false);
}
void Vulkan_017_Collision::OnMouseMove(int button, double x, double y)
{
    VulkanWindow::OnMouseMove(button, x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseMove: Mouse Move !");

    IsCollision(x, y, false);
}
void Vulkan_017_Collision::OnMouseHover(double x, double y)
{
    VulkanWindow::OnMouseHover(x, y);
    //F_LogInfo("Vulkan_017_Collision::OnMouseHover: Mouse Hover !");

    IsCollision(x, y, true);
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
                if (pModelObject->pMesh->typeMesh == F_Mesh_Geometry)
                {
                    switch ((int)pModelObject->pMesh->typeGeometryType)
                    {
                        //Line2D
                        case F_MeshGeometry_LineLine2D:         this->pRend_LineLine2D = pRend;             break;
                        case F_MeshGeometry_LineTriangle2D:     this->pRend_LineTriangle2D = pRend;         break;
                        case F_MeshGeometry_LineQuad2D:         
                            {
                                if (this->pRend_LineQuad2D == nullptr)
                                    this->pRend_LineQuad2D = pRend;      
                                else if (this->pRend_LineQuadConvex2D == nullptr)       
                                    this->pRend_LineQuadConvex2D = pRend;  
                                else if (this->pRend_LineQuadConcave2D == nullptr)       
                                    this->pRend_LineQuadConcave2D = pRend;  
                                break;
                            }
                        case F_MeshGeometry_LineGrid2D:         this->pRend_LineGrid2D = pRend;             break;
                        case F_MeshGeometry_LineCircle2D:       this->pRend_LineCircle2D = pRend;           break;
                        //Flat2D
                        case F_MeshGeometry_FlatTriangle2D:     this->pRend_FlatTriangle2D = pRend;         break;
                        case F_MeshGeometry_FlatQuad2D:         
                            {
                                if (this->pRend_FlatQuad2D == nullptr)
                                    this->pRend_FlatQuad2D = pRend;      
                                else if (this->pRend_FlatQuadConvex2D == nullptr)       
                                    this->pRend_FlatQuadConvex2D = pRend;  
                                else if (this->pRend_FlatQuadConcave2D == nullptr)       
                                    this->pRend_FlatQuadConcave2D = pRend;  
                                break;
                            }
                        case F_MeshGeometry_FlatCircle2D:       this->pRend_FlatCircle2D = pRend;           break;
                        //Line3D
                        case F_MeshGeometry_LineLine3D:         this->pRend_LineLine3D = pRend;             break;
                        case F_MeshGeometry_LineTriangle3D:     this->pRend_LineTriangle3D = pRend;         break;
                        case F_MeshGeometry_LineQuad3D:         
                            {
                                if (this->pRend_LineQuad3D == nullptr)
                                    this->pRend_LineQuad3D = pRend;      
                                else if (this->pRend_LineQuadConvex3D == nullptr)       
                                    this->pRend_LineQuadConvex3D = pRend;  
                                else if (this->pRend_LineQuadConcave3D == nullptr)       
                                    this->pRend_LineQuadConcave3D = pRend;     
                                break;
                            }
                        case F_MeshGeometry_LineGrid3D:         this->pRend_LineGrid3D = pRend;             break;
                        case F_MeshGeometry_LineCircle3D:       this->pRend_LineCircle3D = pRend;           break;
                        case F_MeshGeometry_LineAABB3D:         this->pRend_LineAABB3D = pRend;             break;
                        case F_MeshGeometry_LineSphere3D:       this->pRend_LineSphere3D = pRend;           break;
                        case F_MeshGeometry_LineCylinder3D:     this->pRend_LineCylinder3D = pRend;         break;
                        case F_MeshGeometry_LineCapsule3D:      this->pRend_LineCapsule3D = pRend;          break;
                        case F_MeshGeometry_LineCone3D:         this->pRend_LineCone3D = pRend;             break;
                        case F_MeshGeometry_LineTorus3D:        this->pRend_LineTorus3D = pRend;            break;
                        //Flat3D
                        case F_MeshGeometry_FlatTriangle3D:     this->pRend_FlatTriangle3D = pRend;         break;
                        case F_MeshGeometry_FlatQuad3D:         
                            {
                                if (this->pRend_FlatQuad3D == nullptr)
                                    this->pRend_FlatQuad3D = pRend;      
                                else if (this->pRend_FlatQuadConvex3D == nullptr)       
                                    this->pRend_FlatQuadConvex3D = pRend;  
                                else if (this->pRend_FlatQuadConcave3D == nullptr)       
                                    this->pRend_FlatQuadConcave3D = pRend;     
                                break;
                            }
                        case F_MeshGeometry_FlatCircle3D:       this->pRend_FlatCircle3D = pRend;           break;
                        case F_MeshGeometry_FlatAABB3D:         this->pRend_FlatAABB3D = pRend;             break;
                        case F_MeshGeometry_FlatSphere3D:       this->pRend_FlatSphere3D = pRend;           break;
                        case F_MeshGeometry_FlatCylinder3D:     this->pRend_FlatCylinder3D = pRend;         break;
                        case F_MeshGeometry_FlatCapsule3D:      this->pRend_FlatCapsule3D = pRend;          break;
                        case F_MeshGeometry_FlatCone3D:         this->pRend_FlatCone3D = pRend;             break;
                        case F_MeshGeometry_FlatTorus3D:        this->pRend_FlatTorus3D = pRend;            break;
                        //Entity
                        case F_MeshGeometry_EntityTriangle:     this->pRend_EntityTriangle = pRend;         break;
                        case F_MeshGeometry_EntityQuad:         this->pRend_EntityQuad = pRend;             break;
                        case F_MeshGeometry_EntityGrid:         this->pRend_EntityGrid = pRend;             break;
                        case F_MeshGeometry_EntityCircle:       this->pRend_EntityCircle = pRend;           break;
                        case F_MeshGeometry_EntityAABB:         this->pRend_EntityAABB = pRend;             break;
                        case F_MeshGeometry_EntitySphere:       this->pRend_EntitySphere = pRend;           break;
                        case F_MeshGeometry_EntityGeoSphere:    this->pRend_EntityGeoSphere = pRend;        break;
                        case F_MeshGeometry_EntityCylinder:     this->pRend_EntityCylinder = pRend;         break;
                        case F_MeshGeometry_EntityCapsule:      this->pRend_EntityCapsule = pRend;          break;
                        case F_MeshGeometry_EntityCone:         this->pRend_EntityCone = pRend;             break;
                        case F_MeshGeometry_EntityTorus:        this->pRend_EntityTorus = pRend;            break;
                        case F_MeshGeometry_EntitySkyBox:       this->pRend_EntitySkyBox = pRend;           break;
                        case F_MeshGeometry_EntitySkyDome:      this->pRend_EntitySkyDome = pRend;          break;
                        case F_MeshGeometry_EntityTerrain:      this->pRend_EntityTerrain = pRend;          break;
                    }
                }

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
                pRend->isGeometry3D = g_ObjectRend_Is3D[nIndexObjectRend];
                pRend->isGeometryLine = g_ObjectRend_IsLine[nIndexObjectRend];
                pRend->isGeometryFlat = g_ObjectRend_IsFlat[nIndexObjectRend];
                if (pRend->isGeometryLine)
                {
                    pRend->cfg_vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
                    pRend->cfg_vkPolygonMode = VK_POLYGON_MODE_LINE;
                    pRend->cfg_vkCullModeFlagBits = VK_CULL_MODE_NONE;
                }
                if (pRend->isGeometryFlat)
                {
                    pRend->cfg_vkCullModeFlagBits = VK_CULL_MODE_NONE;
                }
                if (!pRend->isGeometry3D)
                {
                    pModelObject->isInstanceDynamic = false;
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
        pRend->objectCBs_LineFlat2D.clear();
        pRend->objectCBs_LineFlat3D.clear();
        pRend->materialCBs.clear();
        for (int j = 0; j < count_instance; j++)
        {
            if (pRend->isGeometry3D)
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
                    //ObjectLineFlat3D
                    LineFlat3DObjectConstants objectConstants;
                    objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                                g_ObjectRend_Tranforms[3 * i + 1],
                                                                g_ObjectRend_Tranforms[3 * i + 2]);
                    switch ((int32)pRend->pMeshSub->pMesh->typeGeometryType)
                    {
                        //Line3D
                        case F_MeshGeometry_LineLine3D:         { objectConstants.color = s_color_LineLine3D;           break; }
                        case F_MeshGeometry_LineTriangle3D:     { objectConstants.color = s_color_LineTriangle3D;       break; }
                        case F_MeshGeometry_LineQuad3D:         { objectConstants.color = s_color_LineQuad3D;           break; }
                        case F_MeshGeometry_LineGrid3D:         { objectConstants.color = s_color_LineGrid3D;           break; }
                        case F_MeshGeometry_LineCircle3D:       { objectConstants.color = s_color_LineCircle3D;         break; }
                        case F_MeshGeometry_LineAABB3D:         { objectConstants.color = s_color_LineAABB3D;           break; }
                        case F_MeshGeometry_LineSphere3D:       { objectConstants.color = s_color_LineSphere3D;         break; }
                        case F_MeshGeometry_LineCylinder3D:     { objectConstants.color = s_color_LineCylinder3D;       break; }
                        case F_MeshGeometry_LineCapsule3D:      { objectConstants.color = s_color_LineCapsule3D;        break; }
                        case F_MeshGeometry_LineCone3D:         { objectConstants.color = s_color_LineCone3D;           break; }
                        case F_MeshGeometry_LineTorus3D:        { objectConstants.color = s_color_LineTorus3D;          break; }
                        //Flat3D
                        case F_MeshGeometry_FlatTriangle3D:     { objectConstants.color = s_color_FlatTriangle3D;       break; }
                        case F_MeshGeometry_FlatQuad3D:         { objectConstants.color = s_color_FlatQuad3D;           break; }
                        case F_MeshGeometry_FlatCircle3D:       { objectConstants.color = s_color_FlatCircle3D;         break; }
                        case F_MeshGeometry_FlatAABB3D:         { objectConstants.color = s_color_FlatAABB3D;           break; }
                        case F_MeshGeometry_FlatSphere3D:       { objectConstants.color = s_color_FlatSphere3D;         break; }
                        case F_MeshGeometry_FlatCylinder3D:     { objectConstants.color = s_color_FlatCylinder3D;       break; }
                        case F_MeshGeometry_FlatCapsule3D:      { objectConstants.color = s_color_FlatCapsule3D;        break; }
                        case F_MeshGeometry_FlatCone3D:         { objectConstants.color = s_color_FlatCone3D;           break; }
                        case F_MeshGeometry_FlatTorus3D:        { objectConstants.color = s_color_FlatTorus3D;          break; }
                    }
                    pRend->objectCBs_LineFlat3D.push_back(objectConstants);
                    pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
                }
            }
            else
            {
                //ObjectLineFlat2D
                LineFlat2DObjectConstants objectConstants;
                switch ((int32)pRend->pMeshSub->pMesh->typeGeometryType)
                {
                    //Line2D
                    case F_MeshGeometry_LineLine2D:         { objectConstants.color = s_color_LineLine2D;           break; }
                    case F_MeshGeometry_LineTriangle2D:     { objectConstants.color = s_color_LineTriangle2D;       break; }
                    case F_MeshGeometry_LineQuad2D:         { objectConstants.color = s_color_LineQuad2D;           break; }
                    case F_MeshGeometry_LineGrid2D:         { objectConstants.color = s_color_LineGrid2D;           break; }
                    case F_MeshGeometry_LineCircle2D:       { objectConstants.color = s_color_LineCircle2D;         break; }
                    //Flat2D
                    case F_MeshGeometry_FlatTriangle2D:     { objectConstants.color = s_color_FlatTriangle2D;       break; }
                    case F_MeshGeometry_FlatQuad2D:         { objectConstants.color = s_color_FlatQuad2D;           break; }
                    case F_MeshGeometry_FlatCircle2D:       { objectConstants.color = s_color_FlatCircle2D;         break; }
                }
                pRend->objectCBs_LineFlat2D.push_back(objectConstants);
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
                switch ((int32)pRend->pMeshSub->pMesh->typeGeometryType)
                {
                    //Entity
                    case F_MeshGeometry_EntityTriangle:     { materialConstants.factorAmbient = s_color_EntityTriangle;       break; }
                    case F_MeshGeometry_EntityQuad:         { materialConstants.factorAmbient = s_color_EntityQuad;           break; }
                    case F_MeshGeometry_EntityGrid:         { materialConstants.factorAmbient = s_color_EntityGrid;           break; }
                    case F_MeshGeometry_EntityCircle:       { materialConstants.factorAmbient = s_color_EntityCircle;         break; }
                    case F_MeshGeometry_EntityAABB:         { materialConstants.factorAmbient = s_color_EntityAABB;           break; }
                    case F_MeshGeometry_EntitySphere:       { materialConstants.factorAmbient = s_color_EntitySphere;         break; }
                    case F_MeshGeometry_EntityGeoSphere:    { materialConstants.factorAmbient = s_color_EntityGeoSphere;      break; }
                    case F_MeshGeometry_EntityCylinder:     { materialConstants.factorAmbient = s_color_EntityCylinder;       break; }
                    case F_MeshGeometry_EntityCapsule:      { materialConstants.factorAmbient = s_color_EntityCapsule;        break; }
                    case F_MeshGeometry_EntityCone:         { materialConstants.factorAmbient = s_color_EntityCone;           break; }
                    case F_MeshGeometry_EntityTorus:        { materialConstants.factorAmbient = s_color_EntityTorus;          break; }
                    case F_MeshGeometry_EntitySkyBox:       { materialConstants.factorAmbient = s_color_EntitySkyBox;         break; }
                    case F_MeshGeometry_EntitySkyDome:      { materialConstants.factorAmbient = s_color_EntitySkyDome;        break; }
                    case F_MeshGeometry_EntityTerrain:      { materialConstants.factorAmbient = s_color_EntityTerrain;        break; }
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
            if (pRend->isGeometry3D)
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
                    //ObjectLineFlat3D
                    bufferSize = sizeof(LineFlat3DObjectConstants) * MAX_OBJECT_LINEFLAT_3D_COUNT;
                    pRend->poBuffers_ObjectCB_LineFlat3D.resize(count_sci);
                    pRend->poBuffersMemory_ObjectCB_LineFlat3D.resize(count_sci);
                    for (size_t j = 0; j < count_sci; j++) 
                    {
                        createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB_LineFlat3D[j], pRend->poBuffersMemory_ObjectCB_LineFlat3D[j]);
                    }
                }
            }
            else
            {
                //ObjectLineFlat2D
                bufferSize = sizeof(LineFlat2DObjectConstants) * MAX_OBJECT_LINEFLAT_2D_COUNT;
                pRend->poBuffers_ObjectCB_LineFlat2D.resize(count_sci);
                pRend->poBuffersMemory_ObjectCB_LineFlat2D.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB_LineFlat2D[j], pRend->poBuffersMemory_ObjectCB_LineFlat2D[j]);
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
        String nameMesh = g_MeshPaths[7 * i + 0];
        String nameVertexType = g_MeshPaths[7 * i + 1];
        String nameMeshType = g_MeshPaths[7 * i + 2];
        String pathMesh = g_MeshPaths[7 * i + 3];
        String nameGeometryType = g_MeshPaths[7 * i + 4];
        String nameGeometryParam = g_MeshPaths[7 * i + 5];
        String nameGeometryIsVertexUpdate = g_MeshPaths[7 * i + 6];
        bool isVertexUpdate = FUtilString::ParserBool(nameGeometryIsVertexUpdate);

        FMeshVertexType typeVertex = F_ParseMeshVertexType(nameVertexType); 
        FMeshType typeMesh = F_ParseMeshType(nameMeshType);
        FMeshGeometryType typeGeometryType = F_MeshGeometry_EntityTriangle;
        if (!nameGeometryType.empty())
        {
            typeGeometryType = F_ParseMeshGeometryType(nameGeometryType);
        }
        FMeshCreateParam* pParam = nullptr;
        if (!nameGeometryParam.empty())
        {
            pParam = FMeshGeometry::CreateParam(typeGeometryType, nameGeometryParam);
            if (typeGeometryType == F_MeshGeometry_LineCircle2D)
            {
                FMeshCreateParam_LineCircle2D* pParam_LineCircle2D = (FMeshCreateParam_LineCircle2D*)pParam;
                pParam_LineCircle2D->viewWidth = this->poViewport.width;
                pParam_LineCircle2D->viewHeight = this->poViewport.height;
            }
            else if (typeGeometryType == F_MeshGeometry_FlatCircle2D)
            {
                FMeshCreateParam_FlatCircle2D* pParam_FlatCircle2D = (FMeshCreateParam_FlatCircle2D*)pParam;
                pParam_FlatCircle2D->viewWidth = this->poViewport.width;
                pParam_FlatCircle2D->viewHeight = this->poViewport.height;
            }
        }

        Mesh* pMesh = new Mesh(this,
                               nameMesh,
                               pathMesh,
                               typeMesh,
                               typeVertex,
                               typeGeometryType,
                               pParam);
        bool isFlipY = g_MeshIsFlipYs[i];
        bool isTransformLocal = g_MeshIsTranformLocals[i];
        if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals[i], isVertexUpdate))
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
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat2D)) //ObjectLineFlat2D
            {
                VkDescriptorBufferInfo bufferInfo_ObjectLineFlat = {};
                bufferInfo_ObjectLineFlat.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB_LineFlat2D[j] : pRend->poBuffers_ObjectCB_LineFlat2D[j];
                bufferInfo_ObjectLineFlat.offset = 0;
                bufferInfo_ObjectLineFlat.range = sizeof(LineFlat2DObjectConstants) * MAX_OBJECT_LINEFLAT_2D_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            pRendIndirect != nullptr ? pRendIndirect->poDescriptorSets[j] : pRend->pPipelineGraphics->poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_ObjectLineFlat);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat3D)) //ObjectLineFlat3D
            {
                VkDescriptorBufferInfo bufferInfo_ObjectLineFlat = {};
                bufferInfo_ObjectLineFlat.buffer = pRendIndirect != nullptr ? pRendIndirect->poBuffers_ObjectCB_LineFlat3D[j] : pRend->poBuffers_ObjectCB_LineFlat3D[j];
                bufferInfo_ObjectLineFlat.offset = 0;
                bufferInfo_ObjectLineFlat.range = sizeof(LineFlat3DObjectConstants) * MAX_OBJECT_LINEFLAT_3D_COUNT;
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

void Vulkan_017_Collision::updateCBs_Custom()
{
    //1> Object Rend
    float time = this->pTimer->GetTimeSinceStart();
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_object = 0;
        if (pRend->isGeometry3D)
        {
            if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
                count_object = pRend->objectCBs.size();
            else
                count_object = pRend->objectCBs_LineFlat3D.size();
        }
        else
        {
            count_object = pRend->objectCBs_LineFlat2D.size();
        }
        for (size_t j = 0; j < count_object; j++)
        {
            if (pRend->isGeometry3D)
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
                    //ObjectLineFlat3D
                    LineFlat3DObjectConstants& objectCB = pRend->objectCBs_LineFlat3D[j];
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
        if (pRend->isGeometry3D)
        {
            if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
            {
                VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRend->objectCBs.data(), memory);
            }
            else
            {
                VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB_LineFlat3D[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(LineFlat3DObjectConstants) * count_object, pRend->objectCBs_LineFlat3D.data(), memory);
            }
        }
        else
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_ObjectCB_LineFlat2D[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(LineFlat2DObjectConstants) * count_object, pRend->objectCBs_LineFlat2D.data(), memory);
        }
        
        //MaterialConstants
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pRend->materialCBs.data(), memory);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            VkDeviceMemory& memory = pRend->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
            updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pRend->tessellationCBs.data(), memory);
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
            
            if (pRendIndirect->pRend->isGeometry3D)
            {
                if (!pRendIndirect->pRend->isGeometryLine && !pRendIndirect->pRend->isGeometryFlat)
                {
                    //ObjectConstants
                    size_t count_object = pRendIndirect->objectCBs.size();
                    VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRendIndirect->objectCBs.data(), memory);
                }
                else
                {
                    //ObjectLineFlat3D
                    size_t count_object = pRendIndirect->objectCBs_LineFlat3D.size();
                    VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB_LineFlat3D[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(LineFlat3DObjectConstants) * count_object, pRendIndirect->objectCBs_LineFlat3D.data(), memory);
                }
            }
            else
            {
                //ObjectLineFlat2D
                size_t count_object = pRendIndirect->objectCBs_LineFlat2D.size();
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB_LineFlat2D[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(LineFlat2DObjectConstants) * count_object, pRendIndirect->objectCBs_LineFlat2D.data(), memory);
            }

            //MaterialConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_materialCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(MaterialConstants) * count_object, pRendIndirect->materialCBs.data(), memory);
            }

            //TessellationConstants
            if (pRendIndirect->pRend->isUsedTessellation)
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_tessellationCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(TessellationConstants) * count_object, pRendIndirect->tessellationCBs.data(), memory);
            }

            //IndirectCommand
            {
                size_t count_indirectcommand = pRendIndirect->indirectCommandCBs.size();
                updateVKBuffer(0, sizeof(VkDrawIndexedIndirectCommand) * count_indirectcommand, pRendIndirect->indirectCommandCBs.data(), pRendIndirect->poBuffersMemory_indirectCommandCB);
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
                    if (pModelObject->isInstanceDynamic)
                    {
                        pModelObject->countInstanceExt = countInstanceExt;
                        pModelObject->countInstance = countInstanceExt * 2 + 1;
                    }
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
                            ImGui::Text("Vertex: [%d], Index: [%d]", 
                                        (int)pRend->pMeshSub->poVertexCount, 
                                        (int)pRend->pMeshSub->poIndexCount);
                            ImGui::Text("Sphere: Center: [%f - %f - %f]", 
                                         pRend->pMeshSub->sphere.m_vCenter.x, 
                                         pRend->pMeshSub->sphere.m_vCenter.y, 
                                         pRend->pMeshSub->sphere.m_vCenter.z);
                            ImGui::Text("Sphere: Radius: [%f]",
                                         pRend->pMeshSub->sphere.m_fRadius);
                            FVector3 vCenter = pRend->pMeshSub->aabb.GetCenter();
                            FVector3 vExtent = pRend->pMeshSub->aabb.GetExtents();
                            ImGui::Text("AABB: Center: [%f - %f - %f]",
                                        vCenter.x, vCenter.y, vCenter.z);
                            ImGui::Text("AABB: Extent: [%f - %f - %f]",
                                        vExtent.x, vExtent.y, vExtent.z);
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
                                    const FMatrix4* pMat4World = nullptr;
                                    if (pRend->isGeometry3D)
                                    {
                                        if (!pRend->isGeometryLine && !pRend->isGeometryFlat)
                                        {
                                            ObjectConstants& obj = pRend->objectCBs[p];
                                            pMat4World = &obj.g_MatWorld;
                                        }
                                        else
                                        {
                                            LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[p];
                                            pMat4World = &obj.g_MatWorld;
                                        }
                                    }
                                    else
                                    {
                                        pMat4World = &FMath::ms_mat4Unit;
                                    }
                                    
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            //matWorld
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

                                            //color
                                            if (pRend->isGeometry3D)
                                            {
                                                if (pRend->isGeometryLine || pRend->isGeometryFlat)
                                                {
                                                    LineFlat3DObjectConstants& obj = pRend->objectCBs_LineFlat3D[p];
                                                    String nameColor = "Color - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                    if (ImGui::ColorEdit4(nameColor.c_str(), (float*)&obj.color))
                                                    {

                                                    }
                                                }   
                                            }
                                            else
                                            {
                                                LineFlat2DObjectConstants& obj = pRend->objectCBs_LineFlat2D[p];
                                                String nameColor = "Color - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                                if (ImGui::ColorEdit4(nameColor.c_str(), (float*)&obj.color))
                                                {

                                                }
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