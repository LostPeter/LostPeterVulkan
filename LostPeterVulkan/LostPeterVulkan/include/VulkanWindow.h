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
#include "VulkanDefine.h"

namespace LostPeterVulkan
{
    class vulkanExport VulkanWindow : public VulkanBase
    {
    public:
        VulkanWindow(int width, int height, String name);
        virtual ~VulkanWindow();

    ///////////////////////// Internal /////////////////////////
    public:
        //RenderPass
        VKRenderPassShadowMap* m_pVKRenderPassShadowMap; 
        VKRenderPassCull* m_pVKRenderPassCull;
        VKRenderPassTerrain* m_pVKRenderPassTerrain;

        //Uniform ConstantBuffer
        PassConstants passCB;
        VkBufferVector poBuffers_PassCB;
        VkDeviceMemoryVector poBuffersMemory_PassCB;

        //PipelineCompute
        VKPipelineComputeCull* m_pPipelineCompute_Cull;
        VKPipelineComputeTerrain* m_pPipelineCompute_Terrain;

        //PipelineGraphics
        VKPipelineGraphicsCopyBlitFromFrame* m_pPipelineGraphics_CopyBlitFromFrameColor;
        VKPipelineGraphicsCopyBlitFromFrame* m_pPipelineGraphics_CopyBlitFromFrameDepth;
        VKPipelineGraphicsCopyBlitToFrame* m_pPipelineGraphics_CopyBlitToFrame;
        VKPipelineGraphicsDepthShadowMap* m_pPipelineGraphics_DepthShadowMap;
        VKPipelineGraphicsDepthHiz* m_pPipelineGraphics_DepthHiz;
        VKPipelineGraphicsTerrain* m_pPipelineGraphics_Terrain;

        //Mesh
        MeshPtrVector m_aMeshes_Internal;
        MeshPtrMap m_mapMeshes_Internal;    

        //Texture
        TexturePtrVector m_aTextures_Internal;
        TexturePtrMap m_mapTextures_Internal;

        //DescriptorSetLayouts
        VkDescriptorSetLayoutVector m_aVkDescriptorSetLayouts_Internal;
        VkDescriptorSetLayoutMap m_mapVkDescriptorSetLayouts_Internal;
        std::map<String, StringVector> m_mapName2Layouts_Internal;

        //ShaderModule
        VkShaderModuleVector m_aVkShaderModules_Internal;
        VkShaderModuleMap m_mapVkShaderModules_Internal;

        //PipelineLayout
        VkPipelineLayoutVector m_aVkPipelineLayouts_Internal;
        VkPipelineLayoutMap m_mapVkPipelineLayouts_Internal;

    public:
        //Mesh
        virtual Mesh* FindMesh_Internal(const String& nameMesh);

        //Texture
        virtual Texture* FindTexture_Internal(const String& nameTexture);

        //DescriptorSetLayouts
        virtual VkDescriptorSetLayout FindDescriptorSetLayout_Internal(const String& nameDescriptorSetLayout);
        virtual StringVector* FindDescriptorSetLayoutNames_Internal(const String& nameDescriptorSetLayout);

        //ShaderModule
        virtual VkShaderModule FindShaderModule_Internal(const String& nameShaderModule);

        //PipelineLayout
        virtual VkPipelineLayout FindPipelineLayout_Internal(const String& namePipelineLayout);

        //PipelineCompute
        //PipelineCompute-Cull
        virtual void Update_Compute_Cull(VkCommandBuffer& commandBuffer);
        virtual void Update_Compute_HizDepthGenerate(VkCommandBuffer& commandBuffer);

        //PipelineCompute-Terrain
        virtual void UpdateDescriptorSets_Compute_Terrain();
        virtual void Update_Compute_Terrain(VkCommandBuffer& commandBuffer);

        //PipelineGraphics
        //PipelineGraphics-CopyBlitToFrame
        virtual void UpdateDescriptorSets_Graphics_CopyBlitToFrame(const VkDescriptorImageInfo& imageInfo);
        virtual void UpdateBuffer_Graphics_CopyBlitToFrame(const CopyBlitObjectConstants& object);
        virtual void Draw_Graphics_CopyBlitToFrame(VkCommandBuffer& commandBuffer);
        //PipelineGraphics-CopyBlitFromFrame
        virtual void UpdateDescriptorSets_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const VkImageView& imageView);
        virtual void UpdateDescriptorSets_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const VkDescriptorImageInfo& imageInfo);
        virtual void UpdateBuffer_Graphics_CopyBlitFromFrame(VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame, const CopyBlitObjectConstants& object);
        virtual void Draw_Graphics_CopyBlitFromFrame(VkCommandBuffer& commandBuffer, VKPipelineGraphicsCopyBlitFromFrame* pCopyBlitFromFrame);

        //PipelineGraphics_DepthShadowMap
        //ObjectWorld
        virtual void UpdateBuffer_ObjectWorld_Begin();
            virtual void UpdateBuffer_ObjectWorld_AddOne(const ObjectConstants& object);
            virtual void UpdateBuffer_ObjectWorld_AddList(const std::vector<ObjectConstants> objects);
        virtual void UpdateBuffer_ObjectWorld_End();
        virtual bool Draw_Graphics_DepthShadowMapBegin(VkCommandBuffer& commandBuffer);
            virtual void Draw_Graphics_DepthShadowMap(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceCount, int instanceStart);
        virtual void Draw_Graphics_DepthShadowMapEnd(VkCommandBuffer& commandBuffer);
        //CullInstance
        virtual void UpdateDescriptorSet_ShadowMapDepthCull(VkDescriptorSetVector* pescriptorSets, BufferUniform* pCB_CullInstance, BufferCompute* pCB_CullObjectInstances, BufferCompute* pCB_Result);
        virtual bool Draw_Graphics_CullInstance_DepthShadowMapCullBegin(VkCommandBuffer& commandBuffer);
            virtual void Draw_Graphics_CullInstance_DepthShadowMapCull(VkCommandBuffer& commandBuffer);
            virtual void Draw_Graphics_CullInstance_DepthShadowMapCullUnit(VkCommandBuffer& commandBuffer, const VkBuffer& bufferIndirectCmd, int index, MeshSub* pMeshSub);
            virtual void Draw_Graphics_BindDescriptorSet_ShadowMapDepthCull(VkCommandBuffer& commandBuffer, VkDescriptorSetVector* pescriptorSets);
        virtual void Draw_Graphics_CullInstance_DepthShadowMapCullEnd(VkCommandBuffer& commandBuffer);

        //PipelineGraphics_DepthHiz
        virtual void Draw_Graphics_DepthHiz(VkCommandBuffer& commandBuffer);
        virtual void UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToShaderReadOnly(VkCommandBuffer& commandBuffer);
        virtual void UpdateImageLayout_Graphics_DepthHizImageLayoutFromShaderReadOnlyToColorAttachment(VkCommandBuffer& commandBuffer);
        virtual void UpdateImageLayout_Graphics_DepthHizImageLayoutFromColorAttachmentToGeneral(VkCommandBuffer& commandBuffer);
        virtual void UpdateImageLayout_Graphics_DepthHizImageLayoutFromGeneralToColorAttachment(VkCommandBuffer& commandBuffer);

        //PipelineGraphics_Terrain
        virtual void UpdateDescriptorSets_Graphics_Terrain();
        virtual void UpdateBuffer_Graphics_Terrain();
        virtual void Draw_Graphics_Terrain(VkCommandBuffer& commandBuffer);
            virtual void Draw_Graphics_Terrain_Whole(VkCommandBuffer& commandBuffer);
            virtual void Draw_Graphics_Terrain_Instance(VkCommandBuffer& commandBuffer);
        virtual float GetTerrainHeight(const FVector3& vPos);
        virtual float GetTerrainHeight(float x, float z);
        virtual bool RaytraceTerrain(float screenX, float screenY, FVector3& vPos);
        virtual bool RaytraceTerrain(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FVector3& vPos);

    protected:
        virtual void createInternal();
        virtual void cleanupInternal();

        virtual void createResourceInternal();
        virtual void destroyResourceInternal();

        //Mesh
        virtual void destroyMeshes_Internal();
        virtual void createMeshes_Internal();

        //Texture
        virtual void destroyTextures_Internal();
        virtual void createTextures_Internal();

        //DescriptorSetLayouts
        virtual void destroyDescriptorSetLayouts_Internal();
        virtual void createDescriptorSetLayouts_Internal();

        //ShaderModule
        virtual void destroyShaderModules_Internal();
        virtual void createShaderModules_Internal();

        //PipelineLayout
        virtual void destroyPipelineLayouts_Internal();
        virtual void createPipelineLayouts_Internal();

        //Uniform ConstantBuffer
        virtual void destroyUniformCB_Internal();
            virtual void destroyUniform_PassCB();
        virtual void createUniformCB_Internal();
            virtual void createUniform_PassCB();

        //PipelineCompute
        virtual void destroyPipelineCompute_Internal();
            virtual void destroyPipelineCompute_Cull();
            virtual void destroyPipelineCompute_Terrain();
        virtual void createPipelineCompute_Internal();
            virtual void createPipelineCompute_Cull();
            virtual void createPipelineCompute_Terrain();

        //PipelineGraphics
        virtual void destroyPipelineGraphics_Internal();
            virtual void destroyPipelineGraphics_CopyBlitFromFrame();
            virtual void destroyPipelineGraphics_CopyBlitToFrame();
            virtual void destroyPipelineGraphics_DepthShadowMap();
            virtual void destroyPipelineGraphics_DepthHiz();
            virtual void destroyPipelineGraphics_Terrain();
        virtual void createPipelineGraphics_Internal();
            virtual void createPipelineGraphics_CopyBlitFromFrame();
            virtual void createPipelineGraphics_CopyBlitToFrame();
            virtual void createPipelineGraphics_DepthShadowMap();
            virtual void createPipelineGraphics_DepthHiz();
            virtual void createPipelineGraphics_Terrain();
        
    ///////////////////////// Internal /////////////////////////

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

        //VkSpecializationInfo
        VkSpecializationMapEntry CreateSpecializationMapEntry(uint32_t constantID, uint32_t offset, size_t size)
		{
			VkSpecializationMapEntry specializationMapEntry = {};
			specializationMapEntry.constantID = constantID;
			specializationMapEntry.offset = offset;
			specializationMapEntry.size = size;
			return specializationMapEntry;
		}
        VkSpecializationInfo CreateSpecializationInfo(uint32_t mapEntryCount, const VkSpecializationMapEntry* mapEntries, size_t dataSize, const void* data)
		{
			VkSpecializationInfo specializationInfo = {};
			specializationInfo.mapEntryCount = mapEntryCount;
			specializationInfo.pMapEntries = mapEntries;
			specializationInfo.dataSize = dataSize;
			specializationInfo.pData = data;
			return specializationInfo;
		}
        VkSpecializationInfo CreateSpecializationInfo(const VkSpecializationMapEntryVector& mapEntries, size_t dataSize, const void* data)
		{
			VkSpecializationInfo specializationInfo = {};
			specializationInfo.mapEntryCount = static_cast<uint32_t>(mapEntries.size());
			specializationInfo.pMapEntries = mapEntries.data();
			specializationInfo.dataSize = dataSize;
			specializationInfo.pData = data;
			return specializationInfo;
		}

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
        VKDebug* poDebug;
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
        VkImageVector poSwapChainImages;
        VkFormat poSwapChainImageFormat;
        VkExtent2D poSwapChainExtent;
        FVector2 poFramebufferSize;
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
        VkImageView poDepthImageView_Depth;
        VkImageView poDepthImageView_Stencil;
        VkImageVector poColorImageLists;
        VkDeviceMemoryVector poColorImageMemoryLists;
        VkImageViewVector poColorImageViewLists;

        VkRenderPass poRenderPass;
        VkDescriptorSetLayout poDescriptorSetLayout;
        
        VkCommandPool poCommandPoolGraphics;
        std::vector<VkCommandBuffer> poCommandBuffersGraphics;

        VkCommandPool poCommandPoolCompute;
        VkCommandBuffer poCommandBufferComputeBefore;
        VkCommandBuffer poCommandBufferComputeAfter;
        
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

        VkSemaphore poGraphicsWaitComputeBeforeSemaphore;
        VkSemaphore poGraphicsSignalComputeAfterSemaphore;
        VkSemaphore poComputeBeforeWaitSemaphore;
        VkSemaphore poComputeAfterWaitSemaphore;

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
        bool isComputeCullFrustum;
        bool isComputeCullFrustumHizDepth;

        //Config
        FVector4 cfg_colorBackground;
        FVector4Vector cfg_colorValues;
        
        bool cfg_isRenderPassShadowMap;
        bool cfg_isRenderPassCull;
        bool cfg_isRenderPassTerrain;
        bool cfg_isRenderPassDefaultCustom;

        bool cfg_isMSAA;
        bool cfg_isImgui;
        bool cfg_isWireFrame;
        bool cfg_isRotate;
        bool cfg_isNegativeViewport;
        bool cfg_isUseCopyBlitFromFrameColor;
        bool cfg_isUseCopyBlitFromFrameDepth;
        bool cfg_isUseFramebuffer_Depth;
        bool cfg_isUseFramebuffer_Stencil;
        bool cfg_isUseComputeShaderBeforeRender;
        bool cfg_isUseComputeShaderAfterRender;
        bool cfg_isCreateRenderComputeSycSemaphore;
        VkDynamicStateVector cfg_aDynamicStates;
        VkPrimitiveTopology cfg_vkPrimitiveTopology;
        VkFrontFace cfg_vkFrontFace;
        VkPolygonMode cfg_vkPolygonMode;
        VkCullModeFlagBits cfg_vkCullModeFlagBits;
        VkBool32 cfg_isDepthBiasEnable;
        float cfg_DepthBiasConstantFactor;
        float cfg_DepthBiasClamp;
        float cfg_DepthBiasSlopeFactor;
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
        String cfg_terrainTextureDiffuse_Path;
        String cfg_terrainTextureNormal_Path;
        String cfg_terrainTextureControl_Path;
        float cfg_terrainHeightStart;
        float cfg_terrainHeightMax;

        //Imgui
        bool imgui_IsEnable;
        int	imgui_MinimalSwapchainImages;
	    VkDescriptorPool imgui_DescriptorPool;
        String imgui_PathIni;
        String imgui_PathLog;

        //Constants Buffer
        std::vector<ObjectConstants> objectCBs;
        VkBufferVector poBuffers_ObjectCB;
        VkDeviceMemoryVector poBuffersMemory_ObjectCB;

        std::vector<MaterialConstants> materialCBs;
        VkBufferVector poBuffers_MaterialCB;
        VkDeviceMemoryVector poBuffersMemory_MaterialCB;

        std::vector<InstanceConstants> instanceCBs;
        VkBufferVector poBuffers_InstanceCB;
        VkDeviceMemoryVector poBuffersMemory_InstanceCB;

        //Camera
        FCamera* pCamera; //Eye Left
        FCamera* pCameraRight; //Eye Right

        //Light
        LightConstants mainLight; //common.x == Vulkan_Light_Directional, can not change
        LightConstants aAdditionalLights[MAX_LIGHT_COUNT];

        //Shadow
        ShadowConstants shadowMainLight; //mainLight's shadow
        ShadowConstants shadowMainLight_Cfg; //mainLight's shadow cfg
        FCamera* pCameraMainLight; //mainLight's shadow camera
        
        //Mouse
        FVector2 mousePosLast;
        bool mouseButtonDownLeft;
        bool mouseButtonDownRight;
        bool mouseButtonDownMiddle;

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
        EditorLineFlat2DCollector* pEditorLineFlat2DCollector;
        EditorLineFlat3DCollector* pEditorLineFlat3DCollector;

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

        virtual bool OnBeginCompute_BeforeRender();
            virtual void OnUpdateCompute_BeforeRender();
            virtual void OnCompute_BeforeRender();
        virtual void OnEndCompute_BeforeRender();

        virtual bool OnBeginRender();
            virtual void OnUpdateRender();
            virtual void OnRender();
        virtual void OnEndRender();

        virtual bool OnBeginCompute_AfterRender();
            virtual void OnUpdateCompute_AfterRender();
            virtual void OnCompute_AfterRender();
        virtual void OnEndCompute_AfterRender();

        virtual void OnPresent();
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
        virtual bool HasConfig_RenderPassDefaultCustom();
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

    public:
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

            //Camera/Light
            virtual void createCamera();
            virtual void createLightMain();
            virtual void createShadowLightMain();
            virtual void createTerrain();

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
                    virtual void createColorResourceLists();

                virtual void destroyVkSurfaceKHR(VkSurfaceKHR vkSurfaceKHR);
                virtual void destroyVkSwapchainKHR(VkSwapchainKHR vkSwapchainKHR);

            virtual void createDescriptorPool();
                virtual void destroyVkDescriptorPool(VkDescriptorPool vkDescriptorPool);

            virtual void createDescriptorSetLayouts();
                virtual void createDescriptorSetLayout_Default();
                virtual void createDescriptorSetLayout_Custom();

            virtual void createPipelineObjects();
                virtual void createRenderPasses();
                    virtual void createRenderPass_ShadowMap();
                    virtual void createRenderPass_Default();
                    virtual void createRenderPass_Cull();
                    virtual void createRenderPass_Terrain();
                    virtual void createRenderPass_Custom();
                        virtual void createRenderPass_DefaultCustom(VkRenderPass& vkRenderPass);
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
                        virtual void createFramebuffer_DefaultCustom();

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

            //Geometry
            virtual void loadGeometry();
                virtual void loadVertexIndexBuffer();
                    virtual void loadModel();
                        virtual void loadModel_Default();
                        virtual void loadModel_Custom();
                    virtual void createVertexBuffer(const String& nameBuffer,
                                                    size_t bufSize, 
                                                    void* pBuf, 
                                                    VkBuffer& vertexBuffer, 
                                                    VkDeviceMemory& vertexBufferMemory);
                    virtual void createVertexBuffer(const String& nameBuffer,
                                                    size_t bufSize, 
                                                    void* pBuf, 
                                                    VkBuffer& vertexBuffer, 
                                                    VkDeviceMemory& vertexBufferMemory,
                                                    VkBuffer& stagingBuffer,
                                                    VkDeviceMemory& stagingBufferMemory);
                    virtual void updateVertexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkDeviceMemory& vertexBufferMemory);
                    virtual void createIndexBuffer(const String& nameBuffer,
                                                   size_t bufSize, 
                                                   void* pBuf, 
                                                   VkBuffer& indexBuffer, 
                                                   VkDeviceMemory& indexBufferMemory);
                    virtual void createIndexBuffer(const String& nameBuffer,
                                                   size_t bufSize, 
                                                   void* pBuf, 
                                                   VkBuffer& indexBuffer, 
                                                   VkDeviceMemory& indexBufferMemory,
                                                   VkBuffer& stagingBuffer,
                                                   VkDeviceMemory& stagingBufferMemory);
                     virtual void updateIndexBuffer(size_t bufSize, 
                                                    void* pBuf, 
                                                    VkDeviceMemory& indexBufferMemory);
                        virtual void createVkBuffer(const String& nameBuffer,
                                                    VkDeviceSize size, 
                                                    VkBufferUsageFlags usage, 
                                                    VkMemoryPropertyFlags properties, 
                                                    VkBuffer& buffer, 
                                                    VkDeviceMemory& bufferMemory);
                        virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
                        virtual void copyVkBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
                        virtual void updateVKBuffer(size_t offset, size_t bufSize, void* pBuf, VkDeviceMemory& vertexBufferMemory, VkMemoryMapFlags flags = 0);

                        virtual void destroyVkBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory);
                
                virtual void loadTexture();
                    virtual void loadTexture_Default();
                    virtual void loadTexture_Custom();
                    
                    virtual void destroyVkImage(VkImage image, VkDeviceMemory imageMemory, VkImageView imageView);
                    virtual void destroyVkImageView(VkImageView imageView);
                    virtual void destroyVkImageSampler(VkSampler sampler);

                    virtual void createTexture1D(const String& nameTex, 
                                                 const String& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);

                    virtual void createTexture2D(const String& nameTex, 
                                                 const String& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory,
                                                 VkBuffer& buffer, 
                                                 VkDeviceMemory& bufferMemory);
                    virtual void createTexture2D(const String& nameTex, 
                                                 const String& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    virtual void createTexture2D(const String& nameTex, 
                                                 const String& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    
                    virtual void createTexture2DArray(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTexture2DArray(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTexture2DArray(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex, 
                                                      uint32_t& mipMapCount,
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);

                    virtual void createTexture3D(const String& nameTex, 
                                                 VkFormat format,
                                                 const uint8* pDataRGBA,
                                                 uint32_t size,
                                                 uint32_t width,
                                                 uint32_t height,
                                                 uint32_t depth,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory,
                                                 VkBuffer& buffer, 
                                                 VkDeviceMemory& bufferMemory);
                    virtual void createTexture3D(const String& nameTex, 
                                                 VkFormat format,
                                                 const uint8* pDataRGBA,
                                                 uint32_t size,
                                                 uint32_t width,
                                                 uint32_t height,
                                                 uint32_t depth,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureCubeMap(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTextureCubeMap(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTextureCubeMap(const String& nameTex, 
                                                      const StringVector& aPathAsset_Tex,
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);

                    
                    virtual void createTextureRenderTarget1D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget1D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTarget2D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkImageType type,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);

                    virtual void createTextureRenderTarget2D(const String& nameTex,
                                                             uint8* pData,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkImageType type,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2D(const String& nameTex,
                                                             uint8* pData,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTarget2DArray(const String& nameTex,
                                                                  const FVector4& clDefault,
                                                                  bool isSetColor,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t numArray,
                                                                  bool autoMipMap, 
                                                                  uint32_t mipMapCount,
                                                                  VkImageType type,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  VkImageLayout finalLayout,
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory,
                                                                  VkBuffer& buffer, 
                                                                  VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTarget2DArray(const String& nameTex,
                                                                  const FVector4& clDefault,
                                                                  bool isSetColor,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  uint32_t numArray,
                                                                  bool autoMipMap, 
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  VkImageLayout finalLayout,
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory);

                    virtual void createTextureRenderTarget3D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t depth,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory,
                                                             VkBuffer& buffer, 
                                                             VkDeviceMemory& bufferMemory);                                           
                    virtual void createTextureRenderTarget3D(const String& nameTex,
                                                             const FVector4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t height,
                                                             uint32_t depth,
                                                             bool autoMipMap, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             VkImageLayout finalLayout,
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTargetCubeMap(const String& nameTex,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  bool autoMipMap, 
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  VkImageLayout finalLayout,
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory,
                                                                  VkBuffer& buffer, 
                                                                  VkDeviceMemory& bufferMemory);
                    virtual void createTextureRenderTargetCubeMap(const String& nameTex,
                                                                  uint32_t width, 
                                                                  uint32_t height,
                                                                  bool autoMipMap, 
                                                                  uint32_t mipMapCount,
                                                                  VkSampleCountFlagBits numSamples,
                                                                  VkFormat format,
                                                                  VkImageUsageFlags usage, 
                                                                  VkImageLayout finalLayout,
                                                                  bool isGraphicsComputeShared,
                                                                  VkImage& image, 
                                                                  VkDeviceMemory& imageMemory);
                    
                        virtual uint32_t getSizeFromFormat(VkFormat format);
                        virtual void createVkImage(const String& nameTex,
                                                   uint32_t width, 
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
                        virtual void createVkImageView(const String& nameTex,
                                                       VkImage image, 
                                                       VkImageViewType type, 
                                                       VkFormat format, 
                                                       VkImageAspectFlags aspectFlags, 
                                                       uint32_t mipMapCount,
                                                       uint32_t numArray,
                                                       VkImageView& imageView);
                        virtual void createVkImageView(const String& nameTex,
                                                       VkImageViewCreateFlags flags, 
                                                       VkImage image, 
                                                       VkImageViewType type, 
                                                       VkFormat format, 
                                                       VkComponentMapping componentMapping,
                                                       VkImageAspectFlags aspectFlags, 
                                                       uint32_t baseMipLevel,
                                                       uint32_t mipMapCount,
                                                       uint32_t baseArrayLayer,
                                                       uint32_t numArray,
                                                       VkImageView& imageView);
                                                     
                        virtual void createVkSampler(const String& nameTex,
                                                     uint32_t mipMapCount, 
                                                     VkSampler& sampler);
                        virtual void createVkSampler(const String& nameTex,
                                                     FTextureFilterType eTextureFilter,
                                                     FTextureAddressingType eTextureAddressing,
                                                     FTextureBorderColorType eTextureBorderColor,
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
                                                     VkImageLayout finalLayout,
                                                     int32_t width, 
                                                     int32_t height, 
                                                     uint32_t mipMapCount,
                                                     uint32_t numArray,
                                                     bool autoMipMap);

                virtual void createConstBuffers();
                    virtual void createObjectCB();
                        virtual void buildObjectCB();
                    virtual void createMaterialCB();
                        virtual void buildMaterialCB();
                    virtual void createInstanceCB();
                        virtual void buildInstanceCB();
                    virtual void createCustomCB();

                virtual VkShaderModule createVkShaderModule(const String& nameShader, FShaderType typeShader, const String& pathFile);
                virtual VkShaderModule createVkShaderModule(const String& nameShader, const String& strTypeShader, const String& pathFile);
                    virtual void destroyVkShaderModule(VkShaderModule vkShaderModule);

                virtual bool createVkDescriptorSetLayout(const String& nameDescriptorSetLayout, const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, VkDescriptorSetLayout& vkDescriptorSetLayout);
                    virtual void destroyVkDescriptorSetLayout(VkDescriptorSetLayout vkDescriptorSetLayout);

                virtual VkPipelineLayout createVkPipelineLayout(const String& namePipelineLayout, const VkDescriptorSetLayoutVector& aDescriptorSetLayout);
                    virtual void destroyVkPipelineLayout(VkPipelineLayout vkPipelineLayout);
                    virtual void destroyVkPipeline(VkPipeline vkPipeline);

                virtual void createVkPipelineCache();
                virtual void destroyVkPipelineCache(VkPipelineCache vkPipelineCache);


                virtual void createCustomBeforePipeline();
                virtual void createGraphicsPipeline();
                    virtual void createGraphicsPipeline_Default();
                    virtual void createGraphicsPipeline_Custom();
                        virtual VkPipeline createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                    VkShaderModule vertShaderModule, const String& vertMain,
                                                                    VkShaderModule fragShaderModule, const String& fragMain,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth, 
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);
                        virtual VkPipeline createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                    VkShaderModule vertShaderModule, const String& vertMain,
                                                                    VkShaderModule tescShaderModule, const String& tescMain,
                                                                    VkShaderModule teseShaderModule, const String& teseMain,
                                                                    VkShaderModule fragShaderModule, const String& fragMain,
                                                                    VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);
                        virtual VkPipeline createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                    const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                    bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask, uint32_t subpass = 0);
                        virtual VkPipeline createVkGraphicsPipeline(const String& nameGraphicsPipeline,
                                                                    const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                                    bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors, const VkDynamicStateVector& aDynamicStates,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, VkBool32 depthBiasEnable, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor, float lineWidth,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    const VkPipelineColorBlendAttachmentStateVector& aColorBlendAttachmentState, uint32_t subpass = 0);

                virtual void createComputePipeline();
                    virtual void createComputePipeline_Default();
                    virtual void createComputePipeline_Custom();
                        virtual VkPipeline createVkComputePipeline(const String& nameComputePipeline,
                                                                   VkShaderModule compShaderModule,
                                                                   const String& compMain,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0,
                                                                   VkSpecializationInfo* pSpecializationInfo = nullptr);
                        virtual VkPipeline createVkComputePipeline(const String& nameComputePipeline,
                                                                   const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0);


                virtual void createDescriptorSets();
                    virtual void createDescriptorSets_Default();
                    virtual void createDescriptorSets_Terrain();
                    virtual void createDescriptorSets_Custom();
                        virtual void updateDescriptorSets(VkDescriptorSetVector& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler); 

                        virtual void destroyVkDescriptorSet(VkDescriptorSet& vkDescriptorSet);
                        virtual void destroyVkDescriptorSets(VkDescriptorSetVector& aDescriptorSets);
                        virtual void createVkDescriptorSet(const String& nameDescriptorSet, VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSet& vkDescriptorSet);
                        virtual void createVkDescriptorSets(const String& nameDescriptorSets, VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSetVector& aDescriptorSets);
                        virtual void createVkDescriptorSets(const String& nameDescriptorSets, VkDescriptorSetLayout vkDescriptorSetLayout, int countDescriptorSets, VkDescriptorSetVector& aDescriptorSets);

                        virtual VkDescriptorSetLayoutBinding createVkDescriptorSetLayoutBinding_Buffer(uint32_t binding,
                                                                                                       VkDescriptorType descriptorType,
                                                                                                       uint32_t descriptorCount,
                                                                                                       VkShaderStageFlags stageFlags);
                        virtual void createVkDescriptorSetLayoutBinding_Buffer(VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding,
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

                        virtual void pushVkDescriptorSet_Buffer(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                VkDescriptorSet dstSet,
                                                                uint32_t dstBinding,
                                                                uint32_t dstArrayElement,
                                                                uint32_t descriptorCount,
                                                                VkDescriptorType type,
                                                                VkDescriptorBufferInfo& bufferInfo);
                        virtual void pushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                 VkDescriptorSet dstSet,
                                                                 uint32_t dstBinding,
                                                                 uint32_t dstArrayElement,
                                                                 uint32_t descriptorCount,
                                                                 VkDescriptorBufferInfo& bufferInfo);
                        virtual void pushVkDescriptorSet_UniformDynamic(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                        VkDescriptorSet dstSet,
                                                                        uint32_t dstBinding,
                                                                        uint32_t dstArrayElement,
                                                                        uint32_t descriptorCount,
                                                                        VkDescriptorBufferInfo& bufferInfo);
                        virtual void pushVkDescriptorSet_Storage(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                 VkDescriptorSet dstSet,
                                                                 uint32_t dstBinding,
                                                                 uint32_t dstArrayElement,
                                                                 uint32_t descriptorCount,
                                                                 VkDescriptorBufferInfo& bufferInfo);
                        virtual void pushVkDescriptorSet_StorageDynamic(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                                        VkDescriptorSet dstSet,
                                                                        uint32_t dstBinding,
                                                                        uint32_t dstArrayElement,
                                                                        uint32_t descriptorCount,
                                                                        VkDescriptorBufferInfo& bufferInfo);
                         virtual void pushVkDescriptorSet_IndirectCommand(VkWriteDescriptorSetVector& aWriteDescriptorSets,
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

            //Editor
            virtual void createEditor();
                virtual void createEditor_Grid();
                virtual void createEditor_CameraAxis();
                virtual void createEditor_CoordinateAxis();      
                virtual void createEditor_LineFlat2DCollector();
                virtual void createEditor_LineFlat3DCollector();  
            virtual void destroyEditor();

        //Resize
        virtual void resizeWindow(int w, int h, bool force);

        //Compute Before Render
        virtual bool beginCompute_BeforeRender();
            virtual void updateCompute_BeforeRender();
                virtual void updateComputeCommandBuffer_BeforeRender();
                    virtual void updateCompute_BeforeRender_Default(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_BeforeRender_Terrain(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_BeforeRender_Custom(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_BeforeRender_Cull(VkCommandBuffer& commandBuffer);
                    
            virtual void compute_BeforeRender();
        virtual void endCompute_BeforeRender();

        //Compute After Render
        virtual bool beginCompute_AfterRender();
            virtual void updateCompute_AfterRender();
                virtual void updateComputeCommandBuffer_AfterRender();
                    virtual void updateCompute_AfterRender_Default(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_AfterRender_Custom(VkCommandBuffer& commandBuffer);
                    virtual void updateCompute_AfterRender_HizDepthGenerate(VkCommandBuffer& commandBuffer);

            virtual void compute_AfterRender();
        virtual void endCompute_AfterRender();


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
                            virtual void commonShowConfig();
                            virtual void commonEditorConfig();
                        //Camera
                        virtual void cameraConfig();
                            virtual void cameraReset();
                        //Light
                        virtual void lightConfig();
                            virtual void lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType, bool bIsMainLight);
                            virtual void lightMainReset();
                        //Shadow
                        virtual void shadowConfig();
                            virtual void shadowConfigItem(ShadowConstants& sc, const String& name, bool bIsMainLight);
                            virtual void shadowReset();
                         //Cull
                        virtual void cullConfig();
                        //Terrain
                        virtual void terrainConfig();
                            virtual void terrainConfigItem(TerrainConstants& tc, const String& name);
                                virtual bool terrainConfigSplatItem(TerrainSplatConstants& tsc, const String& name);
                            virtual void terrainReset();
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
                    virtual void updateRenderPass_EditorCameraAxis(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_ShadowMap(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshShadowMap(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_Default(VkCommandBuffer& commandBuffer);
                        virtual void updateMeshDefault_Before(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDefault(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshTerrain(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDefault_Editor(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDefault_CustomBeforeImgui(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDefault_Imgui(VkCommandBuffer& commandBuffer);
                        virtual void updateMeshDefault_After(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomAfterDefault(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_BlitFromFrame(VkCommandBuffer& commandBuffer);
                        virtual void updateBlitFromFrame_Color_Before(VkCommandBuffer& commandBuffer);
                            virtual void drawBlitFromFrame_Color(VkCommandBuffer& commandBuffer);
                        virtual void updateBlitFromFrame_Color_After(VkCommandBuffer& commandBuffer);
                        virtual void updateBlitFromFrame_Depth_Before(VkCommandBuffer& commandBuffer);
                            virtual void drawBlitFromFrame_Depth(VkCommandBuffer& commandBuffer);
                        virtual void updateBlitFromFrame_Depth_After(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_DepthHiz(VkCommandBuffer& commandBuffer);
                        virtual void updateMeshDepthHiz_Before(VkCommandBuffer& commandBuffer);
                            virtual void drawMeshDepthHiz(VkCommandBuffer& commandBuffer);
                        virtual void updateMeshDepthHiz_After(VkCommandBuffer& commandBuffer);


                        virtual void beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                     const String& nameRenderPass,
                                                     const VkRenderPass& renderPass, 
                                                     const VkFramebuffer& frameBuffer,
                                                     const VkOffset2D& offset,
                                                     const VkExtent2D& extent,
                                                     const FVector4& clBg,
                                                     float depth,
                                                     uint32_t stencil);
                        virtual void beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                     const String& nameRenderPass,
                                                     const VkRenderPass& renderPass, 
                                                     const VkFramebuffer& frameBuffer,
                                                     const VkOffset2D& offset,
                                                     const VkExtent2D& extent,
                                                     const FVector4Vector& aColors,
                                                     float depth,
                                                     uint32_t stencil);
                        virtual void beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                     const String& nameRenderPass,
                                                     const VkRenderPass& renderPass, 
                                                     const VkFramebuffer& frameBuffer,
                                                     const VkOffset2D& offset,
                                                     const VkExtent2D& extent,
                                                     const VkClearValueVector& aClearValue);
                            virtual void bindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor);
                            virtual void setDepthBias(VkCommandBuffer& commandBuffer, float depthBiasConstantFactor, float depthBiasClamp, float depthBiasSlopeFactor);
                            virtual void setPrimitiveTopology(VkCommandBuffer& commandBuffer, VkPrimitiveTopology vkPrimitiveTopology);
                            virtual void bindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline);
                            virtual void bindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
                            virtual void bindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType);
                            virtual void bindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
                            virtual void bindPipelineAndDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipeline& vkPipeline, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets);
                            virtual void draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance);
                            virtual void drawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
                            virtual void drawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
                            virtual void drawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
                        virtual void endRenderPass(VkCommandBuffer& commandBuffer);

                        virtual void dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ);
                        virtual void dispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset);

                        virtual void pipelineBarrier(VkCommandBuffer& commandBuffer, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount, const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount, const VkImageMemoryBarrier* pImageMemoryBarriers);

                virtual void updateRenderCommandBuffers_CustomAfterDefault();

            virtual void render();
        virtual void endRender();

        //cleanup
        virtual void cleanup();
            virtual void cleanupDefault();
                virtual void cleanupTexture();
                virtual void cleanupVertexIndexBuffer();
            virtual void cleanupImGUI();
            virtual void cleanupEditor();
            virtual void cleanupCustom();

            virtual void cleanupSwapChain();
                virtual void cleanupSwapChain_Default();
                virtual void cleanupSwapChain_Editor();
                virtual void cleanupSwapChain_Custom();
            virtual void recreateSwapChain();
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

}; //LostPeterVulkan

#endif