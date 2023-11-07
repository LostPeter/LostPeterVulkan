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

#include "FPreDefine.h"
#include "FPreInclude.h"
using namespace LostPeterFoundation;
#include "UIPreDefine.h"
using namespace LostPeterUI;

namespace LostPeter
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
    #define C_CONFIG_MAX_BUFFER_SOURCE_COUNT				16
    #define C_CONFIG_MAX_LIGHT_COUNT						8
    #define C_CONFIG_MAX_SHADER_PARAM_COUNT				    32
    #define C_CONFIG_MAX_GLYPHS_COUNT				        (9030 - 32)
    #define C_CONFIG_GLYPH_INDEX(c)					        c - 33

    #define UTIL_CPU_ALLOCATOR              nullptr

////////////////////////////// Typedef /////////////////////////////
    String Utile_VkResult2String(VkResult result);

    #define UTIL_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            String vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            F_LogError("*********************** UTIL_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), Utile_VkResult2String(result).c_str()); \
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
    enum VulkanWindowType
    {
        Vulkan_Window_Main = 0,                         //0:    Main
        Vulkan_Window_Game,                             //1:    Game
        Vulkan_Window_Scene,                            //2:    Scene

        Vulkan_Window_Count,
    };
    const String& Util_GetWindowTypeName(VulkanWindowType type);
    const String& Util_GetWindowTypeName(int type);
    VulkanWindowType Util_ParseWindowType(const String& strName);


    enum VulkanFenceStateType
    {
        Vulkan_FenceState_NotReady = 0,
        Vulkan_FenceState_Signaled,
    };


    enum VulkanSwapStatusType
    {
        Vulkan_SwapStatus_Normal = 0,
        Vulkan_SwapStatus_OutOfDate = -1,
        Vulkan_SwapStatus_Lost = -2,
        Vulkan_SwapStatus_Error = -3,
    };



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


    VkImageType Util_Transform2VkImageType(FTextureType type);
    VkImageViewType Util_Transform2VkImageViewType(FTextureType type);
    VkFormat Util_Transform2VkFormat(FTexturePixelFormatType type);
    VkComponentMapping Util_Transform2VkComponentMapping(FTexturePixelFormatType type);
    VkFilter Util_Transform2VkFilter(FTextureFilterPixelType type);
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(FTextureFilterPixelType type);
    VkFilter Util_Transform2VkFilter(FTextureFilterType typeFilter, FTextureFilterSizeType typeFilterSize);
    VkSamplerMipmapMode Util_Transform2VkSamplerMipmapMode(FTextureFilterType typeFilter);
    VkSamplerAddressMode Util_Transform2VkSamplerAddressMode(FTextureAddressingType type);
    VkBorderColor Util_Transform2VkBorderColor(FTextureBorderColorType type);
    VkSampleCountFlagBits Util_Transform2VkSampleCountFlagBits(FMSAASampleCountType type);

    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(FShaderType type);    
    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(const Uint32Vector& aShaderTypes);    

    VkPrimitiveTopology Util_Transform2VkPrimitiveTopology(FRenderPrimitiveType type);
    VkCullModeFlags Util_Transform2VkCullModeFlags(FCullingType type);
    VkPolygonMode Util_Transform2VkPolygonMode(FPolygonType type);
    VkStencilOp Util_Transform2VkStencilOp(FStencilOPType type);
    VkCompareOp Util_Transform2VkCompareOp(FCompareFuncType type);
    VkBlendOp Util_Transform2VkBlendOp(FSceneBlendingOPType type);
    VkBlendFactor Util_Transform2VkBlendFactor(FSceneBlendingFactorType type);
    

    enum VulkanDescriptorSetType
    {
        Vulkan_DescriptorSet_Pass = 0,                      //0:  Pass
        Vulkan_DescriptorSet_Object,                        //1:  Object
        Vulkan_DescriptorSet_ObjectTerrain,                 //2:  ObjectTerrain
        Vulkan_DescriptorSet_ObjectGrid,                    //3:  ObjectGrid
        Vulkan_DescriptorSet_ObjectCameraAxis,              //4:  ObjectCameraAxis
        Vulkan_DescriptorSet_ObjectCoordinateAxis,          //5:  ObjectCoordinateAxis
        Vulkan_DescriptorSet_ObjectLineFlat2D,              //6:  ObjectLineFlat2D
        Vulkan_DescriptorSet_ObjectLineFlat3D,              //7:  ObjectLineFlat3D
        Vulkan_DescriptorSet_ObjectCopyBlit,                //8:  ObjectCopyBlit
        Vulkan_DescriptorSet_Material,                      //9:  Material
        Vulkan_DescriptorSet_Instance,                      //10: Instance
        Vulkan_DescriptorSet_TextureCopy,                   //11: TextureCopy
        Vulkan_DescriptorSet_Tessellation,                  //12: Tessellation
        Vulkan_DescriptorSet_Geometry,                      //13: Geometry
        Vulkan_DescriptorSet_TextureVS,                     //14: TextureVS
        Vulkan_DescriptorSet_TextureTESC,                   //15: TextureTESC
        Vulkan_DescriptorSet_TextureTESE,                   //16: TextureTESE
        Vulkan_DescriptorSet_TextureFS,                     //17: TextureFS
        Vulkan_DescriptorSet_TextureFrameColor,             //18: TextureFrameColor
        Vulkan_DescriptorSet_TextureCSR,                    //19: TextureCSR
        Vulkan_DescriptorSet_TextureCSRW,                   //20: TextureCSRW

        Vulkan_DescriptorSet_Count,
    };
    const String& Util_GetDescriptorSetTypeName(VulkanDescriptorSetType type);
    const String& Util_GetDescriptorSetTypeName(int type);
    VulkanDescriptorSetType Util_ParseDescriptorSetType(const String& strName);


    enum VulkanDescriptorSetUsageType
    {
        Vulkan_DescriptorSetUsage_Sampler = 0,              //0:  Sampler                   VK_DESCRIPTOR_TYPE_SAMPLER
        Vulkan_DescriptorSetUsage_CombinedImageSampler,     //1:  CombinedImageSampler      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        Vulkan_DescriptorSetUsage_SampledImage,             //2:  SampledImage              VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        Vulkan_DescriptorSetUsage_StorageImage,             //3:  StorageImage              VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        Vulkan_DescriptorSetUsage_UniformTexelBuffer,       //4:  UniformTexelBuffer        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
        Vulkan_DescriptorSetUsage_StorageTexelBuffer,       //5:  StorageTexelBuffer        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
        Vulkan_DescriptorSetUsage_UniformBuffer,            //6:  UniformBuffer             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        Vulkan_DescriptorSetUsage_StorageBuffer,            //7:  StorageBuffer             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        Vulkan_DescriptorSetUsage_UniformBufferDynamic,     //8:  UniformBufferDynamic      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
        Vulkan_DescriptorSetUsage_StorageBufferDynamic,     //9:  StorageBufferDynamic      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        Vulkan_DescriptorSetUsage_InputAttachment,          //10: InputAttachment           VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT

        Vulkan_DescriptorSetUsage_Count,
    };
    const String& Util_GetDescriptorSetUsageTypeName(VulkanDescriptorSetUsageType type);
    const String& Util_GetDescriptorSetUsageTypeName(int type);
    VulkanDescriptorSetUsageType Util_ParseDescriptorSetUsageType(const String& strName);
    VkDescriptorType Util_Transform2VkDescriptorType(VulkanDescriptorSetUsageType type);    


    enum VulkanObjectType
    {
        Vulkan_Object_Mesh = 0,             //0:    Mesh
        Vulkan_Object_SkinMesh,             //1:    SkinMesh
        Vulkan_Object_Camera,               //2:    Camera
        Vulkan_Object_Light,                //3:    Light
        Vulkan_Object_Terrain,              //4:    Terrain
        Vulkan_Object_Water,                //5:    Water
        Vulkan_Object_Sky,                  //6:    Sky
        Vulkan_Object_Cloud,                //7:    Cloud
        Vulkan_Object_Particle,             //8:    Particle

        Vulkan_Object_Count,                
    };
    const String& Util_GetObjectTypeName(VulkanObjectType type);
    const String& Util_GetObjectTypeName(int type);
    VulkanObjectType Util_ParseObjectType(const String& strName);


    enum VulkanRenderQueueType
    {
        Vulkan_RenderQueue_BackGround = 0,  //0:    BackGround
        Vulkan_RenderQueue_Opaque,          //1:    Opaque
        Vulkan_RenderQueue_Terrain,         //2:    Terrain
        Vulkan_RenderQueue_Sky,             //3:    Sky
        Vulkan_RenderQueue_Transparent,     //4:    Transparent
        Vulkan_RenderQueue_UI,              //5:    UI

        Vulkan_RenderQueue_Count,
    };
    const String& Util_GetRenderQueueTypeName(VulkanRenderQueueType type);
    const String& Util_GetRenderQueueTypeName(int type);
    VulkanRenderQueueType Util_ParseRenderQueueType(const String& strName);



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

    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(FMeshVertexType type);
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(FMeshVertexType type);
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(FMeshVertexType type);
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(FMeshVertexType type);


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

    struct utilExport MeshInfo
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


    struct utilExport TextureInfo
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


    struct utilExport ShaderInfo
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

    struct utilExport ShaderModuleInfo
    {
        String nameShader;
        String nameShaderType;
        String pathShader;
    };
    typedef std::vector<ShaderModuleInfo> ShaderModuleInfoVector;


    struct utilExport DescriptorSetInfo
    {
        DescriptorSetInfo();
        DescriptorSetInfo(const String& _nameDescriptorSetInfo,
                          VulkanDescriptorSetType _typeDescriptorSet,
                          VulkanDescriptorSetUsageType _typeDescriptorSetUsage,
                          const String& _nameShaderTypes);
        ~DescriptorSetInfo();

        String nameDescriptorSetInfo;
        VulkanDescriptorSetType typeDescriptorSet;
        VulkanDescriptorSetUsageType typeDescriptorSetUsage;
        String nameShaderTypes;
    };
    typedef std::vector<DescriptorSetInfo*> DescriptorSetInfoPtrVector;
    typedef std::map<String, DescriptorSetInfo*> DescriptorSetInfoPtrMap;


    struct utilExport DescriptorSetLayoutInfo
    {
        DescriptorSetLayoutInfo();
        DescriptorSetLayoutInfo(const String& _nameDescriptorSetLayoutInfo);
        ~DescriptorSetLayoutInfo();

        String nameDescriptorSetLayoutInfo;
    };
    typedef std::vector<DescriptorSetLayoutInfo*> DescriptorSetLayoutInfoPtrVector;
    typedef std::map<String, DescriptorSetLayoutInfo*> DescriptorSetLayoutInfoPtrMap;


    struct utilExport MaterialInfo
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


    struct utilExport SceneInfo
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
    class VulkanBase;
    class VulkanWindow;

    class VKDescriptorSet;
    class VKDescriptorSetManager;
    class VKDescriptorSetSerializer;
    class VKDescriptorSetLayout;
    class VKDescriptorSetLayoutManager;
    class VKDescriptorSetLayoutSerializer;
    class VKFrameBufferAttachment;
    class VKMultiRenderPass;
    class VKPipelineGraphics;
    class VKPipelineCompute;
    class VKPipelineLayout;
    class VKPipelineLayoutManager;
    class VKPipeline;
    class VKPipelineManager;

    typedef std::vector<VKDescriptorSet*> VKDescriptorSetPtrVector;
    typedef std::map<String, VKDescriptorSet*> VKDescriptorSetPtrMap;
    typedef std::vector<VKDescriptorSetLayout*> VKDescriptorSetLayoutPtrVector;
    typedef std::map<String, VKDescriptorSetLayout*> VKDescriptorSetLayoutPtrMap;
    
    typedef std::vector<VKMultiRenderPass*> VKMultiRenderPassPtrVector;
    typedef std::map<String, VKMultiRenderPass*> VKMultiRenderPassPtrMap;
    
    typedef std::vector<VKPipelineGraphics*> VKPipelineGraphicsPtrVector;
    typedef std::map<String, VKPipelineGraphics*> VKPipelineGraphicsPtrMap;
    
    typedef std::vector<VKPipelineCompute*> VKPipelineComputePtrVector;
    typedef std::map<String, VKPipelineCompute*> PipelineComputePtrMap;

    typedef std::vector<VKPipelineLayout*> VKPipelineLayoutPtrVector;
    typedef std::map<String, VKPipelineLayout*> VKPipelineLayoutPtrMap;

    typedef std::vector<VKPipeline*> VKPipelinePtrVector;
    typedef std::map<String, VKPipeline*> VKPipelinePtrMap;
    

    class Base;
    class EditorBase;
    class EditorCameraAxis;
    class EditorCoordinateAxis;
    class EditorGrid;
    class EditorLineFlat2DCollector;
    class EditorLineFlat3DCollector;
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
    class Node;
    class Object;
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
    class RenderTarget;
    class RenderTexture;
    class RenderTextureManager;
    class RenderStateCommon;
    class RenderStateLighting;
    class RenderStateParam;
    class RenderStateTexture;
    class RenderStateShaderItem;
    class RenderStateShader;
    class RenderState;
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
    class Texture;
    class TextureManager;
    class TextureSerializer;
    class Viewport;

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

    typedef std::vector<RenderPass*> RenderPassPtrVector;
    typedef std::map<String, RenderPass*> RenderPassPtrMap;

    typedef std::vector<RenderPipeline*> RenderPipelinePtrVector;
    typedef std::map<String, RenderPipeline*> RenderPipelinePtrMap;
    
    typedef std::vector<RenderQueue*> RenderQueuePtrVector;
    typedef std::map<String, RenderQueue*> RenderQueuePtrMap;

    typedef std::vector<RenderTexture*> RenderTexturePtrVector;
    typedef std::map<String, RenderTexture*> RenderTexturePtrMap;

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

    typedef std::vector<Node*> NodePtrVector;
    typedef std::map<String, Node*> NodePtrMap;

    typedef std::vector<SceneNode*> SceneNodePtrVector;
    typedef std::map<String, SceneNode*> SceneNodePtrMap;

    typedef std::vector<Object*> ObjectPtrVector;
    typedef std::map<String, Object*> ObjectPtrMap;
    typedef std::vector<ObjectCamera*> ObjectCameraPtrVector;
    typedef std::map<String, ObjectCamera*> ObjectCameraPtrMap;
    typedef std::vector<ObjectLight*> ObjectLightPtrVector;
    typedef std::map<String, ObjectLight*> ObjectLightPtrMap;
    typedef std::vector<ObjectMesh*> ObjectMeshPtrVector;
    typedef std::map<String, ObjectMesh*> ObjectMeshPtrMap;
    typedef std::vector<ObjectSkinMesh*> ObjectSkinMeshPtrVector;
    typedef std::map<String, ObjectSkinMesh*> ObjectSkinMeshPtrMap;
    typedef std::vector<ObjectParticle*> ObjectParticlePtrVector;
    typedef std::map<String, ObjectParticle*> ObjectParticlePtrMap;
    typedef std::vector<ObjectSky*> ObjectSkyPtrVector;
    typedef std::map<String, ObjectSky*> ObjectSkyPtrMap;
    typedef std::vector<ObjectCloud*> ObjectCloudPtrVector;
    typedef std::map<String, ObjectCloud*> ObjectCloudPtrMap;
    typedef std::vector<ObjectWater*> ObjectWaterPtrVector;
    typedef std::map<String, ObjectWater*> ObjectWaterPtrMap;
    typedef std::vector<ObjectTerrain*> ObjectTerrainPtrVector;
    typedef std::map<String, ObjectTerrain*> ObjectTerrainPtrMap;

    typedef std::vector<Scene*> ScenePtrVector;
    typedef std::map<String, Scene*> ScenePtrMap;
    typedef std::vector<SceneManager*> SceneManagerPtrVector;
    typedef std::map<String, SceneManager*> SceneManagerPtrMap;

    typedef std::vector<Viewport*> ViewportPtrVector;
    typedef std::map<String, Viewport*> ViewportPtrMap;
    

}; //LostPeter


#endif