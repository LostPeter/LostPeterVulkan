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

#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#include "PreIncludeStd.h"
#include "PreIncludeThird.h"

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;

namespace LostPeterEngine
{
////////////////////////////// Define //////////////////////////////
    #define MAX_LIGHT_COUNT 16
    #define MAX_TEXTURE_COUNT 16
    #define MAX_OBJECT_COUNT 1024
    #define MAX_OBJECT_LINEFLAT_2D_COUNT 2048
    #define MAX_OBJECT_LINEFLAT_3D_COUNT 512
    #define MAX_MATERIAL_COUNT 64
    #define MAX_INSTANCE_COUNT 1024

    #define SHADER_NAME_Vertex                      "vert"
    #define SHADER_NAME_TessellationControl         "tesc"
    #define SHADER_NAME_TessellationEvaluation      "tese"
    #define SHADER_NAME_Geometry                    "geom"
    #define SHADER_NAME_Fragment                    "frag"
    #define SHADER_NAME_Compute                     "comp"


    #define C_THRESH_POINT_ON_PLANE			0.10f
    #define C_THRESH_POINT_ON_SIDE			0.20f
    #define C_THRESH_POINTS_ARE_SAME		0.00002f	
    #define C_THRESH_POINTS_ARE_NEAR		0.015f
    #define C_THRESH_NORMALS_ARE_SAME		0.00002f
    #define C_THRESH_UVS_ARE_SAME			0.0009765625f
    #define C_THRESH_VECTORS_ARE_NEAR		0.0004f
    #define C_THRESH_SPLIT_POLY_WITH_PLANE	0.25f	
    #define C_THRESH_SPLIT_POLY_PRECISELY	0.01f
    #define C_THRESH_ZERO_NORM_SQUARED		0.0001f	
    #define C_THRESH_NORMALS_ARE_PARALLEL	0.999845f	
    #define C_THRESH_NORMALS_ARE_ORTHOGONAL	0.017455f
    #define C_THRESH_VECTOR_NORMALIZED		0.01f
    #define C_THRESH_QUAT_NORMALIZED		0.01f

    #define C_CONFIG_MAX_RENDER_TARGET_MULTIPLE_COUNT       8
    #define C_CONFIG_MAX_TEXTURE_UNIT_COUNT				    16
    #define C_CONFIG_MAX_TEXTURE_COORD_SETS_COUNT		    8
    #define	C_CONFIG_MAX_TEXTURE_UV_SCROLL_ANIM_COUNT	    4
    #define C_CONFIG_MAX_BONE_BLEND_WEIGHTS_COUNT	        4
    #define C_CONFIG_MAX_BONE_COUNT					        256
    #define C_CONFIG_MAX_STREAM_SOURCE_COUNT				16
    #define C_CONFIG_MAX_LIGHT_COUNT						8
    #define C_CONFIG_MAX_SHADER_PARAM_COUNT				    32
    #define C_CONFIG_MAX_GLYPHS_COUNT				        (9030 - 32)
    #define C_CONFIG_GLYPH_INDEX(c)					        c - 33

////////////////////////////// Typedef /////////////////////////////
    


////////////////////////////// Enum ////////////////////////////////
    enum EWindowType
    {
        E_Window_Main = 0,                                  //0:    Main
        E_Window_Game,                                      //1:    Game
        E_Window_Scene,                                     //2:    Scene

        E_Window_Count,
    };
    engineExport const String& E_GetWindowTypeName(EWindowType type);
    engineExport const String& E_GetWindowTypeName(int type);
    engineExport EWindowType E_ParseWindowType(const String& strName);
    

    enum EStreamReleaseType
	{
		E_StreamRelease_Manual = 0,                         //0: Manual
		E_StreamRelease_Auto                                //1: Auto
	};


	enum EStreamUsageType
	{
		E_StreamUsage_Static = 0x00000001,                  //1: Static, GPU to CPU
		E_StreamUsage_Dynamic = 0x00000002,                 //2: Dynamic, CPU only
		E_StreamUsage_WriteOnly	= 0x00000004,               //4: WriteOnly
		E_StreamUsage_Discardable = 0x00000008,             //8: Discardable
		E_StreamUsage_StaticWriteOnly = E_StreamUsage_Static | E_StreamUsage_WriteOnly, //GPU only
		E_StreamUsage_DynamicWriteOnly = E_StreamUsage_Dynamic | E_StreamUsage_WriteOnly, //CPU to GPU
		E_StreamUsage_DynamicWriteOnlyDiscardable = E_StreamUsage_Dynamic | E_StreamUsage_WriteOnly | E_StreamUsage_Discardable, //CPU to GPU
	};


	enum EStreamLockType
	{
		E_StreamLock_Normal = 0,                            //0: Normal
		E_StreamLock_Discard,                               //1: Discard
		E_StreamLock_ReadOnly,                              //2: ReadOnly
		E_StreamLock_NoOverWrite,                           //3: NoOverWrite
	};


	enum EStreamIndexType 
	{
		E_StreamIndex_16Bit = 0,                            //0: 16Bit
		E_StreamIndex_32Bit                                 //1: 32Bit 
	};


    enum ETextureMipMapType
	{
        E_TextureMipMap_Default	= -1,			            //-1, use DfTextureManager default
		E_TextureMipMap_UnLimited = 0x7FFFFFFF,	            //0x7FFFFFFF, generate mipmaps up to 1x1
		E_TextureMipMap_FromFile  = 0x8FFFFFFF,	            //0x8FFFFFFF, generate mipmaps up to 1x1
	};


	enum ETextureUsageType
	{
		E_TextureUsage_Static						= E_StreamUsage_Static,
		E_TextureUsage_Dynamic					    = E_StreamUsage_Dynamic,
		E_TextureUsage_WriteOnly					= E_StreamUsage_WriteOnly,
		E_TextureUsage_StaticWriteOnly			    = E_StreamUsage_StaticWriteOnly, 
		E_TextureUsage_DynamicWriteOnly			    = E_StreamUsage_DynamicWriteOnly,
		E_TextureUsage_DynamicWriteOnlyDiscardable  = E_StreamUsage_DynamicWriteOnlyDiscardable,
		E_TextureUsage_AutoMipMap					= 0x10,
		E_TextureUsage_RenderTarget				    = 0x20,
		E_TextureUsage_NotSRV						= 0x40,
		E_TextureUsage_UnorderedAccessView 	        = 0x80,
        E_TextureUsage_UnorderedAccessViewNotSRV    = E_TextureUsage_UnorderedAccessView | E_TextureUsage_NotSRV,
		E_TextureUsage_Default		                = E_TextureUsage_AutoMipMap | E_TextureUsage_StaticWriteOnly
	};


    enum EImageFlagType
	{
		E_ImageFlag_IsCompressed	= 0x00000001,           //1: IsCompressed
		E_ImageFlag_IsCubeMap		= 0x00000002,           //2: IsCubeMap
		E_ImageFlag_Is3DTexture	    = 0x00000004            //4: Is3DTexture
	};

	
	enum EImageFilterType		
	{
		E_ImageFilter_Nearest = 0,                          //0: Nearest
		E_ImageFilter_Linear,                               //1: Linear
		E_ImageFilter_Bilinear,                             //2: Bilinear
		E_ImageFilter_Box,                                  //3: Box
		E_ImageFilter_Triangle,                             //4: Triangle
		E_ImageFilter_Bicubic                               //5: Bicubic
	};


    enum EObjectType
    {
        E_Object_Mesh = 0,                                  //0:    Mesh
        E_Object_SkinMesh,                                  //1:    SkinMesh
        E_Object_Camera,                                    //2:    Camera
        E_Object_Light,                                     //3:    Light
        E_Object_Terrain,                                   //4:    Terrain
        E_Object_Water,                                     //5:    Water
        E_Object_Sky,                                       //6:    Sky
        E_Object_Cloud,                                     //7:    Cloud
        E_Object_Particle,                                  //8:    Particle

        E_Object_Count,                
    };
    engineExport const String& E_GetObjectTypeName(EObjectType type);
    engineExport const String& E_GetObjectTypeName(int type);
    engineExport EObjectType E_ParseObjectType(const String& strName);


    enum EResourceType
    {
        E_Resource_Mesh = 0,                                //0:    Mesh
        E_Resource_Texture,                                 //1:    Texture
        E_Resource_Shader,                                  //2:    Shader

        E_Resource_Count,
    };
    engineExport const String& E_GetResourceypeName(EResourceType type);
    engineExport const String& E_GetResourceTypeName(int type);
    engineExport EResourceType E_ParseResourceType(const String& strName);
    

    enum EResourceLoadingType
    {
        E_ResourceLoading_Unloaded = 0,		                //0:    Unloaded
        E_ResourceLoading_Loading,			                //1:    Loading
        E_ResourceLoading_Loaded,			                //2:    Loaded
        E_ResourceLoading_Unloading,		                //3:    Unloading
        E_ResourceLoading_Prepared,			                //4:    Prepared
        E_ResourceLoading_Preparing			                //5:    Preparing
    };


    enum ECpuFeatureType
    {
		E_CpuFeature_NONE          	= 0,			        //0:  	 None

#if F_CPU == F_CPU_X86
		E_CpuFeature_SSE            	= 1 << 0,		    //1<<0:  SSE
		E_CpuFeature_SSE2            	= 1 << 1,		    //1<<1:  SSE2
		E_CpuFeature_SSE3            	= 1 << 2,		    //1<<2:  SSE3
		E_CpuFeature_SSE41           	= 1 << 3,		    //1<<3:  SSE41
		E_CpuFeature_SSE42           	= 1 << 4,		    //1<<4:  SSE42
		E_CpuFeature_MMX             	= 1 << 5,		    //1<<5:  MMX 
		E_CpuFeature_MMXEXT          	= 1 << 6,		    //1<<6:  MMXEXT
		E_CpuFeature_3DNOW           	= 1 << 7,		    //1<<7:  3DNOW
		E_CpuFeature_3DNOWEXT        	= 1 << 8,		    //1<<8:  3DNOWEXT
		E_CpuFeature_CMOV            	= 1 << 9,		    //1<<9:  CMOV
		E_CpuFeature_TSC             	= 1 << 10,		    //1<<10: TSC
		E_CpuFeature_INVARIANT_TSC   	= 1 << 11,		    //1<<11: INVARIANT_TSC
		E_CpuFeature_FPU             	= 1 << 12,		    //1<<12: FPU
		E_CpuFeature_PRO             	= 1 << 13,		    //1<<13: PRO
		E_CpuFeature_HTT             	= 1 << 14,		    //1<<14: HTT
#elif F_CPU == F_CPU_ARM          
		E_CpuFeature_VFP             	= 1 << 15,		    //1<<15: VFP
		E_CpuFeature_NEON            	= 1 << 16,		    //1<<16: NEON
#elif F_CPU == F_CPU_MIPS         
        E_CpuFeature_MSA             	= 1 << 17,		    //1<<17: MSA
#endif
    };


	enum ECpuInfoType
	{
		E_CpuInfo_Vender = 0,			                    //0:  Vender
		E_CpuInfo_Brand,						            //1:  Brand
		E_CpuInfo_Code,						                //2:  Code
		E_CpuInfo_VenderType,					            //3:  VenderType
		E_CpuInfo_NumCore,					                //4:  NumCore
		E_CpuInfo_LogicalCpus,				                //5:  LogicalCPUs
		E_CpuInfo_TotalLogicalCpus,			                //6:  TotalLogicalCPUs
		E_CpuInfo_L1DataCache,				                //7:  L1DataCache
		E_CpuInfo_L1InstCache,				                //8:  L1InstCache
		E_CpuInfo_L2Cache,					                //9:  L2Cache
		E_CpuInfo_L3Cache,					                //10: L3Cache
		E_CpuInfo_L1Cache_DataAsso,			                //11: L1Cache_DataAsso
		E_CpuInfo_L2Cache_Asso,				                //12: L2Cache_Asso
		E_CpuInfo_L3Cache_Asso,				                //13: L3Cache_Asso
		E_CpuInfo_L1Cache_DataLine,			                //14: L1Cache_DataLine
		E_CpuInfo_L2Cache_Line,				                //15: L2Cache_Line
		E_CpuInfo_L3Cache_Line,				                //16: L3Cache_Line
		E_CpuInfo_SSESize,					                //17: SSESize
		E_CpuInfo_SSE,						                //18: SSE
		E_CpuInfo_SSE2,						                //19: SSE2
		E_CpuInfo_SSE3,						                //20: SSE3
		E_CpuInfo_SSE41,						            //21: SSE41
		E_CpuInfo_SSE42,						            //22: SSE42
		E_CpuInfo_MMX,						                //23: MMX		
		E_CpuInfo_MMXEXT,						            //24: MMXEXT
		E_CpuInfo_3DNOW,						            //25: 3DNOW
		E_CpuInfo_3DNOWEXT,					                //26: 3DNOWEXT
		E_CpuInfo_CMOV,						                //27: CMOV
		E_CpuInfo_TSC,						                //28: TSC
		E_CpuInfo_INVARIANT_TSC,						    //29: INVARIANT_TSC
		E_CpuInfo_FPU,						                //30: FPU
		E_CpuInfo_PRO,								        //31: PRO				
		E_CpuInfo_HTT,							            //32: HTT
		E_CpuInfo_VFP,							            //33: VFP
		E_CpuInfo_NEON,							            //34: NEON
		E_CpuInfo_MSA,							            //35: MSA

		E_CpuInfo_Count						
	};
    engineExport const String& E_GetCpuInfoTypeName(ECpuInfoType type);
	engineExport const String& E_GetCpuInfoTypeName(int type);


    enum EGpuVendorType
	{
		E_GpuVendor_Unknown = 0,                            //0:  Unknown
		E_GpuVendor_Nvidia,                                 //1:  Nvidia
		E_GpuVendor_AMD,                                    //2:  AMD
		E_GpuVendor_Intel,                                  //3:  Intel
		E_GpuVendor_Imagination,                            //4:  Imagination
		E_GpuVendor_Apple,                                  //5:  Apple
		E_GpuVendor_Nokia,                                  //6:  Nokia
		E_GpuVendor_MS_Software,                            //7:  MS Software
		E_GpuVendor_MS_Warp,                                //8:  MS Warp
		E_GpuVendor_Arm,                                    //9:  Arm Mali 
		E_GpuVendor_Qualcomm,                               //10: Qualcomm
		E_GpuVendor_Mozilla,                                //11: Mozilla/Firefox WebGL
		E_GpuVendor_Webkit,                                 //12: Webkit/Chrome WebGL

		E_GpuVendor_Count
	};
    engineExport const String& E_GetGpuVendorTypeName(EGpuVendorType type);
    engineExport const String& E_GetGpuVendorTypeName(int type);


    enum EEngineConfigType
	{
    ////Common
		E_EngineConfig_Common_MultiThread = 0,	            //0:  MultiThread

    ////Render
        E_EngineConfig_Render_RendererName,                 //1:  RendererName
        
    ////Audio


    ////Physics

		
		E_EngineConfig_Count
	};
    engineExport const String& E_GetEngineConfigTypeName(EEngineConfigType type);
    engineExport const String& E_GetEngineConfigTypeName(int type);


    enum ERenderCapabilityCategoryType
	{
		E_RenderCapabilityCategory_Common = 0,		        //0: Common
		E_RenderCapabilityCategory_Buffer,			        //1: Buffer
		E_RenderCapabilityCategory_Texture,			        //2: Texture
		E_RenderCapabilityCategory_Shader,			        //3: Shader

		E_RenderCapabilityCategory_DirectX,			        //4: DirectX
		E_RenderCapabilityCategory_Vulkan,			        //5: Vulkan
		E_RenderCapabilityCategory_Metal,			        //6: Metal
		E_RenderCapabilityCategory_OpenGL,			        //7: OpenGL/OpenGL ES

		E_RenderCapabilityCategory_Count			        //8: Count
	};
	#define E_RC_CAPS_BITSHIFT (32 - E_RenderCapabilityCategory_Count)
	#define E_RC_CAPS_CATEGORY_MASK (((1 << E_RenderCapabilityCategory_Count) - 1) << E_RC_CAPS_BITSHIFT)
	//cat:[0 - Count], val:[0 - (32-Count-1)] Cur: cat:[0 - 7], val:[0 - 23]
	#define E_RC_CAPS_VALUE(cat, val) ((cat << E_RC_CAPS_BITSHIFT) | (1 << val))

	enum ERenderCapabilityType
	{
	////Common
		E_RenderCapability_Common_PrimitiveRestart = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 0), //Common: Supports specifying "-1" in index buffer to start a new draw command
		E_RenderCapability_Common_WideLines = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 1), //Common: Supports linewidth != 1.0
        E_RenderCapability_Common_HWOcclusion = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 2), //Common: Supports hardware occlusion queries
        E_RenderCapability_Common_UserClipPlanes = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 3), //Common: Supports user clip planes
        E_RenderCapability_Common_PointSprites = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 4), //Common: Supports basic point sprite rendering
        E_RenderCapability_Common_PointExtendedParameters = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 5), //Common: Supports extra point parameters (minsize, maxsize, attenuation)
        E_RenderCapability_Common_FixedFunction = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 6), //Common: Supports fixed-function pipeline
        E_RenderCapability_Common_MRTDifferentBitDepths = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 7), //Common: Supports MRTs with different bit depths
        E_RenderCapability_Common_AlphaToCoverage = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 8), //Common: Supports Alpha to Coverage (A2C)
        E_RenderCapability_Common_HWGamma = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 9), //Common: Supports HW gamma, both in the framebuffer and as texture
        E_RenderCapability_Common_RTTMainDepthBufferAttachable = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 10), //Common: Supports using the MAIN depth buffer for RTTs. D3D 9&10, OGL w/FBO support unknown (undefined behavior?), OGL w/ copy supports it
        E_RenderCapability_Common_RTTDepthBufferResolutionLessEqual = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 11), //Common: Supports attaching a depth buffer to an RTT that has width & height less or equal than RTT's. Otherwise must be of _exact_ same resolution. D3D 9, OGL 3.0 (not 2.0, not D3D10)
		E_RenderCapability_Common_32BitIndex = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Common, 12), //Common: Supports 32bit hardware index buffers
		

	////Buffer
		E_RenderCapability_Buffer_DepthClamp = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Buffer, 0), //Buffer: Supports depth clamp
		E_RenderCapability_Buffer_StencilHW = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Buffer, 1), //Buffer: Supports hardware stencil buffer
		E_RenderCapability_Buffer_StencilTwoSided = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Buffer, 2), //Buffer: Supports separate stencil updates for both front and back faces
        E_RenderCapability_Buffer_StencilWrap = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Buffer, 3), //Buffer: Supports wrapping the stencil value at the range extremeties
		E_RenderCapability_Buffer_ReadWriteBuffers = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Buffer, 4), //Buffer: Supports read/write buffers with atomic counters (e.g. RWStructuredBuffer or SSBO)


	////Texture
        E_RenderCapability_Texture_1D = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 0), //Texture: Supports 1d textures
        E_RenderCapability_Texture_2DArray = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 1), //Texture: Supports 2D Texture Arrays
        E_RenderCapability_Texture_3D = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 2), //Texture: Supports 3d (volume) textures
		E_RenderCapability_Texture_AutoMipmapCompresse = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 3), //Texture: Supports generating mipmaps for compressed formats in hardware (GL ES2/ES3 does not )
		E_RenderCapability_Texture_Anisotropy = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 4), //Texture: Supports anisotropic texture filtering
        E_RenderCapability_Texture_HWRenderToTexture = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 5), //Texture: Supports hardware render-to-texture (bigger than framebuffer)
        E_RenderCapability_Texture_Float = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 6), //Texture: Supports float textures and render targets
        E_RenderCapability_Texture_NonPower2 = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 7), //Texture: Supports non-power of two textures
        E_RenderCapability_Texture_MipmapLodBias = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 8), //Texture: Supports mipmap LOD biasing
        E_RenderCapability_Texture_Compression = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 9), //Texture: Supports compressed textures 
        E_RenderCapability_Texture_CompressionDXT = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 10), //Texture: Supports compressed textures in the DXT/ST3C formats
        E_RenderCapability_Texture_CompressionVTC = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 11), //Texture: Supports compressed textures in the VTC format 
        E_RenderCapability_Texture_CompressionPVRTC = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 12), //Texture: Supports compressed textures in the PVRTC format 
        E_RenderCapability_Texture_CompressionATC = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 13), //Texture: Supports compressed textures in the ATC format 
        E_RenderCapability_Texture_CompressionETC1 = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 14), //Texture: Supports compressed textures in the ETC1 format 
        E_RenderCapability_Texture_CompressionETC2 = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 15), //Texture: Supports compressed textures in the ETC2 format
        E_RenderCapability_Texture_CompressionBC4BC5 = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 16), //Texture: Supports compressed textures in BC4 and BC5 format (DirectX feature level 10_0)
        E_RenderCapability_Texture_CompressionBC6HBC7 = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 17), //Texture: Supports compressed textures in BC6H and BC7 format (DirectX feature level 11_0)
		E_RenderCapability_Texture_CompressionASTC = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Texture, 18), //Texture: Supports compressed textures in the ASTC format


	////Shader
        E_RenderCapability_Shader_ProgramVertex = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 0), //Shader: Supports vertex programs (vertex shaders)
        E_RenderCapability_Shader_ProgramTesslationHull = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 1), //Shader: Supports hardware tessellation hull programs
		E_RenderCapability_Shader_ProgramTesslationDomain = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 2), //Shader: Supports hardware tessellation domain programs 
        E_RenderCapability_Shader_ProgramGeometry = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 3), //Shader: Supports hardware geometry programs
        E_RenderCapability_Shader_ProgramCompute = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 4), //Shader: Supports hardware compute programs
        E_RenderCapability_Shader_VertexTextureFetch = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 5), //Shader: Supports vertex texture fetch
        E_RenderCapability_Shader_HWRenderToVertexBuffer = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 6), //Shader: Supports rendering to vertex buffers
        E_RenderCapability_Shader_CanGetCompiledShaderBuffer = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 7), //Shader: Supports reading back compiled shaders
        E_RenderCapability_Shader_VertexBufferInstanceData = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_Shader, 8), //Shader: Supports using vertex buffers for instance data
        

	////DirectX
        E_RenderCapability_DirectX_PerStageConstant = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_DirectX, 0), //DirectX: Is DirectX feature "per stage constants" supported
        E_RenderCapability_DirectX_ReadBackAsTexture = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_DirectX, 1), //DirectX: D3D11: supports reading back the inactive depth-stencil buffer as texture
        E_RenderCapability_DirectX_WBuffer = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_DirectX, 2), //DirectX: the renderer will try to use W-buffers when available, W-buffers are enabled by default for 16bit depth buffers and disabled for all other 
        E_RenderCapability_DirectX_HWOcclusionAsynchronous = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_DirectX, 3), //DirectX: D3D11: Supports asynchronous hardware occlusion queries
        E_RenderCapability_DirectX_HWRenderToTexture3D = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_DirectX, 4), //DirectX: Render to texture 3D


	////Vulkan


	////Metal


	////OpenGL/OpenGL ES
        E_RenderCapability_OpenGL_PBuffer = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 0), //OpenGL: Support for PBuffer 
        E_RenderCapability_OpenGL_SeparateShaderObjects = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 1), //OpenGL: Support for Separate Shader Objects 
        E_RenderCapability_OpenGL_VAO = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 2), //OpenGL: Support for Vertex Array Objects (VAOs)
        E_RenderCapability_OpenGL_GLSLSSORedeclare = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 3), //OpenGL: with Separate Shader Objects the gl_PerVertex interface block must be redeclared, but some drivers misbehave and do not compile if we do so 
        E_RenderCapability_OpenGL_Debug = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 4), //OpenGL: Supports debugging/ profiling events 
        E_RenderCapability_OpenGL_MapBuffer = E_RC_CAPS_VALUE(E_RenderCapabilityCategory_OpenGL, 5), //OpenGL: RS can map driver buffer storage directly instead of using a shadow buffer

	};


    enum EFrameProfilerType
    {
        E_FrameProfiler_Audio = 0,                          //0: Audio
        E_FrameProfiler_Memory,                             //1: Memory
        E_FrameProfiler_Physics,                            //2: Physics
        E_FrameProfiler_Render,                             //3: Render
        
        E_FrameProfiler_Count
    };
    engineExport const String& E_GetFrameProfilerTypeName(EFrameProfilerType type);
    engineExport const String& E_GetFrameProfilerTypeName(int type);


////////////////////////////// Class ///////////////////////////////
    struct LightConstants;
    struct PassConstants;
    struct ObjectConstants;
    struct TerrainObjectConstants;
    struct GridObjectConstants;
    struct CameraAxisObjectConstants;
    struct CoordinateAxisObjectConstants;
    struct LineFlat3DObjectConstants;
    struct CopyBlitObjectConstants;
    struct MaterialConstants;
    struct InstanceConstants;

    struct engineExport MeshInfo
    {
        MeshInfo();
        MeshInfo(const String& _nameMesh,
                 const String& _pathMesh,
                 FMeshType _typeMesh,
                 FMeshVertexType _typeVertex,
                 FMeshGeometryType _typeGeometryType,
                 FMeshCreateParam* _pMeshCreateParam,
                 bool _isFlipY,
                 bool _isTransformLocal,
                 const FMatrix4& _matTransformLocal);
        ~MeshInfo();

        uint32 group;
        String nameMesh;
        String pathMesh;
        
        FMeshType typeMesh;
        FMeshVertexType typeVertex;
        FMeshGeometryType typeGeometryType;
        FMeshCreateParam* pMeshCreateParam;

        bool isFlipY;
        bool isTransformLocal;
        FMatrix4 matTransformLocal;
    };
    typedef std::vector<MeshInfo*> MeshInfoPtrVector;
    typedef std::map<String, MeshInfo*> MeshInfoPtrMap;
    typedef std::map<uint32, MeshInfoPtrMap> MeshInfoGroupPtrMap;


    struct engineExport TextureInfo
    {
        TextureInfo();
        TextureInfo(const String& _nameTexture,
                    const String& _pathTexture,
                    FTextureType _typeTexture,
                    FTexturePixelFormatType _typeTexturePixelFormat,
                    FTextureFilterType _typeTextureFilter,
                    FTextureAddressingType _typeTextureAddressing,
                    FTextureBorderColorType _typeTextureBorderColor,
                    int _width,
                    int _height,
                    int _depth,
                    int _animChunkX,
                    int _animChunkY,
                    bool _isRT,
                    bool _isGCS);
        ~TextureInfo();

        uint32 group;
        String nameTexture;
        String pathTexture;
        
        FTextureType typeTexture;
        FTexturePixelFormatType typeTexturePixelFormat;
        FTextureFilterType typeTextureFilter;
        FTextureAddressingType typeTextureAddressing;
        FTextureBorderColorType typeTextureBorderColor;
        int width;
        int height;
        int depth;
        int animChunkX;
        int animChunkY;
        bool isRT;
        bool isGCS;
    };
    typedef std::vector<TextureInfo*> TextureInfoPtrVector;
    typedef std::map<String, TextureInfo*> TextureInfoPtrMap;
    typedef std::map<uint32, TextureInfoPtrMap> TextureInfoGroupPtrMap;


    struct engineExport ShaderInfo
    {
        ShaderInfo();
        ShaderInfo(const String& _nameShader,
                   const String& _pathShader,
                   FShaderType _typeShader);
        ~ShaderInfo();

        uint32 group;
        String nameShader;
        String pathShader;
        FShaderType typeShader;

    };
    typedef std::vector<ShaderInfo*> ShaderInfoPtrVector;
    typedef std::map<String, ShaderInfo*> ShaderInfoPtrMap;
    typedef std::map<uint32, ShaderInfoPtrMap> ShaderInfoGroupPtrMap;

    struct engineExport ShaderModuleInfo
    {
        String nameShader;
        String nameShaderType;
        String pathShader;
    };
    typedef std::vector<ShaderModuleInfo> ShaderModuleInfoVector;


    struct engineExport MaterialInfo
    {
        MaterialInfo();
        MaterialInfo(const String& _nameMaterial,
                     const String& _pathMaterial);
        ~MaterialInfo();

        uint32 group;
        String nameMaterial;
        String pathMaterial;
    };
    typedef std::vector<MaterialInfo*> MaterialInfoPtrVector;
    typedef std::map<String, MaterialInfo*> MaterialInfoPtrMap;
    typedef std::map<uint32, MaterialInfoPtrMap> MaterialInfoGroupPtrMap;


    struct engineExport SceneInfo
    {
        SceneInfo();
        SceneInfo(const String& _nameScene,
                  const String& _pathScene);
        ~SceneInfo();

        uint32 group;
        String nameScene;
        String pathScene;
    };
    typedef std::vector<SceneInfo*> SceneInfoPtrVector;
    typedef std::map<String, SceneInfo*> SceneInfoPtrMap;
    typedef std::map<uint32, SceneInfoPtrMap> SceneInfoGroupPtrMap;


    class App;
    class Sample;
    class WindowBase;
    class WindowListener;
    class Window;

    typedef std::vector<WindowBase*> WindowBasePtrVector;
    typedef std::map<String, WindowBase*> WindowBasePtrMap;

    typedef std::vector<WindowListener*> WindowListenerPtrVector;
    typedef std::map<String, WindowListener*> WindowListenerPtrMap;
    typedef std::vector<Window*> WindowPtrVector;
    typedef std::map<String, Window*> WindowPtrMap;
    

    class Base;
    class DataIndex;
	class DataVertex;
	class DataVertexIndex;
    class Engine;
    class EngineConfig;
    class EngineListener;
    class EngineFrameProfiler;
    class EngineFrameState;
    class EngineFrameStateAudio;
    class EngineFrameStateMemory;
    class EngineFrameStatePhysics;
    class EngineFrameStateRender;
    class Image;
    class ImageCodec;
    class ImageCodecDXT;
    class Material;
    class MaterialData;
    class MaterialDataManager;
    class MaterialDataSerializer;
    class MaterialInstance;
    class MaterialManager;
    class MaterialSerializer;
    class Mesh;
    class MeshDataSerializer;
    class MeshManager;
    class MeshSerializer;
    class MeshSub;
    class Movable;
    class MovableListener;
    class MovableFactory;
    class Node;
    class NodeListener;
    class ObjectVisibleBoundsInfo;
    class Object;
    class ObjectFrustum;
    class ObjectCameraListener;
    class ObjectCamera;
    class ObjectCloud;
    class ObjectLight;
    class ObjectMesh;
    class ObjectParticle;
    class ObjectSkinMesh;
    class ObjectSky;
    class ObjectTerrain;
    class ObjectWater;
    class ObjectManager;
    class Renderable;
    class RenderableIndirect;
    class GpuDriverVersion;
    class RenderCapabilities;
    class RenderEngineListener;
    class RenderEngine;
    class RendererListener;
    class Renderer;
    class RenderPassDescriptor;
    class RenderPassDescriptorManager;
    class RenderPass;
    class RenderPassForwardLit;
    class RenderPassDeferredLit;
    class RenderPassShadowCaster;
    class RenderPassDepthOnly;
    class RenderPassManager;
    class RenderPipeline;
    class RenderPipelineForward;
    class RenderPipelineManager;
    class RenderQueue;
    class RenderQueueManager;
    class RenderTargetListener;
    class RenderTarget;
    class RenderTargetManager;
    class RenderTexture;
    class RenderWindowListener;
    class RenderWindow;
    class RenderStateCommon;
    class RenderStateLighting;
    class RenderStateParam;
    class RenderStateTexture;
    class RenderStateShaderItem;
    class RenderStateShader;
    class RenderState;
    class ResourceManualLoader;
    class ResourceListener;
    class Resource;
    class ResourceManager;
    class ResourceLoadingListener;
    class ResourceGroupListener;
    class ResourceGroupManager;
    class Scene;
    class SceneDataManager;
    class SceneDataSerializer;
    class SceneManager;
    class SceneManagerEnumerator;
    class SceneNode;
    class SceneSerializer;
    class Shader;
    class ShaderManager;
    class ShaderSerializer;
    class Stream;
	class StreamIndex;
	class StreamIndexSystem;
	class StreamLock;
	class StreamManager;
	class StreamTemp;
	class StreamTempBlended;
	class StreamTexture;
	class StreamVertex;
	class StreamVertexBinding;
	class StreamVertexBindingManager;
	class StreamVertexSystem;
    class SystemCapabilities;
    class Texture;
    class TextureManager;
    class TextureSerializer;
    class VertexDeclaration;
	class VertexDeclarationManager;
	class VertexElement;
    class Viewport;


    typedef std::vector<DataVertex*> DataVertexPtrVector;
	typedef std::list<DataVertex*> DataVertexPtrList;
	typedef std::vector<DataIndex*> DataIndexPtrVector;
	typedef std::list<DataIndex*> DataIndexPtrList;

    typedef std::vector<MeshSub*> MeshSubPtrVector;
    typedef std::map<String, MeshSub*> MeshSubPtrMap;
    
    typedef std::vector<Mesh*> MeshPtrVector;
    typedef std::map<String, Mesh*> MeshPtrMap;
    typedef std::map<uint32, MeshPtrMap> MeshGroupPtrMap;

    typedef std::vector<Texture*> TexturePtrVector;
    typedef std::map<String, Texture*> TexturePtrMap;
    typedef std::map<uint32, TexturePtrMap> TextureGroupPtrMap;
    typedef std::map<String, TexturePtrVector> TexturePtrShaderSortMap;

    typedef std::vector<Shader*> ShaderPtrVector;
    typedef std::map<String, Shader*> ShaderPtrMap;
    typedef std::map<uint32, ShaderPtrMap> ShaderGroupPtrMap;

    typedef std::vector<RendererListener*> RendererListenerPtrVector;
    typedef std::list<RendererListener*> RendererListenerPtrList;
    typedef std::map<String, RendererListener*> RendererListenerPtrMap;

    typedef std::list<Renderer*> RendererPtrList;
    typedef std::map<String, Renderer*> RendererPtrMap;

    typedef std::vector<RenderPassDescriptor*> RenderPassDescriptorPtrVector;
    typedef std::map<String, RenderPassDescriptor*> RenderPassDescriptorPtrMap;
    typedef std::vector<RenderPass*> RenderPassPtrVector;
    typedef std::map<String, RenderPass*> RenderPassPtrMap;

    typedef std::vector<RenderPipeline*> RenderPipelinePtrVector;
    typedef std::map<String, RenderPipeline*> RenderPipelinePtrMap;
    
    typedef std::vector<RenderQueue*> RenderQueuePtrVector;
    typedef std::map<String, RenderQueue*> RenderQueuePtrMap;

    typedef std::vector<RenderTargetListener*> RenderTargetListenerPtrVector;
    typedef std::map<String, RenderTargetListener*> RenderTargetListenerPtrMap;
    typedef std::vector<RenderTarget*> RenderTargetPtrVector;
    typedef std::map<String, RenderTarget*> RenderTargetPtrMap;
    typedef std::multimap<uint32, RenderTarget*> RenderTargetPtrPriorityMap;

    typedef std::vector<RenderTexture*> RenderTexturePtrVector;
    typedef std::map<String, RenderTexture*> RenderTexturePtrMap;

    typedef std::vector<RenderWindowListener*> RenderWindowListenerPtrVector;
    typedef std::map<String, RenderWindowListener*> RenderWindowListenerPtrMap;
    typedef std::vector<RenderWindow*> RenderWindowPtrVector;
    typedef std::map<String, RenderWindow*> RenderWindowPtrMap;

    typedef std::vector<RenderStateCommon*> RenderStateCommonPtrVector;
    typedef std::map<String, RenderStateCommon*> RenderStateCommonPtrMap;
    typedef std::vector<RenderStateLighting*> RenderStateLightingPtrVector;
    typedef std::map<String, RenderStateLighting*> RenderStateLightingPtrMap;
    typedef std::vector<RenderStateParam*> RenderStateParamPtrVector;
    typedef std::map<String, RenderStateParam*> RenderStateParamPtrMap;
    typedef std::vector<RenderStateTexture*> RenderStateTexturePtrVector;
    typedef std::map<String, RenderStateTexture*> RenderStateTexturePtrMap;
    typedef std::vector<RenderStateShaderItem*> RenderStateShaderItemPtrVector;
    typedef std::map<String, RenderStateShaderItem*> RenderStateShaderItemPtrMap;
    typedef std::vector<RenderStateShader*> RenderStateShaderPtrVector;
    typedef std::map<String, RenderStateShader*> RenderStateShaderPtrMap;
    typedef std::vector<RenderState*> RenderStatePtrVector;
    typedef std::map<String, RenderState*> RenderStatePtrMap;

    typedef std::vector<Image*> ImagePtrVector;
    typedef std::map<String, Image*> ImagePtrMap;

    typedef std::vector<Material*> MaterialPtrVector;
    typedef std::map<String, Material*> MaterialPtrMap;
    typedef std::map<uint32, MaterialPtrMap> MaterialGroupPtrMap;

    typedef std::vector<MaterialInstance*> MaterialInstancePtrVector;
    typedef std::map<String, MaterialInstance*> MaterialInstancePtrMap;

    typedef std::vector<MaterialData*> MaterialDataPtrVector;
    typedef std::map<String, MaterialData*> MaterialDataPtrMap;

    typedef std::vector<Renderable*> RenderablePtrVector;
    typedef std::map<String, Renderable*> RenderablePtrMap;

    typedef std::vector<RenderableIndirect*> RenderableIndirectPtrVector;
    typedef std::map<String, RenderableIndirect*> RenderableIndirectPtrMap;

    typedef std::vector<Movable*> MovablePtrVector;
    typedef std::map<String, Movable*> MovablePtrMap;
    typedef std::vector<Movable*> MovablePtrVector;
    typedef std::list<Movable*> MovablePtrList;
    typedef std::map<String, MovableFactory*> MovableFactoryPtrMap;

    typedef std::vector<Node*> NodePtrVector;
    typedef std::list<Node*> NodePtrList;
	typedef std::set<Node*> NodePtrSet;
    typedef std::map<String, Node*> NodePtrMap;

    typedef std::vector<SceneNode*> SceneNodePtrVector;
    typedef std::set<SceneNode*> SceneNodePtrSet;
    typedef std::map<String, SceneNode*> SceneNodePtrMap;

    //Object
    typedef std::vector<Object*> ObjectPtrVector; 
    typedef std::map<String, Object*> ObjectPtrMap;
    typedef std::map<int, ObjectPtrMap> ObjectPtrGroupMap;
    //ObjectMesh
    typedef std::vector<ObjectMesh*> ObjectMeshPtrVector; 
    typedef std::map<String, ObjectMesh*> ObjectMeshPtrMap;
    typedef std::map<int, ObjectMeshPtrMap> ObjectMeshPtrGroupMap;
    //ObjectSkinMesh
    typedef std::vector<ObjectSkinMesh*> ObjectSkinMeshPtrVector; 
    typedef std::map<String, ObjectSkinMesh*> ObjectSkinMeshPtrMap;
    typedef std::map<int, ObjectSkinMeshPtrMap> ObjectSkinMeshPtrGroupMap;
    //ObjectCamera
    typedef std::vector<ObjectCamera*> ObjectCameraPtrVector; 
    typedef std::map<String, ObjectCamera*> ObjectCameraPtrMap;
    typedef std::map<int, ObjectCameraPtrMap> ObjectCameraPtrGroupMap;
    //ObjectLight
    typedef std::vector<ObjectLight*> ObjectLightPtrVector; 
    typedef std::map<String, ObjectLight*> ObjectLightPtrMap;
    typedef std::map<int, ObjectLightPtrMap> ObjectLightPtrGroupMap;
    //ObjectTerrain
    typedef std::vector<ObjectTerrain*> ObjectTerrainPtrVector; 
    typedef std::map<String, ObjectTerrain*> ObjectTerrainPtrMap;
    typedef std::map<int, ObjectTerrainPtrMap> ObjectTerrainPtrGroupMap;
    //ObjectWater
    typedef std::vector<ObjectWater*> ObjectWaterPtrVector; 
    typedef std::map<String, ObjectWater*> ObjectWaterPtrMap;
    typedef std::map<int, ObjectWaterPtrMap> ObjectWaterPtrGroupMap;
    //ObjectSky
    typedef std::vector<ObjectSky*> ObjectSkyPtrVector; 
    typedef std::map<String, ObjectSky*> ObjectSkyPtrMap;
    typedef std::map<int, ObjectSkyPtrMap> ObjectSkyPtrGroupMap;
    //ObjectCloud
    typedef std::vector<ObjectCloud*> ObjectCloudPtrVector; 
    typedef std::map<String, ObjectCloud*> ObjectCloudPtrMap;
    typedef std::map<int, ObjectCloudPtrMap> ObjectCloudPtrGroupMap;
    //ObjectParticle
    typedef std::vector<ObjectParticle*> ObjectParticlePtrVector; 
    typedef std::map<String, ObjectParticle*> ObjectParticlePtrMap;
    typedef std::map<int, ObjectParticlePtrMap> ObjectParticlePtrGroupMap;

    typedef uint32 ResourceHandle;
    typedef std::vector<Resource*> ResourcePtrVector;
    typedef std::map<String, Resource*> ResourcePtrMap;
    typedef std::map<String, ResourcePtrMap> ResourcePtrGroupMap;
	typedef std::map<ResourceHandle, Resource*>	ResourcePtrHandleMap;
    
    typedef std::list<ResourceListener*> ResourceListenerPtrList;
    typedef std::list<ResourceGroupListener*> ResourceGroupListenerPtrList;

    typedef std::vector<Scene*> ScenePtrVector;
    typedef std::map<String, Scene*> ScenePtrMap;
    typedef std::map<int, ScenePtrMap> SceneGroupPtrMap;

    typedef std::vector<SceneManager*> SceneManagerPtrVector;
    typedef std::map<String, SceneManager*> SceneManagerPtrMap;

    typedef std::vector<StreamVertex*> StreamVertexPtrVector;
	typedef std::list<StreamVertex*> StreamVertexPtrList;
	typedef std::set<StreamVertex*> StreamVertexPtrSet;
	typedef std::map<uint16, StreamVertex*> StreamVertexPtrMap;

	typedef std::vector<StreamIndex*> StreamIndexPtrVector;
	typedef std::list<StreamIndex*> StreamIndexPtrList;
	typedef std::set<StreamIndex*> StreamIndexPtrSet;
	typedef std::map<StreamIndex*, int32> StreamIndexPtrMap;

	typedef std::vector<StreamTexture*> StreamTexturePtrVector;
	typedef std::list<StreamTexture*> StreamTexturePtrList;
	typedef std::set<StreamTexture*> StreamTexturePtrSet;
	
	typedef std::map<StreamTemp*, int32> StreamTempPtrMap;

	typedef std::vector<StreamVertexBinding*> StreamVertexBindingPtrVector;
	typedef std::list<StreamVertexBinding*> StreamVertexBindingPtrList;
	typedef std::set<StreamVertexBinding*> StreamVertexBindingPtrSet;

    typedef std::vector<size_t> VertexSizeVector;
    typedef std::vector<VertexElement> VertexElementVector;
	typedef std::list<VertexElement> VertexElementList;

    typedef FPtrListNode<VertexDeclaration> VertexDeclarationPtrListNode;
	typedef FPtrListNode<StreamVertexBinding> VertexStreamBindingPtrListNode;

    typedef std::vector<VertexDeclaration*> VertexDeclarationPtrVector;
	typedef std::list<VertexDeclaration*> VertexDeclarationPtrList;
	typedef std::set<VertexDeclaration*> VertexDeclarationPtrSet;
	typedef std::map<String, VertexDeclaration*> VertexDeclarationPtrMap;

    typedef std::vector<Viewport*> ViewportPtrVector;
    typedef std::map<String, Viewport*> ViewportPtrMap;
    typedef std::map<int, Viewport*> ViewportPtrOrderMap;

    struct engineExport ConfigItem
	{
		String strName;
		String strCurValue;
		StringVector aPossibleValues;
		bool bImmutable;
	};
	typedef std::map<String, ConfigItem> ConfigItemMap;

    struct engineExport RenderTargetEvent
	{
		RenderTarget* pRenderTargetSrc;
	};

	struct engineExport RenderTargetViewportEvent
	{
		Viewport* pViewportSrc;
	};

    struct engineExport RenderFrameEvent
	{
		float fTimeSinceLastEvent;		
		float fTimeSinceLastFrame;
		float fTimeSystemCurrent;
	};

}; //LostPeterEngine


#endif