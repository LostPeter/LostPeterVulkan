/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanWindow.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

namespace LostPeterVulkan
{
#if F_DEBUG == 1
    bool VulkanWindow::s_isEnableValidationLayers = true;
#else
    bool VulkanWindow::s_isEnableValidationLayers = false;
#endif
    int VulkanWindow::s_maxFramesInFight = 2;
    
    
    /////////////////////////// VulkanWindow Internal /////////////
    void VulkanWindow::createInternal()
    {
        //Mesh
        createMeshes_Internal();
        //Texture
        createTextures_Internal();
    }
    void VulkanWindow::cleanupInternal()
    {
        F_DELETE(m_pVKRenderPassTerrain)
        F_DELETE(m_pVKRenderPassCull)
        F_DELETE(m_pVKRenderPassShadowMap)

        //Texture
        destroyTextures_Internal();
        //Mesh
        destroyMeshes_Internal();
    }

    void VulkanWindow::createResourceInternal()
    {
        //DescriptorSetLayout
        createDescriptorSetLayouts_Internal();
        //PipelineLayout
        createPipelineLayouts_Internal();
        //ShaderModule
        createShaderModules_Internal();

        //Uniform ConstantBuffer
        createUniformCB_Internal();
        //PipelineCompute
        createPipelineCompute_Internal();
        //PipelineGraphics
        createPipelineGraphics_Internal();
    }
    void VulkanWindow::destroyResourceInternal()
    {
        //Uniform ConstantBuffer
        destroyUniformCB_Internal();
        //PipelineCompute
        destroyPipelineCompute_Internal();
        //PipelineGraphics
        destroyPipelineGraphics_Internal();

        //ShaderModule
        destroyShaderModules_Internal();
        //PipelineLayout
        destroyPipelineLayouts_Internal();
        //DescriptorSetLayout
        destroyDescriptorSetLayouts_Internal();

        //RenderPassShadowMap
        if (m_pVKRenderPassShadowMap != nullptr)
        {
            m_pVKRenderPassShadowMap->CleanupSwapChain();
        }
        //RenderPassCull
        if (m_pVKRenderPassCull != nullptr)
        {
            m_pVKRenderPassCull->CleanupSwapChain();
        }
        //RenderPassTerrain
        if (m_pVKRenderPassTerrain != nullptr)
        {
            m_pVKRenderPassTerrain->CleanupSwapChain();
        }
    }

    //Mesh
    static const int g_MeshCount_Internal = 51;
    static const char* g_MeshPaths_Internal[7 * g_MeshCount_Internal] =
    {
        //Mesh Name                     //Vertex Type                 //Mesh Type         //Mesh Path                           //Mesh Geometry Type        //Mesh Geometry Param                                           //Mesh Geometry Vertex Is Update
        "geo_line_line_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineLine2D",               "0.5;0.9;0.7;0.9;1;1;1;1",                                      "false", //geo_line_line_2d
        "geo_line_triangle_2d",         "Pos3Color4",                 "geometry",         "",                                   "LineTriangle2D",           "0.6;0.8;0.5;0.6;0.7;0.6;1;1;1;1",                              "false", //geo_line_triangle_2d
        "geo_line_quad_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.5;0.5;0.5;0.3;0.7;0.3;0.7;0.5;1;1;1;1",                      "false", //geo_line_quad_2d
        "geo_line_grid_2d",             "Pos3Color4",                 "geometry",         "",                                   "LineGrid2D",               "0.5;0.2;0.5;0.0;0.7;0.0;0.7;0.2;10;10;1;1;1;1",                "false", //geo_line_grid_2d
        "geo_line_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.55;-0.1;0.5;-0.3;0.65;-0.3;0.7;-0.1;1;1;1;1",                "false", //geo_line_quad_convex_2d
        "geo_line_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",                                   "LineQuad2D",               "0.5;-0.4;0.5;-0.6;0.7;-0.6;0.55;-0.5;1;1;1;1",                 "false", //geo_line_quad_concave_2d
        "geo_line_circle_2d",           "Pos3Color4",                 "geometry",         "",                                   "LineCircle2D",             "0.6;-0.8;1.0;0.0;0.1;1280;720;50;true;1;1;1;1",                "true", //geo_line_circle_2d

        "geo_flat_triangle_2d",         "Pos3Color4",                 "geometry",         "",                                   "FlatTriangle2D",           "0.85;0.8;0.75;0.6;0.95;0.6;1;1;1;1",                           "false", //geo_flat_triangle_2d
        "geo_flat_quad_2d",             "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.75;0.5;0.75;0.3;0.95;0.3;0.95;0.5;1;1;1;1",                  "false", //geo_flat_quad_2d
        "geo_flat_quad_convex_2d",      "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.8;0.2;0.75;0.0;0.9;0.0;0.95;0.2;1;1;1;1",                    "false", //geo_flat_quad_convex_2d
        "geo_flat_quad_concave_2d",     "Pos3Color4",                 "geometry",         "",                                   "FlatQuad2D",               "0.75;-0.1;0.75;-0.3;0.95;-0.3;0.8;-0.2;1;1;1;1",               "false", //geo_flat_quad_concave_2d
        "geo_flat_circle_2d",           "Pos3Color4",                 "geometry",         "",                                   "FlatCircle2D",             "0.85;-0.5;1.0;0.0;0.1;1280;720;50;1;1;1;1",                    "true", //geo_flat_circle_2d

        "geo_line_line_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineLine3D",               "0;0;0;1;0;0;1;1;1;1",                                          "false", //geo_line_line_3d
        "geo_line_triangle_3d",         "Pos3Color4",                 "geometry",         "",                                   "LineTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_line_triangle_3d
        "geo_line_quad_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_line_quad_3d
        "geo_line_grid_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineGrid3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;10;10;1;1;1;1",    "false", //geo_line_grid_3d
        "geo_line_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_line_quad_convex_3d
        "geo_line_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",                                   "LineQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_line_quad_concave_3d
        "geo_line_circle_3d",           "Pos3Color4",                 "geometry",         "",                                   "LineCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;true;1;1;1;1",                      "false", //geo_line_circle_3d
        "geo_line_aabb_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_line_aabb_3d
        "geo_line_sphere_3d",           "Pos3Color4",                 "geometry",         "",                                   "LineSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_line_sphere_3d
        "geo_line_cylinder_3d",         "Pos3Color4",                 "geometry",         "",                                   "LineCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;true;1;1;1;1",                        "false", //geo_line_cylinder_3d
        "geo_line_capsule_3d",          "Pos3Color4",                 "geometry",         "",                                   "LineCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;1;1;1;1",                              "false", //geo_line_capsule_3d
        "geo_line_cone_3d",             "Pos3Color4",                 "geometry",         "",                                   "LineCone3D",               "0;0;0;0;1;0;0.5;1;50;1;1;1;1",                                 "false", //geo_line_cone_3d
        "geo_line_torus_3d",            "Pos3Color4",                 "geometry",         "",                                   "LineTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_line_torus_3d
        
        "geo_flat_triangle_3d",         "Pos3Color4",                 "geometry",         "",                                   "FlatTriangle3D",           "0;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;1;1;1;1",                       "false", //geo_flat_triangle_3d
        "geo_flat_quad_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.5;0.5;0;1;1;1;1",          "false", //geo_flat_quad_3d
        "geo_flat_quad_convex_3d",      "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.2;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;0.8;0.5;0;1;1;1;1",          "false", //geo_flat_quad_convex_3d
        "geo_flat_quad_concave_3d",     "Pos3Color4",                 "geometry",         "",                                   "FlatQuad3D",               "-0.5;0.5;0;-0.5;-0.5;0;0.5;-0.5;0;-0.25;-0.25;0;1;1;1;1",      "false", //geo_flat_quad_concave_3d
        "geo_flat_circle_3d",           "Pos3Color4",                 "geometry",         "",                                   "FlatCircle3D",             "0;0;0;1;0;0;0;0;-1;0.5;100;1;1;1;1",                           "false", //geo_flat_circle_3d
        "geo_flat_aabb_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatAABB3D",               "0;0;0;0.5;0.5;0.5;1;1;1;1",                                    "false", //geo_flat_aabb_3d
        "geo_flat_sphere_3d",           "Pos3Color4",                 "geometry",         "",                                   "FlatSphere3D",             "0;0;0;0;1;0;0.5;30;30;1;1;1;1",                                "false", //geo_flat_sphere_3d
        "geo_flat_cylinder_3d",         "Pos3Color4",                 "geometry",         "",                                   "FlatCylinder3D",           "0;0;0;0;1;0;0.5;0.5;1;50;30;1;1;1;1",                          "false", //geo_flat_cylinder_3d
        "geo_flat_capsule_3d",          "Pos3Color4",                 "geometry",         "",                                   "FlatCapsule3D",            "0;0;0;0;1;0;0.5;1;10;50;30;1;1;1;1",                           "false", //geo_flat_capsule_3d
        "geo_flat_cone_3d",             "Pos3Color4",                 "geometry",         "",                                   "FlatCone3D",               "0;0;0;0;1;0;0.5;1;50;30;1;1;1;1",                              "false", //geo_flat_cone_3d
        "geo_flat_torus_3d",            "Pos3Color4",                 "geometry",         "",                                   "FlatTorus3D",              "0;0;0;0;1;0;0.5;0.2;50;20;1;1;1;1",                            "false", //geo_flat_torus_3d

        "geo_entity_triangle",          "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityTriangle",           "false;false",                                                  "false", //geo_entity_triangle
        "geo_entity_quad",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityQuad",               "false;false;0;0;1;1;0",                                        "false", //geo_entity_quad
        "geo_entity_grid",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityGrid",               "false;false;1;1;10;10",                                        "false", //geo_entity_grid
        "geo_entity_circle",            "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCircle",             "false;false;0.5;100",                                          "false", //geo_entity_circle
        "geo_entity_aabb",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityAABB",               "false;false;1;1;1;0",                                          "false", //geo_entity_aabb
        "geo_entity_sphere",            "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntitySphere",             "false;false;0.5;30;30",                                        "false", //geo_entity_sphere
        "geo_entity_geosphere",         "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityGeoSphere",          "false;false;0.5;5",                                            "false", //geo_entity_geosphere
        "geo_entity_cylinder",          "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCylinder",           "false;false;0.5;0.5;1;0;50;30",                                "false", //geo_entity_cylinder
        "geo_entity_capsule",           "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCapsule",            "false;false;0.5;1;0;10;50;30",                                 "false", //geo_entity_capsule
        "geo_entity_cone",              "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityCone",               "false;false;0.5;1;0;50;30",                                    "false", //geo_entity_cone
        "geo_entity_torus",             "Pos3Color4Normal3Tex2",      "geometry",         "",                                   "EntityTorus",              "false;false;0.5;0.2;50;20",                                    "false", //geo_entity_torus

        "quad",                         "Pos3Color4Tex2",             "geometry",         "",                                   "EntityQuad",               "",                                                             "false", //quad
        "plane",                        "Pos3Color4Normal3Tex2",      "file",             "Assets/Mesh/Common/plane.fbx",       "",                         "",                                                             "false", //plane
        "cube",                         "Pos3Color4Normal3Tex2",      "file",             "Assets/Mesh/Common/cube.obj",        "",                         "",                                                             "false", //cube
        "sphere",                       "Pos3Color4Normal3Tex2",      "file",             "Assets/Mesh/Common/sphere.fbx",      "",                         "",                                                             "false", //sphere

    };
    static bool g_MeshIsFlipYs_Internal[g_MeshCount_Internal] = 
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

        true, //quad
        true, //plane
        false, //cube
        false, //sphere

    };
    static bool g_MeshIsTranformLocals_Internal[g_MeshCount_Internal] = 
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

        false, //quad
        false, //plane  
        false, //cube
        false, //sphere

    };
    static FMatrix4 g_MeshTranformLocals_Internal[g_MeshCount_Internal] = 
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

        FMath::ms_mat4Unit, //quad
        FMath::ms_mat4Unit, //plane
        FMath::ms_mat4Unit, //cube
        FMath::ms_mat4Unit, //sphere

    };
    void VulkanWindow::destroyMeshes_Internal()
    {
        size_t count = this->m_aMeshes_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            Mesh* pMesh = this->m_aMeshes_Internal[i];
            delete pMesh;
        }
        this->m_aMeshes_Internal.clear();
        this->m_mapMeshes_Internal.clear();
    }
    void VulkanWindow::createMeshes_Internal()
    {
        for (int i = 0; i < g_MeshCount_Internal; i++)
        {
            String nameMesh = g_MeshPaths_Internal[7 * i + 0];
            String nameVertexType = g_MeshPaths_Internal[7 * i + 1];
            String nameMeshType = g_MeshPaths_Internal[7 * i + 2];
            String pathMesh = g_MeshPaths_Internal[7 * i + 3];
            String nameGeometryType = g_MeshPaths_Internal[7 * i + 4];
            String nameGeometryParam = g_MeshPaths_Internal[7 * i + 5];
            String nameGeometryIsVertexUpdate = g_MeshPaths_Internal[7 * i + 6];
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
                    pParam_LineCircle2D->viewWidth = (int32)this->poViewport.width;
                    pParam_LineCircle2D->viewHeight = (int32)this->poViewport.height;
                }
                else if (typeGeometryType == F_MeshGeometry_FlatCircle2D)
                {
                    FMeshCreateParam_FlatCircle2D* pParam_FlatCircle2D = (FMeshCreateParam_FlatCircle2D*)pParam;
                    pParam_FlatCircle2D->viewWidth = (int32)this->poViewport.width;
                    pParam_FlatCircle2D->viewHeight = (int32)this->poViewport.height;
                }
            }

            Mesh* pMesh = new Mesh(0,
                                   nameMesh,
                                   pathMesh,
                                   typeMesh,
                                   typeVertex,
                                   typeGeometryType,
                                   pParam);
            bool isFlipY = g_MeshIsFlipYs_Internal[i];
            bool isTransformLocal = g_MeshIsTranformLocals_Internal[i];
            if (!pMesh->LoadMesh(isFlipY, isTransformLocal, g_MeshTranformLocals_Internal[i], isVertexUpdate))
            {
                String msg = "*********************** VulkanWindow::createMeshes_Internal: create mesh: [" + nameMesh + "] failed !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            this->m_aMeshes_Internal.push_back(pMesh);
            this->m_mapMeshes_Internal[nameMesh] = pMesh;

            F_LogInfo("VulkanWindow::createMeshes_Internal: create mesh: [%s], vertex type: [%s], mesh type: [%s], geometry type: [%s], mesh sub count: [%d], path: [%s] success !", 
                      nameMesh.c_str(), nameVertexType.c_str(), nameMeshType.c_str(), nameGeometryType.c_str(), (int)pMesh->aMeshSubs.size(), pathMesh.c_str());
        }
    }
    Mesh* VulkanWindow::FindMesh_Internal(const String& nameMesh)
    {
        MeshPtrMap::iterator itFind = this->m_mapMeshes_Internal.find(nameMesh);
        if (itFind == this->m_mapMeshes_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //Texture
    static const int g_TextureCount_Internal = 4;
    static const char* g_TexturePaths_Internal[5 * g_TextureCount_Internal] = 
    {
        //Texture Name                      //Texture Type   //TextureIsRenderTarget   //TextureIsGraphicsComputeShared   //Texture Path
        "default_black",                    "2D",            "false",                  "false",                           "Assets/Texture/Common/default_black.bmp", //default_black
        "default_white",                    "2D",            "false",                  "false",                           "Assets/Texture/Common/default_white.bmp", //default_white
        "default_blackwhite",               "2D",            "false",                  "false",                           "Assets/Texture/Common/default_blackwhite.png", //default_blackwhite
        "texture2d",                        "2D",            "false",                  "false",                           "Assets/Texture/Common/texture2d.jpg", //texture2d
        
    };
    static FTexturePixelFormatType g_TextureFormats_Internal[g_TextureCount_Internal] = 
    {
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_black
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_white
        F_TexturePixelFormat_R8G8B8A8_SRGB, //default_blackwhite
        F_TexturePixelFormat_R8G8B8A8_SRGB, //texture2d

    };
    static FTextureFilterType g_TextureFilters_Internal[g_TextureCount_Internal] = 
    {
        F_TextureFilter_Bilinear, //default_black
        F_TextureFilter_Bilinear, //default_white
        F_TextureFilter_Bilinear, //default_blackwhite
        F_TextureFilter_Bilinear, //texture2d

    };
    static FTextureAddressingType g_TextureAddressings_Internal[g_TextureCount_Internal] = 
    {
        F_TextureAddressing_Wrap, //default_black
        F_TextureAddressing_Wrap, //default_white
        F_TextureAddressing_Wrap, //default_blackwhite
        F_TextureAddressing_Clamp, //texture2d

    };
    static FTextureBorderColorType g_TextureBorderColors_Internal[g_TextureCount_Internal] = 
    {
        F_TextureBorderColor_OpaqueBlack, //default_black
        F_TextureBorderColor_OpaqueBlack, //default_white
        F_TextureBorderColor_OpaqueBlack, //default_blackwhite
        F_TextureBorderColor_OpaqueBlack, //texture2d

    };
    static int g_TextureSizes_Internal[3 * g_TextureCount_Internal] = 
    {
         64,     64,    1, //default_black
         64,     64,    1, //default_white
        512,    512,    1, //default_blackwhite
        512,    512,    1, //texture2d

    };
    void VulkanWindow::destroyTextures_Internal()
    {
        size_t count = this->m_aTextures_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            Texture* pTexture = this->m_aTextures_Internal[i];
            delete pTexture;
        }
        this->m_aTextures_Internal.clear();
        this->m_mapTextures_Internal.clear();
    }
    void VulkanWindow::createTextures_Internal()
    {
        for (int i = 0; i < g_TextureCount_Internal; i++)
        {
            String nameTexture = g_TexturePaths_Internal[5 * i + 0];
            String nameType = g_TexturePaths_Internal[5 * i + 1];
            FTextureType typeTexture = F_ParseTextureType(nameType);
            String nameIsRenderTarget = g_TexturePaths_Internal[5 * i + 2];
            bool isRenderTarget = FUtilString::ParserBool(nameIsRenderTarget);
            String nameIsGraphicsComputeShared = g_TexturePaths_Internal[5 * i + 3];
            bool isGraphicsComputeShared = FUtilString::ParserBool(nameIsGraphicsComputeShared);
            String pathTextures = g_TexturePaths_Internal[5 * i + 4];

            StringVector aPathTexture = FUtilString::Split(pathTextures, ";");
            Texture* pTexture = new Texture(0,
                                            nameTexture,
                                            aPathTexture,
                                            typeTexture,
                                            g_TextureFormats_Internal[i],
                                            g_TextureFilters_Internal[i],
                                            g_TextureAddressings_Internal[i],
                                            g_TextureBorderColors_Internal[i],
                                            isRenderTarget,
                                            isGraphicsComputeShared);
            pTexture->texChunkMaxX = 0;
            pTexture->texChunkMaxY = 0; 
            if (pTexture->texChunkMaxX > 0 && 
                pTexture->texChunkMaxY > 0)
            {
                pTexture->texChunkIndex = FMath::Rand(0, pTexture->texChunkMaxX * pTexture->texChunkMaxY - 1);
            }
            pTexture->AddRef();

            int width = g_TextureSizes_Internal[3 * i + 0];
            int height = g_TextureSizes_Internal[3 * i + 1];
            int depth = g_TextureSizes_Internal[3 * i + 1];
            pTexture->LoadTexture(width, 
                                  height,
                                  depth);

            this->m_aTextures_Internal.push_back(pTexture);
            this->m_mapTextures_Internal[nameTexture] = pTexture;

            F_LogInfo("VulkanWindow::createTextures_Internal: create texture: [%s], type: [%s], isRT: [%s], path: [%s] success !", 
                      nameTexture.c_str(), 
                      nameType.c_str(), 
                      isRenderTarget ? "true" : "false",
                      pathTextures.c_str());
        }
    }
    Texture* VulkanWindow::FindTexture_Internal(const String& nameTexture)
    {
        TexturePtrMap::iterator itFind = this->m_mapTextures_Internal.find(nameTexture);
        if (itFind == this->m_mapTextures_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //DescriptorSetLayouts
    static const int g_DescriptorSetLayoutCount_Internal = 13;
    static const char* g_DescriptorSetLayoutNames_Internal[g_DescriptorSetLayoutCount_Internal] =
    {
        "Pass",
        "Pass-Object",
        "Pass-CullInstance-BufferRWObjectCullInstance-BufferRWResultCB",
        "ObjectCopyBlit-TextureFrameColor",
        "ObjectCopyBlit-TextureFrameDepth",
        "Cull-BufferRWArgsCB",
        "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB",
        "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-TextureCSR",
        "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-BufferRWClipCB-TextureCSR",
        "HizDepth-TextureFS",
        "HizDepth-TextureCSRWSrc-TextureCSRWDst",
        "TextureCopy-TextureCSR-TextureCSRW",
        "Pass-ObjectTerrain-Material-Instance-Terrain-TextureVS-TextureVS-TextureFS-TextureFS-TextureFS",
    };
    void VulkanWindow::destroyDescriptorSetLayouts_Internal()
    {
        size_t count = this->m_aVkDescriptorSetLayouts_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            destroyVkDescriptorSetLayout(this->m_aVkDescriptorSetLayouts_Internal[i]);
        }
        this->m_aVkDescriptorSetLayouts_Internal.clear();
        this->m_mapVkDescriptorSetLayouts_Internal.clear();
        this->m_mapName2Layouts_Internal.clear();
    }   
    void VulkanWindow::createDescriptorSetLayouts_Internal()
    {
        for (int i = 0; i < g_DescriptorSetLayoutCount_Internal; i++)
        {
            String nameLayout(g_DescriptorSetLayoutNames_Internal[i]);
            StringVector aLayouts = FUtilString::Split(nameLayout, "-");
            VkDescriptorSetLayout vkDescriptorSetLayout = CreateDescriptorSetLayout(nameLayout, &aLayouts);
            if (vkDescriptorSetLayout == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createDescriptorSetLayouts_Internal: Failed to create descriptor set layout: " + nameLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->m_aVkDescriptorSetLayouts_Internal.push_back(vkDescriptorSetLayout);
            this->m_mapVkDescriptorSetLayouts_Internal[nameLayout] = vkDescriptorSetLayout;
            this->m_mapName2Layouts_Internal[nameLayout] = aLayouts;

            F_LogInfo("VulkanWindow::createDescriptorSetLayouts_Internal: create DescriptorSetLayout: [%s] success !", nameLayout.c_str());
        }
    }
    VkDescriptorSetLayout VulkanWindow::FindDescriptorSetLayout_Internal(const String& nameDescriptorSetLayout)
    {
        VkDescriptorSetLayoutMap::iterator itFind = this->m_mapVkDescriptorSetLayouts_Internal.find(nameDescriptorSetLayout);
        if (itFind == this->m_mapVkDescriptorSetLayouts_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    StringVector* VulkanWindow::FindDescriptorSetLayoutNames_Internal(const String& nameDescriptorSetLayout)
    {
        std::map<String, StringVector>::iterator itFind = this->m_mapName2Layouts_Internal.find(nameDescriptorSetLayout);
        if (itFind == this->m_mapName2Layouts_Internal.end())
        {
            return nullptr;
        }
        return &(itFind->second);
    }

    //ShaderModule
    static const int g_ShaderCount_Internal = 19;
    static const char* g_ShaderModulePaths_Internal[3 * g_ShaderCount_Internal] = 
    {
        //name                                                     //type               //path
        ///////////////////////////////////////// vert /////////////////////////////////////////
        "vert_standard_copy_blit_from_frame",                     "vert",              "Assets/Shader/standard_copy_blit_from_frame.vert.spv", //standard_copy_blit_from_frame vert
        "vert_standard_copy_blit_to_frame",                       "vert",              "Assets/Shader/standard_copy_blit_to_frame.vert.spv", //standard_copy_blit_to_frame vert
        "vert_standard_copy_blit_hiz_depth_from_frame",           "vert",              "Assets/Shader/standard_copy_blit_hiz_depth_from_frame.vert.spv", //standard_copy_blit_hiz_depth_from_frame vert
        "vert_standard_renderpass_shadowmap",                     "vert",              "Assets/Shader/standard_renderpass_shadowmap.vert.spv", //standard_renderpass_shadowmap vert
        "vert_standard_renderpass_shadowmap_cull",                "vert",              "Assets/Shader/standard_renderpass_shadowmap_cull.vert.spv", //standard_renderpass_shadowmap_cull vert
        "vert_standard_terrain_lit",                              "vert",              "Assets/Shader/standard_terrain_lit.vert.spv", //standard_terrain_lit vert

        ///////////////////////////////////////// tesc /////////////////////////////////////////
    

        ///////////////////////////////////////// tese /////////////////////////////////////////
    

        ///////////////////////////////////////// geom /////////////////////////////////////////


        ///////////////////////////////////////// frag /////////////////////////////////////////
        "frag_standard_copy_blit_from_frame",                     "frag",              "Assets/Shader/standard_copy_blit_from_frame.frag.spv", //standard_copy_blit_from_frame frag
        "frag_standard_copy_blit_to_frame",                       "frag",              "Assets/Shader/standard_copy_blit_to_frame.frag.spv", //standard_copy_blit_to_frame frag
        "frag_standard_copy_blit_hiz_depth_from_frame",           "frag",              "Assets/Shader/standard_copy_blit_hiz_depth_from_frame.frag.spv", //standard_copy_blit_hiz_depth_from_frame frag
        "frag_standard_renderpass_shadowmap",                     "frag",              "Assets/Shader/standard_renderpass_shadowmap.frag.spv", //standard_renderpass_shadowmap frag
        "frag_standard_terrain_lit",                              "frag",              "Assets/Shader/standard_terrain_lit.frag.spv", //standard_terrain_lit frag

        ///////////////////////////////////////// comp /////////////////////////////////////////
        "comp_standard_compute_texcopy_tex2d",                    "comp",              "Assets/Shader/standard_compute_texcopy_tex2d.comp.spv", //standard_compute_texcopy_tex2d comp
        "comp_standard_compute_texcopy_tex2darray",               "comp",              "Assets/Shader/standard_compute_texcopy_tex2darray.comp.spv", //standard_compute_texcopy_tex2darray comp
        "comp_standard_compute_texgen_normalmap",                 "comp",              "Assets/Shader/standard_compute_texgen_normalmap.comp.spv", //standard_compute_texgen_normalmap comp

        "comp_standard_compute_cull_clear_args",                  "comp",              "Assets/Shader/standard_compute_cull_clear_args.comp.spv", //standard_compute_cull_clear_args comp
        "comp_standard_compute_cull_frustum",                     "comp",              "Assets/Shader/standard_compute_cull_frustum.comp.spv", //standard_compute_cull_frustum comp
        "comp_standard_compute_cull_frustum_depth_hiz",           "comp",              "Assets/Shader/standard_compute_cull_frustum_depth_hiz.comp.spv", //standard_compute_cull_frustum_depth_hiz comp
        "comp_standard_compute_cull_frustum_depth_hiz_clip",      "comp",              "Assets/Shader/standard_compute_cull_frustum_depth_hiz_clip.comp.spv", //standard_compute_cull_frustum_depth_hiz_clip comp
        "comp_standard_compute_hiz_depth_generate",               "comp",              "Assets/Shader/standard_compute_hiz_depth_generate.comp.spv", //standard_compute_hiz_depth_generate comp
    };
    void VulkanWindow::destroyShaderModules_Internal()
    {
        size_t count = this->m_aVkShaderModules_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            VkShaderModule& vkShaderModule= this->m_aVkShaderModules_Internal[i];
            destroyVkShaderModule(vkShaderModule);
        }
        this->m_aVkShaderModules_Internal.clear();
        this->m_mapVkShaderModules_Internal.clear();
    }
    void VulkanWindow::createShaderModules_Internal()
    {
        for (int i = 0; i < g_ShaderCount_Internal; i++)
        {
            String shaderName = g_ShaderModulePaths_Internal[3 * i + 0];
            String shaderType = g_ShaderModulePaths_Internal[3 * i + 1];
            String shaderPath = g_ShaderModulePaths_Internal[3 * i + 2];

            VkShaderModule shaderModule = createVkShaderModule(shaderName, shaderType, shaderPath);
            this->m_aVkShaderModules_Internal.push_back(shaderModule);
            this->m_mapVkShaderModules_Internal[shaderName] = shaderModule;
            F_LogInfo("VulkanWindow::createShaderModules_Internal: create shader, name: [%s], type: [%s], path: [%s] success !", 
                      shaderName.c_str(), shaderType.c_str(), shaderPath.c_str());
        }
    }
    VkShaderModule VulkanWindow::FindShaderModule_Internal(const String& nameShaderModule)
    {
        VkShaderModuleMap::iterator itFind = this->m_mapVkShaderModules_Internal.find(nameShaderModule);
        if (itFind == this->m_mapVkShaderModules_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //PipelineLayout
    void VulkanWindow::destroyPipelineLayouts_Internal()
    {
        size_t count = this->m_aVkPipelineLayouts_Internal.size();
        for (size_t i = 0; i < count; i++)
        {
            destroyVkPipelineLayout(this->m_aVkPipelineLayouts_Internal[i]);
        }
        this->m_aVkPipelineLayouts_Internal.clear();
        this->m_mapVkPipelineLayouts_Internal.clear();
    }
    void VulkanWindow::createPipelineLayouts_Internal()
    {   
        for (int i = 0; i < g_DescriptorSetLayoutCount_Internal; i++)
        {
            String nameDSL(g_DescriptorSetLayoutNames_Internal[i]);
            VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(nameDSL);
            if (vkDescriptorSetLayout == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanWindow::createPipelineLayouts_Internal: Can not find DescriptorSetLayout by name: [%s]", nameDSL.c_str());
                return;
            }

            VkDescriptorSetLayoutVector aDescriptorSetLayout;
            aDescriptorSetLayout.push_back(vkDescriptorSetLayout);
            VkPipelineLayout vkPipelineLayout = createVkPipelineLayout(nameDSL, aDescriptorSetLayout);
            if (vkPipelineLayout == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanWindow::createPipelineLayouts_Internal: createVkPipelineLayout failed !");
                return;
            }

            this->m_aVkPipelineLayouts_Internal.push_back(vkPipelineLayout);
            this->m_mapVkPipelineLayouts_Internal[nameDSL] = vkPipelineLayout;
        }
    }
    VkPipelineLayout VulkanWindow::FindPipelineLayout_Internal(const String& namePipelineLayout)
    {
        VkPipelineLayoutMap::iterator itFind = this->m_mapVkPipelineLayouts_Internal.find(namePipelineLayout);
        if (itFind == this->m_mapVkPipelineLayouts_Internal.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    //UniformConstantBuffer
    void VulkanWindow::destroyUniformCB_Internal()
    {
        //1> PassCB
        destroyUniform_PassCB();
    }
        void VulkanWindow::destroyUniform_PassCB()
        {
            size_t count = this->poBuffers_PassCB.size();
            for (size_t i = 0; i < count; i++) 
            {
                destroyVkBuffer(this->poBuffers_PassCB[i], this->poBuffersMemory_PassCB[i]);
            }
            this->poBuffers_PassCB.clear();
            this->poBuffersMemory_PassCB.clear();
        }
        
    void VulkanWindow::createUniformCB_Internal()
    {
        //1> PassCB
        createUniform_PassCB();
    }
        void VulkanWindow::createUniform_PassCB()
        {
            VkDeviceSize bufferSize = sizeof(PassConstants);
            size_t count = this->poSwapChainImages.size();
            this->poBuffers_PassCB.resize(count);
            this->poBuffersMemory_PassCB.resize(count);

            for (size_t i = 0; i < count; i++) 
            {
                String nameBuffer = "PassConstants-" + FUtilString::SaveSizeT(i);
                createVkBuffer(nameBuffer,
                               bufferSize, 
                               VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                               this->poBuffers_PassCB[i], 
                               this->poBuffersMemory_PassCB[i]);
            }
            F_LogInfo("VulkanWindow::createUniform_PassCB: Create Uniform Pass constant buffer success !");
        }

    //PipelineCompute
    void VulkanWindow::destroyPipelineCompute_Internal()
    {
        destroyPipelineCompute_Cull();
        destroyPipelineCompute_Terrain();
    }
        void VulkanWindow::destroyPipelineCompute_Cull()
        {
            F_DELETE(m_pPipelineCompute_Cull)
        }
        void VulkanWindow::destroyPipelineCompute_Terrain()
        {
            F_DELETE(m_pPipelineCompute_Terrain)
        }

    void VulkanWindow::createPipelineCompute_Internal()
    {
        createPipelineCompute_Cull();
        createPipelineCompute_Terrain();
    }
        void VulkanWindow::createPipelineCompute_Cull()
        {
            if (!this->cfg_isRenderPassCull)
                return;

            this->m_pPipelineCompute_Cull = new VKPipelineComputeCull("PipelineCompute-Cull", this->m_pVKRenderPassCull);
            if (!this->m_pPipelineCompute_Cull->Init())
            {
                F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->Init failed !");
                return;
            }

            //PipelineCompute-CullClearArgs
            {
                String descriptorSetLayout = "Cull-BufferRWArgsCB";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
                VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_cull_clear_args");

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         vkShaderModule != nullptr &&
                         "VulkanWindow::createPipelineCompute_Cull")

                if (!this->m_pPipelineCompute_Cull->InitCullClearArgs(descriptorSetLayout,
                                                                      pDescriptorSetLayoutNames,
                                                                      vkDescriptorSetLayout,
                                                                      vkPipelineLayout,
                                                                      vkShaderModule))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->InitCullClearArgs failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create [PipelineCompute-CullClearArgs] success !");
            }

            //PipelineCompute-CullFrustum
            {
                String descriptorSetLayout = "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
                VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_cull_frustum");

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         vkShaderModule != nullptr &&
                         "VulkanWindow::createPipelineCompute_Cull")

                if (!this->m_pPipelineCompute_Cull->InitCullFrustum(descriptorSetLayout,
                                                                    pDescriptorSetLayoutNames,
                                                                    vkDescriptorSetLayout,
                                                                    vkPipelineLayout,
                                                                    vkShaderModule))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->InitCullFrustum failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create [PipelineCompute-CullFrustum] success !");
            }

            //PipelineCompute-CullFrustumDepthHiz
            {
                String descriptorSetLayout = "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-TextureCSR";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
                VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_cull_frustum_depth_hiz");

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         vkShaderModule != nullptr &&
                         "VulkanWindow::createPipelineCompute_Cull")

                if (!this->m_pPipelineCompute_Cull->InitCullFrustumDepthHiz(descriptorSetLayout,
                                                                            pDescriptorSetLayoutNames,
                                                                            vkDescriptorSetLayout,
                                                                            vkPipelineLayout,
                                                                            vkShaderModule))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->InitCullFrustumDepthHiz failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create [PipelineCompute-CullFrustumDepthHiz] success !");
            }

            //PipelineCompute-CullFrustumDepthHizClip
            {
                String descriptorSetLayout = "Cull-ObjectCull-BufferRWArgsCB-BufferRWLodCB-BufferRWResultCB-BufferRWClipCB-TextureCSR";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
                VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_cull_frustum_depth_hiz_clip");

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         vkShaderModule != nullptr &&
                         "VulkanWindow::createPipelineCompute_Cull")

                if (!this->m_pPipelineCompute_Cull->InitCullFrustumDepthHizClip(descriptorSetLayout,
                                                                                pDescriptorSetLayoutNames,
                                                                                vkDescriptorSetLayout,
                                                                                vkPipelineLayout,
                                                                                vkShaderModule))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->InitCullFrustumDepthHizClip failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create [PipelineCompute-CullFrustumDepthHizClip] success !");
            }

            //PipelineCompute-HizDepthGenerate
            {
                String descriptorSetLayout = "HizDepth-TextureCSRWSrc-TextureCSRWDst";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
                VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_hiz_depth_generate");

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         vkShaderModule != nullptr &&
                         "VulkanWindow::createPipelineCompute_Cull")

                if (!this->m_pPipelineCompute_Cull->InitHizDepthGenerate(descriptorSetLayout,
                                                                         pDescriptorSetLayoutNames,
                                                                         vkDescriptorSetLayout,
                                                                         vkPipelineLayout,
                                                                         vkShaderModule))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineCompute_Cull: m_pPipelineCompute_Cull->InitHizDepthGenerate failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create [PipelineCompute-HizDepthGenerate] success !");
            }

            F_LogInfo("VulkanWindow::createPipelineCompute_Cull: Create PipelineCompute_Cull success !");
        }
    void VulkanWindow::Update_Compute_Cull(VkCommandBuffer& commandBuffer)
    {
        if (!this->cfg_isRenderPassCull ||
            this->m_pPipelineCompute_Cull == nullptr)
            return;

        this->m_pPipelineCompute_Cull->Dispatch_Cull(commandBuffer);
    }
    void VulkanWindow::Update_Compute_HizDepthGenerate(VkCommandBuffer& commandBuffer)
    {
        if (!this->cfg_isRenderPassCull ||
            !this->isComputeCullFrustumHizDepth ||
            this->m_pPipelineCompute_Cull == nullptr)
            return;
        
        this->m_pPipelineCompute_Cull->Dispatch_HizDepthGenerate(commandBuffer);
    }

        void VulkanWindow::createPipelineCompute_Terrain()
        {
            if (!this->cfg_isRenderPassTerrain)
                return;

            this->m_pPipelineCompute_Terrain = new VKPipelineComputeTerrain("PipelineCompute-Terrain", this->m_pVKRenderPassTerrain);
            String descriptorSetLayout = "TextureCopy-TextureCSR-TextureCSRW";
            StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
            VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
            VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);
            VkShaderModule vkShaderModule = FindShaderModule_Internal("comp_standard_compute_texgen_normalmap");

            F_Assert(pDescriptorSetLayoutNames != nullptr &&
                     vkDescriptorSetLayout != nullptr &&
                     vkPipelineLayout != nullptr &&
                     vkShaderModule != nullptr &&
                     "VulkanWindow::createPipelineCompute_Terrain")

            if (!this->m_pPipelineCompute_Terrain->Init(descriptorSetLayout,
                                                        pDescriptorSetLayoutNames,
                                                        vkDescriptorSetLayout,
                                                        vkPipelineLayout,
                                                        vkShaderModule))
            {
                F_LogError("*********************** VulkanWindow::createPipelineCompute_Terrain: m_pPipelineCompute_Terrain->Init failed !");
                return;
            }
            F_LogInfo("VulkanWindow::createPipelineCompute_Terrain: Create PipelineCompute_Terrain success !");
        }
    void VulkanWindow::UpdateDescriptorSets_Compute_Terrain()
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineCompute_Terrain == nullptr)
            return;

        this->m_pPipelineCompute_Terrain->UpdateDescriptorSet();
    }
        void VulkanWindow::Update_Compute_Terrain(VkCommandBuffer& commandBuffer)
        {
            if (!this->cfg_isRenderPassTerrain ||
                this->m_pPipelineCompute_Terrain == nullptr ||
                this->m_pVKRenderPassTerrain == nullptr)
                return;

            if (!this->m_pPipelineCompute_Terrain->isNormalUpdated ||
                this->m_pPipelineCompute_Terrain->isNormalUpdated_Sustained)
            {
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->m_pPipelineCompute_Terrain->poPipeline);
                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE, this->m_pPipelineCompute_Terrain->poPipelineLayout, 0, 1, &this->m_pPipelineCompute_Terrain->poDescriptorSet, 0, 0);
                
                uint32_t groupX = (uint32_t)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize / 8);
                uint32_t groupY = (uint32_t)(this->m_pVKRenderPassTerrain->poTerrainHeightMapSize / 8);
                dispatch(commandBuffer, groupX, groupY, 1);

                this->m_pPipelineCompute_Terrain->isNormalUpdated = true;
            }
        }

    //PipelineGraphics
    void VulkanWindow::destroyPipelineGraphics_Internal()
    {
        destroyPipelineGraphics_Terrain();
        destroyPipelineGraphics_DepthHiz();
        destroyPipelineGraphics_DepthShadowMap();
        destroyPipelineGraphics_CopyBlitToFrame();
        destroyPipelineGraphics_CopyBlitFromFrame();
    }
        void VulkanWindow::destroyPipelineGraphics_CopyBlitFromFrame()
        {
            F_DELETE(m_pPipelineGraphics_CopyBlitFromFrameColor)
            F_DELETE(m_pPipelineGraphics_CopyBlitFromFrameDepth)
        }
        void VulkanWindow::destroyPipelineGraphics_CopyBlitToFrame()
        {
            F_DELETE(m_pPipelineGraphics_CopyBlitToFrame)
        }
        void VulkanWindow::destroyPipelineGraphics_DepthShadowMap()
        {
            F_DELETE(m_pPipelineGraphics_DepthShadowMap)
        }
        void VulkanWindow::destroyPipelineGraphics_DepthHiz()
        {
            F_DELETE(m_pPipelineGraphics_DepthHiz)
        }
        void VulkanWindow::destroyPipelineGraphics_Terrain()
        {
            F_DELETE(m_pPipelineGraphics_Terrain)
        }

    void VulkanWindow::createPipelineGraphics_Internal()
    {
        createPipelineGraphics_CopyBlitFromFrame();
        createPipelineGraphics_CopyBlitToFrame();
        createPipelineGraphics_DepthShadowMap();
        createPipelineGraphics_DepthHiz();
        createPipelineGraphics_Terrain();

    }
        void VulkanWindow::createPipelineGraphics_CopyBlitFromFrame()
        {
            if (this->cfg_isUseCopyBlitFromFrameColor)
            {
                this->m_pPipelineGraphics_CopyBlitFromFrameColor = new VKPipelineGraphicsCopyBlitFromFrame("PipelineGraphics-CopyBlitFromFrameColor");
                String descriptorSetLayout = "ObjectCopyBlit-TextureFrameColor";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         "VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Color")

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_CopyBlitFromFrameColor;
                String nameShaderVert = "vert_standard_copy_blit_from_frame";
                String nameShaderFrag = "frag_standard_copy_blit_from_frame";
                if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                          "",
                                                          "",
                                                          "",
                                                          nameShaderFrag,
                                                          this->m_mapVkShaderModules_Internal,
                                                          aShaderStageCreateInfos_CopyBlitFromFrameColor))
                {
                    String msg = "*********************** VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Color: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                Mesh* pMeshBlit = FindMesh_Internal("quad");
                F_Assert(pMeshBlit && "VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Color");
                if (!this->m_pPipelineGraphics_CopyBlitFromFrameColor->Init(this->poSwapChainExtent.width,
                                                                            this->poSwapChainExtent.height,
                                                                            this->poSwapChainImageFormat,
                                                                            false,
                                                                            pMeshBlit,
                                                                            descriptorSetLayout,
                                                                            pDescriptorSetLayoutNames,
                                                                            vkDescriptorSetLayout,
                                                                            vkPipelineLayout,
                                                                            aShaderStageCreateInfos_CopyBlitFromFrameColor))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Color: PipelineGraphics_CopyBlitFromFrameColor->Init failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineGraphics_CopyBlitFromFrame: [PipelineGraphics_CopyBlitFromFrameColor] create success !");
            }
            if (this->cfg_isUseCopyBlitFromFrameDepth)
            {
                this->m_pPipelineGraphics_CopyBlitFromFrameDepth = new VKPipelineGraphicsCopyBlitFromFrame("PipelineGraphics-CopyBlitFromFrameDepth");
                String descriptorSetLayout = "ObjectCopyBlit-TextureFrameDepth";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         "VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Depth")

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_CopyBlitFromFrameDepth;
                String nameShaderVert = "vert_standard_copy_blit_from_frame";
                String nameShaderFrag = "frag_standard_copy_blit_from_frame";
                if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                          "",
                                                          "",
                                                          "",
                                                          nameShaderFrag,
                                                          this->m_mapVkShaderModules_Internal,
                                                          aShaderStageCreateInfos_CopyBlitFromFrameDepth))
                {
                    String msg = "*********************** VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Depth: Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                Mesh* pMeshBlit = FindMesh_Internal("quad");
                F_Assert(pMeshBlit && "VulkanWindow::createPipelineGraphics_CopyBlitFromFrame Depth");
                if (!this->m_pPipelineGraphics_CopyBlitFromFrameDepth->Init(this->poSwapChainExtent.width,
                                                                            this->poSwapChainExtent.height,
                                                                            this->poDepthImageFormat,
                                                                            true,
                                                                            pMeshBlit,
                                                                            descriptorSetLayout,
                                                                            pDescriptorSetLayoutNames,
                                                                            vkDescriptorSetLayout,
                                                                            vkPipelineLayout,
                                                                            aShaderStageCreateInfos_CopyBlitFromFrameDepth))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineGraphics_CopyBlitFromFrame: PipelineGraphics_CopyBlitFromFrameDepth->Init failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineGraphics_CopyBlitFromFrame: [PipelineGraphics_CopyBlitFromFrameDepth] create success !");
            }
        }
        void VulkanWindow::createPipelineGraphics_CopyBlitToFrame()
        {
            this->m_pPipelineGraphics_CopyBlitToFrame = new VKPipelineGraphicsCopyBlitToFrame("PipelineGraphics-CopyBlitToFrame");
            String descriptorSetLayout = "ObjectCopyBlit-TextureFrameColor";
            StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
            VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
            VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

            F_Assert(pDescriptorSetLayoutNames != nullptr &&
                     vkDescriptorSetLayout != nullptr &&
                     vkPipelineLayout != nullptr &&
                     "VulkanWindow::createPipelineGraphics_CopyBlitToFrame")

            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_CopyBlitToFrame;
            String nameShaderVert = "vert_standard_copy_blit_to_frame";
            String nameShaderFrag = "frag_standard_copy_blit_to_frame";
            if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                      "",
                                                      "",
                                                      "",
                                                      nameShaderFrag,
                                                      this->m_mapVkShaderModules_Internal,
                                                      aShaderStageCreateInfos_CopyBlitToFrame))
            {
                String msg = "*********************** VulkanWindow::createPipelineGraphics_CopyBlitToFrame: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            Mesh* pMeshBlit = FindMesh_Internal("quad");
            F_Assert(pMeshBlit && "VulkanWindow::createPipelineGraphics_CopyBlitToFrame");
            if (!this->m_pPipelineGraphics_CopyBlitToFrame->Init(pMeshBlit,
                                                                 descriptorSetLayout,
                                                                 pDescriptorSetLayoutNames,
                                                                 vkDescriptorSetLayout,
                                                                 vkPipelineLayout,
                                                                 aShaderStageCreateInfos_CopyBlitToFrame))
            {
                F_LogError("*********************** VulkanWindow::createPipelineGraphics_CopyBlitToFrame: PipelineGraphics_CopyBlitToFrame->Init failed !");
                return;
            }
            F_LogInfo("VulkanWindow::createPipelineGraphics_CopyBlitToFrame: [PipelineGraphics_CopyBlitToFrame] create success !");
        }
    void VulkanWindow::UpdateDescriptorSets_Graphics_CopyBlitToFrame(const VkDescriptorImageInfo& imageInfo)
    {
        this->m_pPipelineGraphics_CopyBlitToFrame->UpdateDescriptorSets(imageInfo);
    }
    void VulkanWindow::UpdateBuffer_Graphics_CopyBlitToFrame(const CopyBlitObjectConstants& object)
    {
        this->m_pPipelineGraphics_CopyBlitToFrame->UpdateBuffer(object);
    }
    void VulkanWindow::Draw_Graphics_CopyBlitToFrame(VkCommandBuffer& commandBuffer)
    {
        Mesh* pMesh = this->m_pPipelineGraphics_CopyBlitToFrame->pMeshBlit;
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
        VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        if (this->cfg_isWireFrame)
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_CopyBlitToFrame->poPipeline_WireFrame);
        else
            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_CopyBlitToFrame->poPipeline);
        bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_CopyBlitToFrame->poPipelineLayout, 0, 1, &this->m_pPipelineGraphics_CopyBlitToFrame->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);
    }

    void VulkanWindow::UpdateDescriptorSets_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const VkImageView& imageView)
    {
        if (pCopyBlitFromFrame == nullptr)
            return;

        pCopyBlitFromFrame->UpdateDescriptorSets(imageView);
    }
    void VulkanWindow::UpdateDescriptorSets_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const VkDescriptorImageInfo& imageInfo)
    {
        if (pCopyBlitFromFrame == nullptr)
            return;

        pCopyBlitFromFrame->UpdateDescriptorSets(imageInfo);
    }
    void VulkanWindow::UpdateBuffer_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const CopyBlitObjectConstants& object)
    {
        if (pCopyBlitFromFrame == nullptr)
            return;

        pCopyBlitFromFrame->UpdateBuffer(object);
    }
    void VulkanWindow::Draw_Graphics_CopyBlitFromFrame(VkCommandBuffer& commandBuffer, VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame)
    {
        if (pCopyBlitFromFrame == nullptr)
            return;

        Mesh* pMesh = pCopyBlitFromFrame->pMeshBlit;
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
        VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pCopyBlitFromFrame->poPipeline);
        bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pCopyBlitFromFrame->poPipelineLayout, 0, 1, &pCopyBlitFromFrame->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
        drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);
    }


        void VulkanWindow::createPipelineGraphics_DepthShadowMap()
        {
            this->m_pPipelineGraphics_DepthShadowMap = new VKPipelineGraphicsDepthShadowMap("PipelineGraphics-DepthShadowMap", this->m_pVKRenderPassShadowMap);
            if (!this->m_pPipelineGraphics_DepthShadowMap->Init())
            {
                F_LogError("*********************** VulkanWindow::createPipelineGraphics_DepthShadowMap: m_pPipelineGraphics_DepthShadowMap->Init failed !");
                return;
            }
            
            //PipelineGraphics-DepthShadowMap
            {
                String descriptorSetLayout = "Pass-Object";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         "VulkanWindow::createPipelineGraphics_DepthShadowMap-[PipelineGraphics-DepthShadowMap]")

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_DepthShadowMap;
                String nameShaderVert = "vert_standard_renderpass_shadowmap";
                if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                          "",
                                                          "",
                                                          "",
                                                          "",
                                                          this->m_mapVkShaderModules_Internal,
                                                          aShaderStageCreateInfos_DepthShadowMap))
                {
                    String msg = "*********************** VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMap] Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                if (!this->m_pPipelineGraphics_DepthShadowMap->InitShadowMapDepth(descriptorSetLayout,
                                                                                  pDescriptorSetLayoutNames,
                                                                                  vkDescriptorSetLayout,
                                                                                  vkPipelineLayout,
                                                                                  aShaderStageCreateInfos_DepthShadowMap))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMap] PipelineGraphics_DepthShadowMap->Init failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMap] create success !");
            }

            //PipelineGraphics-DepthShadowMapCull
            {
                String descriptorSetLayout = "Pass-CullInstance-BufferRWObjectCullInstance-BufferRWResultCB";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         "VulkanWindow::createPipelineGraphics_DepthShadowMap-[PipelineGraphics-DepthShadowMapCull]")

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_DepthShadowMap;
                String nameShaderVert = "vert_standard_renderpass_shadowmap_cull";
                if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                          "",
                                                          "",
                                                          "",
                                                          "",
                                                          this->m_mapVkShaderModules_Internal,
                                                          aShaderStageCreateInfos_DepthShadowMap))
                {
                    String msg = "*********************** VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMapCull] Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                if (!this->m_pPipelineGraphics_DepthShadowMap->InitShadowMapDepthCull(descriptorSetLayout,
                                                                                      pDescriptorSetLayoutNames,
                                                                                      vkDescriptorSetLayout,
                                                                                      vkPipelineLayout,
                                                                                      aShaderStageCreateInfos_DepthShadowMap))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMapCull] PipelineGraphics_DepthShadowMap->Init failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineGraphics_DepthShadowMap: [PipelineGraphics-DepthShadowMapCull] create success !");
            }
        }
    void VulkanWindow::UpdateBuffer_ObjectWorld_Begin()
    {
        this->m_pPipelineGraphics_DepthShadowMap->UpdateBuffer_ObjectWorld_Clear();
    }
        void VulkanWindow::UpdateBuffer_ObjectWorld_AddOne(const ObjectConstants& object)
        {
            this->m_pPipelineGraphics_DepthShadowMap->UpdateBuffer_ObjectWorld_AddOne(object);
        }
        void VulkanWindow::UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects)
        {
            this->m_pPipelineGraphics_DepthShadowMap->UpdateBuffer_ObjectWorld_AddList(objects);
        }
    void VulkanWindow::UpdateBuffer_ObjectWorld_End()
    {
        this->m_pPipelineGraphics_DepthShadowMap->UpdateBuffer_ObjectWorld_Update();
    }
    bool VulkanWindow::Draw_Graphics_DepthShadowMapBegin(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pPipelineGraphics_DepthShadowMap == nullptr ||
            !this->cfg_isRenderPassShadowMap)
            return false;

        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthShadowMap->poPipeline_ShadowMapDepth);
        bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthShadowMap->poPipelineLayout_ShadowMapDepth, 0, 1, &this->m_pPipelineGraphics_DepthShadowMap->poDescriptorSets_ShadowMapDepth[this->poSwapChainImageIndex], 0, nullptr);

        return true;
    }
    void VulkanWindow::Draw_Graphics_DepthShadowMap(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceCount, int instanceStart)
    {   
        VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        if (pMeshSub->poIndexBuffer != nullptr)
        {
            bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

        if (pMeshSub->poIndexBuffer != nullptr)
        {
            drawIndexed(commandBuffer, pMeshSub->poIndexCount, instanceCount, 0, 0, instanceStart);
        }
        else
        {
            draw(commandBuffer, pMeshSub->poVertexCount, instanceCount, 0, instanceStart);
        }
    }
    void VulkanWindow::Draw_Graphics_DepthShadowMapEnd(VkCommandBuffer& commandBuffer)
    {

    }

    void VulkanWindow::UpdateDescriptorSet_ShadowMapDepthCull(VkDescriptorSetVector* pescriptorSets, BufferUniform* pCB_CullInstance, BufferCompute* pCB_CullObjectInstances, BufferCompute* pCB_Result)
    {
        this->m_pPipelineGraphics_DepthShadowMap->UpdateDescriptorSet_ShadowMapDepthCull(pescriptorSets, pCB_CullInstance, pCB_CullObjectInstances, pCB_Result);
    }
    bool VulkanWindow::Draw_Graphics_CullInstance_DepthShadowMapCullBegin(VkCommandBuffer& commandBuffer)
    {
        CullManager* pCullManager = CullManager::GetSingletonPtr();
        bool isCulling = false;
        if (pCullManager)
            isCulling = pCullManager->isEnable;
        if (this->m_pPipelineGraphics_DepthShadowMap == nullptr ||
            !this->cfg_isRenderPassShadowMap ||
            !isCulling)
            return false;

        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthShadowMap->poPipeline_ShadowMapDepthCull);

        return true;
    }
        void VulkanWindow::Draw_Graphics_CullInstance_DepthShadowMapCull(VkCommandBuffer& commandBuffer)
        {
            CullManager* pCullManager = CullManager::GetSingletonPtr();
            if (pCullManager->isEnable)
            {
                pCullManager->ExecuteShadowMapDraw(commandBuffer);
            }
        }
        void VulkanWindow::Draw_Graphics_CullInstance_DepthShadowMapCullUnit(VkCommandBuffer& commandBuffer, const VkBuffer& bufferIndirectCmd, int index, MeshSub* pMeshSub)
        {
            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            if (pMeshSub->poIndexBuffer != nullptr)
            {
                bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            }
            
            drawIndexedIndirect(commandBuffer, bufferIndirectCmd, index * sizeof(VkDrawIndexedIndirectCommand), 1, sizeof(VkDrawIndexedIndirectCommand));
        }
        void VulkanWindow::Draw_Graphics_BindDescriptorSet_ShadowMapDepthCull(VkCommandBuffer& commandBuffer, VkDescriptorSetVector* pescriptorSets)
        {
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthShadowMap->poPipelineLayout_ShadowMapDepthCull, 0, 1, &(*pescriptorSets)[this->poSwapChainImageIndex], 0, nullptr);
        }
    void VulkanWindow::Draw_Graphics_CullInstance_DepthShadowMapCullEnd(VkCommandBuffer& commandBuffer)
    {

    }


        void VulkanWindow::createPipelineGraphics_DepthHiz()
        {
            if (this->m_pVKRenderPassCull == nullptr)
            {
                return;
            }

            Mesh* pMesh = FindMesh_Internal("quad");
            this->m_pPipelineGraphics_DepthHiz = new VKPipelineGraphicsDepthHiz("PipelineGraphics-DepthHiz", this->m_pVKRenderPassCull);
            if (!this->m_pPipelineGraphics_DepthHiz->Init(pMesh))
            {
                F_LogError("*********************** VulkanWindow::createPipelineGraphics_DepthHiz: m_pPipelineGraphics_DepthHiz->Init failed !");
                return;
            }

            //PipelineGraphics-DepthHiz
            {
                String descriptorSetLayout = "HizDepth-TextureFS";
                StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
                VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
                VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

                F_Assert(pDescriptorSetLayoutNames != nullptr &&
                         vkDescriptorSetLayout != nullptr &&
                         vkPipelineLayout != nullptr &&
                         "VulkanWindow::createPipelineGraphics_DepthHiz-[PipelineGraphics-DepthHiz]")

                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_DepthHiz;
                String nameShaderVert = "vert_standard_copy_blit_hiz_depth_from_frame";
                String nameShaderFrag = "frag_standard_copy_blit_hiz_depth_from_frame";
                if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                          "",
                                                          "",
                                                          "",
                                                          nameShaderFrag,
                                                          this->m_mapVkShaderModules_Internal,
                                                          aShaderStageCreateInfos_DepthHiz))
                {
                    String msg = "*********************** VulkanWindow::createPipelineGraphics_DepthHiz: [PipelineGraphics-DepthHiz] Can not find shader used !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                if (!this->m_pPipelineGraphics_DepthHiz->InitHizDepth(descriptorSetLayout,
                                                                      pDescriptorSetLayoutNames,
                                                                      vkDescriptorSetLayout,
                                                                      vkPipelineLayout,
                                                                      aShaderStageCreateInfos_DepthHiz))
                {
                    F_LogError("*********************** VulkanWindow::createPipelineGraphics_DepthHiz: [PipelineGraphics-DepthHiz] PipelineGraphics_DepthHiz->Init failed !");
                    return;
                }
                F_LogInfo("VulkanWindow::createPipelineGraphics_DepthHiz: [PipelineGraphics-DepthHiz] create success !");
            }
        }
    void VulkanWindow::Draw_Graphics_DepthHiz(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pVKRenderPassCull == nullptr ||
            this->m_pPipelineGraphics_DepthHiz == nullptr)
        {
            return;
        }

        this->m_pVKRenderPassCull->UpdateHizDepthBuffer_Render();
        Mesh* pMesh = this->m_pPipelineGraphics_DepthHiz->pMesh;
        MeshSub* pMeshSub = pMesh->aMeshSubs[0];
        VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
        VkDeviceSize offsets[] = { 0 };
        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
        bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthHiz->poPipeline_HizDepth);
        bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_DepthHiz->poPipelineLayout_HizDepth, 0, 1, &this->m_pPipelineGraphics_DepthHiz->poDescriptorSets_HizDepth[this->poSwapChainImageIndex], 0, nullptr);
        drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, 0);
    }
    void VulkanWindow::UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToShaderReadOnly(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pVKRenderPassCull == nullptr ||
            this->m_pPipelineGraphics_DepthHiz == nullptr)
        {
            return;
        }

        this->m_pVKRenderPassCull->UpdateHizDepthBuffer_ImageLayoutFromColorAttachmentToShaderReadOnly(commandBuffer);
    }
    void VulkanWindow::UpdateImageLayout_Graphics_DepthHizImageLayoutFromShaderReadOnlyToColorAttachment(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pVKRenderPassCull == nullptr ||
            this->m_pPipelineGraphics_DepthHiz == nullptr)
        {
            return;
        }

        this->m_pVKRenderPassCull->UpdateHizDepthBuffer_ImageLayoutFromShaderReadOnlyToColorAttachment(commandBuffer);
    }
    void VulkanWindow::UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToGeneral(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pVKRenderPassCull == nullptr ||
            this->m_pPipelineGraphics_DepthHiz == nullptr)
        {
            return;
        }

        this->m_pVKRenderPassCull->UpdateHizDepthBuffer_ImageLayoutFromColorAttachmentToGeneral(commandBuffer);
    }
    void VulkanWindow::UpdateImageLayout_Graphics_DepthHizImageLayoutFromGeneralToColorAttachment(VkCommandBuffer& commandBuffer)
    {
        if (this->m_pVKRenderPassCull == nullptr ||
            this->m_pPipelineGraphics_DepthHiz == nullptr)
        {
            return;
        }

        this->m_pVKRenderPassCull->UpdateHizDepthBuffer_ImageLayoutFromGeneralToColorAttachment(commandBuffer);
    }


        void VulkanWindow::createPipelineGraphics_Terrain()
        {
            if (!this->cfg_isRenderPassTerrain)
                return;

            this->m_pPipelineGraphics_Terrain = new VKPipelineGraphicsTerrain("PipelineGraphics-Terrain", this->m_pVKRenderPassTerrain);
            String descriptorSetLayout = "Pass-ObjectTerrain-Material-Instance-Terrain-TextureVS-TextureVS-TextureFS-TextureFS-TextureFS";
            StringVector* pDescriptorSetLayoutNames = FindDescriptorSetLayoutNames_Internal(descriptorSetLayout);
            VkDescriptorSetLayout vkDescriptorSetLayout = FindDescriptorSetLayout_Internal(descriptorSetLayout);
            VkPipelineLayout vkPipelineLayout = FindPipelineLayout_Internal(descriptorSetLayout);

            F_Assert(pDescriptorSetLayoutNames != nullptr &&
                     vkDescriptorSetLayout != nullptr &&
                     vkPipelineLayout != nullptr &&
                     "VulkanWindow::createPipelineGraphics_Terrain")

            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Terrain;
            String nameShaderVert = "vert_standard_terrain_lit";
            String nameShaderFrag = "frag_standard_terrain_lit";
            if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                      "",
                                                      "",
                                                      "",
                                                      nameShaderFrag,
                                                      this->m_mapVkShaderModules_Internal,
                                                      aShaderStageCreateInfos_Terrain))
            {
                String msg = "*********************** VulkanWindow::createPipelineGraphics_Terrain: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            if (!this->m_pPipelineGraphics_Terrain->Init(descriptorSetLayout,
                                                         pDescriptorSetLayoutNames,
                                                         vkDescriptorSetLayout,
                                                         vkPipelineLayout,
                                                         aShaderStageCreateInfos_Terrain))
            {
                F_LogError("*********************** VulkanWindow::createPipelineGraphics_Terrain: PipelineGraphics_Terrain->Init failed !");
                return;
            }
            F_LogInfo("VulkanWindow::createPipelineGraphics_Terrain: [PipelineGraphics_Terrain] create success !");
        }
    void VulkanWindow::UpdateDescriptorSets_Graphics_Terrain()
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr)
            return;

        this->m_pPipelineGraphics_Terrain->UpdateDescriptorSets();
    }
    void VulkanWindow::UpdateBuffer_Graphics_Terrain()
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr)
            return;

        this->m_pPipelineGraphics_Terrain->UpdateBufferTerrain();
    }
    void VulkanWindow::Draw_Graphics_Terrain(VkCommandBuffer& commandBuffer)
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr ||
            this->m_pVKRenderPassTerrain == nullptr)
            return;

        if (!this->m_pVKRenderPassTerrain->poTerrainInstanceIsDraw)
        {
            Draw_Graphics_Terrain_Whole(commandBuffer);
        }
        else
        {
            Draw_Graphics_Terrain_Instance(commandBuffer);
        }   
    }
        void VulkanWindow::Draw_Graphics_Terrain_Whole(VkCommandBuffer& commandBuffer)
        {
            VkBuffer vertexBuffers[] = { this->m_pVKRenderPassTerrain->poTerrainVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            bindIndexBuffer(commandBuffer, this->m_pVKRenderPassTerrain->poTerrainIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            
            if (this->cfg_isWireFrame)
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_Terrain->poPipeline_WireFrame);
            else
                bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_Terrain->poPipeline);
            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->m_pPipelineGraphics_Terrain->poPipelineLayout, 0, 1, &this->m_pPipelineGraphics_Terrain->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);

            drawIndexed(commandBuffer, this->m_pVKRenderPassTerrain->poTerrainIndexCount, 1, 0, 0, 0);
        }
        void VulkanWindow::Draw_Graphics_Terrain_Instance(VkCommandBuffer& commandBuffer)
        {

        }

    float VulkanWindow::GetTerrainHeight(const FVector3& vPos)
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr)
            return vPos.y;

        return GetTerrainHeight(vPos.x, vPos.z);
    }
    float VulkanWindow::GetTerrainHeight(float x, float z)
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr)
            return 0.0f;

        return this->m_pPipelineGraphics_Terrain->GetTerrainHeight(x, z);
    }
    bool VulkanWindow::RaytraceTerrain(float screenX, float screenY, FVector3& vPos)
    {
        return RaytraceTerrain(screenX, screenY, this->pCamera, this->GetViewportVector4(), vPos);
    }
    bool VulkanWindow::RaytraceTerrain(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FVector3& vPos)
    {
        if (!this->cfg_isRenderPassTerrain ||
            this->m_pPipelineGraphics_Terrain == nullptr)
            return false;

        return this->m_pPipelineGraphics_Terrain->RaytraceTerrain(screenX, screenY, pCamera, vViewport, vPos);
    }


    /////////////////////////// VulkanWindow //////////////////////
    Mesh* VulkanWindow::CreateMesh(const MeshInfo* pMI)
    {
        Mesh* pMesh = new Mesh(0,
                               pMI->nameMesh,
                               pMI->pathMesh,
                               pMI->typeMesh,
                               pMI->typeVertex,
                               pMI->typeGeometryType,
                               pMI->pMeshCreateParam);
        if (!pMesh->LoadMesh(pMI->isFlipY, pMI->isTransformLocal, pMI->matTransformLocal))
        {
            String msg = "*********************** VulkanWindow::CreateMesh: create mesh: name: [" + pMI->nameMesh + "], path: [" + pMI->pathMesh + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        F_LogInfo("VulkanWindow::CreateMesh: create mesh, name: [%s], path: [%s] success !", 
                  pMI->nameMesh.c_str(), pMI->pathMesh.c_str());
        return pMesh;
    }
    void VulkanWindow::CreateMeshes(const MeshInfoPtrVector& aMIs, MeshPtrVector& aMeshes, MeshPtrMap& mapMeshes)
    {
        size_t count = aMIs.size();
        for (size_t i = 0; i < count; i++)
        {
            Mesh* pMesh = CreateMesh(aMIs[i]);
            if (pMesh != nullptr)
            {
                aMeshes.push_back(pMesh);
                mapMeshes[pMesh->GetName()] = pMesh;
            }
        }
    }

    VkShaderModule VulkanWindow::CreateShaderModule(const ShaderModuleInfo& si)
    { 
        VkShaderModule shaderModule = createVkShaderModule(si.nameShader, si.nameShaderType, si.pathShader);
        if (shaderModule == VK_NULL_HANDLE)
        {
            String msg = "*********************** VulkanWindow::CreateShaderModule: create shader: name: [" + si.nameShader + "], type: [" + si.nameShaderType + "], path: [" + si.pathShader + "] failed !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        
        F_LogInfo("VulkanWindow::CreateShaderModule: create shader: name: [%s], type: [%s], path: [%s] success !", 
                  si.nameShader.c_str(), si.nameShaderType.c_str(), si.pathShader.c_str());
        return shaderModule;
    }
    void VulkanWindow::CreateShaderModules(const ShaderModuleInfoVector& aSIs, VkShaderModuleVector& aShaderModules, VkShaderModuleMap& mapShaderModules)
    {
        size_t count = aSIs.size();
        for (size_t i = 0; i < count; i++)
        {
            const ShaderModuleInfo& si = aSIs[i];
            VkShaderModule shaderModule = CreateShaderModule(si);
            if (shaderModule != nullptr)
            {
                aShaderModules.push_back(shaderModule);
                mapShaderModules[si.nameShader] = shaderModule;
            }
        }
    }

    bool VulkanWindow::CreatePipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                            const String& nameShaderTesc,
                                                            const String& nameShaderTese,
                                                            const String& nameShaderGeom,
                                                            const String& nameShaderFrag,
                                                            const String& nameShaderComp,
                                                            VkShaderModuleMap& mapVkShaderModules,
                                                            VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics,
                                                            VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                            VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute)
    {
        if (!CreatePipelineShaderStageCreateInfos(nameShaderVert,
                                                  nameShaderTesc,
                                                  nameShaderTese,
                                                  nameShaderGeom,
                                                  nameShaderFrag,
                                                  mapVkShaderModules,
                                                  aStageCreateInfos_Graphics))
        {
            return false;
        }

        if (!CreatePipelineShaderStageCreateInfos(nameShaderComp,
                                                  mapVkShaderModules,
                                                  aStageCreateInfos_Compute,
                                                  mapStageCreateInfos_Compute))
        {
            return false;
        }

        return true;
    }
    bool VulkanWindow::CreatePipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                            const String& nameShaderTesc,
                                                            const String& nameShaderTese,
                                                            const String& nameShaderGeom,
                                                            const String& nameShaderFrag,
                                                            VkShaderModuleMap& mapVkShaderModules,
                                                            VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics)
    {
        //vert
        {
            VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameShaderVert);
            if (itFind == mapVkShaderModules.end())
            {
                F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find vert shader module: [%s] !", nameShaderVert.c_str());
                return false;
            }
            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
            shaderStageInfo.module = itFind->second;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Graphics.push_back(shaderStageInfo);
        }
        //tesc
        if (!nameShaderTesc.empty())
        {
            VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameShaderTesc);
            if (itFind == mapVkShaderModules.end())
            {
                F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find tesc shader module: [%s] !", nameShaderTesc.c_str());
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
            shaderStageInfo.module = itFind->second;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Graphics.push_back(shaderStageInfo);
        }
        //tese
        if (!nameShaderTese.empty())
        {
            VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameShaderTese);
            if (itFind == mapVkShaderModules.end())
            {
                F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find tese shader module: [%s] !", nameShaderTese.c_str());
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
            shaderStageInfo.module = itFind->second;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Graphics.push_back(shaderStageInfo);
        }
        //geom
        if (!nameShaderGeom.empty())
        {
            VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameShaderGeom);
            if (itFind == mapVkShaderModules.end())
            {
                F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find geom shader module: [%s] !", nameShaderGeom.c_str());
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
            shaderStageInfo.module = itFind->second;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Graphics.push_back(shaderStageInfo);
        }
        //frag
        if (!nameShaderFrag.empty())
        {
            VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameShaderFrag);
            if (itFind == mapVkShaderModules.end())
            {
                F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find frag shader module: [%s] !", nameShaderFrag.c_str());
                return false;
            }

            VkPipelineShaderStageCreateInfo shaderStageInfo = {};
            shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
            shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
            shaderStageInfo.module = itFind->second;
            shaderStageInfo.pName = "main";
            aStageCreateInfos_Graphics.push_back(shaderStageInfo);
        }

        return true;
    }
    bool VulkanWindow::CreatePipelineShaderStageCreateInfos(const String& nameShaderComp,
                                                            VkShaderModuleMap& mapVkShaderModules,
                                                            VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                            VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute)
    {
        //comp
        if (!nameShaderComp.empty())
        {
            StringVector aShaderComps = FUtilString::Split(nameShaderComp, ";");
            int count_comp = (int)aShaderComps.size();
            for (int i = 0; i < count_comp; i++)
            {
                String nameSC = aShaderComps[i];
                VkShaderModuleMap::iterator itFind = mapVkShaderModules.find(nameSC);
                if (itFind == mapVkShaderModules.end())
                {
                    F_LogError("*********************** VulkanWindow::CreatePipelineShaderStageCreateInfos: Can not find comp shader module: [%s] !", nameSC.c_str());
                    return false;
                }

                VkPipelineShaderStageCreateInfo shaderStageInfo = {};
                shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                shaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
                shaderStageInfo.module = itFind->second;
                shaderStageInfo.pName = "main";
                aStageCreateInfos_Compute.push_back(shaderStageInfo);
                mapStageCreateInfos_Compute[nameSC] = shaderStageInfo;
            }
        }

        return true;
    }
    
    VkDescriptorSetLayout VulkanWindow::CreateDescriptorSetLayout(const String& nameLayout, const StringVector* pNamesDescriptorSetLayout)
    {
        VkDescriptorSetLayout vkDescriptorSetLayout;
        VkDescriptorSetLayoutBindingVector bindings;
        uint32_t count_layout = (uint32_t)pNamesDescriptorSetLayout->size();
        for (uint32_t i = 0; i < count_layout; i++)
        {
            const String& strLayout = (*pNamesDescriptorSetLayout)[i];
            if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object)) //Object
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_GEOMETRY_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectTerrain)) //ObjectTerrain
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectGrid)) //ObjectGrid
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCameraAxis)) //ObjectCameraAxis
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCoordinateAxis)) //ObjectCoordinateAxis
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat2D)) //ObjectLineFlat2D
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectLineFlat3D)) //ObjectLineFlat3D
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCopyBlit)) //ObjectCopyBlit
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCull)) //ObjectCull
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material)) //Material
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance)) //Instance
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCopy)) //TextureCopy
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Tessellation)) //Tessellation
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT | VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Geometry)) //Geometry
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_GEOMETRY_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureVS)) //TextureVS
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1, VK_SHADER_STAGE_VERTEX_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESC)) //TextureTESC
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureTESE)) //TextureTESE
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS)) //TextureFS
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameColor)) //TextureFrameColor
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFrameDepth)) //TextureFrameDepth
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSR)) //TextureCSR
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRW)) //TextureCSRW
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWSrc)) //TextureCSRWSrc
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureCSRWDst)) //TextureCSRWDst
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1, VK_SHADER_STAGE_COMPUTE_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureDepthShadow)) //TextureDepthShadow
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Image(i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VK_SHADER_STAGE_FRAGMENT_BIT, nullptr));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachRed)) //InputAttachRed
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachGreen)) //InputAttachGreen
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_InputAttachBlue)) //InputAttachBlue
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1, VK_SHADER_STAGE_FRAGMENT_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Terrain)) //Terrain
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Cull)) //Cull
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_HizDepth)) //HizDepth
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWArgsCB)) //BufferRWArgsCB
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWLodCB)) //BufferRWLodCB
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWResultCB)) //BufferRWResultCB
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWClipCB)) //BufferRWClipCB
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferRWObjectCullInstance)) //BufferRWObjectCullInstance
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_CullInstance)) //CullInstance
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferObjectLineFlat2D)) //BufferObjectLineFlat2D
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else if (strLayout == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_BufferObjectLineFlat3D)) //BufferObjectLineFlat3D
            {
                bindings.push_back(createVkDescriptorSetLayoutBinding_Buffer(i, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_COMPUTE_BIT));
            }
            else
            {
                String msg = "*********************** VulkanWindow::CreateDescriptorSetLayout: Wrong DescriptorSetLayout type: " + strLayout;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
        }

        if (!createVkDescriptorSetLayout(nameLayout, bindings, vkDescriptorSetLayout))
        {
            String msg = "*********************** VulkanWindow::CreateDescriptorSetLayout: Failed to create descriptor set layout: " + nameLayout;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        F_LogInfo("VulkanWindow::CreateDescriptorSetLayout: Success to create descriptor set layout: [%s] !", nameLayout.c_str());
        return vkDescriptorSetLayout;
    }
   

    /////////////////////////// VulkanWindow //////////////////////
    VulkanWindow::VulkanWindow(int width, int height, String name)
        : VulkanBase(width, height, name)
        , poInstance(VK_NULL_HANDLE)
        , poDebugMessenger(VK_NULL_HANDLE)
        , poPhysicalDevice(VK_NULL_HANDLE)
        , poDeviceCreatepNextChain(VK_NULL_HANDLE)
        , poDevice(VK_NULL_HANDLE)
        , poDebug(nullptr)
        , poMSAASamples(VK_SAMPLE_COUNT_1_BIT)
        , poQueueGraphics(VK_NULL_HANDLE)
        , poQueuePresent(VK_NULL_HANDLE)
        , poQueueCompute(VK_NULL_HANDLE)
        , poSurface(VK_NULL_HANDLE)
        , poSwapChain(VK_NULL_HANDLE)
        , poColorImage(VK_NULL_HANDLE)
        , poColorImageMemory(VK_NULL_HANDLE)
        , poColorImageView(VK_NULL_HANDLE)
        , poDepthImage(VK_NULL_HANDLE)
        , poDepthImageMemory(VK_NULL_HANDLE)
        , poDepthImageView(VK_NULL_HANDLE)
        , poDepthImageView_Depth(VK_NULL_HANDLE)
        , poDepthImageView_Stencil(VK_NULL_HANDLE)
        , poRenderPass(VK_NULL_HANDLE)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poCommandPoolGraphics(VK_NULL_HANDLE) 
        , poCommandPoolCompute(VK_NULL_HANDLE)
        , poCommandBufferComputeBefore(VK_NULL_HANDLE)
        , poCommandBufferComputeAfter(VK_NULL_HANDLE)

        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)
        , poVertexBuffer(VK_NULL_HANDLE)
        , poVertexBufferMemory(VK_NULL_HANDLE)
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)
        , poIndexBuffer(VK_NULL_HANDLE)
        , poIndexBufferMemory(VK_NULL_HANDLE)
        , poMatWorld(FMath::Identity4x4())

        , poTypeVertex(F_MeshVertex_Pos3Color4Normal3Tangent3Tex2)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipelineCache(VK_NULL_HANDLE)
        , poPipelineGraphics(VK_NULL_HANDLE)
        , poPipelineGraphics_WireFrame(VK_NULL_HANDLE)

        , poTextureImage(VK_NULL_HANDLE)
        , poTextureImageMemory(VK_NULL_HANDLE)
        , poTextureImageView(VK_NULL_HANDLE)
        , poTextureSampler(VK_NULL_HANDLE)

        , poDescriptorPool(VK_NULL_HANDLE)

        , poCurrentFrame(0)
        , poSwapChainImageIndex(0)
        , poGraphicsWaitComputeBeforeSemaphore(VK_NULL_HANDLE)
        , poGraphicsSignalComputeAfterSemaphore(VK_NULL_HANDLE)
        , poComputeBeforeWaitSemaphore(VK_NULL_HANDLE)
        , poComputeAfterWaitSemaphore(VK_NULL_HANDLE)

        , queueIndexGraphics(0)
        , queueIndexPresent(0)
        , queueIndexCompute(0)

        , isFrameBufferResized(false)
        , isComputeCullFrustum(false)
        , isComputeCullFrustumHizDepth(false)

        , cfg_colorBackground(0.0f, 0.2f, 0.4f, 1.0f)
        
        , cfg_isRenderPassShadowMap(false)
        , cfg_isRenderPassCull(false)
        , cfg_isRenderPassTerrain(false)
        , cfg_isRenderPassDefaultCustom(false)

        , cfg_isMSAA(false)
        , cfg_isImgui(false)
        , cfg_isWireFrame(false)
        , cfg_isRotate(false)
        , cfg_isNegativeViewport(true)
        , cfg_isUseCopyBlitFromFrameColor(false)
        , cfg_isUseCopyBlitFromFrameDepth(false)
        , cfg_isUseFramebuffer_Depth(false)
        , cfg_isUseFramebuffer_Stencil(false)
        , cfg_isUseComputeShaderBeforeRender(false)
        , cfg_isUseComputeShaderAfterRender(false)
        , cfg_isCreateRenderComputeSycSemaphore(false)
        , cfg_vkPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        , cfg_vkFrontFace(VK_FRONT_FACE_CLOCKWISE)
        , cfg_vkPolygonMode(VK_POLYGON_MODE_FILL)
        , cfg_vkCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
        , cfg_isDepthBiasEnable(VK_FALSE)
        , cfg_DepthBiasConstantFactor(0.0f)
        , cfg_DepthBiasClamp(0.0f)
        , cfg_DepthBiasSlopeFactor(0.0f)
        , cfg_LineWidth(1.0f)
        , cfg_isDepthTest(VK_TRUE)
        , cfg_isDepthWrite(VK_TRUE)
        , cfg_DepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL) 
        , cfg_isStencilTest(VK_FALSE)
        , cfg_isBlend(VK_FALSE)
        , cfg_BlendColorFactorSrc(VK_BLEND_FACTOR_ONE)
        , cfg_BlendColorFactorDst(VK_BLEND_FACTOR_ZERO)
        , cfg_BlendColorOp(VK_BLEND_OP_ADD)
        , cfg_BlendAlphaFactorSrc(VK_BLEND_FACTOR_ONE)
        , cfg_BlendAlphaFactorDst(VK_BLEND_FACTOR_ZERO)
        , cfg_BlendAlphaOp(VK_BLEND_OP_ADD)
        , cfg_ColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)

        , cfg_cameraPos(0.0f, 0.0f, -5.0f)
        , cfg_cameraLookTarget(0.0f, 0.0f, 0.0f)
        , cfg_cameraUp(0.0f, 1.0f, 0.0f)
        , cfg_cameraFov(45.0f)
        , cfg_cameraNear(0.05f)
        , cfg_cameraFar(1000.0f)
        , cfg_cameraSpeedMove(1000.0f)
        , cfg_cameraSpeedZoom(0.1f)
        , cfg_cameraSpeedRotate(0.1f)

        , cfg_model_Path("")
        , cfg_shaderVertex_Path("")
        , cfg_shaderFragment_Path("")
        , cfg_texture_Path("")
        , cfg_terrain_Path("")
        , cfg_terrainTextureDiffuse_Path("Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png")
        , cfg_terrainTextureNormal_Path("Assets/Texture/Terrain/shore_sand_norm.png;Assets/Texture/Terrain/moss_norm.tga;Assets/Texture/Terrain/rock_cliff_norm.tga;Assets/Texture/Terrain/cliff_norm.png")
        , cfg_terrainTextureControl_Path("Assets/Texture/Terrain/terrain_control.png")
        , cfg_terrainHeightStart(0.0f)
        , cfg_terrainHeightMax(200.0f)
        
        , imgui_IsEnable(false)
        , imgui_MinimalSwapchainImages(0)
        , imgui_DescriptorPool(VK_NULL_HANDLE)
        , imgui_PathIni("")
        , imgui_PathLog("")

        //Internal
        , m_pVKRenderPassShadowMap(nullptr)
        , m_pVKRenderPassCull(nullptr)
        , m_pVKRenderPassTerrain(nullptr)

        , m_pPipelineCompute_Cull(nullptr)
        , m_pPipelineCompute_Terrain(nullptr)

        , m_pPipelineGraphics_CopyBlitFromFrameColor(nullptr)
        , m_pPipelineGraphics_CopyBlitFromFrameDepth(nullptr)
        , m_pPipelineGraphics_CopyBlitToFrame(nullptr)
        , m_pPipelineGraphics_DepthShadowMap(nullptr)
        , m_pPipelineGraphics_DepthHiz(nullptr)
        , m_pPipelineGraphics_Terrain(nullptr)

        , pCamera(nullptr)
        , pCameraRight(nullptr)
        , pCameraMainLight(new FCamera)

        , mouseButtonDownLeft(false)
        , mouseButtonDownRight(false)
        , mouseButtonDownMiddle(false)

        , cfg_isEditorCreate(false)
        , cfg_isEditorGridShow(false)
        , cfg_isEditorCameraAxisShow(false)
        , cfg_isEditorCoordinateAxisShow(false)
        , cfg_editorGrid_Color(0.5f, 0.5f, 0.5f, 0.5f)
        , cfg_editorCoordinateAxis_MoveSpeed(0.01f)
        , cfg_editorCoordinateAxis_RotateSpeed(0.01f)
        , cfg_editorCoordinateAxis_ScaleSpeed(0.01f)
        , pEditorGrid(nullptr)
        , pEditorCameraAxis(nullptr)
        , pEditorCoordinateAxis(nullptr)
        , pEditorLineFlat2DCollector(nullptr)
        , pEditorLineFlat3DCollector(nullptr)
    {
        cfg_aDynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
        cfg_aDynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

        cfg_StencilOpFront.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        cfg_StencilOpFront.compareMask = 0;
        cfg_StencilOpFront.writeMask = 0;
        cfg_StencilOpFront.reference = 0;

        cfg_StencilOpBack.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        cfg_StencilOpBack.compareMask = 0;
        cfg_StencilOpBack.writeMask = 0;
        cfg_StencilOpBack.reference = 0;

        Base::ms_pWindow = this;
    }

    VulkanWindow::~VulkanWindow()
    {
        F_DELETE(pCamera)
        F_DELETE(pCameraRight)
        F_DELETE(pCameraMainLight)
    }

    void VulkanWindow::OnInit()
    {
        createPipeline();
    }

    void VulkanWindow::OnLoad()
    {
        loadAssets();
    }

    bool VulkanWindow::OnIsInit()
    {
        return this->isCreateDevice &&
            this->isLoadAsset;
    }

    void VulkanWindow::OnResize(int w, int h, bool force)
    {
        resizeWindow(w, h, force);

        if (this->pCamera != nullptr)
        {
            this->pCamera->PerspectiveLH(glm::radians(this->cfg_cameraFov), this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
        }
    }

    bool VulkanWindow::OnBeginCompute_BeforeRender()
    {
        return beginCompute_BeforeRender();
    }
        void VulkanWindow::OnUpdateCompute_BeforeRender()
        {
            updateCompute_BeforeRender();
        }
        void VulkanWindow::OnCompute_BeforeRender()
        {
            compute_BeforeRender();
        }
    void VulkanWindow::OnEndCompute_BeforeRender()
    {
        endCompute_BeforeRender();
    }

    bool VulkanWindow::OnBeginRender()
    {
        return beginRender();
    }
        void VulkanWindow::OnUpdateRender()
        {
            updateRender();
        }
        void VulkanWindow::OnRender()
        {
            render();
        }
    void VulkanWindow::OnEndRender()
    {
        endRender();
    }

    bool VulkanWindow::OnBeginCompute_AfterRender()
    {
        return beginCompute_AfterRender();
    }
        void VulkanWindow::OnUpdateCompute_AfterRender()
        {
            updateCompute_AfterRender();
        }
        void VulkanWindow::OnCompute_AfterRender()
        {
            compute_AfterRender();
        }
    void VulkanWindow::OnEndCompute_AfterRender()
    {
        endCompute_AfterRender();
    }

    void VulkanWindow::OnPresent()
    {
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        VkSemaphoreVector aSemaphores;
        aSemaphores.push_back(this->poRenderCompleteSemaphores[this->poCurrentFrame]);
        if (this->poComputeAfterWaitSemaphore != VK_NULL_HANDLE)
            aSemaphores.push_back(this->poComputeAfterWaitSemaphore);
        presentInfo.waitSemaphoreCount = (uint32_t)aSemaphores.size();
        presentInfo.pWaitSemaphores = aSemaphores.data();
        VkSwapchainKHR swapChains[] = { this->poSwapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &this->poSwapChainImageIndex;
        VkResult result = vkQueuePresentKHR(this->poQueuePresent, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->isFrameBufferResized) 
        {
            this->isFrameBufferResized = false;
            recreateSwapChain();
        } 
        else if (result != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::OnPresent: Failed to present swap chain image !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        this->poCurrentFrame = (this->poCurrentFrame + 1) % s_maxFramesInFight;
    }
    void VulkanWindow::OnDestroy()
    {
        //1> Wait
        if (this->isCreateDevice)
        {
            vkDeviceWaitIdle(this->poDevice);

             //2> Cleanup
            cleanup();
        }
    }

    void VulkanWindow::OnMouseInput()
    {   
        double cursorX; double cursorY;
        glfwGetCursorPos(this->pWindow, &cursorX, &cursorY);
        cursorX *= this->poWindowContentScale.x;
        cursorY *= this->poWindowContentScale.y;

        //Mouse Left
        int actionLeft = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_LEFT);
        if (actionLeft == GLFW_PRESS)
        {
            if (!this->mouseButtonDownLeft)
            {
                OnMouseLeftDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_LEFT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->mouseButtonDownLeft)
            {
                OnMouseLeftUp(cursorX, cursorY);
            }
        }
        
        //Mouse Right
        int actionRight = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_RIGHT);
        if (actionRight == GLFW_PRESS)
        {
            if (!this->mouseButtonDownRight)
            {
                OnMouseRightDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_RIGHT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->mouseButtonDownRight)
            {
                OnMouseRightUp(cursorX, cursorY);
            }
        }

        //Mouse Middle
        int actionMiddle = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_MIDDLE);
        if (actionMiddle == GLFW_PRESS)
        {
            if (!this->mouseButtonDownMiddle)
            {
                OnMouseMiddleDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_MIDDLE, cursorX, cursorY);
            }
        }
        else
        {
            if (this->mouseButtonDownMiddle)
            {
                OnMouseMiddleUp(cursorX, cursorY);
            }
        }

        //Mouse Hover
        if (!this->mouseButtonDownLeft &&
            !this->mouseButtonDownRight &&
            !this->mouseButtonDownMiddle)
        {
            OnMouseHover(cursorX, cursorY);
        }

        this->mousePosLast.x = (float)cursorX;
        this->mousePosLast.y = (float)cursorY;
    }
    void VulkanWindow::OnMouseLeftDown(double x, double y)
    {
        this->mouseButtonDownLeft = true;
        {
            OnEditorCoordinateMouseLeftDown(x, y);
        }
    }
    void VulkanWindow::OnMouseLeftUp(double x, double y)
    {   
        this->mouseButtonDownLeft = false;

        OnEditorCoordinateMouseLeftUp(x, y);
    }
    void VulkanWindow::OnMouseRightDown(double x, double y)
    {
        this->mouseButtonDownRight = true;
        {

        }
    }
    void VulkanWindow::OnMouseRightUp(double x, double y)
    {
        this->mouseButtonDownRight = false;

    }
    void VulkanWindow::OnMouseMiddleDown(double x, double y)
    {
        this->mouseButtonDownMiddle = true;
        {

        }
    }
    void VulkanWindow::OnMouseMiddleUp(double x, double y)
    {
        this->mouseButtonDownMiddle = false;

    }   
    void VulkanWindow::OnMouseMove(int button, double x, double y)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            OnEditorCoordinateMouseMove(x, y);

        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            OnCameraMouseMoveProcess(x, y, this->mousePosLast.x, this->mousePosLast.y);
            
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            
        }
    }
    void VulkanWindow::OnMouseHover(double x, double y)
    {
        OnEditorCoordinateMouseHover(x, y);

    }
    void VulkanWindow::OnMouseWheel(double x, double y)
    {
        OnCameraMouseZoomProcess(y);

    }

    void VulkanWindow::OnKeyboardInput()
    {
        OnCameraMouseKeyboardProcess();

    }
    void VulkanWindow::OnKeyDown(int key)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(this->pWindow, true);
        }
        if (key == GLFW_KEY_R)
        {
            cameraReset();
        }
        if (key == GLFW_KEY_T)
        {
            this->cfg_isRotate = !this->cfg_isRotate;
        }
        if (key == GLFW_KEY_F)
        {
            this->cfg_isWireFrame = !this->cfg_isWireFrame;
        }
    }
    void VulkanWindow::OnKeyUp(int key)
    {

    }

    void VulkanWindow::OnCameraMouseMoveProcess(double newX, double newY, double oldX, double oldY)
    {
        if (this->pCamera != nullptr)
        {
            float fX = static_cast<float>(newX - oldX);
            float fY = static_cast<float>(newY - oldY);
            if (fX != 0 || fY != 0)
            {
                float fRotYAngle = fX * this->cfg_cameraSpeedRotate;
                float fRotXAngle = fY * this->cfg_cameraSpeedRotate;
                FVector3 vEulerAngles = pCamera->GetEulerAngles();
                vEulerAngles.x += fRotXAngle;
                vEulerAngles.y += fRotYAngle;
                vEulerAngles.z = 0;
                pCamera->SetEulerAngles(vEulerAngles);
            }
        }
    }
    void VulkanWindow::OnCameraMouseZoomProcess(double zoom) 
    {
        if (this->pCamera != nullptr)
        {
            float fDis = (float)(this->cfg_cameraSpeedZoom * zoom);
            this->pCamera->Walk(fDis);
        }
    }
    void VulkanWindow::OnCameraMouseKeyboardProcess()
    {
        if (this->pCamera != nullptr)
        {
            float speedMove = this->cfg_cameraSpeedMove;
            if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                glfwGetKey(this->pWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) 
            {
                speedMove *= 10.0f;
            }

            float timeDelta = this->pTimer->GetTimeDelta();
            if (glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS)
            {
                this->pCamera->Walk(speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_S) == GLFW_PRESS)
            {
                this->pCamera->Walk(- speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS)
            {
                this->pCamera->Strafe(- speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS)
            {
                this->pCamera->Strafe(speedMove * timeDelta);
            }
            this->pCamera->UpdateViewMatrix();
        }
    }

    void VulkanWindow::OnEditorCoordinateMouseLeftDown(double x, double y)
    {
        if (this->pEditorCoordinateAxis != nullptr)
        {
            this->pEditorCoordinateAxis->MouseLeftDown(x, y);
        }
    }
    void VulkanWindow::OnEditorCoordinateMouseMove(double x, double y)
    {
        if (this->pEditorCoordinateAxis != nullptr)
        {
            this->pEditorCoordinateAxis->MouseMove(x, y);
        }
    }
    void VulkanWindow::OnEditorCoordinateMouseLeftUp(double x, double y)
    {
        if (this->pEditorCoordinateAxis != nullptr)
        {
            this->pEditorCoordinateAxis->MouseLeftUp(x, y);
        }
    }
    void VulkanWindow::OnEditorCoordinateMouseHover(double x, double y)
    {
        if (this->pEditorCoordinateAxis != nullptr)
        {
            this->pEditorCoordinateAxis->MouseHover(x, y);
        }
    }   

    bool VulkanWindow::HasConfig_RenderPassDefaultCustom()
    {
        return this->cfg_isRenderPassDefaultCustom;
    }
    bool VulkanWindow::HasConfig_MASS()
    {
        return this->cfg_isMSAA;
    }
    bool VulkanWindow::HasConfig_Imgui()
    {
        return this->cfg_isImgui;
    }

    bool VulkanWindow::IsEnable_MASS()
    {
        return this->cfg_isMSAA &&
               this->poMSAASamples != VK_SAMPLE_COUNT_1_BIT;
    }
    bool VulkanWindow::IsEnable_Imgui()
    {
        return this->cfg_isImgui &&
               this->imgui_IsEnable;
    }

    void VulkanWindow::createPipeline()
    {   
        F_LogInfo("**********<1> VulkanWindow::createPipeline start **********");
        {
            //1> Create Resize callback
            createWindowCallback();

            //2> Create Device
            createDevice();

            //3> Create Feature Support
            createFeatureSupport();

            //4> Create Command Objects
            createCommandObjects();

            //5> Create Swap Chain Objects
            createSwapChainObjects();

            //6> createDescriptorPool
            createDescriptorPool();

            //7> createVkPipelineCache
            createVkPipelineCache();

            //8> Camera/Light/Shadow/Terrain
            createCamera();
            createLightMain();
            createShadowLightMain();
            createTerrain();

            //9> Create Pipeline Objects
            createPipelineObjects();

            //10> Create Sync Objects
            createSyncObjects();

            //11> createInternal/createResourceInternal
            createInternal();
            createResourceInternal();

            //12> createDescriptorSetLayouts
            createDescriptorSetLayouts();

            //13> isCreateDevice
            this->isCreateDevice = true;
        }
        F_LogInfo("**********<1> VulkanWindow::createPipeline finish **********");
    }
    //glfw: whenever the window size changed (by OS or user resize) this callback function executes
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        VulkanWindow* pWnd = (VulkanWindow*)glfwGetWindowUserPointer(window);
        pWnd->isFrameBufferResized = true;
        pWnd->OnResize(width, height, false);
    }
    void VulkanWindow::createWindowCallback()
    {   
        glfwSetWindowUserPointer(this->pWindow, this);
        glfwSetFramebufferSizeCallback(this->pWindow, framebuffer_size_callback);

        F_LogInfo("*****<1-1> VulkanWindow::createWindowCallback finish *****");
    }
    void VulkanWindow::createDevice()
    {
        F_LogInfo("*****<1-2> VulkanWindow::createDevice start *****");
        {
            //1> createInstance
            createInstance();

            //2> setUpDebugMessenger
            setUpDebugMessenger();

            //3> createSurface
            createSurface();

            //4> pickPhysicalDevice
            pickPhysicalDevice();

            //5> createLogicalDevice
            createLogicalDevice();
        }
        F_LogInfo("*****<1-2> VulkanWindow::createDevice finish *****");
    }
    void VulkanWindow::destroyVkDevice(VkDevice vkDevice)
    {
        if (vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(vkDevice, nullptr);
        }
    }
    void VulkanWindow::destroyVkInstance(VkInstance vkInstance)
    {
        if (vkInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(vkInstance, nullptr);
        }
    }
    void VulkanWindow::createInstance()
    {
        getInstanceLayersAndExtensions(s_isEnableValidationLayers, 
                                       this->aInstanceLayers, 
                                       this->aInstanceExtensions);
        if (s_isEnableValidationLayers) 
        {
            this->aInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        ConstCharPtrVector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        size_t count_extensions = this->aInstanceExtensions.size();
        size_t count_glfw = extensions.size();
        for (size_t i = 0; i < count_glfw; i++)
        {
            bool isFind = false;
            for (size_t j = 0; j < count_extensions; j++)
            {   
                if (strcmp(extensions[i], this->aInstanceExtensions[j]) == 0)
                {
                    isFind = true;
                    break;
                }
            }

            if (!isFind)
            {
                this->aInstanceExtensions.push_back(extensions[i]);
            }
        }


        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "vulkan_xxx_sample";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "LostPeterVulkan";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
#if F_PLATFORM == F_PLATFORM_MAC       
        createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(this->aInstanceExtensions.size());
        createInfo.ppEnabledExtensionNames = this->aInstanceExtensions.size() > 0 ? this->aInstanceExtensions.data() : nullptr;
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->aInstanceLayers.size());
        createInfo.ppEnabledLayerNames = this->aInstanceLayers.size() > 0 ? this->aInstanceLayers.data() : nullptr;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (s_isEnableValidationLayers) 
        {
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } 

        VkResult result = vkCreateInstance(&createInfo, nullptr, &this->poInstance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER) 
        {
            F_LogError("*********************** VulkanWindow::createInstance: Cannot find a compatible Vulkan driver (ICD)");
        }
        else if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            String missingExtensions;
            uint32 propertyCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
            std::vector<VkExtensionProperties> properties(propertyCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data());

            for (const char* extension : this->aInstanceExtensions)
            {
                bool found = false;
                for (uint32 i = 0; i < propertyCount; ++i)
                {
                    const char* propExtension = properties[i].extensionName;
                    if (strcmp(propExtension, extension) == 0) 
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) 
                {
                    String extensionStr(extension);
                    missingExtensions += extensionStr + "\n";
                }
            }

            F_LogError("*********************** VulkanWindow::createInstance: Vulkan driver doesn't contain specified extensions: [%s] !", missingExtensions.c_str());
        }
        else if (result != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanWindow::createInstance: Create vulkan instance failed !");
        }
        else 
        {
            F_LogInfo("VulkanWindow::createInstance: Create vulkan instance successed !");
        }

        if (result != VK_SUCCESS)
        {
            String msg = "*********************** VulkanWindow::createInstance: Failed to create vulkan instance !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        F_LogInfo("<1-2-1> VulkanWindow::createInstance finish !");
    }
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
        {
            String prefix("");

			if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) 
            {
				prefix = "VERBOSE: ";
			}
			else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) 
            {
				prefix = "INFO: ";
			}
			else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) 
            {
				prefix = "WARNING: ";
			}
			else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) 
            {
				prefix = "ERROR: ";
			}

            F_LogInfo("VulkanWindow.debugCallback: Validation layer: [%s] [%d] - [%s]: [%s]!", prefix.c_str(), pCallbackData->messageIdNumber, pCallbackData->pMessageIdName, pCallbackData->pMessage);
            return VK_FALSE;
        }
        void VulkanWindow::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
        {
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

    void VulkanWindow::setUpDebugMessenger()
    {
        if (s_isEnableValidationLayers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            populateDebugMessengerCreateInfo(createInfo);
                
            if (createDebugUtilsMessengerEXT(this->poInstance, &createInfo, nullptr, &this->poDebugMessenger) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::setUpDebugMessenger: Failed to set up debug messenger !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        
        F_LogInfo("<1-1-2> VulkanWindow::setUpDebugMessenger finish !");
    }

    void VulkanWindow::createSurface()
    {
        VkResult result = glfwCreateWindowSurface(this->poInstance, this->pWindow, nullptr, &this->poSurface);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            String msg = "*********************** VulkanWindow::createSurface: Failed to create window surface, result: " + os.str();
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        F_LogInfo("<1-2-3> VulkanWindow::createSurface finish !");
    }

    void VulkanWindow::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            String msg = "*********************** VulkanWindow::pickPhysicalDevice: Failed to find GPUs width Vulkan support !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, devices.data());

        int count_device = (int)devices.size();
        for (int i = 0; i < count_device; i++)
        {
            VkPhysicalDevice& device = devices[i];
            int indexGraphics = -1;
            int indexPresent = -1;
            int indexCompute = -1;
            if (isDeviceSuitable(device, indexGraphics, indexPresent, indexCompute))
            {
                this->poPhysicalDevice = device;
                this->queueIndexGraphics = indexGraphics;
                this->queueIndexPresent = indexPresent;
                this->queueIndexCompute = indexCompute;

                if (HasConfig_MASS())
                {
                    this->poMSAASamples = getMaxUsableSampleCount();
                }
                break;
            }
        } 

        if (this->poPhysicalDevice == VK_NULL_HANDLE) 
        {
            String msg = "*********************** VulkanWindow::pickPhysicalDevice: Failed to find a suitable GPU !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        vkGetPhysicalDeviceProperties(this->poPhysicalDevice, &this->poPhysicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(this->poPhysicalDevice, &this->poPhysicalDeviceFeatures);

        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceProperties ****************");
        {
            F_LogInfo("  apiVersion: [%u]", this->poPhysicalDeviceProperties.apiVersion);
            F_LogInfo("  driverVersion: [%u]", this->poPhysicalDeviceProperties.driverVersion);
            F_LogInfo("  vendorID: [%u]", this->poPhysicalDeviceProperties.vendorID);
            F_LogInfo("  deviceID: [%u]", this->poPhysicalDeviceProperties.deviceID);
            if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER)
                F_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_OTHER");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                F_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                F_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
                F_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
                F_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_CPU");
            F_LogInfo("  deviceName: [%s]", this->poPhysicalDeviceProperties.deviceName);

            F_LogInfo("  maxImageDimension1D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension1D);
            F_LogInfo("  maxImageDimension2D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension2D);
            F_LogInfo("  maxImageDimension3D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension3D);
            F_LogInfo("  maxImageDimensionCube: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimensionCube);
            F_LogInfo("  maxImageArrayLayers: [%u]", this->poPhysicalDeviceProperties.limits.maxImageArrayLayers);
            F_LogInfo("  maxTexelBufferElements: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelBufferElements);
            F_LogInfo("  maxUniformBufferRange: [%u]", this->poPhysicalDeviceProperties.limits.maxUniformBufferRange);
            F_LogInfo("  maxStorageBufferRange: [%u]", this->poPhysicalDeviceProperties.limits.maxStorageBufferRange);
            F_LogInfo("  maxPushConstantsSize: [%u]", this->poPhysicalDeviceProperties.limits.maxPushConstantsSize);
            F_LogInfo("  maxMemoryAllocationCount: [%u]", this->poPhysicalDeviceProperties.limits.maxMemoryAllocationCount);
            F_LogInfo("  maxSamplerAllocationCount: [%u]", this->poPhysicalDeviceProperties.limits.maxSamplerAllocationCount);
            F_LogInfo("  bufferImageGranularity: [%u]", this->poPhysicalDeviceProperties.limits.bufferImageGranularity);
            F_LogInfo("  sparseAddressSpaceSize: [%u]", this->poPhysicalDeviceProperties.limits.sparseAddressSpaceSize);
            F_LogInfo("  maxBoundDescriptorSets: [%u]", this->poPhysicalDeviceProperties.limits.maxBoundDescriptorSets);
            F_LogInfo("  maxPerStageDescriptorSamplers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorSamplers);
            F_LogInfo("  maxPerStageDescriptorUniformBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorUniformBuffers);
            F_LogInfo("  maxPerStageDescriptorStorageBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageBuffers);
            F_LogInfo("  maxPerStageDescriptorSampledImages: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorSampledImages);
            F_LogInfo("  maxPerStageDescriptorStorageImages: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageImages);
            F_LogInfo("  maxPerStageDescriptorInputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorInputAttachments);
            F_LogInfo("  maxPerStageResources: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageResources);
            F_LogInfo("  maxDescriptorSetSamplers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetSamplers);
            F_LogInfo("  maxDescriptorSetUniformBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetUniformBuffers);
            F_LogInfo("  maxDescriptorSetUniformBuffersDynamic: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetUniformBuffersDynamic);
            F_LogInfo("  maxDescriptorSetStorageBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageBuffers);
            F_LogInfo("  maxDescriptorSetStorageBuffersDynamic: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageBuffersDynamic);
            F_LogInfo("  maxDescriptorSetSampledImages: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetSampledImages);
            F_LogInfo("  maxDescriptorSetStorageImages: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageImages);
            F_LogInfo("  maxDescriptorSetInputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetInputAttachments);
            F_LogInfo("  maxVertexInputAttributes: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputAttributes);
            F_LogInfo("  maxVertexInputBindings: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputBindings);
            F_LogInfo("  maxVertexInputAttributeOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputAttributeOffset);
            F_LogInfo("  maxVertexInputBindingStride: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputBindingStride);
            F_LogInfo("  maxVertexOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexOutputComponents);
            F_LogInfo("  maxTessellationGenerationLevel: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationGenerationLevel);
            F_LogInfo("  maxTessellationPatchSize: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationPatchSize);
            F_LogInfo("  maxTessellationControlPerVertexInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerVertexInputComponents);
            F_LogInfo("  maxTessellationControlPerVertexOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerVertexOutputComponents);
            F_LogInfo("  maxTessellationControlPerPatchOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerPatchOutputComponents);
            F_LogInfo("  maxTessellationControlTotalOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlTotalOutputComponents);
            F_LogInfo("  maxTessellationEvaluationInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationEvaluationInputComponents);
            F_LogInfo("  maxTessellationEvaluationOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationEvaluationOutputComponents);
            F_LogInfo("  maxGeometryShaderInvocations: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryShaderInvocations);
            F_LogInfo("  maxGeometryInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryInputComponents);
            F_LogInfo("  maxGeometryOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryOutputComponents);
            F_LogInfo("  maxGeometryOutputVertices: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryOutputVertices);
            F_LogInfo("  maxGeometryTotalOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryTotalOutputComponents);
            F_LogInfo("  maxFragmentInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentInputComponents);
            F_LogInfo("  maxFragmentOutputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentOutputAttachments);
            F_LogInfo("  maxFragmentDualSrcAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentDualSrcAttachments);
            F_LogInfo("  maxFragmentCombinedOutputResources: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentCombinedOutputResources);
            F_LogInfo("  maxComputeSharedMemorySize: [%u]", this->poPhysicalDeviceProperties.limits.maxComputeSharedMemorySize);
            F_LogInfo("  maxComputeWorkGroupCount: [%u] - [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[0], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[1], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[2]);
            F_LogInfo("  maxComputeWorkGroupInvocations: [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupInvocations);
            F_LogInfo("  maxComputeWorkGroupSize: [%u] - [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[0], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[1], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[2]);
            F_LogInfo("  subPixelPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.subPixelPrecisionBits);
            F_LogInfo("  subTexelPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.subTexelPrecisionBits);
            F_LogInfo("  mipmapPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.mipmapPrecisionBits);
            F_LogInfo("  maxDrawIndexedIndexValue: [%u]", this->poPhysicalDeviceProperties.limits.maxDrawIndexedIndexValue);
            F_LogInfo("  maxDrawIndirectCount: [%u]", this->poPhysicalDeviceProperties.limits.maxDrawIndirectCount);
            F_LogInfo("  maxSamplerLodBias: [%f]", this->poPhysicalDeviceProperties.limits.maxSamplerLodBias);
            F_LogInfo("  maxSamplerAnisotropy: [%f]", this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy);
            F_LogInfo("  maxViewports: [%u]", this->poPhysicalDeviceProperties.limits.maxViewports);
            F_LogInfo("  maxViewportDimensions: [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxViewportDimensions[0], this->poPhysicalDeviceProperties.limits.maxViewportDimensions[1]);
            F_LogInfo("  viewportBoundsRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.viewportBoundsRange[0], this->poPhysicalDeviceProperties.limits.viewportBoundsRange[1]);
            F_LogInfo("  viewportSubPixelBits: [%u]", this->poPhysicalDeviceProperties.limits.viewportSubPixelBits);
            F_LogInfo("  minMemoryMapAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minMemoryMapAlignment);
            F_LogInfo("  minTexelBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minTexelBufferOffsetAlignment);
            F_LogInfo("  minUniformBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
            F_LogInfo("  minStorageBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minStorageBufferOffsetAlignment);
            F_LogInfo("  minTexelOffset: [%d]", this->poPhysicalDeviceProperties.limits.minTexelOffset);
            F_LogInfo("  maxTexelOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelOffset);
            F_LogInfo("  minTexelGatherOffset: [%d]", this->poPhysicalDeviceProperties.limits.minTexelGatherOffset);
            F_LogInfo("  maxTexelGatherOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelGatherOffset);
            F_LogInfo("  minInterpolationOffset: [%f]", this->poPhysicalDeviceProperties.limits.minInterpolationOffset);
            F_LogInfo("  maxInterpolationOffset: [%f]", this->poPhysicalDeviceProperties.limits.maxInterpolationOffset);
            F_LogInfo("  subPixelInterpolationOffsetBits: [%u]", this->poPhysicalDeviceProperties.limits.subPixelInterpolationOffsetBits);
            F_LogInfo("  maxFramebufferWidth: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferWidth);
            F_LogInfo("  maxFramebufferHeight: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferHeight);
            F_LogInfo("  maxFramebufferLayers: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferLayers);
            F_LogInfo("  framebufferColorSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferColorSampleCounts);
            F_LogInfo("  framebufferDepthSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferDepthSampleCounts);
            F_LogInfo("  framebufferStencilSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferStencilSampleCounts);
            F_LogInfo("  framebufferNoAttachmentsSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferNoAttachmentsSampleCounts);
            F_LogInfo("  maxColorAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxColorAttachments);
            F_LogInfo("  sampledImageColorSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageColorSampleCounts);
            F_LogInfo("  sampledImageIntegerSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageIntegerSampleCounts);
            F_LogInfo("  sampledImageDepthSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageDepthSampleCounts);
            F_LogInfo("  sampledImageStencilSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageStencilSampleCounts);
            F_LogInfo("  storageImageSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.storageImageSampleCounts);
            F_LogInfo("  maxSampleMaskWords: [%u]", this->poPhysicalDeviceProperties.limits.maxSampleMaskWords);
            F_LogInfo("  timestampComputeAndGraphics: [%s]", this->poPhysicalDeviceProperties.limits.timestampComputeAndGraphics ? "true" : "false");
            F_LogInfo("  timestampPeriod: [%f]", this->poPhysicalDeviceProperties.limits.timestampPeriod);
            F_LogInfo("  maxClipDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxClipDistances);
            F_LogInfo("  maxCullDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxCullDistances);
            F_LogInfo("  maxCombinedClipAndCullDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxCombinedClipAndCullDistances);
            F_LogInfo("  discreteQueuePriorities: [%u]", this->poPhysicalDeviceProperties.limits.discreteQueuePriorities);
            F_LogInfo("  pointSizeRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.pointSizeRange[0], this->poPhysicalDeviceProperties.limits.pointSizeRange[1]);
            F_LogInfo("  lineWidthRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.lineWidthRange[0], this->poPhysicalDeviceProperties.limits.lineWidthRange[1]);
            F_LogInfo("  pointSizeGranularity: [%f]", this->poPhysicalDeviceProperties.limits.pointSizeGranularity);
            F_LogInfo("  lineWidthGranularity: [%f]", this->poPhysicalDeviceProperties.limits.lineWidthGranularity);
            F_LogInfo("  strictLines: [%s]", this->poPhysicalDeviceProperties.limits.strictLines ? "true" : "false");
            F_LogInfo("  standardSampleLocations: [%s]", this->poPhysicalDeviceProperties.limits.standardSampleLocations ? "true" : "false");
            F_LogInfo("  optimalBufferCopyOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.optimalBufferCopyOffsetAlignment);
            F_LogInfo("  optimalBufferCopyRowPitchAlignment: [%u]", this->poPhysicalDeviceProperties.limits.optimalBufferCopyRowPitchAlignment);
            F_LogInfo("  nonCoherentAtomSize: [%u]", this->poPhysicalDeviceProperties.limits.nonCoherentAtomSize);
        }
        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceProperties ****************");

        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceFeatures ****************");
        {
            F_LogInfo("  robustBufferAccess: [%s]", this->poPhysicalDeviceFeatures.robustBufferAccess ? "true" : "false");
            F_LogInfo("  fullDrawIndexUint32: [%s]", this->poPhysicalDeviceFeatures.fullDrawIndexUint32 ? "true" : "false");
            F_LogInfo("  imageCubeArray: [%s]", this->poPhysicalDeviceFeatures.imageCubeArray ? "true" : "false");
            F_LogInfo("  independentBlend: [%s]", this->poPhysicalDeviceFeatures.independentBlend ? "true" : "false");
            F_LogInfo("  geometryShader: [%s]", this->poPhysicalDeviceFeatures.geometryShader ? "true" : "false");
            F_LogInfo("  tessellationShader: [%s]", this->poPhysicalDeviceFeatures.tessellationShader ? "true" : "false");
            F_LogInfo("  sampleRateShading: [%s]", this->poPhysicalDeviceFeatures.sampleRateShading ? "true" : "false");
            F_LogInfo("  dualSrcBlend: [%s]", this->poPhysicalDeviceFeatures.dualSrcBlend ? "true" : "false");
            F_LogInfo("  logicOp: [%s]", this->poPhysicalDeviceFeatures.logicOp ? "true" : "false");
            F_LogInfo("  multiDrawIndirect: [%s]", this->poPhysicalDeviceFeatures.multiDrawIndirect ? "true" : "false");
            F_LogInfo("  drawIndirectFirstInstance: [%s]", this->poPhysicalDeviceFeatures.drawIndirectFirstInstance ? "true" : "false");
            F_LogInfo("  depthClamp: [%s]", this->poPhysicalDeviceFeatures.depthClamp ? "true" : "false");
            F_LogInfo("  depthBiasClamp: [%s]", this->poPhysicalDeviceFeatures.depthBiasClamp ? "true" : "false");
            F_LogInfo("  fillModeNonSolid: [%s]", this->poPhysicalDeviceFeatures.fillModeNonSolid ? "true" : "false");
            F_LogInfo("  depthBounds: [%s]", this->poPhysicalDeviceFeatures.depthBounds ? "true" : "false");
            F_LogInfo("  wideLines: [%s]", this->poPhysicalDeviceFeatures.wideLines ? "true" : "false");
            F_LogInfo("  largePoints: [%s]", this->poPhysicalDeviceFeatures.largePoints ? "true" : "false");
            F_LogInfo("  alphaToOne: [%s]", this->poPhysicalDeviceFeatures.alphaToOne ? "true" : "false");
            F_LogInfo("  multiViewport: [%s]", this->poPhysicalDeviceFeatures.multiViewport ? "true" : "false");
            F_LogInfo("  samplerAnisotropy: [%s]", this->poPhysicalDeviceFeatures.samplerAnisotropy ? "true" : "false");
            F_LogInfo("  textureCompressionETC2: [%s]", this->poPhysicalDeviceFeatures.textureCompressionETC2 ? "true" : "false");
            F_LogInfo("  textureCompressionASTC_LDR: [%s]", this->poPhysicalDeviceFeatures.textureCompressionASTC_LDR ? "true" : "false");
            F_LogInfo("  textureCompressionBC: [%s]", this->poPhysicalDeviceFeatures.textureCompressionBC ? "true" : "false");
            F_LogInfo("  occlusionQueryPrecise: [%s]", this->poPhysicalDeviceFeatures.occlusionQueryPrecise ? "true" : "false");
            F_LogInfo("  pipelineStatisticsQuery: [%s]", this->poPhysicalDeviceFeatures.pipelineStatisticsQuery ? "true" : "false");
            F_LogInfo("  vertexPipelineStoresAndAtomics: [%s]", this->poPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics ? "true" : "false");
            F_LogInfo("  fragmentStoresAndAtomics: [%s]", this->poPhysicalDeviceFeatures.fragmentStoresAndAtomics ? "true" : "false");
            F_LogInfo("  shaderTessellationAndGeometryPointSize: [%s]", this->poPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize ? "true" : "false");
            F_LogInfo("  shaderImageGatherExtended: [%s]", this->poPhysicalDeviceFeatures.shaderImageGatherExtended ? "true" : "false");
            F_LogInfo("  shaderStorageImageExtendedFormats: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageExtendedFormats ? "true" : "false");
            F_LogInfo("  shaderStorageImageMultisample: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageMultisample ? "true" : "false");
            F_LogInfo("  shaderStorageImageReadWithoutFormat: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat ? "true" : "false");
            F_LogInfo("  shaderStorageImageWriteWithoutFormat: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat ? "true" : "false");
            F_LogInfo("  shaderUniformBufferArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing ? "true" : "false");
            F_LogInfo("  shaderSampledImageArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing ? "true" : "false");
            F_LogInfo("  shaderStorageBufferArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing ? "true" : "false");
            F_LogInfo("  shaderStorageImageArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing ? "true" : "false");
            F_LogInfo("  shaderClipDistance: [%s]", this->poPhysicalDeviceFeatures.shaderClipDistance ? "true" : "false");
            F_LogInfo("  shaderCullDistance: [%s]", this->poPhysicalDeviceFeatures.shaderCullDistance ? "true" : "false");
            F_LogInfo("  shaderFloat64: [%s]", this->poPhysicalDeviceFeatures.shaderFloat64 ? "true" : "false");
            F_LogInfo("  shaderInt64: [%s]", this->poPhysicalDeviceFeatures.shaderInt64 ? "true" : "false");
            F_LogInfo("  shaderInt16: [%s]", this->poPhysicalDeviceFeatures.shaderInt16 ? "true" : "false");
            F_LogInfo("  shaderResourceResidency: [%s]", this->poPhysicalDeviceFeatures.shaderResourceResidency ? "true" : "false");
            F_LogInfo("  shaderResourceMinLod: [%s]", this->poPhysicalDeviceFeatures.shaderResourceMinLod ? "true" : "false");
            F_LogInfo("  sparseBinding: [%s]", this->poPhysicalDeviceFeatures.sparseBinding ? "true" : "false");
            F_LogInfo("  sparseResidencyBuffer: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyBuffer ? "true" : "false");
            F_LogInfo("  sparseResidencyImage2D: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyImage2D ? "true" : "false");
            F_LogInfo("  sparseResidencyImage3D: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyImage3D ? "true" : "false");
            F_LogInfo("  sparseResidency2Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency2Samples ? "true" : "false");
            F_LogInfo("  sparseResidency4Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency4Samples ? "true" : "false");
            F_LogInfo("  sparseResidency8Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency8Samples ? "true" : "false");
            F_LogInfo("  sparseResidency16Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency16Samples ? "true" : "false");
            F_LogInfo("  sparseResidencyAliased: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyAliased ? "true" : "false");
            F_LogInfo("  variableMultisampleRate: [%s]", this->poPhysicalDeviceFeatures.variableMultisampleRate ? "true" : "false");
            F_LogInfo("  inheritedQueries: [%s]", this->poPhysicalDeviceFeatures.inheritedQueries ? "true" : "false");
        }       
        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceFeatures ****************");

        this->poPhysicalDeviceMultiViewFeaturesKHR = {};
        this->poPhysicalDeviceMultiViewFeaturesKHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR;
        this->poPhysicalDeviceFeatures2KHR = {};
        this->poPhysicalDeviceFeatures2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
		this->poPhysicalDeviceFeatures2KHR.pNext = &this->poPhysicalDeviceMultiViewFeaturesKHR;
        PFN_vkGetPhysicalDeviceFeatures2KHR pFuncGetPhysicalDeviceFeatures2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures2KHR>(vkGetInstanceProcAddr(this->poInstance, "vkGetPhysicalDeviceFeatures2KHR"));
        if (pFuncGetPhysicalDeviceFeatures2KHR != nullptr)
        {
            pFuncGetPhysicalDeviceFeatures2KHR(this->poPhysicalDevice, &this->poPhysicalDeviceFeatures2KHR);
        }

        this->poPhysicalDeviceMultiViewPropertiesKHR = {};
        this->poPhysicalDeviceMultiViewPropertiesKHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR;
        this->poPhysicalDeviceProperties2KHR = {};
		this->poPhysicalDeviceProperties2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
		this->poPhysicalDeviceProperties2KHR.pNext = &this->poPhysicalDeviceMultiViewPropertiesKHR;
        PFN_vkGetPhysicalDeviceProperties2KHR pFuncGetPhysicalDeviceProperties2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2KHR>(vkGetInstanceProcAddr(this->poInstance, "vkGetPhysicalDeviceProperties2KHR"));
        if (pFuncGetPhysicalDeviceProperties2KHR != nullptr)
        {
            pFuncGetPhysicalDeviceProperties2KHR(this->poPhysicalDevice, &this->poPhysicalDeviceProperties2KHR);
        }

        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: MultiViewFeatures ****************");
        {
            if (pFuncGetPhysicalDeviceFeatures2KHR != nullptr)
            {
                if (this->poPhysicalDeviceMultiViewFeaturesKHR.multiview)
                {
                    this->poDeviceCreatepNextChain = &this->poPhysicalDeviceMultiViewFeaturesKHR;
                }

                F_LogInfo("  multiview: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiview ? "true" : "false");
                F_LogInfo("  multiviewGeometryShader: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiviewGeometryShader ? "true" : "false");
                F_LogInfo("  multiviewTessellationShader: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiviewTessellationShader ? "true" : "false");
            }
            if (pFuncGetPhysicalDeviceProperties2KHR != nullptr)
            {   
                F_LogInfo("  maxMultiviewViewCount: [%u]", this->poPhysicalDeviceMultiViewPropertiesKHR.maxMultiviewViewCount);
                F_LogInfo("  maxMultiviewInstanceIndex: [%u]", this->poPhysicalDeviceMultiViewPropertiesKHR.maxMultiviewInstanceIndex);
            }
        }
        F_LogInfo("**************** VulkanWindow::pickPhysicalDevice: MultiViewFeatures ****************");

        F_LogInfo("<1-2-4> VulkanWindow::pickPhysicalDevice finish !");
    }
        void VulkanWindow::findQueueFamilies(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute)
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int count_device = (int)queueFamilies.size();
            for (int i = 0; i < count_device; i++)
            {
                VkQueueFamilyProperties& queueFamily = queueFamilies[i];

                //Graphics
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
                {
                    indexGraphics = i;
                }

                //Present
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->poSurface, &presentSupport);
                if (presentSupport)
                {
                    indexPresent = i;
                }

                //Compute
                if (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender)
                {
                    if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
                    {
                        indexCompute = i;
                    }

                    if (indexGraphics >= 0 && indexPresent >= 0 && indexCompute >= 0)
                        break;
                }
                else
                {
                    if (indexGraphics >= 0 && indexPresent >= 0)
                        break;
                }
            }
        }
        VkSampleCountFlagBits VulkanWindow::getMaxUsableSampleCount()
        {
            VkSampleCountFlags counts = this->poPhysicalDeviceProperties.limits.framebufferColorSampleCounts & this->poPhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
            if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
            if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
            if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
            if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
            if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
            if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

            return VK_SAMPLE_COUNT_1_BIT;
        }
        VulkanWindow::SwapChainSupportDetails VulkanWindow::querySwapChainSupport(VkPhysicalDevice device, VulkanWindow::SwapChainSupportDetails& details)
        {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->poSurface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->poSurface, &formatCount, nullptr);

            if (formatCount != 0)
            {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->poSurface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->poSurface, &presentModeCount, nullptr);

            if (presentModeCount != 0)
            {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->poSurface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
        bool VulkanWindow::isDeviceSuitable(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute)
        {   
            findQueueFamilies(device, indexGraphics, indexPresent, indexCompute);
            if (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender)
            {
                if (indexGraphics == -1 || indexPresent == -1 || indexCompute == -1)
                    return false;
            }
            else
            {
                if (indexGraphics == -1 || indexPresent == -1)
                    return false;
            }
            
            bool swapChainAdequate = false;
            querySwapChainSupport(device, this->swapChainSupport);
            swapChainAdequate = !this->swapChainSupport.formats.empty() && !this->swapChainSupport.presentModes.empty();
            return swapChainAdequate;
        }

    void VulkanWindow::setUpEnabledFeatures()
    {
        this->poPhysicalEnabledFeatures = {};

        if (this->poPhysicalDeviceFeatures.samplerAnisotropy)
            this->poPhysicalEnabledFeatures.samplerAnisotropy = VK_TRUE;
        if (this->poPhysicalDeviceFeatures.fillModeNonSolid)
            this->poPhysicalEnabledFeatures.fillModeNonSolid = VK_TRUE;

        if (this->poPhysicalDeviceFeatures.geometryShader)
            this->poPhysicalEnabledFeatures.geometryShader = VK_TRUE;
        if (this->poPhysicalDeviceFeatures.tessellationShader)
            this->poPhysicalEnabledFeatures.tessellationShader = VK_TRUE;
        if (this->poPhysicalDeviceFeatures.multiDrawIndirect)
            this->poPhysicalEnabledFeatures.multiDrawIndirect = VK_TRUE;

        if (this->poPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics)
            this->poPhysicalEnabledFeatures.vertexPipelineStoresAndAtomics = VK_TRUE;

    }
    void VulkanWindow::createLogicalDevice()
    {
        getDeviceLayersAndExtensions(s_isEnableValidationLayers,
                                     this->aDeviceLayers,
                                     this->aDeviceExtensions);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.insert(this->queueIndexGraphics);
        uniqueQueueFamilies.insert(this->queueIndexPresent);
        if (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender)
            uniqueQueueFamilies.insert(this->queueIndexCompute);

        float queuePriority = 1.0f;
        for (std::set<uint32_t>::iterator it = uniqueQueueFamilies.begin(); 
             it != uniqueQueueFamilies.end(); ++it)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = *it;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        setUpEnabledFeatures();
    
        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        if (s_isEnableValidationLayers) 
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(this->aDeviceLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = this->aDeviceLayers.size() > 0 ? this->aDeviceLayers.data() : nullptr;
        } 
        else 
        {
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
        }
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(this->aDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = this->aDeviceExtensions.size() > 0 ? this->aDeviceExtensions.data() : nullptr;
        if (this->poDeviceCreatepNextChain == nullptr)
        {
            deviceCreateInfo.pEnabledFeatures = &this->poPhysicalEnabledFeatures;
        }   
        else
        {
            this->poPhysicalDeviceFeatures2 = {};
            this->poPhysicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			this->poPhysicalDeviceFeatures2.features = this->poPhysicalEnabledFeatures;
			this->poPhysicalDeviceFeatures2.pNext = this->poDeviceCreatepNextChain;
			deviceCreateInfo.pEnabledFeatures = nullptr;
			deviceCreateInfo.pNext = &this->poPhysicalDeviceFeatures2;
        }

        if (vkCreateDevice(this->poPhysicalDevice, &deviceCreateInfo, nullptr, &this->poDevice) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createLogicalDevice: Failed to create logical device !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->poDebug = new VKDebug();
        this->poDebug->Init(this->poDevice);

        //Queue-Graphics
        vkGetDeviceQueue(this->poDevice, this->queueIndexGraphics, 0, &this->poQueueGraphics);
        this->poDebug->SetVkQueueName(this->poDevice, this->poQueueGraphics, "Queue-Graphics");
        //Queue-Present
        vkGetDeviceQueue(this->poDevice, this->queueIndexPresent, 0, &this->poQueuePresent);
        if (this->queueIndexGraphics != this->queueIndexPresent)
        {
            this->poDebug->SetVkQueueName(this->poDevice, this->poQueuePresent, "Queue-Present");
        }
        //Queue-Compute
        if (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender)
        {
            vkGetDeviceQueue(this->poDevice, this->queueIndexCompute, 0, &this->poQueueCompute);
            if (this->queueIndexGraphics != this->queueIndexCompute)
            {
                this->poDebug->SetVkQueueName(this->poDevice, this->poQueueCompute, "Queue-Compute");
            }
        }
        
        F_LogInfo("<1-2-5> VulkanWindow::createLogicalDevice finish !");
    }

    void VulkanWindow::createFeatureSupport()
    {

    }
    void VulkanWindow::createCamera()
    {
        if (this->pCamera == nullptr)
        {
            this->pCamera = new FCamera();
        }
    }
    void VulkanWindow::createLightMain()
    {

    }
    void VulkanWindow::createShadowLightMain()
    {

    }
    void VulkanWindow::createTerrain()
    {
        
    }


    void VulkanWindow::createCommandObjects()
    {
        F_LogInfo("*****<1-4> VulkanWindow::createCommandObjects start *****");
        {
            //1> createCommandPool
            createCommandPool();

        }
        F_LogInfo("*****<1-4> VulkanWindow::createCommandObjects finish *****");
    }
    void VulkanWindow::createCommandPool()
    {
        //1> poCommandPoolGraphics
        createCommandPool_Graphics();
        F_LogInfo("<1-4-1> VulkanWindow::createCommandPool: Create CommandPoolGraphics success !");

        //2> poCommandPoolCompute
        if (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender)
        {
            createCommandPool_Compute();
            F_LogInfo("<1-4-2> VulkanWindow::createCommandPool: Create CommandPoolCompute success !");
        }

        F_LogInfo("<1-4> VulkanWindow::createCommandPool finish, create CommandPoolGraphics: [true], create CommandPoolCompute: [%s]", (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender) ? "true" : "false");
    }
        void VulkanWindow::createCommandPool_Graphics()
        {
            VkCommandPoolCreateInfo poolGraphicsInfo = {};
            poolGraphicsInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolGraphicsInfo.queueFamilyIndex = this->queueIndexGraphics;
            poolGraphicsInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            if (vkCreateCommandPool(this->poDevice, &poolGraphicsInfo, nullptr, &this->poCommandPoolGraphics) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandPool_Graphics: Failed to create command pool graphics !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkCommandPoolName(this->poDevice, this->poCommandPoolGraphics, "CommandPool-Graphics");
        }
        void VulkanWindow::createCommandPool_Compute()
        {
            VkCommandPoolCreateInfo poolComputeInfo = {};
            poolComputeInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolComputeInfo.queueFamilyIndex = this->queueIndexCompute;
            poolComputeInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            if (vkCreateCommandPool(this->poDevice, &poolComputeInfo, nullptr, &this->poCommandPoolCompute) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandPool_Compute: Failed to create command pool compute !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkCommandPoolName(this->poDevice, this->poCommandPoolCompute, "CommandPool-Compute");
        }

            void VulkanWindow::destroyVkCommandPool(VkCommandPool vkCommandPool)
            {
                if (vkCommandPool != VK_NULL_HANDLE)
                {
                    vkDestroyCommandPool(this->poDevice, vkCommandPool, nullptr);
                }
            }
            void VulkanWindow::freeCommandBuffers(VkCommandPool commandPool,  uint32_t count, VkCommandBuffer* pCommandBuffer)
            {
                if (pCommandBuffer != nullptr)
                {
                    vkFreeCommandBuffers(this->poDevice, commandPool, count, pCommandBuffer);
                }
            }

        VkCommandBuffer VulkanWindow::beginSingleTimeCommands() 
        {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = this->poCommandPoolGraphics;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
            vkAllocateCommandBuffers(this->poDevice, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            return commandBuffer;
        }
        void VulkanWindow::endSingleTimeCommands(VkCommandBuffer commandBuffer) 
        {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, nullptr);
            vkQueueWaitIdle(this->poQueueGraphics);

            freeCommandBuffers(this->poCommandPoolGraphics, 1, &commandBuffer);
        }

    void VulkanWindow::createSwapChainObjects()
    {
        F_LogInfo("*****<1-5> VulkanWindow::createSwapChainObjects start *****");
        {
            //1> createSwapChain
            createSwapChain();

            //2> createSwapChainImageViews
            createSwapChainImageViews();

            //3> createColorResources
            if (HasConfig_MASS())
            {
                createColorResources();
            }

            //4> createDepthResources
            createDepthResources();

            //5> createColorResourceLists
            createColorResourceLists();
        }
        F_LogInfo("*****<1-5> VulkanWindow::createSwapChainObjects finish *****");
    }
    void VulkanWindow::createSwapChain()
    {
        querySwapChainSupport(this->poPhysicalDevice, this->swapChainSupport);
        this->imgui_MinimalSwapchainImages = this->swapChainSupport.capabilities.minImageCount + 1;

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(this->swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(this->swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(this->swapChainSupport.capabilities);

        uint32_t imageCount = this->swapChainSupport.capabilities.minImageCount + 1;
        if (this->swapChainSupport.capabilities.maxImageCount > 0 && imageCount > this->swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = this->swapChainSupport.capabilities.maxImageCount;
        }
        F_LogInfo("**************** image count: [%d]", (int)imageCount);

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->poSurface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] =
        {
            this->queueIndexGraphics,
            this->queueIndexPresent
        };

        if (this->queueIndexGraphics != this->queueIndexPresent)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = this->swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        VkResult result = vkCreateSwapchainKHR(this->poDevice, &createInfo, nullptr, &this->poSwapChain);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            String msg = "*********************** VulkanWindow::createSwapChain: Failed to create swap chain, result: " + os.str();
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, nullptr);
        this->poSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, this->poSwapChainImages.data());

        this->poSwapChainImageFormat = surfaceFormat.format;
        this->poSwapChainExtent = extent;
        this->poDepthImageFormat = findDepthFormat();

        int width, height;
        glfwGetFramebufferSize(this->pWindow, &width, &height);
        this->poFramebufferSize.x = (float)width;
        this->poFramebufferSize.y = (float)height;
        float scaleX, scaleY;
        glfwGetWindowContentScale(this->pWindow, &scaleX, &scaleY);
        this->poWindowContentScale.x = scaleX;
        this->poWindowContentScale.y = scaleY;
        F_LogInfo("<1-5-1> VulkanWindow::createSwapChain finish, Swapchain size: [%d,%d], window size: [%d,%d], scale: [%f, %f], format color: [%u], format depth: [%u] !", 
                  (int)extent.width, (int)extent.height, (int)width, (int)height, scaleX, scaleY, (uint32_t)this->poSwapChainImageFormat, (uint32_t)this->poDepthImageFormat);

        createViewport();
    }
        VkSurfaceFormatKHR VulkanWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
        {
            int count = (int)availableFormats.size();
            for (int i = 0; i < count; i++)
            {
                const VkSurfaceFormatKHR& availableFormat = availableFormats[i];
                if ((availableFormat.format == VK_FORMAT_R8G8B8A8_SRGB || availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB) && 
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
                {
                    return availableFormat;
                }
            }
            return availableFormats[0];
        }
        VkPresentModeKHR VulkanWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
        {
            int count = (int)availablePresentModes.size();
            for (int i = 0; i < count; i++)
            {
                const VkPresentModeKHR& availablePresentMode = availablePresentModes[i];
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
                {
                    return availablePresentMode;
                }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }
        VkExtent2D VulkanWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) 
        {
            int width, height;
            glfwGetFramebufferSize(this->pWindow, &width, &height);

            VkExtent2D actualExtent = 
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = FMath::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = FMath::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
        void VulkanWindow::createViewport()
        {
            this->poViewport = {};
            this->poViewport.x = 0.0f;
            this->poViewport.y = 0.0f;
            this->poViewport.width = (float)this->poSwapChainExtent.width;
            this->poViewport.height = (float)this->poSwapChainExtent.height;
            this->poViewport.minDepth = 0.0f;
            this->poViewport.maxDepth = 1.0f;

            this->poScissor = {};
            this->poOffset.x = 0;
            this->poOffset.y = 0;
            this->poExtent = this->poSwapChainExtent;
            this->poScissor.offset = this->poOffset;
            this->poScissor.extent = this->poExtent;
        }
        void VulkanWindow::createViewport(uint32_t width,
                                          uint32_t height,
                                          VkViewport& poViewport, 
                                          VkRect2D& poScissor)
        {
            poViewport  = {};
            poViewport.x = 0.0f;
            poViewport.y = 0.0f;
            poViewport.width = (float)width;
            poViewport.height = (float)height;
            poViewport.minDepth = 0.0f;
            poViewport.maxDepth = 1.0f;

            poScissor = {};
            VkOffset2D offset;
            offset.x = 0;
            offset.y = 0;
            VkExtent2D extent;
            extent.width = width;
            extent.height = height;
            poScissor.offset = offset;
            poScissor.extent = extent;
        }

    void VulkanWindow::createSwapChainImageViews()
    {
        int count = (int)this->poSwapChainImages.size();
        this->poSwapChainImageViews.resize(count);

        for (int i = 0; i < count; i++)
        {
            String nameTexture = "Texture-SwapChain-" + FUtilString::SaveInt(i);
            createVkImageView(nameTexture,
                              this->poSwapChainImages[i], 
                              VK_IMAGE_VIEW_TYPE_2D,
                              this->poSwapChainImageFormat, 
                              VK_IMAGE_ASPECT_COLOR_BIT, 
                              1,
                              1,
                              this->poSwapChainImageViews[i]);
        }

        F_LogInfo("<1-5-2> VulkanWindow::createSwapChainImageViews finish !");
    }
        void VulkanWindow::createColorResources()
        {
            VkFormat colorFormat = this->poSwapChainImageFormat;

            String nameTexture = "Texture-Color";
            createVkImage(nameTexture,
                          this->poSwapChainExtent.width, 
                          this->poSwapChainExtent.height, 
                          1,
                          1,
                          1,
                          VK_IMAGE_TYPE_2D, 
                          false,
                          this->poMSAASamples, 
                          colorFormat, 
                          VK_IMAGE_TILING_OPTIMAL, 
                          VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                          VK_SHARING_MODE_EXCLUSIVE,
                          false,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                          this->poColorImage, 
                          this->poColorImageMemory);

            createVkImageView(nameTexture + "-View",
                              this->poColorImage, 
                              VK_IMAGE_VIEW_TYPE_2D,
                              colorFormat, 
                              VK_IMAGE_ASPECT_COLOR_BIT, 
                              1, 
                              1,
                              this->poColorImageView);

            F_LogInfo("<1-5-3> VulkanWindow::createColorResources finish !");
        }
        void VulkanWindow::createDepthResources()
        {
            VkFormat depthFormat = this->poDepthImageFormat;
            
            String nameTexture = "Texture-Depth";
            createVkImage(nameTexture,
                          this->poSwapChainExtent.width, 
                          this->poSwapChainExtent.height, 
                          1, 
                          1,
                          1,
                          VK_IMAGE_TYPE_2D, 
                          false,
                          this->poMSAASamples, 
                          depthFormat, 
                          VK_IMAGE_TILING_OPTIMAL, 
                          VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT,
                          VK_SHARING_MODE_EXCLUSIVE,
                          false,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                          this->poDepthImage, 
                          this->poDepthImageMemory);

            createVkImageView(nameTexture + "-View",
                              this->poDepthImage, 
                              VK_IMAGE_VIEW_TYPE_2D,
                              depthFormat, 
                              VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT, 
                              1,
                              1,
                              this->poDepthImageView);
            if (this->cfg_isUseFramebuffer_Depth)
            {
                createVkImageView(nameTexture + "-View-Depth",
                                  this->poDepthImage, 
                                  VK_IMAGE_VIEW_TYPE_2D,
                                  depthFormat, 
                                  VK_IMAGE_ASPECT_DEPTH_BIT, 
                                  1,
                                  1,
                                  this->poDepthImageView_Depth);
            }
            if (this->cfg_isUseFramebuffer_Stencil)
            {
                createVkImageView(nameTexture + "-View-Stencil",
                              this->poDepthImage, 
                              VK_IMAGE_VIEW_TYPE_2D,
                              depthFormat, 
                              VK_IMAGE_ASPECT_STENCIL_BIT, 
                              1,
                              1,
                              this->poDepthImageView_Stencil);
            }

            F_LogInfo("<1-5-4> VulkanWindow::createDepthResources finish !");
        }
        void VulkanWindow::createColorResourceLists()
        {

        }
        VkFormat VulkanWindow::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) 
        {
            size_t count = candidates.size();
            for (int i = 0; i < count; i++) 
            {
                VkFormat format = candidates[i];
                VkFormatProperties props;
                vkGetPhysicalDeviceFormatProperties(this->poPhysicalDevice, format, &props);

                if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
                {
                    return format;
                } 
                else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
                {
                    return format;
                }
            }

            String msg = "*********************** VulkanWindow::findSupportedFormat: Failed to find supported format !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        VkFormat VulkanWindow::findDepthFormat() 
        {
            std::vector<VkFormat> candidates;
            candidates.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
            candidates.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
            candidates.push_back(VK_FORMAT_D16_UNORM_S8_UINT);
            return findSupportedFormat(candidates,
                                       VK_IMAGE_TILING_OPTIMAL,
                                       VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        }
        bool VulkanWindow::hasStencilComponent(VkFormat format)
        {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || 
                format == VK_FORMAT_D24_UNORM_S8_UINT ||
                format == VK_FORMAT_D16_UNORM_S8_UINT;
        }

        void VulkanWindow::destroyVkSurfaceKHR(VkSurfaceKHR vkSurfaceKHR)
        {
            if (vkSurfaceKHR != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(this->poInstance, vkSurfaceKHR, nullptr);
            }
        }
        void VulkanWindow::destroyVkSwapchainKHR(VkSwapchainKHR vkSwapchainKHR)
        {
            if (vkSwapchainKHR != VK_NULL_HANDLE)
            {
                vkDestroySwapchainKHR(this->poDevice, vkSwapchainKHR, nullptr);
            }
        }

    void VulkanWindow::createDescriptorPool()
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * F_ARRAYSIZE(pool_sizes);
        poolInfo.poolSizeCount = (uint32_t)F_ARRAYSIZE(pool_sizes);
        poolInfo.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(this->poDevice, &poolInfo, nullptr, &this->poDescriptorPool) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createDescriptorPool: Failed to create descriptor pool !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        F_LogInfo("<1-6> VulkanWindow::createDescriptorPool finish !");
    }
        void VulkanWindow::destroyVkDescriptorPool(VkDescriptorPool vkDescriptorPool)
        {
            if (vkDescriptorPool != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorPool(this->poDevice, vkDescriptorPool, nullptr);
            }
        }

    void VulkanWindow::createDescriptorSetLayouts()
    {
        F_LogInfo("*****<1-12> VulkanWindow::createDescriptorSetLayouts start *****");
        {
            //1> createDescriptorSetLayout_Default
            createDescriptorSetLayout_Default();
            F_LogInfo("<1-12-1> VulkanWindow::createDescriptorSetLayouts: createDescriptorSetLayout_Default finish !");

            //3> createDescriptorSetLayout_Custom
            createDescriptorSetLayout_Custom();
            F_LogInfo("<1-12-2> VulkanWindow::createDescriptorSetLayouts: createDescriptorSetLayout_Custom finish !");
        }
        F_LogInfo("*****<1-12> VulkanWindow::createDescriptorSetLayouts finish *****");
    }
    void VulkanWindow::createDescriptorSetLayout_Default()
    {   
        if (this->cfg_shaderVertex_Path.empty() ||
            this->cfg_shaderFragment_Path.empty())
        {
            return;
        }

        StringVector aNameDescriptorSets;
        aNameDescriptorSets.push_back(Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass));
        aNameDescriptorSets.push_back(Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Object));
        aNameDescriptorSets.push_back(Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Material));
        aNameDescriptorSets.push_back(Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Instance));
        if (!this->cfg_texture_Path.empty())
        {
            aNameDescriptorSets.push_back(Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_TextureFS));
        }

        this->poDescriptorSetLayout = CreateDescriptorSetLayout("DescriptorSetLayout_Default", &aNameDescriptorSets);
        if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
        {
            String msg = "*********************** VulkanWindow::createDescriptorSetLayout_Default: Failed to create descriptor set layout !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanWindow::createDescriptorSetLayout_Custom()
    {
        
    }

    void VulkanWindow::createPipelineObjects()
    {
        F_LogInfo("*****<1-9> VulkanWindow::createPipelineObjects start *****");
        {
            //1> createRenderPasses
            createRenderPasses();
            F_LogInfo("<1-9-1> VulkanWindow::createPipelineObjects: Success to create RenderPasses !");

            //2> createFramebuffers
            createFramebuffers();
            F_LogInfo("<1-9-2> VulkanWindow::createPipelineObjects: Success to create Framebuffers !");
        }
        F_LogInfo("*****<1-9> VulkanWindow::createPipelineObjects finish *****");
    }
    void VulkanWindow::createRenderPasses()
    {
        createRenderPass_ShadowMap();
        createRenderPass_Default();
        createRenderPass_Cull();
        createRenderPass_Terrain();
        createRenderPass_Custom();
    }
        void VulkanWindow::createRenderPass_ShadowMap()
        {
            if (m_pVKRenderPassShadowMap == nullptr)
            {
                m_pVKRenderPassShadowMap = new VKRenderPassShadowMap("RenderPass-ShadowMap");
            }
            m_pVKRenderPassShadowMap->Init(this->shadowMainLight.depthSize,
                                           this->shadowMainLight.depthSize,
                                           this->shadowMainLight.format);
        }
        void VulkanWindow::createRenderPass_Default()
        {
            if (HasConfig_RenderPassDefaultCustom())
            {
                createRenderPass_DefaultCustom(this->poRenderPass);
            }
            else
            {
                if (HasConfig_Imgui())
                {
                    if (HasConfig_MASS())
                    {
                        createRenderPass_ColorDepthImguiMSAA(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poMSAASamples, this->poRenderPass);
                    }
                    else
                    {
                        createRenderPass_KhrDepthImgui(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poRenderPass);
                    }
                }
                else
                {
                    if (HasConfig_MASS())
                    {
                        createRenderPass_ColorDepthMSAA(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poMSAASamples, this->poRenderPass);
                    }
                    else
                    {
                        createRenderPass_KhrDepth(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poRenderPass);
                    }
                }
            }
            
            F_LogInfo("VulkanWindow::createRenderPass_Default: Success to create RenderPass_Default !");
        }
        void VulkanWindow::createRenderPass_Cull()
        {
            if (!this->cfg_isRenderPassCull)
                return;

            if (m_pVKRenderPassCull == nullptr)
            {
                m_pVKRenderPassCull = new VKRenderPassCull("RenderPass-Cull");
            }
            m_pVKRenderPassCull->Init();
        }
        void VulkanWindow::createRenderPass_Terrain()
        {
            if (!this->cfg_isRenderPassTerrain)
                return;

            if (m_pVKRenderPassTerrain == nullptr)
            {
                m_pVKRenderPassTerrain = new VKRenderPassTerrain("RenderPass-Terrain");
            }
            m_pVKRenderPassTerrain->Init();
        }
        void VulkanWindow::createRenderPass_Custom()
        {
            
        }
            void VulkanWindow::createAttachmentDescription(VkAttachmentDescription& attachment,
                                                           VkAttachmentDescriptionFlags flags,
                                                           VkFormat format,
                                                           VkSampleCountFlagBits samples,
                                                           VkAttachmentLoadOp loadOp,
                                                           VkAttachmentStoreOp storeOp,
                                                           VkAttachmentLoadOp stencilLoadOp,
                                                           VkAttachmentStoreOp stencilStoreOp,
                                                           VkImageLayout initialLayout,
                                                           VkImageLayout finalLayout)
            {
                attachment.flags = flags;
                attachment.format = format;
                attachment.samples = samples;
                attachment.loadOp = loadOp;
                attachment.storeOp = storeOp;
                attachment.stencilLoadOp = stencilLoadOp;
                attachment.stencilStoreOp = stencilStoreOp;
                attachment.initialLayout = initialLayout;
                attachment.finalLayout = finalLayout;
            }
            bool VulkanWindow::createVkRenderPass(const String& nameRenderPass,
                                                  const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                  const VkSubpassDescriptionVector& aSubpassDescription,
                                                  const VkSubpassDependencyVector& aSubpassDependency,
                                                  VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                                  VkRenderPass& vkRenderPass)
            {
                VkRenderPassCreateInfo renderPassInfo = {};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                renderPassInfo.attachmentCount = static_cast<uint32_t>(aAttachmentDescription.size());
                renderPassInfo.pAttachments = aAttachmentDescription.data();
                renderPassInfo.subpassCount = static_cast<uint32_t>(aSubpassDescription.size());
                renderPassInfo.pSubpasses = aSubpassDescription.data();
                renderPassInfo.dependencyCount = static_cast<uint32_t>(aSubpassDependency.size());
                renderPassInfo.pDependencies = aSubpassDependency.data();
                if (pMultiviewCI != nullptr)
                    renderPassInfo.pNext = pMultiviewCI;

                if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &vkRenderPass) != VK_SUCCESS)
                {
                    F_LogError("*********************** VulkanWindow::createVkRenderPass: vkCreateRenderPass failed: [%s] !", nameRenderPass.c_str());
                    return false;
                }
                this->poDebug->SetVkRenderPassName(this->poDevice, vkRenderPass, nameRenderPass.c_str());

                F_LogInfo("VulkanWindow::createVkRenderPass: vkCreateRenderPass success: [%s] !", nameRenderPass.c_str());
                return true;
            }
            void VulkanWindow::destroyVkRenderPass(VkRenderPass vkRenderPass)
            {
                if (vkRenderPass != VK_NULL_HANDLE)
                {
                    vkDestroyRenderPass(this->poDevice, vkRenderPass, nullptr);   
                }
            }
            void VulkanWindow::createRenderPass_DefaultCustom(VkRenderPass& vkRenderPass)
            {

            }
            void VulkanWindow::createRenderPass_KhrDepth(VkFormat formatSwapChain, VkFormat formatDepth, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                    
                //3> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                aSubpassDescription.push_back(subpass_SceneRender);
                
                //4> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //5> createVkRenderPass
                String nameRenderPass = "RenderPass-Default-KhrDepth";
                if (!createVkRenderPass(nameRenderPass,
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_KhrDepth: Failed to create [" + nameRenderPass + "] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                F_LogInfo("VulkanWindow::createRenderPass_KhrDepth: Success to create [%s] !", nameRenderPass.c_str());
            }
            void VulkanWindow::createRenderPass_KhrDepthImgui(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                    
                //3> Attachment Imgui Color
                VkAttachmentDescription attachmentImgui_Color = {};
                createAttachmentDescription(attachmentImgui_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_LOAD,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentImgui_Color);
                
                //4> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                aSubpassDescription.push_back(subpass_SceneRender);

                //5> Subpass Imgui
                VkAttachmentReference attachRef_ImguiColor = {};
                attachRef_ImguiColor.attachment = 0;
                attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiDepth = {};
                attachRef_ImguiDepth.attachment = 1;
                attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_Imgui = {};
                subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_Imgui.colorAttachmentCount = 1;
                subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
                subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
                aSubpassDescription.push_back(subpass_Imgui);
                
                //6> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //7> Subpass Dependency Imgui
                VkSubpassDependency subpassDependency_Imgui = {};
                subpassDependency_Imgui.srcSubpass = 0;
                subpassDependency_Imgui.dstSubpass = 1;
                subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_Imgui);

                //8> createVkRenderPass
                String nameRenderPass = "RenderPass-Default-KhrDepthImgui";
                if (!createVkRenderPass(nameRenderPass,
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_KhrDepthImgui: Failed to create [" + nameRenderPass + "] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                F_LogInfo("VulkanWindow::createRenderPass_KhrDepthImgui: Success to create [%s] !", nameRenderPass.c_str());
            }
            void VulkanWindow::createRenderPass_ColorDepthMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                
                //3> Attachment SceneRender Color Resolve
                VkAttachmentDescription attachmentSR_ColorResolve = {};
                createAttachmentDescription(attachmentSR_ColorResolve,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentSR_ColorResolve);
                
                //4> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ColorResolve = {};
                attachRef_ColorResolve.attachment = 2;
                attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
                aSubpassDescription.push_back(subpass_SceneRender);
                
                //5> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //6> createVkRenderPass
                String nameRenderPass = "RenderPass-Default-ColorDepthMSAA";
                if (!createVkRenderPass(nameRenderPass,
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_ColorDepthMSAA: Failed to create [" + nameRenderPass + "] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                F_LogInfo("VulkanWindow::createRenderPass_ColorDepthMSAA: Success to create [%s] !", nameRenderPass.c_str());
            }
            void VulkanWindow::createRenderPass_ColorDepthImguiMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                
                //3> Attachment SceneRender Color Resolve
                VkAttachmentDescription attachmentSR_ColorResolve = {};
                createAttachmentDescription(attachmentSR_ColorResolve,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_ColorResolve);
                
                //4> Attachment Imgui Color
                VkAttachmentDescription attachmentImgui_Color = {};
                createAttachmentDescription(attachmentImgui_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_LOAD,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentImgui_Color);
                
                //5> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ColorResolve = {};
                attachRef_ColorResolve.attachment = 2;
                attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
                aSubpassDescription.push_back(subpass_SceneRender);

                //6> Subpass Imgui
                VkAttachmentReference attachRef_ImguiColor = {};
                attachRef_ImguiColor.attachment = 0;
                attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiDepth = {};
                attachRef_ImguiDepth.attachment = 1;
                attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiColorResolve = {};
                attachRef_ImguiColorResolve.attachment = 2;
                attachRef_ImguiColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_Imgui = {};
                subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_Imgui.colorAttachmentCount = 1;
                subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
                subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
                subpass_Imgui.pResolveAttachments = &attachRef_ImguiColorResolve;
                aSubpassDescription.push_back(subpass_Imgui);
                
                //7> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //8> Subpass Dependency Imgui
                VkSubpassDependency subpassDependency_Imgui = {};
                subpassDependency_Imgui.srcSubpass = 0;
                subpassDependency_Imgui.dstSubpass = 1;
                subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                aSubpassDependency.push_back(subpassDependency_Imgui);

                //9> createVkRenderPass
                String nameRenderPass = "RenderPass-Default-ColorDepthImguiMSAA";
                if (!createVkRenderPass(nameRenderPass,
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_ColorDepthImguiMSAA: Failed to create [" + nameRenderPass + "] !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                F_LogInfo("VulkanWindow::createRenderPass_ColorDepthImguiMSAA: Success to create [%s] !", nameRenderPass.c_str());
            }

        void VulkanWindow::createFramebuffers()
        {
            //1> createFramebuffer_Default
            createFramebuffer_Default();

            //2> createFramebuffer_Custom
            createFramebuffer_Custom();
        }
            void VulkanWindow::createFramebuffer_Default()
            {
                if (HasConfig_RenderPassDefaultCustom())
                {
                    createFramebuffer_DefaultCustom();
                }
                else
                {
                    size_t count = this->poSwapChainImageViews.size();
                    this->poSwapChainFrameBuffers.resize(count);
                    for (size_t i = 0; i < count; i++)
                    {
                        VkImageViewVector aImageViews;
                        if (!HasConfig_Imgui())
                        {
                            if (!HasConfig_MASS())
                            {
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                                aImageViews.push_back(this->poDepthImageView);
                            }
                            else
                            {
                                aImageViews.push_back(this->poColorImageView);
                                aImageViews.push_back(this->poDepthImageView);
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                            }
                        }
                        else
                        {
                            if (!HasConfig_MASS())
                            {
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                                aImageViews.push_back(this->poDepthImageView);
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                            }
                            else
                            {
                                aImageViews.push_back(this->poColorImageView);
                                aImageViews.push_back(this->poDepthImageView);
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                                aImageViews.push_back(this->poSwapChainImageViews[i]);
                            }
                        }

                        String nameFramebuffer = "Framebuffer-" + FUtilString::SaveSizeT(i);
                        if (!createVkFramebuffer(nameFramebuffer,
                                                 aImageViews,
                                                 this->poRenderPass,
                                                 0,
                                                 this->poSwapChainExtent.width,
                                                 this->poSwapChainExtent.height,
                                                 1,
                                                 this->poSwapChainFrameBuffers[i]))
                        {
                            String msg = "*********************** VulkanWindow::createFramebuffer_Default: Failed to create framebuffer: " + nameFramebuffer;
                            F_LogError(msg.c_str());
                            throw std::runtime_error(msg);
                        }
                    }
                }

                F_LogInfo("VulkanWindow::createFramebuffer_Default: Success to create Framebuffer_Default !");
            }
            void VulkanWindow::createFramebuffer_Custom()
            {
                
            }
                void VulkanWindow::createFramebuffer_DefaultCustom()
                {

                }

                bool VulkanWindow::createVkFramebuffer(const String& nameFramebuffer,
                                                       const VkImageViewVector& aImageView, 
                                                       VkRenderPass& vkRenderPass,
                                                       VkFramebufferCreateFlags flags,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint32_t layers,
                                                       VkFramebuffer& vkFramebuffer)
                {
                    VkFramebufferCreateInfo framebufferInfo = {};
                    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    framebufferInfo.renderPass = vkRenderPass;
                    framebufferInfo.attachmentCount = static_cast<uint32_t>(aImageView.size());
                    framebufferInfo.pAttachments = aImageView.data();
                    framebufferInfo.width = width;
                    framebufferInfo.height = height;
                    framebufferInfo.layers = layers;

                    if (vkCreateFramebuffer(this->poDevice, &framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) 
                    {
                        F_LogError("*********************** VulkanWindow::createVkFramebuffer: vkCreateFramebuffer failed: [%s] !", nameFramebuffer.c_str());
                        return false;
                    }
                    this->poDebug->SetVkFramebufferName(this->poDevice, vkFramebuffer, nameFramebuffer.c_str());

                    F_LogInfo("VulkanWindow::createVkFramebuffer: vkCreateFramebuffer success: [%s] !", nameFramebuffer.c_str());
                    return true;
                }
                void VulkanWindow::destroyVkFramebuffer(VkFramebuffer vkFramebuffer)
                {
                    if (vkFramebuffer != VK_NULL_HANDLE)
                    {
                        vkDestroyFramebuffer(this->poDevice, vkFramebuffer, nullptr);
                    }
                }

    void VulkanWindow::createSyncObjects()
    {
        //1> Present/Render VkSemaphore/VkFence 
        createPresentRenderSyncObjects();

        //2> Graphics/Compute Sync VkSemaphore
        if (this->cfg_isCreateRenderComputeSycSemaphore)
        {
            createRenderComputeSyncObjects();
        }

        F_LogInfo("*****<1-10> VulkanWindow::createSyncObjects finish *****");
    }
        void VulkanWindow::createPresentRenderSyncObjects()
        {
            this->poPresentCompleteSemaphores.resize(s_maxFramesInFight);
            this->poRenderCompleteSemaphores.resize(s_maxFramesInFight);
            this->poInFlightFences.resize(s_maxFramesInFight);
            this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

            for (size_t i = 0; i < s_maxFramesInFight; i++) 
            {
                String nameFix = FUtilString::SaveSizeT(i);
                String nameSemaphorePresentComplete = "Semaphore-PresentComplete-" + nameFix;
                String nameSemaphoreRenderComplete = "Semaphore-RenderComplete-" + nameFix;
                String nameFenceInFlight = "Fence-InFlight-" + nameFix; 

                if (!createVkSemaphore(nameSemaphorePresentComplete, 0, this->poPresentCompleteSemaphores[i]) ||
                    !createVkSemaphore(nameSemaphoreRenderComplete, 0, this->poRenderCompleteSemaphores[i]) ||
                    !createVkFence(nameFenceInFlight, VK_FENCE_CREATE_SIGNALED_BIT, this->poInFlightFences[i])) 
                {
                    String msg = "*********************** VulkanWindow::createPresentRenderSyncObjects: Failed to create present/render synchronization objects for a frame !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
            
            F_LogInfo("<1-10-1> VulkanWindow::createPresentRenderSyncObjects finish !");
        }
        void VulkanWindow::createRenderComputeSyncObjects()
        {
            //1> Compute Before WaitSemaphore
            if (this->cfg_isUseComputeShaderBeforeRender)
            {
                //Compute Before Wait Semaphore
                String nameSemaphoreComputeWaitBefore = "Semaphore-ComputeWaitBefore";
                if (!createVkSemaphore(nameSemaphoreComputeWaitBefore, 0, this->poComputeBeforeWaitSemaphore))
                {
                    String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create: " + nameSemaphoreComputeWaitBefore;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                //Graphics Wait ComputeBefore Semaphore
                String nameSemaphoreGraphicsWaitComputeBefore = "Semaphore-GraphicsWaitComputeBefore";
                if (!createVkSemaphore(nameSemaphoreGraphicsWaitComputeBefore, 0, this->poGraphicsWaitComputeBeforeSemaphore))
                {
                    String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create: " + nameSemaphoreGraphicsWaitComputeBefore;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                VkSubmitInfo submitInfo = {};
                submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores = &this->poGraphicsWaitComputeBeforeSemaphore;
                vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, nullptr);
                vkQueueWaitIdle(this->poQueueGraphics);
            }

            //2> Compute After WaitSemaphore
            if (this->cfg_isUseComputeShaderAfterRender)
            {
                //Compute After Wait Semaphore
                String nameSemaphoreComputeWaitAfter = "Semaphore-ComputeWaitAfter";
                if (!createVkSemaphore(nameSemaphoreComputeWaitAfter, 0, this->poComputeAfterWaitSemaphore))
                {
                    String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create: " + nameSemaphoreComputeWaitAfter;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                //Graphics Signal ComputeAfter Semaphore
                String nameSemaphoreGraphicsSignalComputeAfter = "Semaphore-GraphicsSignalComputeAfter";
                if (!createVkSemaphore(nameSemaphoreGraphicsSignalComputeAfter, 0, this->poGraphicsSignalComputeAfterSemaphore))
                {
                    String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create: " + nameSemaphoreGraphicsSignalComputeAfter;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
            
            F_LogInfo("<1-10-2> VulkanWindow::createRenderComputeSyncObjects finish !");
        }
            bool VulkanWindow::createVkSemaphore(const String& nameSemaphore,
                                                 VkSemaphoreCreateFlags flags,
                                                 VkSemaphore& vkSemaphore)
            {
                VkSemaphoreCreateInfo semaphoreInfo = {};
                semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
                semaphoreInfo.flags = flags;

                if (vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &vkSemaphore) != VK_SUCCESS)
                {
                    F_LogError("*********************** VulkanWindow::createVkSemaphore: Failed to create VkSemaphore: [%s] !", nameSemaphore.c_str());
                    return false;
                }
                this->poDebug->SetVkSemaphoreName(this->poDevice, vkSemaphore, nameSemaphore.c_str());

                return true;
            }
            void VulkanWindow::destroyVkSemaphore(VkSemaphore vkSemaphore)
            {   
                if (vkSemaphore != VK_NULL_HANDLE)
                {
                    vkDestroySemaphore(this->poDevice, vkSemaphore, nullptr);
                }
            }

            bool VulkanWindow::createVkFence(const String& nameFence,
                                             VkFenceCreateFlags flags,
                                             VkFence& vkFence)
            {
                VkFenceCreateInfo fenceInfo = {};
                fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
                fenceInfo.flags = flags;

                if (vkCreateFence(this->poDevice, &fenceInfo, nullptr, &vkFence) != VK_SUCCESS)
                {
                    F_LogError("*********************** VulkanWindow::createVkFence: Failed to create VkFence: [%s] !", nameFence.c_str());
                    return false;
                }
                this->poDebug->SetVkFenceName(this->poDevice, vkFence, nameFence.c_str());

                return true;
            }
            VkResult VulkanWindow::getFenceStatus(VkFence vkFence)
            {
                return vkGetFenceStatus(this->poDevice, vkFence);
            }
            VkResult VulkanWindow::waitForFence(VkFence vkFence, VkBool32 waitAll, uint64_t timeout)
            {
                return vkWaitForFences(this->poDevice, 1, &vkFence, waitAll, timeout);
            }
            VkResult VulkanWindow::waitForFences(VkFenceVector& vkFences, VkBool32 waitAll, uint64_t timeout)
            {
                return vkWaitForFences(this->poDevice,(uint32_t)vkFences.size(), vkFences.data(), waitAll, timeout);
            }
            VkResult VulkanWindow::resetVkFence(VkFence vkFence)
            {
                return vkResetFences(this->poDevice, 1, &vkFence);
            }
            VkResult VulkanWindow::resetVkFences(VkFenceVector& vkFences)
            {
                return vkResetFences(this->poDevice, (uint32_t)vkFences.size(), vkFences.data());
            }
            void VulkanWindow::destroyVkFence(VkFence vkFence)
            {
                if (vkFence != VK_NULL_HANDLE)
                {
                    vkDestroyFence(this->poDevice, vkFence, nullptr);
                }
            }

    void VulkanWindow::loadAssets()
    {
        F_LogInfo("**********<2> VulkanWindow::loadAssets start **********");
        {
            //0> Camera/Light/Shadow
            cameraReset();
            lightMainReset();
            shadowReset();
            terrainReset();

            //1> loadGeometry
            loadGeometry();

            //2> Imgui
            if (HasConfig_Imgui())
            {
                createImgui();
            }

            //3> Editor
            if (this->cfg_isEditorCreate)
            {
                createEditor();
            }

            this->isLoadAsset = true;
        }
        F_LogInfo("**********<2> VulkanWindow::loadAssets finish **********");
    }
        void VulkanWindow::loadGeometry()
        {
            F_LogInfo("*****<2-1> VulkanWindow::loadGeometry start *****");
            {
                //1> loadVertexIndexBuffer
                loadVertexIndexBuffer();

                //2> loadTexture
                loadTexture();

                //3> createConstBuffers
                createConstBuffers();

                //4> createCustomBeforePipeline
                createCustomBeforePipeline();

                //5> createGraphicsPipeline
                createGraphicsPipeline();

                //6> createComputePipeline
                createComputePipeline();

                //7> createDescriptorSets
                createDescriptorSets();    

                //8> createCommandBuffers
                createCommandBuffers();    
            }
            F_LogInfo("*****<2-1> VulkanWindow::loadGeometry finish *****");
        }
    void VulkanWindow::loadVertexIndexBuffer()
    {
        F_LogInfo("**<2-1-1> VulkanWindow::loadVertexIndexBuffer start **");
        {
            //1> loadModel
            loadModel();

            //2> createVertexBuffer
            if (this->poVertexBuffer_Size > 0 &&
                this->poVertexBuffer_Data != nullptr)
            {
                createVertexBuffer("Vertex-" + this->nameTitle,
                                   this->poVertexBuffer_Size, 
                                   this->poVertexBuffer_Data, 
                                   this->poVertexBuffer, 
                                   this->poVertexBufferMemory);
            }

            //3> createIndexBuffer
            if (this->poIndexBuffer_Size > 0 &&
                this->poIndexBuffer_Data != nullptr)
            {
                createIndexBuffer("Index-" + this->nameTitle,
                                  this->poIndexBuffer_Size, 
                                  this->poIndexBuffer_Data, 
                                  this->poIndexBuffer, 
                                  this->poIndexBufferMemory);
            }
        }
        F_LogInfo("**<2-1-1> VulkanWindow::loadVertexIndexBuffer finish **");
    }
    void VulkanWindow::loadModel()
    {
        F_LogInfo("**<2-1-1-1> VulkanWindow::loadModel start **");
        {
            //1> model 
            if (!this->cfg_model_Path.empty())
            {
                loadModel_Default();
            }
            //2> model user
            else
            {
                loadModel_Custom();
            }
        }
        F_LogInfo("**<2-1-1-1> VulkanWindow::loadModel finish **");
    }
        void VulkanWindow::loadModel_Default()
        {
            
        }
        void VulkanWindow::loadModel_Custom()
        {

        }
    void VulkanWindow::createVertexBuffer(const String& nameBuffer,
                                          size_t bufSize, 
                                          void* pBuf, 
                                          VkBuffer& vertexBuffer, 
                                          VkDeviceMemory& vertexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createVertexBuffer(nameBuffer,
                           bufSize,
                           pBuf,
                           vertexBuffer,
                           vertexBufferMemory,
                           stagingBuffer,
                           stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createVertexBuffer(const String& nameBuffer,
                                          size_t bufSize, 
                                          void* pBuf, 
                                          VkBuffer& vertexBuffer, 
                                          VkDeviceMemory& vertexBufferMemory,
                                          VkBuffer& stagingBuffer,
                                          VkDeviceMemory& stagingBufferMemory)
    {
        createVkBuffer("Staging-Vertex-" + nameBuffer,
                       bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       stagingBuffer, 
                       stagingBufferMemory);
        {
            updateVertexBuffer(bufSize, pBuf, stagingBufferMemory);
        }
        createVkBuffer(nameBuffer,
                       bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                       vertexBuffer,
                       vertexBufferMemory);
        copyVkBuffer(stagingBuffer, vertexBuffer, bufSize);
    }
    void VulkanWindow::updateVertexBuffer(size_t bufSize, 
                                          void* pBuf, 
                                          VkDeviceMemory& vertexBufferMemory)
    {
        updateVKBuffer(0, bufSize, pBuf, vertexBufferMemory);
    }   
    void VulkanWindow::createIndexBuffer(const String& nameBuffer,
                                         size_t bufSize, 
                                         void* pBuf, 
                                         VkBuffer& indexBuffer, 
                                         VkDeviceMemory& indexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createIndexBuffer(nameBuffer,
                          bufSize,
                          pBuf,
                          indexBuffer,
                          indexBufferMemory,
                          stagingBuffer,
                          stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createIndexBuffer(const String& nameBuffer,
                                         size_t bufSize, 
                                         void* pBuf, 
                                         VkBuffer& indexBuffer, 
                                         VkDeviceMemory& indexBufferMemory,
                                         VkBuffer& stagingBuffer,
                                         VkDeviceMemory& stagingBufferMemory)
    {
        createVkBuffer("Staging-Index-" + nameBuffer,
                       bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       stagingBuffer, 
                       stagingBufferMemory);
        {
            updateIndexBuffer(bufSize, pBuf, stagingBufferMemory);
        }
        createVkBuffer(nameBuffer,
                       bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                       indexBuffer, 
                       indexBufferMemory);
        copyVkBuffer(stagingBuffer, indexBuffer, bufSize);    
    }
    void VulkanWindow::updateIndexBuffer(size_t bufSize, 
                                         void* pBuf, 
                                         VkDeviceMemory& indexBufferMemory)
    {
        updateVKBuffer(0, bufSize, pBuf, indexBufferMemory);
    }   
        void VulkanWindow::createVkBuffer(const String& nameBuffer,
                                          VkDeviceSize size, 
                                          VkBufferUsageFlags usage, 
                                          VkMemoryPropertyFlags properties, 
                                          VkBuffer& buffer, 
                                          VkDeviceMemory& bufferMemory)
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (!Util_CheckVkResult(vkCreateBuffer(this->poDevice, &bufferInfo, nullptr, &buffer), "vkCreateBuffer")) 
            {
                String msg = "*********************** VulkanWindow::createVkBuffer: Failed to create buffer !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkBufferName(this->poDevice, buffer, nameBuffer.c_str());

            VkMemoryRequirements memRequirements = {};
            vkGetBufferMemoryRequirements(this->poDevice, buffer, &memRequirements);
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkBuffer: Failed to allocate buffer memory !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkDeviceMemoryName(this->poDevice, bufferMemory, nameBuffer.c_str());
            vkBindBufferMemory(this->poDevice, buffer, bufferMemory, 0);
        }
            uint32_t VulkanWindow::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
            {
                VkPhysicalDeviceMemoryProperties memProperties;
                vkGetPhysicalDeviceMemoryProperties(this->poPhysicalDevice, &memProperties);

                for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
                {
                    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
                    {
                        return i;
                    }
                }

                String msg = "*********************** VulkanWindow::findMemoryType: Failed to find suitable memory type !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            void VulkanWindow::copyVkBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
            {
                VkCommandBuffer commandBuffer = beginSingleTimeCommands();
                {
                    VkBufferCopy copyRegion = {};
                    copyRegion.size = size;
                    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
                }
                endSingleTimeCommands(commandBuffer);
            }
            void VulkanWindow::updateVKBuffer(size_t offset, size_t bufSize, void* pBuf, VkDeviceMemory& vertexBufferMemory, VkMemoryMapFlags flags /*= 0*/)
            {
                void* data;
                vkMapMemory(this->poDevice, vertexBufferMemory, offset, bufSize, flags, &data);
                    memcpy(data, pBuf, bufSize);
                vkUnmapMemory(this->poDevice, vertexBufferMemory);
            }
            void VulkanWindow::destroyVkBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory)
            {
                if (buffer != VK_NULL_HANDLE)
                {
                    vkDestroyBuffer(this->poDevice, buffer, nullptr);
                    vkFreeMemory(this->poDevice, bufferMemory, nullptr);
                }
            }

    void VulkanWindow::loadTexture()
    {
        F_LogInfo("**<2-1-2> VulkanWindow::loadTexture start **");
        {
            //1> Texture Default 
            loadTexture_Default();

            //2> Texture Custom
            loadTexture_Custom();
        }
        F_LogInfo("**<2-1-2> VulkanWindow::loadTexture finish **");
    }
        void VulkanWindow::loadTexture_Default()
        {
            if (!this->cfg_texture_Path.empty())
            {
                String nameTexture;
                String pathBase;
                FUtilString::SplitFileName(this->cfg_texture_Path, nameTexture, pathBase);
                createTexture2D(nameTexture, this->cfg_texture_Path, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
                createVkImageView(nameTexture, this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, 1, this->poTextureImageView);
                createVkSampler(nameTexture, this->poMipMapCount, this->poTextureSampler);

                F_LogInfo("<2-1-2-1> VulkanWindow::loadTexture_Default finish !");
            }
        }
        void VulkanWindow::loadTexture_Custom()
        {
            
        }
    void VulkanWindow::destroyVkImage(VkImage image, VkDeviceMemory imageMemory, VkImageView imageView)
    {
        if (image != VK_NULL_HANDLE)
        {
            vkDestroyImage(this->poDevice, image, nullptr);
            vkFreeMemory(this->poDevice, imageMemory, nullptr);
        }
        destroyVkImageView(imageView);
    }
    void VulkanWindow::destroyVkImageView(VkImageView imageView)
    {
        if (imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(this->poDevice, imageView, nullptr);
        }
    }
    void VulkanWindow::destroyVkImageSampler(VkSampler sampler)
    {
        if (sampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(this->poDevice, sampler, nullptr);
        }
    }
    void VulkanWindow::createTexture1D(const String& nameTex, 
                                       const String& pathAsset_Tex, 
                                       uint32_t& mipMapCount,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        createTexture2D(nameTex, 
                        pathAsset_Tex,
                        VK_IMAGE_TYPE_1D,
                        VK_SAMPLE_COUNT_1_BIT,
                        VK_FORMAT_R8G8B8A8_SRGB,
                        true,
                        mipMapCount,
                        image,
                        imageMemory);
    }
    
    void VulkanWindow::createTexture2D(const String& nameTex, 
                                       const String& pathAsset_Tex, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory,
                                       VkBuffer& buffer, 
                                       VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        String pathTexture = GetAssetFullPath(pathAsset_Tex);
        int width, height, texChannels;
        stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = width * height * 4;
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        if (!pixels) 
        {
            String msg = "*********************** VulkanWindow::createTexture2D: Failed to load texture image: " + pathAsset_Tex;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        //2> Create Buffer and copy Texture data to buffer
        createVkBuffer(nameTex,
                       imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        updateVKBuffer(0, imageSize, pixels, bufferMemory);

        stbi_image_free(pixels);

        uint32_t depth = 1;
        uint32_t numArray = 1;

        //3> CreateImage
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            width, 
                            height, 
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture2D(const String& nameTex, 
                                       const String& pathAsset_Tex, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture2D(nameTex, 
                        pathAsset_Tex, 
                        type, 
                        numSamples,
                        format,
                        autoMipMap,
                        mipMapCount,
                        image, 
                        imageMemory, 
                        stagingBuffer, 
                        stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTexture2D(const String& nameTex, 
                                       const String& pathAsset_Tex, 
                                       uint32_t& mipMapCount,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        createTexture2D(nameTex, 
                        pathAsset_Tex,
                        VK_IMAGE_TYPE_2D,
                        VK_SAMPLE_COUNT_1_BIT,
                        VK_FORMAT_R8G8B8A8_SRGB,
                        true,
                        mipMapCount,
                        image,
                        imageMemory);
    }

    static void s_DeletePixels(const std::vector<stbi_uc*>& aPixels)
    {
        size_t count_tex = aPixels.size();
        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];
            stbi_image_free(pixels);
        }
    }
    void VulkanWindow::createTexture2DArray(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory,
                                            VkBuffer& buffer, 
                                            VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset_Tex.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** VulkanWindow::createTexture2DArray: Texture path count <= 0 !");
            return;
        }
        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset_Tex = aPathAsset_Tex[i];
            String pathTexture = GetAssetFullPath(pathAsset_Tex);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Failed to load texture image: " + pathTexture;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Texture image's all width must the same !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            if (aHeight[i] != height)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Texture image's all height must the same !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        uint32_t depth = 1;
        uint32_t numArray = (uint32_t)count_tex;
        if (type == VK_IMAGE_TYPE_1D)
        {
            depth = 0;
        }

        //2> Create Buffer and copy Texture data to buffer
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        createVkBuffer(nameTex, 
                       imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];
            updateVKBuffer(width * height * 4 * i, imageSize, pixels, bufferMemory);
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture2DArray(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture2DArray(nameTex, 
                             aPathAsset_Tex, 
                             type, 
                             numSamples,
                             format,
                             autoMipMap,
                             mipMapCount,
                             image, 
                             imageMemory, 
                             stagingBuffer, 
                             stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTexture2DArray(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex, 
                                            uint32_t& mipMapCount,
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        createTexture2DArray(nameTex, 
                             aPathAsset_Tex,
                             VK_IMAGE_TYPE_2D,
                             VK_SAMPLE_COUNT_1_BIT,
                             VK_FORMAT_R8G8B8A8_SRGB,
                             true,
                             mipMapCount,
                             image,
                             imageMemory);
    }
    
    void VulkanWindow::createTexture3D(const String& nameTex, 
                                       VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory,
                                       VkBuffer& buffer, 
                                       VkDeviceMemory& bufferMemory)
    {
        VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(this->poPhysicalDevice, format, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT))
		{
            F_LogError("*********************** VulkanWindow::createTexture3D: Physical device does not support flag 'VK_FORMAT_FEATURE_TRANSFER_DST_BIT' for selected texture format !");
            return;
		}
		uint32_t maxImageDimension3D(this->poPhysicalDeviceProperties.limits.maxImageDimension3D);
		if (width > maxImageDimension3D || height > maxImageDimension3D || depth > maxImageDimension3D)
		{
            F_LogError("*********************** VulkanWindow::createTexture3D: Requested texture dimensions is greater than supported 3D texture dimension !");
			return;
		}

        //1> Create Buffer and copy Texture data to buffer
        VkDeviceSize imageSize = size;
        createVkBuffer(nameTex, 
                       imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        updateVKBuffer(0, imageSize, (void*)pDataRGBA, bufferMemory);

        //2> CreateImage
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      1,
                      1, 
                      VK_IMAGE_TYPE_3D,
                      false,
                      VK_SAMPLE_COUNT_1_BIT, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  1);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  1);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  1);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            width, 
                            height,
                            1,
                            1,
                            false);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture3D(const String& nameTex, 
                                       VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture3D(nameTex, 
                        format, 
                        pDataRGBA, 
                        size,
                        width,
                        height,
                        depth,
                        image, 
                        imageMemory, 
                        stagingBuffer, 
                        stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureCubeMap(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory,
                                            VkBuffer& buffer, 
                                            VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset_Tex.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** VulkanWindow::createTextureCubeMap: Texture path count <= 0 !");
            return;
        }
        if (count_tex != 6)
        {
            F_LogError("*********************** VulkanWindow::createTextureCubeMap: Texture path count != 6 !");
            return;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset_Tex = aPathAsset_Tex[i];
            String pathTexture = GetAssetFullPath(pathAsset_Tex);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Failed to load texture image: " + pathTexture;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Texture image's all width must the same !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            if (aHeight[i] != height)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Texture image's all height must the same !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        uint32_t depth = 1;
        uint32_t numArray = (uint32_t)count_tex;

        //2> Create Buffer and copy Texture data to buffer
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        createVkBuffer(nameTex, 
                       imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];
            updateVKBuffer(width * height * 4 * i, imageSize, pixels, bufferMemory);
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      VK_IMAGE_TYPE_2D,
                      true,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureCubeMap(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureCubeMap(nameTex, 
                             aPathAsset_Tex, 
                             numSamples, 
                             format,
                             autoMipMap,
                             mipMapCount,
                             image, 
                             imageMemory, 
                             stagingBuffer, 
                             stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTextureCubeMap(const String& nameTex, 
                                            const StringVector& aPathAsset_Tex,
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        createTextureCubeMap(nameTex, 
                             aPathAsset_Tex, 
                             VK_SAMPLE_COUNT_1_BIT, 
                             VK_FORMAT_R8G8B8A8_SRGB,
                             true,
                             mipMapCount,
                             image, 
                             imageMemory);
    }

    void VulkanWindow::createTextureRenderTarget1D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory,
                                                   VkBuffer& buffer, 
                                                   VkDeviceMemory& bufferMemory)
    {
        createTextureRenderTarget2D(nameTex,
                                    clDefault,
                                    isSetColor,
                                    width,
                                    1,
                                    autoMipMap, 
                                    mipMapCount,
                                    VK_IMAGE_TYPE_1D,
                                    numSamples,
                                    format,
                                    usage,
                                    finalLayout,
                                    isGraphicsComputeShared,
                                    image,
                                    imageMemory,
                                    buffer,
                                    bufferMemory);
    }
    void VulkanWindow::createTextureRenderTarget1D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget1D(nameTex,
                                    clDefault, 
                                    isSetColor,
                                    width, 
                                    autoMipMap, 
                                    mipMapCount,
                                    numSamples,
                                    format,
                                    usage,
                                    finalLayout,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkImageType type,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory,
                                                   VkBuffer& buffer, 
                                                   VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        createVkBuffer(nameTex, 
                       imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (uint32_t i = 0; i < width * height; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);
        
        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            finalLayout,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureRenderTarget2D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2D(nameTex,
                                    clDefault, 
                                    isSetColor,
                                    width, 
                                    height,
                                    autoMipMap, 
                                    mipMapCount,
                                    VK_IMAGE_TYPE_2D,
                                    numSamples,
                                    format,
                                    usage,
                                    finalLayout,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2D(const String& nameTex,
                                                   uint8* pData,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkImageType type,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory,
                                                   VkBuffer& buffer, 
                                                   VkDeviceMemory& bufferMemory)
    {
        uint32_t sizeFormat = getSizeFromFormat(format);
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * sizeFormat;
        createVkBuffer(nameTex, 
                       imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (pData != nullptr)
        {
            updateVKBuffer(0, imageSize, pData, bufferMemory);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            finalLayout,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureRenderTarget2D(const String& nameTex,
                                                   uint8* pData,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2D(nameTex,
                                    pData,
                                    width, 
                                    height,
                                    autoMipMap, 
                                    mipMapCount,
                                    VK_IMAGE_TYPE_2D,
                                    numSamples,
                                    format,
                                    usage,
                                    finalLayout,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2DArray(const String& nameTex,
                                                        const FVector4& clDefault,
                                                        bool isSetColor,
                                                        uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t numArray,
                                                        bool autoMipMap, 
                                                        uint32_t mipMapCount,
                                                        VkImageType type,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        VkImageLayout finalLayout,
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory,
                                                        VkBuffer& buffer, 
                                                        VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        createVkBuffer(nameTex, 
                       imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSizeAll, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (uint32_t i = 0; i < width * height * numArray; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }

        //2> CreateImage
        uint32_t depth = 1;
        if (type == VK_IMAGE_TYPE_1D)
        {
            depth = 0;
        }
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            finalLayout,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureRenderTarget2DArray(const String& nameTex,  
                                                        const FVector4& clDefault,
                                                        bool isSetColor,
                                                        uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t numArray,
                                                        bool autoMipMap, 
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        VkImageLayout finalLayout,
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2DArray(nameTex,  
                                         clDefault, 
                                         isSetColor,
                                         width, 
                                         height,
                                         numArray,
                                         autoMipMap, 
                                         mipMapCount,
                                         VK_IMAGE_TYPE_2D,
                                         numSamples,
                                         format,
                                         usage,
                                         finalLayout,
                                         isGraphicsComputeShared,
                                         image, 
                                         imageMemory, 
                                         stagingBuffer, 
                                         stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget3D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t depth,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory,
                                                   VkBuffer& buffer, 
                                                   VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * depth * 4;
        createVkBuffer(nameTex, 
                       imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (uint32_t i = 0; i < width * height * depth; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }

        //2> CreateImage
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      1,
                      mipMapCount, 
                      VK_IMAGE_TYPE_3D,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            uint32_t numArray = 1;
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            finalLayout,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureRenderTarget3D(const String& nameTex,
                                                   const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t depth,
                                                   bool autoMipMap, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   VkImageLayout finalLayout,
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget3D(nameTex,
                                    clDefault, 
                                    isSetColor,
                                    width, 
                                    height,
                                    depth,
                                    autoMipMap, 
                                    mipMapCount,
                                    numSamples,
                                    format,
                                    usage,
                                    finalLayout,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTargetCubeMap(const String& nameTex,
                                                        uint32_t width, 
                                                        uint32_t height,
                                                        bool autoMipMap, 
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        VkImageLayout finalLayout,
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory,
                                                        VkBuffer& buffer, 
                                                        VkDeviceMemory& bufferMemory)
    {
        uint32_t numArray = 6;
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        createVkBuffer(nameTex,  
                       imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        //2> CreateImage
        uint32_t depth = 1;
        createVkImage(nameTex, 
                      width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      VK_IMAGE_TYPE_2D,
                      true,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            finalLayout,
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureRenderTargetCubeMap(const String& nameTex,
                                                        uint32_t width, 
                                                        uint32_t height,
                                                        bool autoMipMap, 
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        VkImageLayout finalLayout,
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTargetCubeMap(nameTex,
                                         width, 
                                         height,
                                         autoMipMap, 
                                         mipMapCount,
                                         numSamples,
                                         format,
                                         usage,
                                         finalLayout,
                                         isGraphicsComputeShared,
                                         image, 
                                         imageMemory, 
                                         stagingBuffer, 
                                         stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }                                                     

        uint32_t VulkanWindow::getSizeFromFormat(VkFormat format)
        {
            uint32_t sizeFormat = 4;
            if (format == VK_FORMAT_R8_UNORM)
            {
                sizeFormat = 1;
            }
            else if (format == VK_FORMAT_R16_UNORM)
            {
                sizeFormat = 2;
            }

            return sizeFormat;
        }
        void VulkanWindow::createVkImage(const String& nameTex,
                                         uint32_t width, 
                                         uint32_t height, 
                                         uint32_t depth, 
                                         uint32_t numArray,
                                         uint32_t mipMapCount, 
                                         VkImageType type, 
                                         bool isCubeMap,
                                         VkSampleCountFlagBits numSamples, 
                                         VkFormat format, 
                                         VkImageTiling tiling, 
                                         VkImageUsageFlags usage, 
                                         VkSharingMode sharingMode,
                                         bool isGraphicsComputeShared,
                                         VkMemoryPropertyFlags properties, 
                                         VkImage& image, 
                                         VkDeviceMemory& imageMemory) 
        {
            VkImageCreateInfo imageCreateInfo = {};
            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            if (isCubeMap)
                imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
            imageCreateInfo.imageType = type;
            imageCreateInfo.format = format;
            imageCreateInfo.extent.width = width;
            imageCreateInfo.extent.height = height;
            imageCreateInfo.extent.depth = depth;
            imageCreateInfo.mipLevels = mipMapCount <= 0 ? 1 : mipMapCount;
            imageCreateInfo.arrayLayers = numArray;
            imageCreateInfo.samples = numSamples;
            imageCreateInfo.tiling = tiling;
            imageCreateInfo.usage = usage;
            imageCreateInfo.sharingMode = sharingMode;
            if (isGraphicsComputeShared)
            {
                if (this->queueIndexGraphics != this->queueIndexCompute) 
                {
                    std::vector<uint32_t> queueFamilyIndices;
                    queueFamilyIndices.push_back(this->queueIndexGraphics);
                    queueFamilyIndices.push_back(this->queueIndexCompute);
                    imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
                    imageCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
                    imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
                }
            }
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            if (vkCreateImage(this->poDevice, &imageCreateInfo, nullptr, &image) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImage: Failed to create image !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkImageName(this->poDevice, image, nameTex.c_str());

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(this->poDevice, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImage: Failed to allocate image memory !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkDeviceMemoryName(this->poDevice, imageMemory, nameTex.c_str());
            vkBindImageMemory(this->poDevice, image, imageMemory, 0);
        }
        void VulkanWindow::createVkImageView(const String& nameTex,
                                             VkImage image, 
                                             VkImageViewType type, 
                                             VkFormat format, 
                                             VkImageAspectFlags aspectFlags, 
                                             uint32_t mipMapCount,
                                             uint32_t numArray,
                                             VkImageView& imageView) 
        {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = type;
            viewInfo.format = format;
            viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = mipMapCount;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = numArray;

            if (vkCreateImageView(this->poDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImageView: Failed to create texture image view !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkImageViewName(this->poDevice, imageView, nameTex.c_str());
        }
        void VulkanWindow::createVkImageView(const String& nameTex,
                                             VkImageViewCreateFlags flags, 
                                             VkImage image, 
                                             VkImageViewType type, 
                                             VkFormat format, 
                                             VkComponentMapping componentMapping,
                                             VkImageAspectFlags aspectFlags, 
                                             uint32_t baseMipLevel,
                                             uint32_t mipMapCount,
                                             uint32_t baseArrayLayer,
                                             uint32_t numArray,
                                             VkImageView& imageView)
        {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.flags = flags;
            viewInfo.image = image;
            viewInfo.viewType = type;
            viewInfo.format = format;
            viewInfo.components = componentMapping;
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = baseMipLevel;
            viewInfo.subresourceRange.levelCount = mipMapCount;
            viewInfo.subresourceRange.baseArrayLayer = baseArrayLayer;
            viewInfo.subresourceRange.layerCount = numArray;

            if (vkCreateImageView(this->poDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImageView: Failed to create texture image view !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkImageViewName(this->poDevice, imageView, nameTex.c_str());
        }
        void VulkanWindow::createVkSampler(const String& nameTex,
                                           uint32_t mipMapCount, 
                                           VkSampler& sampler)
        {
            createVkSampler(nameTex,
                            F_TextureFilter_Bilinear,
                            F_TextureAddressing_Clamp,
                            F_TextureBorderColor_OpaqueBlack,
                            true,
                            this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                            0.0f,
                            static_cast<float>(mipMapCount),
                            0.0f,
                            sampler);
        }
        void VulkanWindow::createVkSampler(const String& nameTex,
                                           FTextureFilterType eTextureFilter,
                                           FTextureAddressingType eTextureAddressing,
                                           FTextureBorderColorType eTextureBorderColor,
                                           bool enableAnisotropy,
                                           float maxAnisotropy,
                                           float minLod, 
                                           float maxLod, 
                                           float mipLodBias,
                                           VkSampler& sampler)
        {
            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.minFilter = Util_Transform2VkFilter(eTextureFilter, F_TextureFilterSize_Min);
            samplerInfo.magFilter = Util_Transform2VkFilter(eTextureFilter, F_TextureFilterSize_Mag);
            samplerInfo.addressModeU = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.addressModeV = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.addressModeW = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.anisotropyEnable = enableAnisotropy ? VK_TRUE : VK_FALSE;
            samplerInfo.maxAnisotropy = maxAnisotropy > this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy ? this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy : maxAnisotropy;
            samplerInfo.borderColor = Util_Transform2VkBorderColor(eTextureBorderColor);
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = Util_Transform2VkSamplerMipmapMode(eTextureFilter);
            samplerInfo.minLod = minLod;
            samplerInfo.maxLod = maxLod;
            samplerInfo.mipLodBias = mipLodBias;

            if (vkCreateSampler(this->poDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkSampler: Failed to create texture sampler !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkSamplerName(this->poDevice, sampler, nameTex.c_str());
        }


        void VulkanWindow::transitionImageLayout(VkCommandBuffer cmdBuffer,
                                                 VkImage image, 
                                                 VkImageLayout oldLayout, 
                                                 VkImageLayout newLayout,
                                                 uint32_t mipBase,
                                                 uint32_t mipCount,
                                                 uint32_t numBase,
                                                 uint32_t numArray,
                                                 VkImageAspectFlags aspectFlags /*= VK_IMAGE_ASPECT_COLOR_BIT*/) 
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                VkImageMemoryBarrier barrier = {};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = oldLayout;
                barrier.newLayout = newLayout;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = image;

                barrier.subresourceRange.aspectMask = aspectFlags;
                barrier.subresourceRange.baseMipLevel = mipBase;
                barrier.subresourceRange.levelCount = mipCount;
                barrier.subresourceRange.baseArrayLayer = numBase;
                barrier.subresourceRange.layerCount = numArray;

                VkPipelineStageFlags sourceStage;
                VkPipelineStageFlags destinationStage;

                if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL)
                {
                    // VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_GENERAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = 0;

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL -> VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_VERTEX_SHADER_BIT | VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL -> VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = 0; 

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL) 
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL -> VK_IMAGE_LAYOUT_GENERAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = 0; 

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_GENERAL) 
                {
                    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL -> VK_IMAGE_LAYOUT_GENERAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_GENERAL -> VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL -> VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_GENERAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_GENERAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT; 

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT | VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
                } 
                else 
                {
                    throw std::invalid_argument("VulkanWindow::transitionImageLayout: Unsupported layout transition !");
                }

                pipelineBarrier(cmdBuffer,
                                sourceStage, 
                                destinationStage,
                                0,
                                0, 
                                nullptr,
                                0, 
                                nullptr,
                                1, 
                                &barrier);
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }
        void VulkanWindow::copyBufferToImage(VkCommandBuffer cmdBuffer,
                                             VkBuffer buffer, 
                                             VkImage image, 
                                             uint32_t width, 
                                             uint32_t height,
                                             uint32_t depth,
                                             uint32_t numArray) 
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                std::vector<VkBufferImageCopy> bufferCopyRegions;
                for (uint32_t i = 0; i < numArray; i++)
                {
                    VkBufferImageCopy region = {};
                    region.bufferOffset = width * height * 4 * i;
                    region.imageExtent.width = width;
                    region.imageExtent.height = height;
                    region.imageExtent.depth = depth;
                    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    region.imageSubresource.mipLevel = 0;
                    region.imageSubresource.baseArrayLayer = i;
                    region.imageSubresource.layerCount = 1;
                    bufferCopyRegions.push_back(region);
                }
                vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, (uint32_t)bufferCopyRegions.size(), bufferCopyRegions.data());
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }
        void VulkanWindow::generateMipMaps(VkCommandBuffer cmdBuffer,
                                           VkImage image, 
                                           VkFormat imageFormat, 
                                           VkImageLayout finalLayout,
                                           int32_t width, 
                                           int32_t height, 
                                           uint32_t mipMapCount,
                                           uint32_t numArray,
                                           bool autoMipMap)
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                if (autoMipMap)
                {
                    int32_t mipWidth = width;
                    int32_t mipHeight = height;
                    for (uint32_t i = 1; i < mipMapCount; i++) 
                    {
                        transitionImageLayout(cmdBuffer,
                                              image, 
                                              VK_IMAGE_LAYOUT_UNDEFINED, 
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              i,
                                              1,
                                              0,
                                              numArray);
                        {
                            VkImageBlit blit = {};
                            blit.srcOffsets[0] = {0, 0, 0};
                            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
                            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                            blit.srcSubresource.mipLevel = i - 1;
                            blit.srcSubresource.baseArrayLayer = 0;
                            blit.srcSubresource.layerCount = numArray;

                            blit.dstOffsets[0] = {0, 0, 0};
                            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                            blit.dstSubresource.mipLevel = i;
                            blit.dstSubresource.baseArrayLayer = 0;
                            blit.dstSubresource.layerCount = numArray;

                            vkCmdBlitImage(cmdBuffer,
                                           image, 
                                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                           image, 
                                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                           1, 
                                           &blit,
                                           VK_FILTER_LINEAR);

                            if (mipWidth > 1) 
                                mipWidth /= 2;
                            if (mipHeight > 1) 
                                mipHeight /= 2;
                        }   
                        transitionImageLayout(cmdBuffer,
                                              image,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                              i,
                                              1,
                                              0,
                                              numArray);
                    }
                }

                transitionImageLayout(cmdBuffer,
                                      image,
                                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                      finalLayout,
                                      0,
                                      mipMapCount,
                                      0,
                                      numArray);
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }

    void VulkanWindow::createConstBuffers()
    {
        F_LogInfo("**<2-1-3> VulkanWindow::createConstBuffers start **");
        {
            //1> createObjectCB
            createObjectCB();

            //2> createMaterialCB
            createMaterialCB();

            //3> createInstanceCB
            createInstanceCB();

            //4> createCustomCB
            createCustomCB();
        }
        F_LogInfo("**<2-1-3> VulkanWindow::createConstBuffers finish **");
    }
    void VulkanWindow::createObjectCB()
    {
        buildObjectCB();
        VkDeviceSize bufferSize = sizeof(ObjectConstants) * this->objectCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_ObjectCB.resize(count);
        this->poBuffersMemory_ObjectCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            String nameBuffer = "Object-" + FUtilString::SaveSizeT(i);
            createVkBuffer(nameBuffer,
                           bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_ObjectCB[i], 
                           this->poBuffersMemory_ObjectCB[i]);
        }
        F_LogInfo("<2-1-3-1> VulkanWindow::createObjectCB finish !");
    }
        void VulkanWindow::buildObjectCB()
        {
            ObjectConstants objectConstants;
            this->objectCBs.push_back(objectConstants);
        }
    void VulkanWindow::createMaterialCB()
    {
        buildMaterialCB();
        VkDeviceSize bufferSize = sizeof(MaterialConstants) * this->materialCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_MaterialCB.resize(count);
        this->poBuffersMemory_MaterialCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            String nameBuffer = "Material-" + FUtilString::SaveSizeT(i);
            createVkBuffer(nameBuffer,
                           bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_MaterialCB[i], 
                           this->poBuffersMemory_MaterialCB[i]);
        }
        F_LogInfo("<2-1-3-2> VulkanWindow::createMaterialCB finish !");
    }
        void VulkanWindow::buildMaterialCB()
        {
            MaterialConstants materialConstants;
            this->materialCBs.push_back(materialConstants);
        }
    void VulkanWindow::createInstanceCB()
    {
        buildInstanceCB();
        VkDeviceSize bufferSize = sizeof(InstanceConstants) * this->instanceCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_InstanceCB.resize(count);
        this->poBuffersMemory_InstanceCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            String nameBuffer = "Instance-" + FUtilString::SaveSizeT(i);
            createVkBuffer(nameBuffer,
                           bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_InstanceCB[i], 
                           this->poBuffersMemory_InstanceCB[i]);
        }
        F_LogInfo("<2-1-3-3> VulkanWindow::createInstanceCB finish !");
    }
        void VulkanWindow::buildInstanceCB()
        {
            InstanceConstants instanceConstants;
            this->instanceCBs.push_back(instanceConstants);
        }
    void VulkanWindow::createCustomCB()
    {
        
        F_LogInfo("<2-1-3-4> VulkanWindow::createCustomCB finish !");
    }

    VkShaderModule VulkanWindow::createVkShaderModule(const String& nameShader, FShaderType typeShader, const String& pathFile)
    {
        const String& strTypeShader = F_GetShaderTypeName(typeShader);
        return createVkShaderModule(nameShader, strTypeShader, pathFile);
    }
    VkShaderModule VulkanWindow::createVkShaderModule(const String& nameShader, const String& strTypeShader, const String& pathFile)
    {
        if (pathFile.empty())
            return nullptr;

        CharVector code;
        if (!FUtil::LoadAssetFileContent(pathFile.c_str(), code))
        {
            F_LogError("*********************** VulkanWindow::createVkShaderModule failed, path: [%s] !", pathFile.c_str());
            return nullptr;
        }
        if (code.size() <= 0)
        {
            return nullptr;
        }

        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(this->poDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createVkShaderModule: Failed to create shader module: type: " + strTypeShader + ", name: " + nameShader;
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->poDebug->SetVkShaderModuleName(this->poDevice, shaderModule, nameShader.c_str());

        return shaderModule;
    }
        void VulkanWindow::destroyVkShaderModule(VkShaderModule vkShaderModule)
        {
            if (vkShaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(this->poDevice, vkShaderModule, nullptr);
            }
        }

    bool VulkanWindow::createVkDescriptorSetLayout(const String& nameDescriptorSetLayout, const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(aDescriptorSetLayoutBinding.size());
        layoutInfo.pBindings = aDescriptorSetLayoutBinding.data();
        if (vkCreateDescriptorSetLayout(this->poDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanWindow::createVkDescriptorSetLayout: Failed to create descriptor set layout !");
            return false;
        }
        this->poDebug->SetVkDescriptorSetLayoutName(this->poDevice, vkDescriptorSetLayout, nameDescriptorSetLayout.c_str());
        return true;
    }
        void VulkanWindow::destroyVkDescriptorSetLayout(VkDescriptorSetLayout vkDescriptorSetLayout)
        {
            if (vkDescriptorSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(this->poDevice, vkDescriptorSetLayout, nullptr);
            }
        }


    VkPipelineLayout VulkanWindow::createVkPipelineLayout(const String& namePipelineLayout, const VkDescriptorSetLayoutVector& aDescriptorSetLayout)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(aDescriptorSetLayout.size());
        pipelineLayoutInfo.pSetLayouts = aDescriptorSetLayout.data();

        VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
        if (vkCreatePipelineLayout(this->poDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createVkPipelineLayout: Failed to create pipeline layout !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        this->poDebug->SetVkPipelineLayoutName(this->poDevice, vkPipelineLayout, namePipelineLayout.c_str());
        return vkPipelineLayout;
    }
        void VulkanWindow::destroyVkPipelineLayout(VkPipelineLayout vkPipelineLayout)
        {
            if (vkPipelineLayout != VK_NULL_HANDLE)
            {
                vkDestroyPipelineLayout(this->poDevice, vkPipelineLayout, nullptr);
            }
        }
        void VulkanWindow::destroyVkPipeline(VkPipeline vkPipeline)
        {
            if (vkPipeline != VK_NULL_HANDLE)
            {
                vkDestroyPipeline(this->poDevice, vkPipeline, nullptr);   
            }
        }

    void VulkanWindow::createVkPipelineCache()
    {
        if (this->poPipelineCache == VK_NULL_HANDLE)
        {
            VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
            pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            if (vkCreatePipelineCache(this->poDevice, &pipelineCacheCreateInfo, nullptr, &this->poPipelineCache) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkPipelineCache: Failed to create pipeline cache !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            this->poDebug->SetVkPipelineCacheName(this->poDevice, this->poPipelineCache, "PipelineCache");
        }
    }
        void VulkanWindow::destroyVkPipelineCache(VkPipelineCache vkPipelineCache)
        {
            if (vkPipelineCache != VK_NULL_HANDLE)
            {
                vkDestroyPipelineCache(this->poDevice, vkPipelineCache, nullptr);
            }
        }

    void VulkanWindow::createCustomBeforePipeline()
    {
        F_LogInfo("**<2-1-4> VulkanWindow::createCustomBeforePipeline finish **");
    }
    void VulkanWindow::createGraphicsPipeline()
    {
        F_LogInfo("**<2-1-5> VulkanWindow::createGraphicsPipeline start **");
        {
            //1> createGraphicsPipeline_Default
            createGraphicsPipeline_Default();
            F_LogInfo("<2-1-5-1> VulkanWindow::createGraphicsPipeline: createGraphicsPipeline_Default finish !");

            //2> createGraphicsPipeline_Custom
            createGraphicsPipeline_Custom();
            F_LogInfo("<2-1-5-2> VulkanWindow::createGraphicsPipeline: createGraphicsPipeline_Custom finish !");
        }
        F_LogInfo("**<2-1-5> VulkanWindow::createGraphicsPipeline finish **");
    }
        void VulkanWindow::createGraphicsPipeline_Default()
        {
            if (this->cfg_shaderVertex_Path.empty() ||
                this->cfg_shaderFragment_Path.empty() ||
                this->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                return;
            }

            //1> Shader
            String nameVertexShader;
            String namePathBase;
            FUtilString::SplitFileName(this->cfg_shaderVertex_Path, nameVertexShader, namePathBase);
            VkShaderModule vertShaderModule = createVkShaderModule(nameVertexShader, "VertexShader: ", this->cfg_shaderVertex_Path);
            if (vertShaderModule == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createGraphicsPipeline_Default: Failed to create shader module: " + this->cfg_shaderVertex_Path;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            
            String nameFragmentShader;
            FUtilString::SplitFileName(this->cfg_shaderFragment_Path, nameFragmentShader, namePathBase);
            VkShaderModule fragShaderModule = createVkShaderModule(nameFragmentShader, "FragmentShader: ", this->cfg_shaderFragment_Path);
            if (fragShaderModule == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createGraphicsPipeline_Default: Failed to create shader module: " + this->cfg_shaderFragment_Path;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            //2> Viewport
            VkViewportVector aViewports;
            aViewports.push_back(this->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(this->poScissor);

            //3> PipelineLayout
            VkDescriptorSetLayoutVector aDescriptorSetLayout;
            aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
            this->poPipelineLayout = createVkPipelineLayout("PipelineLayout-Default", aDescriptorSetLayout);
            if (this->poPipelineLayout == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkPipelineLayout failed !");
                return;
            }

            //4> poPipelineGraphics
            this->poPipelineGraphics = createVkGraphicsPipeline("PipelineGraphics-Default",
                                                                vertShaderModule, "main",
                                                                fragShaderModule, "main",
                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                this->cfg_vkPrimitiveTopology, this->cfg_vkFrontFace, this->cfg_vkPolygonMode, this->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                this->cfg_isDepthTest, this->cfg_isDepthWrite,this->cfg_DepthCompareOp,
                                                                this->cfg_isStencilTest, this->cfg_StencilOpFront, this->cfg_StencilOpBack, 
                                                                this->cfg_isBlend, this->cfg_BlendColorFactorSrc, this->cfg_BlendColorFactorDst, this->cfg_BlendColorOp,
                                                                this->cfg_BlendAlphaFactorSrc, this->cfg_BlendAlphaFactorDst, this->cfg_BlendAlphaOp,
                                                                this->cfg_ColorWriteMask);
            if (this->poPipelineGraphics == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkGraphicsPipeline failed !");
                return;
            }

            //5> poPipelineGraphics_WireFrame
            this->poPipelineGraphics_WireFrame = createVkGraphicsPipeline("PipelineGraphics-Wire-Default",
                                                                          vertShaderModule, "main",
                                                                          fragShaderModule, "main",
                                                                          Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                          Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                          this->poRenderPass, this->poPipelineLayout, aViewports, aScissors, this->cfg_aDynamicStates,
                                                                          this->cfg_vkPrimitiveTopology, this->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, this->cfg_vkCullModeFlagBits, this->cfg_isDepthBiasEnable, this->cfg_DepthBiasConstantFactor, this->cfg_DepthBiasClamp, this->cfg_DepthBiasSlopeFactor, this->cfg_LineWidth,
                                                                          this->cfg_isDepthTest, this->cfg_isDepthWrite,this->cfg_DepthCompareOp,
                                                                          this->cfg_isStencilTest, this->cfg_StencilOpFront, this->cfg_StencilOpBack, 
                                                                          this->cfg_isBlend, this->cfg_BlendColorFactorSrc, this->cfg_BlendColorFactorDst, this->cfg_BlendColorOp,
                                                                          this->cfg_BlendAlphaFactorSrc, this->cfg_BlendAlphaFactorDst, this->cfg_BlendAlphaOp,
                                                                          this->cfg_ColorWriteMask);
            if (this->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
            {
                F_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkGraphicsPipeline wire frame failed !");
                return;
            }

            //6> Destroy Shader
            destroyVkShaderModule(fragShaderModule);
            destroyVkShaderModule(vertShaderModule);
        }
        void VulkanWindow::createGraphicsPipeline_Custom()
        {
            
        }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                              VkShaderModule vertShaderModule, const String& vertMain,
                                                              VkShaderModule fragShaderModule, const String& fragMain,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask, uint32_t subpass /*= 0*/)
            {
                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;
                //1> Pipeline Shader Stage
                //vert
                VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
                vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertShaderStageInfo.module = vertShaderModule;
                vertShaderStageInfo.pName = vertMain.c_str();
                aShaderStageCreateInfos.push_back(vertShaderStageInfo);

                //frag
                VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
                fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragShaderStageInfo.module = fragShaderModule;
                fragShaderStageInfo.pName = fragMain.c_str();
                aShaderStageCreateInfos.push_back(fragShaderStageInfo);

                return createVkGraphicsPipeline(nameGraphicsPipeline,
                                                aShaderStageCreateInfos,
                                                false, 0, 0,
                                                pBindingDescriptions,
                                                pAttributeDescriptions,
                                                renderPass, pipelineLayout, aViewports, aScissors, aDynamicStates,
                                                primitiveTopology, frontFace, polygonMode, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
                                                bDepthTest, bDepthWrite, depthCompareOp,
                                                bStencilTest, stencilOpFront, stencilOpBack,
                                                bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                                blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                                colorWriteMask, subpass);
            }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                              VkShaderModule vertShaderModule, const String& vertMain,
                                                              VkShaderModule tescShaderModule, const String& tescMain,
                                                              VkShaderModule teseShaderModule, const String& teseMain,
                                                              VkShaderModule fragShaderModule, const String& fragMain,
                                                              VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask, uint32_t subpass /*= 0*/)
            {
                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;
                //1> Pipeline Shader Stage
                //vert
                VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
                vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertShaderStageInfo.module = vertShaderModule;
                vertShaderStageInfo.pName = vertMain.c_str();
                aShaderStageCreateInfos.push_back(vertShaderStageInfo);

                //tesc
                VkPipelineShaderStageCreateInfo tescShaderStageInfo = {};
                tescShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                tescShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                tescShaderStageInfo.module = tescShaderModule;
                tescShaderStageInfo.pName = tescMain.c_str();
                aShaderStageCreateInfos.push_back(tescShaderStageInfo);

                //tese
                VkPipelineShaderStageCreateInfo teseShaderStageInfo = {};
                teseShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                teseShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                teseShaderStageInfo.module = teseShaderModule;
                teseShaderStageInfo.pName = teseMain.c_str();
                aShaderStageCreateInfos.push_back(teseShaderStageInfo);

                //frag
                VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
                fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragShaderStageInfo.module = fragShaderModule;
                fragShaderStageInfo.pName = fragMain.c_str();
                aShaderStageCreateInfos.push_back(fragShaderStageInfo);

                return createVkGraphicsPipeline(nameGraphicsPipeline,
                                                aShaderStageCreateInfos,
                                                true, tessellationFlags, tessellationPatchControlPoints,
                                                pBindingDescriptions,
                                                pAttributeDescriptions,
                                                renderPass, pipelineLayout, aViewports, aScissors, aDynamicStates,
                                                primitiveTopology, frontFace, polygonMode, cullMode, depthBiasEnable, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor, lineWidth,
                                                bDepthTest, bDepthWrite, depthCompareOp,
                                                bStencilTest, stencilOpFront, stencilOpBack,
                                                bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                                blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                                colorWriteMask, subpass);
            }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                              const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                              bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask, uint32_t subpass /*= 0*/)
            {
                //1> Pipeline VertexInput State
                VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
                vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                if (pBindingDescriptions != nullptr)
                {   
                    vertexInputStateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(pBindingDescriptions->size());
                    vertexInputStateInfo.pVertexBindingDescriptions = pBindingDescriptions->data();
                }
                if (pAttributeDescriptions != nullptr)
                {
                    vertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                    vertexInputStateInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                }

                //2> Pipeline InputAssembly
                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
                inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateInfo.topology = primitiveTopology;
                inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

                //3> Pipeline Viewport State
                VkPipelineViewportStateCreateInfo viewportStateInfo = {};
                viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateInfo.viewportCount = static_cast<uint32_t>(aViewports.size());
                viewportStateInfo.pViewports = aViewports.data();
                viewportStateInfo.scissorCount = static_cast<uint32_t>(aScissors.size());
                viewportStateInfo.pScissors = aScissors.data();

                //4> Pipeline Rasterization State
                VkPipelineRasterizationStateCreateInfo rasterizationStateInfo = {};
                rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateInfo.polygonMode = polygonMode;
                rasterizationStateInfo.cullMode = cullMode;
                rasterizationStateInfo.frontFace = frontFace;
                rasterizationStateInfo.depthBiasEnable = depthBiasEnable;
                rasterizationStateInfo.depthBiasConstantFactor = depthBiasConstantFactor;
                rasterizationStateInfo.depthBiasClamp = depthBiasClamp; 
                rasterizationStateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
                rasterizationStateInfo.lineWidth = lineWidth;

                //5> Pipeline Multisample State
                VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
                multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
                multisamplingStateInfo.rasterizationSamples = IsEnable_MASS() ? this->poMSAASamples : VK_SAMPLE_COUNT_1_BIT;
                multisamplingStateInfo.minSampleShading = 1.0f; // Optional
                multisamplingStateInfo.pSampleMask = nullptr; // Optional
                multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
                multisamplingStateInfo.alphaToOneEnable = VK_FALSE; // Optional

                //6> Pipeline DepthStencil State
                VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
                depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateInfo.depthTestEnable = bDepthTest;
                depthStencilStateInfo.depthWriteEnable = bDepthWrite;
                depthStencilStateInfo.depthCompareOp = depthCompareOp;
                depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateInfo.stencilTestEnable = bStencilTest;
                if (bStencilTest)
                {
                    depthStencilStateInfo.front = stencilOpFront;
                    depthStencilStateInfo.back = stencilOpBack;
                }

                //7> Pipeline ColorBlend State 
                VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
                colorBlendAttachment.blendEnable = bBlend;
                colorBlendAttachment.colorWriteMask = colorWriteMask;
                if (bBlend)
                {
                    colorBlendAttachment.srcColorBlendFactor = blendColorFactorSrc;
                    colorBlendAttachment.dstColorBlendFactor = blendColorFactorDst;
                    colorBlendAttachment.colorBlendOp = blendColorOp;
                    colorBlendAttachment.srcAlphaBlendFactor = blendAlphaFactorSrc;
                    colorBlendAttachment.dstAlphaBlendFactor = blendAlphaFactorDst;
                    colorBlendAttachment.alphaBlendOp = blendAlphaOp;
                }
                
                VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
                colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlendingStateInfo.logicOpEnable = VK_FALSE;
                colorBlendingStateInfo.logicOp = VK_LOGIC_OP_COPY;
                colorBlendingStateInfo.attachmentCount = 1;
                colorBlendingStateInfo.pAttachments = &colorBlendAttachment;
                colorBlendingStateInfo.blendConstants[0] = 0.0f;
                colorBlendingStateInfo.blendConstants[1] = 0.0f;
                colorBlendingStateInfo.blendConstants[2] = 0.0f;
                colorBlendingStateInfo.blendConstants[3] = 0.0f;

                //8> Pipeline Dynamic State
                VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
                dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicStateInfo.pDynamicStates = aDynamicStates.data();
                dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(aDynamicStates.size());
                dynamicStateInfo.flags = 0;

                //9> Tessellation State
                VkPipelineTessellationStateCreateInfo tessellationState = {};
                tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
                tessellationState.flags = tessellationFlags;
                tessellationState.patchControlPoints = tessellationPatchControlPoints;

                //10> Graphics Pipeline
                VkGraphicsPipelineCreateInfo pipelineInfo = {};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.pNext = nullptr;
                pipelineInfo.stageCount = static_cast<uint32_t>(aShaderStageCreateInfos.size());
                pipelineInfo.pStages = aShaderStageCreateInfos.data();
                pipelineInfo.pVertexInputState = &vertexInputStateInfo;
                pipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
                pipelineInfo.pTessellationState = tessellationIsUsed ? &tessellationState : nullptr;
                pipelineInfo.pViewportState = &viewportStateInfo;
                pipelineInfo.pRasterizationState = &rasterizationStateInfo;
                pipelineInfo.pMultisampleState = &multisamplingStateInfo;
                pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
                pipelineInfo.pColorBlendState = &colorBlendingStateInfo;
                pipelineInfo.pDynamicState = &dynamicStateInfo;
                pipelineInfo.layout = pipelineLayout;
                pipelineInfo.renderPass = renderPass;
                pipelineInfo.subpass = subpass;
                pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
                pipelineInfo.basePipelineIndex = 0;

                VkPipeline pipeline;
                if (!Util_CheckVkResult(vkCreateGraphicsPipelines(this->poDevice, this->poPipelineCache, 1, &pipelineInfo, nullptr, &pipeline), "vkCreateGraphicsPipelines"))
                {
                    F_LogError("*********************** VulkanWindow::createVkGraphicsPipeline: vkCreateGraphicsPipelines failed !");
                    return VK_NULL_HANDLE;
                }
                this->poDebug->SetVkPipelineName(this->poDevice, pipeline, nameGraphicsPipeline.c_str());
                return pipeline;
            }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                              const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                              bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              const VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentState, uint32_t subpass /*= 0*/)
            {
                //1> Pipeline VertexInput State
                VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
                vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                if (pBindingDescriptions != nullptr)
                {   
                    vertexInputStateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(pBindingDescriptions->size());
                    vertexInputStateInfo.pVertexBindingDescriptions = pBindingDescriptions->data();
                }
                if (pAttributeDescriptions != nullptr)
                {
                    vertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                    vertexInputStateInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                }

                //2> Pipeline InputAssembly
                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
                inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateInfo.topology = primitiveTopology;
                inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

                //3> Pipeline Viewport State
                VkPipelineViewportStateCreateInfo viewportStateInfo = {};
                viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateInfo.viewportCount = static_cast<uint32_t>(aViewports.size());
                viewportStateInfo.pViewports = aViewports.data();
                viewportStateInfo.scissorCount = static_cast<uint32_t>(aScissors.size());
                viewportStateInfo.pScissors = aScissors.data();

                //4> Pipeline Rasterization State
                VkPipelineRasterizationStateCreateInfo rasterizationStateInfo = {};
                rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateInfo.polygonMode = polygonMode;
                rasterizationStateInfo.cullMode = cullMode;
                rasterizationStateInfo.frontFace = frontFace;
                rasterizationStateInfo.depthBiasEnable = depthBiasEnable;
                rasterizationStateInfo.depthBiasConstantFactor = depthBiasConstantFactor;
                rasterizationStateInfo.depthBiasClamp = depthBiasClamp; 
                rasterizationStateInfo.depthBiasSlopeFactor = depthBiasSlopeFactor;
                rasterizationStateInfo.lineWidth = lineWidth;

                //5> Pipeline Multisample State
                VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
                multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
                multisamplingStateInfo.rasterizationSamples = IsEnable_MASS() ? this->poMSAASamples : VK_SAMPLE_COUNT_1_BIT;
                multisamplingStateInfo.minSampleShading = 1.0f; // Optional
                multisamplingStateInfo.pSampleMask = nullptr; // Optional
                multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
                multisamplingStateInfo.alphaToOneEnable = VK_FALSE; // Optional

                //6> Pipeline DepthStencil State
                VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
                depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateInfo.depthTestEnable = bDepthTest;
                depthStencilStateInfo.depthWriteEnable = bDepthWrite;
                depthStencilStateInfo.depthCompareOp = depthCompareOp;
                depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateInfo.stencilTestEnable = bStencilTest;
                if (bStencilTest)
                {
                    depthStencilStateInfo.front = stencilOpFront;
                    depthStencilStateInfo.back = stencilOpBack;
                }

                //7> Pipeline ColorBlend State 
                VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
                colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlendingStateInfo.logicOpEnable = VK_FALSE;
                colorBlendingStateInfo.logicOp = VK_LOGIC_OP_COPY;
                colorBlendingStateInfo.attachmentCount = static_cast<uint32_t>(aColorBlendAttachmentState.size());
                colorBlendingStateInfo.pAttachments = aColorBlendAttachmentState.data();
                colorBlendingStateInfo.blendConstants[0] = 0.0f;
                colorBlendingStateInfo.blendConstants[1] = 0.0f;
                colorBlendingStateInfo.blendConstants[2] = 0.0f;
                colorBlendingStateInfo.blendConstants[3] = 0.0f;

                //8> Pipeline Dynamic State
                VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
                dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicStateInfo.pDynamicStates = aDynamicStates.data();
                dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(aDynamicStates.size());
                dynamicStateInfo.flags = 0;

                //9> Tessellation State
                VkPipelineTessellationStateCreateInfo tessellationState = {};
                tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
                tessellationState.flags = tessellationFlags;
                tessellationState.patchControlPoints = tessellationPatchControlPoints;

                //10> Graphics Pipeline
                VkGraphicsPipelineCreateInfo pipelineInfo = {};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.pNext = nullptr;
                pipelineInfo.stageCount = static_cast<uint32_t>(aShaderStageCreateInfos.size());
                pipelineInfo.pStages = aShaderStageCreateInfos.data();
                pipelineInfo.pVertexInputState = &vertexInputStateInfo;
                pipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
                pipelineInfo.pTessellationState = tessellationIsUsed ? &tessellationState : nullptr;
                pipelineInfo.pViewportState = &viewportStateInfo;
                pipelineInfo.pRasterizationState = &rasterizationStateInfo;
                pipelineInfo.pMultisampleState = &multisamplingStateInfo;
                pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
                pipelineInfo.pColorBlendState = &colorBlendingStateInfo;
                pipelineInfo.pDynamicState = &dynamicStateInfo;
                pipelineInfo.layout = pipelineLayout;
                pipelineInfo.renderPass = renderPass;
                pipelineInfo.subpass = subpass;
                pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
                pipelineInfo.basePipelineIndex = 0;

                VkPipeline pipeline;
                if (!Util_CheckVkResult(vkCreateGraphicsPipelines(this->poDevice, this->poPipelineCache, 1, &pipelineInfo, nullptr, &pipeline), "vkCreateGraphicsPipelines"))
                {
                    F_LogError("*********************** VulkanWindow::createVkGraphicsPipeline: vkCreateGraphicsPipelines failed !");
                    return VK_NULL_HANDLE;
                }
                this->poDebug->SetVkPipelineName(this->poDevice, pipeline, nameGraphicsPipeline.c_str());
                return pipeline;
            }

    void VulkanWindow::createComputePipeline()
    {
        F_LogInfo("**<2-1-6> VulkanWindow::createComputePipeline start **");
        {
            //1> createComputePipeline_Default
            createComputePipeline_Default();

            //2> createComputePipeline_Custom
            createComputePipeline_Custom();
        }
        F_LogInfo("**<2-1-6> VulkanWindow::createComputePipeline finish **");
    }
        void VulkanWindow::createComputePipeline_Default()
        {

        }
        void VulkanWindow::createComputePipeline_Custom()
        {

        }
            VkPipeline VulkanWindow::createVkComputePipeline(const String& nameComputePipeline,
                                                             VkShaderModule compShaderModule,
                                                             const String& compMain,
                                                             VkPipelineLayout pipelineLayout, 
                                                             VkPipelineCreateFlags flags /*= 0*/,
                                                             VkSpecializationInfo* pSpecializationInfo /*= nullptr*/)
            {
                VkPipelineShaderStageCreateInfo compShaderStageInfo = {};
                compShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                compShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
                compShaderStageInfo.module = compShaderModule;
                compShaderStageInfo.pName = compMain.c_str();
                compShaderStageInfo.pSpecializationInfo = pSpecializationInfo;

                return createVkComputePipeline(nameComputePipeline,
                                               compShaderStageInfo,
                                               pipelineLayout,
                                               flags);
            }
            VkPipeline VulkanWindow::createVkComputePipeline(const String& nameComputePipeline,
                                                             const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                             VkPipelineLayout pipelineLayout, 
                                                             VkPipelineCreateFlags flags /*= 0*/)
            {
                VkComputePipelineCreateInfo pipelineInfo = {};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
                pipelineInfo.pNext = nullptr;
                pipelineInfo.flags = flags; 
                pipelineInfo.stage = shaderStageCreateInfo;
                pipelineInfo.layout = pipelineLayout;
			    
                VkPipeline pipeline;
                if (!Util_CheckVkResult(vkCreateComputePipelines(this->poDevice, this->poPipelineCache, 1, &pipelineInfo, nullptr, &pipeline), "vkCreateComputePipelines"))
                {
                    F_LogError("*********************** VulkanWindow::createVkComputePipeline: vkCreateComputePipelines failed !");
                    return VK_NULL_HANDLE;
                }
                this->poDebug->SetVkPipelineName(this->poDevice, pipeline, nameComputePipeline.c_str());
                return pipeline;
            }


    void VulkanWindow::createDescriptorSets()
    {
        F_LogInfo("**<2-1-7> VulkanWindow::createDescriptorSets start **");
        {
            //1> createDescriptorSets_Default
            createDescriptorSets_Default();
            F_LogInfo("<2-1-7-1> VulkanWindow::createDescriptorSets: createDescriptorSets_Default finish !");

            //2> createDescriptorSets_Terrain
            createDescriptorSets_Terrain();
            F_LogInfo("<2-1-7-2> VulkanWindow::createDescriptorSets: createDescriptorSets_Terrain finish !");

            //3> createDescriptorSets_Custom
            createDescriptorSets_Custom();
            F_LogInfo("<2-1-7-3> VulkanWindow::createDescriptorSets: createDescriptorSets_Custom finish !");
        }
        F_LogInfo("**<2-1-7> VulkanWindow::createDescriptorSets finish **");
    }
        void VulkanWindow::createDescriptorSets_Default()
        {
            if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
                return;

            createVkDescriptorSets("DescriptorSets-Default", this->poDescriptorSetLayout, this->poDescriptorSets);
            updateDescriptorSets(this->poDescriptorSets, this->poTextureImageView, this->poTextureSampler);
        }
        void VulkanWindow::createDescriptorSets_Terrain()
        {
        }
        void VulkanWindow::createDescriptorSets_Custom()
        {   
            
        }
            void VulkanWindow::updateDescriptorSets(VkDescriptorSetVector& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler)
            {
                size_t count = aDescriptorSets.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkWriteDescriptorSetVector descriptorWrites;
                    //(0) PassConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = this->poBuffers_PassCB[i];
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    0,
                                                    0,
                                                    1,
                                                    bufferInfo_Pass);
                    }
                    //(1) ObjectConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = this->poBuffers_ObjectCB[i];
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * this->objectCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    1,
                                                    0,
                                                    1,
                                                    bufferInfo_Object);
                    }
                    //(2) MaterialConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Material = {};
                        bufferInfo_Material.buffer = this->poBuffers_MaterialCB[i];
                        bufferInfo_Material.offset = 0;
                        bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    2,
                                                    0,
                                                    1,
                                                    bufferInfo_Material);
                    }
                    //(3) InstanceConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Instance = {};
                        bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
                        bufferInfo_Instance.offset = 0;
                        bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    3,
                                                    0,
                                                    1,
                                                    bufferInfo_Instance);
                    }
                    //(4) Image
                    if (vkTextureView != VK_NULL_HANDLE)
                    {
                        VkDescriptorImageInfo imageInfo = {};
                        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        imageInfo.imageView = vkTextureView;
                        imageInfo.sampler = vkSampler;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  aDescriptorSets[i],
                                                  4,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  imageInfo);
                    }
                    updateVkDescriptorSets(descriptorWrites);
                }
            }

            void VulkanWindow::destroyVkDescriptorSet(VkDescriptorSet& vkDescriptorSet)
            {
                if (vkDescriptorSet != VK_NULL_HANDLE)
                {
                    vkFreeDescriptorSets(this->poDevice, this->poDescriptorPool, 1, &vkDescriptorSet);
                }
            }
            void VulkanWindow::destroyVkDescriptorSets(VkDescriptorSetVector& aDescriptorSets)
            {
                uint32_t count = (uint32_t)aDescriptorSets.size();
                if (count <= 0)
                    return;
                
                vkFreeDescriptorSets(this->poDevice, this->poDescriptorPool, count, aDescriptorSets.data());
            }
            void VulkanWindow::createVkDescriptorSet(const String& nameDescriptorSet, VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSet& vkDescriptorSet)
            {
                VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = this->poDescriptorPool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = &vkDescriptorSetLayout;

                if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createVkDescriptorSet: Failed to allocate descriptor set !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                this->poDebug->SetVkDescriptorSetName(this->poDevice, vkDescriptorSet, nameDescriptorSet.c_str());
            }
            void VulkanWindow::createVkDescriptorSets(const String& nameDescriptorSets, VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSetVector& aDescriptorSets)
            {
                std::vector<VkDescriptorSetLayout> layouts(this->poSwapChainImages.size(), vkDescriptorSetLayout);
                VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = this->poDescriptorPool;
                allocInfo.descriptorSetCount = static_cast<uint32_t>(this->poSwapChainImages.size());
                allocInfo.pSetLayouts = layouts.data();

                size_t count = this->poSwapChainImages.size();
                aDescriptorSets.resize(count);
                if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, aDescriptorSets.data()) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createVkDescriptorSets: Failed to allocate descriptor sets !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                for (size_t i = 0; i < count; i++)
                {
                    String name = nameDescriptorSets + FUtilString::SaveSizeT(i);
                    this->poDebug->SetVkDescriptorSetName(this->poDevice, aDescriptorSets[i], name.c_str());
                }
            }
            void VulkanWindow::createVkDescriptorSets(const String& nameDescriptorSets, VkDescriptorSetLayout vkDescriptorSetLayout, int countDescriptorSets, VkDescriptorSetVector& aDescriptorSets)
            {
                std::vector<VkDescriptorSetLayout> layouts(countDescriptorSets, vkDescriptorSetLayout);
                VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = this->poDescriptorPool;
                allocInfo.descriptorSetCount = static_cast<uint32_t>(countDescriptorSets);
                allocInfo.pSetLayouts = layouts.data();

                size_t count = (size_t)countDescriptorSets;
                aDescriptorSets.resize(count);
                if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, aDescriptorSets.data()) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createVkDescriptorSets: Failed to allocate descriptor sets !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                for (size_t i = 0; i < count; i++)
                {
                    String name = nameDescriptorSets + FUtilString::SaveSizeT(i);
                    this->poDebug->SetVkDescriptorSetName(this->poDevice, aDescriptorSets[i], name.c_str());
                }
            }

            VkDescriptorSetLayoutBinding VulkanWindow::createVkDescriptorSetLayoutBinding_Buffer(uint32_t binding,
                                                                                                 VkDescriptorType descriptorType,
                                                                                                 uint32_t descriptorCount,
                                                                                                 VkShaderStageFlags stageFlags)
            {
                VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
                createVkDescriptorSetLayoutBinding_Buffer(descriptorSetLayoutBinding,
                                                          binding,
                                                          descriptorType,
                                                          descriptorCount,
                                                          stageFlags);
                return descriptorSetLayoutBinding;
            }
            void VulkanWindow::createVkDescriptorSetLayoutBinding_Buffer(VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding,
                                                                         uint32_t binding,
                                                                         VkDescriptorType descriptorType,
                                                                         uint32_t descriptorCount,
                                                                         VkShaderStageFlags stageFlags)
            {
                descriptorSetLayoutBinding.binding = binding;
                descriptorSetLayoutBinding.descriptorType = descriptorType;
                descriptorSetLayoutBinding.descriptorCount = descriptorCount;
                descriptorSetLayoutBinding.stageFlags = stageFlags;
                descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
            }
            VkDescriptorSetLayoutBinding VulkanWindow::createVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                                                                VkDescriptorType descriptorType,
                                                                                                uint32_t descriptorCount,
                                                                                                VkShaderStageFlags stageFlags,
                                                                                                VkSampler* pImmutableSamplers)
            {
                VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
                createVkDescriptorSetLayoutBinding_Image(descriptorSetLayoutBinding,
                                                         binding,
                                                         descriptorType,
                                                         descriptorCount,
                                                         stageFlags,
                                                         pImmutableSamplers);
                return descriptorSetLayoutBinding;
            }
            void VulkanWindow::createVkDescriptorSetLayoutBinding_Image(VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding,
                                                                        uint32_t binding,
                                                                        VkDescriptorType descriptorType,
                                                                        uint32_t descriptorCount,
                                                                        VkShaderStageFlags stageFlags,
                                                                        VkSampler* pImmutableSamplers)
            {
                descriptorSetLayoutBinding.binding = binding;
                descriptorSetLayoutBinding.descriptorType = descriptorType;
                descriptorSetLayoutBinding.descriptorCount = descriptorCount;
                descriptorSetLayoutBinding.stageFlags = stageFlags;
                descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers;
            }

            void VulkanWindow::pushVkDescriptorSet_Buffer(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                          VkDescriptorSet dstSet,
                                                          uint32_t dstBinding,
                                                          uint32_t dstArrayElement,
                                                          uint32_t descriptorCount,
                                                          VkDescriptorType type,
                                                          VkDescriptorBufferInfo& bufferInfo)
            {
                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = dstSet;
                ds.dstBinding = dstBinding;
                ds.dstArrayElement = dstArrayElement;
                ds.descriptorCount = descriptorCount;
                ds.descriptorType = type;
                ds.pBufferInfo = &bufferInfo;

                aWriteDescriptorSets.push_back(ds);
            }
            void VulkanWindow::pushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                           VkDescriptorSet dstSet,
                                                           uint32_t dstBinding,
                                                           uint32_t dstArrayElement,
                                                           uint32_t descriptorCount,
                                                           VkDescriptorBufferInfo& bufferInfo)
            {
                pushVkDescriptorSet_Buffer(aWriteDescriptorSets,
                                           dstSet,
                                           dstBinding,
                                           dstArrayElement,
                                           descriptorCount,
                                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                                           bufferInfo);
            }
            void VulkanWindow::pushVkDescriptorSet_UniformDynamic(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                  VkDescriptorSet dstSet,
                                                                  uint32_t dstBinding,
                                                                  uint32_t dstArrayElement,
                                                                  uint32_t descriptorCount,
                                                                  VkDescriptorBufferInfo& bufferInfo)
            {
                pushVkDescriptorSet_Buffer(aWriteDescriptorSets,
                                           dstSet,
                                           dstBinding,
                                           dstArrayElement,
                                           descriptorCount,
                                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                                           bufferInfo);
            }
            void VulkanWindow::pushVkDescriptorSet_Storage(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                           VkDescriptorSet dstSet,
                                                           uint32_t dstBinding,
                                                           uint32_t dstArrayElement,
                                                           uint32_t descriptorCount,
                                                           VkDescriptorBufferInfo& bufferInfo)
            {
                pushVkDescriptorSet_Buffer(aWriteDescriptorSets,
                                           dstSet,
                                           dstBinding,
                                           dstArrayElement,
                                           descriptorCount,
                                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                           bufferInfo);
            }
            void VulkanWindow::pushVkDescriptorSet_StorageDynamic(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                  VkDescriptorSet dstSet,
                                                                  uint32_t dstBinding,
                                                                  uint32_t dstArrayElement,
                                                                  uint32_t descriptorCount,
                                                                  VkDescriptorBufferInfo& bufferInfo)
            {
                pushVkDescriptorSet_Buffer(aWriteDescriptorSets,
                                           dstSet,
                                           dstBinding,
                                           dstArrayElement,
                                           descriptorCount,
                                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
                                           bufferInfo);
            }
            void VulkanWindow::pushVkDescriptorSet_IndirectCommand(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                  VkDescriptorSet dstSet,
                                                                  uint32_t dstBinding,
                                                                  uint32_t dstArrayElement,
                                                                  uint32_t descriptorCount,
                                                                  VkDescriptorBufferInfo& bufferInfo)
            {
                pushVkDescriptorSet_Buffer(aWriteDescriptorSets,
                                           dstSet,
                                           dstBinding,
                                           dstArrayElement,
                                           descriptorCount,
                                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                                           bufferInfo);
            }

            void VulkanWindow::pushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                         VkDescriptorSet dstSet,
                                                         uint32_t dstBinding,
                                                         uint32_t dstArrayElement,
                                                         uint32_t descriptorCount,
                                                         VkDescriptorType descriptorType,
                                                         VkDescriptorImageInfo& imageInfo)
            {
                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = dstSet;
                ds.dstBinding = dstBinding;
                ds.dstArrayElement = dstArrayElement;
                ds.descriptorCount = descriptorCount;
                ds.descriptorType = descriptorType;
                ds.pImageInfo = &imageInfo;

                aWriteDescriptorSets.push_back(ds);
            }
            void VulkanWindow::updateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets)
            {
                vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(aWriteDescriptorSets.size()), aWriteDescriptorSets.data(), 0, nullptr);
            }

    void VulkanWindow::createCommandBuffers()
    {
        F_LogInfo("**<2-1-8> VulkanWindow::createCommandBuffers start **");
        {
            //1> createCommandBuffer_Graphics
            createCommandBuffer_Graphics();

            //2> createCommandBuffer_Compute
            createCommandBuffer_Compute();


            F_LogInfo("<2-1-8> VulkanWindow::createCommandBuffers finish, create CommandBuffersGraphics: [true], create CommandBufferCompute: [%s]", (this->cfg_isUseComputeShaderBeforeRender || this->cfg_isUseComputeShaderAfterRender) ? "true" : "false");
        }
        F_LogInfo("**<2-1-8> VulkanWindow::createCommandBuffers finish **");
    }
        void VulkanWindow::createCommandBuffer_Graphics()
        {
            size_t count = this->poSwapChainFrameBuffers.size();
            this->poCommandBuffersGraphics.resize(count);
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = this->poCommandPoolGraphics;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = (uint32_t)count;
            if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, this->poCommandBuffersGraphics.data()) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandBuffer_Graphics: Failed to allocate command buffers graphics !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            for (size_t i = 0; i < count; i++)
            {
                String nameCmdBuffer = "CommandBuffer-Graphics-" + FUtilString::SaveSizeT(i); 
                this->poDebug->SetVkCommandBufferName(this->poDevice, this->poCommandBuffersGraphics[i], nameCmdBuffer.c_str());
            }
            F_LogInfo("<2-1-8-1> VulkanWindow::createCommandBuffer_Graphics: Create CommandBuffersGraphics success !");
        }
        void VulkanWindow::createCommandBuffer_Compute()
        {
            if (this->cfg_isUseComputeShaderBeforeRender)
            {
                VkCommandBufferAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool = this->poCommandPoolCompute;
                allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = 1;
                if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, &this->poCommandBufferComputeBefore) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createCommandBuffer_Compute: Failed to allocate command buffer compute before !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                this->poDebug->SetVkCommandBufferName(this->poDevice, this->poCommandBufferComputeBefore, "CommandBuffer-Compute-Before");

                F_LogInfo("<2-1-8-2> VulkanWindow::createCommandBuffer_Compute: Create CommandBufferComputeBefore success !");
            }
            if (this->cfg_isUseComputeShaderAfterRender)
            {
                VkCommandBufferAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool = this->poCommandPoolCompute;
                allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = 1;
                if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, &this->poCommandBufferComputeAfter) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createCommandBuffer_Compute: Failed to allocate command buffer compute after !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                this->poDebug->SetVkCommandBufferName(this->poDevice, this->poCommandBufferComputeAfter, "CommandBuffer-Compute-After");

                F_LogInfo("<2-1-8-2> VulkanWindow::createCommandBuffer_Compute: Create CommandBufferComputeeAfter success !");
            }
        }

    void VulkanWindow::createImgui()
    {
        F_LogInfo("**********<2-2> VulkanWindow::createImgui start **********");
        {
            //1> createImgui_DescriptorPool
            createImgui_DescriptorPool();
            
            //2> createImgui_Init
            createImgui_Init();
        }
        F_LogInfo("**********<2-2> VulkanWindow::createImgui finish **********");
    }
        void VulkanWindow::createImgui_DescriptorPool()
        {   
            VkDescriptorPoolSize pool_sizes[] = 
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
            };

            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
            pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
            pool_info.pPoolSizes = pool_sizes;

            if (vkCreateDescriptorPool(this->poDevice, &pool_info, nullptr, &this->imgui_DescriptorPool) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createImgui_DescriptorPool: Imgui descriptor pool creation failed !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            F_LogInfo("<2-2-1> VulkanWindow::createImgui_DescriptorPool finish !");
        }
        void checkImguiError(VkResult err)
        {

        }
        void VulkanWindow::createImgui_Init()
        {
            //1> Config
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            this->imgui_PathIni = this->pathBin + "Log/" + this->nameTitle + ".ini";
            this->imgui_PathLog = this->pathBin + "Log/" + this->nameTitle + ".log";
            io.IniFilename = this->imgui_PathIni.c_str();
            io.LogFilename = this->imgui_PathLog.c_str();

            //2> Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsClassic();
            
            //3> Init Vulkan
            ImGui_ImplGlfw_InitForVulkan(this->pWindow, true);

            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = this->poInstance;
            init_info.PhysicalDevice = this->poPhysicalDevice;
            init_info.Device = this->poDevice;
            init_info.QueueFamily = this->queueIndexGraphics;
            init_info.Queue = this->poQueueGraphics;
            init_info.DescriptorPool = this->imgui_DescriptorPool;
            init_info.Subpass = 1;
            init_info.MinImageCount = this->imgui_MinimalSwapchainImages;
            init_info.ImageCount = (uint32_t)this->poSwapChainImages.size();
            init_info.MSAASamples = this->poMSAASamples;
            init_info.CheckVkResultFn = checkImguiError;
            ImGui_ImplVulkan_Init(&init_info, this->poRenderPass);

            VkCommandBuffer commandBuffer = beginSingleTimeCommands();
            {
                ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
            }
            endSingleTimeCommands(commandBuffer);
            ImGui_ImplVulkan_DestroyFontUploadObjects();

            F_LogInfo("<2-2-2> VulkanWindow::createImgui_Init finish !");
        }

    void VulkanWindow::createEditor()
    {
        F_LogInfo("**********<2-3> VulkanWindow::createEditor start **********");
        {
            //1> createEditor_Grid
            createEditor_Grid();
            
            //2> createEditor_CameraAxis
            createEditor_CameraAxis();

            //3> createEditor_CoordinateAxis
            createEditor_CoordinateAxis();

            //4> createEditor_LineFlat2DCollector
            createEditor_LineFlat2DCollector();

            //5> createEditor_LineFlat3DCollector
            createEditor_LineFlat3DCollector();
        }
        F_LogInfo("**********<2-3> VulkanWindow::createEditor finish **********");
    }
        void VulkanWindow::createEditor_Grid()
        {
            this->pEditorGrid = new EditorGrid();
            this->pEditorGrid->Init();

            F_LogInfo("<2-3-1> VulkanWindow::createEditor_Grid finish !");
        }
        void VulkanWindow::createEditor_CameraAxis()
        {
            this->pEditorCameraAxis = new EditorCameraAxis();
            this->pEditorCameraAxis->Init();

            F_LogInfo("<2-3-2> VulkanWindow::createEditor_CameraAxis finish !");
        }
        void VulkanWindow::createEditor_CoordinateAxis()
        {
            this->pEditorCoordinateAxis = new EditorCoordinateAxis();
            this->pEditorCoordinateAxis->Init();

            F_LogInfo("<2-3-3> VulkanWindow::createEditor_CoordinateAxis finish !");
        }
        void VulkanWindow::createEditor_LineFlat2DCollector()
        {
            this->pEditorLineFlat2DCollector = new EditorLineFlat2DCollector();
            this->pEditorLineFlat2DCollector->Init();

            F_LogInfo("<2-3-4> VulkanWindow::createEditor_LineFlat2DCollector finish !");
        }
        void VulkanWindow::createEditor_LineFlat3DCollector()
        {
            this->pEditorLineFlat3DCollector = new EditorLineFlat3DCollector();
            this->pEditorLineFlat3DCollector->Init();

            F_LogInfo("<2-3-5> VulkanWindow::createEditor_LineFlat3DCollector finish !");
        }
    void VulkanWindow::destroyEditor()
    {
        F_DELETE(this->pEditorGrid)
        F_DELETE(this->pEditorCameraAxis)
        F_DELETE(this->pEditorCoordinateAxis)
        F_DELETE(this->pEditorLineFlat2DCollector)
        F_DELETE(this->pEditorLineFlat3DCollector)
    }

    void VulkanWindow::resizeWindow(int w, int h, bool force)
    {
        if (this->width == w &&
            this->height == h &&
            !force)
        {
            return;
        }
        this->width = w;
        this->height = h;
        RefreshAspectRatio();

    }

    //Compute Before Render
    bool VulkanWindow::beginCompute_BeforeRender()
    {
        if (!this->cfg_isUseComputeShaderBeforeRender ||
            this->poQueueCompute == VK_NULL_HANDLE ||
            this->poCommandBufferComputeBefore == VK_NULL_HANDLE)
        {
            return false;
        }

        return true;
    }
        void VulkanWindow::updateCompute_BeforeRender()
        {  
            //CommandBuffer
            updateComputeCommandBuffer_BeforeRender();

        }
                void VulkanWindow::updateComputeCommandBuffer_BeforeRender()
                {
                    vkQueueWaitIdle(this->poQueueCompute);

                    VkCommandBuffer& commandBuffer = this->poCommandBufferComputeBefore;
                    VkCommandBufferBeginInfo beginInfo = {};
                    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                    beginInfo.flags = 0; // Optional
                    beginInfo.pInheritanceInfo = nullptr; // Optional

                    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                    {
                        String msg = "*********************** VulkanWindow::updateComputeCommandBuffer_BeforeRender: vkBeginCommandBuffer: Failed to begin recording compute command buffer !";
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                    {
                        updateCompute_BeforeRender_Default(commandBuffer);
                        updateCompute_BeforeRender_Terrain(commandBuffer);
                        updateCompute_BeforeRender_Custom(commandBuffer);
                        updateCompute_BeforeRender_Cull(commandBuffer);
                    }
                    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                    {
                        String msg = "*********************** VulkanWindow::updateComputeCommandBuffer_BeforeRender: vkEndCommandBuffer: Failed to record compute command buffer !";
                        F_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                }
                    void VulkanWindow::updateCompute_BeforeRender_Default(VkCommandBuffer& commandBuffer)
                    {

                    }
                    void VulkanWindow::updateCompute_BeforeRender_Terrain(VkCommandBuffer& commandBuffer)
                    {
                        if (!this->cfg_isRenderPassTerrain ||
                            this->m_pPipelineCompute_Terrain == nullptr)
                            return;
                        
                        Update_Compute_Terrain(commandBuffer);
                    }
                    void VulkanWindow::updateCompute_BeforeRender_Custom(VkCommandBuffer& commandBuffer)
                    {

                    }
                    void VulkanWindow::updateCompute_BeforeRender_Cull(VkCommandBuffer& commandBuffer)
                    {
                        if (!this->cfg_isRenderPassCull ||
                            !this->isComputeCullFrustumHizDepth ||
                            this->m_pPipelineCompute_Cull == nullptr)
                            return;

                        UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToShaderReadOnly(commandBuffer);
                        {
                            Update_Compute_Cull(commandBuffer);
                        }
                        UpdateImageLayout_Graphics_DepthHizImageLayoutFromShaderReadOnlyToColorAttachment(commandBuffer);
                    }

        void VulkanWindow::compute_BeforeRender()
        {
            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBufferComputeBefore;
            if (this->poGraphicsWaitComputeBeforeSemaphore != VK_NULL_HANDLE)
            {
                submitInfo.waitSemaphoreCount = 1;
                submitInfo.pWaitSemaphores = &this->poGraphicsWaitComputeBeforeSemaphore;
            }
            submitInfo.pWaitDstStageMask = &waitStageMask;
            if (this->poComputeBeforeWaitSemaphore != VK_NULL_HANDLE)
            {
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores = &this->poComputeBeforeWaitSemaphore;
            }
            
            if (vkQueueSubmit(this->poQueueCompute, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::compute_BeforeRender: Failed to submit compute command buffer !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    void VulkanWindow::endCompute_BeforeRender()
    {
        
    }

    //Compute After Render
    bool VulkanWindow::beginCompute_AfterRender()
    {   
        if (!this->cfg_isUseComputeShaderAfterRender ||
            this->poQueueCompute == VK_NULL_HANDLE ||
            this->poCommandBufferComputeAfter == VK_NULL_HANDLE)
        {
            return false;
        }

        return true;
    }
        void VulkanWindow::updateCompute_AfterRender()
        {
            //CommandBuffer
            updateComputeCommandBuffer_AfterRender();

        }
            void VulkanWindow::updateComputeCommandBuffer_AfterRender()
            {
                vkQueueWaitIdle(this->poQueueCompute);

                VkCommandBuffer& commandBuffer = this->poCommandBufferComputeAfter;
                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = 0; // Optional
                beginInfo.pInheritanceInfo = nullptr; // Optional

                if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateComputeCommandBuffer_AfterRender: vkBeginCommandBuffer: Failed to begin recording compute command buffer !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                {
                    updateCompute_AfterRender_Default(commandBuffer);
                    updateCompute_AfterRender_Custom(commandBuffer);
                    updateCompute_AfterRender_HizDepthGenerate(commandBuffer);
                }
                if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateComputeCommandBuffer_AfterRender: vkEndCommandBuffer: Failed to record compute command buffer !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
                void VulkanWindow::updateCompute_AfterRender_Default(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::updateCompute_AfterRender_Custom(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::updateCompute_AfterRender_HizDepthGenerate(VkCommandBuffer& commandBuffer)
                {
                    if (!this->cfg_isRenderPassCull ||
                        !this->isComputeCullFrustumHizDepth ||
                        this->m_pPipelineCompute_Cull == nullptr)
                        return;

                    UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToGeneral(commandBuffer);
                    {
                        Update_Compute_HizDepthGenerate(commandBuffer);
                    }
                    UpdateImageLayout_Graphics_DepthHizImageLayoutFromGeneralToColorAttachment(commandBuffer);
                }

        void VulkanWindow::compute_AfterRender()
        {
            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBufferComputeAfter;
            submitInfo.pWaitDstStageMask = &waitStageMask;
            VkSemaphoreVector aWaitSemaphores;
            if (this->poGraphicsSignalComputeAfterSemaphore != VK_NULL_HANDLE)
            {
                submitInfo.waitSemaphoreCount = 1;
                submitInfo.pWaitSemaphores = &this->poGraphicsSignalComputeAfterSemaphore;
            }
            if (this->poComputeAfterWaitSemaphore != VK_NULL_HANDLE)
            {
                submitInfo.signalSemaphoreCount = 1;
                submitInfo.pSignalSemaphores = &this->poComputeAfterWaitSemaphore;
            }
            
            if (vkQueueSubmit(this->poQueueCompute, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::compute_AfterRender: Failed to submit compute command buffer !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    void VulkanWindow::endCompute_AfterRender()
    {

    }


    bool VulkanWindow::beginRender()
    {
        waitForFence(this->poInFlightFences[this->poCurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(this->poDevice, this->poSwapChain, UINT64_MAX, this->poPresentCompleteSemaphores[this->poCurrentFrame], nullptr, &this->poSwapChainImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return false;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            String msg = "*********************** VulkanWindow::beginRender: Failed to acquire swap chain image !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
            return false;
        }
        return true;
    }
        void VulkanWindow::updateRender()
        {
            //1> updateCBs_Default
            updateCBs_Default();

            //2> updateCBs_Terrain
            updateCBs_Terrain();

            //3> updateCBs_ImGUI
            updateCBs_ImGUI();

            //4> updateCBs_Editor
            updateCBs_Editor();

            //5> CB Custom
            updateCBs_Custom();

            //6> CommandBuffers
            updateRenderCommandBuffers_CustomBeforeDefault();
            {
                updateRenderCommandBuffers_Default();
            }
            updateRenderCommandBuffers_CustomAfterDefault();
        }
            void VulkanWindow::updateCBs_Default()
            {
                updateCBs_Pass();
                updateCBs_Objects();
                updateCBs_Materials();
                updateCBs_Instances();
            }
                void VulkanWindow::updateCBs_Pass()
                {
                    if (this->poBuffersMemory_PassCB.size() <= 0)
                        return;

                    //TransformConstants/CameraConstants
                    if (this->pCamera != nullptr)
                    {
                        updateCBs_PassTransformAndCamera(this->passCB, this->pCamera, 0);
                        if (this->pCameraRight == nullptr)
                        {
                            this->passCB.g_Transforms[1] = this->passCB.g_Transforms[0];
                            this->passCB.g_Cameras[1] = this->passCB.g_Cameras[0];
                        }
                    }
                    else
                    {
                        TransformConstants& transformConstants = this->passCB.g_Transforms[0];
                        transformConstants.mat4View = glm::lookAtLH(this->cfg_cameraPos, 
                                                                    this->cfg_cameraLookTarget,
                                                                    this->cfg_cameraUp);
                        transformConstants.mat4View_Inv = FMath::InverseMatrix4(transformConstants.mat4View);
                        transformConstants.mat4Proj = glm::perspectiveLH(glm::radians(this->cfg_cameraFov), 
                                                                         this->poSwapChainExtent.width / (float)this->poSwapChainExtent.height,
                                                                         this->cfg_cameraNear, 
                                                                         this->cfg_cameraFar);
                        transformConstants.mat4Proj_Inv = FMath::InverseMatrix4(transformConstants.mat4Proj);
                        transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                        transformConstants.mat4ViewProj_Inv = FMath::InverseMatrix4(transformConstants.mat4ViewProj);
                        
                        //CameraConstants
                        CameraConstants& cameraConstants = this->passCB.g_Cameras[0];
                        cameraConstants.posEyeWorld = this->cfg_cameraPos;
                        cameraConstants.fNearZ = this->cfg_cameraNear;
                        cameraConstants.fFarZ = this->cfg_cameraFar;
                    }   
                    if (this->pCameraRight != nullptr)
                    {
                        updateCBs_PassTransformAndCamera(this->passCB, this->pCameraRight, 1); 
                    }

                    //TimeConstants
                    this->passCB.g_TotalTime = this->pTimer->GetTimeSinceStart();
                    this->passCB.g_DeltaTime = this->pTimer->GetTimeDelta();

                    //RenderTarget
                    this->passCB.g_RenderTargetSize = FVector2(this->poViewport.width, this->poViewport.height);
                    this->passCB.g_RenderTargetSize_Inv = FVector2(1.0f / this->poViewport.width, 1.0f / this->poViewport.height);

                    //Light Settings
                    if (this->cfg_isRenderPassShadowMap)
                    {
                        const FMatrix4& depthViewMatrix = this->pCameraMainLight->GetMatrix4View();
                        const FMatrix4& depthProjectionMatrix = this->pCameraMainLight->GetMatrix4Projection();
                        this->mainLight.depthMVP = depthProjectionMatrix * depthViewMatrix;
                    }
                    memcpy(&this->passCB.g_MainLight, &this->mainLight, sizeof(LightConstants));
                    for (int i = 0; i < MAX_LIGHT_COUNT; i++)
                    {
                        memcpy(&this->passCB.g_AdditionalLights[i], &this->aAdditionalLights[i], sizeof(LightConstants));
                    }

                    //Update Buffer
                    VkDeviceMemory& memory = this->poBuffersMemory_PassCB[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(PassConstants), &this->passCB, memory);
                }
                    void VulkanWindow::updateCBs_PassTransformAndCamera(PassConstants& pass, FCamera* pCam, int nIndex)
                    {
                        //TransformConstants
                        TransformConstants& transformConstants = pass.g_Transforms[nIndex];
                        transformConstants.mat4View = pCam->GetMatrix4View();
                        transformConstants.mat4View_Inv = FMath::InverseMatrix4(transformConstants.mat4View);
                        transformConstants.mat4Proj = pCam->GetMatrix4Projection();
                        transformConstants.mat4Proj_Inv = FMath::InverseMatrix4(transformConstants.mat4Proj);
                        transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                        transformConstants.mat4ViewProj_Inv = FMath::InverseMatrix4(transformConstants.mat4ViewProj);

                        //CameraConstants
                        CameraConstants& cameraConstants = pass.g_Cameras[nIndex];
                        cameraConstants.posEyeWorld = pCam->GetPos();
                        cameraConstants.fNearZ = pCam->GetNearZ();
                        cameraConstants.fFarZ = pCam->GetFarZ();
                    }   
                void VulkanWindow::updateCBs_Objects()
                {
                    if (this->poBuffersMemory_ObjectCB.size() <= 0)
                        return;
                    size_t count = this->objectCBs.size();
                    if (count >= MAX_OBJECT_COUNT)
                    {
                        F_LogError("*********************** VulkanWindow::updateCBs_Objects: Max object count can not > [%d]", MAX_OBJECT_COUNT);
                        return;
                    }

                    updateCBs_ObjectsContent();

                    VkDeviceMemory& memory = this->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(ObjectConstants) * count, this->objectCBs.data(), memory);
                }
                    void VulkanWindow::updateCBs_ObjectsContent()
                    {
                        ObjectConstants& objectCB = this->objectCBs[0];
                        if (this->cfg_isRotate)
                        {
                            float time = this->pTimer->GetTimeSinceStart();
                            objectCB.g_MatWorld = glm::rotate(this->poMatWorld, 
                                                            time * glm::radians(90.0f), 
                                                            FVector3(0.0f, 1.0f, 0.0f));
                        }
                        else
                        {
                            objectCB.g_MatWorld = this->poMatWorld;
                        }
                    }
                void VulkanWindow::updateCBs_Materials()
                {
                    if (this->poBuffersMemory_MaterialCB.size() <= 0)
                        return;
                    size_t count = this->materialCBs.size();
                    if (count >= MAX_MATERIAL_COUNT)
                    {
                        F_LogError("*********************** VulkanWindow::updateCBs_Materials: Max material count can not > [%d]", MAX_MATERIAL_COUNT);
                        return;
                    }

                    updateCBs_MaterialsContent();

                    VkDeviceMemory& memory = this->poBuffersMemory_MaterialCB[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(MaterialConstants) * count, this->materialCBs.data(), memory);
                }
                    void VulkanWindow::updateCBs_MaterialsContent()
                    {   
                        
                    }
                void VulkanWindow::updateCBs_Instances()
                {
                    if (this->poBuffersMemory_InstanceCB.size() <= 0)
                        return;
                    size_t count = this->instanceCBs.size();
                    if (count >= MAX_INSTANCE_COUNT)
                    {
                        F_LogError("*********************** VulkanWindow::updateCBs_Instances: Max instance count can not > [%d]", MAX_INSTANCE_COUNT);
                        return;
                    }

                    updateCBs_InstancesContent();

                    VkDeviceMemory& memory = this->poBuffersMemory_InstanceCB[this->poSwapChainImageIndex];
                    updateVKBuffer(0, sizeof(InstanceConstants) * count, this->instanceCBs.data(), memory);
                }
                    void VulkanWindow::updateCBs_InstancesContent()
                    {
                        InstanceConstants materialCB = this->instanceCBs[0];
                        materialCB.indexObject = 0;
                        materialCB.indexMaterial = 0;
                    }
            void VulkanWindow::updateCBs_Terrain()
            {

            }
            void VulkanWindow::updateCBs_ImGUI()
            {
                if (IsEnable_Imgui())
                {
                    if (beginRenderImgui())
                    {
                        endRenderImgui();
                    }
                }
            }
                bool VulkanWindow::beginRenderImgui()
                {
                    
                    return false;
                }
                    void VulkanWindow::commonConfig()
                    {
                        ImGui::Text("Frametime: %f", this->fFPS);

                        ImGui::Separator();
                        commonShowConfig();

                        ImGui::Separator();
                        commonEditorConfig();
                    }
                        void VulkanWindow::commonShowConfig()
                        {
                            if (ImGui::CollapsingHeader("Common Show"))
                            {
                                ImGui::Text("Op ResetCamera -- Key R");
                                ImGui::Text("Op WireFrame -- Key F");
                                ImGui::Text("Op Rotate -- Key T");

                                ImGui::Separator();
                                ImGui::Text("Viewport Offset: [%f, %f]",
                                            this->poViewport.x,
                                            this->poViewport.y);
                                ImGui::Text("Viewport Size: [%f, %f]",
                                            this->poViewport.width,
                                            this->poViewport.height);
                                ImGui::Text("Framebuffer Size: [%f, %f]",
                                            this->poFramebufferSize.x,
                                            this->poFramebufferSize.y);
                                ImGui::Text("Glfw WindowContent Scale: [%f, %f]",
                                            this->poWindowContentScale.x,
                                            this->poWindowContentScale.y);

                                ImGui::Separator();
                                ImGui::Text("Mouse Screen XY: [%f, %f]", 
                                            this->mousePosLast.x,
                                            this->mousePosLast.y);
                                if (this->poViewport.width > 0 && this->poViewport.height > 0)
                                ImGui::Text("Mouse Viewport XY: [%f, %f]", 
                                            this->mousePosLast.x / this->poViewport.width,
                                            this->mousePosLast.y / this->poViewport.height);
                                ImGui::Text("Mouse NDC XY: [%f, %f]", 
                                            (float)((this->mousePosLast.x - this->poViewport.x) * 2.0f / this->poViewport.width - 1.0f),
                                            (float)(1.0f - (this->mousePosLast.y - this->poViewport.y) * 2.0f / this->poViewport.height));
                                ImGui::Text("Mouse LeftDown: [%s]", 
                                            this->mouseButtonDownLeft ? "true" : "false");
                                ImGui::Text("Mouse RightDown: [%s]", 
                                            this->mouseButtonDownRight ? "true" : "false");
                                ImGui::Text("Mouse MiddleDown: [%s]", 
                                            this->mouseButtonDownMiddle ? "true" : "false");

                                ImGui::Separator();
                                if (this->cfg_isRenderPassTerrain &&
                                    this->m_pPipelineGraphics_Terrain != nullptr)
                                {
                                    FVector3 vPosTerrain;
                                    bool isRaytraceTerrain = RaytraceTerrain(this->mousePosLast.x, this->mousePosLast.y, vPosTerrain);

                                    ImGui::Text("RaytraceTerrain: [%s]\nPosTerrain: [%f, %f, %f]", 
                                                isRaytraceTerrain ? "true" : "false", vPosTerrain.x, vPosTerrain.y, vPosTerrain.z);
                                }
                            }
                        }
                        void VulkanWindow::commonEditorConfig()
                        {
                            if (ImGui::CollapsingHeader("Common Op Settings"))
                            {
                                ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame);
                                ImGui::Checkbox("Is Rotate", &cfg_isRotate);
                                ImGui::Separator();
                                if (ImGui::CollapsingHeader("EditorGrid Settings"))
                                {
                                    ImGui::Checkbox("Is EditorGridShow", &cfg_isEditorGridShow);
                                    if (this->pEditorGrid != nullptr)
                                    {
                                        if (ImGui::ColorEdit4("EditorGrid Color", (float*)&this->cfg_editorGrid_Color))
                                        {
                                            this->pEditorGrid->SetColor(this->cfg_editorGrid_Color);
                                        }
                                    }
                                }
                                if (ImGui::CollapsingHeader("EditorCameraAxis Settings"))
                                {
                                    ImGui::Checkbox("Is EditorCameraAxisShow", &cfg_isEditorCameraAxisShow);

                                }
                                if (ImGui::CollapsingHeader("EditorCoordinateAxis Settings"))
                                {
                                    ImGui::Checkbox("Is EditorCoordinateAxisShow", &cfg_isEditorCoordinateAxisShow);
                                    if (this->pEditorCoordinateAxis != nullptr)
                                    {
                                        bool isSelect = this->pEditorCoordinateAxis->IsStateSelect();
                                        ImGui::Checkbox("State Select", &isSelect);
                                        if (isSelect && isSelect != this->pEditorCoordinateAxis->IsStateSelect())
                                        {
                                            this->pEditorCoordinateAxis->SetStateType(EditorCoordinateAxis::CoordinateState_Select);
                                        }
                                        bool isMove = this->pEditorCoordinateAxis->IsStateMove();
                                        ImGui::Checkbox("State Move", &isMove);
                                        if (isMove && isMove != this->pEditorCoordinateAxis->IsStateMove())
                                        {
                                            this->pEditorCoordinateAxis->SetStateType(EditorCoordinateAxis::CoordinateState_Move);
                                        }
                                        bool isRotate = this->pEditorCoordinateAxis->IsStateRotate();
                                        ImGui::Checkbox("State Rotate", &isRotate);
                                        if (isRotate && isRotate != this->pEditorCoordinateAxis->IsStateRotate())
                                        {
                                            this->pEditorCoordinateAxis->SetStateType(EditorCoordinateAxis::CoordinateState_Rotate);
                                        }
                                        bool isScale = this->pEditorCoordinateAxis->IsStateScale();
                                        ImGui::Checkbox("State Scale", &isScale);
                                        if (isScale && isScale != this->pEditorCoordinateAxis->IsStateScale())
                                        {
                                            this->pEditorCoordinateAxis->SetStateType(EditorCoordinateAxis::CoordinateState_Scale);
                                        }

                                        ImGui::Separator();

                                        if (ImGui::DragFloat("Move Speed", &this->cfg_editorCoordinateAxis_MoveSpeed, 0.01f, 0.01f, 100.0f))
                                        {
                                            
                                        }
                                        if (ImGui::DragFloat("Rotate Speed", &this->cfg_editorCoordinateAxis_RotateSpeed, 0.01f, 0.01f, 100.0f))
                                        {
                                            
                                        }
                                        if (ImGui::DragFloat("Scale Speed", &this->cfg_editorCoordinateAxis_ScaleSpeed, 0.01f, 0.01f, 100.0f))
                                        {
                                            
                                        }
                                    }
                                }
                            }
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                    void VulkanWindow::cameraConfig()
                    {
                        if (this->pCamera == nullptr)
                            return;

                        if (ImGui::CollapsingHeader("Camera Settings"))
                        {
                            if (ImGui::Button("Camera Reset"))
                            {
                                cameraReset();
                            }
                            if (ImGui::CollapsingHeader("Camera Transform"))
                            {
                                //Position
                                FVector3 vPos = this->pCamera->GetPos();
                                if (ImGui::DragFloat3("Position", &vPos[0], 0.05f, -FLT_MAX, FLT_MAX))
                                {
                                    this->pCamera->SetPos(vPos);
                                    this->pCamera->UpdateViewMatrix();
                                }
                                //Rotation
                                FVector3 vEulerAngle = this->pCamera->GetEulerAngles();
                                if (ImGui::DragFloat3("Rotation", &vEulerAngle[0], 0.1f, -180, 180))
                                {
                                    this->pCamera->SetEulerAngles(vEulerAngle);
                                    this->pCamera->UpdateViewMatrix();
                                }
                                ImGui::Spacing();
                                //Right
                                FVector3 vRight = this->pCamera->GetRight();
                                if (ImGui::DragFloat3("Right (X axis)", &vRight[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                                //Up
                                FVector3 vUp = this->pCamera->GetUp();
                                if (ImGui::DragFloat3("Up (Y axis)", &vUp[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                                //Direction
                                FVector3 vDir = this->pCamera->GetDir();
                                if (ImGui::DragFloat3("Direction (Z axis)", &vDir[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Param"))
                            {
                                //FovY
                                float fAngle = this->pCamera->GetFovY();
                                if (ImGui::DragFloat("FovY Angle", &fAngle, 0.1f, 0.1f, 180.0f))
                                {
                                    this->pCamera->SetFovY(fAngle);
                                    this->pCamera->UpdateProjectionMatrix();
                                }
                                //Aspect
                                float fAspect = this->pCamera->GetAspect();
                                if (ImGui::DragFloat("Aspect", &fAspect, 0.1f, 0.1f, 10.0f))
                                {
                                    this->pCamera->SetAspect(fAspect);
                                    this->pCamera->UpdateProjectionMatrix();
                                }

                                //NearZ/FarZ
                                float fNearDist = this->pCamera->GetNearZ();
                                float fFarDist = this->pCamera->GetFarZ();
                                if (ImGui::DragFloat("Near Distance", &fNearDist, 0.1f, 0.1f, fFarDist - 1.0f))
                                {
                                    this->pCamera->SetNearZ(fNearDist);
                                    this->pCamera->UpdateProjectionMatrix();
                                }
                                if (ImGui::DragFloat("Far Distance", &fFarDist, 0.1f, fNearDist + 1.0f, FLT_MAX))
                                {
                                    this->pCamera->SetFarZ(fFarDist);
                                    this->pCamera->UpdateProjectionMatrix();
                                }

                                ImGui::Separator();
                                ImGui::Spacing();
                                
                                //SpeedMove
                                if (ImGui::DragFloat("Speed Move", &cfg_cameraSpeedMove, 0.1f, 1.0f, 10000.0f))
                                {
                                    
                                }
                                //SpeedZoom
                                if (ImGui::DragFloat("Speed Zoom", &cfg_cameraSpeedZoom, 0.001f, 0.01f, 5.0f))
                                {

                                }
                                //SpeedRotate
                                if (ImGui::DragFloat("Speed Rotate", &cfg_cameraSpeedRotate, 0.001f, 0.001f, 5.0f))
                                {

                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Matrix4 World"))
                            {
                                FMatrix4 mat4World = this->pCamera->GetMatrix4World();
                                if (ImGui::BeginTable("split_camera_world", 4))
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
                            if (ImGui::CollapsingHeader("Camera Matrix4 View"))
                            {
                                const FMatrix4& mat4View = this->pCamera->GetMatrix4View();
                                if (ImGui::BeginTable("split_camera_view", 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][3]);
                                
                                    ImGui::EndTable();
                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Matrix4 Projection"))
                            {
                                const FMatrix4& mat4Projection = pCamera->GetMatrix4Projection();
                                if (ImGui::BeginTable("split_camera_projection", 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][3]);

                                    ImGui::EndTable();
                                }
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::cameraReset()
                        {
                            if (this->pCamera == nullptr)
                                return;

                            this->pCamera->LookAtLH(this->cfg_cameraPos, this->cfg_cameraLookTarget, this->cfg_cameraUp);
                            this->pCamera->PerspectiveLH(this->cfg_cameraFov, this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
                            this->pCamera->UpdateViewMatrix();
                        }

                    void VulkanWindow::lightConfig()
                    {
                        if (ImGui::CollapsingHeader("Light Settings"))
                        {
                            //Main Light
                            lightConfigItem(this->mainLight, "Light - Main", 0, false, true);
                            
                            //Additional Light
                            int count_light = MAX_LIGHT_COUNT;
                            for (int i = 0; i < count_light; i++)
                            {
                                LightConstants& lc = this->aAdditionalLights[i];
                                String nameLight = "Light - " + FUtilString::SaveInt(i);
                                lightConfigItem(lc, nameLight, i, true, false);
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType, bool bIsMainLight)
                        {
                            struct EnumLightDesc { VulkanLightType Value; const char* Name; const char* Tooltip; };
                            static const EnumLightDesc s_aLightDescs[] =
                            {
                                { Vulkan_Light_Directional,     "Directional",      "Directional Light" },
                                { Vulkan_Light_Point,           "Point",            "Point Light" },
                                { Vulkan_Light_Spot,            "Spot",             "Spot Light" },
                            };

                            struct EnumLightingDesc { VulkanLightingType Value; const char* Name; const char* Tooltip; };
                            static const EnumLightingDesc s_aLightingDescs[] =
                            {
                                { Vulkan_Lighting_Node,                                     "None",                                     "None Lighting" },
                                { Vulkan_Lighting_Ambient,                                  "Ambient",                                  "Ambient Lighting" },
                                { Vulkan_Lighting_DiffuseLambert,                           "DiffuseLambert",                           "DiffuseLambert Lighting" },
                                { Vulkan_Lighting_SpecularPhong,                            "SpecularPhong",                            "SpecularPhong Lighting" },
                                { Vulkan_Lighting_SpecularBlinnPhong,                       "SpecularBlinnPhong",                       "SpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambert,                    "AmbientDiffuseLambert",                    "AmbientDiffuseLambert Lighting" },
                                { Vulkan_Lighting_AmbientSpecularPhong,                     "AmbientSpecularPhong",                     "AmbientSpecularPhong Lighting" },
                                { Vulkan_Lighting_AmbientSpecularBlinnPhong,                "AmbientSpecularBlinnPhong",                "AmbientSpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_DiffuseLambertSpecularPhong,              "DiffuseLambertSpecularPhong",              "DiffuseLambertSpecularPhong Lighting" },
                                { Vulkan_Lighting_DiffuseLambertSpecularBlinnPhong,         "DiffuseLambertSpecularBlinnPhong",         "DiffuseLambertSpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambertSpecularPhong,       "AmbientDiffuseLambertSpecularPhong",       "AmbientDiffuseLambertSpecularPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambertSpecularBlinnPhong,  "AmbientDiffuseLambertSpecularBlinnPhong",  "AmbientDiffuseLambertSpecularBlinnPhong Lighting" },
                            };
                            
                            if (ImGui::CollapsingHeader(name.c_str()))
                            {
                                //Light Enable
                                bool isEnable = lc.common.y == 0.0f ? false : true;
                                String nameEnable = "LightEnable - " + FUtilString::SaveInt(index);
                                if (ImGui::Checkbox(nameEnable.c_str(), &isEnable))
                                {   
                                    lc.common.y = isEnable ? 1.0f : 0.0f;
                                }
                                ImGui::Spacing();

                                //Light Type
                                {
                                    int nIndex = 0;
                                    for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aLightDescs); nIndex++)
                                    {
                                        if (s_aLightDescs[nIndex].Value == lc.common.x)
                                            break;
                                    }
                                    const char* preview_text = s_aLightDescs[nIndex].Name;
                                    String nameType = "LightType - " + FUtilString::SaveInt(index);
                                    if (ImGui::BeginCombo(nameType.c_str(), preview_text))
                                    {
                                        for (int j = 0; j < IM_ARRAYSIZE(s_aLightDescs); j++)
                                        {
                                            if (ImGui::Selectable(s_aLightDescs[j].Name, nIndex == j))
                                            {
                                                if (canChangeType)
                                                {
                                                    lc.common.x = (float)((int)(s_aLightDescs[j].Value));
                                                }
                                                break;
                                            }
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                ImGui::Spacing();

                                //Lighting Type
                                {
                                    int nIndex = 0;
                                    for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aLightingDescs); nIndex++)
                                    {
                                        if (s_aLightingDescs[nIndex].Value == lc.common.z)
                                            break;
                                    }
                                    const char* preview_text = s_aLightingDescs[nIndex].Name;
                                    String nameType = "LightingType - " + FUtilString::SaveInt(index);
                                    if (ImGui::BeginCombo(nameType.c_str(), preview_text))
                                    {
                                        for (int j = 0; j < IM_ARRAYSIZE(s_aLightingDescs); j++)
                                        {
                                            if (ImGui::Selectable(s_aLightingDescs[j].Name, nIndex == j))
                                            {
                                                lc.common.z = (float)((int)(s_aLightingDescs[j].Value));
                                                break;
                                            }
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                ImGui::Spacing();

                                //position
                                FVector3 vPosition = lc.position;
                                String namePosition = "Position - " + FUtilString::SaveInt(index);
                                if (ImGui::DragFloat3(namePosition.c_str(), &vPosition[0], 0.01f, -FLT_MAX, FLT_MAX))
                                {
                                    lc.position = vPosition;

                                    if (bIsMainLight)
                                    {
                                        this->pCameraMainLight->SetPos(vPosition);
                                        this->pCameraMainLight->UpdateViewMatrix();
                                    }
                                }
                                ImGui::Spacing();

                                //Euler Angle
                                String nameEulerAngle = "EulerAngle - " + FUtilString::SaveInt(index);
                                FVector3 vEulerAngle = FMath::ToEulerAngles(lc.direction);
                                if (ImGui::DragFloat3(nameEulerAngle.c_str(), &vEulerAngle[0], 0.1f, -180, 180))
                                {
                                    lc.direction = FMath::ToDirection(vEulerAngle);

                                    if (bIsMainLight)
                                    {
                                        this->pCameraMainLight->SetEulerAngles(vEulerAngle);
                                        this->pCameraMainLight->UpdateViewMatrix();
                                    }
                                }
                                //direction
                                FVector3 vDirection = lc.direction;
                                String nameDirection = "Direction - " + FUtilString::SaveInt(index);
                                if (ImGui::DragFloat3(nameDirection.c_str(), &vDirection[0], 0.0001f, -1.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //ambient
                                String nameAmbient = "Ambient - " + FUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameAmbient.c_str(), (float*)&lc.ambient))
                                {

                                }
                                ImGui::Spacing();

                                //diffuse
                                String nameDiffuse = "Diffuse - " + FUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameDiffuse.c_str(), (float*)&lc.diffuse))
                                {

                                }
                                ImGui::Spacing();

                                //specular
                                String nameSpecular = "Specular - " + FUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameSpecular.c_str(), (float*)&lc.specular))
                                {

                                }
                                ImGui::Spacing();

                                if (lc.common.x == (int)Vulkan_Light_Directional)
                                {
                                    
                                }
                                else if (lc.common.x == (int)Vulkan_Light_Point)
                                {
                                    //falloffStart
                                    float fFalloffStart = lc.falloffStart;
                                    String nameFalloffStart = "FalloffStart - " + FUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffStart.c_str(), &fFalloffStart, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffStart = fFalloffStart;
                                    }
                                    ImGui::Spacing();

                                    //falloffEnd
                                    float fFalloffEnd = lc.falloffEnd;
                                    String nameFalloffEnd = "FalloffEnd - " + FUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffEnd.c_str(), &fFalloffEnd, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffEnd = fFalloffEnd;
                                    }
                                }
                                else if (lc.common.x == (int)Vulkan_Light_Spot)
                                {
                                    //falloffStart
                                    float fFalloffStart = lc.falloffStart;
                                    String nameFalloffStart = "FalloffStart - " + FUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffStart.c_str(), &fFalloffStart, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffStart = fFalloffStart;
                                    }
                                    ImGui::Spacing();

                                    //falloffEnd
                                    float fFalloffEnd = lc.falloffEnd;
                                    String nameFalloffEnd = "FalloffEnd - " + FUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffEnd.c_str(), &fFalloffEnd, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffEnd = fFalloffEnd;
                                    }
                                    ImGui::Spacing();

                                    //spotPower
                                    float fSpotPower = lc.common.w;
                                    String nameSpotPower = "SpotPower - " + FUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameSpotPower.c_str(), &fSpotPower, 0.01f, 0.1f, 200.0f))
                                    {
                                        lc.common.w = fSpotPower;
                                    }
                                }
                                
                            }
                        }
                        void VulkanWindow::lightMainReset()
                        {

                        }
                    void VulkanWindow::shadowConfig()
                    {
                        if (ImGui::CollapsingHeader("Shadow Settings"))
                        {
                            //Shadow MainLight
                            shadowConfigItem(this->shadowMainLight, "Shadow MainLight", true);
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::shadowConfigItem(ShadowConstants& sc, const String& name, bool bIsMainLight)
                        {
                            if (ImGui::CollapsingHeader(name.c_str()))
                            {
                                //fov
                                float fFov = sc.fov;
                                String nameFov = "Fov - " + name;
                                if (ImGui::DragFloat(nameFov.c_str(), &fFov, 0.001f, 20.0f, 90.0f))
                                {
                                    sc.fov = fFov;

                                    if (bIsMainLight)
                                    {
                                        this->pCameraMainLight->SetFovY(fFov);
                                        this->pCameraMainLight->UpdateProjectionMatrix();
                                    }
                                }
                                ImGui::Spacing();

                                //zNear
                                float fZNear = sc.zNear;
                                String nameZNear = "ZNear - " + name;
                                if (ImGui::DragFloat(nameZNear.c_str(), &fZNear, 0.05f, 0.5f, 10.0f))
                                {
                                    sc.zNear = fZNear;

                                    if (bIsMainLight)
                                    {
                                        this->pCameraMainLight->SetNearZ(fZNear);
                                        this->pCameraMainLight->UpdateProjectionMatrix();
                                    }
                                }
                                ImGui::Spacing();

                                //zFar
                                float fZFar = sc.zFar;
                                String nameZFar = "ZFar - " + name;
                                if (ImGui::DragFloat(nameZFar.c_str(), &fZFar, 0.05f, 20.0f, 1000.0f))
                                {
                                    sc.zFar = fZFar;

                                    if (bIsMainLight)
                                    {
                                        this->pCameraMainLight->SetFarZ(fZFar);
                                        this->pCameraMainLight->UpdateProjectionMatrix();
                                    }
                                }
                                ImGui::Spacing();

                                //depthBiasConstant
                                float fDepthBiasConstant = sc.depthBiasConstant;
                                String nameDepthBiasConstant = "DepthBiasConstant - " + name;
                                if (ImGui::DragFloat(nameDepthBiasConstant.c_str(), &fDepthBiasConstant, 0.01f, 0.5f, 5.0f))
                                {
                                    sc.depthBiasConstant = fDepthBiasConstant;
                                }
                                ImGui::Spacing();

                                //depthBiasSlope
                                float fDepthBiasSlope = sc.depthBiasSlope;
                                String nameDepthBiasSlope = "DepthBiasSlope - " + name;
                                if (ImGui::DragFloat(nameDepthBiasSlope.c_str(), &fDepthBiasSlope, 0.01f, 1.0f, 10.0f))
                                {
                                    sc.depthBiasSlope = fDepthBiasSlope;
                                }
                                ImGui::Spacing();
                            }
                        }
                        void VulkanWindow::shadowReset()
                        {

                        }
                    void VulkanWindow::cullConfig()
                    {
                        if (!this->cfg_isRenderPassCull ||
                            this->m_pVKRenderPassCull == nullptr ||
                            this->m_pPipelineCompute_Cull == nullptr)
                        {
                            return;
                        }

                        if (ImGui::CollapsingHeader("Cull Settings"))
                        {
                            if (this->m_pPipelineCompute_Cull == nullptr)
                                return;

                            bool isChanged = false;

                            //isComputeCullFrustum
                            if (ImGui::Checkbox("Is ComputeCull Frustum", &this->isComputeCullFrustum))
                            {   
                                isChanged = true;
                            }
                            ImGui::Spacing();
                            
                            //isComputeCullFrustumHizDepth
                            if (ImGui::Checkbox("Is ComputeCull Frustum HizDepth", &this->isComputeCullFrustumHizDepth))
                            {   
                                isChanged = true;
                            }
                            ImGui::Spacing();

                            //forceNoCulling
                            bool isForceNoCulling = this->m_pPipelineCompute_Cull->cullCB.nIsNoCulling > 0 ? true : false;
                            if (ImGui::Checkbox("Is Force NoCulling", &isForceNoCulling))
                            {   
                                this->m_pPipelineCompute_Cull->cullCB.nIsNoCulling = isForceNoCulling ? 1 : 0;
                            }

                            if (isChanged)
                            {
                                if (this->m_pPipelineCompute_Cull->m_pCullManager != nullptr)
                                {
                                    this->m_pPipelineCompute_Cull->m_pCullManager->RefreshEnable();
                                }
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                    void VulkanWindow::terrainConfig()
                    {
                        if (!this->cfg_isRenderPassTerrain ||
                            this->m_pVKRenderPassTerrain == nullptr ||
                            this->m_pPipelineGraphics_Terrain == nullptr)
                        {
                            return;
                        }

                        if (ImGui::CollapsingHeader("Terrain Settings"))
                        {
                            //poTerrainInstanceIsDraw
                            bool isInstanceIsDraw = this->m_pVKRenderPassTerrain->poTerrainInstanceIsDraw;
                            if (ImGui::Checkbox("Instance Draw", &isInstanceIsDraw))
                            {   
                                this->m_pVKRenderPassTerrain->poTerrainInstanceIsDraw = isInstanceIsDraw;
                            }
                            ImGui::Spacing();

                            terrainConfigItem(this->m_pPipelineGraphics_Terrain->terrainCB, "Terrain");
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::terrainConfigItem(TerrainConstants& tc, const String& name)
                        {
                            if (ImGui::CollapsingHeader(name.c_str()))
                            {
                                bool isChange = false;

                                //heightStart
                                float fHeightStart = tc.heightStart;
                                String nameHeightStart = "HeightStart - " + name;
                                if (ImGui::DragFloat(nameHeightStart.c_str(), &fHeightStart, 0.05f, -500.0f, 1000.0f))
                                {
                                    tc.heightStart = fHeightStart;
                                    isChange = true;
                                }
                                ImGui::Spacing();

                                //heightMax
                                 float fHeightMax = tc.heightMax;
                                String nameHeightMax = "heightMax - " + name;
                                if (ImGui::DragFloat(nameHeightMax.c_str(), &fHeightMax, 0.05f, -500.0f, 1000.0f))
                                {
                                    tc.heightMax = fHeightMax;
                                    isChange = true;
                                }
                                ImGui::Spacing();

                                //aSplats
                                for (int i = 0; i < MAX_TERRAIN_SPLAT_COUNT; i++)
                                {
                                    String nameSplat = "Splat - " + FUtilString::SaveInt(i) + " - " + name;
                                    if (terrainConfigSplatItem(tc.aSplats[i], nameSplat))
                                    {
                                        isChange = true;
                                    }
                                }

                                if (isChange)
                                {
                                    UpdateBuffer_Graphics_Terrain();
                                }
                            }
                        }
                            bool VulkanWindow::terrainConfigSplatItem(TerrainSplatConstants& tsc, const String& name)
                            {
                                bool isChange = false;
                                if (ImGui::CollapsingHeader(name.c_str()))
                                {
                                    //splatSizeX
                                    float fSplatSizeX = tsc.splatSizeX;
                                    String nameSplatSizeX = "SplatSizeX - " + name;
                                    if (ImGui::DragFloat(nameSplatSizeX.c_str(), &fSplatSizeX, 0.05f, 1.0f, 2048.0f))
                                    {
                                        tsc.splatSizeX = fSplatSizeX;
                                        isChange = true;
                                    }
                                    ImGui::Spacing();

                                    //splatSizeY
                                    float fSplatSizeY = tsc.splatSizeY;
                                    String nameSplatSizeY = "SplatSizeY - " + name;
                                    if (ImGui::DragFloat(nameSplatSizeY.c_str(), &fSplatSizeY, 0.05f, 1.0f, 2048.0f))
                                    {
                                        tsc.splatSizeY = fSplatSizeY;
                                        isChange = true;
                                    }
                                    ImGui::Spacing();

                                    //splatOffsetX
                                    float fSplatOffsetX = tsc.splatOffsetX;
                                    String nameSplatOffsetX = "SplatOffsetX - " + name;
                                    if (ImGui::DragFloat(nameSplatOffsetX.c_str(), &fSplatOffsetX, 0.05f, 0.0f, 1024.0f))
                                    {
                                        tsc.splatOffsetX = fSplatOffsetX;
                                        isChange = true;
                                    }
                                    ImGui::Spacing();

                                    //splatOffsetY
                                    float fSplatOffsetY = tsc.splatOffsetY;
                                    String nameSplatOffsetY = "SplatOffsetY - " + name;
                                    if (ImGui::DragFloat(nameSplatOffsetY.c_str(), &fSplatOffsetY, 0.05f, 0.0f, 1024.0f))
                                    {
                                        tsc.splatOffsetY = fSplatOffsetY;
                                        isChange = true;
                                    }
                                    ImGui::Spacing();


                                    //diffuseRemapScale
                                    float fDiffuseRemapScale[4] = { tsc.diffuseRemapScale[0], tsc.diffuseRemapScale[1], tsc.diffuseRemapScale[2], tsc.diffuseRemapScale[3] };
                                    String namediffuseRemapScale = "DiffuseRemapScale - " + name;
                                    if (ImGui::DragFloat4(namediffuseRemapScale.c_str(), &fDiffuseRemapScale[0], 0.01f, 0.0f, 1.0f))
                                    {
                                        tsc.diffuseRemapScale.x = fDiffuseRemapScale[0];
                                        tsc.diffuseRemapScale.y = fDiffuseRemapScale[1];
                                        tsc.diffuseRemapScale.z = fDiffuseRemapScale[2];
                                        tsc.diffuseRemapScale.w = fDiffuseRemapScale[3];
                                        isChange = true;
                                    }
                                    ImGui::Spacing();

                                    //normalRemapScale
                                    float fNormalRemapScale = tsc.normalRemapScale;
                                    String nameNormalRemapScale = "NormalRemapScale - " + name;
                                    if (ImGui::DragFloat(nameNormalRemapScale.c_str(), &fNormalRemapScale, 0.01f, 0.1f, 1.0f))
                                    {
                                        tsc.normalRemapScale = fNormalRemapScale;
                                        isChange = true;
                                    }
                                    ImGui::Spacing();

                                }
                                return isChange;
                            }
                        void VulkanWindow::terrainReset()
                        {

                        }
                    void VulkanWindow::passConstantsConfig()
                    {
                        if (ImGui::CollapsingHeader("PassConstants Settings"))
                        {
                            //g_AmbientLight
                            if (ImGui::ColorEdit4("Global AmbientLight", (float*)&(this->passCB.g_AmbientLight)))
                            {
                                
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                    void VulkanWindow::modelConfig()
                    {

                    }

                void VulkanWindow::endRenderImgui()
                {
                    ImGui::Render();
                }
            void VulkanWindow::updateCBs_Editor()
            {
                if (this->pEditorGrid != nullptr)
                {
                    this->pEditorGrid->UpdateCBs();
                }
                if (this->pEditorCameraAxis != nullptr)
                {
                    this->pEditorCameraAxis->UpdateCBs();
                }
                if (this->pEditorCoordinateAxis != nullptr)
                {
                    this->pEditorCoordinateAxis->UpdateCBs();
                }
            }
            void VulkanWindow::updateCBs_Custom()
            {

            }

            void VulkanWindow::updateRenderCommandBuffers_CustomBeforeDefault()
            {
                
            }
            void VulkanWindow::updateRenderCommandBuffers_Default()
            {
                VkCommandBuffer& commandBuffer = this->poCommandBuffersGraphics[this->poSwapChainImageIndex];
                if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: Failed to reset render command buffer !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = 0; // Optional
                beginInfo.pInheritanceInfo = nullptr; // Optional

                if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: vkBeginCommandBuffer: Failed to begin recording render command buffer !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                {
                    updateRenderPass_SyncComputeGraphics(commandBuffer);
                    updateRenderPass_EditorCameraAxis(commandBuffer);

                    updateRenderPass_ShadowMap(commandBuffer);
                    updateRenderPass_CustomBeforeDefault(commandBuffer);
                    {
                        updateRenderPass_Default(commandBuffer);
                    }
                    updateRenderPass_CustomAfterDefault(commandBuffer);
                    updateRenderPass_BlitFromFrame(commandBuffer);
                    updateRenderPass_DepthHiz(commandBuffer);
                }
                if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: vkEndCommandBuffer: Failed to record render command buffer !";
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
                void VulkanWindow::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::updateRenderPass_EditorCameraAxis(VkCommandBuffer& commandBuffer)
                {
                    if (this->pEditorCameraAxis == nullptr ||
                        !this->cfg_isEditorCameraAxisShow)
                    {
                        return;
                    }

                    VKMultiRenderPass* pRenderPass = this->pEditorCameraAxis->pPipelineGraphics->pRenderPass;
                    if (pRenderPass == nullptr)
                        return;

                    beginRenderPass(commandBuffer,
                                    "[RenderPass-EditorCameraAxis]",
                                    pRenderPass->poRenderPass,
                                    pRenderPass->poFrameBuffer,
                                    this->pEditorCameraAxis->poOffset,
                                    this->pEditorCameraAxis->poExtent,
                                    this->pEditorCameraAxis->poColorBackground,
                                    1.0f,
                                    0);
                    {
                        //1> Viewport
                        bindViewport(commandBuffer, this->pEditorCameraAxis->poViewport, this->pEditorCameraAxis->poScissor);

                        //2> Render CameraAxis
                        this->pEditorCameraAxis->Draw(commandBuffer);
                    } 
                    endRenderPass(commandBuffer);
                }
                void VulkanWindow::updateRenderPass_ShadowMap(VkCommandBuffer& commandBuffer)
                {
                    if (this->m_pVKRenderPassShadowMap == nullptr ||
                        !this->cfg_isRenderPassShadowMap)
                    {
                        return;
                    }

                    beginRenderPass(commandBuffer,
                                    "[RenderPass-ShadowMap]",
                                    this->m_pVKRenderPassShadowMap->poRenderPass,
                                    this->m_pVKRenderPassShadowMap->poFrameBuffer,
                                    this->m_pVKRenderPassShadowMap->offset,
                                    this->m_pVKRenderPassShadowMap->extent,
                                    this->m_pVKRenderPassShadowMap->aClearValue);
                     {
                        //1> Viewport
                        bindViewport(commandBuffer, this->m_pVKRenderPassShadowMap->viewPort, this->m_pVKRenderPassShadowMap->rtScissor);
                        
                        //2> DepthBias
                        setDepthBias(commandBuffer, this->shadowMainLight.depthBiasConstant, 0.0f, this->shadowMainLight.depthBiasSlope);

                        //3> Showmap Render
                        drawMeshShadowMap(commandBuffer);
                    } 
                    endRenderPass(commandBuffer);
                }
                    void VulkanWindow::drawMeshShadowMap(VkCommandBuffer& commandBuffer)
                    {

                    }
                void  VulkanWindow::updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer)
                {
                    
                }
                void VulkanWindow::updateRenderPass_Default(VkCommandBuffer& commandBuffer)
                {
                    updateMeshDefault_Before(commandBuffer);
                    {
                        beginRenderPass(commandBuffer,
                                        "[RenderPass-Default]",
                                        this->poRenderPass,
                                        this->poSwapChainFrameBuffers[this->poSwapChainImageIndex],
                                        this->poOffset,
                                        this->poExtent,
                                        this->cfg_colorBackground,
                                        1.0f,
                                        0);
                        {
                            //1> Viewport
                            bindViewport(commandBuffer, this->poViewport, this->poScissor);
                        
                            //2> Default
                            drawMeshDefault(commandBuffer);
                            drawMeshTerrain(commandBuffer);
                            drawMeshDefault_Custom(commandBuffer);
                            drawMeshDefault_Editor(commandBuffer);
                            drawMeshDefault_CustomBeforeImgui(commandBuffer);

                            //3> ImGui 
                            drawMeshDefault_Imgui(commandBuffer);
                        }
                        endRenderPass(commandBuffer);
                    }
                    updateMeshDefault_After(commandBuffer);
                }
                    void VulkanWindow::updateMeshDefault_Before(VkCommandBuffer& commandBuffer)
                    {

                    }
                        void VulkanWindow::drawMeshDefault(VkCommandBuffer& commandBuffer)
                        {
                            if (this->poVertexBuffer == nullptr)
                                return;

                            VkPipeline vkPipeline = this->poPipelineGraphics;
                            if (this->cfg_isWireFrame)
                                vkPipeline = this->poPipelineGraphics_WireFrame;
                            bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

                            VkBuffer vertexBuffers[] = { this->poVertexBuffer };
                            VkDeviceSize offsets[] = { 0 };
                            bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);

                            if (this->poDescriptorSets.size() > 0)
                            {
                                bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &this->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
                            }
                            if (this->poIndexBuffer != VK_NULL_HANDLE)
                            {
                                bindIndexBuffer(commandBuffer, this->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

                                drawIndexed(commandBuffer, this->poIndexCount, 1, 0, 0, 0);
                            }
                            else
                            {
                                draw(commandBuffer, this->poVertexCount, 1, 0, 0);
                            }
                        }
                        void VulkanWindow::drawMeshTerrain(VkCommandBuffer& commandBuffer)
                        {
                            if (!this->cfg_isRenderPassTerrain || 
                                this->m_pVKRenderPassTerrain == nullptr ||
                                this->m_pPipelineGraphics_Terrain == nullptr)
                                return;
                            
                            Draw_Graphics_Terrain(commandBuffer);
                        }
                        void VulkanWindow::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
                        {

                        }
                        void VulkanWindow::drawMeshDefault_Editor(VkCommandBuffer& commandBuffer)
                        {
                            if (this->pEditorGrid != nullptr)
                            {
                                if (this->cfg_isEditorGridShow)
                                {
                                    this->pEditorGrid->Draw(commandBuffer);
                                }
                            }
                            if (this->pEditorCameraAxis != nullptr)
                            {
                                if (this->cfg_isEditorCameraAxisShow)
                                {
                                    this->pEditorCameraAxis->DrawQuad(commandBuffer);
                                }
                            }
                            if (this->pEditorCoordinateAxis != nullptr)
                            {
                                if (this->cfg_isEditorCoordinateAxisShow)
                                {
                                    this->pEditorCoordinateAxis->Draw(commandBuffer);
                                }
                            }
                            if (this->pEditorLineFlat2DCollector != nullptr)
                            {
                                this->pEditorLineFlat2DCollector->Draw(commandBuffer);
                            }
                            if (this->pEditorLineFlat3DCollector != nullptr)
                            {
                                this->pEditorLineFlat3DCollector->Draw(commandBuffer);
                            }
                        }
                        void VulkanWindow::drawMeshDefault_CustomBeforeImgui(VkCommandBuffer& commandBuffer)
                        {

                        }
                        void VulkanWindow::drawMeshDefault_Imgui(VkCommandBuffer& commandBuffer)
                        {
                            if (HasConfig_Imgui())
                            {
                                ImDrawData* main_draw_data = ImGui::GetDrawData();
                                vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
                                ImGui_ImplVulkan_RenderDrawData(main_draw_data, commandBuffer);
                            }
                        }
                    void VulkanWindow::updateMeshDefault_After(VkCommandBuffer& commandBuffer)
                    {

                    }
                void VulkanWindow::updateRenderPass_CustomAfterDefault(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::updateRenderPass_BlitFromFrame(VkCommandBuffer& commandBuffer)
                {
                    if (this->cfg_isUseCopyBlitFromFrameColor &&
                        this->m_pPipelineGraphics_CopyBlitFromFrameColor != nullptr)
                    {
                        updateBlitFromFrame_Color_Before(commandBuffer);
                        {
                            beginRenderPass(commandBuffer,
                                            "[RenderPass-CopyBlitFromFrameColor]",
                                            this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->poRenderPass,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->poFrameBuffer,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->offset,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->extent,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->aClearValue);
                            {
                                //1> Viewport
                                bindViewport(commandBuffer, this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->viewPort, this->m_pPipelineGraphics_CopyBlitFromFrameColor->pVKRenderPassCopyBlitFromFrame->rtScissor);
                            
                                //2> CopyBlitFromFrame Color
                                drawBlitFromFrame_Color(commandBuffer);
                            }
                            endRenderPass(commandBuffer);
                        }
                        updateBlitFromFrame_Color_After(commandBuffer);
                    }
                    if (this->cfg_isUseCopyBlitFromFrameDepth &&
                        this->m_pPipelineGraphics_CopyBlitFromFrameDepth != nullptr)
                    {
                        updateBlitFromFrame_Depth_Before(commandBuffer);
                        {
                            beginRenderPass(commandBuffer,
                                            "[RenderPass-CopyBlitFromFrameDepth]",
                                            this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->poRenderPass,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->poFrameBuffer,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->offset,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->extent,
                                            this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->aClearValue);
                            {
                                //1> Viewport
                                bindViewport(commandBuffer, this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->viewPort, this->m_pPipelineGraphics_CopyBlitFromFrameDepth->pVKRenderPassCopyBlitFromFrame->rtScissor);
                            
                                //2> CopyBlitFromFrame Depth
                                drawBlitFromFrame_Depth(commandBuffer);
                            }
                            endRenderPass(commandBuffer);
                        }
                        updateBlitFromFrame_Depth_After(commandBuffer);
                    }
                }
                    void VulkanWindow::updateBlitFromFrame_Color_Before(VkCommandBuffer& commandBuffer)
                    {
                        transitionImageLayout(commandBuffer,
                                              this->poSwapChainImages[this->poSwapChainImageIndex],
                                              VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                              0,
                                              1,
                                              0,
                                              1,
                                              VK_IMAGE_ASPECT_COLOR_BIT);

                        UpdateDescriptorSets_Graphics_CopyBlitFromFrame(this->m_pPipelineGraphics_CopyBlitFromFrameColor, this->poSwapChainImageViews[this->poSwapChainImageIndex]);
                    }
                        void VulkanWindow::drawBlitFromFrame_Color(VkCommandBuffer& commandBuffer)
                        {
                            Draw_Graphics_CopyBlitFromFrame(commandBuffer, this->m_pPipelineGraphics_CopyBlitFromFrameColor);
                        }
                    void VulkanWindow::updateBlitFromFrame_Color_After(VkCommandBuffer& commandBuffer)
                    {

                    }
                    void VulkanWindow::updateBlitFromFrame_Depth_Before(VkCommandBuffer& commandBuffer)
                    {
                        transitionImageLayout(commandBuffer,
                                              this->poDepthImage,
                                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                              0,
                                              1,
                                              0,
                                              1,
                                              VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);

                        UpdateDescriptorSets_Graphics_CopyBlitFromFrame(this->m_pPipelineGraphics_CopyBlitFromFrameDepth, this->poDepthImageView_Depth);
                    }
                        void VulkanWindow::drawBlitFromFrame_Depth(VkCommandBuffer& commandBuffer)
                        {
                            Draw_Graphics_CopyBlitFromFrame(commandBuffer, this->m_pPipelineGraphics_CopyBlitFromFrameDepth);
                        }
                    void VulkanWindow::updateBlitFromFrame_Depth_After(VkCommandBuffer& commandBuffer)
                    {

                    }
                void VulkanWindow::updateRenderPass_DepthHiz(VkCommandBuffer& commandBuffer)
                {
                    if (this->m_pVKRenderPassCull == nullptr ||
                        !this->isComputeCullFrustumHizDepth ||
                        this->m_pPipelineGraphics_DepthHiz == nullptr)
                    {
                        return;
                    }
                    
                    updateMeshDepthHiz_Before(commandBuffer);
                    {
                        beginRenderPass(commandBuffer,
                                        "[RenderPass-DepthHiz]",
                                        this->m_pVKRenderPassCull->poRenderPass,
                                        this->m_pVKRenderPassCull->poFrameBuffer,
                                        this->m_pVKRenderPassCull->offset,
                                        this->m_pVKRenderPassCull->extent,
                                        this->m_pVKRenderPassCull->aClearValue);
                        {
                            //1> Viewport
                            bindViewport(commandBuffer, this->m_pVKRenderPassCull->viewPort, this->m_pVKRenderPassCull->rtScissor);
                        
                            //2> DepthHiz
                            drawMeshDepthHiz(commandBuffer);
                        }
                        endRenderPass(commandBuffer);
                    }
                    updateMeshDepthHiz_After(commandBuffer);
                }
                    void VulkanWindow::updateMeshDepthHiz_Before(VkCommandBuffer& commandBuffer)
                    {
                        transitionImageLayout(commandBuffer,
                                              this->poDepthImage,
                                              VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                              0,
                                              1,
                                              0,
                                              1,
                                              VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
                    }
                        void VulkanWindow::drawMeshDepthHiz(VkCommandBuffer& commandBuffer)
                        {
                            Draw_Graphics_DepthHiz(commandBuffer);
                        }
                    void VulkanWindow::updateMeshDepthHiz_After(VkCommandBuffer& commandBuffer)
                    {
                        
                    }


                    void VulkanWindow::beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                       const String& nameRenderPass,
                                                       const VkRenderPass& renderPass, 
                                                       const VkFramebuffer& frameBuffer,
                                                       const VkOffset2D& offset,
                                                       const VkExtent2D& extent,
                                                       const FVector4& clBg,
                                                       float depth,
                                                       uint32_t stencil)
                    {
                        FVector4Vector aColors;
                        aColors.push_back(clBg);
                        if (!this->cfg_colorValues.empty())
                        {
                            for (size_t i = 0; i < this->cfg_colorValues.size(); i++)
                            {
                                aColors.push_back(this->cfg_colorValues[i]);
                            }
                        }
                        beginRenderPass(commandBuffer,
                                        nameRenderPass,
                                        renderPass,
                                        frameBuffer,
                                        offset,
                                        extent,
                                        aColors,
                                        depth,
                                        stencil);
                    }
                    void VulkanWindow::beginRenderPass(VkCommandBuffer& commandBuffer,  
                                                       const String& nameRenderPass,
                                                       const VkRenderPass& renderPass, 
                                                       const VkFramebuffer& frameBuffer,
                                                       const VkOffset2D& offset,
                                                       const VkExtent2D& extent,
                                                       const FVector4Vector& aColors,
                                                       float depth,
                                                       uint32_t stencil)
                    {
                        size_t count = aColors.size();
                        std::vector<VkClearValue> clearValues;
                        clearValues.resize(count + 1);
                        for (size_t i = 0; i < count; i++)
                        {
                            const FVector4& vColor = aColors[i];
                            VkClearColorValue colorValue = { vColor.x, vColor.y, vColor.z, vColor.w };
                            clearValues[i].color = colorValue;
                        }
                        VkClearDepthStencilValue depthStencilValue;
                        depthStencilValue.depth = depth;
                        depthStencilValue.stencil = stencil;
                        clearValues[count].depthStencil = depthStencilValue;

                        beginRenderPass(commandBuffer,
                                        nameRenderPass,
                                        renderPass,
                                        frameBuffer,
                                        offset,
                                        extent,
                                        clearValues);
                    }
                    void VulkanWindow::beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                       const String& nameRenderPass,
                                                       const VkRenderPass& renderPass, 
                                                       const VkFramebuffer& frameBuffer,
                                                       const VkOffset2D& offset,
                                                       const VkExtent2D& extent,
                                                       const VkClearValueVector& aClearValue)
                    {
                        VkRenderPassBeginInfo renderPassInfo = {};
                        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                        renderPassInfo.renderPass = renderPass;
                        renderPassInfo.framebuffer = frameBuffer;
                        renderPassInfo.renderArea.offset = offset;
                        renderPassInfo.renderArea.extent = extent;
                        renderPassInfo.clearValueCount = static_cast<uint32_t>(aClearValue.size());
                        renderPassInfo.pClearValues = aClearValue.data();

                        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

                        VkDebugUtilsLabelEXT label = {};
                        label.sType =  VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT;
                        label.pLabelName = nameRenderPass.c_str();
                        this->poDebug->BeginCmdLabel(commandBuffer, &label);
                    }
                        void VulkanWindow::bindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor)
                        {
                            VkViewport viewport = vkViewport;
                            if (cfg_isNegativeViewport)
                            {
                                viewport.y = viewport.height - viewport.y;
                                viewport.height = -viewport.height;
                            }   
                            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
                            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
                        }
                        void VulkanWindow::setDepthBias(VkCommandBuffer& commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor)
                        {
                            vkCmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
                        }
                        void VulkanWindow::setPrimitiveTopology(VkCommandBuffer& commandBuffer, VkPrimitiveTopology vkPrimitiveTopology)
                        {
                            vkCmdSetPrimitiveTopology(commandBuffer, vkPrimitiveTopology);
                        }
                        void VulkanWindow::bindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline)
                        {
                            vkCmdBindPipeline(commandBuffer, pipelineBindPoint, vkPipeline);
                        }
                        void VulkanWindow::bindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
                        {
                            vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
                        }
                        void VulkanWindow::bindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType)
                        {
                            vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, offset, indexType);
                        }
                        void VulkanWindow::bindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
                        {
                            vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
                        }
                        void VulkanWindow::bindPipelineAndDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipeline& vkPipeline, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
                        {
                            bindPipeline(commandBuffer, pipelineBindPoint, vkPipeline);
                            bindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
                        }
                        void VulkanWindow::draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
                        {
                            vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
                        }
                        void VulkanWindow::drawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
                        {   
                            vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
                        }
                        void VulkanWindow::drawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
                        {
                            vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
                        }
                        void VulkanWindow::drawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
                        {
                            vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
                        }
                    void VulkanWindow::endRenderPass(VkCommandBuffer& commandBuffer)
                    {
                        vkCmdEndRenderPass(commandBuffer);

                        this->poDebug->EndCmdLabel(commandBuffer);
                    }

                    void VulkanWindow::dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ)
                    {
                        vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
                    }
                    void VulkanWindow::dispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset)
                    {
                        vkCmdDispatchIndirect(commandBuffer, buffer, offset);
                    }   

                    void VulkanWindow::pipelineBarrier(VkCommandBuffer& commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers)
                    {
                        vkCmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount, pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount, pImageMemoryBarriers);
                    }

        void VulkanWindow::updateRenderCommandBuffers_CustomAfterDefault()
        {   

        }


        void VulkanWindow::render()
        {
            //1> Wait
            if (this->poImagesInFlight[this->poSwapChainImageIndex] != VK_NULL_HANDLE)
            {
                waitForFence(this->poImagesInFlight[this->poSwapChainImageIndex], VK_TRUE, UINT64_MAX);
            }
            this->poImagesInFlight[this->poSwapChainImageIndex] = this->poInFlightFences[this->poCurrentFrame];

            //2> Submit Command
            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

            VkSemaphoreVector aWaitSemaphores;
            if (this->poComputeBeforeWaitSemaphore != VK_NULL_HANDLE)
                aWaitSemaphores.push_back(this->poComputeBeforeWaitSemaphore);
            aWaitSemaphores.push_back(this->poPresentCompleteSemaphores[this->poCurrentFrame]);

            VkSemaphoreVector aSignalSemaphores;
            if (this->poGraphicsWaitComputeBeforeSemaphore != VK_NULL_HANDLE)
                aSignalSemaphores.push_back(this->poGraphicsWaitComputeBeforeSemaphore);
            if (this->poGraphicsSignalComputeAfterSemaphore != VK_NULL_HANDLE)
               aSignalSemaphores.push_back(this->poGraphicsSignalComputeAfterSemaphore);
            aSignalSemaphores.push_back(this->poRenderCompleteSemaphores[this->poCurrentFrame]);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            //Command Buffer
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBuffersGraphics[this->poSwapChainImageIndex];
            //WaitSemaphores
            submitInfo.waitSemaphoreCount = static_cast<uint32_t>(aWaitSemaphores.size());
            submitInfo.pWaitSemaphores = aWaitSemaphores.data();
            //WaitDstStageMask
            submitInfo.pWaitDstStageMask = waitStages;
            //SignalSemaphores
            submitInfo.signalSemaphoreCount = static_cast<uint32_t>(aSignalSemaphores.size());
            submitInfo.pSignalSemaphores = aSignalSemaphores.data();
            resetVkFence(this->poInFlightFences[this->poCurrentFrame]);
            if (vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, this->poInFlightFences[this->poCurrentFrame]) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::render: Failed to submit render command buffer !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            
        }
    void VulkanWindow::endRender()
    {
        
    }

    void VulkanWindow::cleanup()
    {
        F_LogInfo("---------- VulkanWindow::cleanup start ----------");
        {
            //0> cleanupSwapChain
            cleanupSwapChain();

            //1> cleanupCustom/cleanupEditor/cleanupImGUI/cleanupDefault/cleanupInternal
            cleanupCustom();
            cleanupEditor();
            cleanupImGUI();
            cleanupDefault();
            cleanupInternal();

            //2> VkPipelineCache
            destroyVkPipelineCache(this->poPipelineCache);
            this->poPipelineCache = VK_NULL_HANDLE;

            //3> Semaphores
            for (size_t i = 0; i < s_maxFramesInFight; i++) 
            {
                destroyVkSemaphore(this->poRenderCompleteSemaphores[i]);
                destroyVkSemaphore(this->poPresentCompleteSemaphores[i]);
                destroyVkFence(this->poInFlightFences[i]);
            }
            this->poRenderCompleteSemaphores.clear();
            this->poPresentCompleteSemaphores.clear();
            this->poInFlightFences.clear();

            destroyVkSemaphore(this->poGraphicsWaitComputeBeforeSemaphore);
            this->poGraphicsWaitComputeBeforeSemaphore = VK_NULL_HANDLE;
            destroyVkSemaphore(this->poGraphicsSignalComputeAfterSemaphore);
            this->poGraphicsSignalComputeAfterSemaphore = VK_NULL_HANDLE;
            destroyVkSemaphore(this->poComputeBeforeWaitSemaphore);
            this->poComputeBeforeWaitSemaphore = VK_NULL_HANDLE;
            destroyVkSemaphore(this->poComputeAfterWaitSemaphore);
            this->poComputeAfterWaitSemaphore = VK_NULL_HANDLE;

            //4> CommandPool
            destroyVkCommandPool(this->poCommandPoolGraphics);
            destroyVkCommandPool(this->poCommandPoolCompute);
            
            //5> Device
            F_DELETE(this->poDebug)
            destroyVkDevice(this->poDevice);
            this->poDevice = VK_NULL_HANDLE;
            if (s_isEnableValidationLayers)
            {
                destroyDebugUtilsMessengerEXT(this->poInstance, this->poDebugMessenger, nullptr);
            }

            //6> Surface
            destroyVkSurfaceKHR(this->poSurface);
            this->poSurface = VK_NULL_HANDLE;

            //7> Instance
            destroyVkInstance(this->poInstance);
            this->poInstance = VK_NULL_HANDLE;
        }
        F_LogInfo("---------- VulkanWindow::cleanup finish ----------");
    }
        void VulkanWindow::cleanupDefault()
        {
            destroyVkDescriptorSetLayout(this->poDescriptorSetLayout);
            cleanupTexture();
            cleanupVertexIndexBuffer();
        }
            void VulkanWindow::cleanupTexture()
            {
                destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
                destroyVkImageSampler(this->poTextureSampler);
                this->poTextureImage = VK_NULL_HANDLE;
                this->poTextureImageMemory = VK_NULL_HANDLE;
                this->poTextureImageView = VK_NULL_HANDLE;
                this->poTextureSampler = VK_NULL_HANDLE;            
            }
            void VulkanWindow::cleanupVertexIndexBuffer()
            {
                //1> VertexBuffer
                if (this->poVertexBuffer != VK_NULL_HANDLE)
                {
                    destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
                }
                this->poVertexBuffer = VK_NULL_HANDLE;
                this->poVertexBufferMemory = VK_NULL_HANDLE;
                this->poVertexCount = 0;
                this->poVertexBuffer_Size = 0;
                this->poVertexBuffer_Data = nullptr;

                //2> IndexBuffer
                if (this->poIndexBuffer != VK_NULL_HANDLE)
                {
                    destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
                }
                this->poIndexBuffer = VK_NULL_HANDLE;
                this->poIndexBufferMemory = VK_NULL_HANDLE;
                this->poIndexCount = 0;
                this->poIndexBuffer_Size = 0;
                this->poIndexBuffer_Data = nullptr;
            }
        void VulkanWindow::cleanupImGUI()
        {
            destroyVkDescriptorPool(this->imgui_DescriptorPool);
            this->imgui_DescriptorPool = VK_NULL_HANDLE;

            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_Shutdown();
	            ImGui_ImplGlfw_Shutdown();
            }
        }
        void VulkanWindow::cleanupEditor()
        {
            destroyEditor();
        }
        void VulkanWindow::cleanupCustom()
        {
            
        }
        
        void VulkanWindow::cleanupSwapChain()
        {
            F_LogInfo("----- VulkanWindow::cleanupSwapChain start -----");
            {
                //0> Custom/Editor/Terrain/Default
                cleanupSwapChain_Custom();
                cleanupSwapChain_Editor();
                cleanupSwapChain_Default();
                destroyResourceInternal();

                //1> DepthImage/ColorImage
                if (this->poDepthImageView_Depth != VK_NULL_HANDLE)
                    destroyVkImageView(this->poDepthImageView_Depth);
                this->poDepthImageView_Depth = VK_NULL_HANDLE;
                if (this->poDepthImageView_Stencil != VK_NULL_HANDLE)
                    destroyVkImageView(this->poDepthImageView_Stencil);
                this->poDepthImageView_Stencil = VK_NULL_HANDLE;
                destroyVkImage(this->poDepthImage, this->poDepthImageMemory, this->poDepthImageView);
                this->poDepthImage = VK_NULL_HANDLE;
                this->poDepthImageMemory = VK_NULL_HANDLE;
                this->poDepthImageView = VK_NULL_HANDLE;
                destroyVkImage(this->poColorImage, this->poColorImageMemory, this->poColorImageView);
                this->poColorImage = VK_NULL_HANDLE;
                this->poColorImageMemory = VK_NULL_HANDLE;
                this->poColorImageView = VK_NULL_HANDLE;
                size_t count = this->poColorImageLists.size();
                for (size_t i = 0; i < count; i++)
                {
                    destroyVkImage(this->poColorImageLists[i], this->poColorImageMemoryLists[i], this->poColorImageViewLists[i]);
                }
                this->poColorImageLists.clear();
                this->poColorImageMemoryLists.clear();
                this->poColorImageViewLists.clear();
                
                //2> SwapChainFrameBuffers
                count = this->poSwapChainFrameBuffers.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkFramebuffer& frameBuffer = this->poSwapChainFrameBuffers[i];
                    destroyVkFramebuffer(frameBuffer);
                }
                this->poSwapChainFrameBuffers.clear();

                //3> CommandBuffers
                if (this->poCommandBuffersGraphics.size() > 0)
                {
                    freeCommandBuffers(this->poCommandPoolGraphics, (uint32_t)this->poCommandBuffersGraphics.size(), this->poCommandBuffersGraphics.data());
                }
                this->poCommandBuffersGraphics.clear();
                if (this->poCommandPoolCompute != VK_NULL_HANDLE)
                {
                    if (this->poCommandBufferComputeBefore != VK_NULL_HANDLE)
                    {
                        freeCommandBuffers(this->poCommandPoolCompute, 1, &(this->poCommandBufferComputeBefore));
                        this->poCommandBufferComputeBefore = VK_NULL_HANDLE;
                    }
                    if (this->poCommandBufferComputeAfter != VK_NULL_HANDLE)
                    {
                        freeCommandBuffers(this->poCommandPoolCompute, 1, &(this->poCommandBufferComputeAfter));
                        this->poCommandBufferComputeAfter = VK_NULL_HANDLE;
                    }
                }
                
                //4> RenderPass
                destroyVkRenderPass(this->poRenderPass);
                this->poRenderPass = VK_NULL_HANDLE;

                //5> SwapChainImageViews
                count = this->poSwapChainImageViews.size();
                for (size_t i = 0; i < count; i++)
                {
                    destroyVkImageView(this->poSwapChainImageViews[i]);
                }
                this->poSwapChainImageViews.clear();
                destroyVkSwapchainKHR(this->poSwapChain);
                this->poSwapChain = VK_NULL_HANDLE;

                //6> DescriptorPool
                destroyVkDescriptorPool(this->poDescriptorPool);
                this->poDescriptorPool = VK_NULL_HANDLE;

                //7> VkPipelineCache
                destroyVkPipelineCache(this->poPipelineCache);
                this->poPipelineCache = VK_NULL_HANDLE;
            }
            F_LogInfo("----- VulkanWindow::cleanupSwapChain finish -----");
        }
            void VulkanWindow::cleanupSwapChain_Default()
            {
                size_t count = 0;

                //1> ConstBuffers
                count = this->poBuffers_ObjectCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
                }
                this->poBuffers_ObjectCB.clear();
                this->poBuffersMemory_ObjectCB.clear();

                count = this->poBuffers_MaterialCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_MaterialCB[i], this->poBuffersMemory_MaterialCB[i]);
                }
                this->poBuffers_MaterialCB.clear();
                this->poBuffersMemory_MaterialCB.clear();

                count = this->poBuffers_InstanceCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_InstanceCB[i], this->poBuffersMemory_InstanceCB[i]);
                }
                this->poBuffers_InstanceCB.clear();
                this->poBuffersMemory_InstanceCB.clear();

                //2> Pipelines
                destroyVkPipeline(this->poPipelineGraphics);
                this->poPipelineGraphics = VK_NULL_HANDLE;
                destroyVkPipeline(this->poPipelineGraphics_WireFrame);
                this->poPipelineGraphics_WireFrame = VK_NULL_HANDLE;

                //3> PipelineLayout
                destroyVkPipelineLayout(this->poPipelineLayout);
                this->poPipelineLayout = VK_NULL_HANDLE;
            }
            void VulkanWindow::cleanupSwapChain_Editor()
            {
                if (this->pEditorGrid != nullptr)
                {
                    this->pEditorGrid->CleanupSwapChain();
                }
                if (this->pEditorCameraAxis != nullptr)
                {
                    this->pEditorCameraAxis->CleanupSwapChain();
                }
                if (this->pEditorCoordinateAxis != nullptr)
                {
                    this->pEditorCoordinateAxis->CleanupSwapChain();
                }
                if (this->pEditorLineFlat2DCollector != nullptr)
                {
                    this->pEditorLineFlat2DCollector->CleanupSwapChain();
                }
                if (this->pEditorLineFlat3DCollector != nullptr)
                {
                    this->pEditorLineFlat3DCollector->CleanupSwapChain();
                }
            }
            void VulkanWindow::cleanupSwapChain_Custom()
            {

            }
    void VulkanWindow::recreateSwapChain()
    {
        F_LogInfo("++++++++++ VulkanWindow::recreateSwapChain start ++++++++++");
        {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(this->pWindow, &width, &height);
            while (width == 0 || height == 0)
            {
                glfwGetFramebufferSize(this->pWindow, &width, &height);
                glfwWaitEvents();
                if (glfwWindowShouldClose(this->pWindow)) 
                {
                    //Closing a minimized window
                    cleanup();
                    this->isMinimizedWindowNeedClose = true;
                    return;
                }
            }
            
            vkDeviceWaitIdle(this->poDevice);

            cleanupSwapChain();
            
            createDescriptorPool();
            createSwapChain();
            createSwapChainImageViews();
            if (HasConfig_MASS())
            {
                createColorResources();
            }  
            createDepthResources();
            createRenderPasses();
            createFramebuffers();

            createVkPipelineCache();
            createResourceInternal();
            
            createConstBuffers();
            
            createCustomBeforePipeline();
            createGraphicsPipeline();
            createComputePipeline();
            createDescriptorSets();
            createCommandBuffers();

            recreateSwapChain_Editor();
            recreateSwapChain_Custom();

            this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_SetMinImageCount(this->imgui_MinimalSwapchainImages);
            }
            cameraReset();
        }
        F_LogInfo("++++++++++ VulkanWindow::recreateSwapChain finish ++++++++++");
    }
        void VulkanWindow::recreateSwapChain_Editor()
        {
            if (this->pEditorGrid != nullptr)
            {
                this->pEditorGrid->RecreateSwapChain();
            }
            if (this->pEditorCameraAxis != nullptr)
            {
                this->pEditorCameraAxis->RecreateSwapChain();
            }
            if (this->pEditorCoordinateAxis != nullptr)
            {
                this->pEditorCoordinateAxis->RecreateSwapChain();
            }
            if (this->pEditorLineFlat2DCollector != nullptr)
            {
                this->pEditorLineFlat2DCollector->RecreateSwapChain();
            }
            if (this->pEditorLineFlat3DCollector != nullptr)
            {
                this->pEditorLineFlat3DCollector->RecreateSwapChain();
            }
        }
        void VulkanWindow::recreateSwapChain_Custom()
        {

        }

    VkResult VulkanWindow::createDebugUtilsMessengerEXT(VkInstance instance, 
                                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                        const VkAllocationCallbacks* pAllocator,
                                                        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) 
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } 
        else 
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanWindow::destroyDebugUtilsMessengerEXT(VkInstance instance,
                                                     VkDebugUtilsMessengerEXT debugMessenger,
                                                     const VkAllocationCallbacks* pAllocator)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) 
        {
            func(instance, debugMessenger, pAllocator);
        }
    } 

}; //LostPeterVulkan