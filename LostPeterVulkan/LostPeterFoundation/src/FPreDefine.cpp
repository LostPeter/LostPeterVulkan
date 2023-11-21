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
    //FPlatformType
    static const String s_namePlatformTypes[] = 
    {
        "Windows",                  //0:    Windows
        "MacOS",                    //1:    MacOS
        "Linux",                    //2:    Linux
        "Android",                  //3:    Android
        "iOS"                       //4:    iOS
    };
    const String& F_GetPlatformTypeName(FPlatformType type)
    {
        return s_namePlatformTypes[(int32)type];
    }
    const String& F_GetPlatformTypeName(int type)
    {
        return s_namePlatformTypes[type];
    }


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


    //FTexturePixelFormatType
    static const String s_nameTexturePixelFormatTypes[] = 
    {
        "R8_UNORM",                 //0: R8_UNORM
        "R16_UNORM",                //1: R16_UNORM
        "R8G8B8A8_SRGB",            //2: R8G8B8A8_SRGB
        "R8G8B8A8_UNORM",           //3: R8G8B8A8_UNORM
    };
    const String& F_GetTexturePixelFormatTypeName(FTexturePixelFormatType type)
    {
        return s_nameTexturePixelFormatTypes[(int)type];
    }
    const String& F_GetTexturePixelFormatTypeName(int type)
    {
        return s_nameTexturePixelFormatTypes[(int)type];
    }
    FTexturePixelFormatType F_ParseTexturePixelFormatType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_TextureFormat_Count; i++)
        {
            if (s_nameTexturePixelFormatTypes[i] == strName)
                return (FTexturePixelFormatType)(i);
        }
        F_Assert(false && "F_ParseTexturePixelFormatType: Wrong type name !")
        return F_TexturePixelFormat_R8G8B8A8_SRGB;
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
        F_Assert(false && "F_ParseTextureFilterType: Wrong type name !")
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
    static String s_nameMSAASampleCountTypes[] = 
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
        return s_nameMSAASampleCountTypes[(int)type];
    }
    const String& F_GetMSAASampleCountTypeName(int type)
    {
        return s_nameMSAASampleCountTypes[type];
    }
    FMSAASampleCountType F_ParseMSAASampleCountType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_MSAASampleCount_Count; i++)
        {
            if (s_nameMSAASampleCountTypes[i] == strName)
                return (FMSAASampleCountType)(i);
        }
        F_Assert(false && "F_ParseMSAASampleCountType: Wrong type name !")
        return F_MSAASampleCount_1_Bit;
    }


    //FShaderType
    static String s_nameShaderTypes[] = 
    {
        "vert",                 //0: Vertex
        "tesc",                 //1: TessellationControl
        "tese",                 //2: TessellationEvaluation
        "geom",                 //3: Geometry
        "frag",                 //4: Fragment
        "comp",                 //5: Compute
    };
    const String& F_GetShaderTypeName(FShaderType type)
    {
        return s_nameShaderTypes[(int)type];
    }
    const String& F_GetShaderTypeName(int type)
    {
        return s_nameShaderTypes[type];
    }
    FShaderType F_ParseShaderType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Shader_Count; i++)
        {
            if (s_nameShaderTypes[i] == strName)
                return (FShaderType)(i);
        }
        F_Assert(false && "F_ParseShaderType: Wrong type name !")
        return F_Shader_Vertex;
    }


    //FCameraType
    static const String s_nameCameraTypes[] = 
    {
        "Perspective",              //0: Perspective
        "Orthogonal",               //1: Orthogonal
    };
    const String& F_GetCameraTypeName(FCameraType type)
    {
        return s_nameCameraTypes[(int)type];
    }
    const String& F_GetCameraTypeName(int type)
    {
        return s_nameCameraTypes[(int)type];
    }
    FCameraType F_ParseCameraType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Camera_Count; i++)
        {
            if (s_nameCameraTypes[i] == strName)
                return (FCameraType)(i);
        }
        F_Assert(false && "F_ParseCameraType: Wrong type name !")
        return F_Camera_Perspective;
    }
    

    //FRenderPrimitiveType
    static const String s_nameRenderPrimitiveTypes[] = 
    {
        "PointList",                //0: PointList
        "LineList",                 //1: LineList
        "LineStrip",                //2: LineStrip
        "TriangleList",             //3: TriangleList
        "TriangleStrip",            //4: TriangleStrip
        "TriangleFan",              //5: TriangleFan
    };
    const String& F_GetRenderPrimitiveTypeName(FRenderPrimitiveType type)
    {
        return s_nameRenderPrimitiveTypes[(int)type];
    }
    const String& F_GetRenderPrimitiveTypeName(int type)
    {
        return s_nameRenderPrimitiveTypes[(int)type];
    }
    FRenderPrimitiveType F_ParseRenderPrimitiveType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_RenderPrimitive_Count; i++)
        {
            if (s_nameRenderPrimitiveTypes[i] == strName)
                return (FRenderPrimitiveType)(i);
        }
        F_Assert(false && "F_ParseRenderPrimitiveType: Wrong type name !")
        return F_RenderPrimitive_TriangleList;
    }


    //FCullingType
    static const String s_nameCullingTypes[] = 
    {
        "None",                     //0: None
        "ClockWise",                //1: ClockWise
        "CounterClockWise",         //2: CounterClockWise
    };
    const String& F_GetCullingTypeName(FCullingType type)
    {
        return s_nameCullingTypes[(int)type];
    }
    const String& F_GetCullingTypeName(int type)
    {
        return s_nameCullingTypes[(int)type];
    }
    FCullingType F_ParseCullingType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Culling_Count; i++)
        {
            if (s_nameCullingTypes[i] == strName)
                return (FCullingType)(i);
        }
        F_Assert(false && "F_ParseCullingType: Wrong type name !")
        return F_Culling_None;
    }


    //FPolygonType
    static const String s_namePolygonTypes[] = 
    {
        "Point",                     //0: Point
        "WireFrame",                 //1: WireFrame
        "Soild",                     //2: Soild
    };
    const String& F_GetPolygonTypeName(FPolygonType type)
    {
        return s_namePolygonTypes[(int)type];
    }
    const String& F_GetPolygonTypeName(int type)
    {
        return s_namePolygonTypes[(int)type];
    }
    FPolygonType F_ParsePolygonType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Polygon_Count; i++)
        {
            if (s_namePolygonTypes[i] == strName)
                return (FPolygonType)(i);
        }
        F_Assert(false && "F_ParsePolygonType: Wrong type name !")
        return F_Polygon_Solid;
    }


    //FStencilOPType
    static const String s_nameStencilOPTypes[] = 
    {
        "Keep",                      //0: Keep
        "Zero",                      //1: Zero
        "Replace",                   //2: Replace
        "Increment",                 //3: Increment
        "Decrement",                 //4: Decrement
        "IncrementWrap",             //5: IncrementWrap
        "DecrementWrap",             //6: DecrementWrap
        "Invert",                    //7: Invert
    };
    const String& F_GetStencilOPTypeName(FStencilOPType type)
    {
        return s_nameStencilOPTypes[(int)type];
    }
    const String& F_GetStencilOPTypeName(int type)
    {
        return s_nameStencilOPTypes[(int)type];
    }
    FStencilOPType F_ParseStencilOPType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_StencilOP_Count; i++)
        {
            if (s_nameStencilOPTypes[i] == strName)
                return (FStencilOPType)(i);
        }
        F_Assert(false && "F_ParseStencilOPType: Wrong type name !")
        return F_StencilOP_Keep;
    }


    //FCompareFuncType
    static const String s_nameCompareFuncTypes[] = 
    {
        "AlwaysPass",                //0: AlwaysPass
        "AlwaysFail",                //1: AlwaysFail
        "Less",                      //2: Less
        "LessEqual",                 //3: LessEqual
        "Equal",                     //4: Equal
        "NotEqual",                  //5: NotEqual
        "GreaterEqual",              //6: GreaterEqual
        "Greater",                   //7: Greater
    };
    const String& F_GetCompareFuncTypeName(FCompareFuncType type)
    {
        return s_nameCompareFuncTypes[(int)type];
    }
    const String& F_GetCompareFuncTypeName(int type)
    {
        return s_nameCompareFuncTypes[(int)type];
    }
    FCompareFuncType F_ParseCompareFuncType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_CompareFunc_Count; i++)
        {
            if (s_nameCompareFuncTypes[i] == strName)
                return (FCompareFuncType)(i);
        }
        F_Assert(false && "F_ParseCompareFuncType: Wrong type name !")
        return F_CompareFunc_LessEqual;
    }


    //FSceneBlendingType
    static const String s_nameSceneBlendingTypes[] = 
    {
        "Alpha",                     //0: Alpha
        "Color",                     //1: Color
        "Add",                       //2: Add
        "Modulate",                  //3: Modulate
        "Replace",                   //4: Replace
    };
    const String& F_GetSceneBlendingTypeName(FSceneBlendingType type)
    {
        return s_nameSceneBlendingTypes[(int)type];
    }
    const String& F_GetSceneBlendingTypeName(int type)
    {
        return s_nameSceneBlendingTypes[(int)type];
    }
    FSceneBlendingType F_ParseSceneBlendingType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_SceneBlending_Count; i++)
        {
            if (s_nameSceneBlendingTypes[i] == strName)
                return (FSceneBlendingType)(i);
        }
        F_Assert(false && "F_ParseSceneBlendingType: Wrong type name !")
        return F_SceneBlending_Color;
    }


    //FSceneBlendingOPType
    static const String s_nameSceneBlendingOPTypes[] = 
    {
        "Add",                       //0: Add
        "Subtract",                  //1: Subtract
        "SubtractReverse",           //2: SubtractReverse
        "Min",                       //3: Min
        "Max",                       //4: Max
    };
    const String& F_GetSceneBlendingOPTypeName(FSceneBlendingOPType type)
    {
        return s_nameSceneBlendingOPTypes[(int)type];
    }
    const String& F_GetSceneBlendingOPTypeName(int type)
    {
        return s_nameSceneBlendingOPTypes[(int)type];
    }
    FSceneBlendingOPType F_ParseSceneBlendingOPType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_SceneBlendingOP_Count; i++)
        {
            if (s_nameSceneBlendingOPTypes[i] == strName)
                return (FSceneBlendingOPType)(i);
        }
        F_Assert(false && "F_ParseSceneBlendingOPType: Wrong type name !")
        return F_SceneBlendingOP_Add;
    }


    //FSceneBlendingFactorType
    static const String s_nameSceneBlendingFactorTypes[] = 
    {
        "One",                       //0: One
        "Zero",                      //1: Zero
        "SourceColor",               //2: SourceColor
        "DestColor",                 //3: DestColor
        "OneMinusSourceColor",       //4: OneMinusSourceColor
        "OneMinusDestColor",         //5: OneMinusDestColor
        "SourceAlpha",               //6: SourceAlpha
        "DestAlpha",                 //7: DestAlpha
        "OneMinusSourceAlpha",       //8: OneMinusSourceAlpha
        "OneMinusDestAlpha",         //9: OneMinusDestAlpha
    };
    const String& F_GetSceneBlendingFactorTypeName(FSceneBlendingFactorType type)
    {
        return s_nameSceneBlendingFactorTypes[(int)type];
    }
    const String& F_GetSceneBlendingFactorTypeName(int type)
    {
        return s_nameSceneBlendingFactorTypes[(int)type];
    }
    FSceneBlendingFactorType F_ParseSceneBlendingFactorType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_SceneBlendingFactor_Count; i++)
        {
            if (s_nameSceneBlendingFactorTypes[i] == strName)
                return (FSceneBlendingFactorType)(i);
        }
        F_Assert(false && "F_ParseSceneBlendingFactorType: Wrong type name !")
        return F_SceneBlendingFactor_Zero;
    }


    //FLightingType
    static const String s_nameLightingTypes[] = 
    {
        "Flat",                      //0: Flat
        "Gouraud",                   //1: Gouraud
        "Phong",                     //2: Phong
        "Pbr",                       //3: Pbr
    };
    const String& F_GetLightingTypeName(FLightingType type)
    {
        return s_nameLightingTypes[(int)type];
    }
    const String& F_GetLightingTypeName(int type)
    {
        return s_nameLightingTypes[(int)type];
    }
    FLightingType F_ParseLightingType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_Lighting_Count; i++)
        {
            if (s_nameLightingTypes[i] == strName)
                return (FLightingType)(i);
        }
        F_Assert(false && "F_ParseLightingType: Wrong type name !")
        return F_Lighting_Gouraud;
    }


    //FRenderPipelineType
    static const String s_nameRenderPipelineTypes[] = 
    {
        "Forward",                   //0: Forward
        "Deferred",                  //1: Deferred
    };
    const String& F_GetRenderPipelineTypeName(FRenderPipelineType type)
    {
        return s_nameRenderPipelineTypes[(int)type];
    }
    const String& F_GetRenderPipelineTypeName(int type)
    {
        return s_nameRenderPipelineTypes[(int)type];
    }
    FRenderPipelineType F_ParseRenderPipelineType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_RenderPipeline_Count; i++)
        {
            if (s_nameRenderPipelineTypes[i] == strName)
                return (FRenderPipelineType)(i);
        }
        F_Assert(false && "F_ParseRenderPipelineType: Wrong type name !")
        return F_RenderPipeline_Forward;
    }


    //FRenderQueueType
    static const String s_nameRenderQueueTypes[] = 
    {
        "Background",               //0: Background:   0    - 1000
        "Opaque",                   //1: Opaque:       1000 - 2000
        "Transparent",              //2: Transparent:  2000 - 3000
        "Overlay",                  //3: Overlay:      3000 - 4000
    };
    const String& F_GetRenderQueueTypeName(FRenderQueueType type)
    {
        return s_nameRenderQueueTypes[(int)type];
    }
    const String& F_GetRenderQueueTypeName(int type)
    {
        return s_nameRenderQueueTypes[(int)type];
    }
    FRenderQueueType F_ParseRenderQueueType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_RenderQueue_Count; i++)
        {
            if (s_nameRenderQueueTypes[i] == strName)
                return (FRenderQueueType)(i);
        }
        F_Assert(false && "F_ParseRenderQueueType: Wrong type name !")
        return F_RenderQueue_Opaque;
    }
    
    static const int s_valueRenderQueueTypes[] = 
    {
        1000,
        2000,
        3000,
        4000,
    };
    int F_GetRenderQueueTypeValue(FRenderQueueType type)
    {
        return s_valueRenderQueueTypes[(int)type];
    }
    int F_GetRenderQueueTypeValue(int type)
    {
        return s_valueRenderQueueTypes[type];
    }
    FRenderQueueType F_ParseRenderQueueTypeByValue(int value)
    {
        if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Overlay])
            return F_RenderQueue_Overlay;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Transparent] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_Overlay])
            return F_RenderQueue_Transparent;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Opaque] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_Transparent])
            return F_RenderQueue_Opaque;
        
        return F_RenderQueue_Background;
    }


    //FRenderPassType
    static const String s_nameRenderPassTypes[] = 
    {
        "ForwardLit",               //0: ForwardLit
        "DeferredLit",              //1: DeferredLit
        "ShadowCaster",             //2: ShadowCaster
        "DepthOnly",                //3: DepthOnly
    };
    const String& F_GetRenderPassTypeName(FRenderPassType type)
    {
        return s_nameRenderPassTypes[(int)type];
    }
    const String& F_GetRenderPassTypeName(int type)
    {
        return s_nameRenderPassTypes[(int)type];
    }
    FRenderPassType F_ParseRenderPassType(const String& strName)
    {
        for (size_t i = 0; i < (int)F_RenderPass_Count; i++)
        {
            if (s_nameRenderPassTypes[i] == strName)
                return (FRenderPassType)(i);
        }
        F_Assert(false && "F_ParseRenderPassType: Wrong type name !")
        return F_RenderPass_ForwardLit;
    }


    //FPixelFormatComponentType
    static String s_namePixelFormatComponentTypes[] = 
    {
        "ByteU",
        "ByteS",
        "ShortU",
        "ShortS",
        "IntU",
        "IntS",
        "LongU",
        "LongS",
        "Float16",
        "Float32",
        "Double"
    };
    const String& F_GetPixelFormatComponentTypeName(FPixelFormatComponentType type)
    {   
        return s_namePixelFormatComponentTypes[(int)type];
    }
    const String& F_GetPixelFormatComponentTypeName(int type)
    {
        return s_namePixelFormatComponentTypes[type];
    }
    

}; //LostPeterFoundation