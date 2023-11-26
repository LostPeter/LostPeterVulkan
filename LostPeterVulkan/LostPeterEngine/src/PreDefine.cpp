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


    //ERenderPipelineType
     const String s_nameRenderPipeLineTypes[] = 
	{
		"Forward",
		"Deferred"
	};
    const String& E_GetRenderPipelineTypeName(ERenderPipelineType type)
    {
        return s_nameRenderPipeLineTypes[(int32)type];
    }
    const String& E_GetRenderPipelineTypeName(int32 type)
    {
        return s_nameRenderPipeLineTypes[type];
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


    //VulkanObjectType
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
    const String& Util_GetObjectTypeName(VulkanObjectType type)
    {
        return s_nameObjectTypes[(int)type];
    }
    const String& Util_GetObjectTypeName(int type)
    {
        return s_nameObjectTypes[type];
    }
    VulkanObjectType Util_ParseObjectType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_Object_Count; i++)
        {
            if (s_nameObjectTypes[i] == strName)
                return (VulkanObjectType)(i);
        }
        F_Assert(false && "Util_ParseObjectType: Wrong type name !")
        return Vulkan_Object_Mesh;
    }


    //VulkanRenderQueueType
    static String s_nameRenderQueueTypes[] = 
    {
        "BackGround",               //0:    BackGround            
        "Opaque",                   //1:    Opaque
        "Terrain",                  //2:    Terrain
        "Sky",                      //3:    Sky
        "Transparent",              //4:    Transparent
        "UI",                       //5:    UI
    };
    const String& Util_GetRenderQueueTypeName(VulkanRenderQueueType type)
    {
        return s_nameRenderQueueTypes[(int)type];
    }
    const String& Util_GetRenderQueueTypeName(int type)
    {
        return s_nameRenderQueueTypes[type];
    }
    VulkanRenderQueueType Util_ParseRenderQueueType(const String& strName)
    {
        for (size_t i = 0; i < (int)Vulkan_RenderQueue_Count; i++)
        {
            if (s_nameRenderQueueTypes[i] == strName)
                return (VulkanRenderQueueType)(i);
        }
        F_Assert(false && "Util_ParseRenderQueueType: Wrong type name !")
        return Vulkan_RenderQueue_Opaque;
    }


    //VulkanEngineConfigType
    static const String s_nameEngineConfigTypes[] = 
	{
    ////Common
		"MultiThread",					    //0: MultiThread

    ////Render


    ////Audio


    ////Physics
        

	};
    const String& Util_GetEngineConfigTypeName(VulkanEngineConfigType type)
    {
        return s_nameEngineConfigTypes[(int32)type];
    }
    const String& Util_GetEngineConfigTypeName(int32 type)
    {
        return s_nameEngineConfigTypes[type];
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