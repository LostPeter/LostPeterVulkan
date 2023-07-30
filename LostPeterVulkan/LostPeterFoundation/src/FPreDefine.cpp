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
        F_Assert(false && "F_ParseMeshType: Wrong type name !")
        return F_Mesh_File;
    }


    //FMeshGeometryType
    static const String s_nameMeshGeometryTypes[] = 
    {
        "line",                     //0:    Line
        "linequad",                 //1:    LineQuad
        "lineaabb",                 //2:    LineAABB
        "linesphere",               //3:    LineSphere   
        "triangle",                 //4:    Triangle
        "quad",                     //5:    Quad
        "grid",                     //6:    Grid
        "circle",                   //7:    Circle
        "aabb",                     //8:    AABB
        "sphere",                   //9:    Sphere
        "geosphere",                //10:   GeoSphere
        "cylinder",                 //11:   Cylinder
        "capsule",                  //12:   Capsule
        "cone",                     //13:   Cone
        "torus",                    //14:   Torus
        "skybox",                   //15:   SkyBox
        "skydome",                  //16:   SkyDome
        "terrain",                  //17:   Terrain
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
        F_Assert(false && "F_ParseMeshGeometryType: Wrong type name !")
        return F_MeshGeometry_Triangle;
    }
    

    //FMeshVertexType
    static const String s_nameMeshVertexTypes[] = 
    {
        "Pos2Color4",                               //0:    Pos2Color4
        "Pos3Color4",                               //1:    Pos3Color4
        "Pos3Normal3",                              //2:    Pos3Normal3
        "Pos3Normal3Tex2",                          //3:    Pos3Normal3Tex2
        "Pos2Color4Tex2",                           //4:    Pos2Color4Tex2
        "Pos3Color4Tex2",                           //5:    Pos3Color4Tex2
        "Pos3Color4Normal3Tex2",                    //6:    Pos3Color4Normal3Tex2
        "Pos3Color4Normal3Tex4",                    //7:    Pos3Color4Normal3Tex4
        "Pos3Color4Normal3Tangent3Tex2",            //8:    Pos3Color4Normal3Tangent3Tex2
        "Pos3Color4Normal3Tangent3Tex4",            //9:    Pos3Color4Normal3Tangent3Tex4
        "Pos3Normal3Tangent3BlendWI8Tex2",          //10:   Pos3Normal3Tangent3BlendWI8Tex2
        "Pos3Color4Normal3Tangent3BlendWI8Tex2",    //11:   Pos3Color4Normal3Tangent3BlendWI8Tex2
    };
    const String& F_GetMeshVertexTypeName(FMeshVertexType type)
    {
        return s_nameMeshVertexTypes[(int)type];
    }
    const String& F_GetMeshVertexTypeName(int type)
    {
        return s_nameMeshVertexTypes[(int)type];
    }
    FMeshVertexType F_ParseMeshVertexType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_MeshVertex_Count; i++)
        {
            if (s_nameMeshVertexTypes[i] == strName)
                return (FMeshVertexType)(i);
        }
        F_Assert(false && "F_ParseMeshVertexType: Wrong type name !")
        return F_MeshVertex_Pos3Color4Normal3Tex2;
    }

    
}; //LostPeterFoundation