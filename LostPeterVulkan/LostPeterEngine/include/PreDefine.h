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
#include "UIPreDefine.h"
using namespace LostPeterUI;

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
    #define C_CONFIG_MAX_BUFFER_SOURCE_COUNT				16
    #define C_CONFIG_MAX_LIGHT_COUNT						8
    #define C_CONFIG_MAX_SHADER_PARAM_COUNT				    32
    #define C_CONFIG_MAX_GLYPHS_COUNT				        (9030 - 32)
    #define C_CONFIG_GLYPH_INDEX(c)					        c - 33

////////////////////////////// Typedef /////////////////////////////
    


////////////////////////////// Enum ////////////////////////////////
    enum EWindowType
    {
        E_Window_Main = 0,                         //0:    Main
        E_Window_Game,                             //1:    Game
        E_Window_Scene,                            //2:    Scene

        E_Window_Count,
    };
    engineExport const String& E_GetWindowTypeName(EWindowType type);
    engineExport const String& E_GetWindowTypeName(int type);
    engineExport EWindowType E_ParseWindowType(const String& strName);


    enum ELightType
    {
        E_Light_Directional = 0,
        E_Light_Point,
        E_Light_Spot,
    };


    enum ERenderPipelineType
	{
		E_RenderPipeline_Forward = 0,                       //0: Forward
		E_RenderPipeline_Deferred,                          //1: Deferred

		E_RenderPipeline_Count
	};
    engineExport const String& E_GetRenderPipelineTypeName(ERenderPipelineType type);
    engineExport const String& E_GetRenderPipelineTypeName(int32 type);
    

    enum EVertexElementSemanticType
	{
		E_VertexElementSemantic_Position = 0,		        //0: Position
		E_VertexElementSemantic_BlendWeights,			    //1: BlendWeights
		E_VertexElementSemantic_BlendIndices,			    //2: BlendIndices
		E_VertexElementSemantic_Normal,				        //3: Normal
		E_VertexElementSemantic_Diffuse,				    //4: Diffuse
		E_VertexElementSemantic_Specular,				    //5: Specular
		E_VertexElementSemantic_TextureCoordinates,	        //6: TextureCoordinates
		E_VertexElementSemantic_BiNormal,				    //7: BiNormal
		E_VertexElementSemantic_Tangent,				    //8: Tangent
	};			


	enum EVertexElementDataType
	{
		E_VertexElementData_Float1 = 0,				        //0:  Float1
		E_VertexElementData_Float2,					        //1:  Float2
		E_VertexElementData_Float3,					        //2:  Float3
		E_VertexElementData_Float4,					        //3:  Float4
		E_VertexElementData_Color,					        //4:  Color
		E_VertexElementData_Short1,					        //5:  Short1
		E_VertexElementData_Short2,					        //6:  Short2
		E_VertexElementData_Short3,					        //7:  Short3
		E_VertexElementData_Short4,					        //8:  Short4
		E_VertexElementData_UByte4,					        //9:  UByte4
		E_VertexElementData_ColorARGB,			            //10: ColorARGB, D3D
		E_VertexElementData_ColorABGR		                //11: ColorABGR, OpenGL
	};
    

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
    engineExport const String& Util_GetDescriptorSetTypeName(VulkanDescriptorSetType type);
    engineExport const String& Util_GetDescriptorSetTypeName(int type);
    engineExport VulkanDescriptorSetType Util_ParseDescriptorSetType(const String& strName);


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
    engineExport const String& Util_GetDescriptorSetUsageTypeName(VulkanDescriptorSetUsageType type);
    engineExport const String& Util_GetDescriptorSetUsageTypeName(int type);
    engineExport VulkanDescriptorSetUsageType Util_ParseDescriptorSetUsageType(const String& strName);
    engineExport VkDescriptorType Util_Transform2VkDescriptorType(VulkanDescriptorSetUsageType type);    

    
    enum EObjectType
    {
        E_Object_Mesh = 0,                  //0:    Mesh
        E_Object_SkinMesh,                  //1:    SkinMesh
        E_Object_Camera,                    //2:    Camera
        E_Object_Light,                     //3:    Light
        E_Object_Terrain,                   //4:    Terrain
        E_Object_Water,                     //5:    Water
        E_Object_Sky,                       //6:    Sky
        E_Object_Cloud,                     //7:    Cloud
        E_Object_Particle,                  //8:    Particle

        E_Object_Count,                
    };
    engineExport const String& E_GetObjectTypeName(EObjectType type);
    engineExport const String& E_GetObjectTypeName(int type);
    engineExport EObjectType E_ParseObjectType(const String& strName);


    enum ERenderQueueType
    {
        E_RenderQueue_BackGround = 0,           //0:    BackGround
        E_RenderQueue_Opaque,                   //1:    Opaque
        E_RenderQueue_Terrain,                  //2:    Terrain
        E_RenderQueue_Sky,                      //3:    Sky
        E_RenderQueue_Transparent,              //4:    Transparent
        E_RenderQueue_UI,                       //5:    UI

        E_RenderQueue_Count,
    };
    engineExport const String& E_GetRenderQueueTypeName(ERenderQueueType type);
    engineExport const String& E_GetRenderQueueTypeName(int type);
    engineExport ERenderQueueType E_ParseRenderQueueType(const String& strName);


    enum EEngineConfigType
	{
    ////Common
		E_EngineConfig_Common_MultiThread = 0,	       //0: MultiThread

    ////Render


    ////Audio


    ////Physics

		
		E_EngineConfig_Count
	};
    engineExport const String& E_GetEngineConfigTypeName(EEngineConfigType type);
    engineExport const String& E_GetEngineConfigTypeName(int type);


    enum EFrameProfilerType
    {
        E_FrameProfiler_Audio = 0,                //0: Audio
        E_FrameProfiler_Memory,                   //1: Memory
        E_FrameProfiler_Physics,                  //2: Physics
        E_FrameProfiler_Render,                   //3: Render
        
        E_FrameProfiler_Count
    };
    engineExport const String& E_GetFrameProfilerTypeName(EFrameProfilerType type);
    engineExport const String& E_GetFrameProfilerTypeName(int type);


////////////////////////////// Vulkan //////////////////////////////    
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


    struct engineExport DescriptorSetInfo
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


    struct engineExport DescriptorSetLayoutInfo
    {
        DescriptorSetLayoutInfo();
        DescriptorSetLayoutInfo(const String& _nameDescriptorSetLayoutInfo);
        ~DescriptorSetLayoutInfo();

        String nameDescriptorSetLayoutInfo;
    };
    typedef std::vector<DescriptorSetLayoutInfo*> DescriptorSetLayoutInfoPtrVector;
    typedef std::map<String, DescriptorSetLayoutInfo*> DescriptorSetLayoutInfoPtrMap;


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
    class Window;

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
    class RenderEngineListener;
    class RenderEngine;
    class RendererListener;
    class Renderer;
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
    class RenderWindow;
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
    typedef std::multimap<uint8, RenderTarget*> RenderTargetPtrPriorityMap;

    typedef std::vector<RenderTexture*> RenderTexturePtrVector;
    typedef std::map<String, RenderTexture*> RenderTexturePtrMap;

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