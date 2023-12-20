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
        "Main",             //0:    Main
        "Game",             //1:    Game
        "Scene",            //2:    Scene
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
        for (size_t i = 0; i < (int)E_Window_Count; i++)
        {
            if (s_nameWindows[i] == strName)
                return (EWindowType)(i);
        }
        F_Assert(false && "E_ParseWindowType: Wrong type name !")
        return E_Window_Main;
    }


    //VulkanDescriptorSetType
    static String s_nameDescriptorSetTypes[] = 
    {
        "Pass",                     //0:   Pass
        "Object",                   //1:   Object
        "ObjectTerrain",            //2:   ObjectTerrain
        "ObjectGrid",               //3:   ObjectGrid
        "ObjectCameraAxis",         //4:   ObjectCameraAxis
        "ObjectCoordinateAxis",     //5:   ObjectCoordinateAxis
        "ObjectLineFlat2D",         //6:   ObjectLineFlat2D
        "ObjectLineFlat3D",         //7:   ObjectLineFlat3D
        "ObjectCopyBlit",           //8:   ObjectCopyBlit
        "Material",                 //9:   Material
        "Instance",                 //10:  Instance
        "TextureCopy",              //11:  TextureCopy
        "Tessellation",             //12:  Tessellation
        "Geometry",                 //13:  Geometry
        "TextureVS",                //14:  TextureVS
        "TextureTESC",              //15:  TextureTESC
        "TextureTESE",              //16:  TextureTESE
        "TextureFS",                //17:  TextureFS
        "TextureFrameColor",        //18:  TextureFrameColor
        "TextureCSR",               //19:  TextureCSR
        "TextureCSRW",              //20:  TextureCSRW
    };
    const String& Util_GetDescriptorSetTypeName(VulkanDescriptorSetType type)
    {
        return s_nameDescriptorSetTypes[(int)type];
    }
    const String& Util_GetDescriptorSetTypeName(int type)
    {
        return s_nameDescriptorSetTypes[type];
    }
    VulkanDescriptorSetType Util_ParseDescriptorSetType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_DescriptorSet_Count; i++)
        {
            if (s_nameDescriptorSetTypes[i] == strName)
                return (VulkanDescriptorSetType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetType: Wrong type name !")
        return Vulkan_DescriptorSet_Pass;
    }


    //VulkanDescriptorSetUsageType
    static String s_nameDescriptorSetUsageTypes[] = 
    {
        "Sampler",                  //0:  Sampler                   VK_DESCRIPTOR_TYPE_SAMPLER
        "CombinedImageSampler",     //1:  CombinedImageSampler      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
        "SampledImage",             //2:  SampledImage              VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE
        "StorageImage",             //3:  StorageImage              VK_DESCRIPTOR_TYPE_STORAGE_IMAGE
        "UniformTexelBuffer",       //4:  UniformTexelBuffer        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER
        "StorageTexelBuffer",       //5:  StorageTexelBuffer        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER
        "UniformBuffer",            //6:  UniformBuffer             VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER
        "StorageBuffer",            //7:  StorageBuffer             VK_DESCRIPTOR_TYPE_STORAGE_BUFFER
        "UniformBufferDynamic",     //8:  UniformBufferDynamic      VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
        "StorageBufferDynamic",     //9:  StorageBufferDynamic      VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
        "InputAttachment",          //10: InputAttachment           VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
    };
    const String& Util_GetDescriptorSetUsageTypeName(VulkanDescriptorSetUsageType type)
    {
        return s_nameDescriptorSetUsageTypes[(int)type];
    }
    const String& Util_GetDescriptorSetUsageTypeName(int type)
    {
        return s_nameDescriptorSetUsageTypes[type];
    }
    VulkanDescriptorSetUsageType Util_ParseDescriptorSetUsageType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_DescriptorSetUsage_Count; i++)
        {
            if (s_nameDescriptorSetUsageTypes[i] == strName)
                return (VulkanDescriptorSetUsageType)(i);
        }
        F_Assert(false && "Util_ParseDescriptorSetUsageType: Wrong type name !")
        return Vulkan_DescriptorSetUsage_UniformBuffer;
    }
    VkDescriptorType Util_Transform2VkDescriptorType(VulkanDescriptorSetUsageType type)
    {
        switch((int)type)
        {
        case Vulkan_DescriptorSetUsage_Sampler:                     return VK_DESCRIPTOR_TYPE_SAMPLER;
        case Vulkan_DescriptorSetUsage_CombinedImageSampler:        return VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        case Vulkan_DescriptorSetUsage_SampledImage:                return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case Vulkan_DescriptorSetUsage_StorageImage:                return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case Vulkan_DescriptorSetUsage_UniformTexelBuffer:          return VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
        case Vulkan_DescriptorSetUsage_StorageTexelBuffer:          return VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
        case Vulkan_DescriptorSetUsage_UniformBuffer:               return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case Vulkan_DescriptorSetUsage_StorageBuffer:               return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case Vulkan_DescriptorSetUsage_UniformBufferDynamic:        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
        case Vulkan_DescriptorSetUsage_StorageBufferDynamic:        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        case Vulkan_DescriptorSetUsage_InputAttachment:             return VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        }
        F_Assert(false && "Util_Transform2VkDescriptorType: Wrong type !")
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }


    //EObjectType
    static String s_nameObjectTypes[] = 
    {
        "Mesh",                 //0:    Mesh            
        "SkinMesh",             //1:    SkinMesh
        "Camera",               //2:    Camera
        "Light",                //3:    Light
        "Terrain",              //4:    Terrain
        "Water",                //5:    Water
        "Sky",                  //6:    Sky
        "Cloud",                //7:    Cloud
        "Particle",             //8:    Particle
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
        for (size_t i = 0; i < (int)E_Object_Count; i++)
        {
            if (s_nameObjectTypes[i] == strName)
                return (EObjectType)(i);
        }
        F_Assert(false && "E_ParseObjectType: Wrong type name !")
        return E_Object_Mesh;
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


////////////////////////////// Vulkan //////////////////////////////
    const VkVertexInputBindingDescriptionVector& Util_GetVkVertexInputBindingDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptions();
        }
        F_Assert(false && "Util_GetVkVertexInputBindingDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptions();
    }
    const VkVertexInputAttributeDescriptionVector& Util_GetVkVertexInputAttributeDescriptionVector(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptions();
        }
        F_Assert(false && "Util_GetVkVertexInputAttributeDescriptionVector: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptions();
    }
    VkVertexInputBindingDescriptionVector* Util_GetVkVertexInputBindingDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetBindingDescriptionsPtr();
        }
        F_Assert(false && "Util_GetVkVertexInputBindingDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetBindingDescriptionsPtr();
    }   
    VkVertexInputAttributeDescriptionVector* Util_GetVkVertexInputAttributeDescriptionVectorPtr(FMeshVertexType type)
    {
        switch ((int)type)
        {
            case F_MeshVertex_Pos2Color4:
                return Vertex_Pos2Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4:
                return Vertex_Pos3Color4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3:
                return Vertex_Pos3Normal3::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Normal3Tex2:
                return Vertex_Pos3Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos2Color4Tex2:
                return Vertex_Pos2Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Tex2:
                return Vertex_Pos3Color4Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex2:
                return Vertex_Pos3Color4Normal3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tex4:
                return Vertex_Pos3Color4Normal3Tex4::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex2:
                return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
            case F_MeshVertex_Pos3Color4Normal3Tangent3Tex4:
                return Vertex_Pos3Color4Normal3Tangent3Tex4::GetAttributeDescriptionsPtr();
        }
        F_Assert(false && "Util_GetVkVertexInputAttributeDescriptionVectorPtr: Wrong type !")
        return Vertex_Pos3Color4Normal3Tangent3Tex2::GetAttributeDescriptionsPtr();
    }



    ////////////////////////////// Class ///////////////////////////////
    MeshInfo::MeshInfo()
        : group(0)
        , nameMesh("")
        , pathMesh("")
        , typeMesh(F_Mesh_File)
        , typeVertex(F_MeshVertex_Pos3Color4Tex2)
        , typeGeometryType(F_MeshGeometry_EntityGrid)
        , pMeshCreateParam(nullptr)
        , isFlipY(false)
        , isTransformLocal(false)
        , matTransformLocal(FMath::ms_mat4Unit)
    {

    }
    MeshInfo::MeshInfo(const String& _nameMesh,
                       const String& _pathMesh,
                       FMeshType _typeMesh,
                       FMeshVertexType _typeVertex,
                       FMeshGeometryType _typeGeometryType,
                       FMeshCreateParam* _pMeshCreateParam,
                       bool _isFlipY,
                       bool _isTransformLocal,
                       const FMatrix4& _matTransformLocal)
        : group(0)
        , nameMesh(_nameMesh)
        , pathMesh(_pathMesh)
        , typeMesh(_typeMesh)
        , typeVertex(_typeVertex)
        , typeGeometryType(_typeGeometryType)
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
        , typeTexturePixelFormat(F_TexturePixelFormat_R8G8B8A8_SRGB)
        , typeTextureFilter(F_TextureFilter_Bilinear)
        , typeTextureAddressing(F_TextureAddressing_Clamp)
        , typeTextureBorderColor(F_TextureBorderColor_OpaqueBlack)
        , width(512)
        , height(512)
        , depth(1)
        , animChunkX(0)
        , animChunkY(0)
        , isRT(false)
        , isGCS(false)
    {

    }
    TextureInfo::TextureInfo(const String& _nameTexture,
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
                             bool _isGCS)
        : group(0)
        , nameTexture(_nameTexture)
        , pathTexture(_pathTexture)
        , typeTexture(_typeTexture)
        , typeTexturePixelFormat(_typeTexturePixelFormat)
        , typeTextureFilter(_typeTextureFilter)
        , typeTextureAddressing(_typeTextureAddressing)
        , typeTextureBorderColor(_typeTextureBorderColor)
        , width(_width)
        , height(_height)
        , depth(_depth)
        , animChunkX(_animChunkX)
        , animChunkY(_animChunkY)
        , isRT(_isRT)
        , isGCS(_isGCS)
    {

    }
    TextureInfo::~TextureInfo()
    {

    }


    ShaderInfo::ShaderInfo()
        : group(0)
        , nameShader("")
        , pathShader("")
        , typeShader(F_Shader_Vertex)
    {

    }
    ShaderInfo::ShaderInfo(const String& _nameShader,
                           const String& _pathShader,
                           FShaderType _typeShader)
        : group(0)
        , nameShader(_nameShader)
        , pathShader(_pathShader)
        , typeShader(_typeShader)
    {

    }
    ShaderInfo::~ShaderInfo()
    {

    }


    DescriptorSetInfo::DescriptorSetInfo()
        : nameDescriptorSetInfo("")
        , typeDescriptorSet()
        , typeDescriptorSetUsage()
        , nameShaderTypes("")
    {

    }
    DescriptorSetInfo::DescriptorSetInfo(const String& _nameDescriptorSetInfo,
                                         VulkanDescriptorSetType _typeDescriptorSet,
                                         VulkanDescriptorSetUsageType _typeDescriptorSetUsage,
                                         const String& _nameShaderTypes)
        : nameDescriptorSetInfo(_nameDescriptorSetInfo)
        , typeDescriptorSet(_typeDescriptorSet)
        , typeDescriptorSetUsage(_typeDescriptorSetUsage)
        , nameShaderTypes(_nameShaderTypes)
    {

    }
    DescriptorSetInfo::~DescriptorSetInfo()
    {

    }


    DescriptorSetLayoutInfo::DescriptorSetLayoutInfo()
        : nameDescriptorSetLayoutInfo("")
    {

    }
    DescriptorSetLayoutInfo::DescriptorSetLayoutInfo(const String& _nameDescriptorSetLayoutInfo)
        : nameDescriptorSetLayoutInfo(_nameDescriptorSetLayoutInfo)
    {

    }
    DescriptorSetLayoutInfo::~DescriptorSetLayoutInfo()
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