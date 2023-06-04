/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPreDefine.h"

namespace LostPeterFoundation
{
    ////////////////////////////// Enum ////////////////////////////////
    //FMeshType
    static const String s_nameMeshTypes[] = 
    {
        "file",                     //0:    File
        "geometry",                 //1:    Geometry
    };
    const String& F_GetMeshTypeName(FMeshType type)
    {
        return s_nameMeshTypes[(int)type];
    }
    const String& F_GetMeshTypeName(int type)
    {
        return s_nameMeshTypes[(int)type];
    }
    FMeshType F_ParseMeshType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Mesh_Count; i++)
        {
            if (s_nameMeshTypes[i] == strName)
                return (FMeshType)(i);
        }
        assert(false && "F_ParseMeshType: Wrong type name !");
        return F_Mesh_File;
    }


    //FMeshGeometryType
    static const String s_nameMeshGeometryTypes[] = 
    {
        "triangle",                 //0:    Triangle
        "quad",                     //1:    Quad
        "grid",                     //2:    Grid
        "circle",                   //3:    Circle
        "aabb",                     //4:    AABB
        "sphere",                   //5:    Sphere
        "geosphere",                //6:    GeoSphere
        "cylinder",                 //7:    Cylinder
        "capsule",                  //8:    Capsule
        "cone",                     //9:    Cone
        "torus",                    //10:   Torus
        "skybox",                   //11:   SkyBox
        "skydome",                  //12:   SkyDome
        "terrain",                  //13:   Terrain
    };
    const String& F_GetMeshGeometryTypeName(FMeshGeometryType type)
    {
        return s_nameMeshGeometryTypes[(int)type];
    }
    const String& F_GetMeshGeometryTypeName(int type)
    {
        return s_nameMeshGeometryTypes[type];
    }
    FMeshGeometryType F_ParseMeshGeometryType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_MeshGeometry_Count; i++)
        {
            if (s_nameMeshGeometryTypes[i] == strName)
                return (FMeshGeometryType)(i);
        }
        assert(false && "F_ParseMeshGeometryType: Wrong type name !");
        return F_MeshGeometry_Triangle;
    }
    

    //F_MeshVertexType
    static const String s_nameMeshVertexTypes[] = 
    {
        "Pos2Color4",                               //0:    Pos2Color4
        "Pos3Normal3",                              //1:    Pos3Normal3
        "Pos3Normal3Tex2",                          //2:    Pos3Normal3Tex2
        "Pos2Color4Tex2",                           //3:    Pos2Color4Tex2
        "Pos3Color4Tex2",                           //4:    Pos3Color4Tex2
        "Pos3Color4Normal3Tex2",                    //5:    Pos3Color4Normal3Tex2
        "Pos3Color4Normal3Tex4",                    //6:    Pos3Color4Normal3Tex4
        "Pos3Color4Normal3Tangent3Tex2",            //7:    Pos3Color4Normal3Tangent3Tex2
        "Pos3Color4Normal3Tangent3Tex4",            //8:    Pos3Color4Normal3Tangent3Tex4
        "Pos3Normal3Tangent3BlendWI8Tex2",          //9:    Pos3Normal3Tangent3BlendWI8Tex2
        "Pos3Color4Normal3Tangent3BlendWI8Tex2",    //10:   Pos3Color4Normal3Tangent3BlendWI8Tex2
    };
    const String& F_GetMeshVertexTypeName(F_MeshVertexType type)
    {
        return s_nameMeshVertexTypes[(int)type];
    }
    const String& F_GetMeshVertexTypeName(int type)
    {
        return s_nameMeshVertexTypes[(int)type];
    }
    F_MeshVertexType F_ParseMeshVertexType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_MeshVertex_Count; i++)
        {
            if (s_nameMeshVertexTypes[i] == strName)
                return (F_MeshVertexType)(i);
        }
        assert(false && "F_ParseMeshVertexType: Wrong type name !");
        return F_MeshVertex_Pos3Color4Normal3Tex2;
    }

    
}; //LostPeterFoundation