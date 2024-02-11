/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreDefine.h"
#include "../include/EngineDefine.h"

namespace LostPeterEngine
{   
////////////////////////////// Typedef /////////////////////////////
    


////////////////////////////// Enum ////////////////////////////////
    //EWindowType
    static String s_nameWindows[] = 
    {
        "Main",                     //0:    Main
        "Game",                     //1:    Game
        "Scene",                    //2:    Scene
    };
    const String& E_GetWindowTypeName(EWindowType type)
    {
        return s_nameWindows[(int)type];
    }
    const String& E_GetWindowTypeName(int type)
    {
        return s_nameWindows[(int)type];
    }
    EWindowType E_ParseWindowType(const String& strName)
    {
        for (int i = 0; i < (int)E_Window_Count; i++)
        {
            if (s_nameWindows[i] == strName)
                return (EWindowType)(i);
        }
        F_Assert(false && "E_ParseWindowType: Wrong type name !")
        return E_Window_Main;
    }


    //EObjectType
    static String s_nameObjectTypes[] = 
    {
        "Mesh",                     //0:    Mesh            
        "SkinMesh",                 //1:    SkinMesh
        "Camera",                   //2:    Camera
        "Light",                    //3:    Light
        "Terrain",                  //4:    Terrain
        "Water",                    //5:    Water
        "Sky",                      //6:    Sky
        "Cloud",                    //7:    Cloud
        "Particle",                 //8:    Particle
        "PostProcess",              //9:    PostProcess
        "UI",                       //10:   UI
        "Custom",                   //11:   Custom
    };
    const String& E_GetObjectTypeName(EObjectType type)
    {
        return s_nameObjectTypes[(int)type];
    }
    const String& E_GetObjectTypeName(int type)
    {
        return s_nameObjectTypes[type];
    }
    EObjectType E_ParseObjectType(const String& strName)
    {
        for (int i = 0; i < (int)E_Object_Count; i++)
        {
            if (s_nameObjectTypes[i] == strName)
                return (EObjectType)(i);
        }
        F_Assert(false && "E_ParseObjectType: Wrong type name !")
        return E_Object_Mesh;
    }


    //EResourceType
    static String s_nameResourceTypes[] = 
    {
        "Mesh",                     //0:    Mesh            
        "Texture",                  //1:    Texture
        "Shader",                   //2:    Shader
    };
    const String& E_GetResourceTypeName(EResourceType type)
    {
        return s_nameResourceTypes[(int)type];
    }
    const String& E_GetResourceTypeName(int type)
    {
        return s_nameResourceTypes[type];
    }
    EResourceType E_ParseResourceType(const String& strName)
    {
        for (int i = 0; i < (int)E_Resource_Count; i++)
        {
            if (s_nameResourceTypes[i] == strName)
                return (EResourceType)(i);
        }
        F_Assert(false && "E_ParseResourceType: Wrong type name !")
        return E_Resource_Mesh;
    }

    
    //EMeshParamType
    static String s_nameMeshParamTypes[] = 
    {
        "Usage",                        //0:    Usage
        "MeshType",                     //1:    MeshType
        "MeshVertexType",               //2:    MeshVertexType
        "MeshGeometryType",             //3:    MeshGeometryType
        "IsFlipY",                      //4:    IsFlipY
    };
    const String& E_GetMeshParamTypeName(EMeshParamType type)
    {
        return s_nameMeshParamTypes[(int)type];
    }
    const String& E_GetMeshParamTypeName(int type)
    {
        return s_nameMeshParamTypes[type];
    }
    EMeshParamType E_ParseMeshParamType(const String& strName)
    {
        for (int i = 0; i < (int)E_MeshParam_Count; i++)
        {
            if (s_nameMeshParamTypes[i] == strName)
                return (EMeshParamType)(i);
        }
        F_Assert(false && "E_ParseMeshParamType: Wrong type name !")
        return E_MeshParam_Usage;
    }


    //ETextureParamType
    static String s_nameTextureParamTypes[] = 
    {
        "Usage",                        //0:    Usage    
        "TextureType",                  //1:    TextureType    
        "TextureFilterType",            //2:    TextureFilterType    
        "TextureAddressingType",        //3:    TextureAddressingType    
        "TextureBorderColorType",       //4:    TextureBorderColorType    
        "MSAASampleCountType",          //5:    MSAASampleCountType
        "PixelFormatType",              //6:    PixelFormatType
        "Width",                        //7:    Width            
        "Height",                       //8:    Height
        "Depth",                        //9:    Depth
        "BitDepthInteger",              //10:    BitDepthInteger
        "BitDepthFloat",                //11:   BitDepthFloat
        "IsTreatLuminanceAsAlpha",      //12:   IsTreatLuminanceAsAlpha
        "MipMapsCount",                 //13:   MipMapsCount
        "IsMipMapsHardwareGenerated",   //14:   IsMipMapsHardwareGenerated
        "Gamma",                        //15:   Gamma
        "IsGammaHardware",              //16:   IsGammaHardware
    };
    const String& E_GetTextureParamTypeName(ETextureParamType type)
    {
        return s_nameTextureParamTypes[(int)type];
    }
    const String& E_GetTextureParamTypeName(int type)
    {
        return s_nameTextureParamTypes[type];
    }
    ETextureParamType E_ParseTextureParamType(const String& strName)
    {
        for (int i = 0; i < (int)E_TextureParam_Count; i++)
        {
            if (s_nameTextureParamTypes[i] == strName)
                return (ETextureParamType)(i);
        }
        F_Assert(false && "E_ParseTextureParamType: Wrong type name !")
        return E_TextureParam_Usage;
    }


    //EShaderParamType
    static String s_nameShaderParamTypes[] = 
    {
        "Type",                     //0:    Type    

    };
    const String& E_GetShaderParamTypeName(EShaderParamType type)
    {
        return s_nameShaderParamTypes[(int)type];
    }
    const String& E_GetShaderParamTypeName(int type)
    {
        return s_nameShaderParamTypes[type];
    }
    EShaderParamType E_ParseShaderParamType(const String& strName)
    {
        for (int i = 0; i < (int)E_ShaderParam_Count; i++)
        {
            if (s_nameShaderParamTypes[i] == strName)
                return (EShaderParamType)(i);
        }
        F_Assert(false && "E_ParseShaderParamType: Wrong type name !")
        return E_ShaderParam_Type;
    }


    //ECpuInfoType
    const String s_nameCPUInfoTypes[] = 
	{
		"CPU_Vender",					//0:  Vender
		"CPU_Brand",					//1:  Brand
		"CPU_Code",						//2:  Code
		"CPU_VenderType",				//3:  VenderType
		"CPU_NumCore",					//4:  NumCore
		"CPU_LogicalCpus",				//5:  LogicalCPUs
		"CPU_TotalLogicalCpus",			//6:  TotalLogicalCPUs
		"CPU_L1DataCache",				//7:  L1DataCache
		"CPU_L1InstCache",				//8:  L1InstCache
		"CPU_L2Cache",					//9:  L2Cache
		"CPU_L3Cache",					//10: L3Cache
		"CPU_L1Cache_DataAsso",			//11: L1Cache_DataAsso
		"CPU_L2Cache_Asso",				//12: L2Cache_Asso
		"CPU_L3Cache_Asso",				//13: L3Cache_Asso
		"CPU_L1Cache_DataLine",			//14: L1Cache_DataLine
		"CPU_L2Cache_Line",				//15: L2Cache_Line
		"CPU_L3Cache_Line",				//16: L3Cache_Line
		"CPU_SSESize",					//17: SSESize
		"CPU_SSE",						//18: SSE
		"CPU_SSE2",						//19: SSE2
		"CPU_SSE3",						//20: SSE3
        "CPU_SSE41",					//21: SSE41
        "CPU_SSE42",					//22: SSE42
		"CPU_MMX",						//23: MMX		
		"CPU_MMXEXT",					//24: MMXEXT
		"CPU_3DNOW",					//25: 3DNOW
		"CPU_3DNOWEXT",					//26: 3DNOWEXT
		"CPU_CMOV",						//27: CMOV
		"CPU_TSC",						//28: TSC
        "CPU_INVARIANT_TSC",            //29: INVARIANT_TSC
		"CPU_FPU",						//30: FPU
		"CPU_PRO",						//31: PRO	
        "CPU_HTT",						//32: HTT
        "CPU_VFP",						//33: VFP
        "CPU_NEON",						//34: NEON
        "CPU_MSA",						//35: MSA
	};
    const String& E_GetCpuInfoTypeName(ECpuInfoType type)
    {
        return s_nameCPUInfoTypes[(int)type];
    }
    const String& E_GetCpuInfoTypeName(int type)
    {
        return s_nameCPUInfoTypes[type];
    }


    //EGpuVendorType
    const String s_nameGPUVendorTypes[] = 
	{
		"Unknown",
		"Nvidia",
		"AMD",
		"Intel",
		"Imagination",
		"Apple",
		"Nokia",
		"MS Software",
        "MS Warp",
        "Arm Mali",
        "Qualcomm",
        "Mozilla",
        "Webkit",
	};
    const String& E_GetGpuVendorTypeName(EGpuVendorType type)
    {
        return s_nameGPUVendorTypes[(int)type];
    }
    const String& E_GetGpuVendorTypeName(int type)
    {
        return s_nameGPUVendorTypes[type];
    }


    //EEngineConfigType
    static const String s_nameEngineConfigTypes[] = 
	{
    ////Common
		"MultiThread",					    //0:    MultiThread

    ////Render
        "RendererName",                     //1:    RendererName  
        "PipelineName",                     //2:    PipelineName

        "ShaderGroup",                      //3:    ShaderGroup
        "ShaderConfigName",                 //4:    ShaderConfigName
        "MeshGroup",                        //5:    MeshGroup
        "MeshConfigName",                   //6:    MeshConfigName
        "TextureGroup",                     //7:    TextureGroup
        "TextureConfigName",                //8:    TextureConfigName
        "MaterialGroup",                    //9:    MaterialGroup
        "MaterialConfigName",               //10:   MaterialConfigName

    ////Audio


    ////Physics
        

	};
    const String& E_GetEngineConfigTypeName(EEngineConfigType type)
    {
        return s_nameEngineConfigTypes[(int)type];
    }
    const String& E_GetEngineConfigTypeName(int type)
    {
        return s_nameEngineConfigTypes[type];
    }


    //EFrameProfilerType
    static const String s_nameFrameProfilerTypes[] = 
    { 
        "Audio", 
        "Memory", 
        "Physics",
        "Render",

    };
    const String& E_GetFrameProfilerTypeName(EFrameProfilerType type)
    {
        return s_nameFrameProfilerTypes[(int)type];
    }
    const String& E_GetFrameProfilerTypeName(int type)
    {
        return s_nameFrameProfilerTypes[type];
    }

    
    ////////////////////////////// Class ///////////////////////////////
    MeshInfo::MeshInfo()
        : group(0)
        , nameMesh("")
        , pathMesh("")
        , eMesh(F_Mesh_File)
        , eMeshVertex(F_MeshVertex_Pos3Color4Tex2)
        , eMeshGeometry(F_MeshGeometry_EntityGrid)
        , pMeshCreateParam(nullptr)
        , isFlipY(false)
        , isTransformLocal(false)
        , matTransformLocal(FMath::ms_mat4Unit)
    {

    }
    MeshInfo::MeshInfo(const String& _nameMesh,
                       const String& _pathMesh,
                       FMeshType _eMesh,
                       FMeshVertexType _eMeshVertex,
                       FMeshGeometryType _eMeshGeometry,
                       FMeshCreateParam* _pMeshCreateParam,
                       bool _isFlipY,
                       bool _isTransformLocal,
                       const FMatrix4& _matTransformLocal)
        : group(0)
        , nameMesh(_nameMesh)
        , pathMesh(_pathMesh)
        , eMesh(_eMesh)
        , eMeshVertex(_eMeshVertex)
        , eMeshGeometry(_eMeshGeometry)
        , pMeshCreateParam(_pMeshCreateParam)
        , isFlipY(_isFlipY)
        , isTransformLocal(_isTransformLocal)
        , matTransformLocal(_matTransformLocal)
    {

    }
    MeshInfo::~MeshInfo()
    {
        F_DELETE(this->pMeshCreateParam)
    }


    TextureInfo::TextureInfo()
        : group(0)
        , nameTexture("")
        , pathTexture("")
        , typeTexture(F_Texture_2D)
        , typePixelFormat(F_PixelFormat_BYTE_A8R8G8B8_SRGB)
        , typeTextureFilter(F_TextureFilter_Bilinear)
        , typeTextureAddressing(F_TextureAddressing_Clamp)
        , typeTextureBorderColor(F_TextureBorderColor_OpaqueBlack)
        , typeMSAASampleCount(F_MSAASampleCount_1_Bit)
        , width(512)
        , height(512)
        , depth(1)
        , animChunkX(0)
        , animChunkY(0)
        , isAlpha(false)
        , isRT(false)
        , isGCS(false)
    {

    }
    TextureInfo::TextureInfo(const String& _nameTexture,
                             const String& _pathTexture,
                             FTextureType _typeTexture,
                             FPixelFormatType _typePixelFormat,
                             FTextureFilterType _typeTextureFilter,
                             FTextureAddressingType _typeTextureAddressing,
                             FTextureBorderColorType _typeTextureBorderColor,
                             FMSAASampleCountType _typeMSAASampleCount,
                             int _width,
                             int _height,
                             int _depth,
                             int _animChunkX,
                             int _animChunkY,
                             bool _isAlpha,
                             bool _isRT,
                             bool _isGCS)
        : group(0)
        , nameTexture(_nameTexture)
        , pathTexture(_pathTexture)
        , typeTexture(_typeTexture)
        , typePixelFormat(_typePixelFormat)
        , typeTextureFilter(_typeTextureFilter)
        , typeTextureAddressing(_typeTextureAddressing)
        , typeTextureBorderColor(_typeTextureBorderColor)
        , typeMSAASampleCount(_typeMSAASampleCount)
        , width(_width)
        , height(_height)
        , depth(_depth)
        , animChunkX(_animChunkX)
        , animChunkY(_animChunkY)
        , isAlpha(_isAlpha)
        , isRT(_isRT)
        , isGCS(_isGCS)
    {

    }
    TextureInfo::~TextureInfo()
    {

    }


    ShaderProgramInfo::ShaderProgramInfo()
        : group(0)
        , nameShaderProgram("")
        , pathShaderProgram("")
        , eShader(F_Shader_Vertex)
    {

    }
    ShaderProgramInfo::ShaderProgramInfo(const String& _nameShaderProgram,
                                         const String& _pathShaderProgram,
                                         FShaderType _eShader)
        : group(0)
        , nameShaderProgram(_nameShaderProgram)
        , pathShaderProgram(_pathShaderProgram)
        , eShader(_eShader)
    {

    }
    ShaderProgramInfo::~ShaderProgramInfo()
    {

    }


    MaterialInfo::MaterialInfo()
        : group(0)
        , nameMaterial("")
        , pathMaterial("")
    {

    }
    MaterialInfo::MaterialInfo(const String& _nameMaterial,
                               const String& _pathMaterial)
        : group(0)
        , nameMaterial(_nameMaterial)
        , pathMaterial(_pathMaterial)
    {

    }
    MaterialInfo::~MaterialInfo()
    {

    }


    SceneInfo::SceneInfo()
        : group(0)
        , nameScene("")
        , pathScene("")
    {

    }
    SceneInfo::SceneInfo(const String& _nameScene,
                         const String& _pathScene)
        : group(0)
        , nameScene(_nameScene)
        , pathScene(_pathScene)
    {

    }
    SceneInfo::~SceneInfo()
    {

    }


}; //LostPeterEngine