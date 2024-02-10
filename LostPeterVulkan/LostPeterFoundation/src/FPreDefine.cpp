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
        for (int i = 0; i < (int)F_Mesh_Count; i++)
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
            for (int i = 0; i < (int)F_MeshGeometry_Count; i++)
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
        for (int i = 0; i < (int)F_MeshVertex_Count; i++)
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
        for (int i = 0; i < (int)F_Texture_Count; i++)
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
        for (int i = 0; i < (int)F_TextureFormat_Count; i++)
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
        for (int i = 0; i < (int)F_TextureFilterSize_Count; i++)
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
        for (int i = 0; i < (int)F_TextureFilterPixel_Count; i++)
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
        for (int i = 0; i < (int)F_TextureFilter_Count; i++)
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
        for (int i = 0; i < (int)F_TextureAddressing_Count; i++)
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
        for (int i = 0; i < (int)F_TextureBorderColor_Count; i++)
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
        "1_Bit",                //0: 1_Bit
        "2_Bit",                //1: 2_Bit
        "4_Bit",                //2: 4_Bit
        "8_Bit",                //3: 8_Bit
        "16_Bit",               //4: 16_Bit
        "32_Bit",               //5: 32_Bit
        "64_Bit",               //6: 64_Bit
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
        for (int i = 0; i < (int)F_MSAASampleCount_Count; i++)
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
        for (int i = 0; i < (int)F_Shader_Count; i++)
        {
            if (s_nameShaderTypes[i] == strName)
                return (FShaderType)(i);
        }
        F_Assert(false && "F_ParseShaderType: Wrong type name !")
        return F_Shader_Vertex;
    }


    //FShaderPassType
    static String s_nameShaderPassTypes[] = 
    {
        "Main",                 //0: Main
        "Depth",                //1: Depth
        "Custom",               //2: Custom
    };
    const String& F_GetShaderPassTypeName(FShaderPassType type)
    {
        return s_nameShaderPassTypes[(int)type];
    }
    const String& F_GetShaderPassTypeName(int type)
    {
        return s_nameShaderPassTypes[type];
    }
    FShaderPassType F_ParseShaderPassType(const String& strName)
    {
        for (int i = 0; i < (int)F_ShaderPass_Count; i++)
        {
            if (s_nameShaderPassTypes[i] == strName)
                return (FShaderPassType)(i);
        }
        F_Assert(false && "F_ParseShaderPassType: Wrong type name !")
        return F_ShaderPass_Main;
    }


    //FShaderParamConstantType
    static String s_nameShaderParamConstantTypes[] = 
    {
    ////Float
        "Float1",                     //0:  Float1
		"Float2",                     //1:  Float2
		"Float3",                     //2:  Float3
		"Float4",                     //3:  Float4
		"Matrix_2X2",                 //4:  Matrix_2X2
		"Matrix_2X3",                 //5:  Matrix_2X3
		"Matrix_2X4",                 //6:  Matrix_2X4
		"Matrix_3X2",                 //7:  Matrix_3X2
		"Matrix_3X3",                 //8:  Matrix_3X3
		"Matrix_3X4",                 //9:  Matrix_3X4
		"Matrix_4X2",                 //10: Matrix_4X2
		"Matrix_4X3",                 //11: Matrix_4X3
		"Matrix_4X4",                 //12: Matrix_4X4
    ////Int
        "Int1",                       //13: Int1
		"Int2",                       //14: Int2
		"Int3",                       //15: Int3
		"Int4",                       //16: Int4
    ////Double
        "Double1",                    //17: Double1
        "Double2",                    //18: Double2
        "Double3",                    //19: Double3
        "Double4",                    //20: Double4
        "Matrix_Double_2X2",          //21: Matrix_Double_2X2
        "Matrix_Double_2X3",          //22: Matrix_Double_2X3
        "Matrix_Double_2X4",          //23: Matrix_Double_2X4
        "Matrix_Double_3X2",          //24: Matrix_Double_3X2
        "Matrix_Double_3X3",          //25: Matrix_Double_3X3
        "Matrix_Double_3X4",          //26: Matrix_Double_3X4
        "Matrix_Double_4X2",          //27: Matrix_Double_4X2
        "Matrix_Double_4X3",          //28: Matrix_Double_4X3
        "Matrix_Double_4X4",          //29: Matrix_Double_4X4
    ////Uint
        "Uint1",                      //30: Uint1
        "Uint2",                      //31: Uint2
        "Uint3",                      //32: Uint3
        "Uint4",                      //33: Uint4
    ////Bool
        "Bool1",                      //34: Bool1
        "Bool2",                      //35: Bool2
        "Bool3",                      //36: Bool3
        "Bool4",                      //37: Bool4
    ////Sampler
        "Sampler1D",                  //38: Sampler1D
		"Sampler1D",                  //39: Sampler2D
		"Sampler1D",                  //40: Sampler3D
		"Sampler1D",                  //41: SamplerCube
		"Sampler1D",                  //42: Sampler1DShadow
		"Sampler1D",                  //43: Sampler2DShadow
        "Sampler1D",                  //44: Sampler2DArray
        "Sampler1D",                  //45: SamplerExternalOES
	////Specialization
        "Specialization",             //46: Specialization
    ////Unknown
        "Unknown",                    //47: Unknown
    };
    static std::map<FShaderParamConstantType, int> s_mapShaderParamConstantType2Index
    {
    ////Float
        { F_ShaderParamConstant_Float1,                 0  },                 //0:  Float1
		{ F_ShaderParamConstant_Float2,                 1  },                 //1:  Float2
		{ F_ShaderParamConstant_Float3,                 2  },                 //2:  Float3
		{ F_ShaderParamConstant_Float4,                 3  },                 //3:  Float4
		{ F_ShaderParamConstant_Matrix_2X2,             4  },                 //4:  Matrix_2X2
		{ F_ShaderParamConstant_Matrix_2X3,             5  },                 //5:  Matrix_2X3
		{ F_ShaderParamConstant_Matrix_2X4 ,            6  },                 //6:  Matrix_2X4
		{ F_ShaderParamConstant_Matrix_3X2,             7  },                 //7:  Matrix_3X2
		{ F_ShaderParamConstant_Matrix_3X3,             8  },                 //8:  Matrix_3X3
		{ F_ShaderParamConstant_Matrix_3X4,             9  },                 //9:  Matrix_3X4
		{ F_ShaderParamConstant_Matrix_4X2,             10 },                 //10: Matrix_4X2
		{ F_ShaderParamConstant_Matrix_4X3,             11 },                 //11: Matrix_4X3
		{ F_ShaderParamConstant_Matrix_4X4 ,            12 },                 //12: Matrix_4X4
    ////Int
        { F_ShaderParamConstant_Int1,                   13 },                 //13: Int1
		{ F_ShaderParamConstant_Int2,                   14 },                 //14: Int2
		{ F_ShaderParamConstant_Int3,                   15 },                 //15: Int3
		{ F_ShaderParamConstant_Int4,                   16 },                 //16: Int4
    ////Double
        { F_ShaderParamConstant_Double1,                17 },                 //17: Double1
        { F_ShaderParamConstant_Double2,                18 },                 //18: Double2
        { F_ShaderParamConstant_Double3,                19 },                 //19: Double3
        { F_ShaderParamConstant_Double4,                20 },                 //20: Double4
        { F_ShaderParamConstant_Matrix_Double_2X2,      21 },                 //21: Matrix_Double_2X2
        { F_ShaderParamConstant_Matrix_Double_2X3,      22 },                 //22: Matrix_Double_2X3
        { F_ShaderParamConstant_Matrix_Double_2X4,      23 },                 //23: Matrix_Double_2X4
        { F_ShaderParamConstant_Matrix_Double_3X2,      24 },                 //24: Matrix_Double_3X2
        { F_ShaderParamConstant_Matrix_Double_3X3,      25 },                 //25: Matrix_Double_3X3
        { F_ShaderParamConstant_Matrix_Double_3X4,      26 },                 //26: Matrix_Double_3X4
        { F_ShaderParamConstant_Matrix_Double_4X2,      27 },                 //27: Matrix_Double_4X2
        { F_ShaderParamConstant_Matrix_Double_4X3,      28 },                 //28: Matrix_Double_4X3
        { F_ShaderParamConstant_Matrix_Double_4X4,      29 },                 //29: Matrix_Double_4X4
    ////Uint
        { F_ShaderParamConstant_Uint1,                  30 },                 //30: Uint1
        { F_ShaderParamConstant_Uint2,                  31 },                 //31: Uint2
        { F_ShaderParamConstant_Uint3,                  32 },                 //32: Uint3
        { F_ShaderParamConstant_Uint4,                  33 },                 //33: Uint4
    ////Bool
        { F_ShaderParamConstant_Bool1,                  34 },                 //34: Bool1
        { F_ShaderParamConstant_Bool2,                  35 },                 //35: Bool2
        { F_ShaderParamConstant_Bool3,                  36 },                 //36: Bool3
        { F_ShaderParamConstant_Bool4,                  37 },                 //37: Bool4
    ////Sampler
        { F_ShaderParamConstant_Sampler1D,              38 },                 //38: Sampler1D
		{ F_ShaderParamConstant_Sampler2D,              39 },                 //39: Sampler2D
		{ F_ShaderParamConstant_Sampler3D,              40 },                 //40: Sampler3D
		{ F_ShaderParamConstant_SamplerCube,            41 },                 //41: SamplerCube
		{ F_ShaderParamConstant_Sampler1DShadow,        42 },                 //42: Sampler1DShadow
		{ F_ShaderParamConstant_Sampler2DShadow,        43 },                 //43: Sampler2DShadow
        { F_ShaderParamConstant_Sampler2DArray,         44 },                 //44: Sampler2DArray
        { F_ShaderParamConstant_SamplerExternalOES,     45 },                 //45: SamplerExternalOES
	////Specialization
        { F_ShaderParamConstant_Specialization,         46 },                 //46: Specialization
    ////Unknown
        { F_ShaderParamConstant_Unknown,                47 },                 //47: Unknown
    };
    static std::map<int, FShaderParamConstantType> s_mapIndex2ShaderParamConstantType
    {
    ////Float
        { 0,                F_ShaderParamConstant_Float1  },                  //0:  Float1
		{ 1,                F_ShaderParamConstant_Float2  },                  //1:  Float2
		{ 2,                F_ShaderParamConstant_Float3  },                  //2:  Float3
		{ 3,                F_ShaderParamConstant_Float4  },                  //3:  Float4
		{ 4,                F_ShaderParamConstant_Matrix_2X2  },              //4:  Matrix_2X2
		{ 5,                F_ShaderParamConstant_Matrix_2X3  },              //5:  Matrix_2X3
		{ 6,                F_ShaderParamConstant_Matrix_2X4  },              //6:  Matrix_2X4
		{ 7,                F_ShaderParamConstant_Matrix_3X2  },              //7:  Matrix_3X2
		{ 8,                F_ShaderParamConstant_Matrix_3X3  },              //8:  Matrix_3X3
		{ 9,                F_ShaderParamConstant_Matrix_3X4  },              //9:  Matrix_3X4
		{ 10,               F_ShaderParamConstant_Matrix_4X2  },              //10: Matrix_4X2
		{ 11,               F_ShaderParamConstant_Matrix_4X3  },              //11: Matrix_4X3
		{ 12,               F_ShaderParamConstant_Matrix_4X4  },              //12: Matrix_4X4
    ////Int
        { 13,               F_ShaderParamConstant_Int1  },                    //13: Int1
		{ 14,               F_ShaderParamConstant_Int2  },                    //14: Int2
		{ 15,               F_ShaderParamConstant_Int3  },                    //15: Int3
		{ 16,               F_ShaderParamConstant_Int4  },                    //16: Int4
    ////Double
        { 17,               F_ShaderParamConstant_Double1  },                 //17: Double1
        { 18,               F_ShaderParamConstant_Double2  },                 //18: Double2
        { 19,               F_ShaderParamConstant_Double3  },                 //19: Double3
        { 20,               F_ShaderParamConstant_Double4  },                 //20: Double4
        { 21,               F_ShaderParamConstant_Matrix_Double_2X2  },       //21: Matrix_Double_2X2
        { 22,               F_ShaderParamConstant_Matrix_Double_2X3  },       //22: Matrix_Double_2X3
        { 23,               F_ShaderParamConstant_Matrix_Double_2X4  },       //23: Matrix_Double_2X4
        { 24,               F_ShaderParamConstant_Matrix_Double_3X2  },       //24: Matrix_Double_3X2
        { 25,               F_ShaderParamConstant_Matrix_Double_3X3  },       //25: Matrix_Double_3X3
        { 26,               F_ShaderParamConstant_Matrix_Double_3X4  },       //26: Matrix_Double_3X4
        { 27,               F_ShaderParamConstant_Matrix_Double_4X2  },       //27: Matrix_Double_4X2
        { 28,               F_ShaderParamConstant_Matrix_Double_4X3  },       //28: Matrix_Double_4X3
        { 29,               F_ShaderParamConstant_Matrix_Double_4X4  },       //29: Matrix_Double_4X4
    ////Uint
        { 30,               F_ShaderParamConstant_Uint1  },                   //30: Uint1
        { 31,               F_ShaderParamConstant_Uint2  },                   //31: Uint2
        { 32,               F_ShaderParamConstant_Uint3  },                   //32: Uint3
        { 33,               F_ShaderParamConstant_Uint4  },                   //33: Uint4
    ////Bool
        { 34,               F_ShaderParamConstant_Bool1  },                   //34: Bool1
        { 35,               F_ShaderParamConstant_Bool2  },                   //35: Bool2
        { 36,               F_ShaderParamConstant_Bool3  },                   //36: Bool3
        { 37,               F_ShaderParamConstant_Bool4  },                   //37: Bool4
    ////Sampler
        { 38,               F_ShaderParamConstant_Sampler1D  },               //38: Sampler1D
		{ 39,               F_ShaderParamConstant_Sampler2D  },               //39: Sampler2D
		{ 40,               F_ShaderParamConstant_Sampler3D  },               //40: Sampler3D
		{ 41,               F_ShaderParamConstant_SamplerCube  },             //41: SamplerCube
		{ 42,               F_ShaderParamConstant_Sampler1DShadow  },         //42: Sampler1DShadow
		{ 43,               F_ShaderParamConstant_Sampler2DShadow  },         //43: Sampler2DShadow
        { 44,               F_ShaderParamConstant_Sampler2DArray  },          //44: Sampler2DArray
        { 45,               F_ShaderParamConstant_SamplerExternalOES  },      //45: SamplerExternalOES
	////Specialization
        { 46,               F_ShaderParamConstant_Specialization  },          //46: Specialization
    ////Unknown
        { 47,               F_ShaderParamConstant_Unknown  },                 //47: Unknown
    };
    const String& F_GetShaderParamConstantTypeName(FShaderParamConstantType type)
    {
        return s_nameShaderParamConstantTypes[s_mapShaderParamConstantType2Index[type]];
    }
    FShaderParamConstantType F_ParseShaderParamConstantType(const String& strName)
    {
        int count = (int)s_mapIndex2ShaderParamConstantType.size();
        for (int i = 0; i < (int)count; i++)
        {
            if (s_nameShaderParamConstantTypes[i] == strName)
                return s_mapIndex2ShaderParamConstantType[i];
        }
        F_Assert(false && "F_ParseShaderParamConstantType: Wrong type name !")
        return F_ShaderParamConstant_Float1;
    }
    FShaderParamConstantBaseType F_ParseShaderParamConstantBaseType(FShaderParamConstantType type)
    {
        return FShaderParamConstantBaseType(type / 0x10 * 0x10);
    }
    bool F_ShaderParamConstant_IsFloat(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Float;
    }
    bool F_ShaderParamConstant_IsInt(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Int;
    }
    bool F_ShaderParamConstant_IsDouble(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Double;
    }
    bool F_ShaderParamConstant_IsUint(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Uint;
    }
    bool F_ShaderParamConstant_IsBool(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Bool;
    }
    bool F_ShaderParamConstant_IsSampler(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Sampler;
    }
    bool F_ShaderParamConstant_IsSpecialization(FShaderParamConstantType type)
    {
        return F_ParseShaderParamConstantBaseType(type) == F_ShaderParamConstantBase_Specialization;
    }
    uint32 F_ShaderParamConstant_GetElementSize(FShaderParamConstantType type, bool bIsPadToMultiplesOf4)
    {
        if (bIsPadToMultiplesOf4)
        {
            switch (type)
            {
            case F_ShaderParamConstant_Float1:
            case F_ShaderParamConstant_Float2:
            case F_ShaderParamConstant_Float3:
            case F_ShaderParamConstant_Float4:
            case F_ShaderParamConstant_Int1:
            case F_ShaderParamConstant_Int2:
            case F_ShaderParamConstant_Int3:
            case F_ShaderParamConstant_Int4:
            case F_ShaderParamConstant_Uint1:
            case F_ShaderParamConstant_Uint2:
            case F_ShaderParamConstant_Uint3:
            case F_ShaderParamConstant_Uint4:
            case F_ShaderParamConstant_Bool1:
            case F_ShaderParamConstant_Bool2:
            case F_ShaderParamConstant_Bool3:
            case F_ShaderParamConstant_Bool4:
            case F_ShaderParamConstant_Sampler1D:
            case F_ShaderParamConstant_Sampler2D:
            case F_ShaderParamConstant_Sampler3D:
            case F_ShaderParamConstant_SamplerCube:
            case F_ShaderParamConstant_Sampler1DShadow:
            case F_ShaderParamConstant_Sampler2DShadow:
            case F_ShaderParamConstant_Sampler2DArray:
            case F_ShaderParamConstant_SamplerExternalOES:
                return 4;   //1 float4
            case F_ShaderParamConstant_Matrix_2X2:
            case F_ShaderParamConstant_Matrix_2X3:
            case F_ShaderParamConstant_Matrix_2X4:
            case F_ShaderParamConstant_Double1:
            case F_ShaderParamConstant_Double2:
            case F_ShaderParamConstant_Double3:
            case F_ShaderParamConstant_Double4:
                return 8;   //2 float4
            case F_ShaderParamConstant_Matrix_3X2:
            case F_ShaderParamConstant_Matrix_3X3:
            case F_ShaderParamConstant_Matrix_3X4:
                return 12;  //3 float4
            case F_ShaderParamConstant_Matrix_4X2:
            case F_ShaderParamConstant_Matrix_4X3:
            case F_ShaderParamConstant_Matrix_4X4:
            case F_ShaderParamConstant_Matrix_Double_2X2:
            case F_ShaderParamConstant_Matrix_Double_2X3:
            case F_ShaderParamConstant_Matrix_Double_2X4:
                return 16;  //4 float4
            case F_ShaderParamConstant_Matrix_Double_3X2:
            case F_ShaderParamConstant_Matrix_Double_3X3:
            case F_ShaderParamConstant_Matrix_Double_3X4:
                return 24;  //6 float4
            case F_ShaderParamConstant_Matrix_Double_4X2:
            case F_ShaderParamConstant_Matrix_Double_4X3:
            case F_ShaderParamConstant_Matrix_Double_4X4:
                return 32;  //8 float4
            default:
                return 4;   //1 float4
            }
        }
        else
        {
            switch (type)
            {
            case F_ShaderParamConstant_Sampler1D:
            case F_ShaderParamConstant_Sampler2D:
            case F_ShaderParamConstant_Sampler3D:
            case F_ShaderParamConstant_SamplerCube:
            case F_ShaderParamConstant_Sampler1DShadow:
            case F_ShaderParamConstant_Sampler2DShadow:
            case F_ShaderParamConstant_Sampler2DArray:
            case F_ShaderParamConstant_SamplerExternalOES:
                return 1;   //1 float
            case F_ShaderParamConstant_Matrix_2X2:
            case F_ShaderParamConstant_Matrix_Double_2X2:
                return 4;
            case F_ShaderParamConstant_Matrix_2X3:
            case F_ShaderParamConstant_Matrix_3X2:
            case F_ShaderParamConstant_Matrix_Double_2X3:
            case F_ShaderParamConstant_Matrix_Double_3X2:
                return 6;
            case F_ShaderParamConstant_Matrix_2X4:
            case F_ShaderParamConstant_Matrix_4X2:
            case F_ShaderParamConstant_Matrix_Double_2X4:
            case F_ShaderParamConstant_Matrix_Double_4X2:
                return 8;
            case F_ShaderParamConstant_Matrix_3X3:
            case F_ShaderParamConstant_Matrix_Double_3X3:
                return 9;
            case F_ShaderParamConstant_Matrix_3X4:
            case F_ShaderParamConstant_Matrix_4X3:
            case F_ShaderParamConstant_Matrix_Double_3X4:
            case F_ShaderParamConstant_Matrix_Double_4X3:
                return 12;
            case F_ShaderParamConstant_Matrix_4X4:
            case F_ShaderParamConstant_Matrix_Double_4X4:
                return 16;
            default:
                return type % 0x10;
            }
        }
    }


    //FShaderParamConstantDataType
    static String s_nameShaderParamConstantDataTypes[] = 
    {
        "Float",                //0: Float
        "Bool",                 //1: Bool
        "Int",                  //2: Int
    };
    const String& F_GetShaderParamConstantDataTypeName(FShaderParamConstantDataType type)
    {
        return s_nameShaderParamConstantDataTypes[(int)type];
    }
    const String& F_GetShaderParamConstantDataTypeName(int type)
    {
        return s_nameShaderParamConstantDataTypes[type];
    }
    FShaderParamConstantDataType F_ParseShaderParamConstantDataType(const String& strName)
    {
        for (int i = 0; i < (int)F_ShaderParamConstantData_Count; i++)
        {
            if (s_nameShaderParamConstantDataTypes[i] == strName)
                return (FShaderParamConstantDataType)(i);
        }
        F_Assert(false && "F_ParseShaderParamConstantDataType: Wrong type name !")
        return F_ShaderParamConstantData_Float;
    }


    //FShaderParamConstantAutoType
    static String s_nameShaderParamConstantAutoTypes[] = 
    {
    ////World
		"WorldMatrix",			                        //0:   WorldMatrix
		"WorldMatrixTranspose",						    //1:   WorldMatrixTranspose
		"WorldMatrixInverse",						    //2:   WorldMatrixInverse
		"WorldMatrixInverseTranspose",				    //3:   WorldMatrixInverseTranspose
		"WorldMatrixArray4x4",						    //4:   WorldMatrixArray4x4
		"WorldMatrixArray3x4",						    //5:   WorldMatrixArray3x4
	////View	
		"ViewMatrix",								    //6:   ViewMatrix
		"ViewMatrixTranspose",						    //7:   ViewMatrixTranspose
		"ViewMatrixInverse",							//8:   ViewMatrixInverse
		"ViewMatrixInverseTranspose",				    //9:   ViewMatrixInverseTranspose
	////Projection										
		"ProjectionMatrix",							    //10:  ProjectionMatrix
		"ProjectionMatrixTranspose",					//11:  ProjectionMatrixTranspose
		"ProjectionMatrixInverse",					    //12:  ProjectionMatrixInverse
		"ProjectionMatrixInverseTranspose",			    //13:  ProjectionMatrixInverseTranspose
	////World * View									
		"WorldViewMatrix",							    //14:  WorldViewMatrix
		"WorldViewMatrixTranspose",					    //15:  WorldViewMatrixTranspose
		"WorldViewMatrixInverse",					    //16:  WorldViewMatrixInverse
		"WorldViewMatrixInverseTranspose",			    //17:  WorldViewMatrixInverseTranspose
	////View * Projection								
		"ViewProjectionMatrix",						    //18:  ViewProjectionMatrix
		"ViewProjectionMatrixTranspose",				//19:  ViewProjectionMatrixTranspose
		"ViewProjectionMatrixInverse",				    //20:  ViewProjectionMatrixInverse
		"ViewProjectionMatrixInverseTranspose",		    //21:  ViewProjectionMatrixInverseTranspose
	////World * View * Projection						
		"WorldViewProjectionMatrix",					//22:  WorldViewProjectionMatrix
		"WorldViewProjectionMatrixTranspose",		    //23:  WorldViewProjectionMatrixTranspose
		"WorldViewProjectionMatrixInverse",			    //24:  WorldViewProjectionMatrixInverse
		"WorldViewProjectionMatrixInverseTranspose",	//25:  WorldViewProjectionMatrixInverseTranspose
	////Viewport	
		"ViewportSize",								    //26:  ViewportSize
		"ViewportWidth",								//27:  ViewportWidth
		"ViewportWidthInverse",						    //28:  ViewportWidthInverse
		"ViewportHeight",							    //29:  ViewportHeight
		"ViewportHeightInverse",						//30:  ViewportHeightInverse
	////Camera											
		"CameraPosInObjectSpace",					    //31:  CameraPosInObjectSpace
		"CameraRightInObjectSpace",					    //32:  CameraRightInObjectSpace
		"CameraUpInObjectSpace",						//33:  CameraUpInObjectSpace
		"CameraDirInObjectSpace",					    //34:  CameraDirInObjectSpace
		"CameraPosInWorldSpace",						//35:  CameraPosInWorldSpace
		"CameraRightInWorldSpace",					    //36:  CameraRightInWorldSpace
		"CameraUpInWorldSpace",						    //37:  CameraUpInWorldSpace
		"CameraDirInWorldSpace",						//38:  CameraDirInWorldSpace
		"CameraFov",									//39:  CameraFov
		"CameraNearClipDistance",					    //40:  CameraNearClipDistance
		"CameraFarClipDistance",						//41:  CameraFarClipDistance
		"CameraParam",								    //42:  CameraParam
	////Light
		"LightPosInModelSpace",						    //43:  LightPosInModelSpace
		"LightPosInWorldSpace",						    //44:  LightPosInWorldSpace
		"LightPosInViewSpace",						    //45:  LightPosInViewSpace
		"LightDirInModelSpace",						    //46:  LightDirInModelSpace
		"LightDirInWorldSpace",						    //47:  LightDirInWorldSpace
		"LightDirInViewSpace",						    //48:  LightDirInViewSpace
		"LightDistanceModelSpace",					    //49:  LightDistanceModelSpace
		"LightPowerScale",							    //50:  LightPowerScale
		"LightDiffuseColor",							//51:  LightDiffuseColor
		"LightSpecularColor",						    //52:  LightSpecularColor			 									
		"LightAttenuation",							    //53:  LightAttenuation
		"LightSpotParam",							    //54:  LightSpotParam
		
		"LightPosInModelSpaceArray",					//55:  LightPosInModelSpaceArray
		"LightPosInWorldSpaceArray",					//56:  LightPosInWorldSpaceArray
		"LightPosInViewSpaceArray",					    //57:  LightPosInViewSpaceArray
		"LightDirInModelSpaceArray",					//58:  LightDirInModelSpaceArray
		"LightDirInWorldSpaceArray",					//59:  LightDirInWorldSpaceArray
		"LightDirInViewSpaceArray",					    //60:  LightDirInViewSpaceArray
		"LightDistanceModelSpaceArray",				    //61:  LightDistanceModelSpaceArray
		"LightPowerScaleArray",						    //62:  LightPowerScaleArray
		"LightDiffuseColorArray",					    //63:  LightDiffuseColorArray
		"LightSpecularColorArray",					    //64:  LightSpecularColorArray			 									
		"LightAttenuationArray",						//65:  LightAttenuationArray
		"LightSpotParamArray",						    //66:  LightSpotParamArray
	////Material										
		"GlobalAmbient",								//67:  GlobalAmbient
		"MaterialAmbientColor",						    //68:  MaterialAmbientColor
		"MaterialDiffuseColor",						    //69:  MaterialDiffuseColor
		"MaterialSpecularColor",						//70:  MaterialSpecularColor
		"MaterialEmissiveColor",						//71:  MaterialEmissiveColor
		"MaterialShininess",							//72:  MaterialShininess

		"MaterialLightAmbientColor",					//73:  MaterialLightAmbientColor
		"MaterialLightDiffuseColor",					//74:  MaterialLightDiffuseColor
		"MaterialLightSpecularColor",				    //75:  MaterialLightSpecularColor
		"MaterialLightSceneColor",					    //76:  MaterialLightSceneColor
		"MaterialLightDiffuseColorArray",			    //77:  MaterialLightDiffuseColorArray
		"MaterialLightSpecularColorArray",			    //78:  MaterialLightSpecularColorArray
	////Fog												  
		"FogColor",									    //79:  FogColor
		"FogParam",									    //80:  FogParam
	////Texture											  
		"TextureSizeWHD",							    //81:  TextureSizeWHD
		"TextureSizeWHDInverse",						//82:  TextureSizeWHDInverse
		"TextureSizeWH",								//83:  TextureSizeWH
		"TextureUVTranslation",						    //84:  TextureUVTranslation
		"TextureUVRotation",							//85:  TextureUVRotation
		"TextureUVScale",							    //86:  TextureUVScale
		"TextureUVTransformMatrix",					    //87:  TextureUVTransformMatrix
	////Time											  
		"TimeElapsed",								    //88:  TimeElapsed
		"TimeElapsed_0_X",							    //89:  TimeElapsed_0_X							  
		"TimeElapsed_Sin_0_X",						    //90:  TimeElapsed_Sin_0_X
		"TimeElapsed_Cos_0_X",						    //91:  TimeElapsed_Cos_0_X
		"TimeElapsed_Tan_0_X",						    //92:  TimeElapsed_Tan_0_X
		"TimeElapsed_Packed_0_X",					    //93:  TimeElapsed_Packed_0_X
		"TimeElapsed_0_1",							    //94:  TimeElapsed_0_1							  
		"TimeElapsed_Sin_0_1",						    //95:  TimeElapsed_Sin_0_1
		"TimeElapsed_Cos_0_1",						    //96:  TimeElapsed_Cos_0_1	
		"TimeElapsed_Tan_0_1",						    //97:  TimeElapsed_Tan_0_1
		"TimeElapsed_Packed_0_1",					    //98:  TimeElapsed_Packed_0_1
		"TimeElapsed_0_2PI",							//99:  TimeElapsed_0_2PI			  
		"TimeElapsed_Sin_0_2PI",						//100: TimeElapsed_Sin_0_2PI
		"TimeElapsed_Cos_0_2PI",						//101: TimeElapsed_Cos_0_2PI
		"TimeElapsed_Tan_0_2PI",						//102: TimeElapsed_Tan_0_2PI
		"TimeElapsed_Packed_0_2PI",					    //103: TimeElapsed_Packed_0_2PI
		"FrameTime",									//104: FrameTime
		"FPS",										    //105: FPS
    };
    const String& F_GetShaderParamConstantAutoTypeName(FShaderParamConstantAutoType type)
    {
        return s_nameShaderParamConstantAutoTypes[(int)type];
    }
    const String& F_GetShaderParamConstantAutoTypeName(int type)
    {
        return s_nameShaderParamConstantAutoTypes[type];
    }
    FShaderParamConstantAutoType F_ParseShaderParamConstantAutoType(const String& strName)
    {
        for (int i = 0; i < (int)F_ShaderParamConstantAuto_Count; i++)
        {
            if (s_nameShaderParamConstantAutoTypes[i] == strName)
                return (FShaderParamConstantAutoType)(i);
        }
        F_Assert(false && "F_ParseShaderParamConstantAutoType: Wrong type name !")
        return F_ShaderParamConstantAuto_WorldMatrix;
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
        for (int i = 0; i < (int)F_Camera_Count; i++)
        {
            if (s_nameCameraTypes[i] == strName)
                return (FCameraType)(i);
        }
        F_Assert(false && "F_ParseCameraType: Wrong type name !")
        return F_Camera_Perspective;
    }
    

    //FLightType
    static const String s_nameLightTypes[] = 
    {
        "Directional",              //0: Directional
        "Point",                    //1: Point
        "Spot",                     //2: Spot
    };
    const String& F_GetLightTypeName(FLightType type)
    {
        return s_nameLightTypes[(int)type];
    }
    const String& F_GetLightTypeName(int type)
    {
        return s_nameLightTypes[(int)type];
    }
    FLightType F_ParseLightType(const String& strName)
    {
        for (int i = 0; i < (int)F_Camera_Count; i++)
        {
            if (s_nameLightTypes[i] == strName)
                return (FLightType)(i);
        }
        F_Assert(false && "F_ParseLightType: Wrong type name !")
        return F_Light_Directional;
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
        for (int i = 0; i < (int)F_RenderPrimitive_Count; i++)
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
        for (int i = 0; i < (int)F_Culling_Count; i++)
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
        for (int i = 0; i < (int)F_Polygon_Count; i++)
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
        for (int i = 0; i < (int)F_StencilOP_Count; i++)
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
        for (int i = 0; i < (int)F_CompareFunc_Count; i++)
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
        for (int i = 0; i < (int)F_SceneBlending_Count; i++)
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
        for (int i = 0; i < (int)F_SceneBlendingOP_Count; i++)
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
        for (int i = 0; i < (int)F_SceneBlendingFactor_Count; i++)
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
        for (int i = 0; i < (int)F_Lighting_Count; i++)
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
        for (int i = 0; i < (int)F_RenderPipeline_Count; i++)
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
        "BackGround",               //0:    BackGround:     0    - 1000            
        "Opaque",                   //1:    Opaque:         1000 - 2600
        "Terrain",                  //2:    Terrain:        2600 - 2800
        "Sky",                      //3:    Sky:            2800 - 3000
        "Transparent",              //4:    Transparent:    3000 - 4000
        "UI",                       //5:    UI:             4000 - 5000
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
        for (int i = 0; i < (int)F_RenderQueue_Count; i++)
        {
            if (s_nameRenderQueueTypes[i] == strName)
                return (FRenderQueueType)(i);
        }
        F_Assert(false && "F_ParseRenderQueueType: Wrong type name !")
        return F_RenderQueue_Opaque;
    }
    
    static const int s_valueRenderQueueTypes[] = 
    {
        0,          //0:    BackGround
        1000,       //1:    Opaque
        2600,       //2:    Terrain
        2800,       //3:    Sky
        3000,       //4:    Transparent
        4000,       //5:    UI
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
        if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_UI])
            return F_RenderQueue_UI;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Transparent] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_UI])
            return F_RenderQueue_Transparent;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Sky] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_Transparent])
            return F_RenderQueue_Sky;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Terrain] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_Sky])
            return F_RenderQueue_Terrain;
        else if (value >= s_valueRenderQueueTypes[(int)F_RenderQueue_Opaque] && value < s_valueRenderQueueTypes[(int)F_RenderQueue_Terrain])
            return F_RenderQueue_Opaque;
        
        return F_RenderQueue_BackGround;
    }


    //FRenderPassType
    static const String s_nameRenderPassTypes[] = 
    {
        "BackGround",               //0:  BackGround
        "ForwardLit",               //1:  ForwardLit
        "DeferredLit",              //2:  DeferredLit
        "ShadowCaster",             //3:  ShadowCaster
        "DepthOnly",                //4:  DepthOnly

        "Custom1",                  //5:  Custom1
        "Custom2",                  //6:  Custom2
        "Custom3",                  //7:  Custom3
        "Custom4",                  //8:  Custom4
        "Custom5",                  //9:  Custom5
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
        for (int i = 0; i < (int)F_RenderPass_Count; i++)
        {
            if (s_nameRenderPassTypes[i] == strName)
                return (FRenderPassType)(i);
        }
        F_Assert(false && "F_ParseRenderPassType: Wrong type name !")
        return F_RenderPass_BackGround;
    }


    //FFrameBufferType
    static const String s_nameFrameBufferTypes[] = 
    {
        "Color",                    //0: Color
        "Depth",                    //1: Depth
        "DepthStencil",             //2: DepthStencil
    };
    const String& F_GetFrameBufferTypeName(FFrameBufferType type)
    {
        return s_nameFrameBufferTypes[(int)type];
    }
    const String& F_GetFrameBufferTypeName(int type)
    {
        return s_nameFrameBufferTypes[(int)type];
    }
    FFrameBufferType F_ParseFrameBufferType(const String& strName)
    {
        for (int i = 0; i < (int)F_FrameBuffer_Count; i++)
        {
            if (s_nameFrameBufferTypes[i] == strName)
                return (FFrameBufferType)(i);
        }
        F_Assert(false && "F_ParseFrameBufferType: Wrong type name !")
        return F_FrameBuffer_Color;
    }


    //FPixelFormatComponentType
    static String s_namePixelFormatComponentTypes[] = 
    {
        "ByteU",                    //0:    Byte unsigned
        "ByteS",                    //1:    Byte signed
        "ShortU",                   //2:    Short unsigned
        "ShortS",                   //3:    Short signed
        "IntU",                     //4:    Int unsigned
        "IntS",                     //5:    Int signed
        "LongU",                    //6:    Long unsigned
        "LongS",                    //7:    Long signed
        "Float16",                  //8:    Float 16
        "Float32",                  //9:    Float 32
        "Double"                    //10:   Double
    };
    const String& F_GetPixelFormatComponentTypeName(FPixelFormatComponentType type)
    {   
        return s_namePixelFormatComponentTypes[(int)type];
    }
    const String& F_GetPixelFormatComponentTypeName(int type)
    {
        return s_namePixelFormatComponentTypes[type];
    }
    

    //FParameterType
    static String s_nameParameterTypes[] = 
    {
        "Void",                     //0:    Void    
        "Bool",                     //1:    Bool
        "Char",                     //2:    Char
        "UChar",                    //3:    UChar
        "Short",                    //4:    Short
        "UShort",                   //5:    UShort
        "Int",                      //6:    Int
        "UInt",                     //7:    UInt
        "Long",                     //8:    Long
        "ULong",                    //9:    ULong
        "Int64"                     //10:   Int64
        "UInt64"                    //11:   UInt64
        "Float"                     //12:   Float
        "Double"                    //13:   Double
        "Real"                      //14:   Real
        "Vector2"                   //15:   Vector2
        "Vector3"                   //16:   Vector3
        "Vector4"                   //17:   Vector4
        "Matrix3"                   //18:   Matrix3
        "Matrix4"                   //19:   Matrix4
        "Quaternion"                //20:   Quaternion
        "Color"                     //21:   Color
        "String"                    //22:   String
    };
    const String& F_GetParameterTypeName(FParameterType type)
    {
        return s_nameParameterTypes[(int)type];
    }
    const String& F_GetParameterTypeName(int type)
    {
        return s_nameParameterTypes[(int)type];
    }
    FParameterType F_ParseFParameterType(const String& strName)
    {
        for (int i = 0; i < (int)F_Parameter_Count; i++)
        {
            if (s_nameParameterTypes[i] == strName)
                return (FParameterType)(i);
        }
        F_Assert(false && "F_ParseFParameterType: Wrong type name !")
        return F_Parameter_Void;
    }


}; //LostPeterFoundation