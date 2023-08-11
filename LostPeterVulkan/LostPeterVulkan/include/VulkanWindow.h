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

#ifndef _VULKAN_WINDOW_H_
#define _VULKAN_WINDOW_H_

#include "VulkanBase.h"

namespace LostPeter
{
    class utilExport VulkanWindow : public VulkanBase
    {
    public:
        VulkanWindow(int width, int height, String name);
        virtual ~VulkanWindow();

    public:
        /////////////////////////// MeshInfo //////////////////////////
        struct utilExport MeshInfo
        {
            MeshInfo()
                : nameMesh("")
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
            MeshInfo(const String& _nameMesh,
                     const String& _pathMesh,
                     FMeshType _typeMesh,
                     FMeshVertexType _typeVertex,
                     FMeshGeometryType _typeGeometryType,
                     FMeshCreateParam* _pMeshCreateParam,
                     bool _isFlipY,
                     bool _isTransformLocal,
                     const FMatrix4& _matTransformLocal)
                : nameMesh(_nameMesh)
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
            ~MeshInfo()
            {
                F_DELETE(this->pMeshCreateParam)
            }

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


        /////////////////////////// ShaderModuleInfo //////////////////
        struct utilExport ShaderModuleInfo
        {
            String nameShader;
            String nameShaderType;
            String pathShader;
        };
        typedef std::vector<ShaderModuleInfo> ShaderModuleInfoVector;


    public:
        class Mesh;
        
        /////////////////////////// MeshSub ///////////////////////////
        class utilExport MeshSub
        {
        public:
            MeshSub(Mesh* _pMesh, 
                    const String& _nameMeshSub,
                    const String& _nameOriginal,
                    int _indexMeshSub,
                    FMeshVertexType _poTypeVertex,
                    bool isUpdateVertexBuffer,
                    bool isUpdateIndexBuffer);
            virtual ~MeshSub();

        public:
            Mesh* pMesh;
            String nameMeshSub;
            String nameOriginal;
            int indexMeshSub;
            bool isNeedUpdate_VertexBuffer;
            bool isNeedUpdate_IndexBuffer;

            //Vertex
            FMeshVertexType poTypeVertex;
            FMeshVertexPCVector vertices_Pos3Color4;
            std::vector<FVertex_Pos3Color4Tex2> vertices_Pos3Color4Tex2;
            std::vector<FVertex_Pos3Color4Normal3Tex2> vertices_Pos3Color4Normal3Tex2;
            std::vector<FVertex_Pos3Color4Normal3Tex4> vertices_Pos3Color4Normal3Tex4;
            std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2> vertices_Pos3Color4Normal3Tangent3Tex2;
            std::vector<FVertex_Pos3Color4Normal3Tangent3Tex4> vertices_Pos3Color4Normal3Tangent3Tex4;
            uint32_t poVertexCount;
            size_t poVertexBuffer_Size;
            void* poVertexBuffer_Data;
            VkBuffer poVertexBuffer;
            VkDeviceMemory poVertexBufferMemory;
            VkBuffer poVertexBuffer_Staging;
            VkDeviceMemory poVertexBufferMemory_Staging;

            //Index
            std::vector<uint32_t> indices;
            uint32_t poIndexCount;
            size_t poIndexBuffer_Size;
            void* poIndexBuffer_Data;
            VkBuffer poIndexBuffer;
            VkDeviceMemory poIndexBufferMemory;
            VkBuffer poIndexBuffer_Staging;
            VkDeviceMemory poIndexBufferMemory_Staging;

            //Instance
            uint32_t instanceCount;

        public:
            void Destroy();
            uint32_t GetVertexSize();
            uint32_t GetIndexSize();

            virtual bool CreateMeshSub(FMeshDataPC& meshDataPC, bool isTransformLocal, const FMatrix4& matTransformLocal);
            virtual bool CreateMeshSub(FMeshData& meshData, bool isTransformLocal, const FMatrix4& matTransformLocal);
            virtual void WriteVertexData(std::vector<FVertex_Pos3Color4Normal3Tex2>& aPos3Color4Normal3Tex2,
                                         std::vector<FVertex_Pos3Color4Normal3Tangent3Tex2>& aPos3Color4Normal3Tangent3Tex2);
            virtual void WriteIndexData(std::vector<uint32_t>& indexData);

            virtual void UpdateVertexBuffer();
            virtual void UpdateIndexBuffer();
        };
        typedef std::vector<MeshSub*> MeshSubPtrVector;
        typedef std::map<String, MeshSub*> MeshSubPtrMap;


        /////////////////////////// Mesh //////////////////////////////
        class utilExport Mesh
        {
        public:
            Mesh(VulkanWindow* _pWindow, 
                 const String& _nameMesh,
                 const String& _pathMesh,
                 FMeshType _typeMesh,
                 FMeshVertexType _typeVertex,
                 FMeshGeometryType _typeGeometryType,
                 FMeshCreateParam* _pMeshCreateParam);
            virtual ~Mesh();

        public:
            VulkanWindow* pWindow;
            String nameMesh;
            String pathMesh;
            FMeshType typeMesh;
            FMeshVertexType typeVertex;
            FMeshGeometryType typeGeometryType;
            FMeshCreateParam* pMeshCreateParam;
            MeshSubPtrVector aMeshSubs;
            MeshSubPtrMap mapMeshSubs;

        public:
            void Destroy();

            virtual bool AddMeshSub(MeshSub* pMeshSub);
            virtual bool LoadMesh(bool isFlipY, 
                                  bool isTransformLocal, 
                                  const FMatrix4& matTransformLocal,
                                  bool isUpdateVertexBuffer = false,
                                  bool isUpdateIndexBuffer = false);
        };
        typedef std::vector<Mesh*> MeshPtrVector;
        typedef std::map<String, Mesh*> MeshPtrMap;


        /////////////////////////// Texture ///////////////////////////
        class utilExport Texture
        {
        public:
             Texture(VulkanWindow* _pWindow, 
                     const String& _nameTexture,
                     VulkanTextureType _typeTexture,
                     bool _isRenderTarget,
                     bool _isGraphicsComputeShared,
                     VkFormat _typeFormat,
                     VulkanTextureFilterType _typeFilter,
                     VulkanTextureAddressingType _typeAddressing,
                     VulkanTextureBorderColorType _typeBorderColor,
                     const StringVector& _aPathTexture);
            virtual ~Texture();

        public:
            VulkanWindow* pWindow;
            String nameTexture;
            StringVector aPathTexture;
            VulkanTextureType typeTexture;
            bool isRenderTarget;
            bool isGraphicsComputeShared;
            VkFormat typeFormat; 
            VulkanTextureFilterType typeFilter;
            VulkanTextureAddressingType typeAddressing;
            VulkanTextureBorderColorType typeBorderColor;
            int refCount;
            int width;
            int height;
            int depth;

            uint32_t poMipMapCount;
            VkImage poTextureImage;
            VkDeviceMemory poTextureImageMemory;
            VkImageView poTextureImageView;
            VkSampler poTextureSampler;
            VkDescriptorImageInfo poTextureImageInfo;
            VkImageLayout poTextureImageLayout;

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingBufferMemory;

            //Texture 3D
            uint8* pDataRGBA;

            //Texture Animation
            int texChunkMaxX;
            int texChunkMaxY;
            int texChunkIndex;
            int frameCurrent;

            //Texture RenderTarget
            FVector4 rtColorDefault;
            bool rtIsSetColor; 
            VkImageUsageFlags rtImageUsage;

        public:
            int GetRef() { return this->refCount; }
            int AddRef() { return ++ this->refCount; }
            int DelRef() { return -- this->refCount; }
            bool HasRef() { return this->refCount > 0; }
            bool CanDel() { return !HasRef(); }

            void Destroy();
            virtual int RandomTextureIndex();
            virtual void LoadTexture(int width,
                                     int height,
                                     int depth);
            virtual void UpdateTexture();

        protected:
            virtual void updateNoiseTextureData();
            virtual void updateNoiseTexture();
        };
        typedef std::vector<Texture*> TexturePtrVector;
        typedef std::map<String, Texture*> TexturePtrMap;
        typedef std::map<String, TexturePtrVector> TexturePtrShaderSortMap;


        /////////////////////////// Shader ////////////////////////////
        class utilExport Shader
        {
        public:
            Shader(VulkanWindow* _pWindow,
                   const String& _nameShader);
            virtual ~Shader();

        public:
            VulkanWindow* pWindow;
            String nameShader;

        public:


        };
        typedef std::vector<Shader*> ShaderPtrVector;
        typedef std::map<String, Shader*> ShaderPtrMap;


        /////////////////////////// Material //////////////////////////
        class utilExport Material
        {
        public:
            Material(VulkanWindow* _pWindow,
                     const String& _nameMaterial);
            virtual ~Material();

        public:
            VulkanWindow* pWindow;
            String nameMaterial;

        public:

        };
        typedef std::vector<Material*> MaterialPtrVector;
        typedef std::map<String, Material*> MaterialPtrMap;


        /////////////////////////// MultiRenderPass ///////////////////
        class utilExport FrameBufferAttachment
        {
        public:
            FrameBufferAttachment();
            virtual ~FrameBufferAttachment();

        public:
            bool isDepth;
            bool isImageArray;

            VkImage image;
            VkDeviceMemory memory;
            VkImageView view;

        public:
            void Destroy(VulkanWindow* pWindow);
            virtual void Init(VulkanWindow* pWindow, 
                              uint32_t width, 
                              uint32_t height, 
                              bool _isDepth,
                              bool _isImageArray);
        };
        class utilExport MultiRenderPass
        {
        public:
            MultiRenderPass(VulkanWindow* _pWindow, 
                            const String& _nameRenderPass,
                            bool _isUseDefault,
                            bool _isMultiView2);
            virtual ~MultiRenderPass();

        public:
            //Window
            VulkanWindow* pWindow;
            String nameRenderPass;
            bool isUseDefault;
            bool isMultiView2;

            //Attachment
            FrameBufferAttachment framebufferColor;
            FrameBufferAttachment framebufferDepth;
            VkSampler sampler;
            VkDescriptorImageInfo imageInfo;
            
            //RenderPass
            VkRenderPass poRenderPass;

            //FrameBuffer
            VkFramebuffer poFrameBuffer;

        public:
            void Destroy();

            virtual void Init(uint32_t width, uint32_t height);
        
        public:
            void CleanupSwapChain();
            void RecreateSwapChain();
        };
        typedef std::vector<MultiRenderPass*> MultiRenderPassPtrVector;
        typedef std::map<String, MultiRenderPass*> MultiRenderPassPtrMap;


        /////////////////////////// PipelineGraphics //////////////////
        class utilExport PipelineGraphics
        {
        public:
            PipelineGraphics(VulkanWindow* _pWindow);
            virtual ~PipelineGraphics();

        public:
            VulkanWindow* pWindow;
            String nameDescriptorSetLayout;
            StringVector* poDescriptorSetLayoutNames;
            VkDescriptorSetLayout poDescriptorSetLayout;
            VkPipelineLayout poPipelineLayout;
            VkPipeline poPipeline_WireFrame;
            VkPipeline poPipeline;
            VkDescriptorSetVector poDescriptorSets;

            bool isMultiView;
            VkPipeline poPipeline_WireFrame2;
            VkPipeline poPipeline2;

            String nameRenderPass;
            MultiRenderPass* pRenderPass;

        public:
            void Destroy();

            virtual void CleanupSwapChain();
        };  
        typedef std::vector<PipelineGraphics*> PipelineGraphicsPtrVector;
        typedef std::map<String, PipelineGraphics*> PipelineGraphicsPtrMap;


        /////////////////////////// PipelineCompute ///////////////////
        class utilExport PipelineCompute
        {
        public:
            PipelineCompute(VulkanWindow* _pWindow);
            virtual ~PipelineCompute();

        public:
            VulkanWindow* pWindow;
            String nameDescriptorSetLayout;
            StringVector* poDescriptorSetLayoutNames;
            VkDescriptorSetLayout poDescriptorSetLayout;
            VkPipelineLayout poPipelineLayout;
            VkPipeline poPipeline;
            VkDescriptorSet poDescriptorSet;
            
            Texture* pTextureSource;
            Texture* pTextureTarget;
            TextureCopyConstants* pTextureCopy;
            VkBuffer poBuffer_TextureCopy;  
            VkDeviceMemory poBufferMemory_TextureCopy;
            int frameRand;

        public:
            void Destroy();

            virtual void CleanupSwapChain();

        public:
            void CreateTextureCopy();
            void DestroyTextureCopy();
        };
        typedef std::vector<PipelineCompute*> PipelineComputePtrVector;
        typedef std::map<String, PipelineCompute*> PipelineComputePtrMap;


        /////////////////////////// Renderable ////////////////////////
        class utilExport Renderable
        {
        public:
            Renderable(const String& _nameRenderable);
            virtual ~Renderable();

        public:
            String nameRenderable;

        public:

        };
        typedef std::vector<Renderable*> RenderablePtrVector;
        typedef std::map<String, Renderable*> RenderablePtrMap;


        /////////////////////////// RenderableIndirect ////////////////
        class utilExport RenderableIndirect : public Renderable
        {
        public:
            RenderableIndirect(const String& _nameRenderable);
            virtual ~RenderableIndirect();

        public:

        public:

        };
        typedef std::vector<RenderableIndirect*> RenderableIndirectPtrVector;
        typedef std::map<String, RenderableIndirect*> RenderableIndirectPtrMap;


        /////////////////////////// Movable ///////////////////////////
        class utilExport Movable
        {
        public:
            Movable(const String& _nameMovable);
            virtual ~Movable();

        public:
            String nameMovable;

        public:

        };
        typedef std::vector<Movable*> MovablePtrVector;
        typedef std::map<String, Movable*> MovablePtrMap;


        /////////////////////////// Node //////////////////////////////
        class utilExport Node
        {
        public:
            Node(const String& _nameNode);
            virtual ~Node();

        public:
            String nameNode;

        public:

        };
        typedef std::vector<Node*> NodePtrVector;
        typedef std::map<String, Node*> NodePtrMap;


        /////////////////////////// SceneNode /////////////////////////
        class utilExport SceneNode : public Node
        {
        public:
            SceneNode(const String& _nameNode);
            virtual ~SceneNode();

        public:


        public:

        };
        typedef std::vector<SceneNode*> SceneNodePtrVector;
        typedef std::map<String, SceneNode*> SceneNodePtrMap;


        /////////////////////////// Object ////////////////////////////
        class utilExport Object
        {
        public:
            Object(const String& _nameObject);
            virtual ~Object();

        public:
             String nameObject;

        public:

        };
        typedef std::vector<Object*> ObjectPtrVector;
        typedef std::map<String, Object*> ObjectPtrMap;


        /////////////////////////// ObjectTerrain /////////////////////
        class utilExport ObjectTerrain
        {
        public:
            ObjectTerrain(const String& _nameTerrain);
            virtual ~ObjectTerrain();

        public:
             String nameTerrain;

        public:

        };
        typedef std::vector<ObjectTerrain*> ObjectTerrainPtrVector;
        typedef std::map<String, ObjectTerrain*> ObjectTerrainPtrMap;


        /////////////////////////// Scene /////////////////////////////
        class utilExport Scene
        {
        public:
            Scene(const String& _nameScene);
            virtual ~Scene();

        public:
             String nameScene;

        public:

        };


        /////////////////////////// SceneManager //////////////////////
        class utilExport SceneManager
        {
        public:
            SceneManager(const String& _nameSceneManager);
            virtual ~SceneManager();

        public:
             String nameSceneManager;

        public:

        };


        /////////////////////////// EditorBase ////////////////////////
        class utilExport EditorBase
        {
        public:
            EditorBase(VulkanWindow* _pWindow);
            virtual ~EditorBase();
        
        public:
            VulkanWindow* pWindow;

            //Meshes
            MeshInfoPtrVector aMeshInfos;
            MeshPtrVector aMeshes;
            MeshPtrMap mapMeshes;

            //Shaders
            ShaderModuleInfoVector aShaderModuleInfos;
            VkShaderModuleVector aShaderModules;
            VkShaderModuleMap mapShaderModules;

            //DescriptorSetLayouts
            String nameDescriptorSetLayout; 
            StringVector aNameDescriptorSetLayouts;
            VkDescriptorSetLayout poDescriptorSetLayout;

            //PipelineLayout
            VkPipelineLayout poPipelineLayout;

            //PipelineGraphics
            PipelineGraphics* pPipelineGraphics;

        public:
            virtual void Destroy() = 0;

            virtual void Init();

            virtual void UpdateCBs() = 0;
            virtual void Draw(VkCommandBuffer& commandBuffer);

        public:
            virtual void CleanupSwapChain();
            virtual void RecreateSwapChain();

        protected:
            virtual void initConfigs() = 0;
            virtual void initMeshes();
            virtual void initShaders();
            virtual void initBufferUniforms() = 0;
            virtual void initDescriptorSetLayout();
            virtual void initPipelineLayout();
            virtual void initPipelineGraphics() = 0;
            virtual void updateDescriptorSets_Graphics() = 0;

            virtual void destroyMeshes();
            virtual void destroyShaders();
            virtual void destroyBufferUniforms() = 0;
            virtual void destroyPipelineGraphics();
            virtual void destroyPipelineLayout();
            virtual void destroyDescriptorSetLayout();
        };


        /////////////////////////// EditorGrid ////////////////////////
        class utilExport EditorGrid : public EditorBase
        {
        public:
            EditorGrid(VulkanWindow* _pWindow);
            virtual ~EditorGrid();

        public:
            static const String s_strNameShader_Grid_Vert;
            static const String s_strNameShader_Grid_Frag;

        public:
            GridObjectConstants gridObjectCB;
            VkBuffer poBuffers_ObjectCB;
            VkDeviceMemory poBuffersMemory_ObjectCB;

            void SetColor(FColor& color)
            {
                this->gridObjectCB.color = color;
                SetIsNeedUpdate(true);
            }

        protected:
            bool isNeedUpdate;
        public:
            LP_FORCEINLINE bool IsNeedUpdate() const { return this->isNeedUpdate; }
            LP_FORCEINLINE void SetIsNeedUpdate(bool b) { this->isNeedUpdate = b; }

        public:
            virtual void Destroy();

            virtual void Init();

            virtual void UpdateCBs();

        public:
            virtual void CleanupSwapChain();
            virtual void RecreateSwapChain();

        protected:
            virtual void initConfigs();
            virtual void initBufferUniforms();
            virtual void initPipelineGraphics();
            virtual void updateDescriptorSets_Graphics();

            virtual void destroyBufferUniforms();
            virtual void destroyPipelineGraphics();
        };

        /////////////////////////// EditorCameraAxis //////////////////
        class utilExport EditorCameraAxis : public EditorBase
        {
        public:
            EditorCameraAxis(VulkanWindow* _pWindow);
            virtual ~EditorCameraAxis();

        public:
            static size_t s_nMeshConeIndex;
            static size_t s_nMeshAABBIndex;
            static size_t s_nMeshQuadIndex;
            static size_t s_nMeshCameraAxisCount;

            static const String s_strNameShader_CameraAxis_Vert;
            static const String s_strNameShader_CameraAxis_Frag;
            static const String s_strNameShader_QuadBlit_Vert;
            static const String s_strNameShader_QuadBlit_Frag;

            static const float s_fBlitAreaWidth;
            static const float s_fBlitAreaHeight;

            static FMatrix4 s_aMatrix4Transforms[7];

            static float s_fCameraDistance;
            static FVector3 s_vCameraPos;
            static FVector3 s_vCameraLookTarget;
            static FVector3 s_vCameraUp;
            static float s_fCameraFOV;
            static float s_fCameraAspectRatio;
            static float s_fCameraZNear;
            static float s_fCameraZFar;

        //CameraAxis
        public:
            FCamera* pCamera;
            VkViewport poViewport;
            VkRect2D poScissor;
            VkOffset2D poOffset;
            VkExtent2D poExtent;
            FColor poColorBackground;

            PassConstants passCB;
            VkBuffer poBuffers_PassCB;
            VkDeviceMemory poBuffersMemory_PassCB;

            std::vector<CameraAxisObjectConstants> cameraAxisObjectCBs;
            VkBuffer poBuffers_ObjectCB;
            VkDeviceMemory poBuffersMemory_ObjectCB;

        //Quad Blit
        public:
            //DescriptorSetLayouts
            String nameDescriptorSetLayout_CopyBlit; 
            StringVector aNameDescriptorSetLayouts_CopyBlit;
            VkDescriptorSetLayout poDescriptorSetLayout_CopyBlit;

            //PipelineLayout
            VkPipelineLayout poPipelineLayout_CopyBlit;

            //PipelineGraphics
            PipelineGraphics* pPipelineGraphics_CopyBlit;

            //Uniform Buffer
            CopyBlitObjectConstants copyBlitObjectCB;
            VkBuffer poBuffers_CopyBlitObjectCB;
            VkDeviceMemory poBuffersMemory_CopyBlitObjectCB;

        protected:
            bool isNeedUpdate;
        public:
            LP_FORCEINLINE bool IsNeedUpdate() const { return this->isNeedUpdate; }
            LP_FORCEINLINE void SetIsNeedUpdate(bool b) { this->isNeedUpdate = b; }

        public:
            virtual void Destroy();

            virtual void Init();

            virtual void UpdateCBs();
            virtual void Draw(VkCommandBuffer& commandBuffer);
            virtual void DrawQuad(VkCommandBuffer& commandBuffer);

        public:
            virtual void CleanupSwapChain();
            virtual void RecreateSwapChain();

        protected:
            virtual void initConfigs();
                virtual void initCamera();
                virtual void initViewport();
            virtual void initBufferUniforms();
            virtual void initDescriptorSetLayout();
            virtual void initPipelineLayout();
            virtual void initPipelineGraphics();
            virtual void updateDescriptorSets_Graphics();

            virtual void destroyBufferUniforms();
            virtual void destroyPipelineGraphics();
            virtual void destroyPipelineLayout();
            virtual void destroyDescriptorSetLayout();
        };

        /////////////////////////// EditorCoordinateAxis //////////////
        class utilExport EditorCoordinateAxis : public EditorBase
        {
        public:
            EditorCoordinateAxis(VulkanWindow* _pWindow);
            virtual ~EditorCoordinateAxis();

        public:
            static size_t s_nMeshQuadIndex;
            static size_t s_nMeshQuadLineIndex;
            static size_t s_nMeshCylinderIndex;
            static size_t s_nMeshConeIndex;
            static size_t s_nMeshTorusIndex;
            static size_t s_nMeshAABBIndex;
            static size_t s_nMeshCoordinateAxisCount;

            static const String s_strNameShader_CoordinateAxis_Vert;
            static const String s_strNameShader_CoordinateAxis_Frag;
            static const String s_strNameShader_CoordinateAxisLine_Vert;
            static const String s_strNameShader_CoordinateAxisLine_Frag;
            
            static float s_fScale_Quad;
            static float s_fScale_Cylinder;
            static float s_fScale_Cone;
            static float s_fScale_Torus;
            static float s_fScale_AABB;
            static float s_fScale_AABBCenter;

            static FMatrix4 s_aMatrix4Transforms[19];
            static FColor s_aColors_Default[19];
            static FColor s_aColors_Hover[19];
            static FColor s_aColors_Select[19];
            static FColor s_aColors_UnSelect[19];

            static const float s_fScale_Distance;
            static const float s_fScale_WhenSelect_Axis;
            static const float s_fScale_WhenSelect_AxisCone;
            static const float s_fScale_WhenSelect_AxisTorus;
            static const float s_fScale_WhenSelect_AxisAABB;

        public:
            enum CoordinateStateType
            {
                CoordinateState_None = 0,
                CoordinateState_Select,
                CoordinateState_Move,
                CoordinateState_Rotate,
                CoordinateState_Scale,
            };
            enum CoordinateElementType
            {
                CoordinateElement_None = -1,

                CoordinateElement_Axis_X = 0,   //X Axis or X Scale
                CoordinateElement_Axis_Y,       //Y Axis or Y Scale
                CoordinateElement_Axis_Z,       //Y Axis or Z Scale
                CoordinateElement_Axis_XYZ,     //XYZ Scale

                CoordinateElement_Quad_XY,
                CoordinateElement_Quad_YZ,
                CoordinateElement_Quad_ZX,
            };

        public:
            FCamera* pCamera;
            FVector4 vViewport;

        public:
            std::vector<CoordinateAxisObjectConstants> coordinateAxisObjectCBs;
            VkBuffer poBuffers_ObjectCB;
            VkDeviceMemory poBuffersMemory_ObjectCB;

        protected:
            float scaleCoordinate;
            FVector3 vPos;
            FMatrix4 mat4Trans;

            CoordinateStateType typeState; 
            CoordinateElementType typeElementSelect;
            bool isButtonLeftDown;
            double ptLastX;
            double ptLastY;
            
            FVector3 aAxisX[2];
            FVector3 aAxisY[2];
            FVector3 aAxisZ[2];
            FAABB aQuadAABB[3];
            FVector3 aTorusX[30];
            FVector3 aTorusY[30];
            FVector3 aTorusZ[30];
            FAABB aScaleAABB[4];
            
        public:
            LP_FORCEINLINE float GetScaleCoordinate() const { return this->scaleCoordinate; }
            LP_FORCEINLINE void SetScaleCoordinate(float f) { this->scaleCoordinate = f; }
            LP_FORCEINLINE const FVector3& GetPos() const { return this->vPos; }
            void SetPos(const FVector3& vP);

            LP_FORCEINLINE bool IsButtonLeftDown() const { return this->isButtonLeftDown; }

            LP_FORCEINLINE bool IsStateNone() const { return this->typeState == CoordinateState_None; }
            LP_FORCEINLINE bool IsStateSelect() const  { return this->typeState == CoordinateState_Select; }
            LP_FORCEINLINE bool IsStateMove() const { return this->typeState == CoordinateState_Move; }
            LP_FORCEINLINE bool IsStateRotate() const { return this->typeState == CoordinateState_Rotate; }
            LP_FORCEINLINE bool IsStateScale() const { return this->typeState == CoordinateState_Scale; }
            LP_FORCEINLINE void SetStateType(CoordinateStateType type) { this->typeState = type; }

            bool IsAxisSelected() const;
            bool IsAxisSelectedByIndex(int index) const; //0:X; 1:Y; 2:Z
            bool IsAxisSelectedByType(CoordinateElementType type) const;
            bool IsAxisXSelected() const;
            bool IsAxisYSelected() const;
            bool IsAxisZSelected() const;
            CoordinateElementType GetAxisSelected()  const;
            void SetAxisSelected(CoordinateElementType type);

            bool IsQuadSelected() const;
            bool IsQuadSelectedByIndex(int index) const; //0:XY; 1:YZ; 2:ZX
            bool IsQuadSelectedByType(CoordinateElementType type) const;
            bool IsQuadXYSelected() const;
            bool IsQuadYZSelected() const;
            bool IsQuadZXSelected() const;
            CoordinateElementType GetQuadSelected() const;
            void SetQuadSelected(CoordinateElementType type);
            void ClearSelectState();

            bool IsScaleAABBSelected() const;
            bool IsScaleAABBSelectedByIndex(int index) const; //0:X; 1:Y; 2: Z; 3:XYZ
            bool IsScaleAABBSelectedByType(CoordinateElementType type) const;
            bool IsScaleAABBXSelected() const;
            bool IsScaleAABBYSelected() const;
            bool IsScaleAABBZSelected() const;
            bool IsScaleAABBXYZSelected() const;
            CoordinateElementType GetScaleAABBSelected() const;
            void SetScaleAABBSelected(CoordinateElementType type);

        public:
            virtual void Destroy();

            virtual void Init();

            virtual void UpdateCBs();
                virtual void UpdateCBs_Move();
                virtual void UpdateCBs_Rotate();
                virtual void UpdateCBs_Scale();

            virtual void Draw(VkCommandBuffer& commandBuffer);
                virtual void Draw_Move(VkCommandBuffer& commandBuffer);
                virtual void Draw_Rotate(VkCommandBuffer& commandBuffer);
                virtual void Draw_Scale(VkCommandBuffer& commandBuffer);

                virtual void DrawQuad(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);
                virtual void DrawQuadLine(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);
                virtual void DrawShape(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);


            virtual void CheckStateMove(double x, double y);
            virtual void CheckStateRotate(double x, double y);
            virtual void CheckStateScale(double x, double y);

            virtual void Move(double deltaX, double deltaY);
            virtual void Rotate(double deltaX, double deltaY);
            virtual void Scale(double deltaX, double deltaY);

            virtual void MouseLeftDown(double x, double y);
            virtual void MouseMove(double x, double y);
            virtual void MouseLeftUp(double x, double y);
            virtual void MouseHover(double x, double y);

        public:
            virtual void CleanupSwapChain();
            virtual void RecreateSwapChain();

        protected:
            virtual void initConfigs();
            virtual void initBufferUniforms();
            virtual void initPipelineGraphics();
            virtual void updateDescriptorSets_Graphics();

            virtual void destroyBufferUniforms();
            virtual void destroyPipelineGraphics();
        };

        /////////////////////////// EditorLineFlatCollector ///////////
        class utilExport EditorLineFlatCollector : public EditorBase
        {
        public:
            EditorLineFlatCollector(VulkanWindow* _pWindow);
            virtual ~EditorLineFlatCollector();

        public:
            

        };

    public:
        //Mesh
        virtual Mesh* CreateMesh(const MeshInfo* pMI);
        virtual void CreateMeshes(const MeshInfoPtrVector& aMIs, MeshPtrVector& aMeshes, MeshPtrMap& mapMeshes);

        //ShaderModule
        virtual VkShaderModule CreateShaderModule(const ShaderModuleInfo& si);
        virtual void CreateShaderModules(const ShaderModuleInfoVector& aSIs, VkShaderModuleVector& aShaderModules, VkShaderModuleMap& mapShaderModules);

        //PipelineShaderStageCreateInfos
        virtual bool CreatePipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                          const String& nameShaderTesc,
                                                          const String& nameShaderTese,
                                                          const String& nameShaderGeom,
                                                          const String& nameShaderFrag,
                                                          const String& nameShaderComp,
                                                          VkShaderModuleMap& mapVkShaderModules,
                                                          VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics,
                                                          VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                          VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);
        virtual bool CreatePipelineShaderStageCreateInfos(const String& nameShaderVert,
                                                          const String& nameShaderTesc,
                                                          const String& nameShaderTese,
                                                          const String& nameShaderGeom,
                                                          const String& nameShaderFrag,
                                                          VkShaderModuleMap& mapVkShaderModules,
                                                          VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Graphics);
        virtual bool CreatePipelineShaderStageCreateInfos(const String& nameShaderComp,
                                                          VkShaderModuleMap& mapVkShaderModules,
                                                          VkPipelineShaderStageCreateInfoVector& aStageCreateInfos_Compute,
                                                          VkPipelineShaderStageCreateInfoMap& mapStageCreateInfos_Compute);

        //Texture

        
        //DescriptorSetLayout
        virtual VkDescriptorSetLayout CreateDescriptorSetLayout(const String& nameLayout, const StringVector* pNamesDescriptorSetLayout);


    public: 
        static bool s_isEnableValidationLayers;
        static int s_maxFramesInFight;

    public:
        //Pipeline Objects
        VkInstance poInstance;
        VkDebugUtilsMessengerEXT poDebugMessenger;
        VkPhysicalDevice poPhysicalDevice;
        VkPhysicalDeviceProperties poPhysicalDeviceProperties;
        VkPhysicalDeviceFeatures poPhysicalDeviceFeatures;
        VkPhysicalDeviceFeatures2 poPhysicalDeviceFeatures2;
        VkPhysicalDeviceFeatures poPhysicalEnabledFeatures;
        void* poDeviceCreatepNextChain;
        VkDevice poDevice;
        VkSampleCountFlagBits poMSAASamples;
        VkQueue poQueueGraphics;
        VkQueue poQueuePresent;
        VkQueue poQueueCompute;

        VkPhysicalDeviceFeatures2KHR poPhysicalDeviceFeatures2KHR;
        VkPhysicalDeviceMultiviewFeaturesKHR poPhysicalDeviceMultiViewFeaturesKHR;
        VkPhysicalDeviceProperties2KHR poPhysicalDeviceProperties2KHR;
        VkPhysicalDeviceMultiviewPropertiesKHR poPhysicalDeviceMultiViewPropertiesKHR;
        VkSurfaceKHR poSurface;
        VkSwapchainKHR poSwapChain;
        VkViewport poViewport;
        VkOffset2D poOffset;
        VkExtent2D poExtent;
        VkRect2D poScissor;
        std::vector<VkImage> poSwapChainImages;
        VkFormat poSwapChainImageFormat;
        VkExtent2D poSwapChainExtent;
        FVector2 poWindowContentScale;
        VkImageViewVector poSwapChainImageViews;
        VkFramebufferVector poSwapChainFrameBuffers;
        VkFormat poDepthImageFormat;
        VkImage poColorImage;
        VkDeviceMemory poColorImageMemory;
        VkImageView poColorImageView;
        VkImage poDepthImage;
        VkDeviceMemory poDepthImageMemory;
        VkImageView poDepthImageView;

        VkRenderPass poRenderPass;
        VkDescriptorSetLayout poDescriptorSetLayout;
        
        VkCommandPool poCommandPoolGraphics;
        std::vector<VkCommandBuffer> poCommandBuffersGraphics;

        VkCommandPool poCommandPoolCompute;
        VkCommandBuffer poCommandBufferCompute;
        
        uint32_t poVertexCount;
        size_t poVertexBuffer_Size;
        void* poVertexBuffer_Data;
        VkBuffer poVertexBuffer;
        VkDeviceMemory poVertexBufferMemory;
        uint32_t poIndexCount;
        size_t poIndexBuffer_Size;
        void* poIndexBuffer_Data;
        VkBuffer poIndexBuffer;
        VkDeviceMemory poIndexBufferMemory;
        FMatrix4 poMatWorld;

        FMeshVertexType poTypeVertex;
        VkPipelineLayout poPipelineLayout;
        VkPipelineCache poPipelineCache;
        VkPipeline poPipelineGraphics;
        VkPipeline poPipelineGraphics_WireFrame;

        uint32_t poMipMapCount;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        VkDescriptorPool poDescriptorPool;
        VkDescriptorSetVector poDescriptorSets;


        //Synchronization Objects
        VkSemaphoreVector poPresentCompleteSemaphores;
        VkSemaphoreVector poRenderCompleteSemaphores;
        VkFenceVector poInFlightFences;
        VkFenceVector poImagesInFlight;
        size_t poCurrentFrame;
        uint32_t poSwapChainImageIndex;

        VkSemaphore poGraphicsWaitSemaphore;
        VkSemaphore poComputeWaitSemaphore;


        //Features 
        struct SwapChainSupportDetails 
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        SwapChainSupportDetails swapChainSupport;

        uint32_t queueIndexGraphics;
        uint32_t queueIndexPresent;
        uint32_t queueIndexCompute;
        
        bool isFrameBufferResized;
        //Config
        FVector4 cfg_colorBackground;
        bool cfg_isMSAA;
        bool cfg_isImgui;
        bool cfg_isWireFrame;
        bool cfg_isRotate;
        bool cfg_isNegativeViewport;
        bool cfg_isUseComputeShader;
        bool cfg_isCreateRenderComputeSycSemaphore;
        VkPrimitiveTopology cfg_vkPrimitiveTopology;
        VkFrontFace cfg_vkFrontFace;
        VkPolygonMode cfg_vkPolygonMode;
        VkCullModeFlagBits cfg_vkCullModeFlagBits;
        float cfg_LineWidth;
        VkBool32 cfg_isDepthTest;
        VkBool32 cfg_isDepthWrite; 
        VkCompareOp cfg_DepthCompareOp; 
        VkBool32 cfg_isStencilTest;
        VkStencilOpState cfg_StencilOpFront; 
        VkStencilOpState cfg_StencilOpBack; 
        VkBool32 cfg_isBlend;
        VkBlendFactor cfg_BlendColorFactorSrc; 
        VkBlendFactor cfg_BlendColorFactorDst; 
        VkBlendOp cfg_BlendColorOp;
        VkBlendFactor cfg_BlendAlphaFactorSrc;
        VkBlendFactor cfg_BlendAlphaFactorDst; 
        VkBlendOp cfg_BlendAlphaOp;
        VkColorComponentFlags cfg_ColorWriteMask;

        FVector3 cfg_cameraPos;
        FVector3 cfg_cameraLookTarget;
        FVector3 cfg_cameraUp;

        float cfg_cameraFov;
        float cfg_cameraNear;
        float cfg_cameraFar;

        float cfg_cameraSpeedMove;
        float cfg_cameraSpeedZoom;
        float cfg_cameraSpeedRotate;

        //Custom
        String cfg_model_Path;
        String cfg_shaderVertex_Path;
        String cfg_shaderFragment_Path;
        String cfg_texture_Path;
        String cfg_terrain_Path;
        String cfg_terrainShaderNormalMapGen_Path;
        String cfg_terrainShaderVertex_Path;
        String cfg_terrainShaderFragment_Path;
        String cfg_terrainTextureDiffuse_Path;
        String cfg_terrainTextureNormal_Path;
        String cfg_terrainTextureControl_Path;

        //Imgui
        bool imgui_IsEnable;
        int	imgui_MinimalSwapchainImages;
	    VkDescriptorPool imgui_DescriptorPool;
        String imgui_PathIni;
        String imgui_PathLog;

        //Constants Buffer
        PassConstants passCB;
        std::vector<VkBuffer> poBuffers_PassCB;
        std::vector<VkDeviceMemory> poBuffersMemory_PassCB;

        std::vector<ObjectConstants> objectCBs;
        std::vector<VkBuffer> poBuffers_ObjectCB;
        std::vector<VkDeviceMemory> poBuffersMemory_ObjectCB;

        std::vector<MaterialConstants> materialCBs;
        std::vector<VkBuffer> poBuffers_MaterialCB;
        std::vector<VkDeviceMemory> poBuffersMemory_MaterialCB;

        std::vector<InstanceConstants> instanceCBs;
        std::vector<VkBuffer> poBuffers_InstanceCB;
        std::vector<VkDeviceMemory> poBuffersMemory_InstanceCB;

        //Terrain
        uint8* poTerrainHeightMapData;
        float* poTerrainHeightMapDataFloat;
        int32 poTerrainHeightMapDataSize;
        int32 poTerrainHeightMapSize;
        int32 poTerrainGridVertexCount;
        int32 poTerrainGridInstanceVertexCount;
        int32 poTerrainGridInstanceCount;
        bool poTerrainIsDrawInstance;

        std::vector<FVertex_Pos3Normal3Tex2> poTerrain_Pos3Normal3Tex2;
        uint32_t poTerrainVertexCount;
        size_t poTerrainVertexBuffer_Size;
        void* poTerrainVertexBuffer_Data;
        VkBuffer poTerrainVertexBuffer;
        VkDeviceMemory poTerrainVertexBufferMemory;
        std::vector<uint32_t> poTerrain_Indices;
        uint32_t poTerrainIndexCount;
        size_t poTerrainIndexBuffer_Size;
        void* poTerrainIndexBuffer_Data;
        VkBuffer poTerrainIndexBuffer;
        VkDeviceMemory poTerrainIndexBufferMemory;

        VkImage poTerrainHeightMapImage;
        VkDeviceMemory poTerrainHeightMapImageMemory;
        VkImageView poTerrainHeightMapImageView;
        VkDescriptorImageInfo poTerrainHeightMapImageInfo;
        VkImage poTerrainNormalMapImage;
        VkDeviceMemory poTerrainNormalMapImageMemory;
        VkImageView poTerrainNormalMapImageView;
        VkDescriptorImageInfo poTerrainNormalMapImageInfo;
        VkSampler poTerrainImageSampler;

        VkShaderModule poTerrainComputeShaderModuleNormalGen;

        TextureCopyConstants poTerrainTextureCopy;
        VkBuffer poBuffer_TerrainTextureCopy;
        VkDeviceMemory poBufferMemory_TerrainTextureCopy;

        VkDescriptorSetLayout poTerrainComputeDescriptorSetLayout;
        VkPipelineLayout poTerrainComputePipelineLayout;
        VkPipeline poTerrainComputePipeline;
        VkDescriptorSet poTerrainComputeDescriptorSet;
        
        VkImage poTerrainDiffuseImage;
        VkDeviceMemory poTerrainDiffuseImageMemory;
        VkImageView poTerrainDiffuseImageView;
        VkSampler poTerrainDiffuseImageSampler;
        VkDescriptorImageInfo poTerrainDiffuseImageInfo;
        VkImage poTerrainNormalImage;
        VkDeviceMemory poTerrainNormalImageMemory;
        VkImageView poTerrainNormalImageView;
        VkSampler poTerrainNormalImageSampler;
        VkDescriptorImageInfo poTerrainNormalImageInfo;
        VkImage poTerrainControlImage;
        VkDeviceMemory poTerrainControlImageMemory;
        VkImageView poTerrainControlImageView;
        VkSampler poTerrainControlImageSampler;
        VkDescriptorImageInfo poTerrainControlImageInfo;

        
        std::vector<TerrainObjectConstants> terrainObjectCBs;
        VkBuffer poBuffer_TerrainObjectCB;
        VkDeviceMemory poBufferMemory_TerrainObjectCB;

        VkDescriptorSetLayout poTerrainGraphicsDescriptorSetLayout;
        VkPipelineLayout poTerrainGraphicsPipelineLayout;
        VkShaderModule poTerrainGraphicsShaderModuleVertex;
        VkShaderModule poTerrainGraphicsShaderModuleFragment;
        VkPipeline poTerrainGraphicsPipeline;
        VkPipeline poTerrainGraphicsPipeline_WireFrame;
        VkDescriptorSetVector poTerrainGraphicsDescriptorSets; 
        
        //Camera
        FCamera* pCamera; //Eye Left
        FCamera* pCameraRight; //Eye Right

        //Light
        LightConstants mainLight; //common.x == Vulkan_Light_Directional, can not change
        LightConstants aAdditionalLights[MAX_LIGHT_COUNT];

        //Mouse
        FVector2 mousePosLast;
        bool mouseButtonDownLeft;
        bool mouseButtonDownRight;
        bool mouseButtonDownMiddle;

        //Terrain


        //Editor
        bool cfg_isEditorCreate;
        bool cfg_isEditorGridShow;
        bool cfg_isEditorCameraAxisShow;
        bool cfg_isEditorCoordinateAxisShow;
        FColor cfg_editorGrid_Color;
        float cfg_editorCoordinateAxis_MoveSpeed;
        float cfg_editorCoordinateAxis_RotateSpeed;
        float cfg_editorCoordinateAxis_ScaleSpeed;
        EditorGrid* pEditorGrid;
        EditorCameraAxis* pEditorCameraAxis;
        EditorCoordinateAxis* pEditorCoordinateAxis;

    protected:
        ConstCharPtrVector aInstanceLayers;
        ConstCharPtrVector aInstanceExtensions;
        ConstCharPtrVector aDeviceLayers;
        ConstCharPtrVector aDeviceExtensions;

    public:
        // Common
        virtual void OnInit();
        virtual void OnLoad();
        virtual bool OnIsInit();
        virtual void OnResize(int w, int h, bool force);
        virtual bool OnBeginCompute();
            virtual void OnUpdateCompute();
            virtual void OnCompute();
        virtual void OnEndCompute();
        virtual bool OnBeginRender();
            virtual void OnUpdateRender();
            virtual void OnRender();
        virtual void OnEndRender();
        virtual void OnDestroy();

        // Mouse Input
        virtual void OnMouseInput();
        virtual void OnMouseLeftDown(double x, double y);
        virtual void OnMouseLeftUp(double x, double y);
        virtual void OnMouseRightDown(double x, double y);
        virtual void OnMouseRightUp(double x, double y);
        virtual void OnMouseMiddleDown(double x, double y);
        virtual void OnMouseMiddleUp(double x, double y);
        virtual void OnMouseMove(int button, double x, double y);
        virtual void OnMouseHover(double x, double y);
        virtual void OnMouseWheel(double x, double y);

        // Keyboard Input
        virtual void OnKeyboardInput();
        virtual void OnKeyDown(int key);
        virtual void OnKeyUp(int key);

        // Camera Process
        virtual void OnCameraMouseMoveProcess(double newX, double newY, double oldX, double oldY);
        virtual void OnCameraMouseZoomProcess(double zoom);
        virtual void OnCameraMouseKeyboardProcess();

        // Editor Coordinate Process
        virtual void OnEditorCoordinateMouseLeftDown(double x, double y);
        virtual void OnEditorCoordinateMouseMove(double x, double y);
        virtual void OnEditorCoordinateMouseLeftUp(double x, double y);
        virtual void OnEditorCoordinateMouseHover(double x, double y);


    public:
        virtual bool HasConfig_MASS();
        virtual bool HasConfig_Imgui();

        virtual bool IsEnable_MASS();
        virtual bool IsEnable_Imgui();

    public:
        FCamera* GetCamera() const { return this->pCamera; } 
        FCamera* GetCameraRight() const { return this->pCameraRight; }
        FVector2 GetViewportVector2() 
        {
            FVector2 vRectScreen;
            vRectScreen.x = this->poViewport.width;
            vRectScreen.y = this->poViewport.height;
            return vRectScreen;
        }
        FVector4 GetViewportVector4()
        {
            FVector4 vVP;
            vVP.x = this->poViewport.x;
            vVP.y = this->poViewport.y;
            vVP.z = this->poViewport.width;
            vVP.w = this->poViewport.height;
            return vVP;
        } 
        FVector2 ConvertNDC2Screen(float x, float y) //x: [-1, 1]; y: [-1, 1]
        {
            FVector2 vScreen;
            vScreen.x = (x + 1.0f) * this->poViewport.width / 2.0f + this->poViewport.x;
		    vScreen.y = (1.0f - y) * this->poViewport.height / 2.0f + this->poViewport.y;
            return vScreen;
        }
        FVector2 ConvertNDC2Screen(const FVector2& vNdc)
        {
            return ConvertNDC2Screen(vNdc.x, vNdc.y);
        }
        FPointI ConvertNDC2ScreenPointI(float x, float y)
        {
            FVector2 vScreen = ConvertNDC2Screen(x, y);
            return FPointI((int32)vScreen.x, (int32)vScreen.y);
        }
        FPointI ConvertNDC2ScreenPointI(const FVector2& v)
        {
            return ConvertNDC2ScreenPointI(v.x, v.y);
        }
        FVector2 ConvertScreen2NDC(float x, float y) //x: [0 - width]; y: [0 - height]
        {
            FVector2 vNdc;
            vNdc.x = (x - this->poViewport.x) * 2.0f / this->poViewport.width - 1.0f;
		    vNdc.y = 1.0f - (y - this->poViewport.y) * 2.0f / this->poViewport.height;
            return vNdc;
        }
        FVector2 ConvertScreen2NDC(const FVector2& vScreen)
        {
            return ConvertScreen2NDC(vScreen.x, vScreen.y);
        }

    protected:
        //Create Pipeline
        virtual void createPipeline();
            virtual void createWindowCallback();
            virtual void createDevice();
                virtual void destroyVkDevice(VkDevice vkDevice);
                virtual void destroyVkInstance(VkInstance vkInstance);
                virtual void createInstance();
                    virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
                virtual void setUpDebugMessenger();
                virtual void createSurface();
                virtual void pickPhysicalDevice();
                    virtual void findQueueFamilies(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute);
                    virtual VkSampleCountFlagBits getMaxUsableSampleCount();
                    virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, SwapChainSupportDetails& details);
                    virtual bool isDeviceSuitable(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute);
                virtual void setUpEnabledFeatures();
                virtual void createLogicalDevice();

            virtual void createFeatureSupport();

            virtual void createCommandObjects();
                virtual void createCommandPool();
                    virtual void createCommandPool_Graphics();
                    virtual void createCommandPool_Compute();

                        virtual void destroyVkCommandPool(VkCommandPool vkCommandPool);
                        virtual void freeCommandBuffers(VkCommandPool commandPool, uint32_t count, VkCommandBuffer* pCommandBuffer);

                        virtual VkCommandBuffer beginSingleTimeCommands();
                        virtual void endSingleTimeCommands(VkCommandBuffer commandBuffer);
            
            virtual void createSwapChainObjects();
                virtual void createSwapChain();
                    virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
                    virtual VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
                    virtual VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
                    virtual void createViewport();
                    virtual void createViewport(uint32_t width,
                                                uint32_t height,
                                                VkViewport& poViewport, 
                                                VkRect2D& poScissor);
                virtual void createSwapChainImageViews();
                    virtual void createColorResources();
                    virtual void createDepthResources();
                        virtual VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
                        virtual VkFormat findDepthFormat();
                        virtual bool hasStencilComponent(VkFormat format);

                virtual void destroyVkSurfaceKHR(VkSurfaceKHR vkSurfaceKHR);
                virtual void destroyVkSwapchainKHR(VkSwapchainKHR vkSwapchainKHR);

            virtual void createDescriptorPool();
                virtual void destroyVkDescriptorPool(VkDescriptorPool vkDescriptorPool);

            virtual void createDescriptorSetLayouts();
                virtual void createDescriptorSetLayout_Default();
                virtual void createDescriptorSetLayout_Custom();

            virtual void createPipelineObjects();
                virtual void createRenderPasses();
                    virtual void createRenderPass_Default();
                    virtual void createRenderPass_Custom();
                        virtual void createRenderPass_KhrDepth(VkFormat formatSwapChain, VkFormat formatDepth, VkRenderPass& vkRenderPass);
                        virtual void createRenderPass_KhrDepthImgui(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkRenderPass& vkRenderPass);
                        virtual void createRenderPass_ColorDepthMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass);
                        virtual void createRenderPass_ColorDepthImguiMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass);

                        virtual void createAttachmentDescription(VkAttachmentDescription& attachment,
                                                                 VkAttachmentDescriptionFlags flags,
                                                                 VkFormat format,
                                                                 VkSampleCountFlagBits samples,
                                                                 VkAttachmentLoadOp loadOp,
                                                                 VkAttachmentStoreOp storeOp,
                                                                 VkAttachmentLoadOp stencilLoadOp,
                                                                 VkAttachmentStoreOp stencilStoreOp,
                                                                 VkImageLayout initialLayout,
                                                                 VkImageLayout finalLayout);
                        virtual bool createVkRenderPass(const String& nameRenderPass,
                                                        const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                        const VkSubpassDescriptionVector& aSubpassDescription,
                                                        const VkSubpassDependencyVector& aSubpassDependency,
                                                        VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                                        VkRenderPass& vkRenderPass);
                        virtual void destroyVkRenderPass(VkRenderPass vkRenderPass);
                    
                virtual void createFramebuffers();
                    virtual void createFramebuffer_Default();
                    virtual void createFramebuffer_Custom();

                    virtual bool createVkFramebuffer(const String& nameFramebuffer,
                                                     const VkImageViewVector& aImageView, 
                                                     VkRenderPass& vkRenderPass,
                                                     VkFramebufferCreateFlags flags,
                                                     uint32_t width,
                                                     uint32_t height,
                                                     uint32_t layers,
                                                     VkFramebuffer& vkFramebuffer);
                    virtual void destroyVkFramebuffer(VkFramebuffer vkFramebuffer);

            virtual void createSyncObjects();
                virtual void createPresentRenderSyncObjects();
                virtual void createRenderComputeSyncObjects();

                    virtual void destroyVkFence(VkFence vkFence);
                    virtual void destroyVkSemaphore(VkSemaphore vkSemaphore);

        //Load Assets
        virtual void loadAssets();
            //Camera
            virtual void createCamera();

            //Geometry
            virtual void loadGeometry();
                virtual void loadVertexIndexBuffer();
                    virtual void loadModel();
                        virtual void loadModel_Assimp();
                        virtual void loadModel_Custom();
                    virtual void createVertexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkBuffer& vertexBuffer, 
                                                    VkDeviceMemory& vertexBufferMemory);
                    virtual void createVertexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkBuffer& vertexBuffer, 
                                                    VkDeviceMemory& vertexBufferMemory,
                                                    VkBuffer& stagingBuffer,
                                                    VkDeviceMemory& stagingBufferMemory);
                    virtual void updateVertexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkDeviceMemory& vertexBufferMemory);
                    virtual void createIndexBuffer(size_t bufSize, 
                                                   void* pBuf, 
                                                   VkBuffer& indexBuffer, 
                                                   VkDeviceMemory& indexBufferMemory);
                    virtual void createIndexBuffer(size_t bufSize, 
                                                   void* pBuf, 
                                                   VkBuffer& indexBuffer, 
                                                   VkDeviceMemory& indexBufferMemory,
                                                   VkBuffer& stagingBuffer,
                                                   VkDeviceMemory& stagingBufferMemory);
                     virtual void updateIndexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkDeviceMemory& indexBufferMemory);
                        virtual void createVkBuffer(VkDeviceSize size, 
                                                    VkBufferUsageFlags usage, 
                                                    VkMemoryPropertyFlags properties, 
                                                    VkBuffer& buffer, 
                                                    VkDeviceMemory& bufferMemory);
                        virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
                        virtual void copyVkBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
                        virtual void updateVKBuffer(size_t offset, size_t bufSize, void* pBuf, VkDeviceMemory& vertexBufferMemory, VkMemoryMapFlags flags = 0);

                        virtual void destroyVkBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory);
                
                virtual void loadTexture();
                    virtual void destroyVkImage(VkImage image, VkDeviceMemory imageMemory, VkImageView imageView);
                    virtual void destroyVkImageView(VkImageView imageView);
                    virtual void destroyVkImageSampler(VkSampler sampler);

                    virtual void createTexture1D(const String& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);

                    virtual void createTexture2D(const String& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory,
                                                 VkBuffer& buffer, 
                                                 VkDeviceMemory& bufferMemory);
                    virtual void createTexture2D(const String& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    virtual void createTexture2D(const String& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    
                    virtual void createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                                      uint32_t& mipMapCount,
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);

                    virtual void createTexture3D(VkFormat format,
                                                 const uint8* pDataRGBA,
                                                 uint32_t size,
                                                 uint32_t width,
                                                 uint32_t height,
                                                 uint32_t depth,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory,
                                                 VkBuffer& buffer, 
                                                 VkDeviceMemory& bufferMemory);
                    virtual void createTexture3D(VkFormat format,
                                                 const uint8* pDataRGBA,
                                                 uint32_t size,
                                                 uint32_t width,
                                                 uint32_t height,
                                                 uint32_t depth,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureCubeMap(const StringVector& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTextureCubeMap(const StringVector& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTextureCubeMap(const StringVector& aPathAsset_Tex,
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);

                    
                    virtual void createTextureRenderTarget1D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget1D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTarget2D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t mipMapCount,
                                                             VkImageType type,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);

                    virtual void createTextureRenderTarget2D(uint8* pData,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t mipMapCount,
                                                             VkImageType type,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2D(uint8* pData,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTarget2DArray(const FVector4& clDefault,
                                                                  bool isSetColor,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t numArray,
                                                                  uint32_t mipMapCount,
                                                                  VkImageType type,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory,
                                                                  VkBuffer& buffer, 
                                                                  VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2DArray(const FVector4& clDefault,
                                                                  bool isSetColor,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t numArray,
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory);

                    virtual void createTextureRenderTarget3D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t depth,
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);                                           
                    virtual void createTextureRenderTarget3D(const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t depth,
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTargetCubeMap(uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory,
                                                                  VkBuffer& buffer, 
                                                                  VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTargetCubeMap(uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory);
                    

                        virtual void createVkImage(uint32_t width, 
                                                   uint32_t height, 
                                                   uint32_t depth, 
                                                   uint32_t numArray,
                                                   uint32_t mipMapCount, 
                                                   VkImageType type, 
                                                   bool isCubeMap,
                                                   VkSampleCountFlagBits numSamples, 
                                                   VkFormat format, 
                                                   VkImageTiling tiling, 
                                                   VkImageUsageFlags usage, 
                                                   VkSharingMode sharingMode,
                                                   bool isGraphicsComputeShared,
                                                   VkMemoryPropertyFlags properties, 
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory);
                        virtual void createVkImageView(VkImage image, 
                                                       VkImageViewType type, 
                                                       VkFormat format, 
                                                       VkImageAspectFlags aspectFlags, 
                                                       uint32_t mipMapCount,
                                                       uint32_t numArray,
                                                       VkImageView& imageView);
                                                     
                        virtual void createVkSampler(uint32_t mipMapCount, 
                                                     VkSampler& sampler);
                        virtual void createVkSampler(VulkanTextureFilterType eTextureFilter,
                                                     VulkanTextureAddressingType eTextureAddressing,
                                                     VulkanTextureBorderColorType eTextureBorderColor,
                                                     bool enableAnisotropy,
                                                     float maxAnisotropy,
                                                     float minLod, 
                                                     float maxLod, 
                                                     float mipLodBias,
                                                     VkSampler& sampler);
                        
                        virtual void transitionImageLayout(VkCommandBuffer cmdBuffer,
                                                           VkImage image, 
                                                           VkImageLayout oldLayout, 
                                                           VkImageLayout newLayout,
                                                           uint32_t mipBase,
                                                           uint32_t mipCount,
                                                           uint32_t numBase,
                                                           uint32_t numArray,
                                                           VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
                        virtual void copyBufferToImage(VkCommandBuffer cmdBuffer,
                                                       VkBuffer buffer, 
                                                       VkImage image, 
                                                       uint32_t width, 
                                                       uint32_t height,
                                                       uint32_t depth,
                                                       uint32_t numArray);
                        virtual void generateMipMaps(VkCommandBuffer cmdBuffer,
                                                     VkImage image, 
                                                     VkFormat imageFormat, 
                                                     int32_t width, 
                                                     int32_t height, 
                                                     uint32_t mipMapCount,
                                                     uint32_t numArray,
                                                     bool autoMipMap);
                    
                
                virtual void createConstBuffers();
                    virtual void createPassCB();
                        virtual void buildPassCB();
                    virtual void createObjectCB();
                        virtual void buildObjectCB();
                    virtual void createMaterialCB();
                        virtual void buildMaterialCB();
                    virtual void createInstanceCB();
                        virtual void buildInstanceCB();
                    virtual void createCustomCB();

                virtual VkShaderModule createVkShaderModule(const String& info, const String& pathFile);
                    virtual void destroyVkShaderModule(VkShaderModule vkShaderModule);

                virtual bool createVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, VkDescriptorSetLayout& vkDescriptorSetLayout);
                    virtual void destroyVkDescriptorSetLayout(VkDescriptorSetLayout vkDescriptorSetLayout);

                virtual VkPipelineLayout createVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout);
                    virtual void destroyVkPipelineLayout(VkPipelineLayout vkPipelineLayout);
                    virtual void destroyVkPipeline(VkPipeline vkPipeline);

                virtual void createVkPipelineCache();
                virtual void destroyVkPipelineCache(VkPipelineCache vkPipelineCache);

                virtual void preparePipeline();
                    virtual void createCustomBeforePipeline();

                virtual void createGraphicsPipeline();
                    virtual void createGraphicsPipeline_Default();
                    virtual void createGraphicsPipeline_Terrain();
                    virtual void createGraphicsPipeline_Custom();
                        virtual VkPipeline createVkGraphicsPipeline(VkShaderModule vertShaderModule, const String& vertMain,
                                                                    VkShaderModule fragShaderModule, const String& fragMain,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask);
                        virtual VkPipeline createVkGraphicsPipeline(VkShaderModule vertShaderModule, const String& vertMain,
                                                                    VkShaderModule tescShaderModule, const String& tescMain,
                                                                    VkShaderModule teseShaderModule, const String& teseMain,
                                                                    VkShaderModule fragShaderModule, const String& fragMain,
                                                                    VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask);
                        virtual VkPipeline createVkGraphicsPipeline(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                    bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask);

                virtual void createComputePipeline();
                    virtual void createComputePipeline_Default();
                    virtual void createComputePipeline_Custom();
                        virtual VkPipeline createVkComputePipeline(VkShaderModule compShaderModule,
                                                                   const String& compMain,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0);
                        virtual VkPipeline createVkComputePipeline(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0);


                virtual void createDescriptorSets();
                    virtual void createDescriptorSets_Default();
                    virtual void createDescriptorSets_Terrain();
                    virtual void createDescriptorSets_Custom();
                        virtual void updateDescriptorSets(VkDescriptorSetVector& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler); 

                        virtual void createVkDescriptorSet(VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSet& vkDescriptorSet);
                        virtual void createVkDescriptorSets(VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSetVector& aDescriptorSets);

                        virtual VkDescriptorSetLayoutBinding createVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
                                                                                                        VkDescriptorType descriptorType,
                                                                                                        uint32_t descriptorCount,
                                                                                                        VkShaderStageFlags stageFlags);
                        virtual void createVkDescriptorSetLayoutBinding_Uniform(VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding,
                                                                                uint32_t binding,
                                                                                VkDescriptorType descriptorType,
                                                                                uint32_t descriptorCount,
                                                                                VkShaderStageFlags stageFlags);
                        virtual VkDescriptorSetLayoutBinding createVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                                                                      VkDescriptorType descriptorType,
                                                                                                      uint32_t descriptorCount,
                                                                                                      VkShaderStageFlags stageFlags,
                                                                                                      VkSampler* pImmutableSamplers);
                        virtual void createVkDescriptorSetLayoutBinding_Image(VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding,
                                                                              uint32_t binding,
                                                                              VkDescriptorType descriptorType,
                                                                              uint32_t descriptorCount,
                                                                              VkShaderStageFlags stageFlags,
                                                                              VkSampler* pImmutableSamplers);

                        virtual void pushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                 VkDescriptorSet dstSet,
                                                                 uint32_t dstBinding,
                                                                 uint32_t dstArrayElement,
                                                                 uint32_t descriptorCount,
                                                                 VkDescriptorBufferInfo& bufferInfo);
                        virtual void pushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                               VkDescriptorSet dstSet,
                                                               uint32_t dstBinding,
                                                               uint32_t dstArrayElement,
                                                               uint32_t descriptorCount,
                                                               VkDescriptorType descriptorType,
                                                               VkDescriptorImageInfo& imageInfo);

                        virtual void updateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets);

                virtual void createCommandBuffers();
                    virtual void createCommandBuffer_Graphics();
                    virtual void createCommandBuffer_Compute();

            //Imgui
            virtual void createImgui();
                virtual void createImgui_DescriptorPool();
                virtual void createImgui_Init();

            //Terrain
            virtual void createTerrain();
                virtual bool loadTerrainData();
                virtual void setupTerrainGeometry();
                virtual void setupTerrainTexture();
                virtual void setupTerrainShader();
                virtual void setupTerrainComputePipeline();
                    virtual void createTerrainComputeDescriptorSet();
                    virtual void destroyTerrainComputeDescriptorSet();

                virtual void setupTerrainGraphicsPipeline();
                    virtual void createTerrainGraphicsDescriptorSet();
                    virtual void destroyTerrainGraphicsDescriptorSet();
                    virtual void createTerrainGraphicsPipeline();
                    virtual void destroyTerrainGraphicsPipeline();
            virtual void destroyTerrain();

            //Editor
            virtual void createEditor();
                virtual void createEditor_Grid();
                virtual void createEditor_CameraAxis();
                virtual void createEditor_CoordinateAxis();       
            virtual void destroyEditor();

        //Resize
        virtual void resizeWindow(int w, int h, bool force);

        //Compute
        virtual bool beginCompute();
            virtual void updateCompute();
                virtual void updateComputeCommandBuffer();
                    virtual void updateCompute_Default(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_Custom(VkCommandBuffer& commandBuffer);

            virtual void compute();
        virtual void endCompute();

        //Render/Update
        virtual bool beginRender();
            virtual void updateRender();
                virtual void updateCBs_Default();
                    virtual void updateCBs_Pass();
                        virtual void updateCBs_PassTransformAndCamera(PassConstants& pass, FCamera* pCam, int nIndex);
                    virtual void updateCBs_Objects();
                        virtual void updateCBs_ObjectsContent();
                    virtual void updateCBs_Materials();
                        virtual void updateCBs_MaterialsContent();
                    virtual void updateCBs_Instances();
                        virtual void updateCBs_InstancesContent();
                virtual void updateCBs_Terrain();
                virtual void updateCBs_ImGUI();
                    virtual bool beginRenderImgui();
                        //Common
                        virtual void commonConfig();
                            virtual void commonOpConfig();
                        //Camera
                        virtual void cameraConfig();
                            virtual void cameraReset();
                        //Light
                        virtual void lightConfig();
                            virtual void lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType);
                        //PassConstants
                        virtual void passConstantsConfig();
                        //Model
                        virtual void modelConfig();
                    virtual void endRenderImgui();
                virtual void updateCBs_Editor();
                virtual void updateCBs_Custom();

                virtual void updateRenderCommandBuffers_CustomBeforeDefault();
                virtual void updateRenderCommandBuffers_Default();
                    virtual void updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_EditorCameraAxis(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_Default(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault_Editor(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault_Imgui(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomAfterDefault(VkCommandBuffer& commandBuffer);

                        virtual void beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                     const VkRenderPass& renderPass, 
                                                     const VkFramebuffer& frameBuffer,
                                                     const VkOffset2D& offset,
                                                     const VkExtent2D& extent,
                                                     const FVector4& clBg,
                                                     float depth,
                                                     uint32_t stencil);
                            virtual void bindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor);
                            virtual void bindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline);
                            virtual void bindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
                            virtual void bindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType);
                            virtual void bindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
                            virtual void draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
                            virtual void drawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
                            virtual void drawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
                            virtual void drawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
                        virtual void endRenderPass(VkCommandBuffer& commandBuffer);

                        virtual void dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ);
                        virtual void dispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset);

                virtual void updateRenderCommandBuffers_CustomAfterDefault();

            virtual void render();
        virtual void endRender();

        //cleanup
        virtual void cleanup();
            virtual void cleanupDefault();
                virtual void cleanupTexture();
                virtual void cleanupVertexIndexBuffer();
            virtual void cleanupTerrain();
            virtual void cleanupImGUI();
            virtual void cleanupEditor();
            virtual void cleanupCustom();

            virtual void cleanupSwapChain();
                virtual void cleanupSwapChain_Default();
                virtual void cleanupSwapChain_Terrain();
                virtual void cleanupSwapChain_Editor();
                virtual void cleanupSwapChain_Custom();
            virtual void recreateSwapChain();
                virtual void recreateSwapChain_Terrain();
                virtual void recreateSwapChain_Editor();
                virtual void recreateSwapChain_Custom();

    private:
        VkResult createDebugUtilsMessengerEXT(VkInstance instance, 
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger);
        void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);
        
        void getInstanceLayersAndExtensions(bool bIsEnableValidationLayers,
                                            ConstCharPtrVector& outInstanceLayers, 
                                            ConstCharPtrVector& outInstanceExtensions);
        void getDeviceLayersAndExtensions(bool bIsEnableValidationLayers,
                                          ConstCharPtrVector& outDeviceLayers, 
                                          ConstCharPtrVector& outDeviceExtensions);
    };

}; //LostPeter

#endif