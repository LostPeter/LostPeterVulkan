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

    
    #define UTIL_ARRAYSIZE(_ARR)            ((int)(sizeof(_ARR)/sizeof(*_ARR)))    
    #define UTIL_OFFSETOF(_TYPE,_MEMBER)    ((size_t)&(((_TYPE*)0)->_MEMBER))     
    
    #define UTIL_DELETE(p)                  { if(p) { delete p; p=nullptr; }}
    #define UTIL_DELETE_T(p)                { if(p) { delete[] p; p=nullptr; }}

    std::string Utile_VkResult2String(VkResult result);

    #define UTIL_VK_CHECK(vkcall) \
    { \
        VkResult result = vkcall; \
        if (result != VK_SUCCESS) \
        { \
            std::string vkfunc = #vkcall; \
            vkfunc = vkfunc.substr(0, vkfunc.find('(')); \
            Util_LogError("UTIL_VK_CHECK: [%s] failed with: %s", vkfunc.c_str(), Utile_VkResult2String(result).c_str()); \
        } \
    }

    bool Util_CheckVkResult(VkResult result, const std::string& nameFunc);


////////////////////////////// Enum ////////////////////////////////
    enum VulkanLogType
    {
        Vulkan_Log_Console = 0,                         //0:    Console
        Vulkan_Log_File,                                //0:    File
    };


    enum VulkanVertexType
    {
        Vulkan_Vertex_Pos2Color4 = 0,                   //0:    Pos2Color4
        Vulkan_Vertex_Pos3Normal3,                      //1:    Pos3Normal3
        Vulkan_Vertex_Pos2Color4Tex2,                   //2:    Pos2Color4Tex2
        Vulkan_Vertex_Pos3Color4Tex2,                   //3:    Pos3Color4Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tex2,            //4:    Pos3Color4Normal3Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tex4,            //5:    Pos3Color4Normal3Tex4
        Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2,    //6:    Pos3Color4Normal3Tangent3Tex2
        Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex4,    //7:    Pos3Color4Normal3Tangent3Tex4

        Vulkan_Vertex_Count
    };
    const std::string& Util_GetVertexTypeName(VulkanVertexType type);
    const std::string& Util_GetVertexTypeName(int type);
    VulkanVertexType Util_ParseVertexType(const std::string& strName);


    enum VulkanMeshType
    {
        Vulkan_Mesh_File = 0,                           //0:    File
        Vulkan_Mesh_Geometry,                           //1:    Geometry

        Vulkan_Mesh_Count
    };
    const std::string& Util_GetMeshTypeName(VulkanMeshType type);
    const std::string& Util_GetMeshTypeName(int type);
    VulkanMeshType Util_ParseMeshType(const std::string& strName);


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

        Vulkan_MeshGeometry_Count,
    };
    const std::string& Util_GetMeshGeometryTypeName(VulkanMeshGeometryType type);
    const std::string& Util_GetMeshGeometryTypeName(int type);
    VulkanMeshGeometryType Util_ParseMeshGeometryType(const std::string& strName);


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
    };


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
    const std::string& Util_GetTextureTypeName(VulkanTextureType type);
    const std::string& Util_GetTextureTypeName(int type);
    VulkanTextureType Util_ParseTextureType(const std::string& strName);
    VkImageType Util_Transform2VkImageType(VulkanTextureType type);
    VkImageViewType Util_Transform2VkImageViewType(VulkanTextureType type);


    enum VulkanTextureFilterSizeType
    {
        Vulkan_TextureFilterSize_Min = 0,	                //0: Min		
        Vulkan_TextureFilterSize_Mag,				        //1: Mag
        Vulkan_TextureFilterSize_Mip,				        //2: Mip

        Vulkan_TextureFilterSize_Count,
    };
    const std::string& Util_GetTextureFilterSizeTypeName(VulkanTextureFilterSizeType type);
    const std::string& Util_GetTextureFilterSizeTypeName(int type);
    VulkanTextureFilterSizeType Util_ParseTextureFilterSizeType(const std::string& strName);


    enum VulkanTextureFilterPixelType
    {
        Vulkan_TextureFilterPixel_None	= 0,	            //0: None
        Vulkan_TextureFilterPixel_Point,			        //1: Point
        Vulkan_TextureFilterPixel_Linear,			        //2: Linear
        Vulkan_TextureFilterPixel_Anisotropic,		        //3: Anisotropic

        Vulkan_TextureFilterPixel_Count,
    };
    const std::string& Util_GetTextureFilterPixelTypeName(VulkanTextureFilterPixelType type);
    const std::string& Util_GetTextureFilterPixelTypeName(int type);
    VulkanTextureFilterPixelType Util_ParseTextureFilterPixelType(const std::string& strName);
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
    const std::string& Util_GetTextureFilterTypeName(VulkanTextureFilterType type);
    const std::string& Util_GetTextureFilterTypeName(int type);
    VulkanTextureFilterType Util_ParseTextureFilterType(const std::string& strName);
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
    const std::string& Util_GetTextureAddressingTypeName(VulkanTextureAddressingType type);
    const std::string& Util_GetTextureAddressingTypeName(int type);
    VulkanTextureAddressingType Util_ParseTextureAddressingType(const std::string& strName);
    VkSamplerAddressMode Util_Transform2VkSamplerAddressMode(VulkanTextureAddressingType type);


    enum VulkanTextureBorderColorType
    {
        Vulkan_TextureBorderColor_OpaqueBlack = 0,          //0: OpaqueBlack
        Vulkan_TextureBorderColor_OpaqueWhite,              //1: OpaqueWhite
        Vulkan_TextureBorderColor_TransparentBlack,         //2: TransparentBlack

        Vulkan_TextureBorderColor_Count,
    };
    const std::string& Util_GetTextureBorderColorTypeName(VulkanTextureBorderColorType type);
    const std::string& Util_GetTextureBorderColorTypeName(int type);
    VulkanTextureBorderColorType Util_ParseTextureBorderColorType(const std::string& strName);
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
    const std::string& Util_GetMSAASampleCountTypeName(VulkanMSAASampleCountType type);
    const std::string& Util_GetMSAASampleCountTypeName(int type);
    VulkanMSAASampleCountType Util_ParseMSAASampleCountType(const std::string& strName);
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
    const std::string& Util_GetShaderTypeName(VulkanShaderType type);
    const std::string& Util_GetShaderTypeName(int type);
    VulkanShaderType Util_ParseShaderType(const std::string& strName);
    VkShaderStageFlagBits Util_Transform2VkShaderStageFlagBits(VulkanShaderType type);


////////////////////////////// Vulkan //////////////////////////////
    typedef std::vector<VkVertexInputBindingDescription> VkVertexInputBindingDescriptionVector;
    typedef std::vector<VkVertexInputAttributeDescription> VkVertexInputAttributeDescriptionVector;

    typedef std::vector<VkViewport> VkViewportVector;
    typedef std::vector<VkRect2D> VkRect2DVector;

    typedef std::vector<VkSemaphore> VkSemaphoreVector;
    typedef std::vector<VkFence> VkFenceVector;
        
    typedef std::vector<VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoVector;
    typedef std::map<std::string, VkPipelineShaderStageCreateInfo> VkPipelineShaderStageCreateInfoMap;

    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(VulkanVertexType type);
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(VulkanVertexType type);
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(VulkanVertexType type);
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(VulkanVertexType type);

    typedef std::vector<VkDescriptorSetLayout> VkDescriptorSetLayoutVector;
    typedef std::map<std::string, VkDescriptorSetLayout> VkDescriptorSetLayoutMap;

    typedef std::vector<VkShaderModule> VkShaderModuleVector;
    typedef std::map<std::string, VkShaderModule> VkShaderModuleMap;

    typedef std::vector<VkPipelineLayout> VkPipelineLayoutVector;
    typedef std::map<std::string, VkPipelineLayout> VkPipelineLayoutMap;

    typedef std::vector<VkPipeline> VkPipelineVector;
    typedef std::map<std::string, VkPipeline> VkPipelineMap;


////////////////////////////// Class ///////////////////////////////
    class App;
    
    class VulkanBase;
    class VulkanCamera;
    class VulkanDevice;
    class VulkanInstance;
    class VulkanLight;
    class VulkanLog;
    class VulkanLogConsole;
    class VulkanLogFile;
    class VulkanLogManager;
    class VulkanManager;
    class VulkanMaterial;
    class VulkanMesh;
    class VulkanMeshSub;
    class VulkanObject;
    class VulkanQueue;
    class VulkanSceneManager;
    class VulkanSceneObject;
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

    typedef std::vector<VulkanLog*> VulkanLogPtrVector;
    typedef std::map<std::string, VulkanLog*> VulkanLogPtrMap;


}; //LostPeter


#endif