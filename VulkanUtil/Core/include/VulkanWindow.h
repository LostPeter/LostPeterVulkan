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
        VulkanWindow(int width, int height, std::string name);
        virtual ~VulkanWindow();

    public:
        static int versionVulkan_Major;
        static int versionVulkan_Minor;    

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
        glm::mat4 poMatWorld;

        VulkanVertexType poTypeVertex;
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
        std::vector<VkDescriptorSet> poDescriptorSets;


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
        glm::vec4 cfg_colorBackground;
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

        glm::vec3 cfg_cameraPos;
        glm::vec3 cfg_cameraLookTarget;
        glm::vec3 cfg_cameraUp;

        float cfg_cameraFov;
        float cfg_cameraNear;
        float cfg_cameraFar;

        float cfg_cameraSpeedMove;
        float cfg_cameraSpeedZoom;
        float cfg_cameraSpeedRotate;

        //Custom
        std::string cfg_model_Path;
        std::string cfg_shaderVertex_Path;
        std::string cfg_shaderFragment_Path;
        std::string cfg_texture_Path;

        //Imgui
        bool imgui_IsEnable;
        int	imgui_MinimalSwapchainImages;
	    VkDescriptorPool imgui_DescriptorPool;
        std::string imgui_PathIni;
        std::string imgui_PathLog;

        //Multi object use, top priority
        VulkanSceneManager* pSceneManager;

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

        //Camera
        VulkanCamera* pCamera;

        //Light
        LightConstants mainLight; //common.x == Vulkan_Light_Directional, can not change
        LightConstants aAdditionalLights[MAX_LIGHT_COUNT];

        //Mouse
        glm::vec2 mousePosLast;
        bool mouseButtonDownLeft;
        bool mouseButtonDownRight;


    protected:
        std::vector<const char*> aInstanceLayers;
        std::vector<const char*> aInstanceExtensions;
        std::vector<const char*> aDeviceLayers;
        std::vector<const char*> aDeviceExtensions;

    public:
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
        virtual void OnMouseMove(int button, double x, double y);
        virtual void OnMouseWheel(double x, double y);

        // Keyboard Input
        virtual void OnKeyboardInput();
        virtual void OnKeyDown(int key);
        virtual void OnKeyUp(int key);

    public:
        virtual bool HasConfig_MASS();
        virtual bool HasConfig_Imgui();

        virtual bool IsEnable_MASS();
        virtual bool IsEnable_Imgui();

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
                virtual void createSwapChainImageViews();
                    virtual void createColorResources();
                    virtual void createDepthResources();
                        virtual VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
                        virtual VkFormat findDepthFormat();
                        virtual bool hasStencilComponent(VkFormat format);

                virtual void destroyVkSurfaceKHR(VkSurfaceKHR vkSurfaceKHR);
                virtual void destroyVkSwapchainKHR(VkSwapchainKHR vkSwapchainKHR);

            virtual void createDescriptorObjects();
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
                        virtual bool createVkRenderPass(const std::string& nameRenderPass,
                                                        const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                        const VkSubpassDescriptionVector& aSubpassDescription,
                                                        const VkSubpassDependencyVector& aSubpassDependency,
                                                        VkRenderPass& vkRenderPass);
                        virtual void destroyVkRenderPass(VkRenderPass vkRenderPass);
                    
                virtual void createFramebuffers();
                    virtual void createFramebuffer_Default();
                    virtual void createFramebuffer_Custom();

                    virtual bool createVkFramebuffer(const std::string& nameFramebuffer,
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
            //Scene
            virtual void createScene();
                virtual void createSceneManager();
            virtual void buildScene();
                virtual void buildScene_Shaders();
                virtual void buildScene_InputLayouts();
                virtual void buildScene_Meshes();
                virtual void buildScene_SceneObjects();
                virtual void buildScene_Materials();
                virtual void buildScene_FrameResources();
                virtual void buildScene_ConstantBufferViews();
                virtual void buildScene_PipelineStates();

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
                    virtual void createIndexBuffer(size_t bufSize, 
                                                   void* pBuf, 
                                                   VkBuffer& indexBuffer, 
                                                   VkDeviceMemory& indexBufferMemory);
                        virtual void createVkBuffer(VkDeviceSize size, 
                                                    VkBufferUsageFlags usage, 
                                                    VkMemoryPropertyFlags properties, 
                                                    VkBuffer& buffer, 
                                                    VkDeviceMemory& bufferMemory);
                        virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
                        virtual void copyVkBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

                        virtual void destroyVkBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory);
                
                virtual void loadTexture();
                    virtual void destroyVkImage(VkImage image, VkDeviceMemory imageMemory, VkImageView imageView);
                    virtual void destroyVkImageView(VkImageView imageView);
                    virtual void destroyVkImageSampler(VkSampler sampler);

                    virtual void createTexture1D(const std::string& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);

                    virtual void createTexture2D(const std::string& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory,
                                                 VkBuffer& buffer, 
                                                 VkDeviceMemory& bufferMemory);
                    virtual void createTexture2D(const std::string& pathAsset_Tex, 
                                                 VkImageType type,
                                                 VkSampleCountFlagBits numSamples,
                                                 VkFormat format,
                                                 bool autoMipMap, 
                                                 uint32_t& mipMapCount, 
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    virtual void createTexture2D(const std::string& pathAsset_Tex, 
                                                 uint32_t& mipMapCount,
                                                 VkImage& image, 
                                                 VkDeviceMemory& imageMemory);
                    
                    virtual void createTexture2DArray(const std::vector<std::string>& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTexture2DArray(const std::vector<std::string>& aPathAsset_Tex, 
                                                      VkImageType type,
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTexture2DArray(const std::vector<std::string>& aPathAsset_Tex, 
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
                    
                    virtual void createTextureCubeMap(const std::vector<std::string>& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory,
                                                      VkBuffer& buffer, 
                                                      VkDeviceMemory& bufferMemory);
                    virtual void createTextureCubeMap(const std::vector<std::string>& aPathAsset_Tex, 
                                                      VkSampleCountFlagBits numSamples,
                                                      VkFormat format,
                                                      bool autoMipMap, 
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);
                    virtual void createTextureCubeMap(const std::vector<std::string>& aPathAsset_Tex,
                                                      uint32_t& mipMapCount, 
                                                      VkImage& image, 
                                                      VkDeviceMemory& imageMemory);

                    
                    virtual void createTextureRenderTarget1D(const glm::vec4& clDefault,
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
                    virtual void createTextureRenderTarget1D(const glm::vec4& clDefault,
                                                             bool isSetColor,
                                                             uint32_t width, 
                                                             uint32_t mipMapCount,
                                                             VkSampleCountFlagBits numSamples,
                                                             VkFormat format,
                                                             VkImageUsageFlags usage, 
                                                             bool isGraphicsComputeShared,
                                                             VkImage& image, 
                                                             VkDeviceMemory& imageMemory);
                    
                    virtual void createTextureRenderTarget2D(const glm::vec4& clDefault,
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
                    virtual void createTextureRenderTarget2D(const glm::vec4& clDefault,
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
                    
                    virtual void createTextureRenderTarget2DArray(const glm::vec4& clDefault,
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
                    virtual void createTextureRenderTarget2DArray(const glm::vec4& clDefault,
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

                    virtual void createTextureRenderTarget3D(const glm::vec4& clDefault,
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
                    virtual void createTextureRenderTarget3D(const glm::vec4& clDefault,
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
                    

                        virtual void createImage(uint32_t width, 
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
                        virtual void createImageView(VkImage image, 
                                                     VkImageViewType type, 
                                                     VkFormat format, 
                                                     VkImageAspectFlags aspectFlags, 
                                                     uint32_t mipMapCount,
                                                     uint32_t numArray,
                                                     VkImageView& imageView);
                                                     
                        virtual void createSampler(uint32_t mipMapCount, 
                                                   VkSampler& sampler);
                        virtual void createSampler(VulkanTextureFilterType eTextureFilter,
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


                virtual VkShaderModule createShaderModule(std::string info, std::string pathFile);
                virtual VkPipelineLayout createVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout);
                    virtual void destroyVkPipelineLayout(VkPipelineLayout vkPipelineLayout);
                    virtual void destroyVkPipeline(VkPipeline vkPipeline);

                virtual void preparePipeline();
                    virtual void createVkPipelineCache();
                    virtual void destroyVkPipelineCache(VkPipelineCache vkPipelineCache);
                    virtual void createCustomBeforePipeline();


                virtual void createGraphicsPipeline();
                    virtual void createGraphicsPipeline_Default();
                    virtual void createGraphicsPipeline_Custom();
                        virtual VkPipeline createVkGraphicsPipeline(VkShaderModule vertShaderModule, const std::string& vertMain,
                                                                    VkShaderModule fragShaderModule, const std::string& fragMain,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask);
                        virtual VkPipeline createVkGraphicsPipeline(VkShaderModule vertShaderModule, const std::string& vertMain,
                                                                    VkShaderModule tescShaderModule, const std::string& tescMain,
                                                                    VkShaderModule teseShaderModule, const std::string& teseMain,
                                                                    VkShaderModule fragShaderModule, const std::string& fragMain,
                                                                    VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                                    VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                                    VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                                    VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
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
                                                                    VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                                    VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                                    VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                                    VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                                    VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                                    VkColorComponentFlags colorWriteMask);

                virtual void createComputePipeline();
                    virtual void createComputePipeline_Default();
                    virtual void createComputePipeline_Custom();
                        virtual VkPipeline createVkComputePipeline(VkShaderModule compShaderModule,
                                                                   const std::string& compMain,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0);
                        virtual VkPipeline createVkComputePipeline(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                                   VkPipelineLayout pipelineLayout, 
                                                                   VkPipelineCreateFlags flags = 0);


                virtual void createDescriptor();
                    virtual void createDescriptorPool();

                        virtual void destroyVkDescriptorPool(VkDescriptorPool vkDescriptorPool);

                    virtual void createDescriptorSets_Default();
                    virtual void createDescriptorSets_Custom();
                        virtual void createDescriptorSet(VkDescriptorSet& descriptorSet, VkDescriptorSetLayout vkDescriptorSetLayout);
                        virtual void createDescriptorSets(std::vector<VkDescriptorSet>& aDescriptorSets, VkDescriptorSetLayout vkDescriptorSetLayout);
                        virtual void updateDescriptorSets(std::vector<VkDescriptorSet>& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler); 

                        virtual void destroyVkDescriptorSetLayout(VkDescriptorSetLayout vkDescriptorSetLayout);

                virtual void createCommandBuffers();
                    virtual void createCommandBuffer_Graphics();
                    virtual void createCommandBuffer_Compute();

            //Imgui
            virtual void createImgui();
                virtual void createImgui_DescriptorPool();
                virtual void createImgui_Init();

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
                virtual void updateSceneObjects();
                virtual void updateCBs_Pass();
                virtual void updateCBs_Objects();
                    virtual void updateCBs_ObjectsContent();
                virtual void updateCBs_Materials();
                    virtual void updateCBs_MaterialsContent();
                virtual void updateCBs_Instances();
                    virtual void updateCBs_InstancesContent();
                virtual void updateCBs_Custom();

                virtual void updateImgui();
                    virtual bool beginRenderImgui();
                        //Common
                        virtual void commonConfig();
                            virtual void commonOpConfig();
                        //Camera
                        virtual void cameraConfig();
                            virtual void cameraReset();
                        //Light
                        virtual void lightConfig();
                            virtual void lightConfigItem(LightConstants& lc, const std::string& name, int index, bool canChangeType);
                        //PassConstants
                        virtual void passConstantsConfig();
                        //Model
                        virtual void modelConfig();

                    virtual void endRenderImgui();

                virtual void updateRenderCommandBuffers_CustomBeforeDefault();
                virtual void updateRenderCommandBuffers_Default();
                    virtual void updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_Default(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault_Custom(VkCommandBuffer& commandBuffer);
                        virtual void drawMeshDefault_Imgui(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_CustomAfterDefault(VkCommandBuffer& commandBuffer);

                        virtual void beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                     const VkRenderPass& renderPass, 
                                                     const VkFramebuffer& frameBuffer,
                                                     const VkOffset2D& offset,
                                                     const VkExtent2D& extent,
                                                     const glm::vec4& clBg,
                                                     float depth,
                                                     uint32_t stencil);
                            virtual void bindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport);
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
            virtual void cleanupCustom();
            virtual void cleanupTexture();
            virtual void cleanupVertexIndexBuffer();

            virtual void cleanupSwapChain();
                virtual void cleanupSwapChain_Custom();
            virtual void recreateSwapChain();
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
                                            std::vector<const char*>& outInstanceLayers, 
                                            std::vector<const char*>& outInstanceExtensions);
        void getDeviceLayersAndExtensions(bool bIsEnableValidationLayers,
                                          std::vector<const char*>& outDeviceLayers, 
                                          std::vector<const char*>& outDeviceExtensions);
    };

}; //LostPeter

#endif