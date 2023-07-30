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
    ////////////////////////////////// Line //////////////////////////////////
        "Line",                     //0:    Line
        "LineTriangle",             //1:    LineTriangle
        "LineQuad",                 //2:    LineQuad
        "LineGrid",                 //3:    LineGrid
        "LineCircle",               //4:    LineCircle
        "LineAABB",                 //5:    LineAABB
        "LineSphere",               //6:    LineSphere   
        "LineCylinder",             //7:    LineCylinder
        "LineCapsule",              //8:    LineCapsule
        "LineCone",                 //9:    LineCone
        "LineTorus",                //10:   LineTorus

    ////////////////////////////////// Flat //////////////////////////////////
        "FlatTriangle",             //11:   FlatTriangle
        "FlatQuad",                 //12:   FlatQuad
        "FlatCircle",               //13:   FlatCircle
        "FlatAABB",                 //14:   FlatAABB
        "FlatSphere",               //15:   FlatSphere
        "FlatCylinder",             //16:   FlatCylinder
        "FlatCapsule",              //17:   FlatCapsule
        "FlatCone",                 //18:   FlatCone
        "FlatTorus",                //19:   FlatTorus

    ////////////////////////////////// Entity ////////////////////////////////
        "EntityTriangle",           //20:   EntityTriangle
        "EntityQuad",               //21:   EntityQuad
        "EntityGrid",               //22:   EntityGrid
        "EntityCircle",             //23:   EntityCircle
        "EntityAABB",               //24:   EntityAABB
        "EntitySphere",             //25:   EntitySphere
        "EntityGeoSphere",          //26:   EntityGeoSphere
        "EntityCylinder",           //27:   EntityCylinder
        "EntityCapsule",            //28:   EntityCapsule
        "EntityCone",               //29:   EntityCone
        "EntityTorus",              //30:   EntityTorus
        "EntitySkyBox",             //31:   EntitySkyBox
        "EntitySkyDome",            //32:   EntitySkyDome
        "EntityTerrain",            //33:   EntityTerrain
    };
    static std::map<String, FMeshGeometryType> s_mapName2MeshGeometryType;
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
        if (s_mapName2MeshGeometryType.size() <= 0)
        {
            for (size_t i = 0; i < (int)F_MeshGeometry_Count; i++)
            {
                FMeshGeometryType type = (FMeshGeometryType)i;
                s_mapName2MeshGeometryType[s_nameMeshGeometryTypes[i]] = type;
            }
        }
        
        std::map<String, FMeshGeometryType>::iterator itFind = s_mapName2MeshGeometryType.find(strName);
        if (itFind != s_mapName2MeshGeometryType.end())
        {
            return itFind->second;
        }
        F_Assert(false && "F_ParseMeshGeometryType: Wrong type name !")
        return F_MeshGeometry_EntityTriangle;
    }
    bool F_IsMeshGeometryType_Line(FMeshGeometryType type)
    {
        if ((int)type < (int)F_MeshGeometry_FlatTriangle)
            return true;
        return false;
    }
    bool F_IsMeshGeometryType_Flat(FMeshGeometryType type)
    {
        if ((int)type >= (int)F_MeshGeometry_FlatTriangle && (int)type < (int)F_MeshGeometry_EntityTriangle)
            return true;
        return false;
    }
    bool F_IsMeshGeometryType_Entity(FMeshGeometryType type)
    {
        if ((int)type >= (int)F_MeshGeometry_EntityTriangle)
            return true;
        return false;
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