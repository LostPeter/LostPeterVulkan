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
    ////////////////////////////////// Line2D ////////////////////////////////
        "LineLine2D",               //0:    LineLine2D
        "LineTriangle2D",           //1:    LineTriangle2D
        "LineQuad2D",               //2:    LineQuad2D
        "LineGrid2D",               //3:    LineGrid2D
        "LineCircle2D",             //4:    LineCircle2D

    ////////////////////////////////// Flat2D ////////////////////////////////
        "FlatTriangle2D",           //5:    FlatTriangle2D
        "FlatQuad2D",               //6:    FlatQuad2D
        "FlatCircle2D",             //7:    FlatCircle2D

    ////////////////////////////////// Line3D ////////////////////////////////
        "LineLine3D",               //8:    LineLine3D
        "LineTriangle3D",           //9:    LineTriangle3D
        "LineQuad3D",               //10:   LineQuad3D
        "LineGrid3D",               //11:   LineGrid3D
        "LineCircle3D",             //12:   LineCircle3D
        "LineAABB3D",               //13:   LineAABB3D
        "LineSphere3D",             //14:   LineSphere3D
        "LineCylinder3D",           //15:   LineCylinder3D
        "LineCapsule3D",            //16:   LineCapsule3D
        "LineCone3D",               //17:   LineCone3D
        "LineTorus3D",              //18:   LineTorus3D

    ////////////////////////////////// Flat3D ////////////////////////////////
        "FlatTriangle3D",           //19:   FlatTriangle3D
        "FlatQuad3D",               //20:   FlatQuad3D
        "FlatCircle3D",             //21:   FlatCircle3D
        "FlatAABB3D",               //22:   FlatAABB3D
        "FlatSphere3D",             //23:   FlatSphere3D
        "FlatCylinder3D",           //24:   FlatCylinder3D
        "FlatCapsule3D",            //25:   FlatCapsule3D
        "FlatCone3D",               //26:   FlatCone3D
        "FlatTorus3D",              //27:   FlatTorus3D

    ////////////////////////////////// Entity ////////////////////////////////
        "EntityTriangle",           //28:   EntityTriangle
        "EntityQuad",               //29:   EntityQuad
        "EntityGrid",               //30:   EntityGrid
        "EntityCircle",             //31:   EntityCircle
        "EntityAABB",               //32:   EntityAABB
        "EntitySphere",             //33:   EntitySphere
        "EntityGeoSphere",          //34:   EntityGeoSphere
        "EntityCylinder",           //35:   EntityCylinder
        "EntityCapsule",            //36:   EntityCapsule
        "EntityCone",               //37:   EntityCone
        "EntityTorus",              //38:   EntityTorus
        "EntitySkyBox",             //39:   EntitySkyBox
        "EntitySkyDome",            //40:   EntitySkyDome
        "EntityTerrain",            //41:   EntityTerrain
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
    bool F_IsMeshGeometryType_Line2D(FMeshGeometryType type)
    {
        if ((int)type < (int)F_MeshGeometry_FlatTriangle2D)
            return true;
        return false;
    }
    bool F_IsMeshGeometryType_Flat2D(FMeshGeometryType type)
    {
        if ((int)type >= (int)F_MeshGeometry_FlatTriangle2D && (int)type < (int)F_MeshGeometry_LineLine3D)
            return true;
        return false;
    }
    bool F_IsMeshGeometryType_Line3D(FMeshGeometryType type)
    {
        if ((int)type >= (int)F_MeshGeometry_LineLine3D && (int)type < (int)F_MeshGeometry_FlatTriangle3D)
            return true;
        return false;
    }
    bool F_IsMeshGeometryType_Flat3D(FMeshGeometryType type)
    {
        if ((int)type >= (int)F_MeshGeometry_FlatTriangle3D && (int)type < (int)F_MeshGeometry_EntityTriangle)
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


    //FTextureType
    static const String s_nameTextureTypes[] = 
    {
        "1d",                   //0: 1D
        "2d",                   //1: 2D
        "2darray",              //2: 2DArray        
        "3d",                   //3: 3D
        "cubemap"               //4: CubeMap
    };
    const String& F_GetTextureTypeName(FTextureType type)
    {
        return s_nameTextureTypes[(int)type];
    }
    const String& F_GetTextureTypeName(int type)
    {
        return s_nameTextureTypes[(int)type];
    }
    FTextureType F_ParseTextureType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Texture_Count; i++)
        {
            if (s_nameTextureTypes[i] == strName)
                return (FTextureType)(i);
        }
        F_Assert(false && "F_ParseTextureType: Wrong type name !")
        return F_Texture_2D;
    }


    //FCameraType
    static const String s_nameCameraTypes[] = 
    {
        "Perspective",          //0: Perspective
        "Orthogonal",           //1: Orthogonal
    };
    const String& F_GetCameraTypeTypeName(FCameraType type)
    {
        return s_nameCameraTypes[(int)type];
    }
    const String& F_GetCameraTypeTypeName(int type)
    {
        return s_nameCameraTypes[(int)type];
    }
    FCameraType F_ParseCameraTypeType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Camera_Count; i++)
        {
            if (s_nameCameraTypes[i] == strName)
                return (FCameraType)(i);
        }
        F_Assert(false && "F_ParseCameraTypeType: Wrong type name !")
        return F_Camera_Perspective;
    }
    

}; //LostPeterFoundation