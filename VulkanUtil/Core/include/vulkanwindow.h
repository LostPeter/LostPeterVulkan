// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_WINDOW_H_
#define _VULKAN_WINDOW_H_

#include "vulkanbase.h"

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
        VkDevice poDevice;
        VkSampleCountFlagBits poMSAASamples;
        VkQueue poQueueGraphics;
        VkQueue poQueuePresent;

        VkSurfaceKHR poSurface;
        VkSwapchainKHR poSwapChain;
        VkViewport poViewport;
        VkRect2D poScissor;
        std::vector<VkImage> poSwapChainImages;
        VkFormat poSwapChainImageFormat;
        VkExtent2D poSwapChainExtent;
        std::vector<VkImageView> poSwapChainImageViews;
        std::vector<VkFramebuffer> poSwapChainFrameBuffers;
        VkImage poColorImage;
        VkDeviceMemory poColorImageMemory;
        VkImageView poColorImageView;
        VkImage poDepthImage;
        VkDeviceMemory poDepthImageMemory;
        VkImageView poDepthImageView;

        VkRenderPass poRenderPass;
        VkDescriptorSetLayout poDescriptorSetLayout;
        
        VkCommandPool poCommandPool;
        
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
        VkPipeline poPipelineGraphics;
        VkPipeline poPipelineGraphics_WireFrame;

        uint32_t poMipLevels;
        VkImage poTextureImage;
        VkDeviceMemory poTextureImageMemory;
        VkImageView poTextureImageView;
        VkSampler poTextureSampler;

        VkDescriptorPool poDescriptorPool;
        std::vector<VkDescriptorSet> poDescriptorSets;

        std::vector<VkCommandBuffer> poCommandBuffers;

        //Synchronization Objects
        std::vector<VkSemaphore> poImageAvailableSemaphores;
        std::vector<VkSemaphore> poRenderFinishedSemaphores;
        std::vector<VkFence> poInFlightFences;
        std::vector<VkFence> poImagesInFlight;
        size_t poCurrentFrame;
        uint32_t poSwapChainImageIndex;

        //Features 
        struct SwapChainSupportDetails 
        {
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
        };
        SwapChainSupportDetails swapChainSupport;

        uint32_t graphicsIndex;
        uint32_t presentIndex;
        
        bool framebufferResized;
        //Config
        glm::vec4 cfg_colorBackground;
        bool cfg_isMSAA;
        bool cfg_isImgui;
        bool cfg_isWireFrame;
        bool cfg_isRotate;
        bool cfg_isNegativeViewport;
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

        //Mouse
        glm::vec2 mousePosLast;
        bool mouseButtonDownLeft;
        bool mouseButtonDownRight;


    private:
        std::vector<const char*> validationLayers;
        std::vector<const char*> deviceExtensions; 


    public:
        virtual void OnInit();
        virtual void OnLoad();
        virtual bool OnIsInit();
        virtual void OnResize(int w, int h, bool force);
        virtual bool OnBeginRender();
            virtual void OnUpdate();
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
                virtual void createInstance();
                    virtual bool checkValidationLayerSupport();
                    virtual std::vector<const char*> getRequiredExtensions();
                    virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
                virtual void setUpDebugMessenger();
                virtual void createSurface();
                virtual void pickPhysicalDevice();
                    virtual void findQueueFamilies(VkPhysicalDevice device, int& indexGraphics, int& indexPresent);
                    virtual bool checkDeviceExtensionSupport(VkPhysicalDevice device);
                    virtual VkSampleCountFlagBits getMaxUsableSampleCount();
                    virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, SwapChainSupportDetails& details);
                    virtual bool isDeviceSuitable(VkPhysicalDevice device, int& indexGraphics, int& indexPresent);
                virtual void createLogicalDevice();

            virtual void createFeatureSupport();

            virtual void createCommandObjects();
                virtual void createCommandPool();
                    virtual VkCommandBuffer beginSingleTimeCommands();
                    virtual void endSingleTimeCommands(VkCommandBuffer commandBuffer);
            
            virtual void createSwapChainObjects();
                virtual void createSwapChain();
                    virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
                    virtual VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
                    virtual VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
                    virtual void createViewport();
                virtual void createSwapChainImageViews();
                    virtual VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
                    virtual void createColorResources();
                    virtual void createDepthResources();
                        virtual VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
                        virtual VkFormat findDepthFormat();
                        virtual bool hasStencilComponent(VkFormat format);

            virtual void createDescriptorObjects();
                virtual void createDescriptorSetLayout();

            virtual void createPipelineObjects();
                virtual void createRenderPass();
                    virtual void createRenderPass_KhrDepth();
                    virtual void createRenderPass_KhrDepthImgui();
                    virtual void createRenderPass_ColorDepthMSAA();
                    virtual void createRenderPass_ColorDepthImguiMSAA();
                virtual void createFramebuffers();

            virtual void createSyncObjects();

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
                    virtual void createVertexBuffer(size_t bufSize, void* pBuf, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory);
                    virtual void createIndexBuffer(size_t bufSize, void* pBuf, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory);
                        virtual void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
                        virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
                        virtual void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
                
                virtual void loadTexture();
                    virtual void createTextureImage(const std::string& pathAsset_Tex, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t& mipLevels);
                        virtual void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
                        virtual void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
                        virtual void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
                        virtual void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
                    virtual void createTextureImageView(VkImage& textureImage, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkImageView& textureImageView);
                    virtual void createTextureSampler(uint32_t mipLevels, VkSampler& textureSampler);
                
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

                virtual void createGraphicsPipeline();
                    virtual void createPipeline_Default();
                    virtual void createPipeline_Custom();
                        virtual VkPipelineLayout createVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout);
                        virtual VkShaderModule createShaderModule(std::string info, std::string pathFile);
                        virtual VkPipeline createVkPipeline(VkShaderModule vertShaderModule, const std::string& vertMain,
                                                            VkShaderModule fragShaderModule, const std::string& fragMain,
                                                            VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                            VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                            VkRenderPass renderPass, VkPipelineLayout pipelineLayout, std::vector<VkViewport> viewports, std::vector<VkRect2D> scissors,
                                                            VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                            VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                            VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                            VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                            VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                            VkColorComponentFlags colorWriteMask);

                virtual void createDescriptor();
                    virtual void createDescriptorPool();
                    virtual void createDescriptorSets_Default();
                    virtual void createDescriptorSets_Custom();
                        virtual void createDescriptorSets(std::vector<VkDescriptorSet>& aDescriptorSets);
                        virtual void updateDescriptorSets(std::vector<VkDescriptorSet>& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler);

                virtual void createCommandBuffers();

            //Imgui
            virtual void createImgui();
                virtual void createImgui_DescriptorPool();
                virtual void createImgui_Init();

        //Resize
        virtual void resizeWindow(int w, int h, bool force);

        //Render/Update
        virtual bool beginRender();
            virtual void update();
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
                        //Camera
                        virtual void cameraConfig();
                        virtual void cameraReset();

                    virtual void endRenderImgui();

                virtual void updateCommandBuffers();
                    virtual void updateRenderPass_Default(VkCommandBuffer& commandBuffer);
                    virtual void updateRenderPass_Custom(VkCommandBuffer& commandBuffer);
                        virtual void bindViewport(VkCommandBuffer& commandBuffer);
                        virtual void drawMesh(VkCommandBuffer& commandBuffer);
                        virtual void drawMesh_Custom(VkCommandBuffer& commandBuffer);
                        virtual void drawImgui(VkCommandBuffer& commandBuffer);

            virtual void render();
        virtual void endRender();

        //cleanup
        virtual void cleanup();
            virtual void cleanupCustom();
            virtual void cleanupTexture();
            virtual void cleanupVertexIndexBuffer();

            virtual void cleanupSwapChain();
            virtual void recreateSwapChain();

    private:
        VkResult createDebugUtilsMessengerEXT(VkInstance instance, 
                                              const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDebugUtilsMessengerEXT* pDebugMessenger);
        void destroyDebugUtilsMessengerEXT(VkInstance instance,
                                           VkDebugUtilsMessengerEXT debugMessenger,
                                           const VkAllocationCallbacks* pAllocator);
        
    };

}; //LostPeter

#endif