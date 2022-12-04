#include "../include/preinclude.h"
#include "../include/vulkanwindow.h"
#include "../include/vulkanmeshloader.h"
#include "../include/vulkancamera.h"
#include "../include/vulkantimer.h"

namespace LostPeter
{
    int VulkanWindow::versionVulkan_Major = 1;
    int VulkanWindow::versionVulkan_Minor = 2;

#if UTIL_DEBUG == 1
    bool VulkanWindow::s_isEnableValidationLayers = true;
#else
    bool VulkanWindow::s_isEnableValidationLayers = false;
#endif

    int VulkanWindow::s_maxFramesInFight = 2;

    VulkanWindow::VulkanWindow(int width, int height, std::string name)
        : VulkanBase(width, height, name)
        , poInstance(nullptr)
        , poDebugMessenger(nullptr)
        , poPhysicalDevice(nullptr)
        , poDevice(nullptr)
        , poMSAASamples(VK_SAMPLE_COUNT_1_BIT)
        , poQueueGraphics(nullptr)
        , poQueuePresent(nullptr)
        , poSurface(nullptr)
        , poSwapChain(nullptr)
        , poColorImage(nullptr)
        , poColorImageMemory(nullptr)
        , poColorImageView(nullptr)
        , poDepthImage(nullptr)
        , poDepthImageMemory(nullptr)
        , poDepthImageView(nullptr)
        , poRenderPass(nullptr)
        , poDescriptorSetLayout(nullptr)
        , poCommandPool(nullptr) 

        , poVertexCount(0)
        , poVertexBuffer_Size(0)
        , poVertexBuffer_Data(nullptr)
        , poVertexBuffer(nullptr)
        , poVertexBufferMemory(nullptr)
        , poIndexCount(0)
        , poIndexBuffer_Size(0)
        , poIndexBuffer_Data(nullptr)
        , poIndexBuffer(nullptr)
        , poIndexBufferMemory(nullptr)
        , poMatWorld(MathUtil::Identity4x4())

        , poTypeVertex(VertexType_Default)
        , poPipelineLayout(nullptr)
        , poPipelineGraphics(nullptr)
        , poPipelineGraphics_WireFrame(nullptr)

        , poTextureImage(nullptr)
        , poTextureImageMemory(nullptr)
        , poTextureImageView(nullptr)
        , poTextureSampler(nullptr)

        , poDescriptorPool(nullptr)

        , poCurrentFrame(0)
        , poSwapChainImageIndex(0)

        , framebufferResized(false)

        , cfg_colorBackground(0.0f, 0.2f, 0.4f, 1.0f)
        , cfg_isMSAA(false)
        , cfg_isImgui(false)
        , cfg_isWireFrame(false)
        , cfg_isRotate(true)
        , cfg_isNegativeViewport(true)
        , cfg_vkPrimitiveTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
        , cfg_vkFrontFace(VK_FRONT_FACE_CLOCKWISE)
        , cfg_vkPolygonMode(VK_POLYGON_MODE_FILL)
        , cfg_vkCullModeFlagBits(VK_CULL_MODE_BACK_BIT)
        , cfg_isDepthTest(VK_TRUE)
        , cfg_isDepthWrite(VK_TRUE)
        , cfg_DepthCompareOp(VK_COMPARE_OP_LESS) 
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
        , cfg_cameraSpeedZoom(0.01f)
        , cfg_cameraSpeedRotate(0.1f)

        , cfg_model_Path("")
        , cfg_shaderVertex_Path("")
        , cfg_shaderFragment_Path("")
        , cfg_texture_Path("")

        , imgui_IsEnable(false)
        , imgui_MinimalSwapchainImages(0)
        , imgui_DescriptorPool(nullptr)
        , imgui_PathIni("")
        , imgui_PathLog("")

        , pSceneManager(nullptr)

        , pCamera(nullptr)

        , mouseButtonDownLeft(false)
        , mouseButtonDownRight(false)
    {
        this->validationLayers.push_back("VK_LAYER_KHRONOS_validation");
        this->deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        this->deviceExtensions.push_back(VK_KHR_MAINTENANCE1_EXTENSION_NAME);

        cfg_StencilOpFront.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpFront.compareOp = VK_COMPARE_OP_LESS;
        cfg_StencilOpFront.compareMask = 0;
        cfg_StencilOpFront.writeMask = 0;
        cfg_StencilOpFront.reference = 0;

        cfg_StencilOpBack.failOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.passOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.depthFailOp = VK_STENCIL_OP_KEEP;
        cfg_StencilOpBack.compareOp = VK_COMPARE_OP_LESS;
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

    bool VulkanWindow::OnBeginRender()
    {
        return beginRender();
    }
        void VulkanWindow::OnUpdate()
        {
            update();
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
                float fRotYAngle = fX * this->cfg_cameraSpeedRotate;
                float fRotXAngle = -fY * this->cfg_cameraSpeedRotate;
                glm::vec3 vEulerAngles = pCamera->GetEulerAngles();
                vEulerAngles.x += fRotXAngle;
                vEulerAngles.y += fRotYAngle;
                pCamera->SetEulerAngles(vEulerAngles);

                this->pCamera->UpdateProjectionMatrix();
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
            this->pCamera->UpdateProjectionMatrix();
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

            //6> createDescriptorObjects
            createDescriptorObjects();
            
            //7> Create Pipeline Objects
            createPipelineObjects();

            //8> Create Sync Objects
            createSyncObjects();

            //9> isCreateDevice
            this->isCreateDevice = true;
        }
        Util_LogInfo("**********<1> VulkanWindow::createPipeline finish **********");
    }
    //glfw: whenever the window size changed (by OS or user resize) this callback function executes
    void framebuffer_size_callback(GLFWwindow *window, int width, int height)
    {
        VulkanWindow* pWnd = (VulkanWindow*)glfwGetWindowUserPointer(window);
        pWnd->framebufferResized = true;
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
    void VulkanWindow::createInstance()
    {
        if (s_isEnableValidationLayers && 
            !checkValidationLayerSupport()) 
        {
            std::string msg = "VulkanWindow::createInstance: Validation layers requested, but not available !";
            Util_LogError(msg.c_str());
            throw std::runtime_error(msg.c_str());
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "VulkanUtil";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        std::vector<const char*> extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
        if (s_isEnableValidationLayers) 
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
            createInfo.ppEnabledLayerNames = this->validationLayers.data();

            populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
        } 
        else 
        {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &this->poInstance) != VK_SUCCESS)
        {
            throw std::runtime_error("VulkanWindow::createInstance: Failed to create vulkan instance !");
        }

        Util_LogInfo("<1-2-1> VulkanWindow::createInstance finish !");
    }
        bool VulkanWindow::checkValidationLayerSupport()
        {
            uint32_t layerCount = 0;
            vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

            std::vector<VkLayerProperties> availableLayers(layerCount);
            vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

            int count = (int)this->validationLayers.size();
            for (int i = 0; i < count; i++)
            {
                const char* layerName = this->validationLayers[i];
                bool layerFound = false;

                int count_avai = (int)availableLayers.size();
                for (int j = 0; j < count_avai; j++)
                {
                    VkLayerProperties& layerProperties = availableLayers[j];
                    if (strcmp(layerName, layerProperties.layerName) == 0)
                    {
                        layerFound = true;
                        break;
                    }
                }

                if (!layerFound)
                    return false;
            }

            return true;
        }
        std::vector<const char*> VulkanWindow::getRequiredExtensions()
        {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
            if (s_isEnableValidationLayers) {
                extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
            }
            return extensions;
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
                throw std::runtime_error("VulkanWindow::setUpDebugMessenger: Failed to set up debug messenger !");
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
            throw std::runtime_error("VulkanWindow::createSurface: Failed to create window surface, result: " + os.str());
        }

        Util_LogInfo("<1-2-3> VulkanWindow::createSurface finish !");
    }

    void VulkanWindow::pickPhysicalDevice()
    {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, nullptr);

        if (deviceCount == 0)
        {
            throw std::runtime_error("VulkanWindow::pickPhysicalDevice: Failed to find GPUs width Vulkan support !");
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(this->poInstance, &deviceCount, devices.data());

        int count_device = (int)devices.size();
        for (int i = 0; i < count_device; i++)
        {
            VkPhysicalDevice& device = devices[i];
            int indexGraphics = -1;
            int indexPresent = -1;
            if (isDeviceSuitable(device, indexGraphics, indexPresent))
            {
                this->poPhysicalDevice = device;
                this->graphicsIndex = indexGraphics;
                this->presentIndex = indexPresent;
                if (HasConfig_MASS())
                {
                    this->poMSAASamples = getMaxUsableSampleCount();
                }
                break;
            }
        } 

        if (this->poPhysicalDevice == nullptr) 
        {
            throw std::runtime_error("VulkanWindow::pickPhysicalDevice: Failed to find a suitable GPU !");
        }

        Util_LogInfo("<1-2-4> VulkanWindow::pickPhysicalDevice finish !");
    }
        void VulkanWindow::findQueueFamilies(VkPhysicalDevice device, int& indexGraphics, int& indexPresent)
        {
            uint32_t queueFamilyCount = 0;
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

            std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
            vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

            int count_device = (int)queueFamilies.size();
            for (int i = 0; i < count_device; i++)
            {
                VkQueueFamilyProperties& queueFamily = queueFamilies[i];

                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                    indexGraphics = i;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->poSurface, &presentSupport);
                if (presentSupport)
                {
                    indexPresent = i;
                }

                if (indexGraphics >= 0 && indexPresent >= 0)
                    break;
            }
        }
        bool VulkanWindow::checkDeviceExtensionSupport(VkPhysicalDevice device) 
        {
            uint32_t extensionCount;
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

            std::vector<VkExtensionProperties> availableExtensions(extensionCount);
            vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

            std::set<std::string> requiredExtensions(this->deviceExtensions.begin(), this->deviceExtensions.end());
            int count = (int)availableExtensions.size();
            for (int i = 0; i < count; i++)
            {
                const VkExtensionProperties& extension = availableExtensions[i];
                requiredExtensions.erase(extension.extensionName);
            }

            return requiredExtensions.empty();
        }
        VkSampleCountFlagBits VulkanWindow::getMaxUsableSampleCount()
        {
            VkPhysicalDeviceProperties physicalDeviceProperties;
            vkGetPhysicalDeviceProperties(this->poPhysicalDevice, &physicalDeviceProperties);

            VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
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
        bool VulkanWindow::isDeviceSuitable(VkPhysicalDevice device, int& indexGraphics, int& indexPresent)
        {   
            findQueueFamilies(device, indexGraphics, indexPresent);
            if (indexGraphics == -1 || indexPresent == -1)
                return false;
            
            bool extensionsSupported = checkDeviceExtensionSupport(device);
            bool swapChainAdequate = false;
            if (extensionsSupported) 
            {
                querySwapChainSupport(device, this->swapChainSupport);
                swapChainAdequate = !this->swapChainSupport.formats.empty() && !this->swapChainSupport.presentModes.empty();
            }
            return swapChainAdequate;
        }

    void VulkanWindow::createLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies;
        uniqueQueueFamilies.insert(this->graphicsIndex);
        uniqueQueueFamilies.insert(this->presentIndex);

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

        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(this->deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = this->deviceExtensions.data();

        if (s_isEnableValidationLayers) 
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(this->validationLayers.size());
            createInfo.ppEnabledLayerNames = this->validationLayers.data();
        } 
        else 
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(this->poPhysicalDevice, &createInfo, nullptr, &this->poDevice) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createLogicalDevice: Failed to create logical device !");
        }

        vkGetDeviceQueue(this->poDevice, this->graphicsIndex, 0, &this->poQueueGraphics);
        vkGetDeviceQueue(this->poDevice, this->presentIndex, 0, &this->poQueuePresent);

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
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = this->graphicsIndex;
        if (HasConfig_Imgui())
        {
            poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        }

        if (vkCreateCommandPool(this->poDevice, &poolInfo, nullptr, &this->poCommandPool) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createCommandPool: Failed to create command pool !");
        }

        Util_LogInfo("<1-4-1> VulkanWindow::createCommandPool finish !");
    }
    VkCommandBuffer VulkanWindow::beginSingleTimeCommands() 
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = this->poCommandPool;
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

        vkFreeCommandBuffers(this->poDevice, this->poCommandPool, 1, &commandBuffer);
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
            this->graphicsIndex,
            this->presentIndex
        };

        if (this->graphicsIndex != this->presentIndex)
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
            throw std::runtime_error("VulkanWindow::createSwapChain: Failed to create swap chain, result: " + os.str());
        }

        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, nullptr);
        this->poSwapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(this->poDevice, this->poSwapChain, &imageCount, this->poSwapChainImages.data());

        this->poSwapChainImageFormat = surfaceFormat.format;
        this->poSwapChainExtent = extent;

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

                actualExtent.width = MathUtil::Clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
                actualExtent.height = MathUtil::Clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

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
            VkOffset2D offset;
            offset.x = 0;
            offset.y = 0;
            this->poScissor.offset = offset;
            this->poScissor.extent = this->poSwapChainExtent;
        }

    void VulkanWindow::createSwapChainImageViews()
    {
        int count = (int)this->poSwapChainImages.size();
        this->poSwapChainImageViews.resize(count);

        for (int i = 0; i < count; i++)
        {
            this->poSwapChainImageViews[i] = createImageView(this->poSwapChainImages[i], this->poSwapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        Util_LogInfo("<1-5-2> VulkanWindow::createSwapChainImageViews finish !");
    }
        VkImageView VulkanWindow::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) 
        {
            VkImageViewCreateInfo viewInfo = {};
            viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            viewInfo.image = image;
            viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            viewInfo.format = format;
            viewInfo.subresourceRange.aspectMask = aspectFlags;
            viewInfo.subresourceRange.baseMipLevel = 0;
            viewInfo.subresourceRange.levelCount = mipLevels;
            viewInfo.subresourceRange.baseArrayLayer = 0;
            viewInfo.subresourceRange.layerCount = 1;

            VkImageView imageView;
            if (vkCreateImageView(this->poDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createImageView: Failed to create texture image view !");
            }
            return imageView;
        }
        void VulkanWindow::createColorResources()
        {
            VkFormat colorFormat = this->poSwapChainImageFormat;

            createImage(this->poSwapChainExtent.width, this->poSwapChainExtent.height, 1, this->poMSAASamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->poColorImage, this->poColorImageMemory);
            this->poColorImageView = createImageView(this->poColorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

            Util_LogInfo("<1-5-3> VulkanWindow::createColorResources finish !");
        }
        void VulkanWindow::createDepthResources()
        {
            VkFormat depthFormat = findDepthFormat();

            createImage(this->poSwapChainExtent.width, this->poSwapChainExtent.height, 1, this->poMSAASamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, this->poDepthImage, this->poDepthImageMemory);
            this->poDepthImageView = createImageView(this->poDepthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

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

            throw std::runtime_error("VulkanWindow::findSupportedFormat: Failed to find supported format !");
        }
        VkFormat VulkanWindow::findDepthFormat() 
        {
            std::vector<VkFormat> candidates;
            candidates.push_back(VK_FORMAT_D32_SFLOAT);
            candidates.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
            candidates.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
            return findSupportedFormat(candidates,
                                    VK_IMAGE_TILING_OPTIMAL,
                                    VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
        }
        bool VulkanWindow::hasStencilComponent(VkFormat format)
        {
            return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
        }

    void VulkanWindow::createDescriptorObjects()
    {
        Util_LogInfo("*****<1-6> VulkanWindow::createDescriptorObjects start *****");
        {
            //1> createDescriptorSetLayout
            createDescriptorSetLayout();

        }
        Util_LogInfo("*****<1-6> VulkanWindow::createDescriptorObjects finish *****");
    }
    void VulkanWindow::createDescriptorSetLayout()
    {
        //0> PassConstants
        VkDescriptorSetLayoutBinding passMainLayoutBinding = {};
        passMainLayoutBinding.binding = 0;
        passMainLayoutBinding.descriptorCount = 1;
        passMainLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        passMainLayoutBinding.pImmutableSamplers = nullptr;
        passMainLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //1> ObjectConstants
        VkDescriptorSetLayoutBinding objectLayoutBinding = {};
        objectLayoutBinding.binding = 1;
        objectLayoutBinding.descriptorCount = 1;
        objectLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        objectLayoutBinding.pImmutableSamplers = nullptr;
        objectLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //2> MaterialConstants
        VkDescriptorSetLayoutBinding materialLayoutBinding = {};
        materialLayoutBinding.binding = 2;
        materialLayoutBinding.descriptorCount = 1;
        materialLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        materialLayoutBinding.pImmutableSamplers = nullptr;
        materialLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //3> InstanceConstants
        VkDescriptorSetLayoutBinding instanceLayoutBinding = {};
        instanceLayoutBinding.binding = 3;
        instanceLayoutBinding.descriptorCount = 1;
        instanceLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        instanceLayoutBinding.pImmutableSamplers = nullptr;
        instanceLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

        //4> Texture Sampler
        VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
        samplerLayoutBinding.binding = 4;
        samplerLayoutBinding.descriptorCount = 1;
        samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        samplerLayoutBinding.pImmutableSamplers = nullptr;
        samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

        std::vector<VkDescriptorSetLayoutBinding> bindings;
        bindings.push_back(passMainLayoutBinding);
        bindings.push_back(objectLayoutBinding);
        bindings.push_back(materialLayoutBinding);
        bindings.push_back(instanceLayoutBinding);
        if (!this->cfg_texture_Path.empty())
        {
            bindings.push_back(samplerLayoutBinding);
        }
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
        layoutInfo.pBindings = bindings.data();

        if (vkCreateDescriptorSetLayout(this->poDevice, &layoutInfo, nullptr, &this->poDescriptorSetLayout) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createDescriptorSetLayout: Failed to create descriptor set layout !");
        }

        Util_LogInfo("<1-6-1> VulkanWindow::createDescriptorSetLayout finish !");
    }

    void VulkanWindow::createPipelineObjects()
    {
        Util_LogInfo("*****<1-7> VulkanWindow::createPipelineObjects start *****");
        {
            //1> createRenderPass
            createRenderPass();

            //2> createFramebuffers
            createFramebuffers();
        }
        Util_LogInfo("*****<1-7> VulkanWindow::createPipelineObjects finish *****");
    }
    void VulkanWindow::createRenderPass()
    {
        if (HasConfig_Imgui())
        {
            if (HasConfig_MASS())
            {
                createRenderPass_ColorDepthImguiMSAA();
            }
            else
            {
                createRenderPass_KhrDepthImgui();
            }
        }
        else
        {
            if (HasConfig_MASS())
            {
                createRenderPass_ColorDepthMSAA();
            }
            else
            {
                createRenderPass_KhrDepth();
            }
        }
    }
    void VulkanWindow::createRenderPass_KhrDepth()
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkSubpassDescription> subpasses;
        std::vector<VkSubpassDependency> subpassesDependencies;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        attachmentSR_Color.format = this->poSwapChainImageFormat;
        attachmentSR_Color.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_Color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachments.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        attachmentSR_Depth.format = findDepthFormat();
        attachmentSR_Depth.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_Depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Depth.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Depth.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Depth);
             
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
        subpasses.push_back(subpass_SceneRender);
          
        //4> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.srcAccessMask = 0;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_SceneRender);

        //5> RenderPass Create Info
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = &attachments[0];
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses = &subpasses[0];
        renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassesDependencies.size());
        renderPassInfo.pDependencies = &subpassesDependencies[0];

        //6> vkCreateRenderPass
        if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &this->poRenderPass) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createRenderPass_KhrDepth: Failed to create render pass !");
        }

        Util_LogInfo("<1-7-1> VulkanWindow::createRenderPass_KhrDepth finish !");
    }
    void VulkanWindow::createRenderPass_KhrDepthImgui()
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkSubpassDescription> subpasses;
        std::vector<VkSubpassDependency> subpassesDependencies;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        attachmentSR_Color.format = this->poSwapChainImageFormat;
        attachmentSR_Color.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_Color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Color.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        attachmentSR_Depth.format = findDepthFormat();
        attachmentSR_Depth.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_Depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Depth.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Depth.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Depth);
              
        //3> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        attachmentImgui_Color.format = this->poSwapChainImageFormat;
        attachmentImgui_Color.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentImgui_Color.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachmentImgui_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentImgui_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentImgui_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentImgui_Color.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachmentImgui_Color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachments.push_back(attachmentImgui_Color);
        
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
        subpasses.push_back(subpass_SceneRender);

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
        subpasses.push_back(subpass_Imgui);
        
        //6> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.srcAccessMask = 0;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_SceneRender);

        //7> Subpass Dependency Imgui
       	VkSubpassDependency subpassDependency_Imgui = {};
        subpassDependency_Imgui.srcSubpass = 0;
        subpassDependency_Imgui.dstSubpass = 1;
        subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_Imgui);

        //8> RenderPass Create Info
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = &attachments[0];
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses = &subpasses[0];
        renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassesDependencies.size());
        renderPassInfo.pDependencies = &subpassesDependencies[0];

        //9> vkCreateRenderPass
        if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &this->poRenderPass) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createRenderPass_KhrDepthImgui: Failed to create render pass !");
        }

        Util_LogInfo("<1-7-1> VulkanWindow::createRenderPass_KhrDepthImgui finish !");
    }
    void VulkanWindow::createRenderPass_ColorDepthMSAA()
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkSubpassDescription> subpasses;
        std::vector<VkSubpassDependency> subpassesDependencies;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        attachmentSR_Color.format = this->poSwapChainImageFormat;
        attachmentSR_Color.samples = this->poMSAASamples;
        attachmentSR_Color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Color.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        attachmentSR_Depth.format = findDepthFormat();
        attachmentSR_Depth.samples = this->poMSAASamples;
        attachmentSR_Depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Depth.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Depth.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        attachmentSR_ColorResolve.format = this->poSwapChainImageFormat;
        attachmentSR_ColorResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_ColorResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_ColorResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_ColorResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_ColorResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_ColorResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_ColorResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; 
        attachments.push_back(attachmentSR_ColorResolve);
        
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
        subpasses.push_back(subpass_SceneRender);
          
        //5> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.srcAccessMask = 0;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_SceneRender);

        //6> RenderPass Create Info
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = &attachments[0];
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses = &subpasses[0];
        renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassesDependencies.size());
        renderPassInfo.pDependencies = &subpassesDependencies[0];

        //7> vkCreateRenderPass
        if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &this->poRenderPass) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createRenderPass_ColorDepthMSAA: Failed to create render pass !");
        }

        Util_LogInfo("<1-7-1> VulkanWindow::createRenderPass_ColorDepthMSAA finish !");
    }
    void VulkanWindow::createRenderPass_ColorDepthImguiMSAA()
    {
        std::vector<VkAttachmentDescription> attachments;
        std::vector<VkSubpassDescription> subpasses;
        std::vector<VkSubpassDependency> subpassesDependencies;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        attachmentSR_Color.format = this->poSwapChainImageFormat;
        attachmentSR_Color.samples = this->poMSAASamples;
        attachmentSR_Color.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Color.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Color.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        attachmentSR_Depth.format = findDepthFormat();
        attachmentSR_Depth.samples = this->poMSAASamples;
        attachmentSR_Depth.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        attachmentSR_Depth.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_Depth.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_Depth.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_Depth.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        attachmentSR_ColorResolve.format = this->poSwapChainImageFormat;
        attachmentSR_ColorResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentSR_ColorResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_ColorResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentSR_ColorResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentSR_ColorResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentSR_ColorResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        attachmentSR_ColorResolve.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachments.push_back(attachmentSR_ColorResolve);
        
        //4> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        attachmentImgui_Color.format = this->poSwapChainImageFormat;
        attachmentImgui_Color.samples = VK_SAMPLE_COUNT_1_BIT;
        attachmentImgui_Color.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
        attachmentImgui_Color.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachmentImgui_Color.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachmentImgui_Color.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        attachmentImgui_Color.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attachmentImgui_Color.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachments.push_back(attachmentImgui_Color);
        
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
        subpasses.push_back(subpass_SceneRender);

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
        subpasses.push_back(subpass_Imgui);
        
        //7> Subpass Dependency SceneRender
        VkSubpassDependency subpassDependency_SceneRender = {};
        subpassDependency_SceneRender.srcSubpass = VK_SUBPASS_EXTERNAL;
        subpassDependency_SceneRender.dstSubpass = 0;
        subpassDependency_SceneRender.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.srcAccessMask = 0;
        subpassDependency_SceneRender.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        subpassDependency_SceneRender.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_SceneRender);

        //8> Subpass Dependency Imgui
       	VkSubpassDependency subpassDependency_Imgui = {};
        subpassDependency_Imgui.srcSubpass = 0;
        subpassDependency_Imgui.dstSubpass = 1;
        subpassDependency_Imgui.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency_Imgui.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency_Imgui.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassesDependencies.push_back(subpassDependency_Imgui);

        //9> RenderPass Create Info
        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = &attachments[0];
        renderPassInfo.subpassCount = static_cast<uint32_t>(subpasses.size());
        renderPassInfo.pSubpasses = &subpasses[0];
        renderPassInfo.dependencyCount = static_cast<uint32_t>(subpassesDependencies.size());
        renderPassInfo.pDependencies = &subpassesDependencies[0];

        //10> vkCreateRenderPass
        if (vkCreateRenderPass(this->poDevice, &renderPassInfo, nullptr, &this->poRenderPass) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createRenderPass_ColorDepthImguiMSAA: Failed to create render pass !");
        }

        Util_LogInfo("<1-7-1> VulkanWindow::createRenderPass_ColorDepthImguiMSAA finish !");
    }
    void VulkanWindow::createFramebuffers()
    {
        size_t count = this->poSwapChainImageViews.size();
        this->poSwapChainFrameBuffers.resize(count);

        for (size_t i = 0; i < count; i++)
        {
            std::vector<VkImageView> attachments;
            if (!HasConfig_Imgui())
            {
                if (!HasConfig_MASS())
                {
                    attachments.push_back(this->poSwapChainImageViews[i]);
                    attachments.push_back(this->poDepthImageView);
                }
                else
                {
                    attachments.push_back(this->poColorImageView);
                    attachments.push_back(this->poDepthImageView);
                    attachments.push_back(this->poSwapChainImageViews[i]);
                }
            }
            else
            {
                if (!HasConfig_MASS())
                {
                    attachments.push_back(this->poSwapChainImageViews[i]);
                    attachments.push_back(this->poDepthImageView);
                    attachments.push_back(this->poSwapChainImageViews[i]);
                }
                else
                {
                    attachments.push_back(this->poColorImageView);
                    attachments.push_back(this->poDepthImageView);
                    attachments.push_back(this->poSwapChainImageViews[i]);
                    attachments.push_back(this->poSwapChainImageViews[i]);
                }
            }

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = this->poRenderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = &attachments[0];
            framebufferInfo.width = this->poSwapChainExtent.width;
            framebufferInfo.height = this->poSwapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(this->poDevice, &framebufferInfo, nullptr, &this->poSwapChainFrameBuffers[i]) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createFramebuffers: Failed to create framebuffer !");
            }
        }

        Util_LogInfo("<1-7-2> VulkanWindow::createFramebuffers finish !");
    }

    void VulkanWindow::createSyncObjects()
    {
        this->poImageAvailableSemaphores.resize(s_maxFramesInFight);
        this->poRenderFinishedSemaphores.resize(s_maxFramesInFight);
        this->poInFlightFences.resize(s_maxFramesInFight);
        this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < s_maxFramesInFight; i++) 
        {
            if (vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poImageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(this->poDevice, &semaphoreInfo, nullptr, &this->poRenderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(this->poDevice, &fenceInfo, nullptr, &this->poInFlightFences[i]) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createSyncObjects: Failed to create synchronization objects for a frame !");
            }
        }

        Util_LogInfo("*****<1-8> VulkanWindow::createSyncObjects finish *****");
    }

    void VulkanWindow::loadAssets()
    {
        Util_LogInfo("**********<2> VulkanWindow::loadAssets start **********");
        {
            //1> Create Scene
            createScene();

            //2> Create Camera
            createCamera();

            //3> loadGeometry
            if (this->pSceneManager == nullptr)
            {
                loadGeometry();
            }

            //4> Build Scene
            buildScene();

            //5> Imgui
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
        Util_LogInfo("*****<2-1> VulkanWindow::createScene start *****");
        {
            //1> createSceneManager
            createSceneManager();

        }
        Util_LogInfo("*****<2-1> VulkanWindow::createScene finish *****");
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

            //4> createGraphicsPipeline
            createGraphicsPipeline();

            //5> createDescriptor()
            createDescriptor();    

            //6> createCommandBuffers
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
                createVertexBuffer(this->poVertexBuffer_Size, this->poVertexBuffer_Data, this->poVertexBuffer, this->poVertexBufferMemory);
            }

            //3> createIndexBuffer
            if (this->poIndexBuffer_Size > 0 &&
                this->poIndexBuffer_Data != nullptr)
            {
                createIndexBuffer(this->poIndexBuffer_Size, this->poIndexBuffer_Data, this->poIndexBuffer, this->poIndexBufferMemory);
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
    void VulkanWindow::createVertexBuffer(size_t bufSize, void* pBuf, VkBuffer& vertexBuffer, VkDeviceMemory& vertexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(this->poDevice, stagingBufferMemory, 0, bufSize, 0, &data);
            memcpy(data, pBuf, bufSize);
        vkUnmapMemory(this->poDevice, stagingBufferMemory);

        createBuffer(bufSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufSize);

        vkDestroyBuffer(this->poDevice, stagingBuffer, nullptr);
        vkFreeMemory(this->poDevice, stagingBufferMemory, nullptr);

        Util_LogInfo("<2-2-1-2> VulkanWindow::createVertexBuffer finish !");
    }
    void VulkanWindow::createIndexBuffer(size_t bufSize, void* pBuf, VkBuffer& indexBuffer, VkDeviceMemory& indexBufferMemory)
    {
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(this->poDevice, stagingBufferMemory, 0, bufSize, 0, &data);
            memcpy(data, pBuf, bufSize);
        vkUnmapMemory(this->poDevice, stagingBufferMemory);

        createBuffer(bufSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufSize);

        vkDestroyBuffer(this->poDevice, stagingBuffer, nullptr);
        vkFreeMemory(this->poDevice, stagingBufferMemory, nullptr);

        Util_LogInfo("<2-2-1-3> VulkanWindow::createIndexBuffer finish !");
    }
        void VulkanWindow::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) 
        {
            VkBufferCreateInfo bufferInfo = {};
            bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
            bufferInfo.size = size;
            bufferInfo.usage = usage;
            bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateBuffer(this->poDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createBuffer: Failed to create buffer !");
            }

            VkMemoryRequirements memRequirements;
            vkGetBufferMemoryRequirements(this->poDevice, buffer, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createBuffer: Failed to allocate buffer memory !");
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

                throw std::runtime_error("VulkanWindow::findMemoryType: Failed to find suitable memory type !");
            }
            void VulkanWindow::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) 
            {
                VkCommandBuffer commandBuffer = beginSingleTimeCommands();

                VkBufferCopy copyRegion = {};
                copyRegion.size = size;
                vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

                endSingleTimeCommands(commandBuffer);
            }

    void VulkanWindow::loadTexture()
    {
        if (!this->cfg_texture_Path.empty())
        {
            createTextureImage(this->cfg_texture_Path, this->poTextureImage, this->poTextureImageMemory, this->poMipLevels);
            createTextureImageView(this->poTextureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, this->poMipLevels, this->poTextureImageView);
            createTextureSampler(this->poMipLevels, this->poTextureSampler);

            Util_LogInfo("<2-2-2> VulkanWindow::loadTexture finish !");
        }
    }
    void VulkanWindow::createTextureImage(const std::string& pathAsset_Tex, VkImage& textureImage, VkDeviceMemory& textureImageMemory, uint32_t& mipLevels)
    {
        std::string pathTexture = GetAssetFullPath(pathAsset_Tex);
        int texWidth, texHeight, texChannels;
        stbi_uc* pixels = stbi_load(pathTexture.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = texWidth * texHeight * 4;
        mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(texWidth, texHeight)))) + 1;

        if (!pixels) 
        {
            throw std::runtime_error("VulkanWindow::createTextureImage: Failed to load texture image !");
        }

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(this->poDevice, stagingBufferMemory, 0, imageSize, 0, &data);
            memcpy(data, pixels, static_cast<size_t>(imageSize));
        vkUnmapMemory(this->poDevice, stagingBufferMemory);

        stbi_image_free(pixels);

        createImage(texWidth, texHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

        transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mipLevels);
            copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
        //transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        vkDestroyBuffer(this->poDevice, stagingBuffer, nullptr);
        vkFreeMemory(this->poDevice, stagingBufferMemory, nullptr);

        generateMipmaps(textureImage, VK_FORMAT_R8G8B8A8_SRGB, texWidth, texHeight, mipLevels);
    }
        void VulkanWindow::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) 
        {
            // Check if image format supports linear blitting
            VkFormatProperties formatProperties;
            vkGetPhysicalDeviceFormatProperties(this->poPhysicalDevice, imageFormat, &formatProperties);

            if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) 
            {
                throw std::runtime_error("VulkanWindow::generateMipmaps: Texture image format does not support linear blitting !");
            }

            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

            VkImageMemoryBarrier barrier{};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.image = image;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            barrier.subresourceRange.levelCount = 1;

            int32_t mipWidth = texWidth;
            int32_t mipHeight = texHeight;

            for (uint32_t i = 1; i < mipLevels; i++) 
            {
                barrier.subresourceRange.baseMipLevel = i - 1;
                barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
                barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

                vkCmdPipelineBarrier(commandBuffer,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);

                VkImageBlit blit{};
                blit.srcOffsets[0] = {0, 0, 0};
                blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
                blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.srcSubresource.mipLevel = i - 1;
                blit.srcSubresource.baseArrayLayer = 0;
                blit.srcSubresource.layerCount = 1;
                blit.dstOffsets[0] = {0, 0, 0};
                blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                blit.dstSubresource.mipLevel = i;
                blit.dstSubresource.baseArrayLayer = 0;
                blit.dstSubresource.layerCount = 1;

                vkCmdBlitImage(commandBuffer,
                    image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                    image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                    1, &blit,
                    VK_FILTER_LINEAR);

                barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
                barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                vkCmdPipelineBarrier(commandBuffer,
                    VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                    0, nullptr,
                    0, nullptr,
                    1, &barrier);

                if (mipWidth > 1) mipWidth /= 2;
                if (mipHeight > 1) mipHeight /= 2;
            }

            barrier.subresourceRange.baseMipLevel = mipLevels - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
                0, nullptr,
                0, nullptr,
                1, &barrier);

            endSingleTimeCommands(commandBuffer);
        }
        void VulkanWindow::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) 
        {
            VkImageCreateInfo imageInfo = {};
            imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
            imageInfo.imageType = VK_IMAGE_TYPE_2D;
            imageInfo.extent.width = width;
            imageInfo.extent.height = height;
            imageInfo.extent.depth = 1;
            imageInfo.mipLevels = mipLevels;
            imageInfo.arrayLayers = 1;
            imageInfo.format = format;
            imageInfo.tiling = tiling;
            imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imageInfo.usage = usage;
            imageInfo.samples = numSamples;
            imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

            if (vkCreateImage(this->poDevice, &imageInfo, nullptr, &image) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createImage: Failed to create image !");
            }

            VkMemoryRequirements memRequirements;
            vkGetImageMemoryRequirements(this->poDevice, image, &memRequirements);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
            allocInfo.allocationSize = memRequirements.size;
            allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

            if (vkAllocateMemory(this->poDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::createImage: Failed to allocate image memory !");
            }

            vkBindImageMemory(this->poDevice, image, imageMemory, 0);
        }
        void VulkanWindow::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels) 
        {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = mipLevels;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;

            if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) 
            {
                barrier.srcAccessMask = 0;
                barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            } 
            else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) 
            {
                barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
                barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

                sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
                destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            } 
            else 
            {
                throw std::invalid_argument("VulkanWindow::transitionImageLayout: Unsupported layout transition !");
            }

            vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage, destinationStage,
                0,
                0, nullptr,
                0, nullptr,
                1, &barrier
            );

            endSingleTimeCommands(commandBuffer);
        }
        void VulkanWindow::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height) 
        {
            VkCommandBuffer commandBuffer = beginSingleTimeCommands();

            VkBufferImageCopy region = {};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;
            VkOffset3D offset;
            offset.x = 0;
            offset.y = 0;
            offset.z = 0;
            region.imageOffset = offset;
            VkExtent3D extent;
            extent.width = width;
            extent.height = height;
            extent.depth = 1;
            region.imageExtent = extent;

            vkCmdCopyBufferToImage(commandBuffer, buffer, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            endSingleTimeCommands(commandBuffer);
        }
    void VulkanWindow::createTextureImageView(VkImage& textureImage, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels, VkImageView& textureImageView)
    {
        textureImageView = createImageView(textureImage, format, aspectFlags, mipLevels);
    }
    void VulkanWindow::createTextureSampler(uint32_t mipLevels, VkSampler& textureSampler)
    {
        VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(this->poPhysicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(mipLevels);
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(this->poDevice, &samplerInfo, nullptr, &textureSampler) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createTextureSampler: Failed to create texture sampler !");
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
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_PassCB[i], this->poBuffersMemory_PassCB[i]);
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
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB[i], this->poBuffersMemory_ObjectCB[i]);
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
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_MaterialCB[i], this->poBuffersMemory_MaterialCB[i]);
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
            createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_InstanceCB[i], this->poBuffersMemory_InstanceCB[i]);
        }

        Util_LogInfo("<2-2-3-4> VulkanWindow::createInstanceCB finish !");
    }
        void VulkanWindow::buildInstanceCB()
        {
            InstanceConstants instanceConstants;
            this->instanceCBs.push_back(instanceConstants);
        }

    void VulkanWindow::createGraphicsPipeline()
    {
        VkShaderModule vertShaderModule;
        if (!this->cfg_shaderVertex_Path.empty())
        {
            vertShaderModule = createShaderModule("VertexShader: ", this->cfg_shaderVertex_Path);
        }
        VkShaderModule fragShaderModule;
        if (!this->cfg_shaderFragment_Path.empty())
        {
            fragShaderModule = createShaderModule("FragmentShader: ", this->cfg_shaderFragment_Path);
        }

        //1> Pipeline Shader Stage
        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        //2> Pipeline VertexInput State
        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        createPipelineVertexInputStateCreateInfo(vertexInputInfo);

        //3> Pipeline InputAssembly
        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = this->cfg_vkPrimitiveTopology;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        //4> Pipeline Viewport State
        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &this->poViewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &this->poScissor;

        //5> Pipeline Rasterization State
        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = this->cfg_vkPolygonMode;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = this->cfg_vkCullModeFlagBits;
        rasterizer.frontFace = this->cfg_vkFrontFace;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	    rasterizer.depthBiasClamp = 0.0f; // Optional
	    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        //6> Pipeline Multisample State
        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = this->poMSAASamples;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        //7> Pipeline DepthStencil State
        VkPipelineDepthStencilStateCreateInfo depthStencil = {};
        depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencil.depthTestEnable = this->cfg_isDepthTest;
        depthStencil.depthWriteEnable = this->cfg_isDepthWrite;
        depthStencil.depthCompareOp = this->cfg_DepthCompareOp;
        depthStencil.depthBoundsTestEnable = VK_FALSE;
        depthStencil.stencilTestEnable = this->cfg_isStencilTest;
        if (this->cfg_isStencilTest)
        {
            depthStencil.front = this->cfg_StencilOpFront;
            depthStencil.back = this->cfg_StencilOpBack;
        }

        //8> Pipeline ColorBlend State 
        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.blendEnable = this->cfg_isBlend;
        colorBlendAttachment.colorWriteMask = this->cfg_ColorWriteMask;
        if (this->cfg_isBlend)
        {
            colorBlendAttachment.srcColorBlendFactor = this->cfg_BlendColorFactorSrc;
            colorBlendAttachment.dstColorBlendFactor = this->cfg_BlendColorFactorDst;
            colorBlendAttachment.colorBlendOp = this->cfg_BlendColorOp;
            colorBlendAttachment.srcAlphaBlendFactor = this->cfg_BlendAlphaFactorSrc;
            colorBlendAttachment.dstAlphaBlendFactor = this->cfg_BlendAlphaFactorDst;
            colorBlendAttachment.alphaBlendOp = this->cfg_BlendAlphaOp;
        }

        VkPipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f;
        colorBlending.blendConstants[1] = 0.0f;
        colorBlending.blendConstants[2] = 0.0f;
        colorBlending.blendConstants[3] = 0.0f;

        //9> Pipeline Dynamic State
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

        //10> Pipeline Layout
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        if (this->poDescriptorSetLayout != nullptr)
        {
            pipelineLayoutInfo.setLayoutCount = 1;
            pipelineLayoutInfo.pSetLayouts = &this->poDescriptorSetLayout;
        }
        if (vkCreatePipelineLayout(this->poDevice, &pipelineLayoutInfo, nullptr, &this->poPipelineLayout) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create pipeline layout !");
        }

        //11> Graphics Pipeline
        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = nullptr;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pTessellationState = nullptr;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = &depthStencil;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicStateInfo;
        pipelineInfo.layout = this->poPipelineLayout;
        pipelineInfo.renderPass = this->poRenderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = nullptr;
        pipelineInfo.basePipelineIndex = 0;

        VkResult result = vkCreateGraphicsPipelines(this->poDevice, nullptr, 1, &pipelineInfo, nullptr, &this->poPipelineGraphics);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            Util_LogError("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline, result: [%s] !", os.str().c_str());
            throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline, result: " + os.str());
        }

        //12> Graphics Pipeline WireFrame
        rasterizer.polygonMode = VK_POLYGON_MODE_LINE;
        pipelineInfo.pRasterizationState = &rasterizer;
        result = vkCreateGraphicsPipelines(this->poDevice, nullptr, 1, &pipelineInfo, nullptr, &this->poPipelineGraphics_WireFrame);
        if (result != VK_SUCCESS)
        {
            std::ostringstream os;
            os << (int)result;
            Util_LogError("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline wireframe, result: [%s] !", os.str().c_str());
            throw std::runtime_error("VulkanWindow::createGraphicsPipeline: Failed to create graphics pipeline wireframe, result: " + os.str());
        }

        vkDestroyShaderModule(this->poDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(this->poDevice, vertShaderModule, nullptr);

        Util_LogInfo("<2-2-4> VulkanWindow::createGraphicsPipeline finish !");
    }
    VkShaderModule VulkanWindow::createShaderModule(std::string info, std::string pathFile)
    {
        if (pathFile.empty())
            return nullptr;

        std::vector<char> code;
        if (!VulkanUtil::LoadAssetFileContent(pathFile.c_str(), code))
        {
            Util_LogInfo("VulkanWindow::createShaderModule failed, path: [%s] !", pathFile.c_str());
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
            throw std::runtime_error("VulkanWindow::createShaderModule: Failed to create shader module, " + info);
        }

        return shaderModule;
    }
    void VulkanWindow::createPipelineVertexInputStateCreateInfo(VkPipelineVertexInputStateCreateInfo& vertexInputInfo)
    {
        switch ((int)this->poTypeVertex)
        {
            case (int)VertexType_Pos2Color3:
            {
                VkVertexInputBindingDescription* pBindingDescription = Vertex_Pos2Color3::getBindingDescriptionPtr();
                std::array<VkVertexInputAttributeDescription, 2>* pAttributeDescriptions = Vertex_Pos2Color3::getAttributeDescriptionsPtr();

                vertexInputInfo.pVertexBindingDescriptions = pBindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                vertexInputInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                break;
            }
            case (int)VertexType_Pos2Color3Tex2:
            {
                VkVertexInputBindingDescription* pBindingDescription = Vertex_Pos2Color3Tex2::getBindingDescriptionPtr();
                std::array<VkVertexInputAttributeDescription, 3>* pAttributeDescriptions = Vertex_Pos2Color3Tex2::getAttributeDescriptionsPtr();

                vertexInputInfo.pVertexBindingDescriptions = pBindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                vertexInputInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                break;
            }
            case (int)VertexType_Pos3Color3Tex2:
            {
                VkVertexInputBindingDescription* pBindingDescription = Vertex_Pos3Color3Tex2::getBindingDescriptionPtr();
                std::array<VkVertexInputAttributeDescription, 3>* pAttributeDescriptions = Vertex_Pos3Color3Tex2::getAttributeDescriptionsPtr();

                vertexInputInfo.pVertexBindingDescriptions = pBindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                vertexInputInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                break;
            }
            case (int)VertexType_Pos3Color3Normal3Tex2:
            {
                VkVertexInputBindingDescription* pBindingDescription = Vertex_Pos3Color3Normal3Tex2::getBindingDescriptionPtr();
                std::array<VkVertexInputAttributeDescription, 4>* pAttributeDescriptions = Vertex_Pos3Color3Normal3Tex2::getAttributeDescriptionsPtr();

                vertexInputInfo.pVertexBindingDescriptions = pBindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                vertexInputInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                break;
            }
            case (int)VertexType_Default:
            {
                VkVertexInputBindingDescription* pBindingDescription = MeshVertex::getBindingDescriptionPtr();
                std::array<VkVertexInputAttributeDescription, 5>* pAttributeDescriptions = MeshVertex::getAttributeDescriptionsPtr();

                vertexInputInfo.pVertexBindingDescriptions = pBindingDescription;
                vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(pAttributeDescriptions->size());
                vertexInputInfo.pVertexAttributeDescriptions = pAttributeDescriptions->data();
                break;
            }
            default:
            {
                throw std::runtime_error("VulkanWindow::createPipelineVertexInputStateCreateInfo: Failed to create VkPipelineVertexInputStateCreateInfo, type wrong !");
            }
        }
    }

    void VulkanWindow::createDescriptor()
    {
        Util_LogInfo("**<2-2-5> VulkanWindow::createDescriptor start **");
        {
            //1> createDescriptorPool
            createDescriptorPool();

            //2> createDescriptorSets
            createDescriptorSets();
        }
        Util_LogInfo("**<2-2-5> VulkanWindow::createDescriptor finish **");
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
            throw std::runtime_error("VulkanWindow::createDescriptorPool: Failed to create descriptor pool !");
        }

        Util_LogInfo("<2-2-5-1> VulkanWindow::createDescriptorPool finish !");
    }
    void VulkanWindow::createDescriptorSets()
    {
        std::vector<VkDescriptorSetLayout> layouts(this->poSwapChainImages.size(), this->poDescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = this->poDescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(this->poSwapChainImages.size());
        allocInfo.pSetLayouts = layouts.data();

        size_t count = this->poSwapChainImages.size();
        this->poDescriptorSets.resize(count);
        if (vkAllocateDescriptorSets(this->poDevice, &allocInfo, this->poDescriptorSets.data()) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createDescriptorSets: Failed to allocate descriptor sets !");
        }
        
        updateDescriptorSets();

        Util_LogInfo("<2-2-5-2> VulkanWindow::createDescriptorSets finish !");
    }
    void VulkanWindow::updateDescriptorSets()
    {
        bool bHasTexture = this->poTextureImageView == nullptr ? false : true;
        size_t count = this->poDescriptorSets.size();
        for (size_t i = 0; i < count; i++)
        {
            VkDescriptorBufferInfo bufferInfo_Pass = {};
            bufferInfo_Pass.buffer = this->poBuffers_PassCB[i];
            bufferInfo_Pass.offset = 0;
            bufferInfo_Pass.range = sizeof(PassConstants);

            VkDescriptorBufferInfo bufferInfo_Object = {};
            bufferInfo_Object.buffer = this->poBuffers_ObjectCB[i];
            bufferInfo_Object.offset = 0;
            bufferInfo_Object.range = sizeof(ObjectConstants) * this->objectCBs.size();

            VkDescriptorBufferInfo bufferInfo_Material = {};
            bufferInfo_Material.buffer = this->poBuffers_MaterialCB[i];
            bufferInfo_Material.offset = 0;
            bufferInfo_Material.range = sizeof(MaterialConstants) * this->materialCBs.size();

            VkDescriptorBufferInfo bufferInfo_Instance = {};
            bufferInfo_Instance.buffer = this->poBuffers_InstanceCB[i];
            bufferInfo_Instance.offset = 0;
            bufferInfo_Instance.range = sizeof(InstanceConstants) * this->instanceCBs.size();

            VkDescriptorImageInfo imageInfo = {};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = this->poTextureImageView;
            imageInfo.sampler = this->poTextureSampler;
            
            std::vector<VkWriteDescriptorSet> descriptorWrites;

            //0
            VkWriteDescriptorSet ds0 = {};
            ds0.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds0.dstSet = this->poDescriptorSets[i];
            ds0.dstBinding = 0;
            ds0.dstArrayElement = 0;
            ds0.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds0.descriptorCount = 1;
            ds0.pBufferInfo = &bufferInfo_Pass;
            descriptorWrites.push_back(ds0);

            //1
            VkWriteDescriptorSet ds1 = {};
            ds1.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds1.dstSet = this->poDescriptorSets[i];
            ds1.dstBinding = 1;
            ds1.dstArrayElement = 0;
            ds1.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds1.descriptorCount = 1;
            ds1.pBufferInfo = &bufferInfo_Object;
            descriptorWrites.push_back(ds1);

            //2
            VkWriteDescriptorSet ds2 = {};
            ds2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds2.dstSet = this->poDescriptorSets[i];
            ds2.dstBinding = 2;
            ds2.dstArrayElement = 0;
            ds2.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds2.descriptorCount = 1;
            ds2.pBufferInfo = &bufferInfo_Material;
            descriptorWrites.push_back(ds2);

            //3
            VkWriteDescriptorSet ds3 = {};
            ds3.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            ds3.dstSet = this->poDescriptorSets[i];
            ds3.dstBinding = 3;
            ds3.dstArrayElement = 0;
            ds3.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            ds3.descriptorCount = 1;
            ds3.pBufferInfo = &bufferInfo_Instance;
            descriptorWrites.push_back(ds3);
            
            //4
            if (bHasTexture)
            {
                VkWriteDescriptorSet ds4 = {};
                ds4.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                ds4.dstSet = this->poDescriptorSets[i];
                ds4.dstBinding = 4;
                ds4.dstArrayElement = 0;
                ds4.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                ds4.descriptorCount = 1;
                ds4.pImageInfo = &imageInfo;
                descriptorWrites.push_back(ds4);
            }
            
            vkUpdateDescriptorSets(this->poDevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }
    }

    void VulkanWindow::createCommandBuffers()
    {
        this->poCommandBuffers.resize(this->poSwapChainFrameBuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = this->poCommandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)this->poCommandBuffers.size();
        if (vkAllocateCommandBuffers(this->poDevice, &allocInfo, this->poCommandBuffers.data()) != VK_SUCCESS) 
        {
            throw std::runtime_error("VulkanWindow::createCommandBuffers: Failed to allocate command buffers !");
        }

        Util_LogInfo("<2-2-6> VulkanWindow::createCommandBuffers finish !");
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
            throw std::runtime_error("VulkanWindow::createImgui_DescriptorPool: Imgui descriptor pool creation failed !");
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
        init_info.QueueFamily = this->graphicsIndex;
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

    bool VulkanWindow::beginRender()
    {
        vkWaitForFences(this->poDevice, 1, &this->poInFlightFences[this->poCurrentFrame], VK_TRUE, UINT64_MAX);

        VkResult result = vkAcquireNextImageKHR(this->poDevice, this->poSwapChain, UINT64_MAX, this->poImageAvailableSemaphores[this->poCurrentFrame], nullptr, &this->poSwapChainImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return false;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("VulkanWindow::beginRender: Failed to acquire swap chain image !");
            return false;
        }
        return true;
    }
        void VulkanWindow::update()
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

            //6> Imgui
            if (IsEnable_Imgui())
            {
                updateImgui();
            }

            //7> CommandBuffers
            updateCommandBuffers();
        }
            void VulkanWindow::updateSceneObjects()
            {
                
            }
            void VulkanWindow::updateCBs_Pass()
            {
                if (this->poBuffersMemory_PassCB.size() <= 0)
                    return;

                if (this->pCamera != nullptr)
                {
                    this->passCB.g_MatView = this->pCamera->GetMatrix4View();
                    this->passCB.g_MatProj = this->pCamera->GetMatrix4Projection();
                }
                else
                {
                    this->passCB.g_MatView = glm::lookAtLH(this->cfg_cameraPos, 
                                                           this->cfg_cameraLookTarget,
                                                           this->cfg_cameraUp);
                    this->passCB.g_MatProj = glm::perspectiveLH(glm::radians(this->cfg_cameraFov), 
                                                                this->poSwapChainExtent.width / (float)this->poSwapChainExtent.height,
                                                                this->cfg_cameraNear, 
                                                                this->cfg_cameraFar);
                }   

                VkDeviceMemory& memory = this->poBuffersMemory_PassCB[this->poSwapChainImageIndex];
                void* data;
                vkMapMemory(this->poDevice, memory, 0, sizeof(PassConstants), 0, &data);
                    memcpy(data, &this->passCB, sizeof(PassConstants));
                vkUnmapMemory(this->poDevice, memory);
            }
            void VulkanWindow::updateCBs_Objects()
            {
                if (this->poBuffersMemory_ObjectCB.size() <= 0)
                    return;
                size_t count = this->objectCBs.size();
                if (count >= MAX_OBJECT_COUNT)
                {
                    Util_LogError("VulkanWindow::updateCBs_Objects: Max object count can not > [%d]", MAX_OBJECT_COUNT);
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
                    Util_LogError("VulkanWindow::updateCBs_Materials: Max material count can not > [%d]", MAX_MATERIAL_COUNT);
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
                    MaterialConstants materialCB = this->materialCBs[0];
                    materialCB.diffuseAlbedo = glm::vec4(1);
                }
            void VulkanWindow::updateCBs_Instances()
            {
                if (this->poBuffersMemory_InstanceCB.size() <= 0)
                    return;
                size_t count = this->instanceCBs.size();
                if (count >= MAX_INSTANCE_COUNT)
                {
                    Util_LogError("VulkanWindow::updateCBs_Instances: Max instance count can not > [%d]", MAX_INSTANCE_COUNT);
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

            void VulkanWindow::updateImgui()
            {
                if (beginRenderImgui())
                {
                    endRenderImgui();
                }
            }
                bool VulkanWindow::beginRenderImgui()
                {
                    // ImGui_ImplVulkan_NewFrame();
                    // ImGui_ImplGlfw_NewFrame();
                    // ImGui::NewFrame();
                    // static bool windowOpened = true;
                    // static bool showDemoWindow = false;
                    // ImGui::Begin("Rendertime", &windowOpened, 0);
                    // ImGui::Text("Frametime: %f", 1000.0f / ImGui::GetIO().Framerate);
                    // ImGui::Checkbox("Show ImGui demo window", &showDemoWindow);
                    // ImGui::End();
                    // if (showDemoWindow) 
                    // {
                    //     ImGui::ShowDemoWindow();
                    // }

                    return false;
                }
                    void VulkanWindow::cameraConfig()
                    {
                        if (this->pCamera == nullptr)
                            return;

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
                        ImGui::Separator();
                    }
                    void VulkanWindow::cameraReset()
                    {
                        if (this->pCamera == nullptr)
                            return;

                        this->pCamera->LookAtLH(this->cfg_cameraPos, this->cfg_cameraLookTarget, this->cfg_cameraUp);
                        this->pCamera->PerspectiveLH(this->cfg_cameraFov, this->aspectRatio, this->cfg_cameraNear, this->cfg_cameraFar);
                        this->pCamera->UpdateViewMatrix();
                    }

                void VulkanWindow::endRenderImgui()
                {
                    ImGui::Render();
                }

            void VulkanWindow::updateCommandBuffers()
            {
                VkCommandBuffer& commandBuffer = this->poCommandBuffers[this->poSwapChainImageIndex];
                if (vkResetCommandBuffer(commandBuffer, 0) != VK_SUCCESS) 
                {
                    std::string msg = "VulkanWindow::updateCommandBuffers: Failed to reset command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }

                VkCommandBufferBeginInfo beginInfo = {};
                beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                beginInfo.flags = 0; // Optional
                beginInfo.pInheritanceInfo = nullptr; // Optional

                if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
                {
                    std::string msg = "VulkanWindow::updateCommandBuffers: Failed to begin recording command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
                {
                    updateRenderPass_Default(commandBuffer);
                    updateRenderPass_Custom(commandBuffer);
                }
                if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
                {
                    std::string msg = "VulkanWindow::updateCommandBuffers: Failed to record command buffer !";
                    Util_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            void VulkanWindow::updateRenderPass_Default(VkCommandBuffer& commandBuffer)
            {
                VkRenderPassBeginInfo renderPassInfo = {};
                renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                renderPassInfo.renderPass = this->poRenderPass;
                renderPassInfo.framebuffer = this->poSwapChainFrameBuffers[this->poSwapChainImageIndex];
                VkOffset2D offset;
                offset.x = 0;
                offset.y = 0;
                renderPassInfo.renderArea.offset = offset;
                renderPassInfo.renderArea.extent = this->poSwapChainExtent;

                std::array<VkClearValue, 2> clearValues = {};
                VkClearColorValue colorValue = { this->cfg_colorBackground.x, this->cfg_colorBackground.y, this->cfg_colorBackground.z, this->cfg_colorBackground.w };
                clearValues[0].color = colorValue;
                VkClearDepthStencilValue depthStencilValue;
                depthStencilValue.depth = 1.0f;
                depthStencilValue.stencil = 0;
                clearValues[1].depthStencil = depthStencilValue;
                renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
                renderPassInfo.pClearValues = clearValues.data();

                vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
                {
                    //0> Pipeline
                    bindPipeline(commandBuffer);

                    //1> Viewport
                    bindViewport(commandBuffer);
                   
                    //2> Normal Render Pass
                    drawMesh(commandBuffer);
                    drawMesh_Custom(commandBuffer);

                    //3> ImGui Pass
                    drawImgui(commandBuffer);
                }
                vkCmdEndRenderPass(commandBuffer);
            }
            void VulkanWindow::updateRenderPass_Custom(VkCommandBuffer& commandBuffer)
            {

            }
                void VulkanWindow::bindPipeline(VkCommandBuffer& commandBuffer)
                {
                    if (!this->cfg_isWireFrame)
                        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineGraphics);
                    else 
                        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineGraphics_WireFrame);
                }
                void VulkanWindow::bindViewport(VkCommandBuffer& commandBuffer)
                {
                    VkViewport viewport = this->poViewport;
                    if (cfg_isNegativeViewport)
                    {
                        viewport.y = viewport.height - viewport.y;
                        viewport.height = -viewport.height;
                    }   
                    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
                    vkCmdSetScissor(commandBuffer, 0, 1, &this->poScissor);
                }
                void VulkanWindow::drawMesh(VkCommandBuffer& commandBuffer)
                {
                    if (this->poVertexBuffer == nullptr)
                        return;

                    VkBuffer vertexBuffers[] = { this->poVertexBuffer };
                    VkDeviceSize offsets[] = { 0 };
                    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
                    if (this->poDescriptorSets.size() > 0)
                    {
                        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->poPipelineLayout, 0, 1, &this->poDescriptorSets[this->poSwapChainImageIndex], 0, nullptr);
                    }
                    if (this->poIndexBuffer != nullptr)
                    {
                        vkCmdBindIndexBuffer(commandBuffer, this->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
                        vkCmdDrawIndexed(commandBuffer, this->poIndexCount, 1, 0, 0, 0);
                    }
                    else
                    {
                        vkCmdDraw(commandBuffer, this->poVertexCount, 1, 0, 0);
                    }
                }
                void VulkanWindow::drawMesh_Custom(VkCommandBuffer& commandBuffer)
                {

                }
                void VulkanWindow::drawImgui(VkCommandBuffer& commandBuffer)
                {
                    if (HasConfig_Imgui())
                    {
                        ImDrawData* main_draw_data = ImGui::GetDrawData();
                        vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
                        ImGui_ImplVulkan_RenderDrawData(main_draw_data, commandBuffer);
                    }
                }


        void VulkanWindow::render()
        {
            if (this->poImagesInFlight[this->poSwapChainImageIndex] != nullptr)
            {
                vkWaitForFences(this->poDevice, 1, &this->poImagesInFlight[this->poSwapChainImageIndex], VK_TRUE, UINT64_MAX);
            }
            this->poImagesInFlight[this->poSwapChainImageIndex] = this->poInFlightFences[this->poCurrentFrame];

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore waitSemaphores[] = { this->poImageAvailableSemaphores[this->poCurrentFrame] };
            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitSemaphores;
            submitInfo.pWaitDstStageMask = waitStages;

            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &this->poCommandBuffers[this->poSwapChainImageIndex];

            VkSemaphore signalSemaphores[] = { this->poRenderFinishedSemaphores[this->poCurrentFrame] };
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            vkResetFences(this->poDevice, 1, &this->poInFlightFences[this->poCurrentFrame]);

            if (vkQueueSubmit(this->poQueueGraphics, 1, &submitInfo, this->poInFlightFences[this->poCurrentFrame]) != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::render: Failed to submit draw command buffer !");
            }

            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR swapChains[] = { this->poSwapChain };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = swapChains;
            presentInfo.pImageIndices = &this->poSwapChainImageIndex;

            VkResult result = vkQueuePresentKHR(this->poQueuePresent, &presentInfo);

            if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || this->framebufferResized) 
            {
                this->framebufferResized = false;
                recreateSwapChain();
            } 
            else if (result != VK_SUCCESS) 
            {
                throw std::runtime_error("VulkanWindow::render: Failed to present swap chain image !");
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
            if (this->imgui_DescriptorPool != nullptr)
            {
                vkDestroyDescriptorPool(this->poDevice, this->imgui_DescriptorPool, nullptr);
            }

            //3> cleanupTexture
            cleanupTexture();

            //4> DescriptorSetLayout
            if (this->poDescriptorSetLayout != nullptr)
            {
                vkDestroyDescriptorSetLayout(this->poDevice, this->poDescriptorSetLayout, nullptr);
            }

            //5> cleanupVertexIndexBuffer
            cleanupVertexIndexBuffer();
            
            //6> Semaphores
            for (size_t i = 0; i < s_maxFramesInFight; i++) 
            {
                vkDestroySemaphore(this->poDevice, this->poRenderFinishedSemaphores[i], nullptr);
                vkDestroySemaphore(this->poDevice, this->poImageAvailableSemaphores[i], nullptr);
                vkDestroyFence(this->poDevice, this->poInFlightFences[i], nullptr);
            }

            //7> Imgui
            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_Shutdown();
	            ImGui_ImplGlfw_Shutdown();
            }

            //8> CommandPool
            vkDestroyCommandPool(this->poDevice, this->poCommandPool, nullptr);
            
            //9> Device
            vkDestroyDevice(this->poDevice, nullptr);
            if (s_isEnableValidationLayers)
            {
                destroyDebugUtilsMessengerEXT(this->poInstance, this->poDebugMessenger, nullptr);
            }
            //10> Surface
            vkDestroySurfaceKHR(this->poInstance, this->poSurface, nullptr);

            //11> Instance
            vkDestroyInstance(this->poInstance, nullptr);
        }
        Util_LogInfo("---------- VulkanWindow::cleanup finish ----------");
    }
        void VulkanWindow::cleanupCustom()
        {

        }
        void VulkanWindow::cleanupTexture()
        {
            //1> TextureSampler
            if (this->poTextureSampler != nullptr)
            {
                vkDestroySampler(this->poDevice, this->poTextureSampler, nullptr);
            }
            this->poTextureSampler = nullptr;
            //2> TextureImageView
            if (this->poTextureImageView != nullptr)
            {
                vkDestroyImageView(this->poDevice, this->poTextureImageView, nullptr);
            }
            this->poTextureImageView = nullptr;
            //3> TextureImage
            if (this->poTextureImage != nullptr)
            {
                vkDestroyImage(this->poDevice, this->poTextureImage, nullptr);
                vkFreeMemory(this->poDevice, this->poTextureImageMemory, nullptr);
            }
            this->poTextureImage = nullptr;
            this->poTextureImageMemory = nullptr;
        }
        void VulkanWindow::cleanupVertexIndexBuffer()
        {
            //1> VertexBuffer
            if (this->poVertexBuffer != nullptr)
            {
                vkDestroyBuffer(this->poDevice, this->poVertexBuffer, nullptr);
                vkFreeMemory(this->poDevice, this->poVertexBufferMemory, nullptr);
            }
            this->poVertexBuffer = nullptr;
            this->poVertexBufferMemory = nullptr;
            this->poVertexCount = 0;
            this->poVertexBuffer_Size = 0;
            this->poVertexBuffer_Data = nullptr;

            //2> IndexBuffer
            if (this->poIndexBuffer != nullptr)
            {
                vkDestroyBuffer(this->poDevice, this->poIndexBuffer, nullptr);
                vkFreeMemory(this->poDevice, this->poIndexBufferMemory, nullptr);
            }
            this->poIndexBuffer = nullptr;
            this->poIndexBufferMemory = nullptr;
            this->poIndexCount = 0;
            this->poIndexBuffer_Size = 0;
            this->poIndexBuffer_Data = nullptr;
        }
        void VulkanWindow::cleanupSwapChain()
        {
            Util_LogInfo("----- VulkanWindow::cleanupSwapChain start -----");
            {
                //1> DepthImage
                if (this->poDepthImage != nullptr)
                {
                    vkDestroyImageView(this->poDevice, this->poDepthImageView, nullptr);
                    vkDestroyImage(this->poDevice, this->poDepthImage, nullptr);
                    vkFreeMemory(this->poDevice, this->poDepthImageMemory, nullptr);
                }

                //2> ColorImage
                if (this->poColorImage != nullptr)
                {
                    vkDestroyImageView(this->poDevice, this->poColorImageView, nullptr);
                    vkDestroyImage(this->poDevice, this->poColorImage, nullptr);
                    vkFreeMemory(this->poDevice, this->poColorImageMemory, nullptr);
                }

                //3> SwapChainFrameBuffers
                size_t count = this->poSwapChainFrameBuffers.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkFramebuffer& frameBuffer = this->poSwapChainFrameBuffers[i];
                    vkDestroyFramebuffer(this->poDevice, frameBuffer, nullptr);
                }

                //4> CommandBuffers
                if (this->poCommandBuffers.size() > 0)
                {
                    vkFreeCommandBuffers(this->poDevice, this->poCommandPool, (uint32_t)this->poCommandBuffers.size(), this->poCommandBuffers.data());
                }
                
                //5> PipelineGraphics
                if (this->poPipelineGraphics != nullptr)
                {
                    vkDestroyPipeline(this->poDevice, this->poPipelineGraphics, nullptr);
                }
                if (this->poPipelineGraphics_WireFrame != nullptr)
                {
                    vkDestroyPipeline(this->poDevice, this->poPipelineGraphics_WireFrame, nullptr);
                }
                if (this->poPipelineLayout != nullptr)
                {
                    vkDestroyPipelineLayout(this->poDevice, this->poPipelineLayout, nullptr);
                }
                vkDestroyRenderPass(this->poDevice, this->poRenderPass, nullptr);

                //6> SwapChainImageViews
                count = this->poSwapChainImageViews.size();
                for (size_t i = 0; i < count; i++)
                {
                    VkImageView& imageView = this->poSwapChainImageViews[i];
                    vkDestroyImageView(this->poDevice, imageView, nullptr);
                }
                vkDestroySwapchainKHR(this->poDevice, this->poSwapChain, nullptr);

                //7> ConstBuffers
                count = poBuffers_PassCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    vkDestroyBuffer(this->poDevice, this->poBuffers_PassCB[i], nullptr);
                    vkFreeMemory(this->poDevice, this->poBuffersMemory_PassCB[i], nullptr);
                }
                count = poBuffers_ObjectCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    vkDestroyBuffer(this->poDevice, this->poBuffers_ObjectCB[i], nullptr);
                    vkFreeMemory(this->poDevice, this->poBuffersMemory_ObjectCB[i], nullptr);
                }
                count = poBuffers_MaterialCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    vkDestroyBuffer(this->poDevice, this->poBuffers_MaterialCB[i], nullptr);
                    vkFreeMemory(this->poDevice, this->poBuffersMemory_MaterialCB[i], nullptr);
                }
                count = poBuffers_InstanceCB.size();
                for (size_t i = 0; i < count; i++) 
                {
                    vkDestroyBuffer(this->poDevice, this->poBuffers_InstanceCB[i], nullptr);
                    vkFreeMemory(this->poDevice, this->poBuffersMemory_InstanceCB[i], nullptr);
                }

                //8> DescriptorPool
                if (this->poDescriptorPool != nullptr)
                {
                    vkDestroyDescriptorPool(this->poDevice, this->poDescriptorPool, nullptr);
                }
            }
            Util_LogInfo("----- VulkanWindow::cleanupSwapChain finish -----");
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

            createSwapChain();
            createSwapChainImageViews();
            if (HasConfig_MASS())
            {
                createColorResources();
            }  
            createDepthResources();

            createRenderPass();
            createFramebuffers();

            createConstBuffers();
            createGraphicsPipeline();
            createDescriptor();
            createCommandBuffers();

            this->poImagesInFlight.resize(this->poSwapChainImages.size(), nullptr);

            if (HasConfig_Imgui())
            {
                ImGui_ImplVulkan_SetMinImageCount(this->imgui_MinimalSwapchainImages);
            }
        }
        Util_LogInfo("++++++++++ VulkanWindow::recreateSwapChain finish ++++++++++");
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