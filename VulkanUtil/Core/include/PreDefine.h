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

#ifndef _PRE_DEFINE_H_
#define _PRE_DEFINE_H_

#include "PreIncludeStd.h"
#include "PreIncludeThird.h"


namespace LostPeter
{
////////////////////////////// Define //////////////////////////////
    #define MAX_LIGHT_COUNT 16
    #define MAX_TEXTURE_COUNT 16
    #define MAX_OBJECT_COUNT 1024
    #define MAX_MATERIAL_COUNT 64
    #define MAX_INSTANCE_COUNT 1024

    #define SHADER_NAME_Vertex                      "vert"
    #define SHADER_NAME_TessellationControl         "tesc"
    #define SHADER_NAME_TessellationEvaluation      "tese"
    #define SHADER_NAME_Geometry                    "geom"
    #define SHADER_NAME_Fragment                    "frag"
    #define SHADER_NAME_Compute                     "comp"

    #define C_PI                            3.14159265f                                         // PI
    #define C_PI_HALF				        1.57079632f                                         // 0.5 * PI
    #define	C_PI_TWO					    6.28318530717958647692							    // 2.0 * PI
    #define C_PI_INV				        0.31830988618379067154                              // 1.0 / PI
    #define	C_EXP					        2.71828182845904523536							    // e
	#define	C_LOG2_INVERT			        3.32192809488736234787								// 1.0 / log10(2)
	#define	C_LN2					        0.693147180559945									// ln(2)
	#define	C_LN2_INVERT				    1.44269504089										// 1.0f / ln(2)
			
	#define	C_INV3					        0.33333333333333333333							    // 1/3
	#define	C_INV6					        0.16666666666666666666								// 1/6
	#define	C_INV7					        0.14285714285714285714								// 1/7
	#define	C_INV9					        0.11111111111111111111								// 1/9
	#define	C_INV255					    0.00392156862745098039								// 1/255

	#define	C_SQRT2					        1.4142135623730950488							    // sqrt(2)
	#define	C_INVSQRT2				        0.707106781188										// 1 / sqrt(2)
			
	#define	C_SQRT3					        1.7320508075688772935								// sqrt(3)
	#define	C_INVSQRT3				        0.577350269189										// 1 / sqrt(3)             
    
    #define C_MIN_INT8		                ((int8)		-128)
    #define C_MAX_INT8		                ((int8)		0x7f)
    #define C_MIN_UINT8		                ((uint8)	0x00)
    #define C_MAX_UINT8		                ((uint8)	0xff)
    #define C_MIN_INT16		                ((int16)	-32768)
    #define C_MAX_INT16		                ((int16)	0x7fff)
    #define	C_MIN_UINT16		            ((uint16)	0x0000)
    #define C_MAX_UINT16		            ((uint16)	0xffff)
    #define C_MIN_INT32		                ((int32)	0x80000000)
    #define C_MAX_INT32		                ((int32)	0x7fffffff)
    #define	C_MIN_UINT32		            ((uint32)	0x00000000)
    #define C_MAX_UINT32		            ((uint32)	0xffffffff)
    #define C_MIN_INT64		                ((int64)	0x8000000000000000)
    #define C_MAX_INT64		                ((int64)	0x7fffffffffffffff)
    #define C_MIN_UINT64		            ((uint64)	0x0000000000000000)
    #define C_MAX_UINT64		            ((uint64)	0xffffffffffffffff)
    #define	C_MIN_FLOAT				        (-FLT_MAX)	
    #define	C_MAX_FLOAT				        FLT_MAX		
    
    #define C_POS_INFINITY_NEG              (-FLT_MAX)
    #define C_POS_INFINITY                  FLT_MAX

    #define C_DELTA				            0.00001f

    #define C_NUMBER_SMALL		            1.e-8f
    #define C_NUMBER_SMALL_KINDA	        1.e-4f
    #define C_NUMBER_BIG			        3.4e+38f
    #define C_NUMBER_EULERS                 2.71828182845904523536f

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
    #define C_CONFIG_MAX_BUFFER_SOURCE_COUNT				16
    #define C_CONFIG_MAX_LIGHT_COUNT						8
    #define C_CONFIG_MAX_SHADER_PARAM_COUNT				    32
    #define C_CONFIG_MAX_GLYPHS_COUNT				        (9030 - 32)
    #define C_CONFIG_GLYPH_INDEX(c)					        c - 33

    #define UTIL_CPU_ALLOCATOR              nullptr

////////////////////////////// Typedef /////////////////////////////
    using int8 = std::int8_t;
    using uint8 = std::uint8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using int64 = std::int64_t;
    using uint64 = std::uint64_t;

    typedef std::chrono::steady_clock::time_point TimePoint;
    typedef std::string String;
    typedef std::vector<String> StringVector;
    typedef std::list<String> StringList;
    typedef std::map<String, String> String2StringMap;
    typedef std::vector<const char*> ConstCharPtrVector;
    typedef std::vector<char> CharVector;
    typedef std::vector<uint16> Indices16Vector;
    typedef std::vector<uint32> Indices32Vector;
    
    typedef std::vector<bool> BoolVector;
	typedef std::vector<int8> Int8Vector;
	typedef std::vector<uint8> Uint8Vector;
	typedef std::vector<int16> Int16Vector;
	typedef std::vector<uint16> Uint16Vector;
	typedef std::vector<int32> Int32Vector;
	typedef std::vector<uint32> Uint32Vector;
	typedef std::vector<float> FloatVector;
	typedef std::vector<double>	DoubleVector;

    typedef std::deque<TimePoint> FrameTimePointQueue;

    
    #define UTIL_ARRAYSIZE(_ARR)            ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define UTIL_OFFSETOF(_TYPE,_MEMBER)    ((size_t)&(((_TYPE*)0)->_MEMBER))     
    
    #define UTIL_DELETE(p)                  { if(p) { delete p; p=nullptr; }}
    #define UTIL_DELETE_T(p)                { if(p) { delete[] p; p=nullptr; }}

    String Utile_VkResult2String(VkResult result);

    #define UTIL_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            String vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            Util_LogError("UTIL_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), Utile_VkResult2String(result).c_str()); \
        } \
    }

    bool Util_CheckVkResult(VkResult result, const String& nameFunc);

    template<class T>
    static UTIL_FORCEINLINE void Util_ZeroStruct(T& vkStruct, VkStructureType vkType)
    {
        vkStruct.sType = vkType;
        memset(((uint8*)&vkStruct) + sizeof(VkStructureType), 0, sizeof(T) - sizeof(VkStructureType));
    }


////////////////////////////// Enum ////////////////////////////////
    enum VulkanLogType
    {
        Vulkan_Log_Console = 0,                         //0:    Console
        Vulkan_Log_File,                                //0:    File
    };


    enum VulkanFenceStateType
    {
        Vulkan_FenceState_NotReady = 0,
        Vulkan_FenceState_Signaled,
    };


    enum VulkanVertexType
    {
        Vulkan_Vertex_Pos2Color4 = 0,                   //0:    Pos2Color4
        Vulkan_Vertex_Pos3Normal3,                      //1:    Pos3Normal3
        Vulkan_Vertex_Pos3Normal3Tex2,                  //2:    Pos3Normal3Tex2
        Vulkan_Vertex_Pos2Color4Tex2,                   //3:    Pos2Color4Tex2
        Vulkan_Vertex_Pos3Color4Tex2,                   //4:    Pos3Color4Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tex2,            //5:    Pos3Color4Normal3Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tex4,            //6:    Pos3Color4Normal3Tex4
        Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2,    //7:    Pos3Color4Normal3Tangent3Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex4,    //8:    Pos3Color4Normal3Tangent3Tex4

        Vulkan_Vertex_Count
    };
    const String& Util_GetVertexTypeName(VulkanVertexType type);
    const String& Util_GetVertexTypeName(int type);
    VulkanVertexType Util_ParseVertexType(const String& strName);


    enum VulkanMeshType
    {
        Vulkan_Mesh_File = 0,                           //0:    File
        Vulkan_Mesh_Geometry,                           //1:    Geometry

        Vulkan_Mesh_Count
    };
    const String& Util_GetMeshTypeName(VulkanMeshType type);
    const String& Util_GetMeshTypeName(int type);
    VulkanMeshType Util_ParseMeshType(const String& strName);


    enum VulkanMeshGeometryType
    {
        Vulkan_MeshGeometry_Triangle = 0,               //0:    Triangle
        Vulkan_MeshGeometry_Quad,                       //1:    Quad
        Vulkan_MeshGeometry_Grid,                       //2:    Grid
        Vulkan_MeshGeometry_Circle,                     //3:    Circle
        Vulkan_MeshGeometry_AABB,                       //4:    AABB
        Vulkan_MeshGeometry_Sphere,                     //5:    Sphere
        Vulkan_MeshGeometry_GeoSphere,                  //6:    GeoSphere
        Vulkan_MeshGeometry_Cylinder,                   //7:    Cylinder
        Vulkan_MeshGeometry_Capsule,                    //8:    Capsule
        Vulkan_MeshGeometry_Cone,                       //9:    Cone
        Vulkan_MeshGeometry_Torus,                      //10:   Torus
        Vulkan_MeshGeometry_SkyBox,                     //11:   SkyBox
        Vulkan_MeshGeometry_SkyDome,                    //12:   SkyDome
        Vulkan_MeshGeometry_Terrain,                    //13:   Terrain

        Vulkan_MeshGeometry_Count,
    };
    const String& Util_GetMeshGeometryTypeName(VulkanMeshGeometryType type);
    const String& Util_GetMeshGeometryTypeName(int type);
    VulkanMeshGeometryType Util_ParseMeshGeometryType(const String& strName);


    enum VulkanSwapStatusType
    {
        Vulkan_SwapStatus_Normal = 0,
        Vulkan_SwapStatus_OutOfDate = -1,
        Vulkan_SwapStatus_Lost = -2,
        Vulkan_SwapStatus_Error = -3,
    };


    enum VulkanPixelFormatType
    {
        Vulkan_PixelFormat_Unknown = 0,

        Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM,

        Vulkan_PixelFormat_Count,                           
    };
    VkFormat Util_Transform2VkFormat(VulkanPixelFormatType type);
    VkComponentMapping Util_Transform2VkComponentMapping(VulkanPixelFormatType type);

    struct utilExport VulkanPixelFormatDes
	{
		String name;
		uint8 nElemBytes;
		uint32 eFlags;
		uint32 eComponentType;
		uint32 nComponentCount;
        bool isSupported;
		
		uint8 nRbits;
		uint8 nGbits;
		uint8 nBbits;
		uint8 nAbits;
		
		uint64 nRmask;
		uint64 nGmask;
		uint64 nBmask;
		uint64 nAmask;
	
		uint8 nRshift;
		uint8 nGshift;
		uint8 nBshift;
		uint8 nAshift;
	};


    enum VulkanPixelFormatFlagType
	{
        Vulkan_PixelFormatFlag_IsNative		    = 0x00000001,   //0: IsNative
        Vulkan_PixelFormatFlag_IsCompressed	    = 0x00000002,   //1: IsCompressed
        Vulkan_PixelFormatFlag_IsInteger	    = 0x00000004,   //2: IsInteger
        Vulkan_PixelFormatFlag_IsFloat	        = 0x00000008,   //3: IsFloat
		Vulkan_PixelFormatFlag_IsLuminance	    = 0x00000010,   //4: IsLuminance
        Vulkan_PixelFormatFlag_IsStencil        = 0x00000020,   //5: IsStencil
		Vulkan_PixelFormatFlag_IsDepth		    = 0x00000040,   //6: IsDepth
        Vulkan_PixelFormatFlag_IsDepthStencil   = 0x00000080,   //7: IsDepthStencil
		Vulkan_PixelFormatFlag_HasAlpha		    = 0x00000100,   //8: HasAlpha
	};


    enum VulkanPixelFormatComponentType
	{
		Vulkan_PixelFormatComponent_ByteU = 0,              //0: Byte unsigned
        Vulkan_PixelFormatComponent_ByteS,                  //1: Byte signed
		Vulkan_PixelFormatComponent_ShortU,                 //2: Short unsigned
        Vulkan_PixelFormatComponent_ShortS,                 //3: Short signed
        Vulkan_PixelFormatComponent_IntU,					//4: Int unsigned
		Vulkan_PixelFormatComponent_IntS,					//5: Int signed
        Vulkan_PixelFormatComponent_LongU,					//6: Long unsigned
		Vulkan_PixelFormatComponent_LongS,					//7: Long signed
		Vulkan_PixelFormatComponent_Float16,                //8: Float 16
		Vulkan_PixelFormatComponent_Float32,                //9: Float 32
        Vulkan_PixelFormatComponent_Double,                 //10: Double
	};
    const String& Util_GetPixelFormatComponentTypeName(VulkanPixelFormatComponentType type);
    const String& Util_GetPixelFormatComponentTypeName(int type);


    enum VulkanLightType
    {
        Vulkan_Light_Directional = 0,
        Vulkan_Light_Point,
        Vulkan_Light_Spot,
    };


    enum VulkanLightingType
    {
        Vulkan_Lighting_Node = 0,                                //0:    None
        Vulkan_Lighting_Ambient,                                 //1:    Ambient
        Vulkan_Lighting_DiffuseLambert,                          //2:    DiffuseLambert
        Vulkan_Lighting_SpecularPhong,                           //3:    SpecularPhong
        Vulkan_Lighting_SpecularBlinnPhong,                      //4:    SpecularBlinnPhong
        Vulkan_Lighting_AmbientDiffuseLambert,                   //5:    Ambient + DiffuseLambert
        Vulkan_Lighting_AmbientSpecularPhong,                    //6:    Ambient + SpecularPhong
        Vulkan_Lighting_AmbientSpecularBlinnPhong,               //7:    Ambient + SpecularBlinnPhong
        Vulkan_Lighting_DiffuseLambertSpecularPhong,             //8:    DiffuseLambert + SpecularPhong
        Vulkan_Lighting_DiffuseLambertSpecularBlinnPhong,        //9:    DiffuseLambert + SpecularBlinnPhong
        Vulkan_Lighting_AmbientDiffuseLambertSpecularPhong,      //10:   Ambient + DiffuseLambert + SpecularPhong
        Vulkan_Lighting_AmbientDiffuseLambertSpecularBlinnPhong, //11:   Ambient + DiffuseLambert + SpecularBlinnPhong
    };


    enum VulkanTextureType
    {
        Vulkan_Texture_1D = 0,                              //0: 1D
        Vulkan_Texture_2D,                                  //1: 2D
        Vulkan_Texture_2DArray,                             //2: 2DArray
        Vulkan_Texture_3D,                                  //3: 3D
        Vulkan_Texture_CubeMap,                             //4: CubeMap

        Vulkan_Texture_Count,
    };
    const String& Util_GetTextureTypeName(VulkanTextureType type);
    const String& Util_GetTextureTypeName(int type);
    VulkanTextureType Util_ParseTextureType(const String& strName);
    VkImageType Util_Transform2VkImageType(VulkanTextureType type);
    VkImageViewType Util_Transform2VkImageViewType(VulkanTextureType type);


    enum VulkanTextureFilterSizeType
    {
        Vulkan_TextureFilterSize_Min = 0,	                //0: Min		
        Vulkan_TextureFilterSize_Mag,				        //1: Mag
        Vulkan_TextureFilterSize_Mip,				        //2: Mip

        Vulkan_TextureFilterSize_Count,
    };
    const String& Util_GetTextureFilterSizeTypeName(VulkanTextureFilterSizeType type);
    const String& Util_GetTextureFilterSizeTypeName(int type);
    VulkanTextureFilterSizeType Util_ParseTextureFilterSizeType(const String& strName);


    enum VulkanTextureFilterPixelType
    {
        Vulkan_TextureFilterPixel_None	= 0,	            //0: None
        Vulkan_TextureFilterPixel_Point,			        //1: Point
        Vulkan_TextureFilterPixel_Linear,			        //2: Linear
        Vulkan_TextureFilterPixel_Anisotropic,		        //3: Anisotropic

        Vulkan_TextureFilterPixel_Count,
    };
    const String& Util_GetTextureFilterPixelTypeName(VulkanTextureFilterPixelType type);
    const String& Util_GetTextureFilterPixelTypeName(int type);
    VulkanTextureFilterPixelType Util_ParseTextureFilterPixelType(const String& strName);
    VkFilter Util_Transform2VkFilter(VulkanTextureFilterPixelType type);
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(VulkanTextureFilterPixelType type);


    enum VulkanTextureFilterType
    {
        Vulkan_TextureFilter_None = 0,					    //0: min=Vulkan_TextureFilterPixel_Point,       mag=Vulkan_TextureFilterPixel_Point,  	   mip=Vulkan_TextureFilterPixel_None
        Vulkan_TextureFilter_Bilinear,				        //1: min=Vulkan_TextureFilterPixel_Linear,      mag=Vulkan_TextureFilterPixel_Linear, 	   mip=Vulkan_TextureFilterPixel_Point
        Vulkan_TextureFilter_Trilinear,			            //2: min=Vulkan_TextureFilterPixel_Linear,	    mag=Vulkan_TextureFilterPixel_Linear, 	   mip=Vulkan_TextureFilterPixel_Linear
        Vulkan_TextureFilter_Anisotropic,			        //3: min=Vulkan_TextureFilterPixel_Anisotropic, mag=Vulkan_TextureFilterPixel_Anisotropic, mip=Vulkan_TextureFilterPixel_Linear

        Vulkan_TextureFilter_Count,
    };
    const String& Util_GetTextureFilterTypeName(VulkanTextureFilterType type);
    const String& Util_GetTextureFilterTypeName(int type);
    VulkanTextureFilterType Util_ParseTextureFilterType(const String& strName);
    VkFilter Util_Transform2VkFilter(VulkanTextureFilterType typeFilter, VulkanTextureFilterSizeType typeFilterSize);
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(VulkanTextureFilterType typeFilter);


    enum VulkanTextureAddressingType
    {
        Vulkan_TextureAddressing_Wrap = 0,	                //0: Wrap
        Vulkan_TextureAddressing_Mirror,			        //1: Mirror
        Vulkan_TextureAddressing_Clamp,			            //2: Clamp
        Vulkan_TextureAddressing_Border,			        //3: Border

        Vulkan_TextureAddressing_Count,
    };
    const String& Util_GetTextureAddressingTypeName(VulkanTextureAddressingType type);
    const String& Util_GetTextureAddressingTypeName(int type);
    VulkanTextureAddressingType Util_ParseTextureAddressingType(const String& strName);
    VkSamplerAddressMode Util_Transform2VkSamplerAddressMode(VulkanTextureAddressingType type);


    enum VulkanTextureBorderColorType
    {
        Vulkan_TextureBorderColor_OpaqueBlack = 0,          //0: OpaqueBlack
        Vulkan_TextureBorderColor_OpaqueWhite,              //1: OpaqueWhite
        Vulkan_TextureBorderColor_TransparentBlack,         //2: TransparentBlack

        Vulkan_TextureBorderColor_Count,
    };
    const String& Util_GetTextureBorderColorTypeName(VulkanTextureBorderColorType type);
    const String& Util_GetTextureBorderColorTypeName(int type);
    VulkanTextureBorderColorType Util_ParseTextureBorderColorType(const String& strName);
    VkBorderColor Util_Transform2VkBorderColor(VulkanTextureBorderColorType type);


    enum VulkanMSAASampleCountType
    {
        Vulkan_MSAASampleCount_1_Bit = 0,
        Vulkan_MSAASampleCount_2_Bit,
        Vulkan_MSAASampleCount_4_Bit,
        Vulkan_MSAASampleCount_8_Bit,
        Vulkan_MSAASampleCount_16_Bit,
        Vulkan_MSAASampleCount_32_Bit,
        Vulkan_MSAASampleCount_64_Bit,

        Vulkan_MSAASampleCount_Count,
    };
    const String& Util_GetMSAASampleCountTypeName(VulkanMSAASampleCountType type);
    const String& Util_GetMSAASampleCountTypeName(int type);
    VulkanMSAASampleCountType Util_ParseMSAASampleCountType(const String& strName);
    VkSampleCountFlagBits Util_Transform2VkSampleCountFlagBits(VulkanMSAASampleCountType type);

    
    enum VulkanShaderType
    {
        Vulkan_Shader_Vertex = 0,
        Vulkan_Shader_TessellationControl,
        Vulkan_Shader_TessellationEvaluation,
        Vulkan_Shader_Geometry,
        Vulkan_Shader_Fragment,
        Vulkan_Shader_Compute,

        Vulkan_Shader_Count,
    };
    const String& Util_GetShaderTypeName(VulkanShaderType type);
    const String& Util_GetShaderTypeName(int type);
    VulkanShaderType Util_ParseShaderType(const String& strName);
    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(VulkanShaderType type);


////////////////////////////// Vulkan //////////////////////////////
    typedef std::vector<VkFormat> VkFormatVector;
    typedef std::map<VkFormat, VkFormatProperties> VkFormat2PropertiesMap;
    typedef std::vector<VkClearValue> VkClearValueVector;
    
    typedef std::vector<VkQueueFamilyProperties> VkQueueFamilyPropertiesVector;

    typedef std::vector<VkBuffer> VkBufferVector;
    typedef std::vector<VkDeviceMemory> VkDeviceMemoryVector;

    typedef std::vector<VkVertexInputBindingDescription> VkVertexInputBindingDescriptionVector;
    typedef std::vector<VkVertexInputAttributeDescription> VkVertexInputAttributeDescriptionVector;

    typedef std::vector<VkAttachmentDescription> VkAttachmentDescriptionVector;
    typedef std::vector<VkSubpassDescription> VkSubpassDescriptionVector;
    typedef std::vector<VkSubpassDependency> VkSubpassDependencyVector;

    typedef std::vector<VkImage> VkImageVector;
    typedef std::vector<VkImageView> VkImageViewVector;
    typedef std::vector<VkFramebuffer> VkFramebufferVector;

    typedef std::vector<VkViewport> VkViewportVector;
    typedef std::vector<VkRect2D> VkRect2DVector;

    typedef std::vector<VkSemaphore> VkSemaphoreVector;
    typedef std::vector<VkFence> VkFenceVector;
        
    typedef std::vector<VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoVector;
    typedef std::map<String, VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoMap;

    typedef std::vector<VkDescriptorSetLayoutBinding> VkDescriptorSetLayoutBindingVector;

    typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayoutVector;
    typedef std::map<String, VkDescriptorSetLayout> VkDescriptorSetLayoutMap;

    typedef std::vector<VkShaderModule> VkShaderModuleVector;
    typedef std::map<String, VkShaderModule> VkShaderModuleMap;

    typedef std::vector<VkPipelineLayout> VkPipelineLayoutVector;
    typedef std::map<String, VkPipelineLayout> VkPipelineLayoutMap;

    typedef std::vector<VkPipeline> VkPipelineVector;
    typedef std::map<String, VkPipeline> VkPipelineMap;
    
    typedef std::vector<VkDescriptorSet> VkDescriptorSetVector;
    typedef std::vector<VkWriteDescriptorSet> VkWriteDescriptorSetVector;

    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(VulkanVertexType type);
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(VulkanVertexType type);
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(VulkanVertexType type);
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(VulkanVertexType type);


////////////////////////////// Class ///////////////////////////////
    class App;
    
    class VulkanBase;
    class VulkanCamera;
    class VulkanDevice;
    class VulkanDeviceMemoryAllocation;
    class VulkanDeviceMemoryManager;
    class VulkanFence;
    class VulkanFenceManager;
    class VulkanInstance;
    class VulkanLight;
    class VulkanLog;
    class VulkanLogConsole;
    class VulkanLogFile;
    class VulkanLogManager;
    class VulkanManager;
    class VulkanMaterial;
    class VulkanMath;
    class VulkanMathBox;
    class VulkanMesh;
    class VulkanMeshSub;
    class VulkanObject;
    class VulkanPixelBox;
    class VulkanPixelFormat;
    class VulkanQueue;
    class VulkanSample;
    class VulkanSceneManager;
    class VulkanSceneObject;
    class VulkanSemaphore;
    class VulkanSwapChain;
    class VulkanTexture;
    class VulkanTimer;
    class VulkanWindow;
    
    struct Vertex_Pos2Color4;
    struct Vertex_Pos3Normal3;
    struct Vertex_Pos2Color4Tex2;
    struct Vertex_Pos3Color4Tex2;
    struct Vertex_Pos3Color4Normal3Tex2;
    struct Vertex_Pos3Color4Normal3Tex4;
    struct Vertex_Pos3Color4Normal3Tangent3Tex2;
    struct Vertex_Pos3Color4Normal3Tangent3Tex4;
    typedef Vertex_Pos3Color4Normal3Tangent3Tex2 MeshVertex;

    struct MeshData;
    struct SubmeshGeometry;
    struct MeshGeometry;

    struct LightConstants;
    struct PassConstants;
    struct ObjectConstants;
    struct MaterialConstants;
    struct InstanceConstants;

    typedef glm::vec2 Point;
    typedef glm::vec2 Size;
    typedef glm::vec2 Vector2;
    typedef glm::vec3 Vector3;
    typedef glm::vec4 Vector4;
    typedef glm::mat3 Matrix3;
    typedef glm::mat4 Matrix4;
    typedef glm::quat Quaternion;
    typedef glm::vec4 Color;

    typedef std::vector<VulkanLog*> VulkanLogPtrVector;
    typedef std::map<String, VulkanLog*> VulkanLogPtrMap;

    typedef std::vector<VulkanFence*> VulkanFencePtrVector;

    typedef std::vector<VulkanDeviceMemoryAllocation*> VulkanDeviceMemoryAllocationPtrVector;

}; //LostPeter


#endif