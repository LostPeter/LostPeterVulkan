/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanDevice.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanQueue.h"
#include "../include/VulkanFenceManager.h"
#include "../include/VulkanDeviceMemoryManager.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanDevice::VulkanDevice(VulkanInstance* pInstance, VkPhysicalDevice vkPhysicalDevice)
        : m_vkDevice(VK_NULL_HANDLE)
        , m_vkPhysicalDevice(vkPhysicalDevice)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
        , m_vkMaxMSAASamples(VK_SAMPLE_COUNT_1_BIT)
        , m_vkCommandPoolTransfer(VK_NULL_HANDLE)
        , m_pInstance(pInstance)
        , m_pQueueGraphics(nullptr)
        , m_pQueueCompute(nullptr)
        , m_pQueueTransfer(nullptr)
        , m_pQueuePresent(nullptr)
        , m_pFenceManager(nullptr)
        , m_pDeviceMemoryManager(nullptr)
    {
        F_Assert(m_vkPhysicalDevice != VK_NULL_HANDLE && "VulkanDevice::VulkanDevice")
    }

    VulkanDevice::~VulkanDevice()
    {
        Destroy();
    }

    void VulkanDevice::Destroy()
    {
        m_pQueuePresent = nullptr;
        F_DELETE(m_pQueueTransfer)
        F_DELETE(m_pQueueCompute)
        F_DELETE(m_pQueueGraphics)

        F_DELETE(m_pFenceManager)
        F_DELETE(m_pDeviceMemoryManager)

        if (m_vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_vkDevice, nullptr);
        }
        m_vkDevice = VK_NULL_HANDLE;
    }

    bool VulkanDevice::Init(int32 deviceIndex, bool bIsEnableValidationLayers)
    {
        vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);
        F_LogInfo("VulkanDevice::Init: Using device: [%d], Support Geometry Shader: [%d], Support Tessellation Shader: [%d]", 
                    deviceIndex, m_vkPhysicalDeviceFeatures.geometryShader, m_vkPhysicalDeviceFeatures.tessellationShader);

        //1> createDevice
        if (!createDevice(bIsEnableValidationLayers))
        {
            F_LogError("*********************** VulkanDevice::Init: 1> createDevice failed !");
            return false;
        }
        F_LogInfo("VulkanDevice::Init: 1> createDevice success !");
        
        //2> checkPixelFormats
        if (!checkPixelFormats())
        {
            F_LogError("*********************** VulkanDevice::Init: 2> checkPixelFormats failed !");
            return false;
        }
        F_LogInfo("VulkanDevice::Init: 2> checkPixelFormats success !");
       
        //3> VulkanFenceManager
        m_pFenceManager = new VulkanFenceManager();
        m_pFenceManager->Init(this);

        //4> VulkanDeviceMemoryManager
        m_pDeviceMemoryManager = new VulkanDeviceMemoryManager();
        m_pDeviceMemoryManager->Init(this);

        return true;
    }



    bool VulkanDevice::QueryGPU(int32 deviceIndex)
    {
        vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);

        bool isDiscrete = false;
        String deviceType = getDeviceType(m_vkPhysicalDeviceProperties, isDiscrete);
        m_vkMaxMSAASamples = GetMaxUsableSampleCount();

        F_LogInfo("VulkanDevice::QueryGPU: Device index: [%d], name: [%s]", deviceIndex, m_vkPhysicalDeviceProperties.deviceName);
        F_LogInfo("VulkanDevice::QueryGPU: API: [%d.%d.%d(0x%x)], Driver: [0x%x VendorId 0x%x]", VK_VERSION_MAJOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_MINOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_PATCH(m_vkPhysicalDeviceProperties.apiVersion), m_vkPhysicalDeviceProperties.apiVersion, m_vkPhysicalDeviceProperties.driverVersion, m_vkPhysicalDeviceProperties.vendorID);
        F_LogInfo("VulkanDevice::QueryGPU: DeviceID: [0x%x], Type: [%s]", m_vkPhysicalDeviceProperties.deviceID, deviceType.c_str());
        F_LogInfo("VulkanDevice::QueryGPU: Max Descriptor Sets Bound: [%d], Timestamps: [%d]", m_vkPhysicalDeviceProperties.limits.maxBoundDescriptorSets, m_vkPhysicalDeviceProperties.limits.timestampComputeAndGraphics);
        F_LogInfo("VulkanDevice::QueryGPU: Max MSAA Samples: [%d]", (int32)m_vkMaxMSAASamples);

        uint32 queueCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount, nullptr);
        m_aVkQueueFamilyProperties.resize(queueCount);
        vkGetPhysicalDeviceQueueFamilyProperties(m_vkPhysicalDevice, &queueCount, m_aVkQueueFamilyProperties.data());
        
        return isDiscrete;
    }

    bool VulkanDevice::CreateQueuePresent(VkSurfaceKHR surface)
    {
        if (m_pQueuePresent != nullptr)
            return true;

        bool compute = isSupportPresent(m_pQueueCompute, surface);
        if (m_pQueueTransfer->GetFamilyIndex() != m_pQueueGraphics->GetFamilyIndex() && 
            m_pQueueTransfer->GetFamilyIndex() != m_pQueueCompute->GetFamilyIndex()) 
        {
            isSupportPresent(m_pQueueTransfer, surface);
        }
        if (m_pQueueCompute->GetFamilyIndex() != m_pQueueGraphics->GetFamilyIndex() && compute) 
        {
            m_pQueuePresent = m_pQueueCompute;
        }
        else 
        {
            m_pQueuePresent = m_pQueueGraphics;
        }

        return true;
    }

    bool VulkanDevice::IsPixelFormatSupported(VkFormat format)
    {
        VkFormatProperties prop;
        memset(&prop, 0, sizeof(VkFormatProperties));
        vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, format, &prop);

        return (prop.bufferFeatures != 0 ||
                prop.linearTilingFeatures != 0 ||
                prop.optimalTilingFeatures != 0);
    }
    bool VulkanDevice::IsPixelFormatSupported(FPixelFormatType format)
    {
        return FPixelFormat::GetPixelFormatDes(format).isSupported;
    }

    VkSampleCountFlagBits VulkanDevice::GetMaxUsableSampleCount()
    {
        VkSampleCountFlags counts = m_vkPhysicalDeviceProperties.limits.framebufferColorSampleCounts & m_vkPhysicalDeviceProperties.limits.framebufferDepthSampleCounts;
        if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
        if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
        if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
        if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
        if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
        if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

        return VK_SAMPLE_COUNT_1_BIT;
    }

    VkFormat VulkanDevice::FindSupportedFormat(const VkFormatVector& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
    {
        size_t count = candidates.size();
        for (int i = 0; i < count; i++) 
        {
            VkFormat format = candidates[i];
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_vkPhysicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) 
            {
                return format;
            } 
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) 
            {
                return format;
            }
        }

        throw std::runtime_error("VulkanDevice::FindSupportedFormat: Failed to find supported format !");
    }

    VkFormat VulkanDevice::FindDepthFormat()
    {
        VkFormatVector candidates;
        candidates.push_back(VK_FORMAT_D32_SFLOAT);
        candidates.push_back(VK_FORMAT_D32_SFLOAT_S8_UINT);
        candidates.push_back(VK_FORMAT_D24_UNORM_S8_UINT);
        return FindSupportedFormat(candidates,
                                   VK_IMAGE_TILING_OPTIMAL,
                                   VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
    }

    uint32_t VulkanDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        throw std::runtime_error("VulkanDevice::FindMemoryType: Failed to find suitable memory type !");
    }

    bool VulkanDevice::createDevice(bool bIsEnableValidationLayers)
    {
        ConstCharPtrVector validationLayers;
        ConstCharPtrVector deviceExtensions;
        getDeviceLayersAndExtensions(bIsEnableValidationLayers,
                                     validationLayers,
                                     deviceExtensions);

        size_t countAppDeviceExtensions = m_aAppDeviceExtensions.size();
        if (countAppDeviceExtensions > 0)
        {
            F_LogInfo("VulkanDevice::createDevice: Using app device extensions count: %d", (int32)countAppDeviceExtensions);
            for (int32 i = 0; i < countAppDeviceExtensions; ++i)
            {
                deviceExtensions.push_back(m_aAppDeviceExtensions[i]);
                F_LogInfo("VulkanDevice::createDevice: Using app device extension: %s", m_aAppDeviceExtensions[i]);
            }
        }

        VkDeviceCreateInfo deviceCreateInfo;
        E_ZeroStruct(deviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
        deviceCreateInfo.enabledExtensionCount = uint32_t(deviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
        deviceCreateInfo.enabledLayerCount = uint32_t(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

        if (m_pVkPhysicalDeviceFeatures2) 
        {
            deviceCreateInfo.pNext = m_pVkPhysicalDeviceFeatures2;
            deviceCreateInfo.pEnabledFeatures = nullptr;
            m_pVkPhysicalDeviceFeatures2->features = m_vkPhysicalDeviceFeatures;
        }
        else 
        {
            deviceCreateInfo.pEnabledFeatures = &m_vkPhysicalDeviceFeatures;
        }

        int32 countQueueFamilyProperties = (int32)m_aVkQueueFamilyProperties.size();
        F_LogInfo("VulkanDevice::createDevice: Queue Families count: %d", countQueueFamilyProperties);

        std::vector<VkDeviceQueueCreateInfo> queueFamilyInfos;
	
        int32 queueFamilyIndex_Graphics = -1;
        int32 queueFamilyIndex_Compute = -1;
        int32 queueFamilyIndex_Transfer = -1;
        uint32 numPriorities = 0;
        for (int32 i = 0; i < countQueueFamilyProperties; ++i)
        {
            const VkQueueFamilyProperties& currProps = m_aVkQueueFamilyProperties[i];

            bool isValidQueue = false;
            if ((currProps.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
            {
                if (queueFamilyIndex_Graphics == -1) 
                {
                    queueFamilyIndex_Graphics = i;
                    isValidQueue = true;
                }
            }

            if ((currProps.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
            {
                if (queueFamilyIndex_Compute == -1)
                {
                    queueFamilyIndex_Compute = i;
                    isValidQueue = true;
                }
            }

            if ((currProps.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
            {
                if (queueFamilyIndex_Transfer == -1)
                {
                    queueFamilyIndex_Transfer = i;
                    isValidQueue = true;
                }
            }
            
            if (!isValidQueue)
            {
                F_LogInfo("VulkanDevice::createDevice: Skipping unnecessary Queue Family: [%d : %d], Queues: [%s]", i, currProps.queueCount, getQueueType(currProps).c_str());
                continue;
            }

            VkDeviceQueueCreateInfo queueCreateInfo;
            E_ZeroStruct(queueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
            queueCreateInfo.queueFamilyIndex = i;
            queueCreateInfo.queueCount = currProps.queueCount;
            numPriorities += currProps.queueCount;
            queueFamilyInfos.push_back(queueCreateInfo);
            
            F_LogInfo("VulkanDevice::createDevice: Initializing Queue Family: [%d: %d], Queues: [%s]", i,  currProps.queueCount, getQueueType(currProps).c_str());
        }
        
        FloatVector queuePriorities(numPriorities);
        float* pCurrentPriority = queuePriorities.data();
        for (int32 i = 0; i < queueFamilyInfos.size(); ++i)
        {
            VkDeviceQueueCreateInfo& queueCreateInfo = queueFamilyInfos[i];
            queueCreateInfo.pQueuePriorities = pCurrentPriority;
            const VkQueueFamilyProperties& currProps = m_aVkQueueFamilyProperties[queueCreateInfo.queueFamilyIndex];
            for (int32 j = 0; j < (int32)currProps.queueCount; ++j) 
            {
                *pCurrentPriority++ = 1.0f;
            }
        }

        deviceCreateInfo.queueCreateInfoCount = uint32_t(queueFamilyInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueFamilyInfos.data();
        
        VkResult result = vkCreateDevice(m_vkPhysicalDevice, &deviceCreateInfo, nullptr, &m_vkDevice);
        if (result == VK_ERROR_INITIALIZATION_FAILED)
        {
            F_LogError("*********************** VulkanDevice::createDevice: vkCreateDevice create a Vulkan device failed !");
            return false;
        }

        m_pQueueGraphics = new VulkanQueue();
        m_pQueueGraphics->Init(this, queueFamilyIndex_Graphics);

        if (queueFamilyIndex_Compute == -1) 
        {
            queueFamilyIndex_Compute = queueFamilyIndex_Graphics;
        }
        m_pQueueCompute = new VulkanQueue();
        m_pQueueCompute->Init(this, queueFamilyIndex_Compute);

        if (queueFamilyIndex_Transfer == -1) 
        {
            queueFamilyIndex_Transfer = queueFamilyIndex_Compute;
        }
        m_pQueueTransfer = new VulkanQueue();
        m_pQueueTransfer->Init(this, queueFamilyIndex_Transfer);

        m_vkCommandPoolTransfer = CreateVkCommandPool(VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, (uint32_t)queueFamilyIndex_Transfer);

        return true;
    }
    bool VulkanDevice::checkPixelFormats()
    {
        int count = (int)F_PixelFormat_Count;
        for (int i = 1; i < count; i++)
        {
            FPixelFormatType format = (FPixelFormatType)i;
            FPixelFormatDes& des = FPixelFormat::GetPixelFormatDesRef(format);
            des.isSupported = IsPixelFormatSupported(VulkanConverter::Transform2VkFormat(format));
            F_LogInfo("VulkanDevice::createDevice: [%d]: [%s] is supported [%s] !", i, des.name.c_str(), des.isSupported ? "true" : "false");
        }

        return true;
    }

    String VulkanDevice::getDeviceType(const VkPhysicalDeviceProperties& deviceProperties, bool& isDiscrete)
    {
        isDiscrete = false;
        String info;
		switch (deviceProperties.deviceType)
		{
		case  VK_PHYSICAL_DEVICE_TYPE_OTHER:
			info = ("Other");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			info = ("Integrated GPU");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			info = ("Discrete GPU");
			isDiscrete = true;
			break;
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			info = ("Virtual GPU");
			break;
		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			info = ("CPU");
			break;
		default:
			info = ("Unknown");
			break;
		}
		return info;
    }

    String VulkanDevice::getQueueType(const VkQueueFamilyProperties& props)
    {
        String info;
        if ((props.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT) 
        {
            info += " Graphics";
        }
        if ((props.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT) 
        {
            info += " Compute";
        }
        if ((props.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT) 
        {
            info += " Transfer";
        }
        if ((props.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) == VK_QUEUE_SPARSE_BINDING_BIT) 
        {
            info += " Sparse";
        }
        return info;
    }

    bool VulkanDevice::isSupportPresent(VulkanQueue* pQueue, VkSurfaceKHR surface)
    {
        const uint32 familyIndex = pQueue->GetFamilyIndex();
        VkBool32 supportsPresent = VK_FALSE;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, familyIndex, surface, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
        {
            F_LogError("*********************** VulkanDevice::isSupportPresent: vkGetPhysicalDeviceSurfaceSupportKHR failed !");
            return false;
        }
        if (supportsPresent) 
        {
            F_LogInfo("VulkanDevice::isSupportPresent: Queue Family: [%d] support Present !", familyIndex);
        }
        return (supportsPresent == VK_TRUE);
    }

    uint32_t VulkanDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) 
    {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) 
        {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) 
            {
                return i;
            }
        }

        String msg = "*********************** VulkanDevice::findMemoryType: Failed to find suitable memory type !";
        F_LogError(msg.c_str());
        throw std::runtime_error(msg);
    }

    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    ////////// VkSemaphore //////////
    VkSemaphore VulkanDevice::CreateVkSemaphore()
    {
        VkSemaphore vkSemaphore;
        CreateVkSemaphore(vkSemaphore);
        return vkSemaphore;
    }
    void VulkanDevice::CreateVkSemaphore(VkSemaphore& vkSemaphore)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        E_ZeroStruct(semaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        if (!E_CheckVkResult(vkCreateSemaphore(m_vkDevice, &semaphoreCreateInfo, E_CPU_ALLOCATOR, &vkSemaphore), "vkCreateSemaphore")) 
        {
            String msg = "*********************** VulkanDevice::CreateVkSemaphore: Failed to create VkSemaphore !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::DestroyVkSemaphore(const VkSemaphore& vkSemaphore)
    {
        if (vkSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(m_vkDevice, vkSemaphore, E_CPU_ALLOCATOR);
        }
    }


    ////////// VkFence //////////////
    VkFence VulkanDevice::CreateVkFence(bool isCreateSignaled)
    {
        VkFence vkFence;
        CreateVkFence(isCreateSignaled, vkFence);
        return vkFence;
    }
    void VulkanDevice::CreateVkFence(bool isCreateSignaled, VkFence& vkFence)
    {
        VkFenceCreateInfo fenceCreateInfo;
        E_ZeroStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        fenceCreateInfo.flags = isCreateSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        if (!E_CheckVkResult(vkCreateFence(m_vkDevice, &fenceCreateInfo, E_CPU_ALLOCATOR, &vkFence), "vkCreateFence")) 
        {
            String msg = "*********************** VulkanDevice::CreateVkFence: Failed to create VkFence !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::DestroyVkFence(const VkFence& vkFence)
    {
        if (vkFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(m_vkDevice, vkFence, E_CPU_ALLOCATOR);
        }
    }


    ////////// VkCommandPool ////////
    VkCommandPool VulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                                    uint32_t queueFamilyIndex)
    {
        VkCommandPool vkCommandPool;
        CreateVkCommandPool(flags, 
                            queueFamilyIndex,
                            vkCommandPool);
        return vkCommandPool;
    }                                   
    void VulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                           uint32_t queueFamilyIndex, 
                                           VkCommandPool& vkCommandPool)
    {
        VkCommandPoolCreateInfo commandPoolInfo = {};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.flags = flags;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        if (!E_CheckVkResult(vkCreateCommandPool(m_vkDevice, &commandPoolInfo, E_CPU_ALLOCATOR, &vkCommandPool), "vkCreateCommandPool")) 
        {
            String msg = "*********************** VulkanDevice::CreateVkCommandPool: Failed to create command pool !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::DestroyVkCommandPool(const VkCommandPool& vkCommandPool)
    {   
        if (vkCommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_vkDevice, vkCommandPool, E_CPU_ALLOCATOR);
        }
    }


    ////////// VkCommandBuffer //////
    VkCommandBuffer VulkanDevice::AllocateVkCommandBuffer(const VkCommandPool& vkCommandPool,
                                                          VkCommandBufferLevel level)
    {
        VkCommandBuffer vkCommandBuffer;
        AllocateVkCommandBuffers(vkCommandPool,
                                 level,
                                 1,
                                 &vkCommandBuffer);
        return vkCommandBuffer;
    }
    void VulkanDevice::AllocateVkCommandBuffers(const VkCommandPool& vkCommandPool,
                                                VkCommandBufferLevel level,
                                                uint32_t commandBufferCount,
                                                VkCommandBuffer* pCommandBuffers)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = level;
        allocInfo.commandPool = vkCommandPool;
        allocInfo.commandBufferCount = commandBufferCount;
        if (!E_CheckVkResult(vkAllocateCommandBuffers(m_vkDevice, &allocInfo, pCommandBuffers), "vkAllocateCommandBuffers")) 
        {
            String msg = "*********************** VulkanDevice::AllocateVkCommandBuffers: Failed to allocate VkCommandBuffers !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::FreeVkCommandBuffers(const VkCommandPool& vkCommandPool, 
                                            uint32_t commandBufferCount, 
                                            VkCommandBuffer* pCommandBuffer)
    {
        if (pCommandBuffer != nullptr)
        {
            vkFreeCommandBuffers(m_vkDevice, vkCommandPool, commandBufferCount, pCommandBuffer);
        }
    }
    
    void VulkanDevice::BeginVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer,
                                            VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        if (!E_CheckVkResult(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo), "vkBeginCommandBuffer")) 
        {
            String msg = "*********************** VulkanDevice::BeginVkCommandBuffer: Failed to call vkBeginCommandBuffer !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer)
    {
        if (!E_CheckVkResult(vkEndCommandBuffer(vkCommandBuffer), "vkEndCommandBuffer")) 
        {
            String msg = "*********************** VulkanDevice::EndVkCommandBuffer: Failed to call vkEndCommandBuffer !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }


    ////////// VkQueue //////////////
    VkQueue VulkanDevice::GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex)
    {
        VkQueue vkQueue;
        vkGetDeviceQueue(m_vkDevice, queueFamilyIndex, queueIndex, &vkQueue);
        return vkQueue;
    }
    void VulkanDevice::QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
                                                   uint32_t commandBufferCount, 
                                                   VkCommandBuffer* pCommandBuffer,
                                                   VkFence vkFence)
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = commandBufferCount;
        submitInfo.pCommandBuffers = pCommandBuffer;
        if (!E_CheckVkResult(vkQueueSubmit(vkQueue, commandBufferCount, &submitInfo, vkFence), "vkQueueSubmit")) 
        {
            String msg = "*********************** VulkanDevice::QueueSubmitVkCommandBuffers: Failed to call vkQueueSubmit !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::QueueWaitIdle(const VkQueue& vkQueue)
    {
        if (!E_CheckVkResult(vkQueueWaitIdle(vkQueue), "vkQueueWaitIdle")) 
        {
            String msg = "*********************** VulkanDevice::QueueWaitIdle: Failed to call vkQueueWaitIdle !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }


    ////////// VkBuffer /////////////
    bool VulkanDevice::CreateVkBuffer(VkDeviceSize size, 
                                      VkBufferUsageFlags usage, 
                                      VkMemoryPropertyFlags properties, 
                                      VkBuffer& vkBuffer, 
                                      VkDeviceMemory& vkBufferMemory)
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        if (!E_CheckVkResult(vkCreateBuffer(m_vkDevice, &bufferInfo, E_CPU_ALLOCATOR, &vkBuffer), "vkCreateBuffer")) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkBuffer: Failed to create VkBuffer !");
            return false;
        }

        VkMemoryRequirements memRequirements = { };
        vkGetBufferMemoryRequirements(m_vkDevice, vkBuffer, &memRequirements);
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(m_vkDevice, &allocInfo, E_CPU_ALLOCATOR, &vkBufferMemory) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkBuffer: Failed to allocate VkDeviceMemory !");
            return false;
        }
        vkBindBufferMemory(m_vkDevice, vkBuffer, vkBufferMemory, 0);

        return true;
    }
    bool VulkanDevice::CreateVkBufferVertex(void* pData, 
                                            uint32_t bufSize, 
                                            VkBuffer& vkBuffer, 
                                            VkDeviceMemory& vkBufferMemory)
    {
        VkBuffer vkBufferTransfer;
        VkDeviceMemory vkBufferMemoryTransfer;
        if (!CreateVkBufferVertex(pData,  
                                  bufSize, 
                                  vkBuffer, 
                                  vkBufferMemory, 
                                  vkBufferTransfer, 
                                  vkBufferMemoryTransfer))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferVertex: CreateVkBufferVertex failed !");
            return false;
        }
        DestroyVkBuffer(vkBufferTransfer, vkBufferMemoryTransfer);

        return true;
    }
    bool VulkanDevice::CreateVkBufferVertex(void* pData, 
                                            uint32_t bufSize,
                                            VkBuffer& vkBuffer, 
                                            VkDeviceMemory& vkBufferMemory,
                                            VkBuffer& vkBufferTransfer, 
                                            VkDeviceMemory& vkBufferMemoryTransfer)
    {
        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                            vkBufferTransfer, 
                            vkBufferMemoryTransfer))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferVertex: 1 CreateVkBuffer failed !");
            return false;
        }
        WriteVkBuffer(vkBufferMemoryTransfer, 
                      pData, 
                      bufSize, 
                      0);

        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                            vkBuffer,
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferVertex: 2 CreateVkBuffer failed !");
            return false;
        }
        CopyVkBuffer(vkBufferTransfer, vkBuffer, bufSize);

        return true;
    }
    bool VulkanDevice::CreateVkBufferIndex(void* pData, 
                                           uint32_t bufSize, 
                                           VkBuffer& vkBuffer, 
                                           VkDeviceMemory& vkBufferMemory)
    {
        VkBuffer vkBufferTransfer;
        VkDeviceMemory vkBufferMemoryTransfer;
        if (!CreateVkBufferIndex(pData, 
                                 bufSize, 
                                 vkBuffer, 
                                 vkBufferMemory, 
                                 vkBufferTransfer, 
                                 vkBufferMemoryTransfer))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferIndex: CreateVkBufferIndex failed !");
            return false;
        }
        DestroyVkBuffer(vkBufferTransfer, vkBufferMemoryTransfer);

        return true;
    }
    bool VulkanDevice::CreateVkBufferIndex(void* pData, 
                                           uint32_t bufSize,  
                                           VkBuffer& vkBuffer, 
                                           VkDeviceMemory& vkBufferMemory,
                                           VkBuffer& vkBufferTransfer, 
                                           VkDeviceMemory& vkBufferMemoryTransfer)
    {
        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBufferTransfer, 
                            vkBufferMemoryTransfer))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferIndex: 1 CreateVkBuffer failed !");
            return false;
        }
        WriteVkBuffer(vkBufferMemoryTransfer, 
                      pData, 
                      bufSize, 
                      0);
        
        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, 
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateVkBufferIndex: 2 CreateVkBuffer failed !");
            return false;
        }
        CopyVkBuffer(vkBufferTransfer, vkBuffer, bufSize);

        return true;
    }
    bool VulkanDevice::CreateVkUniformBuffer(VkDeviceSize bufSize, 
                                             VkBuffer& vkBuffer, 
                                             VkDeviceMemory& vkBufferMemory)
    {
        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateVkUniformBuffer: CreateVkBuffer failed !");
            return false;
        }

        return true;
    }
    bool VulkanDevice::CreateVkUniformBuffers(VkDeviceSize bufSize, 
                                              int bufCount,
                                              VkBufferVector& aBuffer, 
                                              VkDeviceMemoryVector& aBufferMemory)
    {
        aBuffer.clear();
        aBuffer.resize(bufCount);
        aBufferMemory.clear();
        aBufferMemory.resize(bufCount);
        for (int32 i = 0; i < bufCount; i++) 
        {
            if (!CreateVkUniformBuffer(bufSize, 
                                       aBuffer[i], 
                                       aBufferMemory[i]))
            {
                F_LogError("*********************** VulkanDevice::CreateVkUniformBuffers: CreateVkUniformBuffer failed !");
                return false;
            }
        }

        return true;
    }
    void VulkanDevice::CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                                    const VkBuffer& vkBufferSrc, 
                                    const VkBuffer& vkBufferDst, 
                                    VkDeviceSize size)
    {
        VkBufferCopy copyRegion = { };
        copyRegion.size = size;
        vkCmdCopyBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, 1, &copyRegion);
    }
    void VulkanDevice::CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                                    const VkBuffer& vkBufferSrc, 
                                    const VkBuffer& vkBufferDst, 
                                    size_t nSrcOffset, 
                                    size_t nDstOffset, 
                                    VkDeviceSize size)
    {
        VkBufferCopy copyRegion = { nSrcOffset, nDstOffset, size };
        vkCmdCopyBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, 1, &copyRegion);
    }
    void VulkanDevice::CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                                    const VkBuffer& vkBufferDst, 
                                    VkDeviceSize size)
    {
        VkCommandBuffer vkCommandBuffer = BeginSingleTimeCommands();
        {
            CopyVkBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, size);
        }
        EndSingleTimeCommands(vkCommandBuffer);
    }
    void VulkanDevice::CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                                    const VkBuffer& vkBufferDst, 
                                    size_t nSrcOffset, 
                                    size_t nDstOffset, 
                                    VkDeviceSize size)
    {
        VkCommandBuffer vkCommandBuffer = BeginSingleTimeCommands();
        {
            CopyVkBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, nSrcOffset, nDstOffset, size);
        }
        EndSingleTimeCommands(vkCommandBuffer);
    }
    void VulkanDevice::WriteVkBuffer(VkDeviceMemory& vkBufferMemory, 
                                     void* pData, 
                                     uint32_t nDataSize, 
                                     uint32_t nDataOffset,
                                     VkMemoryMapFlags flags /*= 0*/)
    {
        void* pDataDst = MapVkDeviceMemory(vkBufferMemory, nDataSize, nDataOffset, flags);
        if (pDataDst != nullptr)
        {
            memcpy(pDataDst, pData, nDataSize);
            UnmapVkDeviceMemory(vkBufferMemory);
        }
    }            
    void VulkanDevice::DestroyVkBuffer(const VkBuffer& vkBuffer, const VkDeviceMemory& vkBufferMemory)
    {
        if (vkBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(m_vkDevice, vkBuffer, E_CPU_ALLOCATOR);
            vkFreeMemory(m_vkDevice, vkBufferMemory, E_CPU_ALLOCATOR);
        }
    }
    void VulkanDevice::DestroyVkBuffers(VkBufferVector& aBuffer, VkDeviceMemoryVector& aBufferMemory)
    {
        size_t count = aBuffer.size();
        for (size_t i = 0; i < count; i++) 
        {
            DestroyVkBuffer(aBuffer[i], aBufferMemory[i]);
        }
    }

    void* VulkanDevice::MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
                                          uint32_t nDataSize, 
                                          uint32_t nDataOffset,
                                          VkMemoryMapFlags flags /*= 0*/)
    {
        void* pData = nullptr;
        if (!MapVkDeviceMemory(vkBufferMemory,
                               nDataSize,
                               nDataOffset,
                               flags,
                               &pData))
        {
            F_LogError("*********************** VulkanDevice::MapVkDeviceMemory: MapVkDeviceMemory failed !");
            return nullptr;
        }

        return pData;
    }
    bool VulkanDevice::MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
                                         uint32_t nDataSize, 
                                         uint32_t nDataOffset,
                                         VkMemoryMapFlags flags,
                                         void** ppData)
    {
        if (!E_CheckVkResult(vkMapMemory(m_vkDevice, vkBufferMemory, nDataOffset, nDataSize, flags, ppData), "vkMapMemory")) 
        {
            F_LogError("*********************** VulkanDevice::MapVkDeviceMemory: Failed to call vkMapMemory !");
            return false;
        }

        return true;
    }
    void VulkanDevice::UnmapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory)
    {
        vkUnmapMemory(m_vkDevice, vkBufferMemory);
    }


    ////////// VkImage //////////////
    bool VulkanDevice::CreateVkImage(uint32_t width, 
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
                                     VkImage& vkImage, 
                                     VkDeviceMemory& vkImageMemory) 
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
            if (this->m_pQueueGraphics->GetFamilyIndex() != this->m_pQueueCompute->GetFamilyIndex()) 
            {
                std::vector<uint32_t> queueFamilyIndices;
                queueFamilyIndices.push_back(this->m_pQueueGraphics->GetFamilyIndex());
                queueFamilyIndices.push_back(this->m_pQueueCompute->GetFamilyIndex());
                imageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
                imageCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
                imageCreateInfo.pQueueFamilyIndices = queueFamilyIndices.data();
            }
        }
        imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

        if (vkCreateImage(this->m_vkDevice, &imageCreateInfo, nullptr, &vkImage) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkImage: Failed to create image !");
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(this->m_vkDevice, vkImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(this->m_vkDevice, &allocInfo, nullptr, &vkImageMemory) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkImage: Failed to allocate image memory !");
            return false;
        }
        vkBindImageMemory(this->m_vkDevice, vkImage, vkImageMemory, 0);
        return true;
    }
    bool VulkanDevice::CreateVkImageView(VkImage vkImage, 
                                         VkImageViewType type, 
                                         VkFormat format, 
                                         VkImageAspectFlags aspectFlags, 
                                         uint32_t mipMapCount,
                                         uint32_t numArray,
                                         VkImageView& vkImageView) 
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = vkImage;
        viewInfo.viewType = type;
        viewInfo.format = format;
        viewInfo.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
        viewInfo.subresourceRange.aspectMask = aspectFlags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = mipMapCount;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = numArray;

        if (vkCreateImageView(this->m_vkDevice, &viewInfo, nullptr, &vkImageView) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkImageView: Failed to create texture image view !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::CreateVkSampler(uint32_t mipMapCount, 
                                       VkSampler& vkSampler)
    {
        return CreateVkSampler(F_TextureFilter_Bilinear,
                               F_TextureAddressing_Clamp,
                               F_TextureBorderColor_OpaqueBlack,
                               true,
                               this->m_vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy,
                               0.0f,
                               static_cast<float>(mipMapCount),
                               0.0f,
                               vkSampler);
    }
    bool VulkanDevice::CreateVkSampler(FTextureFilterType eTextureFilter,
                                       FTextureAddressingType eTextureAddressing,
                                       FTextureBorderColorType eTextureBorderColor,
                                       bool enableAnisotropy,
                                       float maxAnisotropy,
                                       float minLod, 
                                       float maxLod, 
                                       float mipLodBias,
                                       VkSampler& vkSampler)
    {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.minFilter = VulkanConverter::Transform2VkFilter(eTextureFilter, F_TextureFilterSize_Min);
        samplerInfo.magFilter = VulkanConverter::Transform2VkFilter(eTextureFilter, F_TextureFilterSize_Mag);
        samplerInfo.addressModeU = VulkanConverter::Transform2VkSamplerAddressMode(eTextureAddressing);
        samplerInfo.addressModeV = VulkanConverter::Transform2VkSamplerAddressMode(eTextureAddressing);
        samplerInfo.addressModeW = VulkanConverter::Transform2VkSamplerAddressMode(eTextureAddressing);
        samplerInfo.anisotropyEnable = enableAnisotropy ? VK_TRUE : VK_FALSE;
        samplerInfo.maxAnisotropy = maxAnisotropy > this->m_vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy ? this->m_vkPhysicalDeviceProperties.limits.maxSamplerAnisotropy : maxAnisotropy;
        samplerInfo.borderColor = VulkanConverter::Transform2VkBorderColor(eTextureBorderColor);
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VulkanConverter::Transform2VkSamplerMipmapMode(eTextureFilter);
        samplerInfo.minLod = minLod;
        samplerInfo.maxLod = maxLod;
        samplerInfo.mipLodBias = mipLodBias;

        if (vkCreateSampler(this->m_vkDevice, &samplerInfo, nullptr, &vkSampler) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkSampler: Failed to create texture sampler !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkImage(VkImage vkImage, VkDeviceMemory vkImageMemory, VkImageView vkImageView)
    {
        if (vkImage != VK_NULL_HANDLE)
        {
            vkDestroyImage(this->m_vkDevice, vkImage, nullptr);
            vkFreeMemory(this->m_vkDevice, vkImageMemory, nullptr);
        }
        DestroyVkImageView(vkImageView);
    }
    void VulkanDevice::DestroyVkImageView(VkImageView vkImageView)
    {   
        if (vkImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(this->m_vkDevice, vkImageView, nullptr);
        }
    }
    void VulkanDevice::DestroyVkImageSampler(VkSampler vkSampler)
    {
        if (vkSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(this->m_vkDevice, vkSampler, nullptr);
        }
    }

    void VulkanDevice::TransitionVkImageLayout(VkCommandBuffer cmdBuffer,
                                               VkImage vkImage, 
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
            cmdBuffer = BeginSingleTimeCommands();
        }
        {
            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = oldLayout;
            barrier.newLayout = newLayout;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = vkImage;

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
                F_LogError("*********************** VulkanDevice::TransitionVkImageLayout: Unsupported layout transition !");
                throw std::invalid_argument("*********************** VulkanDevice::TransitionVkImageLayout: Unsupported layout transition !");
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
            EndSingleTimeCommands(cmdBuffer);
        }
    }
    void VulkanDevice::CopyVkBufferToVkImage(VkCommandBuffer cmdBuffer,
                                             VkBuffer vkBuffer, 
                                             VkImage vkImage, 
                                             uint32_t width, 
                                             uint32_t height,
                                             uint32_t depth,
                                             uint32_t numArray) 
    {
        bool isCreate = false;
        if (cmdBuffer == VK_NULL_HANDLE)
        {
            isCreate = true;
            cmdBuffer = BeginSingleTimeCommands();
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
            vkCmdCopyBufferToImage(cmdBuffer, 
                                   vkBuffer, 
                                   vkImage, 
                                   VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                   (uint32_t)bufferCopyRegions.size(), 
                                   bufferCopyRegions.data());
        }
        if (isCreate)
        {
            EndSingleTimeCommands(cmdBuffer);
        }
    }
    void VulkanDevice::GenerateVkImageMipMaps(VkCommandBuffer cmdBuffer,
                                              VkImage vkImage, 
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
            cmdBuffer = BeginSingleTimeCommands();
        }
        {
            if (autoMipMap)
            {
                int32_t mipWidth = width;
                int32_t mipHeight = height;
                for (uint32_t i = 1; i < mipMapCount; i++) 
                {
                    TransitionVkImageLayout(cmdBuffer,
                                            vkImage, 
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
                                       vkImage, 
                                       VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                       vkImage, 
                                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                       1, 
                                       &blit,
                                       VK_FILTER_LINEAR);

                        if (mipWidth > 1) 
                            mipWidth /= 2;
                        if (mipHeight > 1) 
                            mipHeight /= 2;
                    }   
                    TransitionVkImageLayout(cmdBuffer,
                                            vkImage,
                                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                            VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                            i,
                                            1,
                                            0,
                                            numArray);
                }
            }

            TransitionVkImageLayout(cmdBuffer,
                                    vkImage,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    0,
                                    mipMapCount,
                                    0,
                                    numArray);
        }
        if (isCreate)
        {
            EndSingleTimeCommands(cmdBuffer);
        }
    }

    bool VulkanDevice::CreateTexture1D(const String& pathAsset, 
                                       uint32_t& mipMapCount,
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory)
    {
        return CreateTexture2D(pathAsset,
                               VK_IMAGE_TYPE_1D,
                               VK_SAMPLE_COUNT_1_BIT,
                               VK_FORMAT_R8G8B8A8_SRGB,
                               true,
                               mipMapCount,
                               vkImage,
                               vkImageMemory);
    }
    
    bool VulkanDevice::CreateTexture2D(const String& pathAsset, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory,
                                       VkBuffer& vkBuffer, 
                                       VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
        Image image;
        if (!image.Load(pathTexture))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2D: Failed to load texture image: [%s] !", pathAsset.c_str());
            return false;
        }
        int width = (int)image.GetWidth();
        int height = (int)image.GetHeight();
        int pixelSize = (int)image.GetPixelSize();
        VkDeviceSize imageSize = width * height * pixelSize;
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        uint8* pData = image.GetData();

        //2> Create Buffer and copy Texture data to buffer
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2D: Failed to create vk buffer: [%s] !", pathTexture.c_str());
            return false;
        }

        WriteVkBuffer(vkBufferMemory, 
                      (void*)pData,
                      imageSize, 
                      0);

        uint32_t depth = 1;
        uint32_t numArray = 1;

        //3> CreateImage
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2D: Failed to create vk image: [%s] !", pathTexture.c_str());
            return false;
        }

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = BeginSingleTimeCommands();
        {
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_UNDEFINED, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);
            {
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(width), 
                                      static_cast<uint32_t>(height),
                                      static_cast<uint32_t>(depth), 
                                      numArray);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   format, 
                                   width, 
                                   height, 
                                   mipMapCount,
                                   numArray,
                                   autoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool VulkanDevice::CreateTexture2D(const String& pathAsset, 
                                       VkImageType type,
                                       VkSampleCountFlagBits numSamples,
                                       VkFormat format,
                                       bool autoMipMap, 
                                       uint32_t& mipMapCount, 
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture2D(pathAsset, 
                                    type, 
                                    numSamples,
                                    format,
                                    autoMipMap,
                                    mipMapCount,
                                    vkImage, 
                                    vkImageMemory, 
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool VulkanDevice::CreateTexture2D(const String& pathAsset, 
                                       uint32_t& mipMapCount,
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory)
    {
        return CreateTexture2D(pathAsset,
                               VK_IMAGE_TYPE_2D,
                               VK_SAMPLE_COUNT_1_BIT,
                               VK_FORMAT_R8G8B8A8_SRGB,
                               true,
                               mipMapCount,
                               vkImage,
                               vkImageMemory);
    }
    
    static void s_DeleteImages(const ImagePtrVector& aImages)
    {
        int count = (int)aImages.size();
        for (int i = 0; i < count; i++)
        {
            Image* pImage = aImages[i];
            F_DELETE(pImage)
        }
    }
    bool VulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory,
                                            VkBuffer& vkBuffer, 
                                            VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<int> aPixelSize;
        ImagePtrVector aImages;

        size_t count_tex = aPathAsset.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Texture path count <= 0 !");
            return false;
        }
        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset = aPathAsset[i];
            String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
            Image* pImage = new Image;
            aImages.push_back(pImage);
            if (!pImage->Load(pathTexture))
            {
                F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Failed to load texture image: [%s] !", pathAsset.c_str());
                s_DeleteImages(aImages);
                return false;
            }
            int width = (int)pImage->GetWidth();
            int height = (int)pImage->GetHeight();
            int pixelSize = (int)pImage->GetPixelSize();
            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixelSize.push_back(pixelSize);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        int pixelSize = aPixelSize[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Texture image's all width must the same !");
                s_DeleteImages(aImages);
                return false;
            }
            if (aHeight[i] != height)
            {
                F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Texture image's all height must the same !");
                s_DeleteImages(aImages);
                return false;
            }
            if (aPixelSize[i] != pixelSize)
            {
                F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Texture image's all pixel size must the same !");
                s_DeleteImages(aImages);
                return false;
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
        VkDeviceSize imageSize = width * height * pixelSize;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Failed to create vk buffer !");
            s_DeleteImages(aImages);
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            Image* pImage = aImages[i];
            WriteVkBuffer(vkBufferMemory,
                          (void*)pImage->GetData(),
                          imageSizeAll,
                          width * height * pixelSize * i);
        }
        s_DeleteImages(aImages);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture2DArray: Failed to create vk image 2d array !");
            return false;
        }

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = BeginSingleTimeCommands();
        {
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_UNDEFINED, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);
            {   
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(width), 
                                      static_cast<uint32_t>(height),
                                      static_cast<uint32_t>(depth), 
                                      numArray);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   format, 
                                   width, 
                                   height,
                                   mipMapCount,
                                   numArray,
                                   autoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool VulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                            VkImageType type,
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture2DArray(aPathAsset, 
                                         type, 
                                         numSamples,
                                         format,
                                         autoMipMap,
                                         mipMapCount,
                                         vkImage, 
                                         vkImageMemory, 
                                         vkStagingBuffer, 
                                         vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool VulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                            uint32_t& mipMapCount,
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory)
    {
        return CreateTexture2DArray(aPathAsset,
                                    VK_IMAGE_TYPE_2D,
                                    VK_SAMPLE_COUNT_1_BIT,
                                    VK_FORMAT_R8G8B8A8_SRGB,
                                    true,
                                    mipMapCount,
                                    vkImage,
                                    vkImageMemory);
    }
    
    bool VulkanDevice::CreateTexture3D(VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory,
                                       VkBuffer& vkBuffer, 
                                       VkDeviceMemory& vkBufferMemory)
    {
        VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(this->m_vkPhysicalDevice, format, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT))
		{
            F_LogError("*********************** VulkanDevice::CreateTexture3D: Physical device does not support flag 'VK_FORMAT_FEATURE_TRANSFER_DST_BIT' for selected texture format !");
            return false;
		}
		uint32_t maxImageDimension3D(this->m_vkPhysicalDeviceProperties.limits.maxImageDimension3D);
		if (width > maxImageDimension3D || height > maxImageDimension3D || depth > maxImageDimension3D)
		{
            F_LogError("*********************** VulkanDevice::CreateTexture3D: Requested texture dimensions is greater than supported 3D texture dimension !");
			return false;
		}

        //1> Create Buffer and copy Texture data to buffer
        VkDeviceSize imageSize = size;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture3D: Failed to create vk buffer !");
            return false;
        }

        WriteVkBuffer(vkBufferMemory, 
                      (void*)pDataRGBA, 
                      imageSize, 
                      0);

        //2> CreateImage
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTexture3D: Failed to create vk image 3d !");
            return false;
        }

        //3> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = BeginSingleTimeCommands();
        {
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_UNDEFINED, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    1);
            {   
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(width), 
                                      static_cast<uint32_t>(height),
                                      static_cast<uint32_t>(depth), 
                                      1);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    1);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   format, 
                                   width, 
                                   height,
                                   1,
                                   1,
                                   false);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool VulkanDevice::CreateTexture3D(VkFormat format,
                                       const uint8* pDataRGBA,
                                       uint32_t size,
                                       uint32_t width,
                                       uint32_t height,
                                       uint32_t depth,
                                       VkImage& vkImage, 
                                       VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture3D(format, 
                                    pDataRGBA, 
                                    size,
                                    width,
                                    height,
                                    depth,
                                    vkImage, 
                                    vkImageMemory, 
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory,
                                            VkBuffer& vkBuffer, 
                                            VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<int> aPixelSize;
        ImagePtrVector aImages;

        size_t count_tex = aPathAsset.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Texture path count <= 0 !");
            return false;
        }
        if (count_tex != 6)
        {
            F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Texture path count != 6 !");
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset = aPathAsset[i];
            String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
            Image* pImage = new Image;
            aImages.push_back(pImage);
            if (!pImage->Load(pathTexture))
            {
                F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Failed to load texture image: [%s] !", pathAsset.c_str());
                s_DeleteImages(aImages);
                return false;
            }
            int width = (int)pImage->GetWidth();
            int height = (int)pImage->GetHeight();
            int pixelSize = (int)pImage->GetPixelSize();
            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixelSize.push_back(pixelSize);
        }

        int width = aWidth[0];
        int height = aHeight[0];
        int pixelSize = aPixelSize[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != width)
            {
                F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Texture image's all width must the same !");
                s_DeleteImages(aImages);
                return false;
            }
            if (aHeight[i] != height)
            {
                F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Texture image's all height must the same !");
                s_DeleteImages(aImages);
                return false;
            }
            if (aPixelSize[i] != pixelSize)
            {
                F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Texture image's all pixel size must the same !");
                s_DeleteImages(aImages);
                return false;
            }
        }

        uint32_t depth = 1;
        uint32_t numArray = count_tex;

        //2> Create Buffer and copy Texture data to buffer
        mipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
        VkDeviceSize imageSize = width * height * pixelSize;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Failed to create vk buffer !");
            s_DeleteImages(aImages);
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            Image* pImage = aImages[i];
            WriteVkBuffer(vkBufferMemory,
                          (void*)pImage->GetData(),
                          imageSizeAll,
                          width * height * pixelSize * i);
        }
        s_DeleteImages(aImages);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureCubeMap: Failed to create vk image cubemap !");
            return false;
        }

        //4> TransitionImageLayout, CopyBufferToImage, GenerateMipMaps
        VkCommandBuffer cmdBuffer = BeginSingleTimeCommands();
        {
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_UNDEFINED, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);
            {   
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(width), 
                                      static_cast<uint32_t>(height),
                                      static_cast<uint32_t>(depth), 
                                      numArray);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    numArray);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   format, 
                                   width, 
                                   height,
                                   mipMapCount,
                                   numArray,
                                   autoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool VulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset, 
                                            VkSampleCountFlagBits numSamples,
                                            VkFormat format,
                                            bool autoMipMap, 
                                            uint32_t& mipMapCount, 
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureCubeMap(aPathAsset, 
                                         numSamples, 
                                         format,
                                         autoMipMap,
                                         mipMapCount,
                                         vkImage, 
                                         vkImageMemory, 
                                         vkStagingBuffer, 
                                         vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool VulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset,
                                            uint32_t& mipMapCount, 
                                            VkImage& vkImage, 
                                            VkDeviceMemory& vkImageMemory)
    {
        return CreateTextureCubeMap(aPathAsset, 
                                    VK_SAMPLE_COUNT_1_BIT, 
                                    VK_FORMAT_R8G8B8A8_SRGB,
                                    true,
                                    mipMapCount,
                                    vkImage, 
                                    vkImageMemory);
    }

    bool VulkanDevice::CreateTextureRenderTarget1D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory,
                                                   VkBuffer& vkBuffer, 
                                                   VkDeviceMemory& vkBufferMemory)
    {
        return CreateTextureRenderTarget2D(clDefault,
                                           isSetColor,
                                           width,
                                           1,
                                           mipMapCount,
                                           VK_IMAGE_TYPE_1D,
                                           numSamples,
                                           format,
                                           usage,
                                           isGraphicsComputeShared,
                                           vkImage,
                                           vkImageMemory,
                                           vkBuffer,
                                           vkBufferMemory);
    }
    bool VulkanDevice::CreateTextureRenderTarget1D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget1D(clDefault, 
                                                isSetColor,
                                                width, 
                                                mipMapCount,
                                                numSamples,
                                                format,
                                                usage,
                                                isGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureRenderTarget2D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkImageType type,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory,
                                                   VkBuffer& vkBuffer, 
                                                   VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2D: Failed to create vk buffer !");
            return false;
        }
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->m_vkDevice, vkBufferMemory, 0, imageSize, 0, &data);
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
            vkUnmapMemory(this->m_vkDevice, vkBufferMemory);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2D: Failed to create vk image 2d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                mipMapCount,
                                0,
                                numArray);
        return true;
    }
    bool VulkanDevice::CreateTextureRenderTarget2D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2D(clDefault, 
                                                isSetColor,
                                                width, 
                                                height,
                                                mipMapCount,
                                                VK_IMAGE_TYPE_2D,
                                                numSamples,
                                                format,
                                                usage,
                                                isGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureRenderTarget2D(uint8* pData,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkImageType type,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory,
                                                   VkBuffer& vkBuffer, 
                                                   VkDeviceMemory& vkBufferMemory)
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
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2D: Failed to create vk buffer !");
            return false;
        }
        if (pData != nullptr)
        {
            WriteVkBuffer(vkBufferMemory,   
                          (void*)pData,
                          imageSize, 
                          0);
        }
        
        //2> CreateImage
        uint32_t depth = 1;
        uint32_t numArray = 1;
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2D: Failed to create vk image 2d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                mipMapCount,
                                0,
                                numArray);
        return true;
    }
    bool VulkanDevice::CreateTextureRenderTarget2D(uint8* pData,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2D(pData,
                                                width, 
                                                height,
                                                mipMapCount,
                                                VK_IMAGE_TYPE_2D,
                                                numSamples,
                                                format,
                                                usage,
                                                isGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureRenderTarget2DArray(const FVector4& clDefault,
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
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory,
                                                        VkBuffer& vkBuffer, 
                                                        VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2DArray: Failed to create vk buffer !");
            return false;
        }
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->m_vkDevice, vkBufferMemory, 0, imageSizeAll, 0, &data);
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
            vkUnmapMemory(this->m_vkDevice, vkBufferMemory);
        }

        //2> CreateImage
        uint32_t depth = 1;
        if (type == VK_IMAGE_TYPE_1D)
        {
            depth = 0;
        }
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget2DArray: Failed to create vk image 2d array !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                mipMapCount,
                                0,
                                numArray);
        return true;
    }
    bool VulkanDevice::CreateTextureRenderTarget2DArray(const FVector4& clDefault,
                                                        bool isSetColor,
                                                        uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t numArray,
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        bool isGraphicsComputeShared,
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2DArray(clDefault, 
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
                                                     vkImage, 
                                                     vkImageMemory, 
                                                     vkStagingBuffer, 
                                                     vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureRenderTarget3D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t depth,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory,
                                                   VkBuffer& vkBuffer, 
                                                   VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * depth * 4;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget3D: Failed to create vk buffer !");
            return false;
        }
        if (isSetColor)
        {
            uint8 r = (uint8)(clDefault.x * 255);
            uint8 g = (uint8)(clDefault.y * 255);
            uint8 b = (uint8)(clDefault.z * 255);
            uint8 a = (uint8)(clDefault.w * 255);

            void* data;
            vkMapMemory(this->m_vkDevice, vkBufferMemory, 0, imageSize, 0, &data);
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
            vkUnmapMemory(this->m_vkDevice, vkBufferMemory);
        }

        //2> CreateImage
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTarget3D: Failed to create vk image 3d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                mipMapCount,
                                0,
                                1);
        return true;
    }
    bool VulkanDevice::CreateTextureRenderTarget3D(const FVector4& clDefault,
                                                   bool isSetColor,
                                                   uint32_t width, 
                                                   uint32_t height,
                                                   uint32_t depth,
                                                   uint32_t mipMapCount,
                                                   VkSampleCountFlagBits numSamples,
                                                   VkFormat format,
                                                   VkImageUsageFlags usage, 
                                                   bool isGraphicsComputeShared,
                                                   VkImage& vkImage, 
                                                   VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget3D(clDefault, 
                                                isSetColor,
                                                width, 
                                                height,
                                                depth,
                                                mipMapCount,
                                                numSamples,
                                                format,
                                                usage,
                                                isGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool VulkanDevice::CreateTextureRenderTargetCubeMap(uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        bool isGraphicsComputeShared,
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory,
                                                        VkBuffer& vkBuffer, 
                                                        VkDeviceMemory& vkBufferMemory)
    {
        uint32_t numArray = 6;
        //1> CreateBuffer
        VkDeviceSize imageSize = width * height * 4;
        VkDeviceSize imageSizeAll = imageSize * numArray;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTargetCubeMap: Failed to create vk buffer !");
            return false;
        }

        //2> CreateImage
        uint32_t depth = 1;
        if (!CreateVkImage(width, 
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
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** VulkanDevice::CreateTextureRenderTargetCubeMap: Failed to create vk image cubemap !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                mipMapCount,
                                0,
                                numArray);
        return true;
    }
    bool VulkanDevice::CreateTextureRenderTargetCubeMap(uint32_t width, 
                                                        uint32_t height,
                                                        uint32_t mipMapCount,
                                                        VkSampleCountFlagBits numSamples,
                                                        VkFormat format,
                                                        VkImageUsageFlags usage, 
                                                        bool isGraphicsComputeShared,
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTargetCubeMap(width, 
                                                     height,
                                                     mipMapCount,
                                                     numSamples,
                                                     format,
                                                     usage,
                                                     isGraphicsComputeShared,
                                                     vkImage, 
                                                     vkImageMemory, 
                                                     vkStagingBuffer, 
                                                     vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }     

    ////////// Command //////////////
    VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
    {
        VkCommandBuffer vkCommandBuffer = AllocateVkCommandBuffer(m_vkCommandPoolTransfer, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        BeginVkCommandBuffer(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        return vkCommandBuffer;
    }
    void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer)
    {
        EndVkCommandBuffer(vkCommandBuffer);
        QueueSubmitVkCommandBuffers(m_pQueueTransfer->GetVkQueue(), 1, &vkCommandBuffer, nullptr);
        QueueWaitIdle(m_pQueueTransfer->GetVkQueue());
        FreeVkCommandBuffers(m_vkCommandPoolTransfer, 1, &vkCommandBuffer);
    }

}; //LostPeterPluginRendererVulkan