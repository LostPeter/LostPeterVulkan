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
        "1D",                   //0: 1D
        "2D",                   //1: 2D
        "2DArray",              //2: 2DArray        
        "3D",                   //3: 3D
        "CubeMap"               //4: CubeMap
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


    //FTextureFormatType
    static const String s_nameTextureFormatTypes[] = 
    {
        "R8G8B8A8_SRGB",            //0: R8G8B8A8_SRGB
        "R8G8B8A8_UNORM",           //1: R8G8B8A8_UNORM
    };
    const String& F_GetTextureFormatTypeName(FTextureFormatType type)
    {
        return s_nameTextureFormatTypes[(int)type];
    }
    const String& F_GetTextureFormatTypeName(int type)
    {
        return s_nameTextureFormatTypes[(int)type];
    }
    FTextureFormatType F_ParseTextureFormatType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureFormat_Count; i++)
        {
            if (s_nameTextureFormatTypes[i] == strName)
                return (FTextureFormatType)(i);
        }
        F_Assert(false && "F_ParseTextureFormatType: Wrong type name !")
        return F_TextureFormat_R8G8B8A8_SRGB;
    }

    
    //FTextureFilterSizeType
    static const String s_nameTextureFilterSizeTypes[] = 
    {
        "Min",          //0: Min	
        "Mag",          //1: Mag
        "Mip",          //2: Mip
    };
    const String& F_GetTextureFilterSizeTypeName(FTextureFilterSizeType type)
    {
        return s_nameTextureFilterSizeTypes[(int)type];
    }
    const String& F_GetTextureFilterSizeTypeName(int type)
    {
        return s_nameTextureFilterSizeTypes[(int)type];
    }
    FTextureFilterSizeType F_ParseTextureFilterSizeType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureFilterSize_Count; i++)
        {
            if (s_nameTextureFilterSizeTypes[i] == strName)
                return (FTextureFilterSizeType)(i);
        }
        F_Assert(false && "F_ParseTextureFilterSizeType: Wrong type name !")
        return F_TextureFilterSize_Min;
    }


    //FTextureFilterPixelType
    static const String s_nameTextureFilterPixelTypes[] = 
    {
        "None",                 //0: None
        "Point",                //1: Point
        "Linear",               //2: Linear
        "Anisotropic",          //3: Anisotropic
    };
    const String& F_GetTextureFilterPixelTypeName(FTextureFilterPixelType type)
    {
        return s_nameTextureFilterPixelTypes[(int)type];
    }
    const String& F_GetTextureFilterPixelTypeName(int type)
    {
        return s_nameTextureFilterPixelTypes[(int)type];
    }
    FTextureFilterPixelType F_ParseTextureFilterPixelType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureFilterPixel_Count; i++)
        {
            if (s_nameTextureFilterPixelTypes[i] == strName)
                return (FTextureFilterPixelType)(i);
        }
        F_Assert(false && "F_ParseTextureFilterPixelType: Wrong type name !")
        return F_TextureFilterPixel_None;
    }


    //FTextureFilterType
    static const String s_nameTextureFilterTypes[] = 
    {
        "None",                 //0: Min=F_TextureFilterPixel_Point,        Mag=F_TextureFilterPixel_Point,  	   Mip=F_TextureFilterPixel_None
        "Bilinear",             //1: Min=F_TextureFilterPixel_Linear,       Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Point
        "Trilinear",            //2: Min=F_TextureFilterPixel_Linear,	    Mag=F_TextureFilterPixel_Linear, 	   Mip=F_TextureFilterPixel_Linear
        "Anisotropic",          //3: Min=F_TextureFilterPixel_Anisotropic,  Mag=F_TextureFilterPixel_Anisotropic,  Mip=F_TextureFilterPixel_Linear
    };
    const String& F_GetTextureFilterTypeName(FTextureFilterType type)
    {
        return s_nameTextureFilterTypes[(int)type];
    }
    const String& F_GetTextureFilterTypeName(int type)
    {
        return s_nameTextureFilterTypes[(int)type];
    }
    FTextureFilterType F_ParseTextureFilterType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureFilter_Count; i++)
        {
            if (s_nameTextureFilterTypes[i] == strName)
                return (FTextureFilterType)(i);
        }
        F_Assert(false && "F_ParseTextureFormatType: Wrong type name !")
        return F_TextureFilter_None;
    }


    //FTextureAddressingType
    static const String s_nameTextureAddressingTypes[] = 
    {
        "Wrap",                 //0: Wrap
        "Mirror",               //1: Mirror
        "Clamp",                //2: Clamp
        "Border",               //3: Border
    };
    const String& F_GetTextureAddressingTypeName(FTextureAddressingType type)
    {
        return s_nameTextureAddressingTypes[(int)type];
    }
    const String& F_GetTextureAddressingTypeName(int type)
    {
        return s_nameTextureAddressingTypes[(int)type];
    }
    FTextureAddressingType F_ParseTextureAddressingType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureAddressing_Count; i++)
        {
            if (s_nameTextureAddressingTypes[i] == strName)
                return (FTextureAddressingType)(i);
        }
        F_Assert(false && "F_ParseTextureAddressingType: Wrong type name !")
        return F_TextureAddressing_Wrap;
    }


    //FTextureBorderColorType
    static const String s_nameTextureBorderColorTypes[] = 
    {
        "OpaqueBlack",          //0: OpaqueBlack
        "OpaqueWhite",          //1: OpaqueWhite
        "TransparentBlack",     //2: TransparentBlack
    };
    const String& F_GetTextureBorderColorTypeName(FTextureBorderColorType type)
    {
        return s_nameTextureBorderColorTypes[(int)type];
    }
    const String& F_GetTextureBorderColorTypeName(int type)
    {
        return s_nameTextureBorderColorTypes[(int)type];
    }
    FTextureBorderColorType F_ParseTextureBorderColorType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureBorderColor_Count; i++)
        {
            if (s_nameTextureBorderColorTypes[i] == strName)
                return (FTextureBorderColorType)(i);
        }
        F_Assert(false && "F_ParseTextureBorderColorType: Wrong type name !")
        return F_TextureBorderColor_OpaqueBlack;
    }


    //FMSAASampleCountType
    static String s_nameMSAASampleCounts[] = 
    {
        "1-Bit",                //0: 1_Bit
        "2-Bit",                //1: 2_Bit
        "4-Bit",                //2: 4_Bit
        "8-Bit",                //3: 8_Bit
        "16-Bit",               //4: 16_Bit
        "32-Bit",               //5: 32_Bit
        "64-Bit",               //6: 64_Bit
    };
    const String& F_GetMSAASampleCountTypeName(FMSAASampleCountType type)
    {
        return s_nameMSAASampleCounts[(int)type];
    }
    const String& F_GetMSAASampleCountTypeName(int type)
    {
        return s_nameMSAASampleCounts[type];
    }
    FMSAASampleCountType F_ParseMSAASampleCountType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_MSAASampleCount_Count; i++)
        {
            if (s_nameMSAASampleCounts[i] == strName)
                return (FMSAASampleCountType)(i);
        }
        F_Assert(false && "F_ParseMSAASampleCountType: Wrong type name !")
        return F_MSAASampleCount_1_Bit;
    }


    //FCameraType
    static const String s_nameCameraTypes[] = 
    {
        "Perspective",              //0: Perspective
        "Orthogonal",               //1: Orthogonal
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