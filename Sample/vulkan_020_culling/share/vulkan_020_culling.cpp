/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_020_culling.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/matrix4x4.h>
#include <assimp/postprocess.h>


/////////////////////////// Mesh ////////////////////////////////
static const int g_MeshCount = 26;
static const char* g_MeshPaths[5 * g_MeshCount] =
{
    //Mesh Name         //Vertex Type                           //Mesh Type         //Mesh Geometry Type        //Mesh Path
    "plane",            "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Common/plane.fbx", //plane
    "cube",             "Pos3Color4Normal3Tex2",                "geometry",         "EntityAABB",               "", //cube Assets/Mesh/Common/cube.obj
    "sphere",           "Pos3Color4Normal3Tex2",                "geometry",         "EntitySphere",             "", //sphere Assets/Mesh/Common/sphere.fbx
    
    "grass_lod",        "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/grass_lod/grass_lod.fbx", //grass_lod
    
    "rock001_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock001_lod.fbx", //rock001_lod
    "rock002_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock002_lod.fbx", //rock002_lod
    "rock003_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock003_lod.fbx", //rock003_lod
    "rock004_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock004_lod.fbx", //rock004_lod
    "rock005_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock005_lod.fbx", //rock005_lod
    "rock006_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock006_lod.fbx", //rock006_lod
    "rock007_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock007_lod.fbx", //rock007_lod
    "rock008_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock008_lod.fbx", //rock008_lod
    "rock009_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock009_lod.fbx", //rock009_lod
    "rock010_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock010_lod.fbx", //rock010_lod
    "rock011_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock011_lod.fbx", //rock011_lod
    "rock012_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock012_lod.fbx", //rock012_lod
    "rock013_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock013_lod.fbx", //rock013_lod
    "rock014_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock014_lod.fbx", //rock014_lod
    "rock015_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock015_lod.fbx", //rock015_lod
    "rock016_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock016_lod.fbx", //rock016_lod
    "rock017_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock017_lod.fbx", //rock017_lod
    "rock018_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock018_lod.fbx", //rock018_lod
    "rock019_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock019_lod.fbx", //rock019_lod
    "rock020_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock020_lod.fbx", //rock020_lod
    "rock021_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock021_lod.fbx", //rock021_lod
    "rock022_lod",      "Pos3Color4Normal3Tex2",                "file",             "",                         "Assets/Mesh/Model/rock_lod/rock022_lod.fbx", //rock022_lod
};
static bool g_MeshIsFlipYs[g_MeshCount] = 
{
    true, //plane
    false, //cube
    false, //sphere

    false, //grass_lod

    false, //rock001_lod
    false, //rock002_lod
    false, //rock003_lod
    false, //rock004_lod
    false, //rock005_lod
    false, //rock006_lod
    false, //rock007_lod
    false, //rock008_lod
    false, //rock009_lod
    false, //rock010_lod
    false, //rock011_lod
    false, //rock012_lod
    false, //rock013_lod
    false, //rock014_lod
    false, //rock015_lod
    false, //rock016_lod
    false, //rock017_lod
    false, //rock018_lod
    false, //rock019_lod
    false, //rock020_lod
    false, //rock021_lod
    false, //rock022_lod
};
static bool g_MeshIsTranformLocals[g_MeshCount] = 
{
    false, //plane  
    false, //cube
    false, //sphere

    true, //grass_lod

    true, //rock001_lod
    true, //rock002_lod
    true, //rock003_lod
    true, //rock004_lod
    true, //rock005_lod
    true, //rock006_lod
    true, //rock007_lod
    true, //rock008_lod
    true, //rock009_lod
    true, //rock010_lod
    true, //rock011_lod
    true, //rock012_lod
    true, //rock013_lod
    true, //rock014_lod
    true, //rock015_lod
    true, //rock016_lod
    true, //rock017_lod
    true, //rock018_lod
    true, //rock019_lod
    true, //rock020_lod
    true, //rock021_lod
    true, //rock022_lod
};
static FMatrix4 g_MeshTranformLocals[g_MeshCount] = 
{
    FMath::ms_mat4Unit, //plane
    FMath::ms_mat4Unit, //cube
    FMath::ms_mat4Unit, //sphere

    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //grass_lod

    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock001_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock002_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock003_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock004_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock005_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock006_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock007_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock008_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock009_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock010_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock011_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock012_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock013_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock014_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock015_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock016_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock017_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock018_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock019_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock020_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock021_lod
    FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(0.001f, 0.001f, 0.001f)), //rock022_lod
};


/////////////////////////// Texture /////////////////////////////
static const int g_TextureCount = 37;
static const char* g_TexturePaths[5 * g_TextureCount] = 
{
    //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
    "terrain",                          "2D",            "false",                  "false",                           "Assets/Texture/Common/terrain.png", //terrain
    "texturecubemap",                   "CubeMap",       "false",                  "false",                           "Assets/Texture/Sky/texturecubemap_x_right.png;Assets/Texture/Sky/texturecubemap_x_left.png;Assets/Texture/Sky/texturecubemap_y_up.png;Assets/Texture/Sky/texturecubemap_y_down.png;Assets/Texture/Sky/texturecubemap_z_front.png;Assets/Texture/Sky/texturecubemap_z_back.png", //texturecubemap

    "default_white",                    "2D",            "false",                  "false",                           "Assets/Texture/Common/default_white.bmp", //default_white
    "texture2d",                        "2D",            "false",                  "false",                           "Assets/Texture/Common/texture2d.jpg", //texture2d
    
    "rock001_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock001_lod_albedo.tga", //rock001_lod_albedo
    "rock001_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock001_lod_normal.tga", //rock001_lod_normal
    "rock001_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock001_lod_ao.tga", //rock001_lod_ao
    "rock002_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock002_lod_albedo.tga", //rock002_lod_albedo
    "rock002_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock002_lod_normal.tga", //rock002_lod_normal
    "rock002_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock002_lod_ao.tga", //rock002_lod_ao
    "rock003_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock003_lod_albedo.tga", //rock003_lod_albedo
    "rock003_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock003_lod_normal.tga", //rock003_lod_normal
    "rock003_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock003_lod_ao.tga", //rock003_lod_ao
    "rock004_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock004_lod_albedo.tga", //rock004_lod_albedo
    "rock004_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock004_lod_normal.tga", //rock004_lod_normal
    "rock004_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock004_lod_ao.tga", //rock004_lod_ao
    "rock005_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock005_lod_albedo.tga", //rock005_lod_albedo
    "rock005_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock005_lod_normal.tga", //rock005_lod_normal
    "rock005_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock005_lod_ao.tga", //rock005_lod_ao
    "rock006_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock006_lod_albedo.tga", //rock006_lod_albedo
    "rock006_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock006_lod_normal.tga", //rock006_lod_normal
    "rock006_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock006_lod_ao.tga", //rock006_lod_ao
    "rock007_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock007_lod_albedo.tga", //rock007_lod_albedo
    "rock007_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock007_lod_normal.tga", //rock007_lod_normal
    "rock007_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock007_lod_ao.tga", //rock007_lod_ao
    "rock008_lod_albedo",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock008_lod_albedo.tga", //rock008_lod_albedo
    "rock008_lod_normal",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock008_lod_normal.tga", //rock008_lod_normal
    "rock008_lod_ao",                   "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock008_lod_ao.tga", //rock008_lod_ao
    "rock009_010_lod_albedo",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock009_010_lod_albedo.tga", //rock009_010_lod_albedo
    "rock009_010_lod_normal",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock009_010_lod_normal.tga", //rock009_010_lod_normal
    "rock009_010_lod_ao",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock009_010_lod_ao.tga", //rock009_010_lod_ao
    "rock011_012_lod_albedo",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock011_012_lod_albedo.tga", //rock011_012_lod_albedo
    "rock011_012_lod_normal",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock011_012_lod_normal.tga", //rock011_012_lod_normal
    "rock011_012_lod_ao",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock011_012_lod_ao.tga", //rock011_012_lod_ao
    "rock013_022_lod_albedo",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock013_022_lod_albedo.tga", //rock013_022_lod_albedo
    "rock013_022_lod_normal",           "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock013_022_lod_normal.tga", //rock013_022_lod_normal
    "rock013_022_lod_ao",               "2D",            "false",                  "false",                           "Assets/Texture/Model/rock_lod/rock013_022_lod_ao.tga", //rock013_022_lod_ao
};
static FTexturePixelFormatType g_TextureFormats[g_TextureCount] = 
{
    F_TexturePixelFormat_R8G8B8A8_SRGB, //terrain
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texturecubemap

    F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white
    F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d
    
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock001_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock001_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock001_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock002_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock002_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock002_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock003_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock003_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock003_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock004_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock004_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock004_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock005_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock005_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock005_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock006_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock006_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock006_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock007_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock007_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock007_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock008_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock008_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock008_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock009_010_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock009_010_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock009_010_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock011_012_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock011_012_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock011_012_lod_ao
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock013_022_lod_albedo
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock013_022_lod_normal
    F_TexturePixelFormat_R8G8B8A8_SRGB, //rock013_022_lod_ao
};
static FTextureFilterType g_TextureFilters[g_TextureCount] = 
{
    F_TextureFilter_Bilinear, //terrain
    F_TextureFilter_Bilinear, //texturecubemap

    F_TextureFilter_Bilinear, //default_white
    F_TextureFilter_Bilinear, //texture2d
    
    F_TextureFilter_Bilinear, //rock001_lod_albedo
    F_TextureFilter_Bilinear, //rock001_lod_normal
    F_TextureFilter_Bilinear, //rock001_lod_ao
    F_TextureFilter_Bilinear, //rock002_lod_albedo
    F_TextureFilter_Bilinear, //rock002_lod_normal
    F_TextureFilter_Bilinear, //rock002_lod_ao
    F_TextureFilter_Bilinear, //rock003_lod_albedo
    F_TextureFilter_Bilinear, //rock003_lod_normal
    F_TextureFilter_Bilinear, //rock003_lod_ao
    F_TextureFilter_Bilinear, //rock004_lod_albedo
    F_TextureFilter_Bilinear, //rock004_lod_normal
    F_TextureFilter_Bilinear, //rock004_lod_ao
    F_TextureFilter_Bilinear, //rock005_lod_albedo
    F_TextureFilter_Bilinear, //rock005_lod_normal
    F_TextureFilter_Bilinear, //rock005_lod_ao
    F_TextureFilter_Bilinear, //rock006_lod_albedo
    F_TextureFilter_Bilinear, //rock006_lod_normal
    F_TextureFilter_Bilinear, //rock006_lod_ao
    F_TextureFilter_Bilinear, //rock007_lod_albedo
    F_TextureFilter_Bilinear, //rock007_lod_normal
    F_TextureFilter_Bilinear, //rock007_lod_ao
    F_TextureFilter_Bilinear, //rock008_lod_albedo
    F_TextureFilter_Bilinear, //rock008_lod_normal
    F_TextureFilter_Bilinear, //rock008_lod_ao
    F_TextureFilter_Bilinear, //rock009_010_lod_albedo
    F_TextureFilter_Bilinear, //rock009_010_lod_normal
    F_TextureFilter_Bilinear, //rock009_010_lod_ao
    F_TextureFilter_Bilinear, //rock011_012_lod_albedo
    F_TextureFilter_Bilinear, //rock011_012_lod_normal
    F_TextureFilter_Bilinear, //rock011_012_lod_ao
    F_TextureFilter_Bilinear, //rock013_022_lod_albedo
    F_TextureFilter_Bilinear, //rock013_022_lod_normal
    F_TextureFilter_Bilinear, //rock013_022_lod_ao
};
static FTextureAddressingType g_TextureAddressings[g_TextureCount] = 
{
    F_TextureAddressing_Clamp, //terrain
    F_TextureAddressing_Wrap, //texturecubemap

    F_TextureAddressing_Clamp, //default_white
    F_TextureAddressing_Clamp, //texture2d
    
    F_TextureAddressing_Clamp, //rock001_lod_albedo
    F_TextureAddressing_Clamp, //rock001_lod_normal
    F_TextureAddressing_Clamp, //rock001_lod_ao
    F_TextureAddressing_Clamp, //rock002_lod_albedo
    F_TextureAddressing_Clamp, //rock002_lod_normal
    F_TextureAddressing_Clamp, //rock002_lod_ao
    F_TextureAddressing_Clamp, //rock003_lod_albedo
    F_TextureAddressing_Clamp, //rock003_lod_normal
    F_TextureAddressing_Clamp, //rock003_lod_ao
    F_TextureAddressing_Clamp, //rock004_lod_albedo
    F_TextureAddressing_Clamp, //rock004_lod_normal
    F_TextureAddressing_Clamp, //rock004_lod_ao
    F_TextureAddressing_Clamp, //rock005_lod_albedo
    F_TextureAddressing_Clamp, //rock005_lod_normal
    F_TextureAddressing_Clamp, //rock005_lod_ao
    F_TextureAddressing_Clamp, //rock006_lod_albedo
    F_TextureAddressing_Clamp, //rock006_lod_normal
    F_TextureAddressing_Clamp, //rock006_lod_ao
    F_TextureAddressing_Clamp, //rock007_lod_albedo
    F_TextureAddressing_Clamp, //rock007_lod_normal
    F_TextureAddressing_Clamp, //rock007_lod_ao
    F_TextureAddressing_Clamp, //rock008_lod_albedo
    F_TextureAddressing_Clamp, //rock008_lod_normal
    F_TextureAddressing_Clamp, //rock008_lod_ao
    F_TextureAddressing_Clamp, //rock009_010_lod_albedo
    F_TextureAddressing_Clamp, //rock009_010_lod_normal
    F_TextureAddressing_Clamp, //rock009_010_lod_ao
    F_TextureAddressing_Clamp, //rock011_012_lod_albedo
    F_TextureAddressing_Clamp, //rock011_012_lod_normal
    F_TextureAddressing_Clamp, //rock011_012_lod_ao
    F_TextureAddressing_Clamp, //rock013_022_lod_albedo
    F_TextureAddressing_Clamp, //rock013_022_lod_normal
    F_TextureAddressing_Clamp, //rock013_022_lod_ao
};
static FTextureBorderColorType g_TextureBorderColors[g_TextureCount] = 
{
    F_TextureBorderColor_OpaqueBlack, //terrain
    F_TextureBorderColor_OpaqueBlack, //texturecubemap

    F_TextureBorderColor_OpaqueBlack, //default_white
    F_TextureBorderColor_OpaqueBlack, //texture2d
    
    F_TextureBorderColor_OpaqueBlack, //rock001_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock001_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock001_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock002_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock002_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock002_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock003_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock003_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock003_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock004_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock004_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock004_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock005_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock005_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock005_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock006_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock006_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock006_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock007_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock007_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock007_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock008_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock008_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock008_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock009_010_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock009_010_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock009_010_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock011_012_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock011_012_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock011_012_lod_ao
    F_TextureBorderColor_OpaqueBlack, //rock013_022_lod_albedo
    F_TextureBorderColor_OpaqueBlack, //rock013_022_lod_normal
    F_TextureBorderColor_OpaqueBlack, //rock013_022_lod_ao
};
static int g_TextureSizes[3 * g_TextureCount] = 
{
    512,    512,    1, //terrain
    512,    512,    1, //texturecubemap

     64,     64,    1, //default_white
    512,    512,    1, //texture2d
    
   1024,   1024,    1, //rock001_lod_albedo
   1024,   1024,    1, //rock001_lod_normal
   1024,   1024,    1, //rock001_lod_ao
   1024,   1024,    1, //rock002_lod_albedo
   1024,   1024,    1, //rock002_lod_normal
   1024,   1024,    1, //rock002_lod_ao
   1024,   1024,    1, //rock003_lod_albedo
   1024,   1024,    1, //rock003_lod_normal
   1024,   1024,    1, //rock003_lod_ao
   1024,   1024,    1, //rock004_lod_albedo
   1024,   1024,    1, //rock004_lod_normal
   1024,   1024,    1, //rock004_lod_ao
   1024,   1024,    1, //rock005_lod_albedo
   1024,   1024,    1, //rock005_lod_normal
   1024,   1024,    1, //rock005_lod_ao
   1024,   1024,    1, //rock006_lod_albedo
   1024,   1024,    1, //rock006_lod_normal
   1024,   1024,    1, //rock006_lod_ao
   1024,   1024,    1, //rock007_lod_albedo
   1024,   1024,    1, //rock007_lod_normal
   1024,   1024,    1, //rock007_lod_ao
   1024,   1024,    1, //rock008_lod_albedo
   1024,   1024,    1, //rock008_lod_normal
   1024,   1024,    1, //rock008_lod_ao
   1024,   1024,    1, //rock009_010_lod_albedo
   1024,   1024,    1, //rock009_010_lod_normal
   1024,   1024,    1, //rock009_010_lod_ao
   1024,   1024,    1, //rock011_012_lod_albedo
   1024,   1024,    1, //rock011_012_lod_normal
   1024,   1024,    1, //rock011_012_lod_ao
   1024,   1024,    1, //rock013_022_lod_albedo
   1024,   1024,    1, //rock013_022_lod_normal
   1024,   1024,    1, //rock013_022_lod_ao
};
static float g_TextureAnimChunks[2 * g_TextureCount] = 
{
    0,    0, //terrain
    0,    0, //texturecubemap

    0,    0, //default_white
    0,    0, //texture2d
    
    0,    0, //rock001_lod_albedo
    0,    0, //rock001_lod_normal
    0,    0, //rock001_lod_ao
    0,    0, //rock002_lod_albedo
    0,    0, //rock002_lod_normal
    0,    0, //rock002_lod_ao
    0,    0, //rock003_lod_albedo
    0,    0, //rock003_lod_normal
    0,    0, //rock003_lod_ao
    0,    0, //rock004_lod_albedo
    0,    0, //rock004_lod_normal
    0,    0, //rock004_lod_ao
    0,    0, //rock005_lod_albedo
    0,    0, //rock005_lod_normal
    0,    0, //rock005_lod_ao
    0,    0, //rock006_lod_albedo
    0,    0, //rock006_lod_normal
    0,    0, //rock006_lod_ao
    0,    0, //rock007_lod_albedo
    0,    0, //rock007_lod_normal
    0,    0, //rock007_lod_ao
    0,    0, //rock008_lod_albedo
    0,    0, //rock008_lod_normal
    0,    0, //rock008_lod_ao
    0,    0, //rock009_010_lod_albedo
    0,    0, //rock009_010_lod_normal
    0,    0, //rock009_010_lod_ao
    0,    0, //rock011_012_lod_albedo
    0,    0, //rock011_012_lod_normal
    0,    0, //rock011_012_lod_ao
    0,    0, //rock013_022_lod_albedo
    0,    0, //rock013_022_lod_normal
    0,    0, //rock013_022_lod_ao
};


/////////////////////////// DescriptorSetLayout /////////////////
static const int g_DescriptorSetLayoutCount = 6;
static const char* g_DescriptorSetLayoutNames[g_DescriptorSetLayoutCount] =
{
    "Pass-Object-Material-Instance-TextureFS",
    "Pass-Object-Material-CullInstance-TextureFS-BufferRWObjectCullInstance-BufferRWResultCB",
    "Pass-Object-Material-Instance-TextureDepthShadow",
    "Pass-Object-Material-Instance-TextureFS-TextureDepthShadow",

    "Pass-Object-Material-Instance-TextureFS-TextureFS",
    "Pass-Object-Material-Instance-TextureFS-TextureFS-TextureFS",
};


/////////////////////////// Shader //////////////////////////////
static const int g_ShaderCount = 15;
static const char* g_ShaderModulePaths[3 * g_ShaderCount] = 
{
    //name                                                     //type               //path
    ///////////////////////////////////////// vert /////////////////////////////////////////
    "vert_standard_mesh_opaque_tex2d_lit",                     "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.vert.spv", //standard_mesh_opaque_tex2d_lit vert
    "vert_standard_mesh_opaque_tex2d_lit_cull",                "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit_cull.vert.spv", //standard_mesh_opaque_tex2d_lit_cull vert
    "vert_standard_mesh_opaque_texcubemap_lit",                "vert",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.vert.spv", //standard_mesh_opaque_texcubemap_lit vert
    "vert_standard_mesh_opaque_tex2d_lit_shadow",              "vert",              "Assets/Shader/standard_mesh_opaque_tex2d_lit_shadow.vert.spv", //standard_mesh_opaque_tex2d_lit_shadow vert
    
    "vert_standard_debug_shadow_depth",                        "vert",              "Assets/Shader/standard_debug_shadow_depth.vert.spv", //standard_debug_shadow_depth vert
    
    "vert_standard_mesh_opaque_normalmap_lit",                 "vert",              "Assets/Shader/standard_mesh_opaque_normalmap_lit.vert.spv", //standard_mesh_opaque_normalmap_lit vert
    "vert_standard_mesh_opaque_tree_alphatest_lit",            "vert",              "Assets/Shader/standard_mesh_opaque_tree_alphatest_lit.vert.spv", //standard_mesh_opaque_tree_alphatest_lit vert
    "vert_standard_mesh_opaque_grass_alphatest_lit",           "vert",              "Assets/Shader/standard_mesh_opaque_grass_alphatest_lit.vert.spv", //standard_mesh_opaque_grass_alphatest_lit vert  

    ///////////////////////////////////////// tesc /////////////////////////////////////////
   

    ///////////////////////////////////////// tese /////////////////////////////////////////
   

    ///////////////////////////////////////// geom /////////////////////////////////////////


    ///////////////////////////////////////// frag /////////////////////////////////////////
    "frag_standard_mesh_opaque_tex2d_lit",                     "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit.frag.spv", //standard_mesh_opaque_tex2d_lit frag
    "frag_standard_mesh_opaque_texcubemap_lit",                "frag",              "Assets/Shader/standard_mesh_opaque_texcubemap_lit.frag.spv", //standard_mesh_opaque_texcubemap_lit frag
    "frag_standard_mesh_opaque_tex2d_lit_shadow",              "frag",              "Assets/Shader/standard_mesh_opaque_tex2d_lit_shadow.frag.spv", //standard_mesh_opaque_tex2d_lit_shadow frag
    
    "frag_standard_debug_shadow_depth",                        "frag",              "Assets/Shader/standard_debug_shadow_depth.frag.spv", //standard_debug_shadow_depth frag
    
    "frag_standard_mesh_opaque_normalmap_lit",                 "frag",              "Assets/Shader/standard_mesh_opaque_normalmap_lit.frag.spv", //standard_mesh_opaque_normalmap_lit frag
    "frag_standard_mesh_opaque_tree_alphatest_lit",            "frag",              "Assets/Shader/standard_mesh_opaque_tree_alphatest_lit.frag.spv", //standard_mesh_opaque_tree_alphatest_lit frag
    "frag_standard_mesh_opaque_grass_alphatest_lit",           "frag",              "Assets/Shader/standard_mesh_opaque_grass_alphatest_lit.frag.spv", //standard_mesh_opaque_grass_alphatest_lit frag

    ///////////////////////////////////////// comp /////////////////////////////////////////

};


/////////////////////////// CullInfo ////////////////////////////
static const int g_ObjectRend_CullInfo_Count = 1;
static const char* g_ObjectRend_CullingDesc[4 * g_ObjectRend_CullInfo_Count] = 
{
    "vert_standard_mesh_opaque_tex2d_lit",      "vert_standard_mesh_opaque_tex2d_lit_cull",     "Pass-Object-Material-Instance-TextureFS",      "Pass-Object-Material-CullInstance-TextureFS-BufferRWObjectCullInstance-BufferRWResultCB", 

};


/////////////////////////// Object //////////////////////////////
static const int g_Object_Count = 6; //28;
static const char* g_Object_Configs[2 * g_Object_Count] = 
{
    //Object Name                          //Mesh Name          
    "object_terrain",                      "plane", //object_terrain
    "object_skybox",                       "cube", //object_skybox
    "object_depth",                        "plane", //object_depth

    "object_cube",                         "cube", //object_cube   
    "object_sphere",                       "sphere", //object_sphere
    
//    "object_grass_lod",                    "grass_lod", //grass_lod

    "object_rock001_lod",                  "rock001_lod", //object_rock001_lod
    // "object_rock002_lod",                  "rock002_lod", //object_rock002_lod
    // "object_rock003_lod",                  "rock003_lod", //object_rock003_lod
    // "object_rock004_lod",                  "rock004_lod", //object_rock004_lod
    // "object_rock005_lod",                  "rock005_lod", //object_rock005_lod
    // "object_rock006_lod",                  "rock006_lod", //object_rock006_lod
    // "object_rock007_lod",                  "rock007_lod", //object_rock007_lod
    // "object_rock008_lod",                  "rock008_lod", //object_rock008_lod
    // "object_rock009_lod",                  "rock009_lod", //object_rock009_lod
    // "object_rock010_lod",                  "rock010_lod", //object_rock010_lod
    // "object_rock011_lod",                  "rock011_lod", //object_rock011_lod
    // "object_rock012_lod",                  "rock012_lod", //object_rock012_lod
    // "object_rock013_lod",                  "rock013_lod", //object_rock013_lod
    // "object_rock014_lod",                  "rock014_lod", //object_rock014_lod
    // "object_rock015_lod",                  "rock015_lod", //object_rock015_lod
    // "object_rock016_lod",                  "rock016_lod", //object_rock016_lod
    // "object_rock017_lod",                  "rock017_lod", //object_rock017_lod
    // "object_rock018_lod",                  "rock018_lod", //object_rock018_lod
    // "object_rock019_lod",                  "rock019_lod", //object_rock019_lod
    // "object_rock020_lod",                  "rock020_lod", //object_rock020_lod
    // "object_rock021_lod",                  "rock021_lod", //object_rock021_lod
    // "object_rock022_lod",                  "rock022_lod", //object_rock022_lod
};
static const char* g_Object_MeshSubsUsed[g_Object_Count] =
{
    "0", //object_terrain
    "0", //object_skybox
    "0", //object_depth

    "0", //object_cube
    "0", //object_sphere
    
    //"0;1;2", //object_grass_lod

    // "0;1;2", //object_rock001_lod
    // "0;1;2", //object_rock002_lod
    // "0;1;2", //object_rock003_lod
    // "0;1;2", //object_rock004_lod
    // "0;1;2", //object_rock005_lod
    // "0;1;2", //object_rock006_lod
    // "0;1;2", //object_rock007_lod
    // "0;1;2", //object_rock008_lod
    // "0;1;2", //object_rock009_lod
    // "0;1;2", //object_rock010_lod
    // "0;1;2", //object_rock011_lod
    // "0;1;2", //object_rock012_lod
    // "0;1;2", //object_rock013_lod
    // "0;1;2", //object_rock014_lod
    // "0;1;2", //object_rock015_lod
    // "0;1;2", //object_rock016_lod
    // "0;1;2", //object_rock017_lod
    // "0;1;2", //object_rock018_lod
    // "0;1;2", //object_rock019_lod
    // "0;1;2", //object_rock020_lod
    // "0;1;2", //object_rock021_lod
    // "0;1;2", //object_rock022_lod

 //   "0", //object_grass_lod

    "0", //object_rock001_lod
    // "0", //object_rock002_lod
    // "0", //object_rock003_lod
    // "0", //object_rock004_lod
    // "0", //object_rock005_lod
    // "0", //object_rock006_lod
    // "0", //object_rock007_lod
    // "0", //object_rock008_lod
    // "0", //object_rock009_lod
    // "0", //object_rock010_lod
    // "0", //object_rock011_lod
    // "0", //object_rock012_lod
    // "0", //object_rock013_lod
    // "0", //object_rock014_lod
    // "0", //object_rock015_lod
    // "0", //object_rock016_lod
    // "0", //object_rock017_lod
    // "0", //object_rock018_lod
    // "0", //object_rock019_lod
    // "0", //object_rock020_lod
    // "0", //object_rock021_lod
    // "0", //object_rock022_lod
};  

static float g_Object_InstanceGap = 1.5f;
static int g_Object_InstanceExtCount[g_Object_Count] =
{
    0, //object_terrain
    0, //object_skybox
    0, //object_depth

    0, //object_cube 
    0, //object_sphere 
    
 //   16, //object_grass_lod 

    16, //object_rock001_lod 
    // 16, //object_rock002_lod 
    // 16, //object_rock003_lod 
    // 16, //object_rock004_lod 
    // 16, //object_rock005_lod 
    // 16, //object_rock006_lod 
    // 16, //object_rock007_lod 
    // 16, //object_rock008_lod 
    // 16, //object_rock009_lod 
    // 16, //object_rock010_lod 
    // 16, //object_rock011_lod 
    // 16, //object_rock012_lod 
    // 16, //object_rock013_lod 
    // 16, //object_rock014_lod 
    // 16, //object_rock015_lod 
    // 16, //object_rock016_lod 
    // 16, //object_rock017_lod 
    // 16, //object_rock018_lod 
    // 16, //object_rock019_lod 
    // 16, //object_rock020_lod 
    // 16, //object_rock021_lod 
    // 16, //object_rock022_lod 
};
static bool g_Object_IsShows[] = 
{
    true, //object_terrain
    true, //object_skybox
    true, //object_depth

    true, //object_cube
    true, //object_sphere
    
 //   true, //object_grass_lod

    true, //object_rock001_lod
    // true, //object_rock002_lod
    // true, //object_rock003_lod
    // true, //object_rock004_lod
    // true, //object_rock005_lod
    // true, //object_rock006_lod
    // true, //object_rock007_lod
    // true, //object_rock008_lod
    // true, //object_rock009_lod
    // true, //object_rock010_lod
    // true, //object_rock011_lod
    // true, //object_rock012_lod
    // true, //object_rock013_lod
    // true, //object_rock014_lod
    // true, //object_rock015_lod
    // true, //object_rock016_lod
    // true, //object_rock017_lod
    // true, //object_rock018_lod
    // true, //object_rock019_lod
    // true, //object_rock020_lod
    // true, //object_rock021_lod
    // true, //object_rock022_lod
};
static bool g_Object_IsRotates[g_Object_Count] =
{
    false, //object_terrain
    false, //object_skybox
    false, //object_depth

    false, //object_cube
    false, //object_sphere
    
 //   false, //object_grass_lod

    false, //object_rock001_lod
    // false, //object_rock002_lod
    // false, //object_rock003_lod
    // false, //object_rock004_lod
    // false, //object_rock005_lod
    // false, //object_rock006_lod
    // false, //object_rock007_lod
    // false, //object_rock008_lod
    // false, //object_rock009_lod
    // false, //object_rock010_lod
    // false, //object_rock011_lod
    // false, //object_rock012_lod
    // false, //object_rock013_lod
    // false, //object_rock014_lod
    // false, //object_rock015_lod
    // false, //object_rock016_lod
    // false, //object_rock017_lod
    // false, //object_rock018_lod
    // false, //object_rock019_lod
    // false, //object_rock020_lod
    // false, //object_rock021_lod
    // false, //object_rock022_lod
};
static bool g_Object_IsLightings[g_Object_Count] =
{
    true, //object_terrain
    true, //object_skybox
    false, //object_depth

    true, //object_cube
    true, //object_sphere
    
 //   true, //object_grass_lod

    true, //object_rock001_lod
    // true, //object_rock002_lod
    // true, //object_rock003_lod
    // true, //object_rock004_lod
    // true, //object_rock005_lod
    // true, //object_rock006_lod
    // true, //object_rock007_lod
    // true, //object_rock008_lod
    // true, //object_rock009_lod
    // true, //object_rock010_lod
    // true, //object_rock011_lod
    // true, //object_rock012_lod
    // true, //object_rock013_lod
    // true, //object_rock014_lod
    // true, //object_rock015_lod
    // true, //object_rock016_lod
    // true, //object_rock017_lod
    // true, //object_rock018_lod
    // true, //object_rock019_lod
    // true, //object_rock020_lod
    // true, //object_rock021_lod
    // true, //object_rock022_lod
};
static bool g_Object_IsIndirectDraw[g_Object_Count] =
{
    false, //object_terrain
    false, //object_skybox
    false, //object_depth

    false, //object_cube
    false, //object_sphere
    
 //   false, //object_grass_lod

    false, //object_rock001_lod
    // false, //object_rock002_lod
    // false, //object_rock003_lod
    // false, //object_rock004_lod
    // false, //object_rock005_lod
    // false, //object_rock006_lod
    // false, //object_rock007_lod
    // false, //object_rock008_lod
    // false, //object_rock009_lod
    // false, //object_rock010_lod
    // false, //object_rock011_lod
    // false, //object_rock012_lod
    // false, //object_rock013_lod
    // false, //object_rock014_lod
    // false, //object_rock015_lod
    // false, //object_rock016_lod
    // false, //object_rock017_lod
    // false, //object_rock018_lod
    // false, //object_rock019_lod
    // false, //object_rock020_lod
    // false, //object_rock021_lod
    // false, //object_rock022_lod
};


/////////////////////////// ObjectRend //////////////////////////
static const int g_ObjectRend_Count = 6; //28;
static const char* g_ObjectRend_Configs[7 * g_ObjectRend_Count] = 
{
    //Object Rend Name                     //Texture VS            //TextureTESC                    //TextureTESE               //TextureGS            //Texture FS                                                                    //Texture CS
    "object_terrain-1",                    "",                     "",                              "",                         "",                    "terrain",                                                                      "", //object_terrain-1
    "object_skybox-1",                     "",                     "",                              "",                         "",                    "texturecubemap",                                                               "", //object_skybox-1
    "object_depth-1",                      "",                     "",                              "",                         "",                    "",                                                                             "", //object_depth-1

    "object_cube-1",                       "",                     "",                              "",                         "",                    "texture2d",                                                                    "", //object_cube-1
    "object_sphere-1",                     "",                     "",                              "",                         "",                    "texture2d",                                                                    "", //object_sphere-1
    
//    "object_grass_lod-1",                  "",                     "",                              "",                         "",                    "default_white",                                                                "", //object_grass_lod-1
    
    "object_rock001_lod-1",                "",                     "",                              "",                         "",                    "rock001_lod_albedo",                                                           "", //object_rock001_lod-1
//     "object_rock002_lod-1",                "",                     "",                              "",                         "",                    "rock002_lod_albedo",                                                           "", //object_rock002_lod-1
//     "object_rock003_lod-1",                "",                     "",                              "",                         "",                    "rock003_lod_albedo",                                                           "", //object_rock003_lod-1
//     "object_rock004_lod-1",                "",                     "",                              "",                         "",                    "rock004_lod_albedo",                                                           "", //object_rock004_lod-1
//     "object_rock005_lod-1",                "",                     "",                              "",                         "",                    "rock005_lod_albedo",                                                           "", //object_rock005_lod-1
//     "object_rock006_lod-1",                "",                     "",                              "",                         "",                    "rock006_lod_albedo",                                                           "", //object_rock006_lod-1
//     "object_rock007_lod-1",                "",                     "",                              "",                         "",                    "rock007_lod_albedo",                                                           "", //object_rock007_lod-1
//     "object_rock008_lod-1",                "",                     "",                              "",                         "",                    "rock008_lod_albedo",                                                           "", //object_rock008_lod-1
//     "object_rock009_lod-1",                "",                     "",                              "",                         "",                    "rock009_010_lod_albedo",                                                       "", //object_rock009_lod-1
//     "object_rock010_lod-1",                "",                     "",                              "",                         "",                    "rock009_010_lod_albedo",                                                       "", //object_rock010_lod-1
//     "object_rock011_lod-1",                "",                     "",                              "",                         "",                    "rock011_012_lod_albedo",                                                       "", //object_rock011_lod-1
//     "object_rock012_lod-1",                "",                     "",                              "",                         "",                    "rock011_012_lod_albedo",                                                       "", //object_rock012_lod-1
//     "object_rock013_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock013_lod-1
//     "object_rock014_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock014_lod-1
//     "object_rock015_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock015_lod-1
//     "object_rock016_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock016_lod-1
//     "object_rock017_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock017_lod-1
//     "object_rock018_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock018_lod-1
//     "object_rock019_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock019_lod-1
//     "object_rock020_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock020_lod-1
//     "object_rock021_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock021_lod-1
//     "object_rock022_lod-1",                "",                     "",                              "",                         "",                    "rock013_022_lod_albedo",                                                       "", //object_rock022_lod-1
};
static const char* g_ObjectRend_NameShaderModules[6 * g_ObjectRend_Count] = 
{
    //vert                                                  //tesc                                          //tese                                      //geom                      //frag                                                  //comp
    "vert_standard_mesh_opaque_tex2d_lit_shadow",           "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit_shadow",           "", //object_terrain-1
    "vert_standard_mesh_opaque_texcubemap_lit",             "",                                             "",                                         "",                         "frag_standard_mesh_opaque_texcubemap_lit",             "", //object_skybox-1
    "vert_standard_debug_shadow_depth",                     "",                                             "",                                         "",                         "frag_standard_debug_shadow_depth",                     "", //object_depth-1

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_cube-1
    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_sphere-1
    
//    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_grass_lod-1

    "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock001_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock002_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock003_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock004_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock005_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock006_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock007_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock008_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock009_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock010_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock011_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock012_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock013_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock014_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock015_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock016_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock017_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock018_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock019_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock020_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock021_lod-1
    // "vert_standard_mesh_opaque_tex2d_lit",                  "",                                             "",                                         "",                         "frag_standard_mesh_opaque_tex2d_lit",                  "", //object_rock022_lod-1

};
static const char* g_ObjectRend_NameDescriptorSetLayouts[2 * g_ObjectRend_Count] = 
{
    //Pipeline Graphics                                                 //Pipeline Compute
    "Pass-Object-Material-Instance-TextureFS-TextureDepthShadow",       "", //object_terrain-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_skybox-1
    "Pass-Object-Material-Instance-TextureDepthShadow",                 "", //object_depth-1

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_cube-1
    "Pass-Object-Material-Instance-TextureFS",                          "", //object_sphere-1

//    "Pass-Object-Material-Instance-TextureFS",                          "", //object_grass_lod-1

    "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock001_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock002_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock003_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock004_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock005_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock006_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock007_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock008_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock009_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock010_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock011_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock012_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock013_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock014_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock015_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock016_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock017_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock018_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock019_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock020_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock021_lod-1
    // "Pass-Object-Material-Instance-TextureFS",                          "", //object_rock022_lod-1

};
static FVector3 g_ObjectRend_Tranforms[3 * g_ObjectRend_Count] = 
{   
    FVector3(    0,  0.0,    0.0),    FVector3(     0,   0,  0),    FVector3(   10.0f,     10.0f,     10.0f), //object_terrain-1
    FVector3(    0,  0.0,    0.0),    FVector3(     0,   0,  0),    FVector3(  500.0f,    500.0f,    500.0f), //object_skybox-1
    FVector3(    0,    1,  15.0f),    FVector3(     0,   0,  0),    FVector3(   0.04f,      1.0f,     0.04f), //object_depth-1
 
    FVector3( 2.0f,    1,   0.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_cube-1
    FVector3(-2.0f,    1,   0.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_sphere-1

//    FVector3( 0.0f,    1,   0.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_grass_lod-1

    FVector3( 0.0f,    1,  -1.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock001_lod-1
    // FVector3( 0.0f,    1,  -2.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock002_lod-1
    // FVector3( 0.0f,    1,  -3.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock003_lod-1
    // FVector3( 0.0f,    1,  -4.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock004_lod-1
    // FVector3( 0.0f,    1,  -5.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock005_lod-1
    // FVector3( 0.0f,    1,  -6.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock006_lod-1
    // FVector3( 0.0f,    1,  -7.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock007_lod-1
    // FVector3( 0.0f,    1,  -8.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock008_lod-1
    // FVector3( 0.0f,    1,  -9.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock009_lod-1
    // FVector3( 0.0f,    1, -10.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock010_lod-1
    // FVector3( 0.0f,    1, -11.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock011_lod-1
    // FVector3( 0.0f,    1,  11.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock012_lod-1
    // FVector3( 0.0f,    1,  10.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock013_lod-1
    // FVector3( 0.0f,    1,   9.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock014_lod-1
    // FVector3( 0.0f,    1,   8.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock015_lod-1
    // FVector3( 0.0f,    1,   7.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock016_lod-1
    // FVector3( 0.0f,    1,   6.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock017_lod-1
    // FVector3( 0.0f,    1,   5.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock018_lod-1
    // FVector3( 0.0f,    1,   4.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock019_lod-1
    // FVector3( 0.0f,    1,   3.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock020_lod-1
    // FVector3( 0.0f,    1,   2.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock021_lod-1
    // FVector3( 0.0f,    1,   1.0f),    FVector3(     0,   0,  0),    FVector3(    1.0f,      1.0f,      1.0f), //object_rock022_lod-1

};
static bool g_ObjectRend_IsTransparents[g_ObjectRend_Count] = 
{
    false, //object_terrain-1
    false, //object_skybox-1
    false, //object_depth-1

    false, //object_cube-1
    false, //object_sphere-1

//    false, //object_grass_lod-1

    false, //object_rock001_lod-1
    // false, //object_rock002_lod-1
    // false, //object_rock003_lod-1
    // false, //object_rock004_lod-1
    // false, //object_rock005_lod-1
    // false, //object_rock006_lod-1
    // false, //object_rock007_lod-1
    // false, //object_rock008_lod-1
    // false, //object_rock009_lod-1
    // false, //object_rock010_lod-1
    // false, //object_rock011_lod-1
    // false, //object_rock012_lod-1
    // false, //object_rock013_lod-1
    // false, //object_rock014_lod-1
    // false, //object_rock015_lod-1
    // false, //object_rock016_lod-1
    // false, //object_rock017_lod-1
    // false, //object_rock018_lod-1
    // false, //object_rock019_lod-1
    // false, //object_rock020_lod-1
    // false, //object_rock021_lod-1
    // false, //object_rock022_lod-1

};
static bool g_ObjectRend_IsCastShadows[g_ObjectRend_Count] = 
{
    false, //object_terrain-1
    false, //object_skybox-1
    false, //object_depth-1

    true, //object_cube-1
    true, //object_sphere-1
    
 //   true, //object_grass_lod-1
    
    true, //object_rock001_lod-1
    // true, //object_rock002_lod-1
    // true, //object_rock003_lod-1
    // true, //object_rock004_lod-1
    // true, //object_rock005_lod-1
    // true, //object_rock006_lod-1
    // true, //object_rock007_lod-1
    // true, //object_rock008_lod-1
    // true, //object_rock009_lod-1
    // true, //object_rock010_lod-1
    // true, //object_rock011_lod-1
    // true, //object_rock012_lod-1
    // true, //object_rock013_lod-1
    // true, //object_rock014_lod-1
    // true, //object_rock015_lod-1
    // true, //object_rock016_lod-1
    // true, //object_rock017_lod-1
    // true, //object_rock018_lod-1
    // true, //object_rock019_lod-1
    // true, //object_rock020_lod-1
    // true, //object_rock021_lod-1
    // true, //object_rock022_lod-1
};
static bool g_ObjectRend_IsReceiveShadows[g_ObjectRend_Count] = 
{
    true, //object_terrain-1
    false, //object_skybox-1
    false, //object_depth-1

    false, //object_cube-1
    false, //object_sphere-1
   
 //   false, //object_grass_lod-1
    
    false, //object_rock001_lod-1
    // false, //object_rock002_lod-1
    // false, //object_rock003_lod-1
    // false, //object_rock004_lod-1
    // false, //object_rock005_lod-1
    // false, //object_rock006_lod-1
    // false, //object_rock007_lod-1
    // false, //object_rock008_lod-1
    // false, //object_rock009_lod-1
    // false, //object_rock010_lod-1
    // false, //object_rock011_lod-1
    // false, //object_rock012_lod-1
    // false, //object_rock013_lod-1
    // false, //object_rock014_lod-1
    // false, //object_rock015_lod-1
    // false, //object_rock016_lod-1
    // false, //object_rock017_lod-1
    // false, //object_rock018_lod-1
    // false, //object_rock019_lod-1
    // false, //object_rock020_lod-1
    // false, //object_rock021_lod-1
    // false, //object_rock022_lod-1
};
static bool g_ObjectRend_IsCanCullings[g_ObjectRend_Count] = 
{
    false, //object_terrain-1
    false, //object_skybox-1
    false, //object_depth-1

    true, //object_cube-1
    true, //object_sphere-1
    
 //   true, //object_grass_lod-1
    
    true, //object_rock001_lod-1
    // true, //object_rock002_lod-1
    // true, //object_rock003_lod-1
    // true, //object_rock004_lod-1
    // true, //object_rock005_lod-1
    // true, //object_rock006_lod-1
    // true, //object_rock007_lod-1
    // true, //object_rock008_lod-1
    // true, //object_rock009_lod-1
    // true, //object_rock010_lod-1
    // true, //object_rock011_lod-1
    // true, //object_rock012_lod-1
    // true, //object_rock013_lod-1
    // true, //object_rock014_lod-1
    // true, //object_rock015_lod-1
    // true, //object_rock016_lod-1
    // true, //object_rock017_lod-1
    // true, //object_rock018_lod-1
    // true, //object_rock019_lod-1
    // true, //object_rock020_lod-1
    // true, //object_rock021_lod-1
    // true, //object_rock022_lod-1
};
static bool g_ObjectRend_IsShowBoundAABB_All_Line = false;
static bool g_ObjectRend_IsShowBoundSphere_All_Line = false;
static bool g_ObjectRend_IsShowBoundAABB_All_Flat = false;
static bool g_ObjectRend_IsShowBoundSphere_All_Flat = false;


/////////////////////////// ModelObjectRend /////////////////////
void Vulkan_020_Culling::ModelObjectRend::AddLine3D_AABB()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundAABB_Line[i] == nullptr)
        {
            FMatrix4 mat4 = this->objectCBs[i].g_MatWorld;
            FAABB aabb(this->pMeshSub->aabb);
            aabb.Transform(mat4);
            this->aPointerBoundAABB_Line[i] = this->pModelObject->pWindow->pEditorLineFlat3DCollector->AddLine3D_AABB(aabb, FMath::ms_clRed, false);
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strLine3D_AABB);
    }
}
void Vulkan_020_Culling::ModelObjectRend::RemoveLine3D_AABB()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundAABB_Line[i] != nullptr)
        {
            this->pModelObject->pWindow->pEditorLineFlat3DCollector->RemoveLineFlat3D(this->aPointerBoundAABB_Line[i], false);
            this->aPointerBoundAABB_Line[i] = nullptr;
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strLine3D_AABB);
    }
}

void Vulkan_020_Culling::ModelObjectRend::AddLine3D_Sphere()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundSphere_Line[i] == nullptr)
        {
            FMatrix4 mat4 = this->objectCBs[i].g_MatWorld;
            FSphere sphere(this->pMeshSub->sphere);
            sphere.Transform(mat4);
            this->aPointerBoundSphere_Line[i] = this->pModelObject->pWindow->pEditorLineFlat3DCollector->AddLine3D_Sphere(sphere, FMath::ms_clRed, false);
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strLine3D_Sphere);
    }
}
void Vulkan_020_Culling::ModelObjectRend::RemoveLine3D_Sphere()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundSphere_Line[i] != nullptr)
        {
            this->pModelObject->pWindow->pEditorLineFlat3DCollector->RemoveLineFlat3D(this->aPointerBoundSphere_Line[i], false);
            this->aPointerBoundSphere_Line[i] = nullptr;
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strLine3D_Sphere);
    }
}


void Vulkan_020_Culling::ModelObjectRend::AddFlat3D_AABB()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundAABB_Flat[i] == nullptr)
        {
            FMatrix4 mat4 = this->objectCBs[i].g_MatWorld;
            FAABB aabb(this->pMeshSub->aabb);
            aabb.Transform(mat4);
            this->aPointerBoundAABB_Flat[i] = this->pModelObject->pWindow->pEditorLineFlat3DCollector->AddFlat3D_AABB(aabb, FMath::ms_clGreen, false);
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strFlat3D_AABB);
    }
}
void Vulkan_020_Culling::ModelObjectRend::RemoveFlat3D_AABB()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundAABB_Flat[i] != nullptr)
        {
            this->pModelObject->pWindow->pEditorLineFlat3DCollector->RemoveLineFlat3D(this->aPointerBoundAABB_Flat[i], false);
            this->aPointerBoundAABB_Flat[i] = nullptr;
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strFlat3D_AABB);
    }
}

void Vulkan_020_Culling::ModelObjectRend::AddFlat3D_Sphere()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundSphere_Flat[i] == nullptr)
        {
            FMatrix4 mat4 = this->objectCBs[i].g_MatWorld;
            FSphere sphere(this->pMeshSub->sphere);
            sphere.Transform(mat4);
            this->aPointerBoundSphere_Flat[i] = this->pModelObject->pWindow->pEditorLineFlat3DCollector->AddFlat3D_Sphere(sphere, FMath::ms_clGreen, false);
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strFlat3D_Sphere);
    }
}
void Vulkan_020_Culling::ModelObjectRend::RemoveFlat3D_Sphere()
{
    bool isNeedUpdateBuffer = false;
    int count_instance = this->pModelObject->countInstance;
    for (int i = 0; i < count_instance; i++)
    {
        if (this->aPointerBoundSphere_Flat[i] != nullptr)
        {
            this->pModelObject->pWindow->pEditorLineFlat3DCollector->RemoveLineFlat3D(this->aPointerBoundSphere_Flat[i], false);
            this->aPointerBoundSphere_Flat[i] = nullptr;
            isNeedUpdateBuffer = true;
        }
    }
    if (isNeedUpdateBuffer)
    {
        this->pModelObject->pWindow->pEditorLineFlat3DCollector->UpdateBuffer(EditorLineFlat3DCollector::c_strFlat3D_Sphere);
    }
}


/////////////////////////// ModelObjectRendIndirect /////////////
void Vulkan_020_Culling::ModelObjectRendIndirect::Destroy()
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

void Vulkan_020_Culling::ModelObjectRendIndirect::CleanupSwapChain()
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

void Vulkan_020_Culling::ModelObjectRendIndirect::SetupVertexIndexBuffer(const ModelObjectRendPtrVector& _aRends)
{
    F_Assert(_aRends.size() > 0 && "Vulkan_020_Culling::ModelObjectRendIndirect::SetupVertexIndexBuffer")
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
        F_Assert(false && "Vulkan_020_Culling::ModelObjectRendIndirect::SetupVertexIndexBuffer: No vertex data !")
    }
    this->poIndexCount = this->indices.size();
    this->poIndexBuffer_Size = this->poIndexCount * sizeof(uint32_t);
    this->poIndexBuffer_Data =  &this->indices[0];

    //2> createVertexBuffer
    this->pRend->pModelObject->pWindow->createVertexBuffer("Vertex-" + this->nameObjectRendIndirect, this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);

    //3> createIndexBuffer
    if (this->poIndexBuffer_Size > 0 &&
        this->poIndexBuffer_Data != nullptr)
    {
        this->pRend->pModelObject->pWindow->createIndexBuffer("Index-" + this->nameObjectRendIndirect, this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
    }
}

void Vulkan_020_Culling::ModelObjectRendIndirect::SetupUniformIndirectCommandBuffer()
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
            String nameBuffer = "ObjectConstants-" + FUtilString::SaveSizeT(j);
            this->pRend->pModelObject->pWindow->createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB[j], this->poBuffersMemory_ObjectCB[j]);
        }

        //MaterialConstants
        bufferSize = sizeof(MaterialConstants) * this->materialCBs.size();
        this->poBuffers_materialCB.resize(count_sci);
        this->poBuffersMemory_materialCB.resize(count_sci);
        for (size_t j = 0; j < count_sci; j++) 
        {
            String nameBuffer = "MaterialConstants-" + FUtilString::SaveSizeT(j);
            this->pRend->pModelObject->pWindow->createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_materialCB[j], this->poBuffersMemory_materialCB[j]);
        }

        //TessellationConstants
        if (pRend->isUsedTessellation)
        {
            bufferSize = sizeof(TessellationConstants) * this->tessellationCBs.size();
            this->poBuffers_tessellationCB.resize(count_sci);
            this->poBuffersMemory_tessellationCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "TessellationConstants-" + FUtilString::SaveSizeT(j);
                this->pRend->pModelObject->pWindow->createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_tessellationCB[j], this->poBuffersMemory_tessellationCB[j]);
            }
        }
    }

    //2> IndirectCommand Buffer
    {
        bufferSize = sizeof(VkDrawIndexedIndirectCommand) * this->indirectCommandCBs.size();
        this->pRend->pModelObject->pWindow->createVkBuffer("VkDrawIndexedIndirectCommand", bufferSize, VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_indirectCommandCB, this->poBuffersMemory_indirectCommandCB);
    }
}

void Vulkan_020_Culling::ModelObjectRendIndirect::UpdateUniformBuffer()
{
    this->objectCBs.clear();
    this->materialCBs.clear();
    this->tessellationCBs.clear();

    size_t count_rend = this->aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pR = this->aRends[i];
        MeshSub* pMeshSub = pR->pMeshSub;

        this->objectCBs.insert(this->objectCBs.end(), pR->objectCBs.begin(), pR->objectCBs.end());
        this->materialCBs.insert(this->materialCBs.end(), pR->materialCBs.begin(), pR->materialCBs.end());
        if (pRend->isUsedTessellation)
        {
            this->tessellationCBs.insert(this->tessellationCBs.end(), pR->tessellationCBs.begin(), pR->tessellationCBs.end());
        }
    }
}

void Vulkan_020_Culling::ModelObjectRendIndirect::UpdateIndirectCommandBuffer()
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



Vulkan_020_Culling::Vulkan_020_Culling(int width, int height, String name)
    : VulkanWindow(width, height, name)
    , m_isDrawIndirect(false)
    , m_isDrawIndirectMulti(false)
{
    this->isComputeCullFrustum = true;
    this->isComputeCullFrustumHizDepth = false;

    this->cfg_isRenderPassShadowMap = true;
    this->cfg_isRenderPassCull = true;
    this->cfg_isUseComputeShader = true;
    this->cfg_isCreateRenderComputeSycSemaphore = true;

    this->cfg_isImgui = true;
    this->imgui_IsEnable = true;
    this->cfg_isEditorCreate = true;
    this->cfg_isEditorGridShow = true;
    this->cfg_isEditorCameraAxisShow = true;
    this->cfg_isEditorCoordinateAxisShow = true;
    this->cfg_editorGrid_Color.a = 0.2f;
}

void Vulkan_020_Culling::setUpEnabledFeatures()
{
    VulkanWindow::setUpEnabledFeatures();

    if (this->poPhysicalEnabledFeatures.multiDrawIndirect)
    {
        this->m_isDrawIndirectMulti = true;
    }
    else
    {
        this->m_isDrawIndirectMulti = false;
        F_LogError("*********************** Vulkan_020_Culling::setUpEnabledFeatures: multiDrawIndirect is not supported !");
    }
}

void Vulkan_020_Culling::createDescriptorSetLayout_Custom()
{
    VulkanWindow::createDescriptorSetLayout_Custom();
}

void Vulkan_020_Culling::createCamera()
{
    VulkanWindow::createCamera();

    cameraReset();
}
    void Vulkan_020_Culling::cameraReset()
    {
        VulkanWindow::cameraReset();

        this->pCamera->SetPos(FVector3(0.0f, 18.0f, -18.0f));
        this->pCamera->SetEulerAngles(FVector3(46.0f, 0.0f, 0.0f));
        this->pCamera->SetFarZ(1000000.0f);
    }
void Vulkan_020_Culling::createLightMain()
{
    lightMainReset();
}
    void Vulkan_020_Culling::lightMainReset()
    {
        VulkanWindow::lightMainReset();

        this->mainLight.common.x = 0; //Directional Type
        this->mainLight.common.y = 1.0f; //Enable
        this->mainLight.common.z = 11; //Ambient + DiffuseLambert + SpecularBlinnPhong Type
        this->mainLight.position = FVector3(0.0f, 65.0f, -30.0f);
        FVector3 vEulerAngles(55.0f, 00.0f, 0.0f);
        this->mainLight.direction = FMath::ToDirection(vEulerAngles);

        this->shadowMainLight.zNear = 5.0f;
        this->shadowMainLight.depthSize = 4096;
        //Camera MainLight
        this->pCameraMainLight->SetPos(this->mainLight.position);
        this->pCameraMainLight->SetEulerAngles(vEulerAngles);
        this->pCameraMainLight->SetFovY(this->shadowMainLight.fov);
        this->pCameraMainLight->SetAspect(1.0f);
        this->pCameraMainLight->SetNearZ(this->shadowMainLight.zNear);
        this->pCameraMainLight->SetFarZ(this->shadowMainLight.zFar);
        this->pCameraMainLight->UpdateViewMatrix();
        this->pCameraMainLight->UpdateProjectionMatrix();
    }
void Vulkan_020_Culling::createShadowLightMain()
{

}


void Vulkan_020_Culling::loadModel_Custom()
{
    createMeshes();
    createTextures();
    createCullInfos();

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
                F_Assert(pMesh != nullptr && "Vulkan_020_Culling::loadModel_Custom")
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
                F_Assert(indexMeshSub >= 0 && indexMeshSub < count_mesh_sub && "Vulkan_020_Culling::loadModel_Custom")

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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Vertex), pTextureVS);
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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationControl), pTextureTESC);
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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_TessellationEvaluation), pTextureTESE);
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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureGS);
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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Fragment), pTextureFS);
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
                            pRend->AddTexture(F_GetShaderTypeName(F_Shader_Compute), pTextureCS);
                        }
                    }
                }

                String nameShaderTesc = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 1];
                String nameShaderTese = g_ObjectRend_NameShaderModules[6 * nIndexObjectRend + 2];
                if (!nameShaderTesc.empty() || !nameShaderTese.empty())
                {
                    pRend->isUsedTessellation = true;
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
                        VKPipelineCompute* pPipelineCompute = new VKPipelineCompute("PipelineC-Object");
                        pPipelineCompute->nameDescriptorSetLayout = nameDescriptorSetLayout;
                        pRend->AddPipelineCompute(pPipelineCompute);
                    }
                }

                //Common
                pRend->isTransparent = g_ObjectRend_IsTransparents[nIndexObjectRend];
                pRend->isCastShadow = g_ObjectRend_IsCastShadows[nIndexObjectRend];
                pRend->isReceiveShadow = g_ObjectRend_IsReceiveShadows[nIndexObjectRend];
                pRend->isCanCullingInit = g_ObjectRend_IsCanCullings[nIndexObjectRend];
                pRend->isCanCulling = pRend->isCanCullingInit;

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
void Vulkan_020_Culling::createIndirectCommands()
{

}

void Vulkan_020_Culling::createCustomCB()
{
    rebuildInstanceCBs(true);
}
void Vulkan_020_Culling::rebuildInstanceCBs(bool isCreateVkBuffer)
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
        bool isObjectCastShadow = pRend->isCastShadow;
        bool isObjectReceiveShadow = pRend->isReceiveShadow;
        bool isCanCullingInit = pRend->isCanCullingInit;

        F_DELETE(pRend->pCullLodData)
        pRend->pCullRenderData = nullptr;
        if (isCanCullingInit)
        {
            pRend->pCullLodData = new CullLodData();
            pRend->pCullLodData->isRender = true;
            pRend->pCullLodData->isCastShadow = isObjectCastShadow;
            pRend->pCullLodData->pMesh = pRend->pMeshSub->pMesh;
        }
        
        pRend->instanceMatWorld.clear();
        pRend->objectCBs.clear();
        pRend->materialCBs.clear();
        pRend->aPointerBoundAABB_Line.clear();
        pRend->aPointerBoundAABB_Line.resize(count_instance);
        pRend->aPointerBoundSphere_Line.clear();
        pRend->aPointerBoundSphere_Line.resize(count_instance);
        pRend->aPointerBoundAABB_Flat.clear();
        pRend->aPointerBoundAABB_Flat.resize(count_instance);
        pRend->aPointerBoundSphere_Flat.clear();
        pRend->aPointerBoundSphere_Flat.resize(count_instance);
        for (int j = 0; j < count_instance; j++)
        {
            //ObjectConstants
            ObjectConstants objectConstants;
            objectConstants.g_MatWorld = FMath::FromTRS(g_ObjectRend_Tranforms[3 * i + 0] + FVector3((j - pRend->pModelObject->countInstanceExt) * g_Object_InstanceGap , 0, 0),
                                                        g_ObjectRend_Tranforms[3 * i + 1],
                                                        g_ObjectRend_Tranforms[3 * i + 2]);
            pRend->objectCBs.push_back(objectConstants);
            pRend->instanceMatWorld.push_back(objectConstants.g_MatWorld);
            pRend->aPointerBoundAABB_Line[j] = nullptr;
            pRend->aPointerBoundSphere_Line[j] = nullptr;
            pRend->aPointerBoundAABB_Flat[j] = nullptr;
            pRend->aPointerBoundSphere_Flat[j] = nullptr;

            //MaterialConstants
            MaterialConstants materialConstants;
            materialConstants.factorAmbient = FMath::RandomColor(false);
            materialConstants.factorDiffuse = FMath::RandomColor(false);
            materialConstants.factorSpecular = FMath::RandomColor(false);
            materialConstants.shininess = FMath::RandF(10.0f, 100.0f);
            materialConstants.alpha = FMath::RandF(0.2f, 0.9f);
            materialConstants.lighting = isObjectLighting;
            materialConstants.castshadow = isObjectCastShadow;
            materialConstants.receiveshadow = isObjectReceiveShadow;
            //Texture VS
            {
                TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                if (pTextureVSs != nullptr)
                {

                }
            }
            //Texture FS
            {
                TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
                if (pTextureFSs != nullptr)
                {

                }
            }
            //Texture CS
            {
                TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
                if (pTextureCSs != nullptr)
                {

                }
            }
            pRend->materialCBs.push_back(materialConstants);

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                TessellationConstants tessellationConstants;
                tessellationConstants.tessLevelOuter = 3.0f;
                tessellationConstants.tessLevelInner = 3.0f;
                tessellationConstants.tessAlpha = 1.0f;
                pRend->tessellationCBs.push_back(tessellationConstants);
            }

            //Cull
            if (pRend->pCullLodData != nullptr)
            {
                if (j == 0)
                    pRend->pCullLodData->aMaterialConstants.push_back(materialConstants);

                CullObjectInstanceConstants instanceConstants;
                instanceConstants.mat4Object2World = objectConstants.g_MatWorld;
                pRend->pCullLodData->aInstanceDatas.push_back(instanceConstants);
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
                String nameBuffer = "ObjectConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_ObjectCB[j], pRend->poBuffersMemory_ObjectCB[j]);
            }

            //MaterialConstants
            bufferSize = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
            pRend->poBuffers_materialCB.resize(count_sci);
            pRend->poBuffersMemory_materialCB.resize(count_sci);
            for (size_t j = 0; j < count_sci; j++) 
            {
                String nameBuffer = "MaterialConstants-" + FUtilString::SavePointI(FPointI(i,j));
                createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_materialCB[j], pRend->poBuffersMemory_materialCB[j]);
            }

            //TessellationConstants
            if (pRend->isUsedTessellation)
            {
                bufferSize = sizeof(TessellationConstants) * MAX_OBJECT_COUNT;
                pRend->poBuffers_tessellationCB.resize(count_sci);
                pRend->poBuffersMemory_tessellationCB.resize(count_sci);
                for (size_t j = 0; j < count_sci; j++) 
                {
                    String nameBuffer = "TessellationConstants-" + FUtilString::SavePointI(FPointI(i,j));
                    createVkBuffer(nameBuffer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, pRend->poBuffers_tessellationCB[j], pRend->poBuffersMemory_tessellationCB[j]);
                }
            }
        }

        //Cull
        if (pRend->pCullLodData != nullptr)
        {
            pRend->pCullRenderData = CullManager::GetSingleton().AddStaticCullRenderData(pRend->pCullLodData);
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

void Vulkan_020_Culling::createCustomBeforePipeline()
{
    //1> DescriptorSetLayout
    createDescriptorSetLayouts();

    //2> PipelineLayout
    createPipelineLayouts();

    //3> Shader
    createShaderModules();
}   
void Vulkan_020_Culling::createGraphicsPipeline_Custom()
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
            String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Graphics
        {
            pRend->pPipelineGraphics->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poDescriptorSetLayout = findDescriptorSetLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Can not find DescriptorSetLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            pRend->pPipelineGraphics->poPipelineLayout = findPipelineLayout(pRend->pPipelineGraphics->nameDescriptorSetLayout);
            if (pRend->pPipelineGraphics->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Can not find PipelineLayout by name: " + pRend->pPipelineGraphics->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            //pPipelineGraphics->poPipeline_WireFrame
            pRend->pPipelineGraphics->poPipeline_WireFrame = createVkGraphicsPipeline("PipelineGraphics-Wire-" + pRend->nameObjectRend,
                                                                                      pRend->aShaderStageCreateInfos_Graphics,
                                                                                      pRend->isUsedTessellation, 0, 3,
                                                                                      Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                      this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                      pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                      pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                      pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                      pRend->cfg_isBlend, pRend->cfg_BlendColorFactorSrc, pRend->cfg_BlendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                      pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                      pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_020_Culling::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

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
            uint32_t enablePCF = 0;
            if (pRend->isReceiveShadow)
            {
                VkSpecializationMapEntry specializationMapEntry = CreateSpecializationMapEntry(0, 0, sizeof(uint32_t));
                VkSpecializationInfo specializationInfo = CreateSpecializationInfo(1, &specializationMapEntry, sizeof(uint32_t), &enablePCF);  
                pRend->aShaderStageCreateInfos_Graphics[1].pSpecializationInfo = &specializationInfo;
            }
            pRend->pPipelineGraphics->poPipeline = createVkGraphicsPipeline("PipelineGraphics-" + pRend->nameObjectRend,
                                                                            pRend->aShaderStageCreateInfos_Graphics,
                                                                            pRend->isUsedTessellation, 0, 3,
                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                            this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                            pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                            isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                            pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                            isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                            pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                            pRend->cfg_ColorWriteMask);
            if (pRend->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Failed to create pipeline graphics: " + pRend->nameObjectRend;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("Vulkan_020_Culling::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics success !", pRend->nameObjectRend.c_str());

            //pPipelineGraphics->poPipeline2
            if (pRend->isReceiveShadow)
            {
                enablePCF = 1;

                pRend->pPipelineGraphics->poPipeline2 = createVkGraphicsPipeline("PipelineGraphics-2-" + pRend->nameObjectRend,
                                                                                 pRend->aShaderStageCreateInfos_Graphics,
                                                                                 pRend->isUsedTessellation, 0, 3,
                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                 this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                 pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                 isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                                 pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                 isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                 pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                 pRend->cfg_ColorWriteMask);
                if (pRend->pPipelineGraphics->poPipeline2 == VK_NULL_HANDLE)
                {
                    String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: Failed to create pipeline graphics pcf: " + pRend->nameObjectRend;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                F_LogInfo("Vulkan_020_Culling::createGraphicsPipeline_Custom: Object: [%s] Create pipeline graphics graphics pcf success !", pRend->nameObjectRend.c_str());
            }

            //pPipelineGraphics->poPipeline_Cull
            if (pRend->isCanCullingInit)
            {
                CullInfo* pCullInfo = findCullInfo(nameShaderVert);
                if (pCullInfo != nullptr)
                {
                    if (!CreatePipelineShaderStageCreateInfos(pCullInfo->nameShaderCull,
                                                              nameShaderTesc,
                                                              nameShaderTese,
                                                              nameShaderGeom,
                                                              nameShaderFrag,
                                                              m_mapVkShaderModules,
                                                              pRend->aShaderStageCreateInfos_GraphicsCull))
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Can not find shader used !";
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }

                    pRend->pPipelineGraphics->nameDescriptorSetLayout_Cull = pCullInfo->nameDescriptorSetLayoutCull;
                    pRend->pPipelineGraphics->poDescriptorSetLayoutNames_Cull = findDescriptorSetLayoutNames(pCullInfo->nameDescriptorSetLayoutCull);
                    if (pRend->pPipelineGraphics->poDescriptorSetLayoutNames_Cull == nullptr)
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Can not find DescriptorSetLayoutNames by name: " + pCullInfo->nameDescriptorSetLayoutCull;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                    pRend->pPipelineGraphics->poDescriptorSetLayout_Cull = findDescriptorSetLayout(pCullInfo->nameDescriptorSetLayoutCull);
                    if (pRend->pPipelineGraphics->poDescriptorSetLayout_Cull == VK_NULL_HANDLE)
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Can not find DescriptorSetLayout by name: " + pCullInfo->nameDescriptorSetLayoutCull;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                    pRend->pPipelineGraphics->poPipelineLayout_Cull = findPipelineLayout(pCullInfo->nameDescriptorSetLayoutCull);
                    if (pRend->pPipelineGraphics->poPipelineLayout_Cull == VK_NULL_HANDLE)
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Can not find PipelineLayout by name: " + pCullInfo->nameDescriptorSetLayoutCull;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }

                    //pPipelineGraphics->poPipeline_WireFrame_Cull
                    pRend->pPipelineGraphics->poPipeline_WireFrame_Cull = createVkGraphicsPipeline("PipelineGraphics-Cull-Wire-" + pRend->nameObjectRend,
                                                                                                   pRend->aShaderStageCreateInfos_GraphicsCull,
                                                                                                   pRend->isUsedTessellation, 0, 3,
                                                                                                   Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                                   Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                                   this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout_Cull, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                                   pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, pRend->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                                   pRend->cfg_isDepthTest, pRend->cfg_isDepthWrite, pRend->cfg_DepthCompareOp,
                                                                                                   pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                                   pRend->cfg_isBlend, pRend->cfg_BlendColorFactorSrc, pRend->cfg_BlendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                                   pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                                   pRend->cfg_ColorWriteMask);
                    if (pRend->pPipelineGraphics->poPipeline_WireFrame_Cull == VK_NULL_HANDLE)
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Failed to create pipeline graphics wire frame: " + pRend->nameObjectRend;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                    F_LogInfo("Vulkan_020_Culling::createGraphicsPipeline_Custom: Object: [Cull] [%s] Create pipeline graphics wire frame success !", pRend->nameObjectRend.c_str());

                    //pPipelineGraphics->poPipeline_Cull
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
                    pRend->pPipelineGraphics->poPipeline_Cull = createVkGraphicsPipeline("PipelineGraphics-Cull-" + pRend->nameObjectRend,
                                                                                         pRend->aShaderStageCreateInfos_GraphicsCull,
                                                                                         pRend->isUsedTessellation, 0, 3,
                                                                                         Util_GetVkVertexInputBindingDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex), 
                                                                                         Util_GetVkVertexInputAttributeDescriptionVectorPtr(pRend->pMeshSub->poTypeVertex),
                                                                                         this->poRenderPass, pRend->pPipelineGraphics->poPipelineLayout_Cull, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                                         pRend->cfg_vkPrimitiveTopology, pRend->cfg_vkFrontFace, pRend->cfg_vkPolygonMode, VK_CULL_MODE_NONE, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                                         isDepthTestEnable, isDepthWriteEnable, pRend->cfg_DepthCompareOp,
                                                                                         pRend->cfg_isStencilTest, pRend->cfg_StencilOpFront, pRend->cfg_StencilOpBack, 
                                                                                         isBlend, blendColorFactorSrc, blendColorFactorDst, pRend->cfg_BlendColorOp,
                                                                                         pRend->cfg_BlendAlphaFactorSrc, pRend->cfg_BlendAlphaFactorDst, pRend->cfg_BlendAlphaOp,
                                                                                         pRend->cfg_ColorWriteMask);
                    if (pRend->pPipelineGraphics->poPipeline_Cull == VK_NULL_HANDLE)
                    {
                        String msg = "*********************** Vulkan_020_Culling::createGraphicsPipeline_Custom: [Cull] Failed to create pipeline graphics: " + pRend->nameObjectRend;
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg.c_str());
                    }
                    F_LogInfo("Vulkan_020_Culling::createGraphicsPipeline_Custom: Object: [Cull] [%s] Create pipeline graphics graphics success !", pRend->nameObjectRend.c_str());
                }
            }
        }
    }
}
void Vulkan_020_Culling::createComputePipeline_Custom()
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
            String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Can not find shader used !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        //[2] Pipeline Compute
        if (count_pipeline != pRend->aShaderStageCreateInfos_Computes.size())
        {
            String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Pipeline count is not equal shader count !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
        for (size_t j = 0; j < count_pipeline; j ++)
        {
            VKPipelineCompute* p = pRend->aPipelineComputes[j];
            VkPipelineShaderStageCreateInfo& shaderStageCreateInfo = pRend->aShaderStageCreateInfos_Computes[j];

            p->poDescriptorSetLayoutNames = findDescriptorSetLayoutNames(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayoutNames == nullptr)
            {
                String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Can not find DescriptorSetLayoutNames by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poDescriptorSetLayout = findDescriptorSetLayout(p->nameDescriptorSetLayout);
            if (p->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Can not find DescriptorSetLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            p->poPipelineLayout = findPipelineLayout(p->nameDescriptorSetLayout);
            if (p->poPipelineLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Can not find PipelineLayout by name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            p->poPipeline = createVkComputePipeline("PipelineCompute-" + p->nameDescriptorSetLayout, shaderStageCreateInfo, p->poPipelineLayout, 0);
            if (p->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** Vulkan_020_Culling::createComputePipeline_Custom: Create compute pipeline failed, PipelineLayout name: " + p->nameDescriptorSetLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
    }   
}

void Vulkan_020_Culling::destroyMeshes()
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
void Vulkan_020_Culling::createMeshes()
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

        Mesh* pMesh = new Mesh(0,
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
            String msg = "*********************** Vulkan_020_Culling::createMeshes: create mesh: [" + nameMesh + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->m_aModelMesh.push_back(pMesh);
        this->m_mapModelMesh[nameMesh] = pMesh;

        F_LogInfo("Vulkan_020_Culling::createMeshes: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                  nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
    }
}
Mesh* Vulkan_020_Culling::findMesh(const String& nameMesh)
{
    MeshPtrMap::iterator itFind = this->m_mapModelMesh.find(nameMesh);
    if (itFind == this->m_mapModelMesh.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_020_Culling::destroyTextures()
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
void Vulkan_020_Culling::createTextures()
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

        F_LogInfo("Vulkan_020_Culling::createTextures: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                  nameTexture.c_str(), 
                  nameType.c_str(), 
                  isRenderTarget ? "true" : "false",
                  pathTextures.c_str());
    }
}
Texture* Vulkan_020_Culling::findTexture(const String& nameTexture)
{
    TexturePtrMap::iterator itFind = this->m_mapModelTexture.find(nameTexture);
    if (itFind == this->m_mapModelTexture.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_020_Culling::destroyCullInfos()
{
    size_t count = this->m_aCullInfo.size();
    for (size_t i = 0; i < count; i++)
    {
        CullInfo* pCullInfo = this->m_aCullInfo[i];
        delete pCullInfo;
    }
    this->m_aCullInfo.clear();
    this->m_mapCullInfo.clear();
}
void Vulkan_020_Culling::createCullInfos()
{
    for (int i = 0; i < g_ObjectRend_CullInfo_Count; i++)
    {
        CullInfo* pCullInfo = new CullInfo();
        pCullInfo->nameShader = g_ObjectRend_CullingDesc[4 * i + 0];
        pCullInfo->nameShaderCull = g_ObjectRend_CullingDesc[4 * i + 1];
        pCullInfo->nameDescriptorSetLayout = g_ObjectRend_CullingDesc[4 * i + 2];
        pCullInfo->nameDescriptorSetLayoutCull = g_ObjectRend_CullingDesc[4 * i + 3];

        this->m_aCullInfo.push_back(pCullInfo);
        this->m_mapCullInfo[pCullInfo->nameShader] = pCullInfo;
    }
}
Vulkan_020_Culling::CullInfo* Vulkan_020_Culling::findCullInfo(const String& nameShader)
{
    CullInfoPtrMap::iterator itFind = this->m_mapCullInfo.find(nameShader);
    if (itFind == this->m_mapCullInfo.end())
    {
        return nullptr;
    }
    return itFind->second;
}


void Vulkan_020_Culling::destroyDescriptorSetLayouts()
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
void Vulkan_020_Culling::createDescriptorSetLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameLayout(g_DescriptorSetLayoutNames[i]);
        StringVector aLayouts = FUtilString::Split(nameLayout, "-");
        VkDescriptorSetLayout vkDescriptorSetLayout = CreateDescriptorSetLayout(nameLayout, &aLayouts);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** Vulkan_020_Culling::createDescriptorSetLayouts: Failed to create descriptor set layout: " + nameLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->m_aVkDescriptorSetLayouts.push_back(vkDescriptorSetLayout);
        this->m_mapVkDescriptorSetLayout[nameLayout] = vkDescriptorSetLayout;
        this->m_mapName2Layouts[nameLayout] = aLayouts;

        F_LogInfo("Vulkan_020_Culling::createDescriptorSetLayouts: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
    }
}
VkDescriptorSetLayout Vulkan_020_Culling::findDescriptorSetLayout(const String& nameDescriptorSetLayout)
{
    VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayout.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapVkDescriptorSetLayout.end())
    {
        return nullptr;
    }
    return itFind->second;
}
StringVector* Vulkan_020_Culling::findDescriptorSetLayoutNames(const String& nameDescriptorSetLayout)
{
    std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts.find(nameDescriptorSetLayout);
    if (itFind == this->m_mapName2Layouts.end())
    {
        return nullptr;
    }
    return &(itFind->second);
}


void Vulkan_020_Culling::destroyShaderModules()
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
void Vulkan_020_Culling::createShaderModules()
{
    for (int i = 0; i < g_ShaderCount; i++)
    {
        String shaderName = g_ShaderModulePaths[3 * i + 0];
        String shaderType = g_ShaderModulePaths[3 * i + 1];
        String shaderPath = g_ShaderModulePaths[3 * i + 2];

        VkShaderModule shaderModule = createVkShaderModule(shaderName, shaderType, shaderPath);
        this->m_aVkShaderModules.push_back(shaderModule);
        this->m_mapVkShaderModules[shaderName] = shaderModule;
        F_LogInfo("Vulkan_020_Culling::createShaderModules: create shader, name: [%s], type: [%s], path: [%s] success !", 
                  shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
    }
}
VkShaderModule Vulkan_020_Culling::findShaderModule(const String& nameShaderModule)
{
    VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules.find(nameShaderModule);
    if (itFind == this->m_mapVkShaderModules.end())
    {
        return nullptr;
    }
    return itFind->second;
}   


void Vulkan_020_Culling::destroyPipelineLayouts()
{
    size_t count = this->m_aVkPipelineLayouts.size();
    for (size_t i = 0; i < count; i++)
    {
        destroyVkPipelineLayout(this->m_aVkPipelineLayouts[i]);
    }
    this->m_aVkPipelineLayouts.clear();
    this->m_mapVkPipelineLayouts.clear();
}
void Vulkan_020_Culling::createPipelineLayouts()
{
    for (int i = 0; i < g_DescriptorSetLayoutCount; i++)
    {
        String nameDescriptorSetLayout(g_DescriptorSetLayoutNames[i]);
        VkDescriptorSetLayout vkDescriptorSetLayout = findDescriptorSetLayout(nameDescriptorSetLayout);
        if (vkDescriptorSetLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_020_Culling::createPipelineLayouts: Can not find DescriptorSetLayout by name: [%s]", nameDescriptorSetLayout.c_str());
            return;
        }

        VkDescriptorSetLayoutVector aDescriptorSetLayout;
        aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
        VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(nameDescriptorSetLayout, aDescriptorSetLayout);
        if (vkPipelineLayout == VK_NULL_HANDLE)
        {
            F_LogError("*********************** Vulkan_020_Culling::createPipelineLayouts: createVkPipelineLayout failed !");
            return;
        }

        this->m_aVkPipelineLayouts.push_back(vkPipelineLayout);
        this->m_mapVkPipelineLayouts[nameDescriptorSetLayout] = vkPipelineLayout;
    }
}
VkPipelineLayout Vulkan_020_Culling::findPipelineLayout(const String& namePipelineLayout)
{
    VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts.find(namePipelineLayout);
    if (itFind == this->m_mapVkPipelineLayouts.end())
    {
        return nullptr;
    }
    return itFind->second;
}



void Vulkan_020_Culling::createDescriptorSets_Custom()
{
    //1> Object Rend
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        //Pipeline Graphics
        {
            createVkDescriptorSets("DescriptorSets-" + pRend->nameObjectRend, pRend->pPipelineGraphics->poDescriptorSetLayout, pRend->pPipelineGraphics->poDescriptorSets);
            updateDescriptorSets_Graphics(pRend,
                                          pRend->pPipelineGraphics->poDescriptorSets, 
                                          pRend->pPipelineGraphics->poDescriptorSetLayoutNames,
                                          pRend->poBuffers_ObjectCB,
                                          pRend->poBuffers_materialCB,
                                          nullptr,
                                          nullptr,
                                          nullptr);

            if (pRend->pPipelineGraphics->poPipeline_Cull != nullptr &&
                pRend->pCullRenderData != nullptr)
            {
                createVkDescriptorSets("DescriptorSets-Cull-" + pRend->nameObjectRend, pRend->pPipelineGraphics->poDescriptorSetLayout_Cull, pRend->pPipelineGraphics->poDescriptorSets_Cull);
                updateDescriptorSets_Graphics(pRend,
                                              pRend->pPipelineGraphics->poDescriptorSets_Cull, 
                                              pRend->pPipelineGraphics->poDescriptorSetLayoutNames_Cull, 
                                              pRend->poBuffers_ObjectCB,
                                              pRend->poBuffers_materialCB,
                                              pRend->pCullRenderData->pBuffer_CullInstance,
                                              pRend->pCullRenderData->pBuffer_CullObjectInstances,
                                              pRend->pCullRenderData->pCullUnit->GetResultCB());

                if (m_pPipelineGraphics_DepthShadowMap != nullptr)
                {
                    m_pPipelineGraphics_DepthShadowMap->CreateDescriptorSet_ShadowMapDepthCull("DescriptorSets-Cull-ShadowMap-" + pRend->nameObjectRend, pRend->pPipelineGraphics->poDescriptorSets_ShadowMapCull);
                    pRend->pCullRenderData->pDescriptorSets_ShadowMapDepthCull = &pRend->pPipelineGraphics->poDescriptorSets_ShadowMapCull;
                    m_pPipelineGraphics_DepthShadowMap->UpdateDescriptorSet_ShadowMapDepthCull(&pRend->pPipelineGraphics->poDescriptorSets_ShadowMapCull,
                                                                                               pRend->pCullRenderData->pBuffer_CullInstance, 
                                                                                               pRend->pCullRenderData->pBuffer_CullObjectInstances,
                                                                                               pRend->pCullRenderData->pCullUnit->GetResultCB());
                }
            }
        }   
        
        //Pipeline Computes
        size_t count_comp_rend = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp_rend; j++)
        {       
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
            createVkDescriptorSet("DescriptorSet-" + pRend->nameObjectRend, pPipelineCompute->poDescriptorSetLayout, pPipelineCompute->poDescriptorSet);
            updateDescriptorSets_Compute(pRend, pPipelineCompute);
        }
    }

    //2> Object Rend Indirect
    size_t count_object = this->m_aModelObjects.size();
    for (size_t i = 0; i < count_object; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];
        if (pModelObject->pRendIndirect != nullptr)
        {
            createVkDescriptorSets("DescriptorSets-Indirect-" + pModelObject->nameObject, pModelObject->pRendIndirect->pRend->pPipelineGraphics->poDescriptorSetLayout, pModelObject->pRendIndirect->poDescriptorSets);
            updateDescriptorSets_Graphics(pModelObject->pRendIndirect->pRend,
                                          pModelObject->pRendIndirect->poDescriptorSets,    
                                          pModelObject->pRendIndirect->pRend->pPipelineGraphics->poDescriptorSetLayoutNames, 
                                          pModelObject->pRendIndirect->poBuffers_ObjectCB,
                                          pModelObject->pRendIndirect->poBuffers_materialCB,
                                          nullptr,
                                          nullptr,
                                          nullptr);
        }
    }
}
void Vulkan_020_Culling::updateDescriptorSets_Graphics(ModelObjectRend* pRend,
                                                       VkDescriptorSetVector& poDescriptorSets, 
                                                       StringVector* poDescriptorSetLayoutNames,
                                                       const VkBufferVector& poBuffersObjectCB,
                                                       const VkBufferVector& poBuffersMaterialCB,
                                                       BufferUniform* pCB_CullInstance,
                                                       BufferCompute* pCB_CullObjectInstances,
                                                       BufferCompute* pCB_Result)
{
    F_Assert(pRend && poDescriptorSetLayoutNames != nullptr && "Vulkan_020_Culling::updateDescriptorSets_Graphics")
    size_t count_ds = poDescriptorSets.size();
    for (size_t j = 0; j < count_ds; j++)
    {   
        VkWriteDescriptorSetVector descriptorWrites;
        int nIndexTextureFS = 0;

        size_t count_names = poDescriptorSetLayoutNames->size();
        for (size_t p = 0; p < count_names; p++)
        {
            String& nameDescriptorSet = (*poDescriptorSetLayoutNames)[p];
            if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
            {
                VkDescriptorBufferInfo bufferInfo_Pass = {};
                bufferInfo_Pass.buffer = this->poBuffers_PassCB[j];
                bufferInfo_Pass.offset = 0;
                bufferInfo_Pass.range = sizeof(PassConstants);
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Pass);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
            {
                VkDescriptorBufferInfo bufferInfo_Object = {};
                bufferInfo_Object.buffer = poBuffersObjectCB[j];
                bufferInfo_Object.offset = 0;
                bufferInfo_Object.range = sizeof(ObjectConstants) * MAX_OBJECT_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Object);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
            {
                VkDescriptorBufferInfo bufferInfo_Material = {};
                bufferInfo_Material.buffer = poBuffersMaterialCB[j];
                bufferInfo_Material.offset = 0;
                bufferInfo_Material.range = sizeof(MaterialConstants) * MAX_MATERIAL_COUNT;
                pushVkDescriptorSet_Uniform(descriptorWrites,
                                            poDescriptorSets[j],
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
                                            poDescriptorSets[j],
                                            p,
                                            0,
                                            1,
                                            bufferInfo_Instance);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_CullInstance)) //CullInstance
            {
                if (pCB_CullInstance != VK_NULL_HANDLE)
                {
                    VkDescriptorBufferInfo bufferInfo_CullInstance = {};
                    bufferInfo_CullInstance.buffer = pCB_CullInstance->poBuffer_Uniform;
                    bufferInfo_CullInstance.offset = 0;
                    bufferInfo_CullInstance.range = sizeof(CullInstanceConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      poDescriptorSets[j],
                                                                      p,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_CullInstance);
                }
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
            {
                Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Fragment), nIndexTextureFS);
                nIndexTextureFS ++;
                pushVkDescriptorSet_Image(descriptorWrites,
                                          poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          pTexture->poTextureImageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureDepthShadow)) //TextureDepthShadow
            {
                pushVkDescriptorSet_Image(descriptorWrites,
                                          poDescriptorSets[j],
                                          p,
                                          0,
                                          1,
                                          VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                          m_pVKRenderPassShadowMap->imageInfo);
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWObjectCullInstance)) //BufferRWObjectCullInstance
            {
                if (pCB_CullObjectInstances != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_InstanceCull = {};
                    bufferInfo_InstanceCull.buffer = pCB_CullObjectInstances->poBuffer_Compute;
                    bufferInfo_InstanceCull.offset = 0;
                    bufferInfo_InstanceCull.range = (VkDeviceSize)pCB_CullObjectInstances->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      poDescriptorSets[j],
                                                                      p,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_InstanceCull);
                }
            }
            else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
            {
                if (pCB_Result != nullptr)
                {
                    VkDescriptorBufferInfo bufferInfo_Result = {};
                    bufferInfo_Result.buffer = pCB_Result->poBuffer_Compute;
                    bufferInfo_Result.offset = 0;
                    bufferInfo_Result.range = (VkDeviceSize)pCB_Result->GetBufferSize();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Storage(descriptorWrites,
                                                                      poDescriptorSets[j],
                                                                      p,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Result);
                }
            }
            else
            {
                String msg = "*********************** Vulkan_020_Culling::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }
        updateVkDescriptorSets(descriptorWrites);
    }
}
void Vulkan_020_Culling::updateDescriptorSets_Compute(ModelObjectRend* pRend,
                                                      VKPipelineCompute* pPipelineCompute)
{
    StringVector* poDescriptorSetLayoutNames = pPipelineCompute->poDescriptorSetLayoutNames;
    F_Assert(pRend && poDescriptorSetLayoutNames != nullptr && "Vulkan_020_Culling::updateDescriptorSets_Compute")

    VkWriteDescriptorSetVector descriptorWrites;
    int nIndexTextureCS = 0;
    size_t count_names = poDescriptorSetLayoutNames->size();
    for (size_t p = 0; p < count_names; p++)
    {
        String& nameDescriptorSet = (*poDescriptorSetLayoutNames)[p];
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
            Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
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
            Texture* pTexture = pRend->GetTexture(F_GetShaderTypeName(F_Shader_Compute), nIndexTextureCS);
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
            String msg = "*********************** Vulkan_020_Culling::createDescriptorSets_Compute: Compute: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }
    }  
    updateVkDescriptorSets(descriptorWrites);
}

void Vulkan_020_Culling::updateCompute_Custom(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
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

void Vulkan_020_Culling::updateCBs_Custom()
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
                                                  FVector3(0.0f, 1.0f, 0.0f));
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
            updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRend->objectCBs.data(), memory);
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
            size_t count_object = pRendIndirect->objectCBs.size();
            
            //ObjectConstants
            {
                VkDeviceMemory& memory = pRendIndirect->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                updateVKBuffer(0, sizeof(ObjectConstants) * count_object, pRendIndirect->objectCBs.data(), memory);
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

void Vulkan_020_Culling::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
{
    size_t count_object_rend = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count_object_rend; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];

        size_t count_comp = pRend->aPipelineComputes.size();
        for (int j = 0; j < count_comp; j++)
        {
            VKPipelineCompute* pPipelineCompute = pRend->aPipelineComputes[j];
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

    void Vulkan_020_Culling::drawMeshShadowMap(VkCommandBuffer& commandBuffer)
    {
        size_t count_rend = m_aModelObjectRends_All.size();
        if (count_rend <= 0)
            return;
        int instanceStart = 0;
        
        CullManager* pCullManager = CullManager::GetSingletonPtr();
        bool isCulling = false;
        if (pCullManager)
            isCulling = pCullManager->isEnable;

        //1> Update Object World
        UpdateBuffer_ObjectWorld_Begin();
        {
            for (size_t i = 0; i < count_rend; i++)
            {
                ModelObjectRend* pRend = m_aModelObjectRends_All[i];
                if (!pRend->isShow ||
                    !pRend->isCastShadow ||
                    (pRend->isCanCulling && pRend->pCullLodData != nullptr && pRend->pCullRenderData != nullptr && isCulling))
                    continue;
                
                int instanceCount = (int)pRend->objectCBs.size();
                //F_LogInfo("1111111111: instance count: [%d], start: [%d] ", instanceCount, instanceStart);
                UpdateBuffer_ObjectWorld_AddList(pRend->objectCBs);
                instanceStart += instanceCount;
            }
        }
        UpdateBuffer_ObjectWorld_End();
        
        //2> Draw Depth
        if (Draw_Graphics_DepthShadowMapBegin(commandBuffer))
        {
            instanceStart = 0;
            for (size_t i = 0; i < count_rend; i++)
            {
                ModelObjectRend* pRend = m_aModelObjectRends_All[i];
                if (!pRend->isShow ||
                    !pRend->isCastShadow ||
                    (pRend->isCanCulling && pRend->pCullLodData != nullptr && pRend->pCullRenderData != nullptr && isCulling))
                    continue;

                int instanceCount = (int)pRend->objectCBs.size();
                //F_LogInfo("2222222222: instance count: [%d], start: [%d] ", instanceCount, instanceStart);
                Draw_Graphics_DepthShadowMap(commandBuffer, pRend->pMeshSub, instanceCount, instanceStart);
                instanceStart += instanceCount;
            }
            Draw_Graphics_DepthShadowMapEnd(commandBuffer);
        }

        //3> Cull Instance
        if (isCulling)
        {
            //Draw Depth
            if (Draw_Graphics_CullInstance_DepthShadowMapCullBegin(commandBuffer))
            {
                Draw_Graphics_CullInstance_DepthShadowMapCull(commandBuffer);
                Draw_Graphics_CullInstance_DepthShadowMapCullEnd(commandBuffer);
            }
        }   
    }

bool Vulkan_020_Culling::beginRenderImgui()
{
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    static bool windowOpened = true;
    ImGui::Begin("Vulkan_020_Culling", &windowOpened, 0);
    {
        //0> Common
        commonConfig();

        //1> Camera
        cameraConfig();

        //2> Light
        lightConfig();

        //3> Shadow
        shadowConfig();

        //4> Cull
        cullConfig();

        //5> PassConstants
        passConstantsConfig();

        //6> Model
        modelConfig();

    }
    ImGui::End();

    return true;
}
void Vulkan_020_Culling::modelConfig()
{
    if (ImGui::CollapsingHeader("Model Settings"))
    {
        size_t count_object = this->m_aModelObjects.size();

        //m_isDrawIndirect
        // if (ImGui::Checkbox("Is DrawIndirect", &this->m_isDrawIndirect))
        // {
            
        // }
        //m_isDrawIndirectMulti
        // if (ImGui::Checkbox("Is DrawIndirectMulti", &this->m_isDrawIndirectMulti))
        // {
            
        // }

        //g_Object_InstanceGap
        float fGap = g_Object_InstanceGap;
        if (ImGui::DragFloat("Instance Gap: ", &fGap, 0.1f, 1.0f, 100.0f))
        {
            g_Object_InstanceGap = fGap;
            rebuildInstanceCBs(false);
        }

        //g_ObjectRend_IsShowBoundAABB_All_Line
        if (ImGui::Checkbox("Is ShowBound AABB Line - All", &g_ObjectRend_IsShowBoundAABB_All_Line))
        {
            for (int i = 0; i < count_object; i++)
            {
                ModelObject* pModelObject = this->m_aModelObjects[i];
                size_t count_object_rend = pModelObject->aRends.size();
                for (int j = 0; j < count_object_rend; j++)
                {
                    ModelObjectRend* pRend = pModelObject->aRends[j];
                    if (!pRend->isCanCullingInit)
                        continue;

                    if (g_ObjectRend_IsShowBoundAABB_All_Line)
                        pRend->AddLine3D_AABB();
                    else
                        pRend->RemoveLine3D_AABB();
                }
            }
        }
        //g_ObjectRend_IsShowBoundSphere_All_Line
        if (ImGui::Checkbox("Is ShowBound Sphere Line - All", &g_ObjectRend_IsShowBoundSphere_All_Line))
        {
            for (int i = 0; i < count_object; i++)
            {
                ModelObject* pModelObject = this->m_aModelObjects[i];
                size_t count_object_rend = pModelObject->aRends.size();
                for (int j = 0; j < count_object_rend; j++)
                {
                    ModelObjectRend* pRend = pModelObject->aRends[j];
                    if (!pRend->isCanCullingInit)
                        continue;

                    if (g_ObjectRend_IsShowBoundSphere_All_Line)
                        pRend->AddLine3D_Sphere();
                    else
                        pRend->RemoveLine3D_Sphere();
                }
            }
        }
        //g_ObjectRend_IsShowBoundAABB_All_Flat
        if (ImGui::Checkbox("Is ShowBound AABB Flat - All", &g_ObjectRend_IsShowBoundAABB_All_Flat))
        {
            for (int i = 0; i < count_object; i++)
            {
                ModelObject* pModelObject = this->m_aModelObjects[i];
                size_t count_object_rend = pModelObject->aRends.size();
                for (int j = 0; j < count_object_rend; j++)
                {
                    ModelObjectRend* pRend = pModelObject->aRends[j];
                    if (!pRend->isCanCullingInit)
                        continue;

                    if (g_ObjectRend_IsShowBoundAABB_All_Flat)
                        pRend->AddFlat3D_AABB();
                    else
                        pRend->RemoveFlat3D_AABB();
                }
            }
        }
        //g_ObjectRend_IsShowBoundSphere_All_Flat
        if (ImGui::Checkbox("Is ShowBound Sphere Flat - All", &g_ObjectRend_IsShowBoundSphere_All_Flat))
        {
            for (int i = 0; i < count_object; i++)
            {
                ModelObject* pModelObject = this->m_aModelObjects[i];
                size_t count_object_rend = pModelObject->aRends.size();
                for (int j = 0; j < count_object_rend; j++)
                {
                    ModelObjectRend* pRend = pModelObject->aRends[j];
                    if (!pRend->isCanCullingInit)
                        continue;

                    if (g_ObjectRend_IsShowBoundSphere_All_Flat)
                        pRend->AddFlat3D_Sphere();
                    else
                        pRend->RemoveFlat3D_Sphere();
                }
            }
        }

        
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
                //isCastShadow
                String nameIsCastShadow = "Is CastShadow - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameIsCastShadow.c_str(), &pModelObject->isCastShadow))
                {
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        pRend->isCastShadow = pModelObject->isCastShadow;
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

                //Bound
                String nameBoundAABBLine = "Is ShowBound AABB Line - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameBoundAABBLine.c_str(), &pModelObject->isBoundAABB_Line))
                {
                    size_t count_object_rend = pModelObject->aRends.size();
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        if (!pRend->isCanCullingInit)
                            continue;

                        if (pModelObject->isBoundAABB_Line)
                            pRend->AddLine3D_AABB();
                        else
                            pRend->RemoveLine3D_AABB();
                    }
                }
                String nameBoundSphereLine = "Is ShowBound Sphere Line - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameBoundSphereLine.c_str(), &pModelObject->isBoundSphere_Line))
                {
                    size_t count_object_rend = pModelObject->aRends.size();
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        if (!pRend->isCanCullingInit)
                            continue;

                        if (pModelObject->isBoundSphere_Line)
                            pRend->AddLine3D_Sphere();
                        else
                            pRend->RemoveLine3D_Sphere();
                    }
                }
                String nameBoundAABBFlat = "Is ShowBound AABB Flat - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameBoundAABBFlat.c_str(), &pModelObject->isBoundAABB_Flat))
                {
                    size_t count_object_rend = pModelObject->aRends.size();
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        if (!pRend->isCanCullingInit)
                            continue;

                        if (pModelObject->isBoundAABB_Flat)
                            pRend->AddFlat3D_AABB();
                        else
                            pRend->RemoveFlat3D_AABB();
                    }
                }
                String nameBoundSphereFlat = "Is ShowBound Sphere Flat - " + pModelObject->nameObject;
                if (ImGui::Checkbox(nameBoundSphereFlat.c_str(), &pModelObject->isBoundSphere_Flat))
                {
                    size_t count_object_rend = pModelObject->aRends.size();
                    for (int j = 0; j < count_object_rend; j++)
                    {
                        ModelObjectRend* pRend = pModelObject->aRends[j];
                        if (!pRend->isCanCullingInit)
                            continue;

                        if (pModelObject->isBoundSphere_Flat)
                            pRend->AddFlat3D_Sphere();
                        else
                            pRend->RemoveFlat3D_Sphere();
                    }
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

                        //isCastShadow
                        String nameIsCastShadow = "Is CastShadow - " + nameObjectRendIndirect;
                        if (ImGui::Checkbox(nameIsCastShadow.c_str(), &pRendIndirect->isCastShadow))
                        {
                            if (pRendIndirect->isCastShadow)
                            {
                                pModelObject->isCastShadow = true;
                            }
                        }

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

                            //isCastShadow
                            String nameIsCastShadow = "Is CastShadow - " + nameObjectRend;
                            if (ImGui::Checkbox(nameIsCastShadow.c_str(), &pRend->isCastShadow))
                            {
                                if (pRend->isCastShadow)
                                {
                                    pModelObject->isCastShadow = true;
                                }
                            }

                            //isReceiveShadow
                            if (pRend->isReceiveShadow)
                            {
                                //isShadowPCF
                                String nameIsShadowPCF = "Is ShadowPCF - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsShadowPCF.c_str(), &pRend->isShadowPCF))
                                {
                                    
                                }
                            }

                            //isCanCulling
                            if (pRend->isCanCullingInit)
                            {
                                String nameIsCanCulling = "Is CanCulling - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsCanCulling.c_str(), &pRend->isCanCulling))
                                {
                                    
                                }
                                
                                //Bound Line
                                String nameIsBoundAABB_Line = "Is BoundAABB Line - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsBoundAABB_Line.c_str(), &pRend->isBoundAABB_Line))
                                {
                                    if (pRend->isBoundAABB_Line)
                                    {
                                        pRend->AddLine3D_AABB();
                                    }
                                    else
                                    {
                                        pRend->RemoveLine3D_AABB();
                                    }
                                }
                                String nameIsBoundSphere_Line = "Is BoundSphere Line - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsBoundSphere_Line.c_str(), &pRend->isBoundSphere_Line))
                                {
                                    int count_instance = pModelObject->countInstance;
                                    if (pRend->isBoundSphere_Line)
                                    {
                                        pRend->AddLine3D_Sphere();
                                    }   
                                    else
                                    {
                                        pRend->RemoveLine3D_Sphere();
                                    }
                                }

                                //Bound Flat
                                String nameIsBoundAABB_Flat = "Is BoundAABB Flat - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsBoundAABB_Flat.c_str(), &pRend->isBoundAABB_Flat))
                                {
                                    if (pRend->isBoundAABB_Flat)
                                    {
                                        pRend->AddFlat3D_AABB();
                                    }
                                    else
                                    {
                                        pRend->RemoveFlat3D_AABB();
                                    }
                                }
                                String nameIsBoundSphere_Flat = "Is BoundSphere Flat - " + nameObjectRend;
                                if (ImGui::Checkbox(nameIsBoundSphere_Flat.c_str(), &pRend->isBoundSphere_Flat))
                                {
                                    int count_instance = pModelObject->countInstance;
                                    if (pRend->isBoundSphere_Flat)
                                    {
                                        pRend->AddFlat3D_Sphere();
                                    }   
                                    else
                                    {
                                        pRend->RemoveFlat3D_Sphere();
                                    }
                                }
                            }
                           
                            
                            String nameWorld = "Model Object - " + nameObjectRend;
                            if (ImGui::CollapsingHeader(nameWorld.c_str()))
                            {
                                int count_instance = pModelObject->countInstance;
                                for (int p = 0; p < count_instance; p++)
                                {
                                    ObjectConstants& obj = pRend->objectCBs[p];
                                    MaterialConstants& mat = pRend->materialCBs[p];

                                    String nameModelInstance = FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                    if (ImGui::CollapsingHeader(nameModelInstance.c_str()))
                                    {
                                        //ObjectConstants
                                        String nameObject = FUtilString::SaveInt(p) + " - Object - " + nameObjectRend;
                                        if (ImGui::CollapsingHeader(nameObject.c_str()))
                                        {
                                            const FMatrix4& mat4World = obj.g_MatWorld;
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

                                            //castshadow
                                            String nameCastshadow = "Castshadow - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isCastshadow = mat.castshadow == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameCastshadow.c_str(), &isCastshadow))
                                            {
                                                mat.castshadow = isCastshadow ? 1.0f : 0.0f;
                                            }

                                            //receiveshadow
                                            String nameReceiveshadow = "receiveshadow - " + FUtilString::SaveInt(p) + " - " + nameObjectRend;
                                            bool isReceiveshadow = mat.receiveshadow == 1.0f ? true : false;
                                            if (ImGui::Checkbox(nameReceiveshadow.c_str(), &isReceiveshadow))
                                            {
                                                mat.receiveshadow = isReceiveshadow ? 1.0f : 0.0f;
                                            }

                                            //Texture VS
                                            {
                                                TexturePtrVector* pTextureVSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Vertex));
                                                if (pTextureVSs != nullptr)
                                                {

                                                }
                                            }
                                            //Texture FS
                                            {
                                                TexturePtrVector* pTextureFSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Fragment));
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
                                                            if (pTexture->typeTexture == F_Texture_2DArray)
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
                                                TexturePtrVector* pTextureCSs = pRend->GetTextures(F_GetShaderTypeName(F_Shader_Compute));
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

void Vulkan_020_Culling::endRenderImgui()
{
    VulkanWindow::endRenderImgui();

}

void Vulkan_020_Culling::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
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

    CullManager* pCullManager = CullManager::GetSingletonPtr();
    if (pCullManager && pCullManager->isEnable)
    {
        drawModelObjectRendCulls(commandBuffer);
    }
}
void Vulkan_020_Culling::drawModelObjectRendCulls(VkCommandBuffer& commandBuffer)
{
    size_t count = this->m_aModelObjectRends_All.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObjectRend* pRend = this->m_aModelObjectRends_All[i];
        if (!pRend->isCanCullingInit)
            continue;
        if (!pRend->isShow ||
            !pRend->pModelObject->isShow ||
            !pRend->isCanCulling ||
            pRend->pCullLodData == nullptr || 
            pRend->pCullRenderData == nullptr)
            continue;

        drawModelObjectRendCull(commandBuffer, pRend);
    }
}
void Vulkan_020_Culling::drawModelObjectRendCull(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
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
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_WireFrame_Cull);
    }
    else
    {
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline_Cull);
    }
    if (pRend->pPipelineGraphics->poDescriptorSets_Cull.size() > 0)
    {
        bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipelineLayout_Cull, 0, 1, &pRend->pPipelineGraphics->poDescriptorSets_Cull[this->poSwapChainImageIndex], 0, nullptr);
    }

    BufferIndirectCommand* pBufferIndirectCommand = pRend->pCullRenderData->pCullUnit->GetRenderArgsCB();
    drawIndexedIndirect(commandBuffer, pBufferIndirectCommand->poBuffer_IndirectCommand, pRend->pCullRenderData->nRenderIndex * sizeof(VkDrawIndexedIndirectCommand), 1, sizeof(VkDrawIndexedIndirectCommand));
}

void Vulkan_020_Culling::drawModelObjectRendIndirects(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
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
void Vulkan_020_Culling::drawModelObjectRendIndirect(VkCommandBuffer& commandBuffer, ModelObjectRendIndirect* pRendIndirect)
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

void Vulkan_020_Culling::drawModelObjectRends(VkCommandBuffer& commandBuffer, ModelObjectRendPtrVector& aRends)
{
    CullManager* pCullManager = CullManager::GetSingletonPtr();
    bool isCulling = false;
    if (pCullManager)
        isCulling = pCullManager->isEnable;
    size_t count_rend = aRends.size();
    for (size_t i = 0; i < count_rend; i++)
    {
        ModelObjectRend* pRend = aRends[i];
        if (!pRend->isShow ||
            !pRend->pModelObject->isShow ||
            (pRend->isCanCulling && pRend->pCullLodData != nullptr && pRend->pCullRenderData != nullptr && isCulling))
            continue;

        drawModelObjectRend(commandBuffer, pRend);
    }
}
void Vulkan_020_Culling::drawModelObjectRend(VkCommandBuffer& commandBuffer, ModelObjectRend* pRend)
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
        if (pRend->isReceiveShadow && pRend->isShadowPCF && pRend->pPipelineGraphics->poPipeline2 != VK_NULL_HANDLE)
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pRend->pPipelineGraphics->poPipeline2);
        else
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

void Vulkan_020_Culling::cleanupCustom()
{   
    destroyCullInfos();
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

void Vulkan_020_Culling::cleanupSwapChain_Custom()
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

void Vulkan_020_Culling::recreateSwapChain_Custom()
{   
    size_t count = this->m_aModelObjects.size();
    for (size_t i = 0; i < count; i++)
    {
        ModelObject* pModelObject = this->m_aModelObjects[i];

        pModelObject->RecreateSwapChain();
    }
}