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

#include "../include/PreInclude.h"
#include "../include/VulkanWindow.h"
#include "../include/VulkanMeshLoader.h"
#include "../include/VulkanMeshGeometry.h"
#include "../include/VulkanCamera.h"
#include "../include/VulkanTimer.h"
#include "../include/VulkanUtilString.h"

namespace LostPeter
{
#if UTIL_DEBUG == 1
    bool VulkanWindow::s_isEnableValidationLayers = true;
#else
    bool VulkanWindow::s_isEnableValidationLayers = false;
#endif

    int VulkanWindow::s_maxFramesInFight = 2;

    VulkanWindow::VulkanWindow(int width, int height, String name)
        : VulkanBase(width, height, name)
        , poInstance(VK_NULL_HANDLE)
        , poDebugMessenger(VK_NULL_HANDLE)
        , poPhysicalDevice(VK_NULL_HANDLE)
        , poDeviceCreatepNextChain(VK_NULL_HANDLE)
        , poDevice(VK_NULL_HANDLE)
        , poMSAASamples(VK_SAMPLE_COUNT_1_BIT)
        , poQueueGraphics(VK_NULL_HANDLE)
        , poQueuePresent(VK_NULL_HANDLE)
        , poQueueCompute(VK_NULL_HANDLE)
        , poSurface(VK_NULL_HANDLE)
        , poSwapChain(VK_NULL_HANDLE)
        , poColorImage(VK_NULL_HANDLE)
        , poColorImageMemory(VK_NULL_HANDLE)
        , poColorImageView(VK_NULL_HANDLE)
        , poDepthImage(VK_NULL_HANDLE)
        , poDepthImageMemory(VK_NULL_HANDLE)
        , poDepthImageView(VK_NULL_HANDLE)
        , poRenderPass(VK_NULL_HANDLE)
        , poDescriptorSetLayout(VK_NULL_HANDLE)
        , poCommandPoolGraphics(VK_NULL_HANDLE) 
        , poCommandPoolCompute(VK_NULL_HANDLE)
        , poCommandBufferCompute(VK_NULL_HANDLE)

        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)
        , poVertexBuffer(VK_NULL_HANDLE)
        , poVertexBufferMemory(VK_NULL_HANDLE)
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)
        , poIndexBuffer(VK_NULL_HANDLE)
        , poIndexBufferMemory(VK_NULL_HANDLE)
        , poMatWorld(VulkanMath::Identity4x4())

        , poTypeVertex(Vulkan_Vertex_Pos3Color4Normal3Tangent3Tex2)
        , poPipelineLayout(VK_NULL_HANDLE)
        , poPipelineCache(VK_NULL_HANDLE)
        , poPipelineGraphics(VK_NULL_HANDLE)
        , poPipelineGraphics_WireFrame(VK_NULL_HANDLE)

        , poTextureImage(VK_NULL_HANDLE)
        , poTextureImageMemory(VK_NULL_HANDLE)
        , poTextureImageView(VK_NULL_HANDLE)
        , poTextureSampler(VK_NULL_HANDLE)

        , poDescriptorPool(VK_NULL_HANDLE)

        , poCurrentFrame(0)
        , poSwapChainImageIndex(0)
        , poGraphicsWaitSemaphore(VK_NULL_HANDLE)
        , poComputeWaitSemaphore(VK_NULL_HANDLE)

        , queueIndexGraphics(0)
        , queueIndexPresent(0)
        , queueIndexCompute(0)

        , isFrameBufferResized(false)

        , cfg_colorBackground(0.0f, 0.2f, 0.4f, 1.0f)
        , cfg_isMSAA(false)
        , cfg_isImgui(false)
        , cfg_isWireFrame(false)
        , cfg_isRotate(false)
        , cfg_isNegativeViewport(true)
        , cfg_isUseComputeShader(false)
        , cfg_isCreateRenderComputeSycSemaphore(false)
        , cfg_vkPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        , cfg_vkFrontFace(VK_FRONT_FACE_CLOCKWISE)
        , cfg_vkPolygonMode(VK_POLYGON_MODE_FILL)
        , cfg_vkCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
        , cfg_isDepthTest(VK_TRUE)
        , cfg_isDepthWrite(VK_TRUE)
        , cfg_DepthCompareOp(VK_COMPARE_OP_LESS_OR_EQUAL) 
        , cfg_isStencilTest(VK_FALSE)
        , cfg_isBlend(VK_FALSE)
        , cfg_BlendColorFactorSrc(VK_BLEND_FACTOR_ONE)
        , cfg_BlendColorFactorDst(VK_BLEND_FACTOR_ZERO)
        , cfg_BlendColorOp(VK_BLEND_OP_ADD)
        , cfg_BlendAlphaFactorSrc(VK_BLEND_FACTOR_ONE)
        , cfg_BlendAlphaFactorDst(VK_BLEND_FACTOR_ZERO)
        , cfg_BlendAlphaOp(VK_BLEND_OP_ADD)
        , cfg_ColorWriteMask(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)

        , cfg_cameraPos(0.0f, 0.0f, -5.0f)
        , cfg_cameraLookTarget(0.0f, 0.0f, 0.0f)
        , cfg_cameraUp(0.0f, 1.0f, 0.0f)
        , cfg_cameraFov(45.0f)
        , cfg_cameraNear(0.05f)
        , cfg_cameraFar(1000.0f)
        , cfg_cameraSpeedMove(1000.0f)
        , cfg_cameraSpeedZoom(0.1f)
        , cfg_cameraSpeedRotate(0.1f)

        , cfg_model_Path("")
        , cfg_shaderVertex_Path("")
        , cfg_shaderFragment_Path("")
        , cfg_texture_Path("")
        , cfg_terrain_Path("")
        , cfg_terrainShaderNormalMapGen_Path("Assets/Shader/standard_compute_texgen_normalmap.comp.spv")
        , cfg_terrainShaderVertex_Path("Assets/Shader/standard_terrain_opaque_lit.vert.spv")
        , cfg_terrainShaderFragment_Path("Assets/Shader/standard_terrain_opaque_lit.frag.spv")
        , cfg_terrainTextureDiffuse_Path("Assets/Texture/Terrain/shore_sand_albedo.png;Assets/Texture/Terrain/moss_albedo.png;Assets/Texture/Terrain/rock_cliff_albedo.png;Assets/Texture/Terrain/cliff_albedo.png")
        , cfg_terrainTextureNormal_Path("Assets/Texture/Terrain/shore_sand_norm.png;Assets/Texture/Terrain/moss_norm.tga;Assets/Texture/Terrain/rock_cliff_norm.tga;Assets/Texture/Terrain/cliff_norm.png")
        , cfg_terrainTextureControl_Path("Assets/Texture/Terrain/terrain_control.png")

        , imgui_IsEnable(false)
        , imgui_MinimalSwapchainImages(0)
        , imgui_DescriptorPool(VK_NULL_HANDLE)
        , imgui_PathIni("")
        , imgui_PathLog("")

        , pSceneManager(nullptr)

        , poTerrainHeightMapData(nullptr)
        , poTerrainHeightMapDataFloat(nullptr)
        , poTerrainHeightMapDataSize(0)
        , poTerrainHeightMapSize(0)
        , poTerrainGridVertexCount(129)
        , poTerrainGridInstanceVertexCount(17)
        , poTerrainGridInstanceCount(0)
        , poTerrainIsDrawInstance(false)
        , poTerrainVertexCount(0)
        , poTerrainVertexBuffer_Size(0)
        , poTerrainVertexBuffer_Data(nullptr)
        , poTerrainVertexBuffer(VK_NULL_HANDLE)
        , poTerrainVertexBufferMemory(VK_NULL_HANDLE)
        , poTerrainIndexCount(0)
        , poTerrainIndexBuffer_Size(0)
        , poTerrainIndexBuffer_Data(nullptr)
        , poTerrainIndexBuffer(VK_NULL_HANDLE)
        , poTerrainIndexBufferMemory(VK_NULL_HANDLE)

        , poTerrainHeightMapImage(VK_NULL_HANDLE)
        , poTerrainHeightMapImageMemory(VK_NULL_HANDLE)
        , poTerrainHeightMapImageView(VK_NULL_HANDLE)
        , poTerrainNormalMapImage(VK_NULL_HANDLE)
        , poTerrainNormalMapImageMemory(VK_NULL_HANDLE)
        , poTerrainNormalMapImageView(VK_NULL_HANDLE)
        , poTerrainImageSampler(VK_NULL_HANDLE)
        , poTerrainComputeShaderModuleNormalGen(VK_NULL_HANDLE)
        , poBuffer_TerrainTextureCopy(VK_NULL_HANDLE)
        , poBufferMemory_TerrainTextureCopy(VK_NULL_HANDLE)
        , poTerrainComputeDescriptorSetLayout(VK_NULL_HANDLE)
        , poTerrainComputePipelineLayout(VK_NULL_HANDLE)
        , poTerrainComputePipeline(VK_NULL_HANDLE)
        , poTerrainComputeDescriptorSet(VK_NULL_HANDLE)

        , poTerrainDiffuseImage(VK_NULL_HANDLE)
        , poTerrainDiffuseImageMemory(VK_NULL_HANDLE)
        , poTerrainDiffuseImageView(VK_NULL_HANDLE)
        , poTerrainDiffuseImageSampler(VK_NULL_HANDLE)
        , poTerrainNormalImage(VK_NULL_HANDLE)
        , poTerrainNormalImageMemory(VK_NULL_HANDLE)
        , poTerrainNormalImageView(VK_NULL_HANDLE)
        , poTerrainNormalImageSampler(VK_NULL_HANDLE)
        , poTerrainControlImage(VK_NULL_HANDLE)
        , poTerrainControlImageMemory(VK_NULL_HANDLE)
        , poTerrainControlImageView(VK_NULL_HANDLE)
        , poTerrainControlImageSampler(VK_NULL_HANDLE)
        , poBuffer_TerrainObjectCB(VK_NULL_HANDLE)
        , poBufferMemory_TerrainObjectCB(VK_NULL_HANDLE)
        , poTerrainGraphicsDescriptorSetLayout(VK_NULL_HANDLE)
        , poTerrainGraphicsPipelineLayout(VK_NULL_HANDLE)
        , poTerrainGraphicsShaderModuleVertex(VK_NULL_HANDLE)
        , poTerrainGraphicsShaderModuleFragment(VK_NULL_HANDLE)
        , poTerrainGraphicsPipeline(VK_NULL_HANDLE)
        , poTerrainGraphicsPipeline_WireFrame(VK_NULL_HANDLE)

        , pCamera(nullptr)
        , pCameraRight(nullptr)

        , mouseButtonDownLeft(false)
        , mouseButtonDownRight(false)
    {
        cfg_StencilOpFront.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        cfg_StencilOpFront.compareMask = 0;
        cfg_StencilOpFront.writeMask = 0;
        cfg_StencilOpFront.reference = 0;

        cfg_StencilOpBack.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.compareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
        cfg_StencilOpBack.compareMask = 0;
        cfg_StencilOpBack.writeMask = 0;
        cfg_StencilOpBack.reference = 0;
    }

    VulkanWindow::~VulkanWindow()
    {
        UTIL_DELETE(pCamera)
    }

    void VulkanWindow::OnInit()
    {
        createPipeline();
    }

    void VulkanWindow::OnLoad()
    {
        loadAssets();
    }

    bool VulkanWindow::OnIsInit()
    {
        return this->isCreateDevice &&
            this->isLoadAsset;
    }

    void VulkanWindow::OnResize(int w, int h, bool force)
    {
        resizeWindow(w, h, force);

        if (this->pCamera != nullptr)
        {
            this->pCamera->PerspectiveLH(glm::radians(this->cfg_cameraFov), this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
        }
    }

    bool VulkanWindow::OnBeginCompute()
    {
        return beginCompute();
    }
        void VulkanWindow::OnUpdateCompute()
        {
            updateCompute();
        }
        void VulkanWindow::OnCompute()
        {
            compute();
        }
    void VulkanWindow::OnEndCompute()
    {
        endCompute();
    }

    bool VulkanWindow::OnBeginRender()
    {
        return beginRender();
    }
        void VulkanWindow::OnUpdateRender()
        {
            updateRender();
        }
        void VulkanWindow::OnRender()
        {
            render();
        }
    void VulkanWindow::OnEndRender()
    {
        endRender();
    }

    void VulkanWindow::OnDestroy()
    {
        //1> Wait
        if (this->isCreateDevice)
        {
            vkDeviceWaitIdle(this->poDevice);

             //2> Cleanup
            cleanup();
        }
    }

    void VulkanWindow::OnMouseInput()
    {   
        double cursorX; double cursorY;
        glfwGetCursorPos(this->pWindow, &cursorX, &cursorY);


        //Mouse Left
        int actionLeft = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_LEFT);
        if (actionLeft == GLFW_PRESS)
        {
            if (!this->mouseButtonDownLeft)
            {
                OnMouseLeftDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_LEFT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->mouseButtonDownLeft)
            {
                OnMouseLeftUp(cursorX, cursorY);
            }
        }
        
        //Mouse Right
        int actionRight = glfwGetMouseButton(this->pWindow, GLFW_MOUSE_BUTTON_RIGHT);
        if (actionRight == GLFW_PRESS)
        {
            if (!this->mouseButtonDownRight)
            {
                OnMouseRightDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_RIGHT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->mouseButtonDownRight)
            {
                OnMouseRightUp(cursorX, cursorY);
            }
        }

        //Mouse Middle

    }
    void VulkanWindow::OnMouseLeftDown(double x, double y)
    {
        this->mouseButtonDownLeft = true;

    }
    void VulkanWindow::OnMouseLeftUp(double x, double y)
    {   
        this->mouseButtonDownLeft = false;

    }
    void VulkanWindow::OnMouseRightDown(double x, double y)
    {
        this->mouseButtonDownRight = true;

        this->mousePosLast.x = (float)x;
        this->mousePosLast.y = (float)y;
    }
    void VulkanWindow::OnMouseRightUp(double x, double y)
    {
        this->mouseButtonDownRight = false;

    }
    void VulkanWindow::OnMouseMove(int button, double x, double y)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            if (this->pCamera != nullptr)
            {
                float fX = static_cast<float>(x - this->mousePosLast.x);
                float fY = static_cast<float>(y - this->mousePosLast.y);
                if (fX != 0 || fY != 0)
                {
                    float fRotYAngle = fX * this->cfg_cameraSpeedRotate;
                    float fRotXAngle = fY * this->cfg_cameraSpeedRotate;
                    glm::vec3 vEulerAngles = pCamera->GetEulerAngles();
                    vEulerAngles.x += fRotXAngle;
                    vEulerAngles.y += fRotYAngle;
                    vEulerAngles.z = 0;
                    pCamera->SetEulerAngles(vEulerAngles);
                }
            }
            
            this->mousePosLast.x = (float)x;
            this->mousePosLast.y = (float)y;
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            
        }
    }
    void VulkanWindow::OnMouseWheel(double x, double y)
    {
        if (this->pCamera != nullptr)
        {
            float fDis = (float)(this->cfg_cameraSpeedZoom * y);
            this->pCamera->Walk(fDis);
        }
    }

    void VulkanWindow::OnKeyboardInput()
    {
        if (this->pCamera != nullptr)
        {
            float speedMove = this->cfg_cameraSpeedMove;
            if (glfwGetKey(this->pWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                glfwGetKey(this->pWindow, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) 
            {
                speedMove *= 10.0f;
            }

            float timeDelta = this->pTimer->GetTimeDelta();
            if (glfwGetKey(this->pWindow, GLFW_KEY_W) == GLFW_PRESS)
            {
                this->pCamera->Walk(speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_S) == GLFW_PRESS)
            {
                this->pCamera->Walk(- speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_A) == GLFW_PRESS)
            {
                this->pCamera->Strafe(- speedMove * timeDelta);
            }
            if (glfwGetKey(this->pWindow, GLFW_KEY_D) == GLFW_PRESS)
            {
                this->pCamera->Strafe(speedMove * timeDelta);
            }
            this->pCamera->UpdateViewMatrix();
        }
    }
    void VulkanWindow::OnKeyDown(int key)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            glfwSetWindowShouldClose(this->pWindow, true);
        }
        if (key == GLFW_KEY_R)
        {
            cameraReset();
        }
        if (key == GLFW_KEY_T)
        {
            this->cfg_isRotate = !this->cfg_isRotate;
        }
        if (key == GLFW_KEY_F)
        {
            this->cfg_isWireFrame = !this->cfg_isWireFrame;
        }
    }
    void VulkanWindow::OnKeyUp(int key)
    {

    }

    bool VulkanWindow::HasConfig_MASS()
    {
        return this->cfg_isMSAA;
    }
    bool VulkanWindow::HasConfig_Imgui()
    {
        return this->cfg_isImgui;
    }

    bool VulkanWindow::IsEnable_MASS()
    {
        return this->cfg_isMSAA &&
               this->poMSAASamples != VK_SAMPLE_COUNT_1_BIT;
    }
    bool VulkanWindow::IsEnable_Imgui()
    {
        return this->cfg_isImgui &&
               this->imgui_IsEnable;
    }

    void VulkanWindow::createPipeline()
    {   
        Util_LogInfo("**********<1> VulkanWindow::createPipeline start **********");
        {
            //1> Create Resize callback
            createWindowCallback();

            //2> Create Device
            createDevice();

            //3> Create Feature Support
            createFeatureSupport();

            //4> Create Command Objects
            createCommandObjects();

            //5> Create Swap Chain Objects
            createSwapChainObjects();

            //6> createDescriptorPool
            createDescriptorPool();

            //7> createDescriptorSetLayouts
            createDescriptorSetLayouts();
            
            //8> Create Pipeline Objects
            createPipelineObjects();

            //9> Create Sync Objects
            createSyncObjects();

            //10> isCreateDevice
            this->isCreateDevice = true;
        }
        Util_LogInfo("**********<1> VulkanWindow::createPipeline finish **********");
    }
    //glfw: whenever the window size changed (by OS or user resize) this callback function executes
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        VulkanWindow* pWnd = (VulkanWindow*)glfwGetWindowUserPointer(window);
        pWnd->isFrameBufferResized = true;
        pWnd->OnResize(width, height, false);
    }
    void VulkanWindow::createWindowCallback()
    {   
        glfwSetWindowUserPointer(this->pWindow, this);
        glfwSetFramebufferSizeCallback(this->pWindow, framebuffer_size_callback);

        Util_LogInfo("*****<1-1> VulkanWindow::createWindowCallback finish *****");
    }
    void VulkanWindow::createDevice()
    {
        Util_LogInfo("*****<1-2> VulkanWindow::createDevice start *****");
        {
            //1> createInstance
            createInstance();

            //2> setUpDebugMessenger
            setUpDebugMessenger();

            //3> createSurface
            createSurface();

            //4> pickPhysicalDevice
            pickPhysicalDevice();

            //5> createLogicalDevice
            createLogicalDevice();
        }
        Util_LogInfo("*****<1-2> VulkanWindow::createDevice finish *****");
    }
    void VulkanWindow::destroyVkDevice(VkDevice vkDevice)
    {
        if (vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(vkDevice, nullptr);
        }
    }
    void VulkanWindow::destroyVkInstance(VkInstance vkInstance)
    {
        if (vkInstance != VK_NULL_HANDLE)
        {
            vkDestroyInstance(vkInstance, nullptr);
        }
    }
    void VulkanWindow::createInstance()
    {
        getInstanceLayersAndExtensions(s_isEnableValidationLayers, 
                                       this->aInstanceLayers, 
                                       this->aInstanceExtensions);
        if (s_isEnableValidationLayers) 
        {
            this->aInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        ConstCharPtrVector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        size_t count_extensions = this->aInstanceExtensions.size();
        size_t count_glfw = extensions.size();
        for (size_t i = 0; i < count_glfw; i++)
        {
            bool isFind = false;
            for (size_t j = 0; j < count_extensions; j++)
            {   
                if (strcmp(extensions[i], this->aInstanceExtensions[j]) == 0)
                {
                    isFind = true;
                    break;
                }
            }

            if (!isFind)
            {
                this->aInstanceExtensions.push_back(extensions[i]);
            }
        }


        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "vulkan_xxx_samples";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "vulkan_xxx_samples";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(this->aInstanceExtensions.size());
        createInfo.ppEnabledExtensionNames = this->aInstanceExtensions.size() > 0 ? this->aInstanceExtensions.data() : nullptr;
        createInfo.enabledLayerCount = static_cast<uint32_t>(this->aInstanceLayers.size());
        createInfo.ppEnabledLayerNames = this->aInstanceLayers.size() > 0 ? this->aInstanceLayers.data() : nullptr;

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (s_isEnableValidationLayers) 
        {
            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } 

        VkResult result = vkCreateInstance(&createInfo, nullptr, &this->poInstance);
        if (result == VK_ERROR_INCOMPATIBLE_DRIVER) 
        {
            Util_LogError("*********************** VulkanWindow::createInstance: Cannot find a compatible Vulkan driver (ICD)");
        }
        else if (result == VK_ERROR_EXTENSION_NOT_PRESENT)
        {
            String missingExtensions;
            uint32 propertyCount = 0;
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
            std::vector<VkExtensionProperties> properties(propertyCount);
            vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, properties.data());

            for (const char* extension : this->aInstanceExtensions)
            {
                bool found = false;
                for (uint32 i = 0; i < propertyCount; ++i)
                {
                    const char* propExtension = properties[i].extensionName;
                    if (strcmp(propExtension, extension) == 0) 
                    {
                        found = true;
                        break;
                    }
                }
                if (!found) 
                {
                    String extensionStr(extension);
                    missingExtensions += extensionStr + "\n";
                }
            }

            Util_LogError("*********************** VulkanWindow::createInstance: Vulkan driver doesn't contain specified extensions: [%s] !", missingExtensions.c_str());
        }
        else if (result != VK_SUCCESS) 
        {
            Util_LogError("*********************** VulkanWindow::createInstance: Create vulkan instance failed !");
        }
        else 
        {
            Util_LogInfo("VulkanWindow::createInstance: Create vulkan instance successed !");
        }

        if (result != VK_SUCCESS)
        {
            String msg = "*********************** VulkanWindow::createInstance: Failed to create vulkan instance !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        Util_LogInfo("<1-2-1> VulkanWindow::createInstance finish !");
    }
        static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) 
        {
            Util_LogInfo("VulkanWindow.debugCallback: Validation layer: [%s] !", pCallbackData->pMessage);
            return VK_FALSE;
        }
        void VulkanWindow::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo) 
        {
            createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
            createInfo.pfnUserCallback = debugCallback;
        }

    void VulkanWindow::setUpDebugMessenger()
    {
        if (s_isEnableValidationLayers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            populateDebugMessengerCreateInfo(createInfo);
                
            if (createDebugUtilsMessengerEXT(this->poInstance, &createInfo, nullptr, &this->poDebugMessenger) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::setUpDebugMessenger: Failed to set up debug messenger !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        
        Util_LogInfo("<1-2-2> VulkanWindow::setUpDebugMessenger finish !");
    }

    void VulkanWindow::createSurface()
    {
        VkResult result = glfwCreateWindowSurface(this->poInstance, this->pWindow, nullptr, &this->poSurface);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            String msg = "*********************** VulkanWindow::createSurface: Failed to create window surface, result: " + os.str();
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        Util_LogInfo("<1-2-3> VulkanWindow::createSurface finish !");
    }

    void VulkanWindow::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            String msg = "*********************** VulkanWindow::pickPhysicalDevice: Failed to find GPUs width Vulkan support !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, devices.data());

        int count_device = (int)devices.size();
        for (int i = 0; i < count_device; i++)
        {
            VkPhysicalDevice& device = devices[i];
            int indexGraphics = -1;
            int indexPresent = -1;
            int indexCompute = -1;
            if (isDeviceSuitable(device, indexGraphics, indexPresent, indexCompute))
            {
                this->poPhysicalDevice = device;
                this->queueIndexGraphics = indexGraphics;
                this->queueIndexPresent = indexPresent;
                this->queueIndexCompute = indexCompute;

                if (HasConfig_MASS())
                {
                    this->poMSAASamples = getMaxUsableSampleCount();
                }
                break;
            }
        } 

        if (this->poPhysicalDevice == VK_NULL_HANDLE) 
        {
            String msg = "*********************** VulkanWindow::pickPhysicalDevice: Failed to find a suitable GPU !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        vkGetPhysicalDeviceProperties(this->poPhysicalDevice, &this->poPhysicalDeviceProperties);
        vkGetPhysicalDeviceFeatures(this->poPhysicalDevice, &this->poPhysicalDeviceFeatures);

        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceProperties ****************");
        {
            Util_LogInfo("  apiVersion: [%u]", this->poPhysicalDeviceProperties.apiVersion);
            Util_LogInfo("  driverVersion: [%u]", this->poPhysicalDeviceProperties.driverVersion);
            Util_LogInfo("  vendorID: [%u]", this->poPhysicalDeviceProperties.vendorID);
            Util_LogInfo("  deviceID: [%u]", this->poPhysicalDeviceProperties.deviceID);
            if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_OTHER)
                Util_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_OTHER");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
                Util_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
                Util_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU)
                Util_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU");
            else if (this->poPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_CPU)
                Util_LogInfo("  deviceType: VK_PHYSICAL_DEVICE_TYPE_CPU");
            Util_LogInfo("  deviceName: [%s] \n", this->poPhysicalDeviceProperties.deviceName);

            Util_LogInfo("  maxImageDimension1D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension1D);
            Util_LogInfo("  maxImageDimension2D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension2D);
            Util_LogInfo("  maxImageDimension3D: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimension3D);
            Util_LogInfo("  maxImageDimensionCube: [%u]", this->poPhysicalDeviceProperties.limits.maxImageDimensionCube);
            Util_LogInfo("  maxImageArrayLayers: [%u]", this->poPhysicalDeviceProperties.limits.maxImageArrayLayers);
            Util_LogInfo("  maxTexelBufferElements: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelBufferElements);
            Util_LogInfo("  maxUniformBufferRange: [%u]", this->poPhysicalDeviceProperties.limits.maxUniformBufferRange);
            Util_LogInfo("  maxStorageBufferRange: [%u]", this->poPhysicalDeviceProperties.limits.maxStorageBufferRange);
            Util_LogInfo("  maxPushConstantsSize: [%u]", this->poPhysicalDeviceProperties.limits.maxPushConstantsSize);
            Util_LogInfo("  maxMemoryAllocationCount: [%u]", this->poPhysicalDeviceProperties.limits.maxMemoryAllocationCount);
            Util_LogInfo("  maxSamplerAllocationCount: [%u]", this->poPhysicalDeviceProperties.limits.maxSamplerAllocationCount);
            Util_LogInfo("  bufferImageGranularity: [%u]", this->poPhysicalDeviceProperties.limits.bufferImageGranularity);
            Util_LogInfo("  sparseAddressSpaceSize: [%u]", this->poPhysicalDeviceProperties.limits.sparseAddressSpaceSize);
            Util_LogInfo("  maxBoundDescriptorSets: [%u]", this->poPhysicalDeviceProperties.limits.maxBoundDescriptorSets);
            Util_LogInfo("  maxPerStageDescriptorSamplers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorSamplers);
            Util_LogInfo("  maxPerStageDescriptorUniformBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorUniformBuffers);
            Util_LogInfo("  maxPerStageDescriptorStorageBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageBuffers);
            Util_LogInfo("  maxPerStageDescriptorSampledImages: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorSampledImages);
            Util_LogInfo("  maxPerStageDescriptorStorageImages: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorStorageImages);
            Util_LogInfo("  maxPerStageDescriptorInputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageDescriptorInputAttachments);
            Util_LogInfo("  maxPerStageResources: [%u]", this->poPhysicalDeviceProperties.limits.maxPerStageResources);
            Util_LogInfo("  maxDescriptorSetSamplers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetSamplers);
            Util_LogInfo("  maxDescriptorSetUniformBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetUniformBuffers);
            Util_LogInfo("  maxDescriptorSetUniformBuffersDynamic: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetUniformBuffersDynamic);
            Util_LogInfo("  maxDescriptorSetStorageBuffers: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageBuffers);
            Util_LogInfo("  maxDescriptorSetStorageBuffersDynamic: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageBuffersDynamic);
            Util_LogInfo("  maxDescriptorSetSampledImages: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetSampledImages);
            Util_LogInfo("  maxDescriptorSetStorageImages: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetStorageImages);
            Util_LogInfo("  maxDescriptorSetInputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxDescriptorSetInputAttachments);
            Util_LogInfo("  maxVertexInputAttributes: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputAttributes);
            Util_LogInfo("  maxVertexInputBindings: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputBindings);
            Util_LogInfo("  maxVertexInputAttributeOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputAttributeOffset);
            Util_LogInfo("  maxVertexInputBindingStride: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexInputBindingStride);
            Util_LogInfo("  maxVertexOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxVertexOutputComponents);
            Util_LogInfo("  maxTessellationGenerationLevel: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationGenerationLevel);
            Util_LogInfo("  maxTessellationPatchSize: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationPatchSize);
            Util_LogInfo("  maxTessellationControlPerVertexInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerVertexInputComponents);
            Util_LogInfo("  maxTessellationControlPerVertexOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerVertexOutputComponents);
            Util_LogInfo("  maxTessellationControlPerPatchOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlPerPatchOutputComponents);
            Util_LogInfo("  maxTessellationControlTotalOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationControlTotalOutputComponents);
            Util_LogInfo("  maxTessellationEvaluationInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationEvaluationInputComponents);
            Util_LogInfo("  maxTessellationEvaluationOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxTessellationEvaluationOutputComponents);
            Util_LogInfo("  maxGeometryShaderInvocations: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryShaderInvocations);
            Util_LogInfo("  maxGeometryInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryInputComponents);
            Util_LogInfo("  maxGeometryOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryOutputComponents);
            Util_LogInfo("  maxGeometryOutputVertices: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryOutputVertices);
            Util_LogInfo("  maxGeometryTotalOutputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxGeometryTotalOutputComponents);
            Util_LogInfo("  maxFragmentInputComponents: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentInputComponents);
            Util_LogInfo("  maxFragmentOutputAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentOutputAttachments);
            Util_LogInfo("  maxFragmentDualSrcAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentDualSrcAttachments);
            Util_LogInfo("  maxFragmentCombinedOutputResources: [%u]", this->poPhysicalDeviceProperties.limits.maxFragmentCombinedOutputResources);
            Util_LogInfo("  maxComputeSharedMemorySize: [%u]", this->poPhysicalDeviceProperties.limits.maxComputeSharedMemorySize);
            Util_LogInfo("  maxComputeWorkGroupCount: [%u] - [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[0], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[1], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupCount[2]);
            Util_LogInfo("  maxComputeWorkGroupInvocations: [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupInvocations);
            Util_LogInfo("  maxComputeWorkGroupSize: [%u] - [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[0], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[1], this->poPhysicalDeviceProperties.limits.maxComputeWorkGroupSize[2]);
            Util_LogInfo("  subPixelPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.subPixelPrecisionBits);
            Util_LogInfo("  subTexelPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.subTexelPrecisionBits);
            Util_LogInfo("  mipmapPrecisionBits: [%u]", this->poPhysicalDeviceProperties.limits.mipmapPrecisionBits);
            Util_LogInfo("  maxDrawIndexedIndexValue: [%u]", this->poPhysicalDeviceProperties.limits.maxDrawIndexedIndexValue);
            Util_LogInfo("  maxDrawIndirectCount: [%u]", this->poPhysicalDeviceProperties.limits.maxDrawIndirectCount);
            Util_LogInfo("  maxSamplerLodBias: [%f]", this->poPhysicalDeviceProperties.limits.maxSamplerLodBias);
            Util_LogInfo("  maxSamplerAnisotropy: [%f]", this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy);
            Util_LogInfo("  maxViewports: [%u]", this->poPhysicalDeviceProperties.limits.maxViewports);
            Util_LogInfo("  maxViewportDimensions: [%u] - [%u]", this->poPhysicalDeviceProperties.limits.maxViewportDimensions[0], this->poPhysicalDeviceProperties.limits.maxViewportDimensions[1]);
            Util_LogInfo("  viewportBoundsRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.viewportBoundsRange[0], this->poPhysicalDeviceProperties.limits.viewportBoundsRange[1]);
            Util_LogInfo("  viewportSubPixelBits: [%u]", this->poPhysicalDeviceProperties.limits.viewportSubPixelBits);
            Util_LogInfo("  minMemoryMapAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minMemoryMapAlignment);
            Util_LogInfo("  minTexelBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minTexelBufferOffsetAlignment);
            Util_LogInfo("  minUniformBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minUniformBufferOffsetAlignment);
            Util_LogInfo("  minStorageBufferOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.minStorageBufferOffsetAlignment);
            Util_LogInfo("  minTexelOffset: [%d]", this->poPhysicalDeviceProperties.limits.minTexelOffset);
            Util_LogInfo("  maxTexelOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelOffset);
            Util_LogInfo("  minTexelGatherOffset: [%d]", this->poPhysicalDeviceProperties.limits.minTexelGatherOffset);
            Util_LogInfo("  maxTexelGatherOffset: [%u]", this->poPhysicalDeviceProperties.limits.maxTexelGatherOffset);
            Util_LogInfo("  minInterpolationOffset: [%f]", this->poPhysicalDeviceProperties.limits.minInterpolationOffset);
            Util_LogInfo("  maxInterpolationOffset: [%f]", this->poPhysicalDeviceProperties.limits.maxInterpolationOffset);
            Util_LogInfo("  subPixelInterpolationOffsetBits: [%u]", this->poPhysicalDeviceProperties.limits.subPixelInterpolationOffsetBits);
            Util_LogInfo("  maxFramebufferWidth: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferWidth);
            Util_LogInfo("  maxFramebufferHeight: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferHeight);
            Util_LogInfo("  maxFramebufferLayers: [%u]", this->poPhysicalDeviceProperties.limits.maxFramebufferLayers);
            Util_LogInfo("  framebufferColorSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferColorSampleCounts);
            Util_LogInfo("  framebufferDepthSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferDepthSampleCounts);
            Util_LogInfo("  framebufferStencilSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferStencilSampleCounts);
            Util_LogInfo("  framebufferNoAttachmentsSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.framebufferNoAttachmentsSampleCounts);
            Util_LogInfo("  maxColorAttachments: [%u]", this->poPhysicalDeviceProperties.limits.maxColorAttachments);
            Util_LogInfo("  sampledImageColorSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageColorSampleCounts);
            Util_LogInfo("  sampledImageIntegerSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageIntegerSampleCounts);
            Util_LogInfo("  sampledImageDepthSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageDepthSampleCounts);
            Util_LogInfo("  sampledImageStencilSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.sampledImageStencilSampleCounts);
            Util_LogInfo("  storageImageSampleCounts: [%u]", this->poPhysicalDeviceProperties.limits.storageImageSampleCounts);
            Util_LogInfo("  maxSampleMaskWords: [%u]", this->poPhysicalDeviceProperties.limits.maxSampleMaskWords);
            Util_LogInfo("  timestampComputeAndGraphics: [%s]", this->poPhysicalDeviceProperties.limits.timestampComputeAndGraphics ? "true" : "false");
            Util_LogInfo("  timestampPeriod: [%f]", this->poPhysicalDeviceProperties.limits.timestampPeriod);
            Util_LogInfo("  maxClipDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxClipDistances);
            Util_LogInfo("  maxCullDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxCullDistances);
            Util_LogInfo("  maxCombinedClipAndCullDistances: [%u]", this->poPhysicalDeviceProperties.limits.maxCombinedClipAndCullDistances);
            Util_LogInfo("  discreteQueuePriorities: [%u]", this->poPhysicalDeviceProperties.limits.discreteQueuePriorities);
            Util_LogInfo("  pointSizeRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.pointSizeRange[0], this->poPhysicalDeviceProperties.limits.pointSizeRange[1]);
            Util_LogInfo("  lineWidthRange: [%f] - [%f]", this->poPhysicalDeviceProperties.limits.lineWidthRange[0], this->poPhysicalDeviceProperties.limits.lineWidthRange[1]);
            Util_LogInfo("  pointSizeGranularity: [%f]", this->poPhysicalDeviceProperties.limits.pointSizeGranularity);
            Util_LogInfo("  lineWidthGranularity: [%f]", this->poPhysicalDeviceProperties.limits.lineWidthGranularity);
            Util_LogInfo("  strictLines: [%s]", this->poPhysicalDeviceProperties.limits.strictLines ? "true" : "false");
            Util_LogInfo("  standardSampleLocations: [%s]", this->poPhysicalDeviceProperties.limits.standardSampleLocations ? "true" : "false");
            Util_LogInfo("  optimalBufferCopyOffsetAlignment: [%u]", this->poPhysicalDeviceProperties.limits.optimalBufferCopyOffsetAlignment);
            Util_LogInfo("  optimalBufferCopyRowPitchAlignment: [%u]", this->poPhysicalDeviceProperties.limits.optimalBufferCopyRowPitchAlignment);
            Util_LogInfo("  nonCoherentAtomSize: [%u]", this->poPhysicalDeviceProperties.limits.nonCoherentAtomSize);
        }
        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceProperties ****************");

        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceFeatures ****************");
        {
            Util_LogInfo("  robustBufferAccess: [%s]", this->poPhysicalDeviceFeatures.robustBufferAccess ? "true" : "false");
            Util_LogInfo("  fullDrawIndexUint32: [%s]", this->poPhysicalDeviceFeatures.fullDrawIndexUint32 ? "true" : "false");
            Util_LogInfo("  imageCubeArray: [%s]", this->poPhysicalDeviceFeatures.imageCubeArray ? "true" : "false");
            Util_LogInfo("  independentBlend: [%s]", this->poPhysicalDeviceFeatures.independentBlend ? "true" : "false");
            Util_LogInfo("  geometryShader: [%s]", this->poPhysicalDeviceFeatures.geometryShader ? "true" : "false");
            Util_LogInfo("  tessellationShader: [%s]", this->poPhysicalDeviceFeatures.tessellationShader ? "true" : "false");
            Util_LogInfo("  sampleRateShading: [%s]", this->poPhysicalDeviceFeatures.sampleRateShading ? "true" : "false");
            Util_LogInfo("  dualSrcBlend: [%s]", this->poPhysicalDeviceFeatures.dualSrcBlend ? "true" : "false");
            Util_LogInfo("  logicOp: [%s]", this->poPhysicalDeviceFeatures.logicOp ? "true" : "false");
            Util_LogInfo("  multiDrawIndirect: [%s]", this->poPhysicalDeviceFeatures.multiDrawIndirect ? "true" : "false");
            Util_LogInfo("  drawIndirectFirstInstance: [%s]", this->poPhysicalDeviceFeatures.drawIndirectFirstInstance ? "true" : "false");
            Util_LogInfo("  depthClamp: [%s]", this->poPhysicalDeviceFeatures.depthClamp ? "true" : "false");
            Util_LogInfo("  depthBiasClamp: [%s]", this->poPhysicalDeviceFeatures.depthBiasClamp ? "true" : "false");
            Util_LogInfo("  fillModeNonSolid: [%s]", this->poPhysicalDeviceFeatures.fillModeNonSolid ? "true" : "false");
            Util_LogInfo("  depthBounds: [%s]", this->poPhysicalDeviceFeatures.depthBounds ? "true" : "false");
            Util_LogInfo("  wideLines: [%s]", this->poPhysicalDeviceFeatures.wideLines ? "true" : "false");
            Util_LogInfo("  largePoints: [%s]", this->poPhysicalDeviceFeatures.largePoints ? "true" : "false");
            Util_LogInfo("  alphaToOne: [%s]", this->poPhysicalDeviceFeatures.alphaToOne ? "true" : "false");
            Util_LogInfo("  multiViewport: [%s]", this->poPhysicalDeviceFeatures.multiViewport ? "true" : "false");
            Util_LogInfo("  samplerAnisotropy: [%s]", this->poPhysicalDeviceFeatures.samplerAnisotropy ? "true" : "false");
            Util_LogInfo("  textureCompressionETC2: [%s]", this->poPhysicalDeviceFeatures.textureCompressionETC2 ? "true" : "false");
            Util_LogInfo("  textureCompressionASTC_LDR: [%s]", this->poPhysicalDeviceFeatures.textureCompressionASTC_LDR ? "true" : "false");
            Util_LogInfo("  textureCompressionBC: [%s]", this->poPhysicalDeviceFeatures.textureCompressionBC ? "true" : "false");
            Util_LogInfo("  occlusionQueryPrecise: [%s]", this->poPhysicalDeviceFeatures.occlusionQueryPrecise ? "true" : "false");
            Util_LogInfo("  pipelineStatisticsQuery: [%s]", this->poPhysicalDeviceFeatures.pipelineStatisticsQuery ? "true" : "false");
            Util_LogInfo("  vertexPipelineStoresAndAtomics: [%s]", this->poPhysicalDeviceFeatures.vertexPipelineStoresAndAtomics ? "true" : "false");
            Util_LogInfo("  fragmentStoresAndAtomics: [%s]", this->poPhysicalDeviceFeatures.fragmentStoresAndAtomics ? "true" : "false");
            Util_LogInfo("  shaderTessellationAndGeometryPointSize: [%s]", this->poPhysicalDeviceFeatures.shaderTessellationAndGeometryPointSize ? "true" : "false");
            Util_LogInfo("  shaderImageGatherExtended: [%s]", this->poPhysicalDeviceFeatures.shaderImageGatherExtended ? "true" : "false");
            Util_LogInfo("  shaderStorageImageExtendedFormats: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageExtendedFormats ? "true" : "false");
            Util_LogInfo("  shaderStorageImageMultisample: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageMultisample ? "true" : "false");
            Util_LogInfo("  shaderStorageImageReadWithoutFormat: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageReadWithoutFormat ? "true" : "false");
            Util_LogInfo("  shaderStorageImageWriteWithoutFormat: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageWriteWithoutFormat ? "true" : "false");
            Util_LogInfo("  shaderUniformBufferArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing ? "true" : "false");
            Util_LogInfo("  shaderSampledImageArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing ? "true" : "false");
            Util_LogInfo("  shaderStorageBufferArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing ? "true" : "false");
            Util_LogInfo("  shaderStorageImageArrayDynamicIndexing: [%s]", this->poPhysicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing ? "true" : "false");
            Util_LogInfo("  shaderClipDistance: [%s]", this->poPhysicalDeviceFeatures.shaderClipDistance ? "true" : "false");
            Util_LogInfo("  shaderCullDistance: [%s]", this->poPhysicalDeviceFeatures.shaderCullDistance ? "true" : "false");
            Util_LogInfo("  shaderFloat64: [%s]", this->poPhysicalDeviceFeatures.shaderFloat64 ? "true" : "false");
            Util_LogInfo("  shaderInt64: [%s]", this->poPhysicalDeviceFeatures.shaderInt64 ? "true" : "false");
            Util_LogInfo("  shaderInt16: [%s]", this->poPhysicalDeviceFeatures.shaderInt16 ? "true" : "false");
            Util_LogInfo("  shaderResourceResidency: [%s]", this->poPhysicalDeviceFeatures.shaderResourceResidency ? "true" : "false");
            Util_LogInfo("  shaderResourceMinLod: [%s]", this->poPhysicalDeviceFeatures.shaderResourceMinLod ? "true" : "false");
            Util_LogInfo("  sparseBinding: [%s]", this->poPhysicalDeviceFeatures.sparseBinding ? "true" : "false");
            Util_LogInfo("  sparseResidencyBuffer: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyBuffer ? "true" : "false");
            Util_LogInfo("  sparseResidencyImage2D: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyImage2D ? "true" : "false");
            Util_LogInfo("  sparseResidencyImage3D: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyImage3D ? "true" : "false");
            Util_LogInfo("  sparseResidency2Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency2Samples ? "true" : "false");
            Util_LogInfo("  sparseResidency4Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency4Samples ? "true" : "false");
            Util_LogInfo("  sparseResidency8Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency8Samples ? "true" : "false");
            Util_LogInfo("  sparseResidency16Samples: [%s]", this->poPhysicalDeviceFeatures.sparseResidency16Samples ? "true" : "false");
            Util_LogInfo("  sparseResidencyAliased: [%s]", this->poPhysicalDeviceFeatures.sparseResidencyAliased ? "true" : "false");
            Util_LogInfo("  variableMultisampleRate: [%s]", this->poPhysicalDeviceFeatures.variableMultisampleRate ? "true" : "false");
            Util_LogInfo("  inheritedQueries: [%s]", this->poPhysicalDeviceFeatures.inheritedQueries ? "true" : "false");
        }       
        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: PhysicalDeviceFeatures ****************");

        this->poPhysicalDeviceMultiViewFeaturesKHR = {};
        this->poPhysicalDeviceMultiViewFeaturesKHR .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_FEATURES_KHR;
        this->poPhysicalDeviceFeatures2KHR = {};
        this->poPhysicalDeviceFeatures2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2_KHR;
		this->poPhysicalDeviceFeatures2KHR.pNext = &this->poPhysicalDeviceMultiViewFeaturesKHR;
        PFN_vkGetPhysicalDeviceFeatures2KHR pFuncGetPhysicalDeviceFeatures2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures2KHR>(vkGetInstanceProcAddr(this->poInstance, "vkGetPhysicalDeviceFeatures2KHR"));
        if (pFuncGetPhysicalDeviceFeatures2KHR != nullptr)
        {
            pFuncGetPhysicalDeviceFeatures2KHR(this->poPhysicalDevice, &this->poPhysicalDeviceFeatures2KHR);
        }

        this->poPhysicalDeviceMultiViewPropertiesKHR = {};
        this->poPhysicalDeviceMultiViewPropertiesKHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MULTIVIEW_PROPERTIES_KHR;
        this->poPhysicalDeviceProperties2KHR = {};
		this->poPhysicalDeviceProperties2KHR.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2_KHR;
		this->poPhysicalDeviceProperties2KHR.pNext = &this->poPhysicalDeviceMultiViewPropertiesKHR;
        PFN_vkGetPhysicalDeviceProperties2KHR pFuncGetPhysicalDeviceProperties2KHR = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2KHR>(vkGetInstanceProcAddr(this->poInstance, "vkGetPhysicalDeviceProperties2KHR"));
        if (pFuncGetPhysicalDeviceProperties2KHR != nullptr)
        {
            pFuncGetPhysicalDeviceProperties2KHR(this->poPhysicalDevice, &this->poPhysicalDeviceProperties2KHR);
        }

        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: MultiViewFeatures ****************");
        {
            if (pFuncGetPhysicalDeviceFeatures2KHR != nullptr)
            {
                Util_LogInfo("  multiview: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiview ? "true" : "false");
                Util_LogInfo("  multiviewGeometryShader: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiviewGeometryShader ? "true" : "false");
                Util_LogInfo("  multiviewTessellationShader: [%s]", this->poPhysicalDeviceMultiViewFeaturesKHR.multiviewTessellationShader ? "true" : "false");
            }
            if (pFuncGetPhysicalDeviceProperties2KHR != nullptr)
            {   
                Util_LogInfo("  maxMultiviewViewCount: [%u]", this->poPhysicalDeviceMultiViewPropertiesKHR.maxMultiviewViewCount);
                Util_LogInfo("  maxMultiviewInstanceIndex: [%u]", this->poPhysicalDeviceMultiViewPropertiesKHR.maxMultiviewInstanceIndex);
            }
        }
        Util_LogInfo("**************** VulkanWindow::pickPhysicalDevice: MultiViewFeatures ****************");

        Util_LogInfo("<1-2-4> VulkanWindow::pickPhysicalDevice finish !");
    }
        void VulkanWindow::findQueueFamilies(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute)
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int count_device = (int)queueFamilies.size();
            for (int i = 0; i < count_device; i++)
            {
                VkQueueFamilyProperties& queueFamily = queueFamilies[i];

                //Graphics
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) 
                {
                    indexGraphics = i;
                }

                //Present
                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->poSurface, &presentSupport);
                if (presentSupport)
                {
                    indexPresent = i;
                }

                //Compute
                if (this->cfg_isUseComputeShader)
                {
                    if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT)
                    {
                        indexCompute = i;
                    }

                    if (indexGraphics >= 0 && indexPresent >= 0 && indexCompute >= 0)
                        break;
                }
                else
                {
                    if (indexGraphics >= 0 && indexPresent >= 0)
                        break;
                }
            }
        }
        VkSampleCountFlagBits VulkanWindow::getMaxUsableSampleCount()
        {
            VkSampleCountFlags counts = this->poPhysicalDeviceProperties.limits.framebufferColorSampleCounts & this->poPhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
            if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
            if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
            if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
            if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
            if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
            if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

            return VK_SAMPLE_COUNT_1_BIT;
        }
        VulkanWindow::SwapChainSupportDetails VulkanWindow::querySwapChainSupport(VkPhysicalDevice device, VulkanWindow::SwapChainSupportDetails& details)
        {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->poSurface, &details.capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->poSurface, &formatCount, nullptr);

            if (formatCount != 0)
            {
                details.formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->poSurface, &formatCount, details.formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->poSurface, &presentModeCount, nullptr);

            if (presentModeCount != 0)
            {
                details.presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->poSurface, &presentModeCount, details.presentModes.data());
            }

            return details;
        }
        bool VulkanWindow::isDeviceSuitable(VkPhysicalDevice device, int& indexGraphics, int& indexPresent, int& indexCompute)
        {   
            findQueueFamilies(device, indexGraphics, indexPresent, indexCompute);
            if (this->cfg_isUseComputeShader)
            {
                if (indexGraphics == -1 || indexPresent == -1 || indexCompute == -1)
                    return false;
            }
            else
            {
                if (indexGraphics == -1 || indexPresent == -1)
                    return false;
            }
            
            bool swapChainAdequate = false;
            querySwapChainSupport(device, this->swapChainSupport);
            swapChainAdequate = !this->swapChainSupport.formats.empty() && !this->swapChainSupport.presentModes.empty();
            return swapChainAdequate;
        }

    void VulkanWindow::setUpEnabledFeatures()
    {
        this->poPhysicalEnabledFeatures = {};

        if (this->poPhysicalDeviceFeatures.samplerAnisotropy)
            this->poPhysicalEnabledFeatures.samplerAnisotropy = VK_TRUE;
        if (this->poPhysicalDeviceFeatures.fillModeNonSolid)
            this->poPhysicalEnabledFeatures.fillModeNonSolid = VK_TRUE;

        if (this->poPhysicalDeviceFeatures.tessellationShader)
            this->poPhysicalEnabledFeatures.tessellationShader = VK_TRUE;
        if (this->poPhysicalDeviceFeatures.multiDrawIndirect)
            this->poPhysicalEnabledFeatures.multiDrawIndirect = VK_TRUE;
    }
    void VulkanWindow::createLogicalDevice()
    {
        getDeviceLayersAndExtensions(s_isEnableValidationLayers,
                                     this->aDeviceLayers,
                                     this->aDeviceExtensions);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.insert(this->queueIndexGraphics);
        uniqueQueueFamilies.insert(this->queueIndexPresent);
        if (this->cfg_isUseComputeShader)
            uniqueQueueFamilies.insert(this->queueIndexCompute);

        float queuePriority = 1.0f;
        for (std::set<uint32_t>::iterator it = uniqueQueueFamilies.begin(); 
             it != uniqueQueueFamilies.end(); ++it)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = *it;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        setUpEnabledFeatures();
    
        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
        if (s_isEnableValidationLayers) 
        {
            deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(this->aDeviceLayers.size());
            deviceCreateInfo.ppEnabledLayerNames = this->aDeviceLayers.size() > 0 ? this->aDeviceLayers.data() : nullptr;
        } 
        else 
        {
            deviceCreateInfo.enabledLayerCount = 0;
            deviceCreateInfo.ppEnabledLayerNames = nullptr;
        }
        deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(this->aDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = this->aDeviceExtensions.size() > 0 ? this->aDeviceExtensions.data() : nullptr;
        if (this->poDeviceCreatepNextChain == nullptr)
        {
            deviceCreateInfo.pEnabledFeatures = &this->poPhysicalEnabledFeatures;
        }   
        else
        {
            this->poPhysicalDeviceFeatures2 = {};
            this->poPhysicalDeviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
			this->poPhysicalDeviceFeatures2.features = this->poPhysicalEnabledFeatures;
			this->poPhysicalDeviceFeatures2.pNext = this->poDeviceCreatepNextChain;
			deviceCreateInfo.pEnabledFeatures = nullptr;
			deviceCreateInfo.pNext = &this->poPhysicalDeviceFeatures2;
        }

        if (vkCreateDevice(this->poPhysicalDevice, &deviceCreateInfo, nullptr, &this->poDevice) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createLogicalDevice: Failed to create logical device !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        vkGetDeviceQueue(this->poDevice, this->queueIndexGraphics, 0, &this->poQueueGraphics);
        vkGetDeviceQueue(this->poDevice, this->queueIndexPresent, 0, &this->poQueuePresent);
        if (this->cfg_isUseComputeShader)
            vkGetDeviceQueue(this->poDevice, this->queueIndexCompute, 0, &this->poQueueCompute);

        Util_LogInfo("<1-2-5> VulkanWindow::createLogicalDevice finish !");
    }

    void VulkanWindow::createFeatureSupport()
    {

        Util_LogInfo("*****<1-3> VulkanWindow::createFeatureSupport finish *****");
    }

    void VulkanWindow::createCommandObjects()
    {
        Util_LogInfo("*****<1-4> VulkanWindow::createCommandObjects start *****");
        {
            //1> createCommandPool
            createCommandPool();

        }
        Util_LogInfo("*****<1-4> VulkanWindow::createCommandObjects finish *****");
    }
    void VulkanWindow::createCommandPool()
    {
        //1> poCommandPoolGraphics
        createCommandPool_Graphics();
        Util_LogInfo("<1-4-1> VulkanWindow::createCommandPool: Create CommandPoolGraphics success !");

        //2> poCommandPoolCompute
        if (this->cfg_isUseComputeShader)
        {
            createCommandPool_Compute();
            Util_LogInfo("<1-4-2> VulkanWindow::createCommandPool: Create CommandPoolCompute success !");
        }

        Util_LogInfo("<1-4> VulkanWindow::createCommandPool finish, create CommandPoolGraphics: [true], create CommandPoolCompute: [%s]", this->cfg_isUseComputeShader ? "true" : "false");
    }
        void VulkanWindow::createCommandPool_Graphics()
        {
            VkCommandPoolCreateInfo poolGraphicsInfo = {};
            poolGraphicsInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolGraphicsInfo.queueFamilyIndex = this->queueIndexGraphics;
            poolGraphicsInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            if (vkCreateCommandPool(this->poDevice, &poolGraphicsInfo, nullptr, &this->poCommandPoolGraphics) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandPool_Graphics: Failed to create command pool graphics !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        void VulkanWindow::createCommandPool_Compute()
        {
            VkCommandPoolCreateInfo poolComputeInfo = {};
            poolComputeInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
            poolComputeInfo.queueFamilyIndex = this->queueIndexCompute;
            poolComputeInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

            if (vkCreateCommandPool(this->poDevice, &poolComputeInfo, nullptr, &this->poCommandPoolCompute) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandPool_Compute: Failed to create command pool compute !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

            void VulkanWindow::destroyVkCommandPool(VkCommandPool vkCommandPool)
            {
                if (vkCommandPool != VK_NULL_HANDLE)
                {
                    vkDestroyCommandPool(this->poDevice, vkCommandPool, nullptr);
                }
            }
            void VulkanWindow::freeCommandBuffers(VkCommandPool commandPool,  uint32_t count, VkCommandBuffer* pCommandBuffer)
            {
                if (pCommandBuffer != nullptr)
                {
                    vkFreeCommandBuffers(this->poDevice, commandPool, count, pCommandBuffer);
                }
            }

        VkCommandBuffer VulkanWindow::beginSingleTimeCommands() 
        {
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandPool = this->poCommandPoolGraphics;
            allocInfo.commandBufferCount = 1;

            VkCommandBuffer commandBuffer = VK_NULL_HANDLE;
            vkAllocateCommandBuffers(this->poDevice, &allocInfo, &commandBuffer);

            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

            vkBeginCommandBuffer(commandBuffer, &beginInfo);

            return commandBuffer;
        }
        void VulkanWindow::endSingleTimeCommands(VkCommandBuffer commandBuffer) 
        {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, nullptr);
            vkQueueWaitIdle(this->poQueueGraphics);

            freeCommandBuffers(this->poCommandPoolGraphics, 1, &commandBuffer);
        }

    void VulkanWindow::createSwapChainObjects()
    {
        Util_LogInfo("*****<1-5> VulkanWindow::createSwapChainObjects start *****");
        {
            //1> createSwapChain
            createSwapChain();

            //2> createSwapChainImageViews
            createSwapChainImageViews();

            //3> createColorResources
            if (HasConfig_MASS())
            {
                createColorResources();
            }

            //4> createDepthResources
            createDepthResources();
        }
        Util_LogInfo("*****<1-5> VulkanWindow::createSwapChainObjects finish *****");
    }
    void VulkanWindow::createSwapChain()
    {
        querySwapChainSupport(this->poPhysicalDevice, this->swapChainSupport);
        this->imgui_MinimalSwapchainImages = this->swapChainSupport.capabilities.minImageCount + 1;

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(this->swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(this->swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(this->swapChainSupport.capabilities);

        uint32_t imageCount = this->swapChainSupport.capabilities.minImageCount + 1;
        if (this->swapChainSupport.capabilities.maxImageCount > 0 && imageCount > this->swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = this->swapChainSupport.capabilities.maxImageCount;
        }
        Util_LogInfo("**************** image count: [%d]", (int)imageCount);

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = this->poSurface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] =
        {
            this->queueIndexGraphics,
            this->queueIndexPresent
        };

        if (this->queueIndexGraphics != this->queueIndexPresent)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform = this->swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;

        VkResult result = vkCreateSwapchainKHR(this->poDevice, &createInfo, nullptr, &this->poSwapChain);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            String msg = "*********************** VulkanWindow::createSwapChain: Failed to create swap chain, result: " + os.str();
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, nullptr);
        this->poSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, this->poSwapChainImages.data());

        this->poSwapChainImageFormat = surfaceFormat.format;
        this->poSwapChainExtent = extent;
        this->poDepthImageFormat = findDepthFormat();

        int width, height;
        glfwGetFramebufferSize(this->pWindow, &width, &height);
        Util_LogInfo("<1-5-1> VulkanWindow::createSwapChain finish, Swapchain size: [%d,%d], window size: [%d,%d]", 
                    (int)extent.width, (int)extent.height, (int)width, (int)height);

        createViewport();
    }
        VkSurfaceFormatKHR VulkanWindow::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) 
        {
            int count = (int)availableFormats.size();
            for (int i = 0; i < count; i++)
            {
                const VkSurfaceFormatKHR& availableFormat = availableFormats[i];
                if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && 
                    availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
                {
                    return availableFormat;
                }
            }
            return availableFormats[0];
        }
        VkPresentModeKHR VulkanWindow::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) 
        {
            int count = (int)availablePresentModes.size();
            for (int i = 0; i < count; i++)
            {
                const VkPresentModeKHR& availablePresentMode = availablePresentModes[i];
                if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
                {
                    return availablePresentMode;
                }
            }
            return VK_PRESENT_MODE_FIFO_KHR;
        }
        VkExtent2D VulkanWindow::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities) 
        {
            if (capabilities.currentExtent.width != UINT32_MAX) 
            {
                return capabilities.currentExtent;
            } 
            else 
            {
                int width, height;
                glfwGetFramebufferSize(this->pWindow, &width, &height);

                VkExtent2D actualExtent = 
                {
                    static_cast<uint32_t>(width),
                    static_cast<uint32_t>(height)
                };

                actualExtent.width = VulkanMath::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = VulkanMath::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

                return actualExtent;
            }
        }
        void VulkanWindow::createViewport()
        {
            this->poViewport = {};
            this->poViewport.x = 0.0f;
            this->poViewport.y = 0.0f;
            this->poViewport.width = (float)this->poSwapChainExtent.width;
            this->poViewport.height = (float)this->poSwapChainExtent.height;
            this->poViewport.minDepth = 0.0f;
            this->poViewport.maxDepth = 1.0f;

            this->poScissor = {};
            this->poOffset.x = 0;
            this->poOffset.y = 0;
            this->poExtent = this->poSwapChainExtent;
            this->poScissor.offset = this->poOffset;
            this->poScissor.extent = this->poExtent;
        }

    void VulkanWindow::createSwapChainImageViews()
    {
        int count = (int)this->poSwapChainImages.size();
        this->poSwapChainImageViews.resize(count);

        for (int i = 0; i < count; i++)
        {
            createVkImageView(this->poSwapChainImages[i], 
                              VK_IMAGE_VIEW_TYPE_2D,
                              this->poSwapChainImageFormat, 
                              VK_IMAGE_ASPECT_COLOR_BIT, 
                              1,
                              1,
                              this->poSwapChainImageViews[i]);
        }

        Util_LogInfo("<1-5-2> VulkanWindow::createSwapChainImageViews finish !");
    }
        void VulkanWindow::createColorResources()
        {
            VkFormat colorFormat = this->poSwapChainImageFormat;

            createVkImage(this->poSwapChainExtent.width, 
                          this->poSwapChainExtent.height, 
                          1,
                          1,
                          1,
                          VK_IMAGE_TYPE_2D, 
                          false,
                          this->poMSAASamples, 
                          colorFormat, 
                          VK_IMAGE_TILING_OPTIMAL, 
                          VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
                          VK_SHARING_MODE_EXCLUSIVE,
                          false,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                          this->poColorImage, 
                          this->poColorImageMemory);

            createVkImageView(this->poColorImage, 
                              VK_IMAGE_VIEW_TYPE_2D,
                              colorFormat, 
                              VK_IMAGE_ASPECT_COLOR_BIT, 
                              1, 
                              1,
                              this->poColorImageView);

            Util_LogInfo("<1-5-3> VulkanWindow::createColorResources finish !");
        }
        void VulkanWindow::createDepthResources()
        {
            VkFormat depthFormat = this->poDepthImageFormat;

            createVkImage(this->poSwapChainExtent.width, 
                          this->poSwapChainExtent.height, 
                          1, 
                          1,
                          1,
                          VK_IMAGE_TYPE_2D, 
                          false,
                          this->poMSAASamples, 
                          depthFormat, 
                          VK_IMAGE_TILING_OPTIMAL, 
                          VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
                          VK_SHARING_MODE_EXCLUSIVE,
                          false,
                          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                          this->poDepthImage, 
                          this->poDepthImageMemory);

            createVkImageView(this->poDepthImage, 
                              VK_IMAGE_VIEW_TYPE_2D,
                              depthFormat, 
                              VK_IMAGE_ASPECT_DEPTH_BIT, 
                              1,
                              1,
                              this->poDepthImageView);

            Util_LogInfo("<1-5-4> VulkanWindow::createDepthResources finish !");
        }
        VkFormat VulkanWindow::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) 
        {
            size_t count = candidates.size();
            for (int i = 0; i < count; i++) 
            {
                VkFormat format = candidates[i];
                VkFormatProperties props;
                vkGetPhysicalDeviceFormatProperties(this->poPhysicalDevice, format, &props);

                if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
                {
                    return format;
                } 
                else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
                {
                    return format;
                }
            }

            String msg = "*********************** VulkanWindow::findSupportedFormat: Failed to find supported format !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        VkFormat VulkanWindow::findDepthFormat() 
        {
            std::vector<VkFormat> candidates;
            candidates.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
            candidates.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
            candidates.push_back(VK_FORMAT_D16_UNORM_S8_UINT);
            return findSupportedFormat(candidates,
                                       VK_IMAGE_TILING_OPTIMAL,
                                       VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        }
        bool VulkanWindow::hasStencilComponent(VkFormat format)
        {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || 
                format == VK_FORMAT_D24_UNORM_S8_UINT ||
                format == VK_FORMAT_D16_UNORM_S8_UINT;
        }

        void VulkanWindow::destroyVkSurfaceKHR(VkSurfaceKHR vkSurfaceKHR)
        {
            if (vkSurfaceKHR != VK_NULL_HANDLE)
            {
                vkDestroySurfaceKHR(this->poInstance, vkSurfaceKHR, nullptr);
            }
        }
        void VulkanWindow::destroyVkSwapchainKHR(VkSwapchainKHR vkSwapchainKHR)
        {
            if (vkSwapchainKHR != VK_NULL_HANDLE)
            {
                vkDestroySwapchainKHR(this->poDevice, vkSwapchainKHR, nullptr);
            }
        }

    void VulkanWindow::createDescriptorPool()
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * UTIL_ARRAYSIZE(pool_sizes);
        poolInfo.poolSizeCount = (uint32_t)UTIL_ARRAYSIZE(pool_sizes);
        poolInfo.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(this->poDevice, &poolInfo, nullptr, &this->poDescriptorPool) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createDescriptorPool: Failed to create descriptor pool !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        Util_LogInfo("<1-6> VulkanWindow::createDescriptorPool finish !");
    }
        void VulkanWindow::destroyVkDescriptorPool(VkDescriptorPool vkDescriptorPool)
        {
            if (vkDescriptorPool != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorPool(this->poDevice, vkDescriptorPool, nullptr);
            }
        }

    void VulkanWindow::createDescriptorSetLayouts()
    {
        Util_LogInfo("*****<1-7> VulkanWindow::createDescriptorSetLayouts start *****");
        {
            //1> createDescriptorSetLayout_Default
            createDescriptorSetLayout_Default();
            Util_LogInfo("<1-7-1> VulkanWindow::createDescriptorSetLayouts finish !");

            //3> createDescriptorSetLayout_Custom
            createDescriptorSetLayout_Custom();
            Util_LogInfo("<1-7-2> VulkanWindow::createDescriptorSetLayouts finish !");
        }
        Util_LogInfo("*****<1-7> VulkanWindow::createDescriptorSetLayouts finish *****");
    }
    void VulkanWindow::createDescriptorSetLayout_Default()
    {   
        if (this->cfg_shaderVertex_Path.empty() ||
            this->cfg_shaderFragment_Path.empty())
        {
            return;
        }

        //0> PassConstants
        VkDescriptorSetLayoutBinding passMainLayoutBinding = {};
        passMainLayoutBinding.binding = 0;
        passMainLayoutBinding.descriptorCount = 1;
        passMainLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        passMainLayoutBinding.pImmutableSamplers = nullptr;
        passMainLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        //1> ObjectConstants
        VkDescriptorSetLayoutBinding objectLayoutBinding = {};
        objectLayoutBinding.binding = 1;
        objectLayoutBinding.descriptorCount = 1;
        objectLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        objectLayoutBinding.pImmutableSamplers = nullptr;
        objectLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        //2> MaterialConstants
        VkDescriptorSetLayoutBinding materialLayoutBinding = {};
        materialLayoutBinding.binding = 2;
        materialLayoutBinding.descriptorCount = 1;
        materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        materialLayoutBinding.pImmutableSamplers = nullptr;
        materialLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        //3> InstanceConstants
        VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
        instanceLayoutBinding.binding = 3;
        instanceLayoutBinding.descriptorCount = 1;
        instanceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        instanceLayoutBinding.pImmutableSamplers = nullptr;
        instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

        //4> Texture Sampler
        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 4;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        VkDescriptorSetLayoutBindingVector bindings;
        bindings.push_back(passMainLayoutBinding);
        bindings.push_back(objectLayoutBinding);
        bindings.push_back(materialLayoutBinding);
        bindings.push_back(instanceLayoutBinding);
        if (!this->cfg_texture_Path.empty())
        {
            bindings.push_back(samplerLayoutBinding);
        }
        
        if (!createVkDescriptorSetLayout(bindings, this->poDescriptorSetLayout))
        {
            String msg = "*********************** VulkanWindow::createDescriptorSetLayout_Default: Failed to create descriptor set layout !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanWindow::createDescriptorSetLayout_Custom()
    {
        
    }

    void VulkanWindow::createPipelineObjects()
    {
        Util_LogInfo("*****<1-8> VulkanWindow::createPipelineObjects start *****");
        {
            //1> createRenderPasses
            createRenderPasses();
            Util_LogInfo("<1-8-1> VulkanWindow::createPipelineObjects: Success to create RenderPasses !");

            //2> createFramebuffers
            createFramebuffers();
            Util_LogInfo("<1-8-2> VulkanWindow::createPipelineObjects: Success to create Framebuffers !");
        }
        Util_LogInfo("*****<1-8> VulkanWindow::createPipelineObjects finish *****");
    }
    void VulkanWindow::createRenderPasses()
    {
        //1> createRenderPass_Default
        createRenderPass_Default();

        //2> createRenderPass_Custom
        createRenderPass_Custom();
    }
        void VulkanWindow::createRenderPass_Default()
        {
            if (HasConfig_Imgui())
            {
                if (HasConfig_MASS())
                {
                    createRenderPass_ColorDepthImguiMSAA(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poMSAASamples, this->poRenderPass);
                }
                else
                {
                    createRenderPass_KhrDepthImgui(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poRenderPass);
                }
            }
            else
            {
                if (HasConfig_MASS())
                {
                    createRenderPass_ColorDepthMSAA(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poSwapChainImageFormat, this->poMSAASamples, this->poRenderPass);
                }
                else
                {
                    createRenderPass_KhrDepth(this->poSwapChainImageFormat, this->poDepthImageFormat, this->poRenderPass);
                }
            }

            Util_LogInfo("VulkanWindow::createRenderPass_Default: Success to create RenderPass_Default !");
        }
        void VulkanWindow::createRenderPass_Custom()
        {
            
        }
            void VulkanWindow::createAttachmentDescription(VkAttachmentDescription& attachment,
                                                           VkAttachmentDescriptionFlags flags,
                                                           VkFormat format,
                                                           VkSampleCountFlagBits samples,
                                                           VkAttachmentLoadOp loadOp,
                                                           VkAttachmentStoreOp storeOp,
                                                           VkAttachmentLoadOp stencilLoadOp,
                                                           VkAttachmentStoreOp stencilStoreOp,
                                                           VkImageLayout initialLayout,
                                                           VkImageLayout finalLayout)
            {
                attachment.flags = flags;
                attachment.format = format;
                attachment.samples = samples;
                attachment.loadOp = loadOp;
                attachment.storeOp = storeOp;
                attachment.stencilLoadOp = stencilLoadOp;
                attachment.stencilStoreOp = stencilStoreOp;
                attachment.initialLayout = initialLayout;
                attachment.finalLayout = finalLayout;
            }
            bool VulkanWindow::createVkRenderPass(const String& nameRenderPass,
                                                  const VkAttachmentDescriptionVector& aAttachmentDescription,
                                                  const VkSubpassDescriptionVector& aSubpassDescription,
                                                  const VkSubpassDependencyVector& aSubpassDependency,
                                                  VkRenderPassMultiviewCreateInfo* pMultiviewCI,
                                                  VkRenderPass& vkRenderPass)
            {
                VkRenderPassCreateInfo renderPassInfo = {};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
                renderPassInfo.attachmentCount = static_cast<uint32_t>(aAttachmentDescription.size());
                renderPassInfo.pAttachments = &aAttachmentDescription[0];
                renderPassInfo.subpassCount = static_cast<uint32_t>(aSubpassDescription.size());
                renderPassInfo.pSubpasses = &aSubpassDescription[0];
                renderPassInfo.dependencyCount = static_cast<uint32_t>(aSubpassDependency.size());
                renderPassInfo.pDependencies = &aSubpassDependency[0];
                if (pMultiviewCI != nullptr)
                    renderPassInfo.pNext = pMultiviewCI;

                if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &vkRenderPass) != VK_SUCCESS)
                {
                    Util_LogError("*********************** VulkanWindow::createVkRenderPass: vkCreateRenderPass failed: [%s] !", nameRenderPass.c_str());
                    return false;
                }

                Util_LogInfo("VulkanWindow::createVkRenderPass: vkCreateRenderPass success: [%s] !", nameRenderPass.c_str());
                return true;
            }
            void VulkanWindow::destroyVkRenderPass(VkRenderPass vkRenderPass)
            {
                if (vkRenderPass != VK_NULL_HANDLE)
                {
                    vkDestroyRenderPass(this->poDevice, vkRenderPass, nullptr);   
                }
            }
            void VulkanWindow::createRenderPass_KhrDepth(VkFormat formatSwapChain, VkFormat formatDepth, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                    
                //3> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                aSubpassDescription.push_back(subpass_SceneRender);
                
                //4> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //5> createVkRenderPass
                if (!createVkRenderPass("RenderPass_Default_KhrDepth",
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_KhrDepth: Failed to create RenderPass_Default_KhrDepth !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                Util_LogInfo("VulkanWindow::createRenderPass_KhrDepth: Success to create RenderPass_Default_KhrDepth !");
            }
            void VulkanWindow::createRenderPass_KhrDepthImgui(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                    
                //3> Attachment Imgui Color
                VkAttachmentDescription attachmentImgui_Color = {};
                createAttachmentDescription(attachmentImgui_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_LOAD,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentImgui_Color);
                
                //4> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                aSubpassDescription.push_back(subpass_SceneRender);

                //5> Subpass Imgui
                VkAttachmentReference attachRef_ImguiColor = {};
                attachRef_ImguiColor.attachment = 0;
                attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiDepth = {};
                attachRef_ImguiDepth.attachment = 1;
                attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_Imgui = {};
                subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_Imgui.colorAttachmentCount = 1;
                subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
                subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
                aSubpassDescription.push_back(subpass_Imgui);
                
                //6> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //7> Subpass Dependency Imgui
                VkSubpassDependency subpassDependency_Imgui = {};
                subpassDependency_Imgui.srcSubpass = 0;
                subpassDependency_Imgui.dstSubpass = 1;
                subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_Imgui);

                //8> createVkRenderPass
                if (!createVkRenderPass("RenderPass_Default_KhrDepthImgui",
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_KhrDepthImgui: Failed to create RenderPass_Default_KhrDepthImgui !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                Util_LogInfo("VulkanWindow::createRenderPass_KhrDepthImgui: Success to create RenderPass_Default_KhrDepthImgui !");
            }
            void VulkanWindow::createRenderPass_ColorDepthMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                
                //3> Attachment SceneRender Color Resolve
                VkAttachmentDescription attachmentSR_ColorResolve = {};
                createAttachmentDescription(attachmentSR_ColorResolve,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentSR_ColorResolve);
                
                //4> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ColorResolve = {};
                attachRef_ColorResolve.attachment = 2;
                attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
                aSubpassDescription.push_back(subpass_SceneRender);
                
                //5> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //6> createVkRenderPass
                if (!createVkRenderPass("RenderPass_Default_ColorDepthMSAA",
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_ColorDepthMSAA: Failed to create RenderPass_Default_ColorDepthMSAA !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                Util_LogInfo("VulkanWindow::createRenderPass_ColorDepthMSAA: Success to create RenderPass_Default_ColorDepthMSAA !");
            }
            void VulkanWindow::createRenderPass_ColorDepthImguiMSAA(VkFormat formatColor, VkFormat formatDepth, VkFormat formatSwapChain, VkSampleCountFlagBits samples, VkRenderPass& vkRenderPass)
            {
                std::vector<VkAttachmentDescription> aAttachmentDescription;
                std::vector<VkSubpassDescription> aSubpassDescription;
                std::vector<VkSubpassDependency> aSubpassDependency;

                //1> Attachment SceneRender Color
                VkAttachmentDescription attachmentSR_Color = {};
                createAttachmentDescription(attachmentSR_Color,
                                            0,
                                            formatColor,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Color);
                
                //2> Attachment SceneRender Depth
                VkAttachmentDescription attachmentSR_Depth = {};
                createAttachmentDescription(attachmentSR_Depth,
                                            0,
                                            formatDepth,
                                            samples,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_CLEAR,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_Depth);
                
                //3> Attachment SceneRender Color Resolve
                VkAttachmentDescription attachmentSR_ColorResolve = {};
                createAttachmentDescription(attachmentSR_ColorResolve,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_UNDEFINED,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL);
                aAttachmentDescription.push_back(attachmentSR_ColorResolve);
                
                //4> Attachment Imgui Color
                VkAttachmentDescription attachmentImgui_Color = {};
                createAttachmentDescription(attachmentImgui_Color,
                                            0,
                                            formatSwapChain,
                                            VK_SAMPLE_COUNT_1_BIT,
                                            VK_ATTACHMENT_LOAD_OP_LOAD,
                                            VK_ATTACHMENT_STORE_OP_STORE,
                                            VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                            VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                            VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                            VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
                aAttachmentDescription.push_back(attachmentImgui_Color);
                
                //5> Subpass SceneRender
                VkAttachmentReference attachRef_Color = {};
                attachRef_Color.attachment = 0;
                attachRef_Color.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_Depth = {};
                attachRef_Depth.attachment = 1;
                attachRef_Depth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ColorResolve = {};
                attachRef_ColorResolve.attachment = 2;
                attachRef_ColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_SceneRender = {};
                subpass_SceneRender.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_SceneRender.colorAttachmentCount = 1;
                subpass_SceneRender.pColorAttachments = &attachRef_Color;
                subpass_SceneRender.pDepthStencilAttachment = &attachRef_Depth;
                subpass_SceneRender.pResolveAttachments = &attachRef_ColorResolve;
                aSubpassDescription.push_back(subpass_SceneRender);

                //6> Subpass Imgui
                VkAttachmentReference attachRef_ImguiColor = {};
                attachRef_ImguiColor.attachment = 0;
                attachRef_ImguiColor.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiDepth = {};
                attachRef_ImguiDepth.attachment = 1;
                attachRef_ImguiDepth.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

                VkAttachmentReference attachRef_ImguiColorResolve = {};
                attachRef_ImguiColorResolve.attachment = 2;
                attachRef_ImguiColorResolve.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

                VkSubpassDescription subpass_Imgui = {};
                subpass_Imgui.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
                subpass_Imgui.colorAttachmentCount = 1;
                subpass_Imgui.pColorAttachments = &attachRef_ImguiColor;
                subpass_Imgui.pDepthStencilAttachment = &attachRef_ImguiDepth;
                subpass_Imgui.pResolveAttachments = &attachRef_ImguiColorResolve;
                aSubpassDescription.push_back(subpass_Imgui);
                
                //7> Subpass Dependency SceneRender
                VkSubpassDependency subpassDependency_SceneRender = {};
                subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
                subpassDependency_SceneRender.dstSubpass = 0;
                subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
                subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_SceneRender.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
                subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_SceneRender.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
                aSubpassDependency.push_back(subpassDependency_SceneRender);

                //8> Subpass Dependency Imgui
                VkSubpassDependency subpassDependency_Imgui = {};
                subpassDependency_Imgui.srcSubpass = 0;
                subpassDependency_Imgui.dstSubpass = 1;
                subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                aSubpassDependency.push_back(subpassDependency_Imgui);

                //9> createVkRenderPass
                if (!createVkRenderPass("RenderPass_Default_ColorDepthImguiMSAA",
                                        aAttachmentDescription,
                                        aSubpassDescription,
                                        aSubpassDependency,
                                        nullptr,
                                        vkRenderPass))
                {
                    String msg = "*********************** VulkanWindow::createRenderPass_ColorDepthImguiMSAA: Failed to create RenderPass_Default_ColorDepthImguiMSAA !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                Util_LogInfo("VulkanWindow::createRenderPass_ColorDepthImguiMSAA: Success to create RenderPass_Default_ColorDepthImguiMSAA !");
            }

        void VulkanWindow::createFramebuffers()
        {
            //1> createFramebuffer_Default
            createFramebuffer_Default();

            //2> createFramebuffer_Custom
            createFramebuffer_Custom();
        }
            void VulkanWindow::createFramebuffer_Default()
            {
                size_t count = this->poSwapChainImageViews.size();
                this->poSwapChainFrameBuffers.resize(count);

                for (size_t i = 0; i < count; i++)
                {
                    VkImageViewVector aImageViews;
                    if (!HasConfig_Imgui())
                    {
                        if (!HasConfig_MASS())
                        {
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                            aImageViews.push_back(this->poDepthImageView);
                        }
                        else
                        {
                            aImageViews.push_back(this->poColorImageView);
                            aImageViews.push_back(this->poDepthImageView);
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                        }
                    }
                    else
                    {
                        if (!HasConfig_MASS())
                        {
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                            aImageViews.push_back(this->poDepthImageView);
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                        }
                        else
                        {
                            aImageViews.push_back(this->poColorImageView);
                            aImageViews.push_back(this->poDepthImageView);
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                            aImageViews.push_back(this->poSwapChainImageViews[i]);
                        }
                    }

                    String nameFramebuffer = "Framebuffer-" + VulkanUtilString::SaveSizeT(i);
                    if (!createVkFramebuffer(nameFramebuffer,
                                             aImageViews,
                                             this->poRenderPass,
                                             0,
                                             this->poSwapChainExtent.width,
                                             this->poSwapChainExtent.height,
                                             1,
                                             this->poSwapChainFrameBuffers[i]))
                    {
                        String msg = "*********************** VulkanWindow::createFramebuffer_Default: Failed to create framebuffer: " + nameFramebuffer;
                        Util_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                }

                Util_LogInfo("VulkanWindow::createFramebuffer_Default: Success to create Framebuffer_Default !");
            }
            void VulkanWindow::createFramebuffer_Custom()
            {
                
            }

                bool VulkanWindow::createVkFramebuffer(const String& nameFramebuffer,
                                                       const VkImageViewVector& aImageView, 
                                                       VkRenderPass& vkRenderPass,
                                                       VkFramebufferCreateFlags flags,
                                                       uint32_t width,
                                                       uint32_t height,
                                                       uint32_t layers,
                                                       VkFramebuffer& vkFramebuffer)
                {
                    VkFramebufferCreateInfo framebufferInfo = {};
                    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
                    framebufferInfo.renderPass = vkRenderPass;
                    framebufferInfo.attachmentCount = static_cast<uint32_t>(aImageView.size());
                    framebufferInfo.pAttachments = &aImageView[0];
                    framebufferInfo.width = width;
                    framebufferInfo.height = height;
                    framebufferInfo.layers = layers;

                    if (vkCreateFramebuffer(this->poDevice, &framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) 
                    {
                        Util_LogError("*********************** VulkanWindow::createVkFramebuffer: vkCreateFramebuffer failed: [%s] !", nameFramebuffer.c_str());
                        return false;
                    }

                    Util_LogInfo("VulkanWindow::createVkFramebuffer: vkCreateFramebuffer success: [%s] !", nameFramebuffer.c_str());
                    return true;
                }
                void VulkanWindow::destroyVkFramebuffer(VkFramebuffer vkFramebuffer)
                {
                    if (vkFramebuffer != VK_NULL_HANDLE)
                    {
                        vkDestroyFramebuffer(this->poDevice, vkFramebuffer, nullptr);
                    }
                }

    void VulkanWindow::createSyncObjects()
    {
        //1> Present/Render VkSemaphore/VkFence 
        createPresentRenderSyncObjects();

        //2> Graphics/Compute Sync VkSemaphore
        if (this->cfg_isCreateRenderComputeSycSemaphore)
        {
            createRenderComputeSyncObjects();
        }

        Util_LogInfo("*****<1-9> VulkanWindow::createSyncObjects finish *****");
    }
        void VulkanWindow::createPresentRenderSyncObjects()
        {
            this->poPresentCompleteSemaphores.resize(s_maxFramesInFight);
            this->poRenderCompleteSemaphores.resize(s_maxFramesInFight);
            this->poInFlightFences.resize(s_maxFramesInFight);
            this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            VkFenceCreateInfo fenceInfo = {};
            fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
            fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

            for (size_t i = 0; i < s_maxFramesInFight; i++) 
            {
                if (vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poPresentCompleteSemaphores[i]) != VK_SUCCESS ||
                    vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poRenderCompleteSemaphores[i]) != VK_SUCCESS ||
                    vkCreateFence(this->poDevice, &fenceInfo, nullptr, &this->poInFlightFences[i]) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createPresentRenderSyncObjects: Failed to create present/render synchronization objects for a frame !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
            
            Util_LogInfo("<1-9-1> VulkanWindow::createPresentRenderSyncObjects finish !");
        }
        void VulkanWindow::createRenderComputeSyncObjects()
        {
            VkSemaphoreCreateInfo semaphoreInfo = {};
            semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

            //1> poGraphicsWaitSemaphore
            if (vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poGraphicsWaitSemaphore) != VK_SUCCESS)
            {
                String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create GraphicsWaitSemaphore !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            //Signal the semaphore
            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &this->poGraphicsWaitSemaphore;
            vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, nullptr);
            vkQueueWaitIdle(this->poQueueGraphics);

            //2> poComputeWaitSemaphore
            if (vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poComputeWaitSemaphore) != VK_SUCCESS)
            {
                String msg = "*********************** VulkanWindow::createRenderComputeSyncObjects: Failed to create ComputeWaitSemaphore!";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            Util_LogInfo("<1-9-2> VulkanWindow::createRenderComputeSyncObjects finish !");
        }

            void VulkanWindow::destroyVkFence(VkFence vkFence)
            {
                if (vkFence != VK_NULL_HANDLE)
                {
                    vkDestroyFence(this->poDevice, vkFence, nullptr);
                }
            }
            void VulkanWindow::destroyVkSemaphore(VkSemaphore vkSemaphore)
            {   
                if (vkSemaphore != VK_NULL_HANDLE)
                {
                    vkDestroySemaphore(this->poDevice, vkSemaphore, nullptr);
                }
            }

    void VulkanWindow::loadAssets()
    {
        Util_LogInfo("**********<2> VulkanWindow::loadAssets start **********");
        {
            //1> Create Scene/Terrain/Camera
            createScene();
            createTerrain();
            createCamera();

            //2> - 3> 
            if (this->pSceneManager == nullptr)
            {
                //2> loadGeometry
                loadGeometry();
            }
            else
            {
                //3> Build Scene
                buildScene();
            }
            
            //4> Imgui
            if (HasConfig_Imgui())
            {
                createImgui();
            }

            this->isLoadAsset = true;
        }
        Util_LogInfo("**********<2> VulkanWindow::loadAssets finish **********");
    }
    void VulkanWindow::createScene()
    {
        Util_LogInfo("*****<2-1-1> VulkanWindow::createScene start *****");
        {
            //1> createSceneManager
            createSceneManager();

        }
        Util_LogInfo("*****<2-1-1> VulkanWindow::createScene finish *****");
    }
    void VulkanWindow::createSceneManager()
    {

    }
    void VulkanWindow::buildScene()
    {
        if (this->pSceneManager == nullptr)
            return;

        Util_LogInfo("*****<2-3> VulkanWindow::buildScene start *****");
        {
            //1> build shaders
            buildScene_Shaders();

            //2> build input layout
            buildScene_InputLayouts();

            //3> build meshes
            buildScene_Meshes();

            //4> build scene object
            buildScene_SceneObjects();

            //5> build scene material
            buildScene_Materials();

            //6> build frame resource
            buildScene_FrameResources();
            
            //7> build constant buffer view
            buildScene_ConstantBufferViews();

            //8> build pipeline state
            buildScene_PipelineStates();
        }
        Util_LogInfo("*****<2-3> VulkanWindow::buildScene finish *****");
    }
        void VulkanWindow::buildScene_Shaders()
        {

            Util_LogInfo("<2-3-1> VulkanWindow::buildScene_Shaders finish !");
        }
        void VulkanWindow::buildScene_InputLayouts()
        {

            Util_LogInfo("<2-3-2> VulkanWindow::buildScene_InputLayouts finish !");
        }
        void VulkanWindow::buildScene_Meshes()
        {

            Util_LogInfo("<2-3-3> VulkanWindow::buildScene_Meshes finish !");
        }
        void VulkanWindow::buildScene_SceneObjects()
        {

            Util_LogInfo("<2-3-4> VulkanWindow::buildScene_SceneObjects finish !");
        }
        void VulkanWindow::buildScene_Materials()
        {

            Util_LogInfo("<2-3-5> VulkanWindow::buildScene_Materials finish !");
        }
        void VulkanWindow::buildScene_FrameResources()
        {

            Util_LogInfo("<2-3-6> VulkanWindow::buildScene_FrameResources finish !");
        }
        void VulkanWindow::buildScene_ConstantBufferViews()
        {

            Util_LogInfo("<2-3-7> VulkanWindow::buildScene_ConstantBufferViews finish !");
        }
        void VulkanWindow::buildScene_PipelineStates()
        {

            Util_LogInfo("<2-3-8> VulkanWindow::buildScene_PipelineStates finish !");
        }

    void VulkanWindow::createTerrain()
    {
         Util_LogInfo("*****<2-1-2> VulkanWindow::createTerrain start *****");
        {
            //1> loadTerrainData
            if (loadTerrainData())
            {
                //2> setupTerrainGeometry
                setupTerrainGeometry();
                //3> setupTerrainTexture
                setupTerrainTexture();
                //4> setupTerrainComputePipeline
                setupTerrainComputePipeline();
                //5> setupTerrainGraphicsPipeline
                setupTerrainGraphicsPipeline();
            }
        }
        Util_LogInfo("*****<2-1-2> VulkanWindow::createTerrain finish *****");
    }
        bool VulkanWindow::loadTerrainData()
        {
            if (this->cfg_terrain_Path.empty())
                return false;

            this->poTerrainHeightMapData = nullptr;
            this->poTerrainHeightMapDataFloat = nullptr;
            this->poTerrainHeightMapDataSize = 0;
            this->poTerrainHeightMapSize = 0;
            if (!VulkanUtil::LoadAssetFileToBuffer(this->cfg_terrain_Path.c_str(), &this->poTerrainHeightMapData, this->poTerrainHeightMapDataSize, false))
            {
                Util_LogError("*********************** VulkanWindow::loadTerrainData failed, path: [%s] !", this->cfg_terrain_Path.c_str());
                return false;
            }
            this->poTerrainHeightMapSize = (int32)VulkanMath::Sqrt(this->poTerrainHeightMapDataSize / 2);

            int nSize = this->poTerrainHeightMapSize;
            this->poTerrainHeightMapDataFloat = new float[nSize * nSize];
            for (int i = 0; i < nSize; i++)
            {
                for (int j = 0; j < nSize; j++)
                {
                    uint8* pStart = this->poTerrainHeightMapData + i * nSize * 2 + j * 2;
                    uint16 v = *((uint16*)pStart);
					this->poTerrainHeightMapDataFloat[i * nSize + j] = (float)v / 0xFFFF;
                }
            }
            this->poTerrainGridInstanceCount = (nSize - 1) / (this->poTerrainGridInstanceVertexCount - 1);

            Util_LogInfo("VulkanWindow::loadTerrainData: Load terrain data: [%s] success, heightmap data size: [%d], heightmap size: [%d] !", 
                         this->cfg_terrain_Path.c_str(), this->poTerrainHeightMapDataSize, this->poTerrainHeightMapSize);
            return true;
        }
        void VulkanWindow::setupTerrainGeometry()
        {   
            //1> Mesh Geometry
            MeshData meshData;
            float fSize = (float)(this->poTerrainHeightMapSize - 1.0f);
            uint32 nSizeVertex = (uint32)(this->poTerrainHeightMapSize);
            uint32 nVertexCount = 0;
            float* pHeight = nullptr;
            uint32 heightDataGap = 1;
            if (this->poTerrainIsDrawInstance)
            {
                nVertexCount = this->poTerrainGridInstanceVertexCount;
            }
            else
            {
                nVertexCount = nSizeVertex;
                if (nVertexCount > this->poTerrainGridVertexCount)
                    nVertexCount = this->poTerrainGridVertexCount;
                
                pHeight = this->poTerrainHeightMapDataFloat;
                heightDataGap = (nSizeVertex - 1) / (nVertexCount - 1);
            }
            VulkanMeshGeometry::CreateTerrain(meshData,
                                              0.0f,
                                              0.0f,
                                              fSize,
                                              fSize,
                                              nVertexCount,
                                              nVertexCount,
                                              pHeight,
                                              heightDataGap,
                                              false,
                                              false);

            int count_vertex = (int)meshData.vertices.size();
            this->poTerrain_Pos3Normal3Tex2.clear();
            this->poTerrain_Pos3Normal3Tex2.reserve(count_vertex);
            for (int i = 0; i < count_vertex; i++)
            {
                MeshVertex& vertex = meshData.vertices[i];
                Vertex_Pos3Normal3Tex2 v;
                v.pos = vertex.pos;
                v.normal = vertex.normal;
                v.texCoord = vertex.texCoord;
                this->poTerrain_Pos3Normal3Tex2.push_back(v);
            }

            int count_index = (int)meshData.indices32.size();
            this->poTerrain_Indices.clear();
            this->poTerrain_Indices.reserve(count_index);
            for (int i = 0; i < count_index; i++)
            {
                this->poTerrain_Indices.push_back(meshData.indices32[i]);
            }

            this->poTerrainVertexCount = (uint32_t)this->poTerrain_Pos3Normal3Tex2.size();
            this->poTerrainVertexBuffer_Size = this->poTerrainVertexCount * sizeof(Vertex_Pos3Normal3Tex2);
            this->poTerrainVertexBuffer_Data = &this->poTerrain_Pos3Normal3Tex2[0];
            this->poTerrainIndexCount = (uint32_t)this->poTerrain_Indices.size();
            this->poTerrainIndexBuffer_Size = this->poTerrainIndexCount * sizeof(uint32_t);
            this->poTerrainIndexBuffer_Data = &this->poTerrain_Indices[0];

            Util_LogInfo("VulkanWindow::setupTerrainGeometry: create terrain mesh: [Pos3Normal3Tex2]: Grid: [%d - %d], Vertex-Index: [%d - %d], Instance-Grid: [%d - %d] success !", 
                     nVertexCount, nVertexCount,
                     (int)this->poTerrain_Pos3Normal3Tex2.size(), 
                     (int)this->poTerrain_Indices.size(),
                     (int)this->poTerrainGridInstanceCount,
                     (int)this->poTerrainGridInstanceVertexCount);

            //2> createVertexBuffer
            createVertexBuffer(this->poTerrainVertexBuffer_Size, this->poTerrainVertexBuffer_Data, this->poTerrainVertexBuffer, this->poTerrainVertexBufferMemory);

            //3> createIndexBuffer
            createIndexBuffer(this->poTerrainIndexBuffer_Size, this->poTerrainIndexBuffer_Data, this->poTerrainIndexBuffer, this->poTerrainIndexBufferMemory);
            
        }
        void VulkanWindow::setupTerrainTexture()
        {
            //Compute
            {
                //1> TerrainHeightMap Texture
                {
                    createTextureRenderTarget2D(this->poTerrainHeightMapData,
                                                this->poTerrainHeightMapSize,
                                                this->poTerrainHeightMapSize,
                                                1,
                                                VK_SAMPLE_COUNT_1_BIT,
                                                VK_FORMAT_R16_UNORM,
                                                VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                                true,
                                                this->poTerrainHeightMapImage,
                                                this->poTerrainHeightMapImageMemory);
                    createVkImageView(this->poTerrainHeightMapImage, 
                                      VK_IMAGE_VIEW_TYPE_2D, 
                                      VK_FORMAT_R16_UNORM, 
                                      VK_IMAGE_ASPECT_COLOR_BIT, 
                                      1, 
                                      1, 
                                      this->poTerrainHeightMapImageView);
                    Util_LogInfo("VulkanWindow::setupTerrainTexture: Compute: Create render texture [TerrainHeightMap] - [%d, %d] success !",
                                (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
                //2> TerrainNormalMap Texture
                {
                    createTextureRenderTarget2D(nullptr,
                                                this->poTerrainHeightMapSize,
                                                this->poTerrainHeightMapSize,
                                                1,
                                                VK_SAMPLE_COUNT_1_BIT,
                                                VK_FORMAT_R8G8B8A8_UNORM,
                                                VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT,
                                                true,
                                                this->poTerrainNormalMapImage,
                                                this->poTerrainNormalMapImageMemory);
                    createVkImageView(this->poTerrainNormalMapImage, 
                                    VK_IMAGE_VIEW_TYPE_2D, 
                                    VK_FORMAT_R8G8B8A8_UNORM, 
                                    VK_IMAGE_ASPECT_COLOR_BIT, 
                                    1, 
                                    1, 
                                    this->poTerrainNormalMapImageView);
                    Util_LogInfo("VulkanWindow::setupTerrainTexture: Compute: Create render texture [TerrainNormalMap] - [%d, %d] success !",
                                (int)this->poTerrainHeightMapSize, (int)this->poTerrainHeightMapSize);
                }
                //3> Terrain ImageSampler
                {
                    createVkSampler(Vulkan_TextureFilter_Bilinear, 
                                    Vulkan_TextureAddressing_Clamp,
                                    Vulkan_TextureBorderColor_OpaqueBlack,
                                    true,
                                    this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                    0.0f,
                                    1.0f,
                                    0.0f,
                                    this->poTerrainImageSampler);
                }
                //4> ImageInfo
                {
                    this->poTerrainHeightMapImageInfo = {};
                    this->poTerrainHeightMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainHeightMapImageInfo.imageView = this->poTerrainHeightMapImageView;
                    this->poTerrainHeightMapImageInfo.sampler = this->poTerrainImageSampler;

                    this->poTerrainNormalMapImageInfo = {};
                    this->poTerrainNormalMapImageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
                    this->poTerrainNormalMapImageInfo.imageView = this->poTerrainNormalMapImageView;
                    this->poTerrainNormalMapImageInfo.sampler = this->poTerrainImageSampler;
                }
            }

            //Graphics
            {
                uint32_t mipMapCount = 1;
                //1> Terrain Diffuse
                {
                    StringVector aPathTextureDiffuse = VulkanUtilString::Split(this->cfg_terrainTextureDiffuse_Path, ";");
                    createTexture2DArray(aPathTextureDiffuse, 
                                         VK_IMAGE_TYPE_2D,
                                         VK_SAMPLE_COUNT_1_BIT, 
                                         VK_FORMAT_R8G8B8A8_SRGB, 
                                         true, 
                                         mipMapCount, 
                                         this->poTerrainDiffuseImage, 
                                         this->poTerrainDiffuseImageMemory);
                    createVkImageView(this->poTerrainDiffuseImage, 
                                      VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                      VK_FORMAT_R8G8B8A8_SRGB, 
                                      VK_IMAGE_ASPECT_COLOR_BIT, 
                                      mipMapCount, 
                                      (int)aPathTextureDiffuse.size(), 
                                      this->poTerrainDiffuseImageView);
                    createVkSampler(Vulkan_TextureFilter_Bilinear, 
                                    Vulkan_TextureAddressing_Clamp,
                                    Vulkan_TextureBorderColor_OpaqueBlack,
                                    true,
                                    this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                    0.0f,
                                    static_cast<float>(mipMapCount),
                                    0.0f,
                                    this->poTerrainDiffuseImageSampler);

                    this->poTerrainDiffuseImageInfo = {};
                    this->poTerrainDiffuseImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainDiffuseImageInfo.imageView = this->poTerrainDiffuseImageView;
                    this->poTerrainDiffuseImageInfo.sampler = this->poTerrainDiffuseImageSampler;

                    Util_LogInfo("VulkanWindow::setupTerrainTexture: Graphics: Create terrain diffuse texture array: [%s] success !",
                                 this->cfg_terrainTextureDiffuse_Path.c_str());
                }
                //2> Terrain Normal
                {
                    StringVector aPathTextureNormal = VulkanUtilString::Split(this->cfg_terrainTextureNormal_Path, ";");
                    createTexture2DArray(aPathTextureNormal, 
                                         VK_IMAGE_TYPE_2D,
                                         VK_SAMPLE_COUNT_1_BIT, 
                                         VK_FORMAT_R8G8B8A8_UNORM, 
                                         true, 
                                         mipMapCount, 
                                         this->poTerrainNormalImage, 
                                         this->poTerrainNormalImageMemory);
                    createVkImageView(this->poTerrainNormalImage, 
                                      VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                      VK_FORMAT_R8G8B8A8_UNORM, 
                                      VK_IMAGE_ASPECT_COLOR_BIT, 
                                      mipMapCount, 
                                      (int)aPathTextureNormal.size(), 
                                      this->poTerrainNormalImageView);
                    createVkSampler(Vulkan_TextureFilter_Bilinear, 
                                    Vulkan_TextureAddressing_Clamp,
                                    Vulkan_TextureBorderColor_OpaqueBlack,
                                    true,
                                    this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                    0.0f,
                                    static_cast<float>(mipMapCount),
                                    0.0f,
                                    this->poTerrainNormalImageSampler);

                    this->poTerrainNormalImageInfo = {};
                    this->poTerrainNormalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainNormalImageInfo.imageView = this->poTerrainNormalImageView;
                    this->poTerrainNormalImageInfo.sampler = this->poTerrainNormalImageSampler;

                    Util_LogInfo("VulkanWindow::setupTerrainTexture: Graphics: Create terrain normal texture array: [%s] success !",
                                 this->cfg_terrainTextureNormal_Path.c_str());
                }
                //3> Terrain Control
                {
                    StringVector aPathTextureControl = VulkanUtilString::Split(this->cfg_terrainTextureControl_Path, ";");
                    createTexture2DArray(aPathTextureControl, 
                                         VK_IMAGE_TYPE_2D,
                                         VK_SAMPLE_COUNT_1_BIT, 
                                         VK_FORMAT_R8G8B8A8_UNORM, 
                                         true, 
                                         mipMapCount, 
                                         this->poTerrainControlImage, 
                                         this->poTerrainControlImageMemory);
                    createVkImageView(this->poTerrainControlImage, 
                                      VK_IMAGE_VIEW_TYPE_2D_ARRAY, 
                                      VK_FORMAT_R8G8B8A8_UNORM, 
                                      VK_IMAGE_ASPECT_COLOR_BIT, 
                                      mipMapCount, 
                                      (int)aPathTextureControl.size(), 
                                      this->poTerrainControlImageView);
                    createVkSampler(Vulkan_TextureFilter_Bilinear, 
                                    Vulkan_TextureAddressing_Clamp,
                                    Vulkan_TextureBorderColor_OpaqueBlack,
                                    true,
                                    this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                                    0.0f,
                                    static_cast<float>(mipMapCount),
                                    0.0f,
                                    this->poTerrainControlImageSampler);

                    this->poTerrainControlImageInfo = {};
                    this->poTerrainControlImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                    this->poTerrainControlImageInfo.imageView = this->poTerrainControlImageView;
                    this->poTerrainControlImageInfo.sampler = this->poTerrainControlImageSampler;

                    Util_LogInfo("VulkanWindow::setupTerrainTexture: Graphics: Create terrain control texture array: [%s] success !",
                                 this->cfg_terrainTextureControl_Path.c_str());
                }
            }
        }
        void VulkanWindow::setupTerrainShader()
        {
            //Compute
            {
                this->poTerrainComputeShaderModuleNormalGen = createVkShaderModule("TerrainComputeShaderModuleNormalGen", this->cfg_terrainShaderNormalMapGen_Path);
                if (this->poTerrainComputeShaderModuleNormalGen == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainShader: Compute: createVkShaderModule failed: " + this->cfg_terrainShaderNormalMapGen_Path;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                Util_LogInfo("VulkanWindow::setupTerrainShader: Compute: Create terrain normal gen shader: [%s] success !",
                             this->cfg_terrainShaderNormalMapGen_Path.c_str());
            }

            //Graphics
            {
                this->poTerrainGraphicsShaderModuleVertex = createVkShaderModule("TerrainGraphicsShaderModuleVertex", this->cfg_terrainShaderVertex_Path);
                if (this->poTerrainGraphicsShaderModuleVertex == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainShader: Graphics: createVkShaderModule failed: " + this->cfg_terrainShaderVertex_Path;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                Util_LogInfo("VulkanWindow::setupTerrainShader: Graphics: Create terrain vertex shader: [%s] success !",
                             this->cfg_terrainShaderVertex_Path.c_str());

                this->poTerrainGraphicsShaderModuleFragment = createVkShaderModule("TerrainGraphicsShaderModuleFragment", this->cfg_terrainShaderFragment_Path);
                if (this->poTerrainGraphicsShaderModuleFragment == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainShader: Graphics: createVkShaderModule failed: " + this->cfg_terrainShaderFragment_Path;
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                Util_LogInfo("VulkanWindow::setupTerrainShader: Graphics: Create terrain fragment shader: [%s] success !",
                             this->cfg_terrainShaderFragment_Path.c_str());
            }
        }
        void VulkanWindow::setupTerrainComputePipeline()
        {
            //0> poBuffer_TerrainTextureCopy
            {
                this->poTerrainTextureCopy.texInfo.x = this->poTerrainHeightMapSize;
                this->poTerrainTextureCopy.texInfo.y = this->poTerrainHeightMapSize;
                VkDeviceSize bufferSize = sizeof(TextureCopyConstants);
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TerrainTextureCopy, this->poBufferMemory_TerrainTextureCopy);
                void* data;
                vkMapMemory(this->poDevice, this->poBufferMemory_TerrainTextureCopy, 0, sizeof(TextureCopyConstants), 0, &data);
                    memcpy(data, &this->poTerrainTextureCopy, sizeof(TextureCopyConstants));
                vkUnmapMemory(this->poDevice, this->poBufferMemory_TerrainTextureCopy);
            }

            //1> poTerrainComputeDescriptorSetLayout
            {
                VkDescriptorSetLayoutBindingVector bindings;
                //(0) TextureCopyConstants
                {
                    VkDescriptorSetLayoutBinding textureCopyLayoutBinding = {};
                    textureCopyLayoutBinding.binding = 0;
                    textureCopyLayoutBinding.descriptorCount = 1;
                    textureCopyLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    textureCopyLayoutBinding.pImmutableSamplers = nullptr;
                    textureCopyLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
                    bindings.push_back(textureCopyLayoutBinding);
                }
                //(1) TextureCSR
                {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 1;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
                    bindings.push_back(samplerLayoutBinding);
                }
                //(2) TextureCSRW
                {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 2;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
                    bindings.push_back(samplerLayoutBinding);
                }

                if (!createVkDescriptorSetLayout(bindings, this->poTerrainComputeDescriptorSetLayout))
                {
                    String msg = "*********************** VulkanWindow::setupTerrainComputePipeline: Failed to create descriptor set layout !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            //2> poTerrainComputePipelineLayout
            {
                VkDescriptorSetLayoutVector aDescriptorSetLayout;
                aDescriptorSetLayout.push_back(this->poTerrainComputeDescriptorSetLayout);
                this->poTerrainComputePipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
                if (this->poTerrainComputePipelineLayout == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainComputePipeline: createVkPipelineLayout failed !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            //3> poTerrainComputePipeline
            {
                this->poTerrainComputePipeline = createVkComputePipeline(this->poTerrainComputeShaderModuleNormalGen, "main", this->poTerrainComputePipelineLayout);
                if (this->poTerrainComputePipeline == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainComputePipeline: createVkComputePipeline failed !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
        }
            void VulkanWindow::createTerrainComputeDescriptorSet()
            {
                if (this->poTerrainComputeDescriptorSetLayout == VK_NULL_HANDLE)
                {
                    return;
                }
                
                createVkDescriptorSet(this->poTerrainComputeDescriptorSetLayout, this->poTerrainComputeDescriptorSet);
                
                VkWriteDescriptorSetVector descriptorWrites;
                //(0) TextureCopyConstants
                {
                    VkDescriptorBufferInfo bufferInfo_TextureCopy = {};
                    bufferInfo_TextureCopy.buffer = this->poBuffer_TerrainTextureCopy;
                    bufferInfo_TextureCopy.offset = 0;
                    bufferInfo_TextureCopy.range = sizeof(TextureCopyConstants);
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                this->poTerrainComputeDescriptorSet,
                                                0,
                                                0,
                                                1,
                                                bufferInfo_TextureCopy);
                }
                //(1) TextureCSR
                {
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              this->poTerrainComputeDescriptorSet,
                                              1,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              this->poTerrainHeightMapImageInfo);
                }
                //(2) TextureCSRW
                {
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              this->poTerrainComputeDescriptorSet,
                                              2,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                                              this->poTerrainNormalMapImageInfo);
                }
                updateVkDescriptorSets(descriptorWrites);
            }
            void VulkanWindow::destroyTerrainComputeDescriptorSet()
            {
                this->poTerrainComputeDescriptorSet = VK_NULL_HANDLE;
            }

        void VulkanWindow::setupTerrainGraphicsPipeline()
        {
            //0> poBuffer_TerrainObjectCB
            {
                this->terrainObjectCBs.clear();
                TerrainObjectConstants toWhole;
                this->terrainObjectCBs.push_back(toWhole);
                float fTerrainSize = (float)(this->poTerrainHeightMapSize - 1.0f);
                float fTerrainSizeHalf = fTerrainSize / 2.0f;
                float fTerrainInstanceSize = (float)(this->poTerrainGridInstanceVertexCount - 1.0f);
                float fTerrainInstanceSizeHalf = fTerrainInstanceSize / 2.0f;
                int nHalfInstanceCount = this->poTerrainGridInstanceCount / 2;
                for (int i = 0; i < this->poTerrainGridInstanceCount; i++)
                {
                    for (int j = 0; j < this->poTerrainGridInstanceCount; j++)
                    {
                        glm::vec3 vPos = glm::vec3(j * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf,
                                                   0.0f,
                                                   i * fTerrainInstanceSize + fTerrainInstanceSizeHalf - fTerrainSizeHalf);
                        TerrainObjectConstants toInstance;
                        toInstance.g_MatWorld = VulkanMath::Translate(vPos);
                        this->terrainObjectCBs.push_back(toWhole);
                    }
                }
                VkDeviceSize bufferSize = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
                createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffer_TerrainObjectCB, this->poBufferMemory_TerrainObjectCB);
                void* data;
                vkMapMemory(this->poDevice, this->poBufferMemory_TerrainObjectCB, 0, bufferSize, 0, &data);
                    memcpy(data, &this->terrainObjectCBs[0], bufferSize);
                vkUnmapMemory(this->poDevice, this->poBufferMemory_TerrainObjectCB);
            }

            //1> poTerrainGraphicsDescriptorSetLayout
            {
                VkDescriptorSetLayoutBindingVector bindings;
                //(0) PassConstants
                {
                    VkDescriptorSetLayoutBinding passConstants = {};
                    passConstants.binding = 0;
                    passConstants.descriptorCount = 1;
                    passConstants.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    passConstants.pImmutableSamplers = nullptr;
                    passConstants.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(passConstants);
                }
                //(1) TerrainObjectConstants
                {
                    VkDescriptorSetLayoutBinding terrainObjectConstants = {};
                    terrainObjectConstants.binding = 1;
                    terrainObjectConstants.descriptorCount = 1;
                    terrainObjectConstants.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    terrainObjectConstants.pImmutableSamplers = nullptr;
                    terrainObjectConstants.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(terrainObjectConstants);
                }
                //(2) MaterialConstants
                {
                    VkDescriptorSetLayoutBinding materialConstants = {};
                    materialConstants.binding = 2;
                    materialConstants.descriptorCount = 1;
                    materialConstants.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    materialConstants.pImmutableSamplers = nullptr;
                    materialConstants.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(materialConstants);
                }
                //(3) InstanceConstants
                {
                    VkDescriptorSetLayoutBinding instanceConstants = {};
                    instanceConstants.binding = 3;
                    instanceConstants.descriptorCount = 1;
                    instanceConstants.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    instanceConstants.pImmutableSamplers = nullptr;
                    instanceConstants.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(instanceConstants);
                }
                //(4) texture2DArrayDiffuse
                {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 4;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(samplerLayoutBinding);
                }
                //(5) texture2DArrayNormal
                {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 5;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(samplerLayoutBinding);
                }
                //(6) texture2DArrayControl
                {
                    VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
                    samplerLayoutBinding.binding = 6;
                    samplerLayoutBinding.descriptorCount = 1;
                    samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    samplerLayoutBinding.pImmutableSamplers = nullptr;
                    samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
                    bindings.push_back(samplerLayoutBinding);
                }

                if (!createVkDescriptorSetLayout(bindings, this->poTerrainGraphicsDescriptorSetLayout))
                {
                    String msg = "*********************** VulkanWindow::setupTerrainGraphicsPipeline: Failed to create descriptor set layout !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            //2> poTerrainGraphicsPipelineLayout
            {
                VkDescriptorSetLayoutVector aDescriptorSetLayout;
                aDescriptorSetLayout.push_back(this->poTerrainGraphicsDescriptorSetLayout);
                this->poTerrainGraphicsPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
                if (this->poTerrainGraphicsPipelineLayout == VK_NULL_HANDLE)
                {
                    String msg = "*********************** VulkanWindow::setupTerrainGraphicsPipeline: createVkPipelineLayout failed !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
        }
        void VulkanWindow::createTerrainGraphicsDescriptorSet()
        {
            if (this->poTerrainGraphicsDescriptorSetLayout == VK_NULL_HANDLE)
            {
                return;
            }

            createVkDescriptorSets(this->poTerrainGraphicsDescriptorSetLayout, this->poTerrainGraphicsDescriptorSets);
            
            size_t count = this->poTerrainGraphicsDescriptorSets.size();
            for (size_t i = 0; i < count; i++)
            {
                VkWriteDescriptorSetVector descriptorWrites;
                //(0) PassConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = this->poBuffers_PassCB[i];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                this->poTerrainGraphicsDescriptorSets[i],
                                                0,
                                                0,
                                                1,
                                                bufferInfo_Pass);
                }
                //(1) TerrainObjectConstants
                {
                    VkDescriptorBufferInfo bufferInfo_TerrainObject = {};
                    bufferInfo_TerrainObject.buffer = this->poBuffer_TerrainObjectCB;
                    bufferInfo_TerrainObject.offset = 0;
                    bufferInfo_TerrainObject.range = sizeof(TerrainObjectConstants) * this->terrainObjectCBs.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                this->poTerrainGraphicsDescriptorSets[i],
                                                1,
                                                0,
                                                1,
                                                bufferInfo_TerrainObject);
                }
                //(2) MaterialConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Material = {};
                    bufferInfo_Material.buffer = this->poBuffers_MaterialCB[i];
                    bufferInfo_Material.offset = 0;
                    bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                this->poTerrainGraphicsDescriptorSets[i],
                                                2,
                                                0,
                                                1,
                                                bufferInfo_Material);
                }
                //(3) InstanceConstants
                {
                    VkDescriptorBufferInfo bufferInfo_Instance = {};
                    bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
                    bufferInfo_Instance.offset = 0;
                    bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                    pushVkDescriptorSet_Uniform(descriptorWrites,
                                                this->poTerrainGraphicsDescriptorSets[i],
                                                3,
                                                0,
                                                1,
                                                bufferInfo_Instance);
                }
                //(4) texture2DArrayDiffuse
                {
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              this->poTerrainGraphicsDescriptorSets[i],
                                              4,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              this->poTerrainDiffuseImageInfo);
                }
                //(5) texture2DArrayNormal
                {
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              this->poTerrainGraphicsDescriptorSets[i],
                                              5,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              this->poTerrainNormalImageInfo);
                }
                //(6) texture2DArrayControl
                {
                    pushVkDescriptorSet_Image(descriptorWrites,
                                              this->poTerrainGraphicsDescriptorSets[i],
                                              6,
                                              0,
                                              1,
                                              VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                              this->poTerrainControlImageInfo);
                }
                updateVkDescriptorSets(descriptorWrites);
            }
        }
        void VulkanWindow::destroyTerrainGraphicsDescriptorSet()
        {
            this->poTerrainGraphicsDescriptorSets.clear();
        }

        void VulkanWindow::createTerrainGraphicsPipeline()
        {
            if (this->poTerrainGraphicsShaderModuleVertex == VK_NULL_HANDLE ||
                this->poTerrainGraphicsShaderModuleFragment == VK_NULL_HANDLE ||
                this->poTerrainGraphicsPipelineLayout == VK_NULL_HANDLE)
            {
                return;
            }

            VkViewportVector aViewports;
            aViewports.push_back(this->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(this->poScissor);

            VkPrimitiveTopology vkPrimitiveTopology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
            VkFrontFace vkFrontFace = VK_FRONT_FACE_CLOCKWISE;
            VkPolygonMode vkPolygonMode = VK_POLYGON_MODE_FILL;
            VkCullModeFlagBits vkCullModeFlagBits = VK_CULL_MODE_BACK_BIT;
            VkBool32 isDepthTest = VK_TRUE;
            VkBool32 isDepthWrite = VK_TRUE; 
            VkCompareOp vkDepthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL; 
            VkBool32 isStencilTest = VK_FALSE;
            VkStencilOpState vkStencilOpFront; 
            VkStencilOpState vkStencilOpBack; 
            VkBool32 isBlend = VK_FALSE;
            VkBlendFactor vkBlendColorFactorSrc = VK_BLEND_FACTOR_ONE; 
            VkBlendFactor vkBlendColorFactorDst = VK_BLEND_FACTOR_ZERO; 
            VkBlendOp vkBlendColorOp = VK_BLEND_OP_ADD;
            VkBlendFactor vkBlendAlphaFactorSrc = VK_BLEND_FACTOR_ONE;
            VkBlendFactor vkBlendAlphaFactorDst = VK_BLEND_FACTOR_ZERO; 
            VkBlendOp vkBlendAlphaOp = VK_BLEND_OP_ADD;
            VkColorComponentFlags vkColorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

            this->poTerrainGraphicsPipeline = createVkGraphicsPipeline(this->poTerrainGraphicsShaderModuleVertex, "main", 
                                                                       this->poTerrainGraphicsShaderModuleFragment, "main", 
                                                                       Util_GetVkVertexInputBindingDescriptionVectorPtr(Vulkan_Vertex_Pos3Normal3Tex2),
                                                                       Util_GetVkVertexInputAttributeDescriptionVectorPtr(Vulkan_Vertex_Pos3Normal3Tex2),
                                                                       this->poRenderPass, this->poTerrainGraphicsPipelineLayout, aViewports, aScissors,
                                                                       vkPrimitiveTopology, vkFrontFace, vkPolygonMode, vkCullModeFlagBits,
                                                                       isDepthTest, isDepthWrite, vkDepthCompareOp,
                                                                       isStencilTest, vkStencilOpFront, vkStencilOpBack, 
                                                                       isBlend, vkBlendColorFactorSrc, vkBlendColorFactorDst, vkBlendColorOp,
                                                                       vkBlendAlphaFactorSrc, vkBlendAlphaFactorDst, vkBlendAlphaOp,
                                                                       vkColorWriteMask);
            if (this->poTerrainGraphicsPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createTerrainGraphicsPipeline: createVkGraphicsPipeline failed !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            Util_LogInfo("VulkanWindow::createTerrainGraphicsPipeline: Graphics: Create terrain pipeline success !");
            
            this->poTerrainGraphicsPipeline_WireFrame = createVkGraphicsPipeline(this->poTerrainGraphicsShaderModuleVertex, "main", 
                                                                                 this->poTerrainGraphicsShaderModuleFragment, "main", 
                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(Vulkan_Vertex_Pos3Normal3Tex2),
                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(Vulkan_Vertex_Pos3Normal3Tex2),
                                                                                 this->poRenderPass, this->poTerrainGraphicsPipelineLayout, aViewports, aScissors,
                                                                                 vkPrimitiveTopology, vkFrontFace, VK_POLYGON_MODE_LINE, vkCullModeFlagBits,
                                                                                 isDepthTest, isDepthWrite, vkDepthCompareOp,
                                                                                 isStencilTest, vkStencilOpFront, vkStencilOpBack, 
                                                                                 isBlend, vkBlendColorFactorSrc, vkBlendColorFactorDst, vkBlendColorOp,
                                                                                 vkBlendAlphaFactorSrc, vkBlendAlphaFactorDst, vkBlendAlphaOp,
                                                                                 vkColorWriteMask);
            if (this->poTerrainGraphicsPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createTerrainGraphicsPipeline: createVkGraphicsPipeline wire frame failed !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            Util_LogInfo("VulkanWindow::createTerrainGraphicsPipeline: Graphics: Create terrain pipeline wire frame success !");
        }
        void VulkanWindow::destroyTerrainGraphicsPipeline()
        {
            if (this->poTerrainGraphicsPipeline != VK_NULL_HANDLE)
            {
                destroyVkPipeline(this->poTerrainGraphicsPipeline);
            }
            this->poTerrainGraphicsPipeline = VK_NULL_HANDLE;
            if (this->poTerrainGraphicsPipeline_WireFrame != VK_NULL_HANDLE)
            {
                destroyVkPipeline(this->poTerrainGraphicsPipeline_WireFrame);
            }
            this->poTerrainGraphicsPipeline_WireFrame = VK_NULL_HANDLE;
        }
        

    void VulkanWindow::destroyTerrain()
    {
        UTIL_DELETE_T(this->poTerrainHeightMapData)
        UTIL_DELETE_T(this->poTerrainHeightMapDataFloat)
        this->poTerrainHeightMapDataSize = 0;
        this->poTerrainHeightMapSize = 0;

        //Vertex/Index   
        this->poTerrain_Pos3Normal3Tex2.clear();
        if (this->poTerrainVertexBuffer != VK_NULL_HANDLE)
        {
            destroyVkBuffer(this->poTerrainVertexBuffer, this->poTerrainVertexBufferMemory);
        }
        this->poTerrainVertexBuffer = VK_NULL_HANDLE;
        this->poTerrainVertexBufferMemory = VK_NULL_HANDLE;
        this->poTerrainVertexCount = 0;
        this->poTerrainVertexBuffer_Size = 0;
        this->poTerrainVertexBuffer_Data = nullptr;

        this->poTerrain_Indices.clear();
        if (this->poTerrainIndexBuffer != VK_NULL_HANDLE)
        {
            destroyVkBuffer(this->poTerrainIndexBuffer, this->poTerrainIndexBufferMemory);
        }
        this->poTerrainIndexBuffer = VK_NULL_HANDLE;
        this->poTerrainIndexBufferMemory = VK_NULL_HANDLE;
        this->poTerrainIndexCount = 0;
        this->poTerrainIndexBuffer_Size = 0;
        this->poTerrainIndexBuffer_Data = nullptr;

        //Texture
        if (this->poTerrainHeightMapImage != VK_NULL_HANDLE)
        {
            destroyVkImage(this->poTerrainHeightMapImage, this->poTerrainHeightMapImageMemory, this->poTerrainHeightMapImageView);
        }
        this->poTerrainHeightMapImage = VK_NULL_HANDLE;
        this->poTerrainHeightMapImageMemory = VK_NULL_HANDLE;
        this->poTerrainHeightMapImageView = VK_NULL_HANDLE;
        if (this->poTerrainNormalMapImage != VK_NULL_HANDLE)
        {
            destroyVkImage(this->poTerrainNormalMapImage, this->poTerrainNormalMapImageMemory, this->poTerrainNormalMapImageView);
        }
        this->poTerrainNormalMapImage = VK_NULL_HANDLE;
        this->poTerrainNormalMapImageMemory = VK_NULL_HANDLE;
        this->poTerrainNormalMapImageView = VK_NULL_HANDLE;
        if (this->poTerrainImageSampler != VK_NULL_HANDLE)
        {
            destroyVkImageSampler(this->poTerrainImageSampler);
        }
        this->poTerrainImageSampler = VK_NULL_HANDLE;

        //Compute
        if (this->poBuffer_TerrainTextureCopy != VK_NULL_HANDLE)
        {
            destroyVkBuffer(this->poBuffer_TerrainTextureCopy, this->poBufferMemory_TerrainTextureCopy);
        }
        this->poBuffer_TerrainTextureCopy = VK_NULL_HANDLE;
        this->poBufferMemory_TerrainTextureCopy = VK_NULL_HANDLE;
        if (this->poTerrainComputeDescriptorSetLayout != VK_NULL_HANDLE)
        {
            destroyVkDescriptorSetLayout(this->poTerrainComputeDescriptorSetLayout);
        }
        this->poTerrainComputeDescriptorSetLayout = VK_NULL_HANDLE;
        if (this->poTerrainComputePipelineLayout != VK_NULL_HANDLE)
        {
            destroyVkPipelineLayout(this->poTerrainComputePipelineLayout);
        }
        this->poTerrainComputePipelineLayout = VK_NULL_HANDLE;
        if (this->poTerrainComputePipeline != VK_NULL_HANDLE)
        {
            destroyVkPipeline(this->poTerrainComputePipeline);
        }
        this->poTerrainComputePipeline = VK_NULL_HANDLE;
         if (this->poTerrainComputeShaderModuleNormalGen != VK_NULL_HANDLE)
        {
            destroyVkShaderModule(this->poTerrainComputeShaderModuleNormalGen);
        }
        this->poTerrainComputeShaderModuleNormalGen = VK_NULL_HANDLE;
        
        destroyTerrainComputeDescriptorSet();
        
        //Graphics
        if (this->poBuffer_TerrainObjectCB != VK_NULL_HANDLE)
        {
            destroyVkBuffer(this->poBuffer_TerrainObjectCB, this->poBufferMemory_TerrainObjectCB);
        }
        this->poBuffer_TerrainObjectCB = VK_NULL_HANDLE;
        this->poBufferMemory_TerrainObjectCB = VK_NULL_HANDLE;
        if (this->poTerrainGraphicsDescriptorSetLayout != VK_NULL_HANDLE)
        {
            destroyVkDescriptorSetLayout(this->poTerrainGraphicsDescriptorSetLayout);
        }
        this->poTerrainGraphicsDescriptorSetLayout = VK_NULL_HANDLE;
        if (this->poTerrainGraphicsPipelineLayout != VK_NULL_HANDLE)
        {
            destroyVkPipelineLayout(this->poTerrainGraphicsPipelineLayout);
        }
        this->poTerrainGraphicsPipelineLayout = VK_NULL_HANDLE;
        
        if (this->poTerrainGraphicsShaderModuleVertex != VK_NULL_HANDLE)
        {
            destroyVkShaderModule(this->poTerrainGraphicsShaderModuleVertex);
        }
        this->poTerrainGraphicsShaderModuleVertex = VK_NULL_HANDLE;
        if (this->poTerrainGraphicsShaderModuleFragment != VK_NULL_HANDLE)
        {
            destroyVkShaderModule(this->poTerrainGraphicsShaderModuleFragment);
        }
        this->poTerrainGraphicsShaderModuleFragment = VK_NULL_HANDLE;
    }

    void VulkanWindow::createCamera()
    {

    }

    void VulkanWindow::loadGeometry()
    {
        Util_LogInfo("*****<2-2> VulkanWindow::loadGeometry start *****");
        {
            //1> loadVertexIndexBuffer
            loadVertexIndexBuffer();

            //2> loadTexture
            loadTexture();

            //3> createConstBuffers
            createConstBuffers();

            //4> preparePipeline
            preparePipeline();

            //5> createGraphicsPipeline
            createGraphicsPipeline();

            //6> createComputePipeline
            createComputePipeline();

            //7> createDescriptorSets
            createDescriptorSets();    

            //8> createCommandBuffers
            createCommandBuffers();    
        }
        Util_LogInfo("*****<2-2> VulkanWindow::loadGeometry finish *****");
    }
    void VulkanWindow::loadVertexIndexBuffer()
    {
        Util_LogInfo("**<2-2-1> VulkanWindow::loadVertexIndexBuffer start **");
        {
            //1> loadModel
            loadModel();

            //2> createVertexBuffer
            if (this->poVertexBuffer_Size > 0 &&
                this->poVertexBuffer_Data != nullptr)
            {
                createVertexBuffer(this->poVertexBuffer_Size, 
                                   this->poVertexBuffer_Data, 
                                   this->poVertexBuffer, 
                                   this->poVertexBufferMemory);
            }

            //3> createIndexBuffer
            if (this->poIndexBuffer_Size > 0 &&
                this->poIndexBuffer_Data != nullptr)
            {
                createIndexBuffer(this->poIndexBuffer_Size, 
                                  this->poIndexBuffer_Data, 
                                  this->poIndexBuffer, 
                                  this->poIndexBufferMemory);
            }
        }
        Util_LogInfo("**<2-2-1> VulkanWindow::loadVertexIndexBuffer finish **");
    }
    void VulkanWindow::loadModel()
    {
        Util_LogInfo("**<2-2-1-1> VulkanWindow::loadModel start **");
        {
            //1> model 
            if (!this->cfg_model_Path.empty())
            {
                loadModel_Assimp();
            }
            //2> model user
            else
            {
                loadModel_Custom();
            }
        }
        Util_LogInfo("**<2-2-1-1> VulkanWindow::loadModel finish **");
    }
        void VulkanWindow::loadModel_Assimp()
        {
            
        }
        void VulkanWindow::loadModel_Custom()
        {

        }
    void VulkanWindow::createVertexBuffer(size_t bufSize, 
                                          void* pBuf, 
                                          VkBuffer& vertexBuffer, 
                                          VkDeviceMemory& vertexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createVkBuffer(bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       stagingBuffer, 
                       stagingBufferMemory);
        {
            void* data;
            vkMapMemory(this->poDevice, stagingBufferMemory, 0, bufSize, 0, &data);
                memcpy(data, pBuf, bufSize);
            vkUnmapMemory(this->poDevice, stagingBufferMemory);
        }
        createVkBuffer(bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                       vertexBuffer,
                       vertexBufferMemory);

        copyVkBuffer(stagingBuffer, vertexBuffer, bufSize);

        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createIndexBuffer(size_t bufSize, 
                                         void* pBuf, 
                                         VkBuffer& indexBuffer, 
                                         VkDeviceMemory& indexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createVkBuffer(bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       stagingBuffer, 
                       stagingBufferMemory);
        {
            void* data;
            vkMapMemory(this->poDevice, stagingBufferMemory, 0, bufSize, 0, &data);
                memcpy(data, pBuf, bufSize);
            vkUnmapMemory(this->poDevice, stagingBufferMemory);
        }
        createVkBuffer(bufSize, 
                       VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                       indexBuffer, 
                       indexBufferMemory);

        copyVkBuffer(stagingBuffer, indexBuffer, bufSize);    

        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
        void VulkanWindow::createVkBuffer(VkDeviceSize size, 
                                          VkBufferUsageFlags usage, 
                                          VkMemoryPropertyFlags properties, 
                                          VkBuffer& buffer, 
                                          VkDeviceMemory& bufferMemory)
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (!Util_CheckVkResult(vkCreateBuffer(this->poDevice, &bufferInfo, nullptr, &buffer), "vkCreateBuffer")) 
            {
                String msg = "*********************** VulkanWindow::createVkBuffer: Failed to create buffer !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            VkMemoryRequirements memRequirements = {};
            vkGetBufferMemoryRequirements(this->poDevice, buffer, &memRequirements);
            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkBuffer: Failed to allocate buffer memory !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            vkBindBufferMemory(this->poDevice, buffer, bufferMemory, 0);
        }
            uint32_t VulkanWindow::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
            {
                VkPhysicalDeviceMemoryProperties memProperties;
                vkGetPhysicalDeviceMemoryProperties(this->poPhysicalDevice, &memProperties);

                for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
                {
                    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
                    {
                        return i;
                    }
                }

                String msg = "*********************** VulkanWindow::findMemoryType: Failed to find suitable memory type !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            void VulkanWindow::copyVkBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
            {
                VkCommandBuffer commandBuffer = beginSingleTimeCommands();
                {
                    VkBufferCopy copyRegion = {};
                    copyRegion.size = size;
                    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
                }
                endSingleTimeCommands(commandBuffer);
            }
            void VulkanWindow::destroyVkBuffer(VkBuffer buffer, VkDeviceMemory bufferMemory)
            {
                if (buffer != VK_NULL_HANDLE)
                {
                    vkDestroyBuffer(this->poDevice, buffer, nullptr);
                    vkFreeMemory(this->poDevice, bufferMemory, nullptr);
                }
            }

    void VulkanWindow::loadTexture()
    {
        if (!this->cfg_texture_Path.empty())
        {
            createTexture2D(this->cfg_texture_Path, this->poMipMapCount, this->poTextureImage, this->poTextureImageMemory);
            createVkImageView(this->poTextureImage, VK_IMAGE_VIEW_TYPE_2D, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipMapCount, 1, this->poTextureImageView);
            createVkSampler(this->poMipMapCount, this->poTextureSampler);

            Util_LogInfo("<2-2-2> VulkanWindow::loadTexture finish !");
        }
    }
    void VulkanWindow::destroyVkImage(VkImage image, VkDeviceMemory imageMemory, VkImageView imageView)
    {
        if (image != VK_NULL_HANDLE)
        {
            vkDestroyImage(this->poDevice, image, nullptr);
            vkFreeMemory(this->poDevice, imageMemory, nullptr);
        }
        destroyVkImageView(imageView);
    }
    void VulkanWindow::destroyVkImageView(VkImageView imageView)
    {
        if (imageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(this->poDevice, imageView, nullptr);
        }
    }
    void VulkanWindow::destroyVkImageSampler(VkSampler sampler)
    {
        if (sampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(this->poDevice, sampler, nullptr);
        }
    }
    void VulkanWindow::createTexture1D(const String& pathAsset_Tex, 
                                       uint32_t& mipMapCount,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        createTexture2D(pathAsset_Tex,
                        VK_IMAGE_TYPE_1D,
                        VK_SAMPLE_COUNT_1_BIT,
                        VK_FORMAT_R8G8B8A8_SRGB,
                        true,
                        mipMapCount,
                        image,
                        imageMemory);
    }
    
    void VulkanWindow::createTexture2D(const String& pathAsset_Tex, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory,
                                       VkBuffer& buffer, 
                                       VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        String pathTexture = GetAssetFullPath(pathAsset_Tex);
        int width, height, texChannels;
        stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = width * height * 4;
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        if (!pixels) 
        {
            String msg = "*********************** VulkanWindow::createTexture2D: Failed to load texture image: " + pathAsset_Tex;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        //2> Create Buffer and copy Texture data to buffer
        createVkBuffer(imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        void* data;
        vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(this->poDevice, bufferMemory);

        stbi_image_free(pixels);

        uint32_t depth = 1;
        uint32_t numArray = 1;

        //3> CreateImage
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            width, 
                            height, 
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture2D(const String& pathAsset_Tex, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture2D(pathAsset_Tex, 
                        type, 
                        numSamples,
                        format,
                        autoMipMap,
                        mipMapCount,
                        image, 
                        imageMemory, 
                        stagingBuffer, 
                        stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTexture2D(const String& pathAsset_Tex, 
                                       uint32_t& mipMapCount,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        createTexture2D(pathAsset_Tex,
                        VK_IMAGE_TYPE_2D,
                        VK_SAMPLE_COUNT_1_BIT,
                        VK_FORMAT_R8G8B8A8_SRGB,
                        true,
                        mipMapCount,
                        image,
                        imageMemory);
    }

    static void s_DeletePixels(const std::vector<stbi_uc*>& aPixels)
    {
        size_t count_tex = aPixels.size();
        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];
            stbi_image_free(pixels);
        }
    }
    void VulkanWindow::createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory,
                                            VkBuffer& buffer, 
                                            VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset_Tex.size();
        if (count_tex <= 0)
        {
            Util_LogError("*********************** VulkanWindow::createTexture2DArray: Texture path count <= 0 !");
            return;
        }
        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset_Tex = aPathAsset_Tex[i];
            String pathTexture = GetAssetFullPath(pathAsset_Tex);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Failed to load texture image: " + pathTexture;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Texture image's all width must the same !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            if (aHeight[i] != height)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTexture2DArray: Texture image's all height must the same !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        uint32_t depth = 1;
        uint32_t numArray = count_tex;
        if (type == VK_IMAGE_TYPE_1D)
        {
            depth = 0;
        }

        //2> Create Buffer and copy Texture data to buffer
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        createVkBuffer(imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, width * height * 4 * i, imageSize, 0, &data);
                memcpy(data, pixels, static_cast<size_t>(imageSize));
            vkUnmapMemory(this->poDevice, bufferMemory);
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture2DArray(aPathAsset_Tex, 
                             type, 
                             numSamples,
                             format,
                             autoMipMap,
                             mipMapCount,
                             image, 
                             imageMemory, 
                             stagingBuffer, 
                             stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTexture2DArray(const StringVector& aPathAsset_Tex, 
                                            uint32_t& mipMapCount,
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        createTexture2DArray(aPathAsset_Tex,
                             VK_IMAGE_TYPE_2D,
                             VK_SAMPLE_COUNT_1_BIT,
                             VK_FORMAT_R8G8B8A8_SRGB,
                             true,
                             mipMapCount,
                             image,
                             imageMemory);
    }
    
    void VulkanWindow::createTexture3D(VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory,
                                       VkBuffer& buffer, 
                                       VkDeviceMemory& bufferMemory)
    {
        VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(this->poPhysicalDevice, format, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT))
		{
            Util_LogError("*********************** VulkanWindow::createTexture3D: Physical device does not support flag 'VK_FORMAT_FEATURE_TRANSFER_DST_BIT' for selected texture format !");
            return;
		}
		uint32_t maxImageDimension3D(this->poPhysicalDeviceProperties.limits.maxImageDimension3D);
		if (width > maxImageDimension3D || height > maxImageDimension3D || depth > maxImageDimension3D)
		{
            Util_LogError("*********************** VulkanWindow::createTexture3D: Requested texture dimensions is greater than supported 3D texture dimension !");
			return;
		}

        //1> Create Buffer and copy Texture data to buffer
        VkDeviceSize imageSize = size;
        createVkBuffer(imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        void* data;
        vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            memcpy(data, pDataRGBA, static_cast<size_t>(imageSize));
        vkUnmapMemory(this->poDevice, bufferMemory);

        //2> CreateImage
        createVkImage(width, 
                      height, 
                      depth,
                      1,
                      1, 
                      VK_IMAGE_TYPE_3D,
                      false,
                      VK_SAMPLE_COUNT_1_BIT, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  1);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  1);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  1);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            width, 
                            height,
                            1,
                            1,
                            false);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTexture3D(VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& image, 
                                       VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTexture3D(format, 
                        pDataRGBA, 
                        size,
                        width,
                        height,
                        depth,
                        image, 
                        imageMemory, 
                        stagingBuffer, 
                        stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureCubeMap(const StringVector& aPathAsset_Tex, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory,
                                            VkBuffer& buffer, 
                                            VkDeviceMemory& bufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset_Tex.size();
        if (count_tex <= 0)
        {
            Util_LogError("*********************** VulkanWindow::createTextureCubeMap: Texture path count <= 0 !");
            return;
        }
        if (count_tex != 6)
        {
            Util_LogError("*********************** VulkanWindow::createTextureCubeMap: Texture path count != 6 !");
            return;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset_Tex = aPathAsset_Tex[i];
            String pathTexture = GetAssetFullPath(pathAsset_Tex);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Failed to load texture image: " + pathTexture;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Texture image's all width must the same !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            if (aHeight[i] != height)
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** VulkanWindow::createTextureCubeMap: Texture image's all height must the same !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }

        uint32_t depth = 1;
        uint32_t numArray = count_tex;

        //2> Create Buffer and copy Texture data to buffer
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        createVkBuffer(imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, width * height * 4 * i, imageSize, 0, &data);
                memcpy(data, pixels, static_cast<size_t>(imageSize));
            vkUnmapMemory(this->poDevice, bufferMemory);
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      VK_IMAGE_TYPE_2D,
                      true,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      false,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = beginSingleTimeCommands();
        {
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_UNDEFINED, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);
            {   
                copyBufferToImage(cmdBuffer,
                                  buffer, 
                                  image, 
                                  static_cast<uint32_t>(width), 
                                  static_cast<uint32_t>(height),
                                  static_cast<uint32_t>(depth), 
                                  numArray);
            }
            transitionImageLayout(cmdBuffer,
                                  image, 
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                  VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                  0,
                                  1,
                                  0,
                                  numArray);

            generateMipMaps(cmdBuffer,
                            image, 
                            format, 
                            width, 
                            height,
                            mipMapCount,
                            numArray,
                            autoMipMap);
        }
        endSingleTimeCommands(cmdBuffer);
    }
    void VulkanWindow::createTextureCubeMap(const StringVector& aPathAsset_Tex, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureCubeMap(aPathAsset_Tex, 
                             numSamples, 
                             format,
                             autoMipMap,
                             mipMapCount,
                             image, 
                             imageMemory, 
                             stagingBuffer, 
                             stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }
    void VulkanWindow::createTextureCubeMap(const StringVector& aPathAsset_Tex,
                                            uint32_t& mipMapCount, 
                                            VkImage& image, 
                                            VkDeviceMemory& imageMemory)
    {
        createTextureCubeMap(aPathAsset_Tex, 
                             VK_SAMPLE_COUNT_1_BIT, 
                             VK_FORMAT_R8G8B8A8_SRGB,
                             true,
                             mipMapCount,
                             image, 
                             imageMemory);
    }

    void VulkanWindow::createTextureRenderTarget1D(const glm::vec4& clDefault,
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
                                                   VkDeviceMemory& bufferMemory)
    {
        createTextureRenderTarget2D(clDefault,
                                    isSetColor,
                                    width,
                                    1,
                                    mipMapCount,
                                    VK_IMAGE_TYPE_1D,
                                    numSamples,
                                    format,
                                    usage,
                                    isGraphicsComputeShared,
                                    image,
                                    imageMemory,
                                    buffer,
                                    bufferMemory);
    }
    void VulkanWindow::createTextureRenderTarget1D(const glm::vec4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget1D(clDefault, 
                                    isSetColor,
                                    width, 
                                    mipMapCount,
                                    numSamples,
                                    format,
                                    usage,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2D(const glm::vec4& clDefault,
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
                                                   VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        createVkBuffer(imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (int i = 0; i < width * height; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout
        transitionImageLayout(VK_NULL_HANDLE,
                              image, 
                              VK_IMAGE_LAYOUT_UNDEFINED, 
                              VK_IMAGE_LAYOUT_GENERAL,
                              0,
                              mipMapCount,
                              0,
                              numArray);
    }
    void VulkanWindow::createTextureRenderTarget2D(const glm::vec4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2D(clDefault, 
                                    isSetColor,
                                    width, 
                                    height,
                                    mipMapCount,
                                    VK_IMAGE_TYPE_2D,
                                    numSamples,
                                    format,
                                    usage,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2D(uint8* pData,
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
                                                   VkDeviceMemory& bufferMemory)
    {
        uint32_t sizeFormat = 4;
        if (format == VK_FORMAT_R8_UNORM)
        {
            sizeFormat = 1;
        }
        else if (format == VK_FORMAT_R16_UNORM)
        {
            sizeFormat = 2;
        }
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * sizeFormat;
        createVkBuffer(imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (pData != nullptr)
        {
            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            {
                memcpy(data, pData, imageSize);
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout
        transitionImageLayout(VK_NULL_HANDLE,
                              image, 
                              VK_IMAGE_LAYOUT_UNDEFINED, 
                              VK_IMAGE_LAYOUT_GENERAL,
                              0,
                              mipMapCount,
                              0,
                              numArray);
    }
    void VulkanWindow::createTextureRenderTarget2D(uint8* pData,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& image, 
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2D(pData,
                                    width, 
                                    height,
                                    mipMapCount,
                                    VK_IMAGE_TYPE_2D,
                                    numSamples,
                                    format,
                                    usage,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget2DArray(const glm::vec4& clDefault,
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
                                                        VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        createVkBuffer(imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSizeAll, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (int i = 0; i < width * height * numArray; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }

        //2> CreateImage
        uint32_t depth = 1;
        if (type == VK_IMAGE_TYPE_1D)
        {
            depth = 0;
        }
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      type,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout
        transitionImageLayout(VK_NULL_HANDLE,
                              image, 
                              VK_IMAGE_LAYOUT_UNDEFINED, 
                              VK_IMAGE_LAYOUT_GENERAL,
                              0,
                              mipMapCount,
                              0,
                              numArray);
    }
    void VulkanWindow::createTextureRenderTarget2DArray(const glm::vec4& clDefault,
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
                                                        VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget2DArray(clDefault, 
                                         isSetColor,
                                         width, 
                                         height,
                                         numArray,
                                         mipMapCount,
                                         VK_IMAGE_TYPE_2D,
                                         numSamples,
                                         format,
                                         usage,
                                         isGraphicsComputeShared,
                                         image, 
                                         imageMemory, 
                                         stagingBuffer, 
                                         stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTarget3D(const glm::vec4& clDefault,
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
                                                   VkDeviceMemory& bufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * depth * 4;
        createVkBuffer(imageSize, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->poDevice, bufferMemory, 0, imageSize, 0, &data);
            {
                uint8* pColor = (uint8*)data;
                for (int i = 0; i < width * height * depth; i++)
                {
                    pColor[4 * i + 0] = r;
                    pColor[4 * i + 1] = g;
                    pColor[4 * i + 2] = b;
                    pColor[4 * i + 3] = a;
                }
            }
            vkUnmapMemory(this->poDevice, bufferMemory);
        }

        //2> CreateImage
        createVkImage(width, 
                      height, 
                      depth,
                      1,
                      mipMapCount, 
                      VK_IMAGE_TYPE_3D,
                      false,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout
        transitionImageLayout(VK_NULL_HANDLE,
                              image, 
                              VK_IMAGE_LAYOUT_UNDEFINED, 
                              VK_IMAGE_LAYOUT_GENERAL,
                              0,
                              mipMapCount,
                              0,
                              1);
    }
    void VulkanWindow::createTextureRenderTarget3D(const glm::vec4& clDefault,
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
                                                   VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTarget3D(clDefault, 
                                    isSetColor,
                                    width, 
                                    height,
                                    depth,
                                    mipMapCount,
                                    numSamples,
                                    format,
                                    usage,
                                    isGraphicsComputeShared,
                                    image, 
                                    imageMemory, 
                                    stagingBuffer, 
                                    stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }

    void VulkanWindow::createTextureRenderTargetCubeMap(uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory,
                                                        VkBuffer& buffer, 
                                                        VkDeviceMemory& bufferMemory)
    {
        uint32_t numArray = 6;
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        createVkBuffer(imageSizeAll, 
                       VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                       buffer, 
                       bufferMemory);

        //2> CreateImage
        uint32_t depth = 1;
        createVkImage(width, 
                      height, 
                      depth,
                      numArray,
                      mipMapCount, 
                      VK_IMAGE_TYPE_2D,
                      true,
                      numSamples, 
                      format, 
                      VK_IMAGE_TILING_OPTIMAL, 
                      usage, 
                      VK_SHARING_MODE_EXCLUSIVE,
                      isGraphicsComputeShared,
                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                      image, 
                      imageMemory);

        //3> TransitionImageLayout
        transitionImageLayout(VK_NULL_HANDLE,
                              image, 
                              VK_IMAGE_LAYOUT_UNDEFINED, 
                              VK_IMAGE_LAYOUT_GENERAL,
                              0,
                              mipMapCount,
                              0,
                              numArray);
    }
    void VulkanWindow::createTextureRenderTargetCubeMap(uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        bool isGraphicsComputeShared,
                                                        VkImage& image, 
                                                        VkDeviceMemory& imageMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createTextureRenderTargetCubeMap(width, 
                                         height,
                                         mipMapCount,
                                         numSamples,
                                         format,
                                         usage,
                                         isGraphicsComputeShared,
                                         image, 
                                         imageMemory, 
                                         stagingBuffer, 
                                         stagingBufferMemory);
        destroyVkBuffer(stagingBuffer, stagingBufferMemory);
    }                                                     


        void VulkanWindow::createVkImage(uint32_t width, 
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
                                         VkDeviceMemory& imageMemory) 
        {
            VkImageCreateInfo imageCreateInfo = {};
            imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            if (isCubeMap)
                imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
            imageCreateInfo.imageType = type;
            imageCreateInfo.format = format;
            imageCreateInfo.extent.width = width;
            imageCreateInfo.extent.height = height;
            imageCreateInfo.extent.depth = depth;
            imageCreateInfo.mipLevels = mipMapCount <= 0 ? 1 : mipMapCount;
            imageCreateInfo.arrayLayers = numArray;
            imageCreateInfo.samples = numSamples;
            imageCreateInfo.tiling = tiling;
            imageCreateInfo.usage = usage;
            imageCreateInfo.sharingMode = sharingMode;
            if (isGraphicsComputeShared)
            {
                if (this->queueIndexGraphics != this->queueIndexCompute) 
                {
                    std::vector<uint32_t> queueFamilyIndices;
                    queueFamilyIndices.push_back(this->queueIndexGraphics);
                    queueFamilyIndices.push_back(this->queueIndexCompute);
                    imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
                    imageCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
                    imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
                }
            }
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

            if (vkCreateImage(this->poDevice, &imageCreateInfo, nullptr, &image) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImage: Failed to create image !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(this->poDevice, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImage: Failed to allocate image memory !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            vkBindImageMemory(this->poDevice, image, imageMemory, 0);
        }
        void VulkanWindow::createVkImageView(VkImage image, 
                                             VkImageViewType type, 
                                             VkFormat format, 
                                             VkImageAspectFlags aspectFlags, 
                                             uint32_t mipMapCount,
                                             uint32_t numArray,
                                             VkImageView& imageView) 
        {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = type;
            viewInfo.format = format;
            viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = mipMapCount;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = numArray;

            if (vkCreateImageView(this->poDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkImageView: Failed to create texture image view !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
        void VulkanWindow::createVkSampler(uint32_t mipMapCount, 
                                           VkSampler& sampler)
        {
            createVkSampler(Vulkan_TextureFilter_Bilinear,
                            Vulkan_TextureAddressing_Clamp,
                            Vulkan_TextureBorderColor_OpaqueBlack,
                            true,
                            this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                            0.0f,
                            static_cast<float>(mipMapCount),
                            0.0f,
                            sampler);
        }
        void VulkanWindow::createVkSampler(VulkanTextureFilterType eTextureFilter,
                                           VulkanTextureAddressingType eTextureAddressing,
                                           VulkanTextureBorderColorType eTextureBorderColor,
                                           bool enableAnisotropy,
                                           float maxAnisotropy,
                                           float minLod, 
                                           float maxLod, 
                                           float mipLodBias,
                                           VkSampler& sampler)
        {
            VkSamplerCreateInfo samplerInfo = {};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.minFilter = Util_Transform2VkFilter(eTextureFilter, Vulkan_TextureFilterSize_Min);
            samplerInfo.magFilter = Util_Transform2VkFilter(eTextureFilter, Vulkan_TextureFilterSize_Mag);
            samplerInfo.addressModeU = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.addressModeV = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.addressModeW = Util_Transform2VkSamplerAddressMode(eTextureAddressing);
            samplerInfo.anisotropyEnable = enableAnisotropy ? VK_TRUE : VK_FALSE;
            samplerInfo.maxAnisotropy = maxAnisotropy > this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy ? this->poPhysicalDeviceProperties.limits.maxSamplerAnisotropy : maxAnisotropy;
            samplerInfo.borderColor = Util_Transform2VkBorderColor(eTextureBorderColor);
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = Util_Transform2VkSamplerMipmapMode(eTextureFilter);
            samplerInfo.minLod = minLod;
            samplerInfo.maxLod = maxLod;
            samplerInfo.mipLodBias = mipLodBias;

            if (vkCreateSampler(this->poDevice, &samplerInfo, nullptr, &sampler) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkSampler: Failed to create texture sampler !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }


        void VulkanWindow::transitionImageLayout(VkCommandBuffer cmdBuffer,
                                                 VkImage image, 
                                                 VkImageLayout oldLayout, 
                                                 VkImageLayout newLayout,
                                                 uint32_t mipBase,
                                                 uint32_t mipCount,
                                                 uint32_t numBase,
                                                 uint32_t numArray,
                                                 VkImageAspectFlags aspectFlags /*= VK_IMAGE_ASPECT_COLOR_BIT*/) 
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                VkImageMemoryBarrier barrier = {};
                barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
                barrier.oldLayout = oldLayout;
                barrier.newLayout = newLayout;
                barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
                barrier.image = image;

                barrier.subresourceRange.aspectMask = aspectFlags;
                barrier.subresourceRange.baseMipLevel = mipBase;
                barrier.subresourceRange.levelCount = mipCount;
                barrier.subresourceRange.baseArrayLayer = numBase;
                barrier.subresourceRange.layerCount = numArray;

                VkPipelineStageFlags sourceStage;
                VkPipelineStageFlags destinationStage;

                if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                } 
                else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_GENERAL)
                {
                    // VK_IMAGE_LAYOUT_UNDEFINED -> VK_IMAGE_LAYOUT_GENERAL
                    barrier.srcAccessMask = 0;
                    barrier.dstAccessMask = 0;

                    sourceStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                    destinationStage = VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL -> VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                }
                else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
                {
                    // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL -> VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
                } 
                else 
                {
                    throw std::invalid_argument("VulkanWindow::transitionImageLayout: Unsupported layout transition !");
                }

                vkCmdPipelineBarrier(cmdBuffer,
                                     sourceStage, 
                                     destinationStage,
                                     0,
                                     0, 
                                     nullptr,
                                     0, 
                                     nullptr,
                                     1, 
                                     &barrier);
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }
        void VulkanWindow::copyBufferToImage(VkCommandBuffer cmdBuffer,
                                             VkBuffer buffer, 
                                             VkImage image, 
                                             uint32_t width, 
                                             uint32_t height,
                                             uint32_t depth,
                                             uint32_t numArray) 
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                std::vector<VkBufferImageCopy> bufferCopyRegions;
                for (uint32_t i = 0; i < numArray; i++)
                {
                    VkBufferImageCopy region = {};
                    region.bufferOffset = width * height * 4 * i;
                    region.imageExtent.width = width;
                    region.imageExtent.height = height;
                    region.imageExtent.depth = depth;
                    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                    region.imageSubresource.mipLevel = 0;
                    region.imageSubresource.baseArrayLayer = i;
                    region.imageSubresource.layerCount = 1;
                    bufferCopyRegions.push_back(region);
                }
                vkCmdCopyBufferToImage(cmdBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, (uint32_t)bufferCopyRegions.size(), bufferCopyRegions.data());
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }
        void VulkanWindow::generateMipMaps(VkCommandBuffer cmdBuffer,
                                           VkImage image, 
                                           VkFormat imageFormat, 
                                           int32_t width, 
                                           int32_t height, 
                                           uint32_t mipMapCount,
                                           uint32_t numArray,
                                           bool autoMipMap)
        {
            bool isCreate = false;
            if (cmdBuffer == VK_NULL_HANDLE)
            {
                isCreate = true;
                cmdBuffer = beginSingleTimeCommands();
            }
            {
                if (autoMipMap)
                {
                    int32_t mipWidth = width;
                    int32_t mipHeight = height;
                    for (uint32_t i = 1; i < mipMapCount; i++) 
                    {
                        transitionImageLayout(cmdBuffer,
                                              image, 
                                              VK_IMAGE_LAYOUT_UNDEFINED, 
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                              i,
                                              1,
                                              0,
                                              numArray);
                        {
                            VkImageBlit blit = {};
                            blit.srcOffsets[0] = {0, 0, 0};
                            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
                            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                            blit.srcSubresource.mipLevel = i - 1;
                            blit.srcSubresource.baseArrayLayer = 0;
                            blit.srcSubresource.layerCount = numArray;

                            blit.dstOffsets[0] = {0, 0, 0};
                            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                            blit.dstSubresource.mipLevel = i;
                            blit.dstSubresource.baseArrayLayer = 0;
                            blit.dstSubresource.layerCount = numArray;

                            vkCmdBlitImage(cmdBuffer,
                                           image, 
                                           VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                           image, 
                                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                           1, 
                                           &blit,
                                           VK_FILTER_LINEAR);

                            if (mipWidth > 1) 
                                mipWidth /= 2;
                            if (mipHeight > 1) 
                                mipHeight /= 2;
                        }   
                        transitionImageLayout(cmdBuffer,
                                              image,
                                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                              VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                              i,
                                              1,
                                              0,
                                              numArray);
                    }
                }

                transitionImageLayout(cmdBuffer,
                                      image,
                                      VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                      0,
                                      mipMapCount,
                                      0,
                                      numArray);
            }
            if (isCreate)
            {
                endSingleTimeCommands(cmdBuffer);
            }
        }

    void VulkanWindow::createConstBuffers()
    {
        Util_LogInfo("**<2-2-3> VulkanWindow::createConstBuffers start **");
        {
            //1> createPassCB
            createPassCB();

            //2> createObjectCB
            createObjectCB();

            //3> createMaterialCB
            createMaterialCB();

            //4> createInstanceCB
            createInstanceCB();

            //5> createCustomCB
            createCustomCB();
        }
        Util_LogInfo("**<2-2-3> VulkanWindow::createConstBuffers finish **");
    }
    void VulkanWindow::createPassCB()
    {
        buildPassCB();
        VkDeviceSize bufferSize = sizeof(PassConstants);

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_PassCB.resize(count);
        this->poBuffersMemory_PassCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            createVkBuffer(bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_PassCB[i], 
                           this->poBuffersMemory_PassCB[i]);
        }

        Util_LogInfo("<2-2-3-1> VulkanWindow::createPassCB finish !");
    }
        void VulkanWindow::buildPassCB()
        {
            
        }
    void VulkanWindow::createObjectCB()
    {
        buildObjectCB();
        VkDeviceSize bufferSize = sizeof(ObjectConstants) * this->objectCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_ObjectCB.resize(count);
        this->poBuffersMemory_ObjectCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            createVkBuffer(bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_ObjectCB[i], 
                           this->poBuffersMemory_ObjectCB[i]);
        }

        Util_LogInfo("<2-2-3-2> VulkanWindow::createObjectCB finish !");
    }
        void VulkanWindow::buildObjectCB()
        {
            ObjectConstants objectConstants;
            this->objectCBs.push_back(objectConstants);
        }
    void VulkanWindow::createMaterialCB()
    {
        buildMaterialCB();
        VkDeviceSize bufferSize = sizeof(MaterialConstants) * this->materialCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_MaterialCB.resize(count);
        this->poBuffersMemory_MaterialCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            createVkBuffer(bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_MaterialCB[i], 
                           this->poBuffersMemory_MaterialCB[i]);
        }

        Util_LogInfo("<2-2-3-3> VulkanWindow::createMaterialCB finish !");
    }
        void VulkanWindow::buildMaterialCB()
        {
            MaterialConstants materialConstants;
            this->materialCBs.push_back(materialConstants);
        }
    void VulkanWindow::createInstanceCB()
    {
        buildInstanceCB();
        VkDeviceSize bufferSize = sizeof(InstanceConstants) * this->instanceCBs.size();

        size_t count = this->poSwapChainImages.size();
        this->poBuffers_InstanceCB.resize(count);
        this->poBuffersMemory_InstanceCB.resize(count);

        for (size_t i = 0; i < count; i++) 
        {
            createVkBuffer(bufferSize, 
                           VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                           this->poBuffers_InstanceCB[i], 
                           this->poBuffersMemory_InstanceCB[i]);
        }

        Util_LogInfo("<2-2-3-4> VulkanWindow::createInstanceCB finish !");
    }
        void VulkanWindow::buildInstanceCB()
        {
            InstanceConstants instanceConstants;
            this->instanceCBs.push_back(instanceConstants);
        }
    void VulkanWindow::createCustomCB()
    {
        
        Util_LogInfo("<2-2-3-5> VulkanWindow::createCustomCB finish !");
    }

    VkShaderModule VulkanWindow::createVkShaderModule(const String& info, const String& pathFile)
    {
        if (pathFile.empty())
            return nullptr;

        CharVector code;
        if (!VulkanUtil::LoadAssetFileContent(pathFile.c_str(), code))
        {
            Util_LogError("*********************** VulkanWindow::createVkShaderModule failed, path: [%s] !", pathFile.c_str());
            return nullptr;
        }
        if (code.size() <= 0)
        {
            return nullptr;
        }

        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(this->poDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createVkShaderModule: Failed to create shader module: " + info;
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }

        return shaderModule;
    }
        void VulkanWindow::destroyVkShaderModule(VkShaderModule vkShaderModule)
        {
            if (vkShaderModule != VK_NULL_HANDLE)
            {
                vkDestroyShaderModule(this->poDevice, vkShaderModule, nullptr);
            }
        }

    bool VulkanWindow::createVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(aDescriptorSetLayoutBinding.size());
        layoutInfo.pBindings = aDescriptorSetLayoutBinding.data();
        if (vkCreateDescriptorSetLayout(this->poDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS) 
        {
            Util_LogError("*********************** VulkanWindow::createVkDescriptorSetLayout: Failed to create descriptor set layout !");
            return false;
        }
        return true;
    }
        void VulkanWindow::destroyVkDescriptorSetLayout(VkDescriptorSetLayout vkDescriptorSetLayout)
        {
            if (vkDescriptorSetLayout != VK_NULL_HANDLE)
            {
                vkDestroyDescriptorSetLayout(this->poDevice, vkDescriptorSetLayout, nullptr);
            }
        }


    VkPipelineLayout VulkanWindow::createVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(aDescriptorSetLayout.size());
        pipelineLayoutInfo.pSetLayouts = aDescriptorSetLayout.data();

        VkPipelineLayout vkPipelineLayout = VK_NULL_HANDLE;
        if (vkCreatePipelineLayout(this->poDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) 
        {
            String msg = "*********************** VulkanWindow::createVkPipelineLayout: Failed to create pipeline layout !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
        return vkPipelineLayout;
    }
        void VulkanWindow::destroyVkPipelineLayout(VkPipelineLayout vkPipelineLayout)
        {
            if (vkPipelineLayout != VK_NULL_HANDLE)
            {
                vkDestroyPipelineLayout(this->poDevice, vkPipelineLayout, nullptr);
            }
        }
        void VulkanWindow::destroyVkPipeline(VkPipeline vkPipeline)
        {
            if (vkPipeline != VK_NULL_HANDLE)
            {
                vkDestroyPipeline(this->poDevice, vkPipeline, nullptr);   
            }
        }

    void VulkanWindow::createVkPipelineCache()
    {
        if (this->poPipelineCache == VK_NULL_HANDLE)
        {
            VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
            pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
            if (vkCreatePipelineCache(this->poDevice, &pipelineCacheCreateInfo, nullptr, &this->poPipelineCache) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createVkPipelineCache: Failed to create pipeline cache !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    }
        void VulkanWindow::destroyVkPipelineCache(VkPipelineCache vkPipelineCache)
        {
            if (vkPipelineCache != VK_NULL_HANDLE)
            {
                vkDestroyPipelineCache(this->poDevice, vkPipelineCache, nullptr);
            }
        }

    void VulkanWindow::preparePipeline()
    {
        Util_LogInfo("**<2-2-4> VulkanWindow::preparePipeline start **");
        {
            //1> createVkPipelineCache
            createVkPipelineCache();

            //2> createCustomBeforePipeline
            createCustomBeforePipeline();
        }
        Util_LogInfo("**<2-2-4> VulkanWindow::preparePipeline end **");
    }
        void VulkanWindow::createCustomBeforePipeline()
        {
            
        }

    void VulkanWindow::createGraphicsPipeline()
    {
        Util_LogInfo("**<2-2-5> VulkanWindow::createGraphicsPipeline start **");
        {
            //1> createGraphicsPipeline_Default
            createGraphicsPipeline_Default();
            Util_LogInfo("<2-2-5-1> VulkanWindow::createGraphicsPipeline: createGraphicsPipeline_Default finish !");

            //2> createGraphicsPipeline_Terrain
            createGraphicsPipeline_Terrain();
            Util_LogInfo("<2-2-5-2> VulkanWindow::createGraphicsPipeline: createGraphicsPipeline_Terrain finish !");

            //3> createGraphicsPipeline_Custom
            createGraphicsPipeline_Custom();
            Util_LogInfo("<2-2-5-3> VulkanWindow::createGraphicsPipeline: createGraphicsPipeline_Custom finish !");
        }
        Util_LogInfo("**<2-2-5> VulkanWindow::createGraphicsPipeline finish **");
    }
        void VulkanWindow::createGraphicsPipeline_Default()
        {
            if (this->cfg_shaderVertex_Path.empty() ||
                this->cfg_shaderFragment_Path.empty() ||
                this->poDescriptorSetLayout == VK_NULL_HANDLE)
            {
                return;
            }

            //1> Shader
            VkShaderModule vertShaderModule = createVkShaderModule("VertexShader: ", this->cfg_shaderVertex_Path);
            if (vertShaderModule == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createGraphicsPipeline_Default: Failed to create shader module: " + this->cfg_shaderVertex_Path;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            
            VkShaderModule fragShaderModule = createVkShaderModule("FragmentShader: ", this->cfg_shaderFragment_Path);
            if (fragShaderModule == VK_NULL_HANDLE)
            {
                String msg = "*********************** VulkanWindow::createGraphicsPipeline_Default: Failed to create shader module: " + this->cfg_shaderFragment_Path;
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            //2> Viewport
            VkViewportVector aViewports;
            aViewports.push_back(this->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(this->poScissor);

            //3> PipelineLayout
            VkDescriptorSetLayoutVector aDescriptorSetLayout;
            aDescriptorSetLayout.push_back(this->poDescriptorSetLayout);
            this->poPipelineLayout = createVkPipelineLayout(aDescriptorSetLayout);
            if (this->poPipelineLayout == VK_NULL_HANDLE)
            {
                Util_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkPipelineLayout failed !");
                return;
            }

            //4> poPipelineGraphics
            this->poPipelineGraphics = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                fragShaderModule, "main",
                                                                Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                this->cfg_vkPrimitiveTopology, this->cfg_vkFrontFace, this->cfg_vkPolygonMode, this->cfg_vkCullModeFlagBits,
                                                                this->cfg_isDepthTest, this->cfg_isDepthWrite,this->cfg_DepthCompareOp,
                                                                this->cfg_isStencilTest, this->cfg_StencilOpFront, this->cfg_StencilOpBack, 
                                                                this->cfg_isBlend, this->cfg_BlendColorFactorSrc, this->cfg_BlendColorFactorDst, this->cfg_BlendColorOp,
                                                                this->cfg_BlendAlphaFactorSrc, this->cfg_BlendAlphaFactorDst, this->cfg_BlendAlphaOp,
                                                                this->cfg_ColorWriteMask);
            if (this->poPipelineGraphics == VK_NULL_HANDLE)
            {
                Util_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkGraphicsPipeline failed !");
                return;
            }

            //5> poPipelineGraphics_WireFrame
            this->poPipelineGraphics_WireFrame = createVkGraphicsPipeline(vertShaderModule, "main",
                                                                          fragShaderModule, "main",
                                                                          Util_GetVkVertexInputBindingDescriptionVectorPtr(this->poTypeVertex), 
                                                                          Util_GetVkVertexInputAttributeDescriptionVectorPtr(this->poTypeVertex),
                                                                          this->poRenderPass, this->poPipelineLayout, aViewports, aScissors,
                                                                          this->cfg_vkPrimitiveTopology, this->cfg_vkFrontFace, VK_POLYGON_MODE_LINE, this->cfg_vkCullModeFlagBits,
                                                                          this->cfg_isDepthTest, this->cfg_isDepthWrite,this->cfg_DepthCompareOp,
                                                                          this->cfg_isStencilTest, this->cfg_StencilOpFront, this->cfg_StencilOpBack, 
                                                                          this->cfg_isBlend, this->cfg_BlendColorFactorSrc, this->cfg_BlendColorFactorDst, this->cfg_BlendColorOp,
                                                                          this->cfg_BlendAlphaFactorSrc, this->cfg_BlendAlphaFactorDst, this->cfg_BlendAlphaOp,
                                                                          this->cfg_ColorWriteMask);
            if (this->poPipelineGraphics_WireFrame == VK_NULL_HANDLE)
            {
                Util_LogError("*********************** VulkanPipeline::createGraphicsPipeline_Default: createVkGraphicsPipeline wire frame failed !");
                return;
            }

            //6> Destroy Shader
            destroyVkShaderModule(fragShaderModule);
            destroyVkShaderModule(vertShaderModule);
        }
        void VulkanWindow::createGraphicsPipeline_Terrain()
        {
            createTerrainGraphicsPipeline();
        }
        void VulkanWindow::createGraphicsPipeline_Custom()
        {
            
        }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(VkShaderModule vertShaderModule, const String& vertMain,
                                                              VkShaderModule fragShaderModule, const String& fragMain,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask)
            {
                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;
                //1> Pipeline Shader Stage
                //vert
                VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
                vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertShaderStageInfo.module = vertShaderModule;
                vertShaderStageInfo.pName = vertMain.c_str();
                aShaderStageCreateInfos.push_back(vertShaderStageInfo);

                //frag
                VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
                fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragShaderStageInfo.module = fragShaderModule;
                fragShaderStageInfo.pName = fragMain.c_str();
                aShaderStageCreateInfos.push_back(fragShaderStageInfo);

                return createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                false, 0, 0,
                                                pBindingDescriptions,
                                                pAttributeDescriptions,
                                                renderPass, pipelineLayout, aViewports, aScissors,
                                                primitiveTopology, frontFace, polygonMode, cullMode,
                                                bDepthTest, bDepthWrite, depthCompareOp,
                                                bStencilTest, stencilOpFront, stencilOpBack,
                                                bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                                blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                                colorWriteMask);
            }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(VkShaderModule vertShaderModule, const String& vertMain,
                                                              VkShaderModule tescShaderModule, const String& tescMain,
                                                              VkShaderModule teseShaderModule, const String& teseMain,
                                                              VkShaderModule fragShaderModule, const String& fragMain,
                                                              VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask)
            {
                VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos;
                //1> Pipeline Shader Stage
                //vert
                VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
                vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
                vertShaderStageInfo.module = vertShaderModule;
                vertShaderStageInfo.pName = vertMain.c_str();
                aShaderStageCreateInfos.push_back(vertShaderStageInfo);

                //tesc
                VkPipelineShaderStageCreateInfo tescShaderStageInfo = {};
                tescShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                tescShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
                tescShaderStageInfo.module = tescShaderModule;
                tescShaderStageInfo.pName = tescMain.c_str();
                aShaderStageCreateInfos.push_back(tescShaderStageInfo);

                //tese
                VkPipelineShaderStageCreateInfo teseShaderStageInfo = {};
                teseShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                teseShaderStageInfo.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
                teseShaderStageInfo.module = teseShaderModule;
                teseShaderStageInfo.pName = teseMain.c_str();
                aShaderStageCreateInfos.push_back(teseShaderStageInfo);

                //frag
                VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
                fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
                fragShaderStageInfo.module = fragShaderModule;
                fragShaderStageInfo.pName = fragMain.c_str();
                aShaderStageCreateInfos.push_back(fragShaderStageInfo);

                return createVkGraphicsPipeline(aShaderStageCreateInfos,
                                                true, tessellationFlags, tessellationPatchControlPoints,
                                                pBindingDescriptions,
                                                pAttributeDescriptions,
                                                renderPass, pipelineLayout, aViewports, aScissors,
                                                primitiveTopology, frontFace, polygonMode, cullMode,
                                                bDepthTest, bDepthWrite, depthCompareOp,
                                                bStencilTest, stencilOpFront, stencilOpBack,
                                                bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                                blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                                colorWriteMask);
            }
            VkPipeline VulkanWindow::createVkGraphicsPipeline(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                              bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                              VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                              VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                              VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                              VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode,
                                                              VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                              VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                              VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                              VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                              VkColorComponentFlags colorWriteMask)
            {
                //1> Pipeline VertexInput State
                VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
                vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
                if (pBindingDescriptions != nullptr)
                {   
                    vertexInputStateInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(pBindingDescriptions->size());
                    vertexInputStateInfo.pVertexBindingDescriptions = pBindingDescriptions->data();
                }
                if (pAttributeDescriptions != nullptr)
                {
                    vertexInputStateInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                    vertexInputStateInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                }

                //2> Pipeline InputAssembly
                VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo = {};
                inputAssemblyStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
                inputAssemblyStateInfo.topology = primitiveTopology;
                inputAssemblyStateInfo.primitiveRestartEnable = VK_FALSE;

                //3> Pipeline Viewport State
                VkPipelineViewportStateCreateInfo viewportStateInfo = {};
                viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
                viewportStateInfo.viewportCount = static_cast<uint32_t>(aViewports.size());
                viewportStateInfo.pViewports = aViewports.data();
                viewportStateInfo.scissorCount = static_cast<uint32_t>(aScissors.size());
                viewportStateInfo.pScissors = aScissors.data();

                //4> Pipeline Rasterization State
                VkPipelineRasterizationStateCreateInfo rasterizationStateInfo = {};
                rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
                rasterizationStateInfo.depthClampEnable = VK_FALSE;
                rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
                rasterizationStateInfo.polygonMode = polygonMode;
                rasterizationStateInfo.lineWidth = 1.0f;
                rasterizationStateInfo.cullMode = cullMode;
                rasterizationStateInfo.frontFace = frontFace;
                rasterizationStateInfo.depthBiasEnable = VK_FALSE;
                rasterizationStateInfo.depthBiasConstantFactor = 0.0f; // Optional
                rasterizationStateInfo.depthBiasClamp = 0.0f; // Optional
                rasterizationStateInfo.depthBiasSlopeFactor = 0.0f; // Optional

                //5> Pipeline Multisample State
                VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
                multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
                multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
                multisamplingStateInfo.rasterizationSamples = IsEnable_MASS() ? this->poMSAASamples : VK_SAMPLE_COUNT_1_BIT;
                multisamplingStateInfo.minSampleShading = 1.0f; // Optional
                multisamplingStateInfo.pSampleMask = nullptr; // Optional
                multisamplingStateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
                multisamplingStateInfo.alphaToOneEnable = VK_FALSE; // Optional

                //6> Pipeline DepthStencil State
                VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
                depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
                depthStencilStateInfo.depthTestEnable = bDepthTest;
                depthStencilStateInfo.depthWriteEnable = bDepthWrite;
                depthStencilStateInfo.depthCompareOp = depthCompareOp;
                depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
                depthStencilStateInfo.stencilTestEnable = bStencilTest;
                if (bStencilTest)
                {
                    depthStencilStateInfo.front = stencilOpFront;
                    depthStencilStateInfo.back = stencilOpBack;
                }

                //7> Pipeline ColorBlend State 
                VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
                colorBlendAttachment.blendEnable = bBlend;
                colorBlendAttachment.colorWriteMask = colorWriteMask;
                if (bBlend)
                {
                    colorBlendAttachment.srcColorBlendFactor = blendColorFactorSrc;
                    colorBlendAttachment.dstColorBlendFactor = blendColorFactorDst;
                    colorBlendAttachment.colorBlendOp = blendColorOp;
                    colorBlendAttachment.srcAlphaBlendFactor = blendAlphaFactorSrc;
                    colorBlendAttachment.dstAlphaBlendFactor = blendAlphaFactorDst;
                    colorBlendAttachment.alphaBlendOp = blendAlphaOp;
                }
                
                VkPipelineColorBlendStateCreateInfo colorBlendingStateInfo = {};
                colorBlendingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
                colorBlendingStateInfo.logicOpEnable = VK_FALSE;
                colorBlendingStateInfo.logicOp = VK_LOGIC_OP_COPY;
                colorBlendingStateInfo.attachmentCount = 1;
                colorBlendingStateInfo.pAttachments = &colorBlendAttachment;
                colorBlendingStateInfo.blendConstants[0] = 0.0f;
                colorBlendingStateInfo.blendConstants[1] = 0.0f;
                colorBlendingStateInfo.blendConstants[2] = 0.0f;
                colorBlendingStateInfo.blendConstants[3] = 0.0f;

                //8> Pipeline Dynamic State
                VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
                const std::vector<VkDynamicState> dynamicStateEnables = 
                { 
                    VK_DYNAMIC_STATE_VIEWPORT, 
                    VK_DYNAMIC_STATE_SCISSOR 
                };
                dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
                dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
                dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
                dynamicStateInfo.flags = 0;

                //9> Tessellation State
                VkPipelineTessellationStateCreateInfo tessellationState = {};
                tessellationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
                tessellationState.flags = tessellationFlags;
                tessellationState.patchControlPoints = tessellationPatchControlPoints;

                //10> Graphics Pipeline
                VkGraphicsPipelineCreateInfo pipelineInfo = {};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
                pipelineInfo.pNext = nullptr;
                pipelineInfo.stageCount = static_cast<uint32_t>(aShaderStageCreateInfos.size());
                pipelineInfo.pStages = aShaderStageCreateInfos.data();
                pipelineInfo.pVertexInputState = &vertexInputStateInfo;
                pipelineInfo.pInputAssemblyState = &inputAssemblyStateInfo;
                pipelineInfo.pTessellationState = tessellationIsUsed ? &tessellationState : nullptr;
                pipelineInfo.pViewportState = &viewportStateInfo;
                pipelineInfo.pRasterizationState = &rasterizationStateInfo;
                pipelineInfo.pMultisampleState = &multisamplingStateInfo;
                pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
                pipelineInfo.pColorBlendState = &colorBlendingStateInfo;
                pipelineInfo.pDynamicState = &dynamicStateInfo;
                pipelineInfo.layout = pipelineLayout;
                pipelineInfo.renderPass = renderPass;
                pipelineInfo.subpass = 0;
                pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
                pipelineInfo.basePipelineIndex = 0;

                VkPipeline pipeline;
                if (!Util_CheckVkResult(vkCreateGraphicsPipelines(this->poDevice, this->poPipelineCache, 1, &pipelineInfo, nullptr, &pipeline), "vkCreateGraphicsPipelines"))
                {
                    Util_LogError("*********************** VulkanWindow::createVkGraphicsPipeline: vkCreateGraphicsPipelines failed !");
                    return VK_NULL_HANDLE;
                }
                return pipeline;
            }

    void VulkanWindow::createComputePipeline()
    {
        Util_LogInfo("**<2-2-6> VulkanWindow::createComputePipeline start **");
        {
            //1> createComputePipeline_Default
            createComputePipeline_Default();

            //2> createComputePipeline_Custom
            createComputePipeline_Custom();
        }
        Util_LogInfo("**<2-2-6> VulkanWindow::createComputePipeline finish **");
    }
        void VulkanWindow::createComputePipeline_Default()
        {

        }
        void VulkanWindow::createComputePipeline_Custom()
        {

        }
            VkPipeline VulkanWindow::createVkComputePipeline(VkShaderModule compShaderModule,
                                                             const String& compMain,
                                                             VkPipelineLayout pipelineLayout, 
                                                             VkPipelineCreateFlags flags /*= 0*/)
            {
                VkPipelineShaderStageCreateInfo compShaderStageInfo = {};
                compShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                compShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
                compShaderStageInfo.module = compShaderModule;
                compShaderStageInfo.pName = compMain.c_str();

                return createVkComputePipeline(compShaderStageInfo,
                                               pipelineLayout,
                                               flags);
            }
            VkPipeline VulkanWindow::createVkComputePipeline(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                             VkPipelineLayout pipelineLayout, 
                                                             VkPipelineCreateFlags flags /*= 0*/)
            {
                VkComputePipelineCreateInfo pipelineInfo = {};
                pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
                pipelineInfo.pNext = nullptr;
                pipelineInfo.flags = flags; 
                pipelineInfo.stage = shaderStageCreateInfo;
                pipelineInfo.layout = pipelineLayout;
			    
                VkPipeline pipeline;
                if (!Util_CheckVkResult(vkCreateComputePipelines(this->poDevice, this->poPipelineCache, 1, &pipelineInfo, nullptr, &pipeline), "vkCreateComputePipelines"))
                {
                    Util_LogError("*********************** VulkanWindow::createVkComputePipeline: vkCreateComputePipelines failed !");
                    return VK_NULL_HANDLE;
                }
                return pipeline;
            }


    void VulkanWindow::createDescriptorSets()
    {
        Util_LogInfo("**<2-2-7> VulkanWindow::createDescriptorSets start **");
        {
            //1> createDescriptorSets_Default
            createDescriptorSets_Default();
            Util_LogInfo("<2-2-7-1> VulkanWindow::createDescriptorSets: createDescriptorSets_Default finish !");

            //2> createDescriptorSets_Terrain
            createDescriptorSets_Terrain();
            Util_LogInfo("<2-2-7-2> VulkanWindow::createDescriptorSets: createDescriptorSets_Terrain finish !");

            //3> createDescriptorSets_Custom
            createDescriptorSets_Custom();
            Util_LogInfo("<2-2-7-3> VulkanWindow::createDescriptorSets: createDescriptorSets_Custom finish !");
        }
        Util_LogInfo("**<2-2-7> VulkanWindow::createDescriptorSets finish **");
    }
        void VulkanWindow::createDescriptorSets_Default()
        {
            if (this->poDescriptorSetLayout == VK_NULL_HANDLE)
                return;

            createVkDescriptorSets(this->poDescriptorSetLayout, this->poDescriptorSets);
            updateDescriptorSets(this->poDescriptorSets, this->poTextureImageView, this->poTextureSampler);
        }
        void VulkanWindow::createDescriptorSets_Terrain()
        {
            createTerrainComputeDescriptorSet();
            createTerrainGraphicsDescriptorSet();
        }
        void VulkanWindow::createDescriptorSets_Custom()
        {   
            
        }
            void VulkanWindow::updateDescriptorSets(VkDescriptorSetVector& aDescriptorSets, VkImageView vkTextureView, VkSampler vkSampler)
            {
                size_t count = aDescriptorSets.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkWriteDescriptorSetVector descriptorWrites;
                    //(0) PassConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Pass = {};
                        bufferInfo_Pass.buffer = this->poBuffers_PassCB[i];
                        bufferInfo_Pass.offset = 0;
                        bufferInfo_Pass.range = sizeof(PassConstants);
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    0,
                                                    0,
                                                    1,
                                                    bufferInfo_Pass);
                    }
                    //(1) ObjectConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Object = {};
                        bufferInfo_Object.buffer = this->poBuffers_ObjectCB[i];
                        bufferInfo_Object.offset = 0;
                        bufferInfo_Object.range = sizeof(ObjectConstants) * this->objectCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    1,
                                                    0,
                                                    1,
                                                    bufferInfo_Object);
                    }
                    //(2) MaterialConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Material = {};
                        bufferInfo_Material.buffer = this->poBuffers_MaterialCB[i];
                        bufferInfo_Material.offset = 0;
                        bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    2,
                                                    0,
                                                    1,
                                                    bufferInfo_Material);
                    }
                    //(3) InstanceConstants
                    {
                        VkDescriptorBufferInfo bufferInfo_Instance = {};
                        bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
                        bufferInfo_Instance.offset = 0;
                        bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();
                        pushVkDescriptorSet_Uniform(descriptorWrites,
                                                    aDescriptorSets[i],
                                                    3,
                                                    0,
                                                    1,
                                                    bufferInfo_Instance);
                    }
                    //(4) Image
                    if (vkTextureView != VK_NULL_HANDLE)
                    {
                        VkDescriptorImageInfo imageInfo = {};
                        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                        imageInfo.imageView = vkTextureView;
                        imageInfo.sampler = vkSampler;
                        pushVkDescriptorSet_Image(descriptorWrites,
                                                  aDescriptorSets[i],
                                                  4,
                                                  0,
                                                  1,
                                                  VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                                                  imageInfo);
                    }
                    updateVkDescriptorSets(descriptorWrites);
                }
            }

            void VulkanWindow::createVkDescriptorSet(VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSet& vkDescriptorSet)
            {
                VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = this->poDescriptorPool;
                allocInfo.descriptorSetCount = 1;
                allocInfo.pSetLayouts = &vkDescriptorSetLayout;

                if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createVkDescriptorSet: Failed to allocate descriptor set !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
            void VulkanWindow::createVkDescriptorSets(VkDescriptorSetLayout vkDescriptorSetLayout, VkDescriptorSetVector& aDescriptorSets)
            {
                std::vector<VkDescriptorSetLayout> layouts(this->poSwapChainImages.size(), vkDescriptorSetLayout);
                VkDescriptorSetAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
                allocInfo.descriptorPool = this->poDescriptorPool;
                allocInfo.descriptorSetCount = static_cast<uint32_t>(this->poSwapChainImages.size());
                allocInfo.pSetLayouts = layouts.data();

                size_t count = this->poSwapChainImages.size();
                aDescriptorSets.resize(count);
                if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, aDescriptorSets.data()) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createVkDescriptorSets: Failed to allocate descriptor sets !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }

            void VulkanWindow::pushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                           VkDescriptorSet dstSet,
                                                           uint32_t dstBinding,
                                                           uint32_t dstArrayElement,
                                                           uint32_t descriptorCount,
                                                           VkDescriptorBufferInfo& bufferInfo)
            {
                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = dstSet;
                ds.dstBinding = dstBinding;
                ds.dstArrayElement = dstArrayElement;
                ds.descriptorCount = descriptorCount;
                ds.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                ds.pBufferInfo = &bufferInfo;

                aWriteDescriptorSets.push_back(ds);
            }
            void VulkanWindow::pushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
                                                         VkDescriptorSet dstSet,
                                                         uint32_t dstBinding,
                                                         uint32_t dstArrayElement,
                                                         uint32_t descriptorCount,
                                                         VkDescriptorType descriptorType,
                                                         VkDescriptorImageInfo& imageInfo)
            {
                VkWriteDescriptorSet ds = {};
                ds.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds.dstSet = dstSet;
                ds.dstBinding = dstBinding;
                ds.dstArrayElement = dstArrayElement;
                ds.descriptorCount = descriptorCount;
                ds.descriptorType = descriptorType;
                ds.pImageInfo = &imageInfo;

                aWriteDescriptorSets.push_back(ds);
            }
            void VulkanWindow::updateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets)
            {
                vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(aWriteDescriptorSets.size()), aWriteDescriptorSets.data(), 0, nullptr);
            }

    void VulkanWindow::createCommandBuffers()
    {
        Util_LogInfo("**<2-2-8> VulkanWindow::createCommandBuffers start **");
        {
            //1> createCommandBuffer_Graphics
            createCommandBuffer_Graphics();

            //2> createCommandBuffer_Compute
            createCommandBuffer_Compute();


            Util_LogInfo("<2-2-8> VulkanWindow::createCommandBuffers finish, create CommandBuffersGraphics: [true], create CommandBufferCompute: [%s]", this->cfg_isUseComputeShader ? "true" : "false");
        }
        Util_LogInfo("**<2-2-8> VulkanWindow::createCommandBuffers finish **");
    }
        void VulkanWindow::createCommandBuffer_Graphics()
        {
            this->poCommandBuffersGraphics.resize(this->poSwapChainFrameBuffers.size());
            VkCommandBufferAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
            allocInfo.commandPool = this->poCommandPoolGraphics;
            allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
            allocInfo.commandBufferCount = (uint32_t)this->poCommandBuffersGraphics.size();
            if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, this->poCommandBuffersGraphics.data()) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createCommandBuffer_Graphics: Failed to allocate command buffers graphics !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            Util_LogInfo("<2-2-8-1> VulkanWindow::createCommandBuffer_Graphics: Create CommandBuffersGraphics success !");
        }
        void VulkanWindow::createCommandBuffer_Compute()
        {
            if (this->cfg_isUseComputeShader)
            {
                VkCommandBufferAllocateInfo allocInfo = {};
                allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
                allocInfo.commandPool = this->poCommandPoolCompute;
                allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
                allocInfo.commandBufferCount = 1;
                if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, &this->poCommandBufferCompute) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::createCommandBuffer_Compute: Failed to allocate command buffer compute !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                Util_LogInfo("<2-2-8-2> VulkanWindow::createCommandBuffer_Compute: Create CommandBufferCompute success !");
            }
        }

    void VulkanWindow::createImgui()
    {
        Util_LogInfo("**********<2-4> VulkanWindow::createImgui start **********");
        {
            //1> createImgui_DescriptorPool
            createImgui_DescriptorPool();
            
            //2> createImgui_Init
            createImgui_Init();
        }
        Util_LogInfo("**********<2-4> VulkanWindow::createImgui finish **********");
    }
        void VulkanWindow::createImgui_DescriptorPool()
        {   
            VkDescriptorPoolSize pool_sizes[] = 
            {
                { VK_DESCRIPTOR_TYPE_SAMPLER,                1000 },
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
                { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,          1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,          1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,   1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,   1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,         1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,         1000 },
                { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
                { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,       1000 }
            };

            VkDescriptorPoolCreateInfo pool_info = {};
            pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
            pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
            pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
            pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
            pool_info.pPoolSizes = pool_sizes;

            if (vkCreateDescriptorPool(this->poDevice, &pool_info, nullptr, &this->imgui_DescriptorPool) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::createImgui_DescriptorPool: Imgui descriptor pool creation failed !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            Util_LogInfo("<2-4-1> VulkanWindow::createImgui_DescriptorPool finish !");
        }
        void checkImguiError(VkResult err)
        {

        }
        void VulkanWindow::createImgui_Init()
        {
            //1> Config
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            this->imgui_PathIni = this->pathBin + "Log/" + this->nameTitle + ".ini";
            this->imgui_PathLog = this->pathBin + "Log/" + this->nameTitle + ".log";
            io.IniFilename = this->imgui_PathIni.c_str();
            io.LogFilename = this->imgui_PathLog.c_str();

            //2> Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsClassic();
            
            //3> Init Vulkan
            ImGui_ImplGlfw_InitForVulkan(this->pWindow, true);

            ImGui_ImplVulkan_InitInfo init_info = {};
            init_info.Instance = this->poInstance;
            init_info.PhysicalDevice = this->poPhysicalDevice;
            init_info.Device = this->poDevice;
            init_info.QueueFamily = this->queueIndexGraphics;
            init_info.Queue = this->poQueueGraphics;
            init_info.DescriptorPool = this->imgui_DescriptorPool;
            init_info.Subpass = 1;
            init_info.MinImageCount = this->imgui_MinimalSwapchainImages;
            init_info.ImageCount = this->poSwapChainImages.size();
            init_info.MSAASamples = this->poMSAASamples;
            init_info.CheckVkResultFn = checkImguiError;
            ImGui_ImplVulkan_Init(&init_info, this->poRenderPass);

            VkCommandBuffer commandBuffer = beginSingleTimeCommands();
            {
                ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
            }
            endSingleTimeCommands(commandBuffer);
            ImGui_ImplVulkan_DestroyFontUploadObjects();

            Util_LogInfo("<2-4-2> VulkanWindow::createImgui_Init finish !");
        }

    void VulkanWindow::resizeWindow(int w, int h, bool force)
    {
        if (this->width == w &&
            this->height == h &&
            !force)
        {
            return;
        }
        this->width = w;
        this->height = h;
        RefreshAspectRatio();

    }

    bool VulkanWindow::beginCompute()
    {
        if (this->poQueueCompute == VK_NULL_HANDLE ||
            this->poCommandBufferCompute == VK_NULL_HANDLE ||
            this->poGraphicsWaitSemaphore == VK_NULL_HANDLE ||
            this->poComputeWaitSemaphore == VK_NULL_HANDLE)
        {
            return false;
        }

        return true;
    }
        void VulkanWindow::updateCompute()
        {  
            // CommandBuffer
            updateComputeCommandBuffer();

        }
                void VulkanWindow::updateComputeCommandBuffer()
                {
                    vkQueueWaitIdle(this->poQueueCompute);

                    VkCommandBuffer& commandBuffer = this->poCommandBufferCompute;
                    VkCommandBufferBeginInfo beginInfo = {};
                    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                    beginInfo.flags = 0; // Optional
                    beginInfo.pInheritanceInfo = nullptr; // Optional

                    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                    {
                        String msg = "*********************** VulkanWindow::updateComputeCommandBuffer: vkBeginCommandBuffer: Failed to begin recording compute command buffer !";
                        Util_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                    {
                        updateCompute_Default(commandBuffer);
                        updateCompute_Custom(commandBuffer);
                    }
                    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                    {
                        String msg = "*********************** VulkanWindow::updateComputeCommandBuffer: vkEndCommandBuffer: Failed to record compute command buffer !";
                        Util_LogError(msg.c_str());
                        throw std::runtime_error(msg);
                    }
                }
                    void VulkanWindow::updateCompute_Default(VkCommandBuffer& commandBuffer)
                    {

                    }
                    void VulkanWindow::updateCompute_Custom(VkCommandBuffer& commandBuffer)
                    {

                    }

        void VulkanWindow::compute()
        {
            VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBufferCompute;
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = &this->poGraphicsWaitSemaphore;
            submitInfo.pWaitDstStageMask = &waitStageMask;
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = &this->poComputeWaitSemaphore;
            
            if (vkQueueSubmit(this->poQueueCompute, 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::compute: Failed to submit compute command buffer !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    void VulkanWindow::endCompute()
    {
        
    }


    bool VulkanWindow::beginRender()
    {
        vkWaitForFences(this->poDevice, 1, &this->poInFlightFences[this->poCurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(this->poDevice, this->poSwapChain, UINT64_MAX, this->poPresentCompleteSemaphores[this->poCurrentFrame], nullptr, &this->poSwapChainImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return false;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            String msg = "*********************** VulkanWindow::beginRender: Failed to acquire swap chain image !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg);
            return false;
        }
        return true;
    }
        void VulkanWindow::updateRender()
        {
            //1> SceneObject
            updateSceneObjects();

            //2> CB Pass
            updateCBs_Pass();

            //3> CB Objects
            updateCBs_Objects();

            //4> CB Materials
            updateCBs_Materials();

            //5> CB Instances
            updateCBs_Instances();

            //6> CB Custom
            updateCBs_Custom();

            //7> Imgui
            if (IsEnable_Imgui())
            {
                updateImgui();
            }

            //8> CommandBuffers
            updateRenderCommandBuffers_CustomBeforeDefault();
            {
                updateRenderCommandBuffers_Default();
            }
            updateRenderCommandBuffers_CustomAfterDefault();
        }
            void VulkanWindow::updateSceneObjects()
            {
                
            }
            void VulkanWindow::updateCBs_Pass()
            {
                if (this->poBuffersMemory_PassCB.size() <= 0)
                    return;

                //TransformConstants/CameraConstants
                if (this->pCamera != nullptr)
                {
                    updateCBs_PassTransformAndCamera(this->pCamera, 0);
                    if (this->pCameraRight == nullptr)
                    {
                        this->passCB.g_Transforms[1] = this->passCB.g_Transforms[0];
                        this->passCB.g_Cameras[1] = this->passCB.g_Cameras[0];
                    }
                }
                else
                {
                    TransformConstants& transformConstants = this->passCB.g_Transforms[0];
                    transformConstants.mat4View = glm::lookAtLH(this->cfg_cameraPos, 
                                                                this->cfg_cameraLookTarget,
                                                                this->cfg_cameraUp);
                    transformConstants.mat4View_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4View);
                    transformConstants.mat4Proj = glm::perspectiveLH(glm::radians(this->cfg_cameraFov), 
                                                                     this->poSwapChainExtent.width / (float)this->poSwapChainExtent.height,
                                                                     this->cfg_cameraNear, 
                                                                     this->cfg_cameraFar);
                    transformConstants.mat4Proj_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4Proj);
                    transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                    transformConstants.mat4ViewProj_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4ViewProj);
                    
                    //CameraConstants
                    CameraConstants& cameraConstants = this->passCB.g_Cameras[0];
                    cameraConstants.posEyeWorld = this->cfg_cameraPos;
                    cameraConstants.fNearZ = this->cfg_cameraNear;
                    cameraConstants.fFarZ = this->cfg_cameraFar;
                }   
                if (this->pCameraRight != nullptr)
                {
                    updateCBs_PassTransformAndCamera(this->pCameraRight, 1); 
                }

                //TimeConstants
                this->passCB.g_TotalTime = this->pTimer->GetTimeSinceStart();
                this->passCB.g_DeltaTime = this->pTimer->GetTimeDelta();

                //RenderTarget
                this->passCB.g_RenderTargetSize = glm::vec2(this->poViewport.width, this->poViewport.height);
                this->passCB.g_RenderTargetSize_Inv = glm::vec2(1.0f / this->poViewport.width, 1.0f / this->poViewport.height);

                //Light Settings
                memcpy(&this->passCB.g_MainLight, &this->mainLight, sizeof(LightConstants));
                for (int i = 0; i < MAX_LIGHT_COUNT; i++)
                {
                    memcpy(&this->passCB.g_AdditionalLights[i], &this->aAdditionalLights[i], sizeof(LightConstants));
                }

                //Update Buffer
                VkDeviceMemory& memory = this->poBuffersMemory_PassCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(PassConstants), 0, &data);
                    memcpy(data, &this->passCB, sizeof(PassConstants));
                vkUnmapMemory(this->poDevice, memory);
            }
                void VulkanWindow::updateCBs_PassTransformAndCamera(VulkanCamera* pCam, int nIndex)
                {
                    //TransformConstants
                    TransformConstants& transformConstants = this->passCB.g_Transforms[nIndex];
                    transformConstants.mat4View = pCam->GetMatrix4View();
                    transformConstants.mat4View_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4View);
                    transformConstants.mat4Proj = pCam->GetMatrix4Projection();
                    transformConstants.mat4Proj_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4Proj);
                    transformConstants.mat4ViewProj = transformConstants.mat4Proj * transformConstants.mat4View;
                    transformConstants.mat4ViewProj_Inv = VulkanMath::InverseMatrix4(transformConstants.mat4ViewProj);

                    //CameraConstants
                    CameraConstants& cameraConstants = this->passCB.g_Cameras[nIndex];
                    cameraConstants.posEyeWorld = pCam->GetPos();
                    cameraConstants.fNearZ = pCam->GetNearZ();
                    cameraConstants.fFarZ = pCam->GetFarZ();
                }   
            void VulkanWindow::updateCBs_Objects()
            {
                if (this->poBuffersMemory_ObjectCB.size() <= 0)
                    return;
                size_t count = this->objectCBs.size();
                if (count >= MAX_OBJECT_COUNT)
                {
                    Util_LogError("*********************** VulkanWindow::updateCBs_Objects: Max object count can not > [%d]", MAX_OBJECT_COUNT);
                    return;
                }

                updateCBs_ObjectsContent();

                VkDeviceMemory& memory = this->poBuffersMemory_ObjectCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(ObjectConstants) * count, 0, &data);
                    memcpy(data, this->objectCBs.data(), sizeof(ObjectConstants) * count);
                vkUnmapMemory(this->poDevice, memory);
            }
                void VulkanWindow::updateCBs_ObjectsContent()
                {
                    ObjectConstants& objectCB = this->objectCBs[0];
                    if (this->cfg_isRotate)
                    {
                        float time = this->pTimer->GetTimeSinceStart();
                        objectCB.g_MatWorld = glm::rotate(this->poMatWorld, 
                                                          time * glm::radians(90.0f), 
                                                          glm::vec3(0.0f, 1.0f, 0.0f));
                    }
                    else
                    {
                        objectCB.g_MatWorld = this->poMatWorld;
                    }
                }
            void VulkanWindow::updateCBs_Materials()
            {
                if (this->poBuffersMemory_MaterialCB.size() <= 0)
                    return;
                size_t count = this->materialCBs.size();
                if (count >= MAX_MATERIAL_COUNT)
                {
                    Util_LogError("*********************** VulkanWindow::updateCBs_Materials: Max material count can not > [%d]", MAX_MATERIAL_COUNT);
                    return;
                }

                updateCBs_MaterialsContent();

                VkDeviceMemory& memory = this->poBuffersMemory_MaterialCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(MaterialConstants) * count, 0, &data);
                    memcpy(data, this->materialCBs.data(), sizeof(MaterialConstants) * count);
                vkUnmapMemory(this->poDevice, memory);
            }
                void VulkanWindow::updateCBs_MaterialsContent()
                {   
                    
                }
            void VulkanWindow::updateCBs_Instances()
            {
                if (this->poBuffersMemory_InstanceCB.size() <= 0)
                    return;
                size_t count = this->instanceCBs.size();
                if (count >= MAX_INSTANCE_COUNT)
                {
                    Util_LogError("*********************** VulkanWindow::updateCBs_Instances: Max instance count can not > [%d]", MAX_INSTANCE_COUNT);
                    return;
                }

                updateCBs_InstancesContent();

                VkDeviceMemory& memory = this->poBuffersMemory_InstanceCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(InstanceConstants) * count, 0, &data);
                    memcpy(data, this->instanceCBs.data(), sizeof(InstanceConstants) * count);
                vkUnmapMemory(this->poDevice, memory);
            }
                void VulkanWindow::updateCBs_InstancesContent()
                {
                    InstanceConstants materialCB = this->instanceCBs[0];
                    materialCB.indexObject = 0;
                    materialCB.indexMaterial = 0;
                }
            void VulkanWindow::updateCBs_Custom()
            {

            }

            void VulkanWindow::updateImgui()
            {
                if (beginRenderImgui())
                {
                    endRenderImgui();
                }
            }
                bool VulkanWindow::beginRenderImgui()
                {
                    
                    return false;
                }
                    void VulkanWindow::commonConfig()
                    {
                        ImGui::Text("Frametime: %f", this->fFPS);
                        ImGui::Text("[R - ResetCamera]; [F - WireFrame]; [T - Rotate];");
                        ImGui::Separator();

                        commonOpConfig();
                    }
                        void VulkanWindow::commonOpConfig()
                        {
                            if (ImGui::CollapsingHeader("Common Settings"))
                            {
                                ImGui::Checkbox("Is WireFrame", &cfg_isWireFrame);
                                ImGui::Checkbox("Is Rotate", &cfg_isRotate);
                            }
                            ImGui::Separator();
                            ImGui::Spacing();
                        }
                    void VulkanWindow::cameraConfig()
                    {
                        if (this->pCamera == nullptr)
                            return;

                        if (ImGui::CollapsingHeader("Camera Settings"))
                        {
                            if (ImGui::Button("Camera Reset"))
                            {
                                cameraReset();
                            }
                            if (ImGui::CollapsingHeader("Camera Transform"))
                            {
                                //Position
                                glm::vec3 vPos = this->pCamera->GetPos();
                                if (ImGui::DragFloat3("Position", &vPos[0], 0.05f, -FLT_MAX, FLT_MAX))
                                {
                                    this->pCamera->SetPos(vPos);
                                    this->pCamera->UpdateViewMatrix();
                                }
                                //Rotation
                                glm::vec3 vEulerAngle = this->pCamera->GetEulerAngles();
                                if (ImGui::DragFloat3("Rotation", &vEulerAngle[0], 0.1f, -180, 180))
                                {
                                    this->pCamera->SetEulerAngles(vEulerAngle);
                                    this->pCamera->UpdateViewMatrix();
                                }
                                ImGui::Spacing();
                                //Right
                                glm::vec3 vRight = this->pCamera->GetRight();
                                if (ImGui::DragFloat3("Right (X axis)", &vRight[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                                //Up
                                glm::vec3 vUp = this->pCamera->GetUp();
                                if (ImGui::DragFloat3("Up (Y axis)", &vUp[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                                //Direction
                                glm::vec3 vDir = this->pCamera->GetDir();
                                if (ImGui::DragFloat3("Direction (Z axis)", &vDir[0], 0.1f, -1.0f, 1.0f))
                                {
                                    
                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Param"))
                            {
                                //FovY
                                float fAngle = this->pCamera->GetFovY();
                                if (ImGui::DragFloat("FovY Angle", &fAngle, 0.1f, 0.1f, 180.0f))
                                {
                                    this->pCamera->SetFovY(fAngle);
                                    this->pCamera->UpdateProjectionMatrix();
                                }
                                //Aspect
                                float fAspect = this->pCamera->GetAspect();
                                if (ImGui::DragFloat("Aspect", &fAspect, 0.1f, 0.1f, 10.0f))
                                {
                                    this->pCamera->SetAspect(fAspect);
                                    this->pCamera->UpdateProjectionMatrix();
                                }

                                //NearZ/FarZ
                                float fNearDist = this->pCamera->GetNearZ();
                                float fFarDist = this->pCamera->GetFarZ();
                                if (ImGui::DragFloat("Near Distance", &fNearDist, 0.1f, 0.1f, fFarDist - 1.0f))
                                {
                                    this->pCamera->SetNearZ(fNearDist);
                                    this->pCamera->UpdateProjectionMatrix();
                                }
                                if (ImGui::DragFloat("Far Distance", &fFarDist, 0.1f, fNearDist + 1.0f, FLT_MAX))
                                {
                                    this->pCamera->SetFarZ(fFarDist);
                                    this->pCamera->UpdateProjectionMatrix();
                                }

                                ImGui::Separator();
                                ImGui::Spacing();
                                
                                //SpeedMove
                                if (ImGui::DragFloat("Speed Move", &cfg_cameraSpeedMove, 0.1f, 1.0f, 10000.0f))
                                {
                                    
                                }
                                //SpeedZoom
                                if (ImGui::DragFloat("Speed Zoom", &cfg_cameraSpeedZoom, 0.001f, 0.01f, 5.0f))
                                {

                                }
                                //SpeedRotate
                                if (ImGui::DragFloat("Speed Rotate", &cfg_cameraSpeedRotate, 0.001f, 0.001f, 5.0f))
                                {

                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Matrix4 World"))
                            {
                                glm::mat4 mat4World = this->pCamera->GetMatrix4World();
                                if (ImGui::BeginTable("split_camera_world", 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4World[3][3]);
                                
                                    ImGui::EndTable();
                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Matrix4 View"))
                            {
                                const glm::mat4& mat4View = this->pCamera->GetMatrix4View();
                                if (ImGui::BeginTable("split_camera_view", 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4View[3][3]);
                                
                                    ImGui::EndTable();
                                }
                            }
                            if (ImGui::CollapsingHeader("Camera Matrix4 Projection"))
                            {
                                const glm::mat4& mat4Projection = pCamera->GetMatrix4Projection();
                                if (ImGui::BeginTable("split_camera_projection", 4))
                                {
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[0][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[1][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[2][3]);

                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][0]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][1]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][2]);
                                    ImGui::TableNextColumn(); ImGui::Text("%f", mat4Projection[3][3]);

                                    ImGui::EndTable();
                                }
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::cameraReset()
                        {
                            if (this->pCamera == nullptr)
                                return;

                            this->pCamera->LookAtLH(this->cfg_cameraPos, this->cfg_cameraLookTarget, this->cfg_cameraUp);
                            this->pCamera->PerspectiveLH(this->cfg_cameraFov, this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
                            this->pCamera->UpdateViewMatrix();
                        }

                    void VulkanWindow::lightConfig()
                    {
                        if (ImGui::CollapsingHeader("Light Settings"))
                        {
                            //Main Light
                            lightConfigItem(this->mainLight, "Light - Main", 0, false);
                            
                            //Additional Light
                            int count_light = MAX_LIGHT_COUNT;
                            for (int i = 0; i < count_light; i++)
                            {
                                LightConstants& lc = this->aAdditionalLights[i];
                                String nameLight = "Light - " + VulkanUtilString::SaveInt(i);
                                lightConfigItem(lc, nameLight, i, true);
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                        void VulkanWindow::lightConfigItem(LightConstants& lc, const String& name, int index, bool canChangeType)
                        {
                            struct EnumLightDesc { VulkanLightType Value; const char* Name; const char* Tooltip; };
                            static const EnumLightDesc s_aLightDescs[] =
                            {
                                { Vulkan_Light_Directional,     "Directional",      "Directional Light" },
                                { Vulkan_Light_Point,           "Point",            "Point Light" },
                                { Vulkan_Light_Spot,            "Spot",             "Spot Light" },
                            };

                            struct EnumLightingDesc { VulkanLightingType Value; const char* Name; const char* Tooltip; };
                            static const EnumLightingDesc s_aLightingDescs[] =
                            {
                                { Vulkan_Lighting_Node,                                     "None",                                     "None Lighting" },
                                { Vulkan_Lighting_Ambient,                                  "Ambient",                                  "Ambient Lighting" },
                                { Vulkan_Lighting_DiffuseLambert,                           "DiffuseLambert",                           "DiffuseLambert Lighting" },
                                { Vulkan_Lighting_SpecularPhong,                            "SpecularPhong",                            "SpecularPhong Lighting" },
                                { Vulkan_Lighting_SpecularBlinnPhong,                       "SpecularBlinnPhong",                       "SpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambert,                    "AmbientDiffuseLambert",                    "AmbientDiffuseLambert Lighting" },
                                { Vulkan_Lighting_AmbientSpecularPhong,                     "AmbientSpecularPhong",                     "AmbientSpecularPhong Lighting" },
                                { Vulkan_Lighting_AmbientSpecularBlinnPhong,                "AmbientSpecularBlinnPhong",                "AmbientSpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_DiffuseLambertSpecularPhong,              "DiffuseLambertSpecularPhong",              "DiffuseLambertSpecularPhong Lighting" },
                                { Vulkan_Lighting_DiffuseLambertSpecularBlinnPhong,         "DiffuseLambertSpecularBlinnPhong",         "DiffuseLambertSpecularBlinnPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambertSpecularPhong,       "AmbientDiffuseLambertSpecularPhong",       "AmbientDiffuseLambertSpecularPhong Lighting" },
                                { Vulkan_Lighting_AmbientDiffuseLambertSpecularBlinnPhong,  "AmbientDiffuseLambertSpecularBlinnPhong",  "AmbientDiffuseLambertSpecularBlinnPhong Lighting" },
                            };
                            
                            if (ImGui::CollapsingHeader(name.c_str()))
                            {
                                //Light Enable
                                bool isEnable = lc.common.y == 0.0f ? false : true;
                                String nameEnable = "LightEnable - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::Checkbox(nameEnable.c_str(), &isEnable))
                                {   
                                    lc.common.y = isEnable ? 1.0f : 0.0f;
                                }
                                ImGui::Spacing();

                                //Light Type
                                {
                                    int nIndex = 0;
                                    for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aLightDescs); nIndex++)
                                    {
                                        if (s_aLightDescs[nIndex].Value == lc.common.x)
                                            break;
                                    }
                                    const char* preview_text = s_aLightDescs[nIndex].Name;
                                    String nameType = "LightType - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::BeginCombo(nameType.c_str(), preview_text))
                                    {
                                        for (int j = 0; j < IM_ARRAYSIZE(s_aLightDescs); j++)
                                        {
                                            if (ImGui::Selectable(s_aLightDescs[j].Name, nIndex == j))
                                            {
                                                if (canChangeType)
                                                {
                                                    lc.common.x = (int)s_aLightDescs[j].Value;
                                                }
                                                break;
                                            }
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                ImGui::Spacing();

                                //Lighting Type
                                {
                                    int nIndex = 0;
                                    for (nIndex = 0; nIndex < IM_ARRAYSIZE(s_aLightingDescs); nIndex++)
                                    {
                                        if (s_aLightingDescs[nIndex].Value == lc.common.z)
                                            break;
                                    }
                                    const char* preview_text = s_aLightingDescs[nIndex].Name;
                                    String nameType = "LightingType - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::BeginCombo(nameType.c_str(), preview_text))
                                    {
                                        for (int j = 0; j < IM_ARRAYSIZE(s_aLightingDescs); j++)
                                        {
                                            if (ImGui::Selectable(s_aLightingDescs[j].Name, nIndex == j))
                                            {
                                                lc.common.z = (int)s_aLightingDescs[j].Value;
                                                break;
                                            }
                                        }
                                        ImGui::EndCombo();
                                    }
                                }
                                ImGui::Spacing();

                                //position
                                glm::vec3 vPosition = lc.position;
                                String namePosition = "Position - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::DragFloat3(namePosition.c_str(), &vPosition[0], 0.01f, -FLT_MAX, FLT_MAX))
                                {
                                    lc.position = vPosition;
                                }
                                ImGui::Spacing();

                                //Euler Angle
                                String nameEulerAngle = "EulerAngle - " + VulkanUtilString::SaveInt(index);
                                glm::vec3 vEulerAngle = VulkanMath::ToEulerAngles(lc.direction);
                                if (ImGui::DragFloat3(nameEulerAngle.c_str(), &vEulerAngle[0], 0.1f, -180, 180))
                                {
                                    lc.direction = VulkanMath::ToDirection(vEulerAngle);
                                }
                                //direction
                                glm::vec3 vDirection = lc.direction;
                                String nameDirection = "Direction - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::DragFloat3(nameDirection.c_str(), &vDirection[0], 0.0001f, -1.0f, 1.0f))
                                {
                                    
                                }
                                ImGui::Spacing();

                                //ambient
                                String nameAmbient = "Ambient - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameAmbient.c_str(), (float*)&lc.ambient))
                                {

                                }
                                ImGui::Spacing();

                                //diffuse
                                String nameDiffuse = "Diffuse - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameDiffuse.c_str(), (float*)&lc.diffuse))
                                {

                                }
                                ImGui::Spacing();

                                //specular
                                String nameSpecular = "Specular - " + VulkanUtilString::SaveInt(index);
                                if (ImGui::ColorEdit4(nameSpecular.c_str(), (float*)&lc.specular))
                                {

                                }
                                ImGui::Spacing();

                                if (lc.common.x == (int)Vulkan_Light_Directional)
                                {
                                    
                                }
                                else if (lc.common.x == (int)Vulkan_Light_Point)
                                {
                                    //falloffStart
                                    float fFalloffStart = lc.falloffStart;
                                    String nameFalloffStart = "FalloffStart - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffStart.c_str(), &fFalloffStart, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffStart = fFalloffStart;
                                    }
                                    ImGui::Spacing();

                                    //falloffEnd
                                    float fFalloffEnd = lc.falloffEnd;
                                    String nameFalloffEnd = "FalloffEnd - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffEnd.c_str(), &fFalloffEnd, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffEnd = fFalloffEnd;
                                    }
                                }
                                else if (lc.common.x == (int)Vulkan_Light_Spot)
                                {
                                    //falloffStart
                                    float fFalloffStart = lc.falloffStart;
                                    String nameFalloffStart = "FalloffStart - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffStart.c_str(), &fFalloffStart, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffStart = fFalloffStart;
                                    }
                                    ImGui::Spacing();

                                    //falloffEnd
                                    float fFalloffEnd = lc.falloffEnd;
                                    String nameFalloffEnd = "FalloffEnd - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameFalloffEnd.c_str(), &fFalloffEnd, 0.001f, 0.01f, 10.0f))
                                    {
                                        lc.falloffEnd = fFalloffEnd;
                                    }
                                    ImGui::Spacing();

                                    //spotPower
                                    float fSpotPower = lc.common.w;
                                    String nameSpotPower = "SpotPower - " + VulkanUtilString::SaveInt(index);
                                    if (ImGui::DragFloat(nameSpotPower.c_str(), &fSpotPower, 0.01f, 0.1f, 200.0f))
                                    {
                                        lc.common.w = fSpotPower;
                                    }
                                }
                                
                            }
                        }
                    void VulkanWindow::passConstantsConfig()
                    {
                        if (ImGui::CollapsingHeader("PassConstants Settings"))
                        {
                            //g_AmbientLight
                            if (ImGui::ColorEdit4("Global AmbientLight", (float*)&(this->passCB.g_AmbientLight)))
                            {
                                
                            }
                        }
                        ImGui::Separator();
                        ImGui::Spacing();
                    }
                    void VulkanWindow::modelConfig()
                    {

                    }

                void VulkanWindow::endRenderImgui()
                {
                    ImGui::Render();
                }

            void VulkanWindow::updateRenderCommandBuffers_CustomBeforeDefault()
            {
                
            }
            void VulkanWindow::updateRenderCommandBuffers_Default()
            {
                VkCommandBuffer& commandBuffer = this->poCommandBuffersGraphics[this->poSwapChainImageIndex];
                if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: Failed to reset render command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }

                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = 0; // Optional
                beginInfo.pInheritanceInfo = nullptr; // Optional

                if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: vkBeginCommandBuffer: Failed to begin recording render command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
                {
                    updateRenderPass_SyncComputeGraphics(commandBuffer);

                    updateRenderPass_CustomBeforeDefault(commandBuffer);
                    {
                        updateRenderPass_Default(commandBuffer);
                    }
                    updateRenderPass_CustomAfterDefault(commandBuffer);
                }
                if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                {
                    String msg = "*********************** VulkanWindow::updateRenderCommandBuffers_Default: vkEndCommandBuffer: Failed to record render command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg);
                }
            }
                void VulkanWindow::updateRenderPass_SyncComputeGraphics(VkCommandBuffer& commandBuffer)
                {

                }
                void  VulkanWindow::updateRenderPass_CustomBeforeDefault(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::updateRenderPass_Default(VkCommandBuffer& commandBuffer)
                {
                    beginRenderPass(commandBuffer,
                                    this->poRenderPass,
                                    this->poSwapChainFrameBuffers[this->poSwapChainImageIndex],
                                    this->poOffset,
                                    this->poExtent,
                                    this->cfg_colorBackground,
                                    1.0f,
                                    0);
                    {
                        //1> Viewport
                        bindViewport(commandBuffer, this->poViewport, this->poScissor);
                    
                        //2> Normal Render Pass
                        drawMeshDefault(commandBuffer);
                        drawMeshDefault_Custom(commandBuffer);

                        //3> ImGui Pass
                        drawMeshDefault_Imgui(commandBuffer);
                    }
                    endRenderPass(commandBuffer);
                }
                    void VulkanWindow::drawMeshDefault(VkCommandBuffer& commandBuffer)
                    {
                        if (this->poVertexBuffer == nullptr)
                            return;

                        VkPipeline vkPipeline = this->poPipelineGraphics;
                        if (this->cfg_isWireFrame)
                            vkPipeline = this->poPipelineGraphics_WireFrame;
                        bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline);

                        VkBuffer vertexBuffers[] = { this->poVertexBuffer };
                        VkDeviceSize offsets[] = { 0 };
                        bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);

                        if (this->poDescriptorSets.size() > 0)
                        {
                            bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &this->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
                        }
                        if (this->poIndexBuffer != VK_NULL_HANDLE)
                        {
                            bindIndexBuffer(commandBuffer, this->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

                            drawIndexed(commandBuffer, this->poIndexCount, 1, 0, 0, 0);
                        }
                        else
                        {
                            draw(commandBuffer, this->poVertexCount, 1, 0, 0);
                        }
                    }
                    void VulkanWindow::drawMeshDefault_Custom(VkCommandBuffer& commandBuffer)
                    {

                    }
                    void VulkanWindow::drawMeshDefault_Imgui(VkCommandBuffer& commandBuffer)
                    {
                        if (HasConfig_Imgui())
                        {
                            ImDrawData* main_draw_data = ImGui::GetDrawData();
                            vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
                            ImGui_ImplVulkan_RenderDrawData(main_draw_data, commandBuffer);
                        }
                    }
                void VulkanWindow::updateRenderPass_CustomAfterDefault(VkCommandBuffer& commandBuffer)
                {

                }
                    void VulkanWindow::beginRenderPass(VkCommandBuffer& commandBuffer, 
                                                       const VkRenderPass& renderPass, 
                                                       const VkFramebuffer& frameBuffer,
                                                       const VkOffset2D& offset,
                                                       const VkExtent2D& extent,
                                                       const glm::vec4& clBg,
                                                       float depth,
                                                       uint32_t stencil)
                    {
                        VkRenderPassBeginInfo renderPassInfo = {};
                        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                        renderPassInfo.renderPass = renderPass;
                        renderPassInfo.framebuffer = frameBuffer;
                        renderPassInfo.renderArea.offset = offset;
                        renderPassInfo.renderArea.extent = extent;

                        std::array<VkClearValue, 2> clearValues = {};
                        VkClearColorValue colorValue = { clBg.x, clBg.y, clBg.z, clBg.w };
                        clearValues[0].color = colorValue;
                        VkClearDepthStencilValue depthStencilValue;
                        depthStencilValue.depth = depth;
                        depthStencilValue.stencil = stencil;
                        clearValues[1].depthStencil = depthStencilValue;
                        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
                        renderPassInfo.pClearValues = clearValues.data();

                        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                    }
                        void VulkanWindow::bindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor)
                        {
                            VkViewport viewport = vkViewport;
                            if (cfg_isNegativeViewport)
                            {
                                viewport.y = viewport.height - viewport.y;
                                viewport.height = -viewport.height;
                            }   
                            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
                            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
                        }
                        void VulkanWindow::bindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline)
                        {
                            vkCmdBindPipeline(commandBuffer, pipelineBindPoint, vkPipeline);
                        }
                        void VulkanWindow::bindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
                        {
                            vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
                        }
                        void VulkanWindow::bindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType)
                        {
                            vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, offset, indexType);
                        }
                        void VulkanWindow::bindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
                        {
                            vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
                        }
                        void VulkanWindow::draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
                        {
                            vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
                        }
                        void VulkanWindow::drawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
                        {   
                            vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
                        }
                        void VulkanWindow::drawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
                        {
                            vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
                        }
                        void VulkanWindow::drawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
                        {
                            vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
                        }
                    void VulkanWindow::endRenderPass(VkCommandBuffer& commandBuffer)
                    {
                        vkCmdEndRenderPass(commandBuffer);
                    }

                    void VulkanWindow::dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ)
                    {
                        vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
                    }
                    void VulkanWindow::dispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset)
                    {
                        vkCmdDispatchIndirect(commandBuffer, buffer, offset);
                    }   

        void VulkanWindow::updateRenderCommandBuffers_CustomAfterDefault()
        {   

        }


        void VulkanWindow::render()
        {
            //1> Wait
            if (this->poImagesInFlight[this->poSwapChainImageIndex] != VK_NULL_HANDLE)
            {
                vkWaitForFences(this->poDevice, 1, &this->poImagesInFlight[this->poSwapChainImageIndex], VK_TRUE, UINT64_MAX);
            }
            this->poImagesInFlight[this->poSwapChainImageIndex] = this->poInFlightFences[this->poCurrentFrame];

            //2> Submit Command
            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

            VkSemaphoreVector aWaitSemaphores;
            if (this->poComputeWaitSemaphore != VK_NULL_HANDLE)
                aWaitSemaphores.push_back(this->poComputeWaitSemaphore);
            VkSemaphore presentCompleteSemaphore = this->poPresentCompleteSemaphores[this->poCurrentFrame];
            aWaitSemaphores.push_back(presentCompleteSemaphore);

            VkSemaphoreVector aSignalSemaphores;
            if (this->poGraphicsWaitSemaphore != VK_NULL_HANDLE)
                aSignalSemaphores.push_back(this->poGraphicsWaitSemaphore);
            VkSemaphore renderCompleteSemaphore = this->poRenderCompleteSemaphores[this->poCurrentFrame];
            aSignalSemaphores.push_back(renderCompleteSemaphore);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            //Command Buffer
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBuffersGraphics[this->poSwapChainImageIndex];
            //WaitSemaphores
            submitInfo.waitSemaphoreCount = static_cast<uint32_t>(aWaitSemaphores.size());
            submitInfo.pWaitSemaphores = aWaitSemaphores.data();
            //WaitDstStageMask
            submitInfo.pWaitDstStageMask = waitStages;
            //SignalSemaphores
            submitInfo.signalSemaphoreCount = static_cast<uint32_t>(aSignalSemaphores.size());
            submitInfo.pSignalSemaphores = aSignalSemaphores.data();
            vkResetFences(this->poDevice, 1, &this->poInFlightFences[this->poCurrentFrame]);
            if (vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, this->poInFlightFences[this->poCurrentFrame]) != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::render: Failed to submit render command buffer !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }

            //3> Present
            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = &renderCompleteSemaphore;
            VkSwapchainKHR swapChains[] = { this->poSwapChain };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &this->poSwapChainImageIndex;
            VkResult result = vkQueuePresentKHR(this->poQueuePresent, &presentInfo);
            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->isFrameBufferResized) 
            {
                this->isFrameBufferResized = false;
                recreateSwapChain();
            } 
            else if (result != VK_SUCCESS) 
            {
                String msg = "*********************** VulkanWindow::render: Failed to present swap chain image !";
                Util_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
        }
    void VulkanWindow::endRender()
    {
        this->poCurrentFrame = (this->poCurrentFrame + 1) % s_maxFramesInFight;
    }

    void VulkanWindow::cleanup()
    {
        Util_LogInfo("---------- VulkanWindow::cleanup start ----------");
        {
            //0> cleanupCustom
            cleanupCustom();

            //1> cleanupSwapChain
            cleanupSwapChain();

            //2> imgui
            destroyVkDescriptorPool(this->imgui_DescriptorPool);
            this->imgui_DescriptorPool = VK_NULL_HANDLE;

            //3> cleanupTexture
            cleanupTexture();

            //4> VkPipelineCache
            destroyVkPipelineCache(this->poPipelineCache);
            this->poPipelineCache = VK_NULL_HANDLE;

            //5> DescriptorSetLayout
            destroyVkDescriptorSetLayout(this->poDescriptorSetLayout);

            //6> cleanupVertexIndexBuffer
            cleanupVertexIndexBuffer();

            //7> destroyTerrain
            destroyTerrain();
            
            //8> Semaphores
            for (size_t i = 0; i < s_maxFramesInFight; i++) 
            {
                destroyVkSemaphore(this->poRenderCompleteSemaphores[i]);
                destroyVkSemaphore(this->poPresentCompleteSemaphores[i]);
                destroyVkFence(this->poInFlightFences[i]);
            }
            this->poRenderCompleteSemaphores.clear();
            this->poPresentCompleteSemaphores.clear();
            this->poInFlightFences.clear();

            destroyVkSemaphore(this->poGraphicsWaitSemaphore);
            this->poGraphicsWaitSemaphore = VK_NULL_HANDLE;
            destroyVkSemaphore(this->poComputeWaitSemaphore);
            this->poComputeWaitSemaphore = VK_NULL_HANDLE;

            //9> Imgui
            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_Shutdown();
	            ImGui_ImplGlfw_Shutdown();
            }

            //10> CommandPool
            destroyVkCommandPool(this->poCommandPoolGraphics);
            destroyVkCommandPool(this->poCommandPoolCompute);
            
            //11> Device
            destroyVkDevice(this->poDevice);
            this->poDevice = VK_NULL_HANDLE;
            if (s_isEnableValidationLayers)
            {
                destroyDebugUtilsMessengerEXT(this->poInstance, this->poDebugMessenger, nullptr);
            }
            //12> Surface
            destroyVkSurfaceKHR(this->poSurface);
            this->poSurface = VK_NULL_HANDLE;

            //13> Instance
            destroyVkInstance(this->poInstance);
            this->poInstance = VK_NULL_HANDLE;
        }
        Util_LogInfo("---------- VulkanWindow::cleanup finish ----------");
    }
        void VulkanWindow::cleanupCustom()
        {

        }
        void VulkanWindow::cleanupTexture()
        {
            destroyVkImage(this->poTextureImage, this->poTextureImageMemory, this->poTextureImageView);
            destroyVkImageSampler(this->poTextureSampler);
            this->poTextureImage = VK_NULL_HANDLE;
            this->poTextureImageMemory = VK_NULL_HANDLE;
            this->poTextureImageView = VK_NULL_HANDLE;
            this->poTextureSampler = VK_NULL_HANDLE;            
        }
        void VulkanWindow::cleanupVertexIndexBuffer()
        {
            //1> VertexBuffer
            if (this->poVertexBuffer != VK_NULL_HANDLE)
            {
                destroyVkBuffer(this->poVertexBuffer, this->poVertexBufferMemory);
            }
            this->poVertexBuffer = VK_NULL_HANDLE;
            this->poVertexBufferMemory = VK_NULL_HANDLE;
            this->poVertexCount = 0;
            this->poVertexBuffer_Size = 0;
            this->poVertexBuffer_Data = nullptr;

            //2> IndexBuffer
            if (this->poIndexBuffer != VK_NULL_HANDLE)
            {
                destroyVkBuffer(this->poIndexBuffer, this->poIndexBufferMemory);
            }
            this->poIndexBuffer = VK_NULL_HANDLE;
            this->poIndexBufferMemory = VK_NULL_HANDLE;
            this->poIndexCount = 0;
            this->poIndexBuffer_Size = 0;
            this->poIndexBuffer_Data = nullptr;
        }
        void VulkanWindow::cleanupSwapChain()
        {
            Util_LogInfo("----- VulkanWindow::cleanupSwapChain start -----");
            {
                //0> Custom
                cleanupSwapChain_Custom();

                //1> DepthImage
                destroyVkImage(this->poDepthImage, this->poDepthImageMemory, this->poDepthImageView);
                
                //2> ColorImage
                destroyVkImage(this->poColorImage, this->poColorImageMemory, this->poColorImageView);
                
                //3> SwapChainFrameBuffers
                size_t count = this->poSwapChainFrameBuffers.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkFramebuffer& frameBuffer = this->poSwapChainFrameBuffers[i];
                    destroyVkFramebuffer(frameBuffer);
                }
                this->poSwapChainFrameBuffers.clear();

                //4> CommandBuffers
                if (this->poCommandBuffersGraphics.size() > 0)
                {
                    freeCommandBuffers(this->poCommandPoolGraphics, (uint32_t)this->poCommandBuffersGraphics.size(), this->poCommandBuffersGraphics.data());
                }
                this->poCommandBuffersGraphics.clear();
                if (this->poCommandPoolCompute != VK_NULL_HANDLE &&
                    this->poCommandBufferCompute != VK_NULL_HANDLE)
                {
                    freeCommandBuffers(this->poCommandPoolCompute, 1, &(this->poCommandBufferCompute));
                }
                
                //5> PipelineGraphics
                destroyVkPipeline(this->poPipelineGraphics);
                this->poPipelineGraphics = VK_NULL_HANDLE;
                destroyVkPipeline(this->poPipelineGraphics_WireFrame);
                this->poPipelineGraphics_WireFrame = VK_NULL_HANDLE;
                destroyVkPipelineLayout(this->poPipelineLayout);
                this->poPipelineLayout = VK_NULL_HANDLE;
                destroyVkRenderPass(this->poRenderPass);
                this->poRenderPass = VK_NULL_HANDLE;

                destroyTerrainGraphicsPipeline();
                destroyTerrainGraphicsDescriptorSet();

                //6> SwapChainImageViews
                count = this->poSwapChainImageViews.size();
                for (size_t i = 0; i < count; i++)
                {
                    destroyVkImageView(this->poSwapChainImageViews[i]);
                }
                this->poSwapChainImageViews.clear();
                destroyVkSwapchainKHR(this->poSwapChain);
                this->poSwapChain = VK_NULL_HANDLE;

                //7> ConstBuffers
                count = this->poBuffers_PassCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_PassCB[i], this->poBuffersMemory_PassCB[i]);
                }
                this->poBuffers_PassCB.clear();
                this->poBuffersMemory_PassCB.clear();

                count = this->poBuffers_ObjectCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
                }
                this->poBuffers_ObjectCB.clear();
                this->poBuffersMemory_ObjectCB.clear();

                count = this->poBuffers_MaterialCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_MaterialCB[i], this->poBuffersMemory_MaterialCB[i]);
                }
                this->poBuffers_MaterialCB.clear();
                this->poBuffersMemory_MaterialCB.clear();

                count = this->poBuffers_InstanceCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    destroyVkBuffer(this->poBuffers_InstanceCB[i], this->poBuffersMemory_InstanceCB[i]);
                }
                this->poBuffers_InstanceCB.clear();
                this->poBuffersMemory_InstanceCB.clear();

                //8> DescriptorPool
                destroyVkDescriptorPool(this->poDescriptorPool);
                this->poDescriptorPool = VK_NULL_HANDLE;
            }
            Util_LogInfo("----- VulkanWindow::cleanupSwapChain finish -----");
        }
            void VulkanWindow::cleanupSwapChain_Custom()
            {

            }
    void VulkanWindow::recreateSwapChain()
    {
        Util_LogInfo("++++++++++ VulkanWindow::recreateSwapChain start ++++++++++");
        {
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(this->pWindow, &width, &height);
            while (width == 0 || height == 0)
            {
                glfwGetFramebufferSize(this->pWindow, &width, &height);
                glfwWaitEvents();
                if (glfwWindowShouldClose(this->pWindow)) 
                {
                    //Closing a minimized window
                    cleanup();
                    this->isMinimizedWindowNeedClose = true;
                    return;
                }
            }
            
            vkDeviceWaitIdle(this->poDevice);

            cleanupSwapChain();

            createDescriptorPool();
            createSwapChain();
            createSwapChainImageViews();
            if (HasConfig_MASS())
            {
                createColorResources();
            }  
            createDepthResources();

            createRenderPasses();
            createFramebuffers();

            createConstBuffers();
            preparePipeline();
            createGraphicsPipeline();
            createComputePipeline();
            createDescriptorSets();
            createCommandBuffers();

            recreateSwapChain_Custom();

            this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_SetMinImageCount(this->imgui_MinimalSwapchainImages);
            }
            cameraReset();
        }
        Util_LogInfo("++++++++++ VulkanWindow::recreateSwapChain finish ++++++++++");
    }
        void VulkanWindow::recreateSwapChain_Custom()
        {

        }

    VkResult VulkanWindow::createDebugUtilsMessengerEXT(VkInstance instance, 
                                                        const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                                        const VkAllocationCallbacks* pAllocator,
                                                        VkDebugUtilsMessengerEXT* pDebugMessenger)
    {
        PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) 
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } 
        else 
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void VulkanWindow::destroyDebugUtilsMessengerEXT(VkInstance instance,
                                                     VkDebugUtilsMessengerEXT debugMessenger,
                                                     const VkAllocationCallbacks* pAllocator)
    {
        PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) 
        {
            func(instance, debugMessenger, pAllocator);
        }
    } 

}; //LostPeter