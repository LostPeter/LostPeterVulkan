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
#include "../include/VulkanVolk.h"
#include "../include/VulkanQueue.h"
#include "../include/VulkanSemaphore.h"
#include "../include/VulkanFence.h"
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

        DestroyVkCommandPool(this->m_vkCommandPoolTransfer);
        this->m_vkCommandPoolTransfer = VK_NULL_HANDLE;
        DestroyVkDevice(this->m_vkDevice);
        this->m_vkDevice = VK_NULL_HANDLE;
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

    bool VulkanDevice::CreateQueuePresent(VkSurfaceKHR vkSurfaceKHR)
    {
        if (m_pQueuePresent != nullptr)
            return true;

        bool compute = isSupportPresent(m_pQueueCompute, vkSurfaceKHR);
        if (m_pQueueTransfer->GetFamilyIndex() != m_pQueueGraphics->GetFamilyIndex() && 
            m_pQueueTransfer->GetFamilyIndex() != m_pQueueCompute->GetFamilyIndex()) 
        {
            isSupportPresent(m_pQueueTransfer, vkSurfaceKHR);
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

        String msg = "*********************** VulkanDevice::FindSupportedFormat: Failed to find supported format !";
        F_LogError(msg.c_str());
        throw std::runtime_error(msg);
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

        String msg = "*********************** VulkanDevice::FindMemoryType: Failed to find suitable memory type !";
        F_LogError(msg.c_str());
        throw std::runtime_error(msg);
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
                F_LogInfo("VulkanDevice::createDevice: Skipping unnecessary Queue Family: [%d : %d], Queues: [%s ]", i, currProps.queueCount, getQueueType(currProps).c_str());
                continue;
            }

            VkDeviceQueueCreateInfo queueCreateInfo;
            E_ZeroStruct(queueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
            queueCreateInfo.queueFamilyIndex = i;
            queueCreateInfo.queueCount = currProps.queueCount;
            numPriorities += currProps.queueCount;
            queueFamilyInfos.push_back(queueCreateInfo);
            
            F_LogInfo("VulkanDevice::createDevice: Initializing Queue Family: [%d: %d], Queues: [%s ]", i,  currProps.queueCount, getQueueType(currProps).c_str());
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
        m_pInstance->GetVolk()->VolkLoadDevice(m_vkDevice);

        //VulkanQueue Graphics
        m_pQueueGraphics = new VulkanQueue();
        if (!m_pQueueGraphics->Init(this, queueFamilyIndex_Graphics))
        {
            F_LogError("*********************** VulkanDevice::createDevice: VulkanQueue Graphics init failed !");
            return false;
        }

        //VulkanQueue Compute
        if (queueFamilyIndex_Compute == -1) 
        {
            queueFamilyIndex_Compute = queueFamilyIndex_Graphics;
        }
        m_pQueueCompute = new VulkanQueue();
        if (!m_pQueueCompute->Init(this, queueFamilyIndex_Compute))
        {
            F_LogError("*********************** VulkanDevice::createDevice: VulkanQueue Compute init failed !");
            return false;
        }

        //VulkanQueue Transfer
        if (queueFamilyIndex_Transfer == -1) 
        {
            queueFamilyIndex_Transfer = queueFamilyIndex_Compute;
        }
        m_pQueueTransfer = new VulkanQueue();
        if (!m_pQueueTransfer->Init(this, queueFamilyIndex_Transfer))
        {
            F_LogError("*********************** VulkanDevice::createDevice: VulkanQueue Transfer init failed !");
            return false;
        }

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

    bool VulkanDevice::isSupportPresent(VulkanQueue* pQueue, VkSurfaceKHR vkSurfaceKHR)
    {
        const uint32 familyIndex = pQueue->GetFamilyIndex();
        VkBool32 supportsPresent = VK_FALSE;
        if (!E_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, familyIndex, vkSurfaceKHR, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
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
    //////////////////// VkDevice ///////////////////////
    void VulkanDevice::DestroyVkDevice(const VkDevice& vkDevice)
    {
        if (vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(vkDevice, nullptr);
        }
    }


    //////////////////// VkCommandPool //////////////////
    VkCommandPool VulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                                    uint32_t queueFamilyIndex)
    {
        VkCommandPool vkCommandPool = VK_NULL_HANDLE;
        CreateVkCommandPool(flags, 
                            queueFamilyIndex,
                            vkCommandPool);
        return vkCommandPool;
    }                                   
    bool VulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                           uint32_t queueFamilyIndex, 
                                           VkCommandPool& vkCommandPool)
    {
        VkCommandPoolCreateInfo commandPoolInfo = {};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.flags = flags;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        if (!E_CheckVkResult(vkCreateCommandPool(this->m_vkDevice, &commandPoolInfo, E_CPU_ALLOCATOR, &vkCommandPool), "vkCreateCommandPool")) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkCommandPool: Failed to create command pool !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkCommandPool(const VkCommandPool& vkCommandPool)
    {   
        if (vkCommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(this->m_vkDevice, vkCommandPool, E_CPU_ALLOCATOR);
        }
    }


    //////////////////// VkCommandBuffer ////////////////
    VkCommandBuffer VulkanDevice::AllocateVkCommandBuffer(const VkCommandPool& vkCommandPool,
                                                          VkCommandBufferLevel level)
    {
        VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
        AllocateVkCommandBuffers(vkCommandPool,
                                 level,
                                 1,
                                 &vkCommandBuffer);
        return vkCommandBuffer;
    }
    bool VulkanDevice::AllocateVkCommandBuffers(const VkCommandPool& vkCommandPool,
                                                VkCommandBufferLevel level,
                                                uint32_t commandBufferCount,
                                                VkCommandBuffer* pCommandBuffers)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = level;
        allocInfo.commandPool = vkCommandPool;
        allocInfo.commandBufferCount = commandBufferCount;
        if (!E_CheckVkResult(vkAllocateCommandBuffers(this->m_vkDevice, &allocInfo, pCommandBuffers), "vkAllocateCommandBuffers")) 
        {
            F_LogError("*********************** VulkanDevice::AllocateVkCommandBuffers: Failed to allocate VkCommandBuffers !");
            return false;
        }
        return true;
    }
    void VulkanDevice::FreeVkCommandBuffers(const VkCommandPool& vkCommandPool, 
                                            uint32_t commandBufferCount, 
                                            VkCommandBuffer* pCommandBuffer)
    {
        if (pCommandBuffer != nullptr)
        {
            vkFreeCommandBuffers(this->m_vkDevice, vkCommandPool, commandBufferCount, pCommandBuffer);
        }
    }
    
    bool VulkanDevice::BeginVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer,
                                            VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        if (!E_CheckVkResult(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo), "vkBeginCommandBuffer")) 
        {
            F_LogError("*********************** VulkanDevice::BeginVkCommandBuffer: Failed to call vkBeginCommandBuffer !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer)
    {
        if (!E_CheckVkResult(vkEndCommandBuffer(vkCommandBuffer), "vkEndCommandBuffer")) 
        {
            F_LogError("*********************** VulkanDevice::EndVkCommandBuffer: Failed to call vkEndCommandBuffer !");
            return false;
        }
        return true;
    }


    //////////////////// VkQueue ////////////////////////
    VkQueue VulkanDevice::GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex)
    {
        VkQueue vkQueue;
        vkGetDeviceQueue(this->m_vkDevice, queueFamilyIndex, queueIndex, &vkQueue);
        return vkQueue;
    }
    bool VulkanDevice::QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
                                                   uint32_t commandBufferCount, 
                                                   VkCommandBuffer* pCommandBuffer,
                                                   uint32_t signalSemaphoreCount,
                                                   VkSemaphore* pSignalSemaphores,
                                                   VkFence vkFence)
    {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = commandBufferCount;
        submitInfo.pCommandBuffers = pCommandBuffer;
        submitInfo.signalSemaphoreCount = signalSemaphoreCount;
        submitInfo.pSignalSemaphores = pSignalSemaphores;
        if (!E_CheckVkResult(vkQueueSubmit(vkQueue, commandBufferCount, &submitInfo, vkFence), "vkQueueSubmit")) 
        {
            F_LogError("*********************** VulkanDevice::QueueSubmitVkCommandBuffers: Failed to call vkQueueSubmit !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::QueueWaitIdle(const VkQueue& vkQueue)
    {
        if (!E_CheckVkResult(vkQueueWaitIdle(vkQueue), "vkQueueWaitIdle")) 
        {
            F_LogError("*********************** VulkanDevice::QueueWaitIdle: Failed to call vkQueueWaitIdle !");
            return false;
        }
        return true;
    }


    //////////////////// VkSemaphore ////////////////////
    VkSemaphore VulkanDevice::CreateVkSemaphore()
    {
        VkSemaphore vkSemaphore;
        CreateVkSemaphore(vkSemaphore);
        return vkSemaphore;
    }
    bool VulkanDevice::CreateVkSemaphore(VkSemaphore& vkSemaphore)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        E_ZeroStruct(semaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        if (!E_CheckVkResult(vkCreateSemaphore(this->m_vkDevice, &semaphoreCreateInfo, E_CPU_ALLOCATOR, &vkSemaphore), "vkCreateSemaphore")) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkSemaphore: Failed to create VkSemaphore !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkSemaphore(const VkSemaphore& vkSemaphore)
    {
        if (vkSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(this->m_vkDevice, vkSemaphore, E_CPU_ALLOCATOR);
        }
    }

    void VulkanDevice::DestroyVkSemaphore(VulkanSemaphore* pSemaphore)
    {
        F_DELETE(pSemaphore)
    }
    void VulkanDevice::DestroyVkSemaphores(VulkanSemaphorePtrVector& aSemaphore)
    {
        size_t count = aSemaphore.size();
        for (size_t i = 0; i < count; i++)
        {
            VulkanSemaphore* pSemaphore = aSemaphore[i];
            F_DELETE(pSemaphore)
        }
    }


    //////////////////// VkFence ////////////////////////
    VkFence VulkanDevice::CreateVkFence(bool isCreateSignaled)
    {
        VkFence vkFence;
        CreateVkFence(isCreateSignaled, vkFence);
        return vkFence;
    }
    bool VulkanDevice::CreateVkFence(bool isCreateSignaled, VkFence& vkFence)
    {
        VkFenceCreateInfo fenceCreateInfo;
        E_ZeroStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        fenceCreateInfo.flags = isCreateSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        if (!E_CheckVkResult(vkCreateFence(this->m_vkDevice, &fenceCreateInfo, E_CPU_ALLOCATOR, &vkFence), "vkCreateFence")) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkFence: Failed to create VkFence !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkFence(const VkFence& vkFence)
    {
        if (vkFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(this->m_vkDevice, vkFence, E_CPU_ALLOCATOR);
        }
    }

    void VulkanDevice::DestroyVkFence(VulkanFence* pFence)
    {
        m_pFenceManager->DestoryFence(pFence);
    }
    void VulkanDevice::DestroyVkFences(VulkanFencePtrVector& aFence)
    {
        size_t count = aFence.size();
        for (size_t i = 0; i < count; i++)
        {
            VulkanFence* pFence = aFence[i];
            m_pFenceManager->DestoryFence(pFence);
        }
    }

    void VulkanDevice::RecoveryFence(VulkanFence* pFence)
    {
        m_pFenceManager->RecoveryFence(pFence);
    }
    void VulkanDevice::RecoveryFences(VulkanFencePtrVector& aFence)
    {
        size_t count = aFence.size();
        for (size_t i = 0; i < count; i++)
        {
            VulkanFence* pFence = aFence[i];
            m_pFenceManager->RecoveryFence(pFence);
        }
    }


    //////////////////// VkDescriptorPool ///////////////
    bool VulkanDevice::CreateVkDescriptorPool(uint32_t descriptorCount,    
                                              VkDescriptorPool& vkDescriptorPool)
    {
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, descriptorCount },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, descriptorCount },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, descriptorCount },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, descriptorCount },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, descriptorCount },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, descriptorCount }
        };

        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = descriptorCount * F_ARRAYSIZE(pool_sizes);
        poolInfo.poolSizeCount = (uint32_t)F_ARRAYSIZE(pool_sizes);
        poolInfo.pPoolSizes = pool_sizes;

        if (vkCreateDescriptorPool(this->m_vkDevice, &poolInfo, nullptr, &vkDescriptorPool) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkDescriptorPool: Failed to create descriptor pool, count pool: [%u] !", descriptorCount);
            return false;
        }

        F_LogInfo("VulkanDevice::CreateVkDescriptorPool: Success to create descriptor pool, count pool: [%u] !", descriptorCount);
        return true;
    }
    void VulkanDevice::DestroyVkDescriptorPool(const VkDescriptorPool& vkDescriptorPool)
    {
        if (vkDescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(this->m_vkDevice, vkDescriptorPool, nullptr);
        }
    }


    //////////////////// VkSwapchainKHR /////////////////
    bool VulkanDevice::CreateVkSwapchainKHR(VkSwapchainCreateInfoKHR& vkSwapChainCreateInfoKHR,
                                            VkSwapchainKHR& vkSwapChainKHR)
    {
        if (!E_CheckVkResult(vkCreateSwapchainKHR(this->m_vkDevice, &vkSwapChainCreateInfoKHR, nullptr, &vkSwapChainKHR), "vkCreateSwapchainKHR"))
        {
            F_LogError("*********************** VulkanDevice::CreateVkSwapchainKHR: Failed to create VkSwapchainKHR !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::GetVkSwapchainImagesKHR(const VkSwapchainKHR& vkSwapChainKHR,
                                               uint32& numSwapChainImages,
                                               VkImageVector* pVkImages)
    {
        if (!E_CheckVkResult(vkGetSwapchainImagesKHR(this->m_vkDevice, vkSwapChainKHR, &numSwapChainImages, (pVkImages != nullptr ? pVkImages->data() : nullptr)), "vkGetSwapchainImagesKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::GetVkSwapchainImagesKHR: vkGetSwapchainImagesKHR failed !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkSwapchainKHR(const VkSwapchainKHR& vkSwapChainKHR)
    {
        if (vkSwapChainKHR != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(this->m_vkDevice, vkSwapChainKHR, nullptr);
        }
    }

    VulkanSwapStatusType VulkanDevice::VkAcquireNextImageKHR(VkSwapchainKHR vkSwapChainKHR,
                                                             uint64_t timeout,
                                                             VkSemaphore vkSemaphore,
                                                             VkFence vkFence,
                                                             uint32_t* pImageIndex)
    {
        VkResult result = vkAcquireNextImageKHR(this->m_vkDevice, vkSwapChainKHR, timeout, vkSemaphore, vkFence, pImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            return Vulkan_SwapStatus_OutOfDate;
        }
        else if (result == VK_ERROR_SURFACE_LOST_KHR) 
        {
            return Vulkan_SwapStatus_Lost;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            F_LogError("*********************** VulkanDevice::VkAcquireNextImageKHR: Failed to call vkAcquireNextImageKHR, error: [%s] !", E_VkResult2String(result).c_str());
            throw std::runtime_error("*********************** VulkanDevice::VkAcquireNextImageKHR: Failed to acquire swap chain image index !");
            return Vulkan_SwapStatus_Error;
        }
        return Vulkan_SwapStatus_Normal;
    }

    VulkanSwapStatusType VulkanDevice::VkQueuePresentKHR(VkSwapchainKHR vkSwapChainKHR,
                                                         const VkSemaphoreVector& aWaitSemaphores,
                                                         uint32_t* pImageIndices)
    {
        VkSwapchainKHRVector aSwapChainKHR;
        aSwapChainKHR.push_back(vkSwapChainKHR);
        return VkQueuePresentKHR(aSwapChainKHR,
                                 aWaitSemaphores,
                                 pImageIndices);
    }
    VulkanSwapStatusType VulkanDevice::VkQueuePresentKHR(const VkSwapchainKHRVector& aSwapChainKHR,
                                                         const VkSemaphoreVector& aWaitSemaphores,
                                                         uint32_t* pImageIndices)
    {
        VkPresentInfoKHR presentInfo;
        E_ZeroStruct(presentInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
        presentInfo.waitSemaphoreCount = (uint32_t)aWaitSemaphores.size();
        presentInfo.pWaitSemaphores = aWaitSemaphores.data();
        presentInfo.swapchainCount = (uint32_t)aSwapChainKHR.size();
        presentInfo.pSwapchains = aSwapChainKHR.data();
        presentInfo.pImageIndices = pImageIndices;

        VkResult result = vkQueuePresentKHR(this->m_pQueuePresent->GetVkQueue(), &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            return Vulkan_SwapStatus_OutOfDate;
        }
        else if (result == VK_ERROR_SURFACE_LOST_KHR) 
        {
            return Vulkan_SwapStatus_Lost;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        {
            F_LogError("*********************** VulkanDevice::VkQueuePresentKHR: Failed to call vkQueuePresentKHR, error: [%s] !", E_VkResult2String(result).c_str());
            throw std::runtime_error("*********************** VulkanDevice::VkQueuePresentKHR: Failed to present swap chain image !");
            return Vulkan_SwapStatus_Error;
        }
        return Vulkan_SwapStatus_Normal;
    }

    //////////////////// VkViewport /////////////////////
    void VulkanDevice::CreateVkViewport(float width,
                                        float height,
                                        float offsetX,
                                        float offsetY,
                                        float minDepth,
                                        float maxDepth,
                                        VkViewport& vkViewport, 
                                        VkRect2D& vkScissor)
    {
        vkViewport  = {};
        vkViewport.x = offsetX;
        vkViewport.y = offsetY;
        vkViewport.width = width;
        vkViewport.height = height;
        vkViewport.minDepth = minDepth;
        vkViewport.maxDepth = maxDepth;

        vkScissor = {};
        VkOffset2D offset;
        offset.x = (int32_t)offsetX;
        offset.y = (int32_t)offsetY;
        VkExtent2D extent;
        extent.width = (uint32_t)width;
        extent.height = (uint32_t)height;
        vkScissor.offset = offset;
        vkScissor.extent = extent;
    }


    //////////////////// VkAttachmentDescription ////////
    void VulkanDevice::CreateVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                                     VkFormat format,
                                                     VkSampleCountFlagBits samples,
                                                     VkAttachmentLoadOp loadOp,
                                                     VkAttachmentStoreOp storeOp,
                                                     VkAttachmentLoadOp stencilLoadOp,
                                                     VkAttachmentStoreOp stencilStoreOp,
                                                     VkImageLayout initialLayout,
                                                     VkImageLayout finalLayout,
                                                     VkAttachmentDescription& vkAttachmentDescription)
    {
        vkAttachmentDescription.flags = flags;
        vkAttachmentDescription.format = format;
        vkAttachmentDescription.samples = samples;
        vkAttachmentDescription.loadOp = loadOp;
        vkAttachmentDescription.storeOp = storeOp;
        vkAttachmentDescription.stencilLoadOp = stencilLoadOp;
        vkAttachmentDescription.stencilStoreOp = stencilStoreOp;
        vkAttachmentDescription.initialLayout = initialLayout;
        vkAttachmentDescription.finalLayout = finalLayout;
    }


    //////////////////// VkRenderPass ///////////////////
    bool VulkanDevice::CreateVkRenderPass(const String& nameRenderPass,
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
        {
            renderPassInfo.pNext = pMultiviewCI;
        }

        if (vkCreateRenderPass(this->m_vkDevice, &renderPassInfo, nullptr, &vkRenderPass) != VK_SUCCESS)
        {
            F_LogError("*********************** VulkanDevice::CreateVkRenderPass: vkCreateRenderPass failed: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateVkRenderPass: vkCreateRenderPass success: [%s] !", nameRenderPass.c_str());
        return true;
    }
    void VulkanDevice::DestroyVkRenderPass(const VkRenderPass& vkRenderPass)
    {
        if (vkRenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(this->m_vkDevice, vkRenderPass, nullptr);   
        }
    }

    bool VulkanDevice::CreateRenderPass_KhrDepth(const String& nameRenderPass,
                                                 VkFormat formatSwapChain, 
                                                 VkFormat formatDepth, 
                                                 VkRenderPass& vkRenderPass)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatSwapChain,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                      attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        CreateVkAttachmentDescription(0,
                                      formatDepth,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Depth);
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

        //5> CreateVkRenderPass
        if (!CreateVkRenderPass(nameRenderPass,
                                aAttachmentDescription,
                                aSubpassDescription,
                                aSubpassDependency,
                                nullptr,
                                vkRenderPass))
        {
            F_LogError("*********************** VulkanDevice::CreateRenderPass_KhrDepth: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateRenderPass_KhrDepth: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return true;
    }
    bool VulkanDevice::CreateRenderPass_KhrDepthImgui(const String& nameRenderPass,
                                                      VkFormat formatColor, 
                                                      VkFormat formatDepth, 
                                                      VkFormat formatSwapChain, 
                                                      VkRenderPass& vkRenderPass)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatColor,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        CreateVkAttachmentDescription(0,
                                      formatDepth,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Depth);
        aAttachmentDescription.push_back(attachmentSR_Depth);
            
        //3> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatSwapChain,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_LOAD,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                      attachmentImgui_Color);
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

        //8> CreateVkRenderPass
        if (!CreateVkRenderPass(nameRenderPass,
                                aAttachmentDescription,
                                aSubpassDescription,
                                aSubpassDependency,
                                nullptr,
                                vkRenderPass))
        {
            F_LogError("*********************** VulkanDevice::CreateRenderPass_KhrDepthImgui: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateRenderPass_KhrDepthImgui: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return true;
    }
    bool VulkanDevice::CreateRenderPass_ColorDepthMSAA(const String& nameRenderPass,
                                                       VkFormat formatColor, 
                                                       VkFormat formatDepth, 
                                                       VkFormat formatSwapChain, 
                                                       VkSampleCountFlagBits samples, 
                                                       VkRenderPass& vkRenderPass)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatColor,
                                      samples,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        CreateVkAttachmentDescription(0,
                                      formatDepth,
                                      samples,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Depth);
        aAttachmentDescription.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        CreateVkAttachmentDescription(0,
                                      formatSwapChain,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                      attachmentSR_ColorResolve);
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

        //6> CreateVkRenderPass
        if (!CreateVkRenderPass(nameRenderPass,
                                aAttachmentDescription,
                                aSubpassDescription,
                                aSubpassDependency,
                                nullptr,
                                vkRenderPass))
        {
            F_LogError("*********************** VulkanDevice::CreateRenderPass_ColorDepthMSAA: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateRenderPass_ColorDepthMSAA: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return true;
    }
    bool VulkanDevice::CreateRenderPass_ColorDepthImguiMSAA(const String& nameRenderPass,
                                                            VkFormat formatColor, 
                                                            VkFormat formatDepth, 
                                                            VkFormat formatSwapChain, 
                                                            VkSampleCountFlagBits samples, 
                                                            VkRenderPass& vkRenderPass)
    {
        std::vector<VkAttachmentDescription> aAttachmentDescription;
        std::vector<VkSubpassDescription> aSubpassDescription;
        std::vector<VkSubpassDependency> aSubpassDependency;

        //1> Attachment SceneRender Color
        VkAttachmentDescription attachmentSR_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatColor,
                                      samples,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Color);
        aAttachmentDescription.push_back(attachmentSR_Color);
        
        //2> Attachment SceneRender Depth
        VkAttachmentDescription attachmentSR_Depth = {};
        CreateVkAttachmentDescription(0,
                                      formatDepth,
                                      samples,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_CLEAR,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                      attachmentSR_Depth);
        aAttachmentDescription.push_back(attachmentSR_Depth);
        
        //3> Attachment SceneRender Color Resolve
        VkAttachmentDescription attachmentSR_ColorResolve = {};
        CreateVkAttachmentDescription(0,
                                      formatSwapChain,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_UNDEFINED,
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                      attachmentSR_ColorResolve);
        aAttachmentDescription.push_back(attachmentSR_ColorResolve);
        
        //4> Attachment Imgui Color
        VkAttachmentDescription attachmentImgui_Color = {};
        CreateVkAttachmentDescription(0,
                                      formatSwapChain,
                                      VK_SAMPLE_COUNT_1_BIT,
                                      VK_ATTACHMENT_LOAD_OP_LOAD,
                                      VK_ATTACHMENT_STORE_OP_STORE,
                                      VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                                      VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                      VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                      attachmentImgui_Color);
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

        //9> CreateVkRenderPass
        if (!CreateVkRenderPass(nameRenderPass,
                                aAttachmentDescription,
                                aSubpassDescription,
                                aSubpassDependency,
                                nullptr,
                                vkRenderPass))
        {
            F_LogError("*********************** VulkanDevice::CreateRenderPass_ColorDepthImguiMSAA: Failed to create VkRenderPass: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateRenderPass_ColorDepthImguiMSAA: Success to create VkRenderPass: [%s] !", nameRenderPass.c_str());
        return true;
    }


    //////////////////// VkFramebuffer //////////////////
    bool VulkanDevice::CreateVkFramebuffer(const String& nameFramebuffer,
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

        if (vkCreateFramebuffer(this->m_vkDevice, &framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkFramebuffer: Failed to create VkFramebuffer: [%s] !", nameFramebuffer.c_str());
            return false;
        }

        F_LogInfo("VulkanDevice::CreateVkFramebuffer: Success to create VkFramebuffer: [%s] !", nameFramebuffer.c_str());
        return true;
    }
    void VulkanDevice::DestroyVkFramebuffer(const VkFramebuffer& vkFramebuffer)
    {
        if (vkFramebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(this->m_vkDevice, vkFramebuffer, nullptr);
        }
    }


    //////////////////// VkBuffer ///////////////////////
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
        if (!E_CheckVkResult(vkCreateBuffer(this->m_vkDevice, &bufferInfo, E_CPU_ALLOCATOR, &vkBuffer), "vkCreateBuffer")) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkBuffer: Failed to create VkBuffer !");
            return false;
        }

        VkMemoryRequirements memRequirements = { };
        vkGetBufferMemoryRequirements(this->m_vkDevice, vkBuffer, &memRequirements);
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(this->m_vkDevice, &allocInfo, E_CPU_ALLOCATOR, &vkBufferMemory) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkBuffer: Failed to allocate VkDeviceMemory !");
            return false;
        }
        vkBindBufferMemory(this->m_vkDevice, vkBuffer, vkBufferMemory, 0);

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
            vkDestroyBuffer(this->m_vkDevice, vkBuffer, E_CPU_ALLOCATOR);
            vkFreeMemory(this->m_vkDevice, vkBufferMemory, E_CPU_ALLOCATOR);
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
        if (!E_CheckVkResult(vkMapMemory(this->m_vkDevice, vkBufferMemory, nDataOffset, nDataSize, flags, ppData), "vkMapMemory")) 
        {
            F_LogError("*********************** VulkanDevice::MapVkDeviceMemory: Failed to call vkMapMemory !");
            return false;
        }

        return true;
    }
    void VulkanDevice::UnmapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory)
    {
        vkUnmapMemory(this->m_vkDevice, vkBufferMemory);
    }


    //////////////////// VkImage ////////////////////////
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
                                         VkComponentMapping components,
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
        viewInfo.components = components;
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
    void VulkanDevice::DestroyVkImage(const VkImage& vkImage, const VkDeviceMemory& vkImageMemory, const VkImageView& vkImageView)
    {
        if (vkImage != VK_NULL_HANDLE)
        {
            vkDestroyImage(this->m_vkDevice, vkImage, nullptr);
            vkFreeMemory(this->m_vkDevice, vkImageMemory, nullptr);
        }
        DestroyVkImageView(vkImageView);
    }
    void VulkanDevice::DestroyVkImageView(const VkImageView& vkImageView)
    {   
        if (vkImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(this->m_vkDevice, vkImageView, nullptr);
        }
    }
    void VulkanDevice::DestroyVkSampler(const VkSampler& vkSampler)
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
                      (uint32_t)imageSize, 
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
        uint32_t numArray = (uint32_t)count_tex;
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
                          (uint32_t)imageSizeAll,
                          (uint32_t)(width * height * pixelSize * i));
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
                      (uint32_t)imageSize, 
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
        uint32_t numArray = (uint32_t)count_tex;

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
                          (uint32_t)imageSizeAll,
                          (uint32_t)(width * height * pixelSize * i));
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
                for (uint32_t i = 0; i < width * height; i++)
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
                          (uint32_t)imageSize, 
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
                for (uint32_t i = 0; i < width * height * numArray; i++)
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
                for (uint32_t i = 0; i < width * height * depth; i++)
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


    //////////////////// VkShaderModule /////////////////
    bool VulkanDevice::CreateVkShaderModule(FShaderType typeShader, 
                                            const String& pathFile,
                                            VkShaderModule& vkShaderModule)
    {
        const String& strTypeShader = F_GetShaderTypeName(typeShader);
        return CreateVkShaderModule(strTypeShader, 
                                    pathFile,
                                    vkShaderModule);
    }
    bool VulkanDevice::CreateVkShaderModule(const String& strTypeShader, 
                                            const String& pathFile,
                                            VkShaderModule& vkShaderModule)
    {
        if (pathFile.empty())
        {
            F_LogError("*********************** VulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, path file is empty !");
            return false;
        }

        CharVector code;
        if (!FUtil::LoadAssetFileContent(pathFile.c_str(), code))
        {
            F_LogError("*********************** VulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, path file: [%s] !", pathFile.c_str());
            return false;
        }
        if (code.size() <= 0)
        {
            F_LogError("*********************** VulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, code size <= 0, path file: [%s] !", pathFile.c_str());
            return false;
        }

        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(this->m_vkDevice, &createInfo, nullptr, &vkShaderModule) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule: shader type: [%s], path file: [%s] !", strTypeShader.c_str(), pathFile.c_str());
            return false;
        }

        return true;
    }
    void VulkanDevice::DestroyVkShaderModule(const VkShaderModule& vkShaderModule)
    {
        if (vkShaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(this->m_vkDevice, vkShaderModule, nullptr);
        }
    }


    //////////////////// VkDescriptorSetLayout //////////
    bool VulkanDevice::CreateVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, 
                                                   VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(aDescriptorSetLayoutBinding.size());
        layoutInfo.pBindings = aDescriptorSetLayoutBinding.data();
        if (vkCreateDescriptorSetLayout(this->m_vkDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkDescriptorSetLayout: Failed to create VkDescriptorSetLayout !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkDescriptorSetLayout(const VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        if (vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorSetLayout(this->m_vkDevice, vkDescriptorSetLayout, nullptr);
        }
    }


    //////////////////// VkPipelineLayout ///////////////
    bool VulkanDevice::CreateVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout,
                                              VkPipelineLayout& vkPipelineLayout)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(aDescriptorSetLayout.size());
        pipelineLayoutInfo.pSetLayouts = aDescriptorSetLayout.data();

        if (vkCreatePipelineLayout(this->m_vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkPipelineLayout: Failed to create VkPipelineLayout !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkPipelineLayout(const VkPipelineLayout& vkPipelineLayout)
    {
        if (vkPipelineLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(this->m_vkDevice, vkPipelineLayout, nullptr);
        }
    }


    //////////////////// VkPipelineCache ////////////////
    bool VulkanDevice::CreateVkPipelineCache(VkPipelineCache& vkPipelineCache)
    {
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        if (vkCreatePipelineCache(this->m_vkDevice, &pipelineCacheCreateInfo, nullptr, &vkPipelineCache) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkPipelineCache: Failed to create VkPipelineCache !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkPipelineCache(const VkPipelineCache& vkPipelineCache)
    {
        if (vkPipelineCache != VK_NULL_HANDLE)
        {
            vkDestroyPipelineCache(this->m_vkDevice, vkPipelineCache, nullptr);
        }
    }


    //////////////////// VkPipeline /////////////////////
    bool VulkanDevice::CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
                                                 VkShaderModule fragShaderModule, const String& fragMain,
                                                 VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                 VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                 VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                 VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                                 VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                 VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                 VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                 VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                 VkColorComponentFlags colorWriteMask,
                                                 VkSampleCountFlagBits msaaSamples,
                                                 VkPipelineCache vkPipelineCache,
                                                 VkPipeline& vkPipeline)
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

        return CreateVkPipeline_Graphics(aShaderStageCreateInfos,
                                         false, 0, 0,
                                         pBindingDescriptions,
                                         pAttributeDescriptions,
                                         renderPass, pipelineLayout, aViewports, aScissors,
                                         primitiveTopology, frontFace, polygonMode, cullMode, lineWidth,
                                         bDepthTest, bDepthWrite, depthCompareOp,
                                         bStencilTest, stencilOpFront, stencilOpBack,
                                         bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                         blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                         colorWriteMask,
                                         msaaSamples,
                                         vkPipelineCache,
                                         vkPipeline);
    }
    bool VulkanDevice::CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
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
                                                 VkColorComponentFlags colorWriteMask,
                                                 VkSampleCountFlagBits msaaSamples,
                                                 VkPipelineCache vkPipelineCache,
                                                 VkPipeline& vkPipeline)
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

        return CreateVkPipeline_Graphics(aShaderStageCreateInfos,
                                         true, tessellationFlags, tessellationPatchControlPoints,
                                         pBindingDescriptions,
                                         pAttributeDescriptions,
                                         renderPass, pipelineLayout, aViewports, aScissors,
                                         primitiveTopology, frontFace, polygonMode, cullMode, lineWidth,
                                         bDepthTest, bDepthWrite, depthCompareOp,
                                         bStencilTest, stencilOpFront, stencilOpBack,
                                         bBlend, blendColorFactorSrc, blendColorFactorDst, blendColorOp,
                                         blendAlphaFactorSrc, blendAlphaFactorDst, blendAlphaOp,
                                         colorWriteMask,
                                         msaaSamples,
                                         vkPipelineCache,
                                         vkPipeline);
    }
    bool VulkanDevice::CreateVkPipeline_Graphics(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
                                                 bool tessellationIsUsed, VkPipelineTessellationStateCreateFlags tessellationFlags, uint32_t tessellationPatchControlPoints,
                                                 VkVertexInputBindingDescriptionVector* pBindingDescriptions,
                                                 VkVertexInputAttributeDescriptionVector* pAttributeDescriptions,
                                                 VkRenderPass renderPass, VkPipelineLayout pipelineLayout, const VkViewportVector& aViewports, const VkRect2DVector& aScissors,
                                                 VkPrimitiveTopology primitiveTopology, VkFrontFace frontFace, VkPolygonMode polygonMode, VkCullModeFlagBits cullMode, float lineWidth,
                                                 VkBool32 bDepthTest, VkBool32 bDepthWrite, VkCompareOp depthCompareOp, 
                                                 VkBool32 bStencilTest, const VkStencilOpState& stencilOpFront, const VkStencilOpState& stencilOpBack, 
                                                 VkBool32 bBlend, VkBlendFactor blendColorFactorSrc, VkBlendFactor blendColorFactorDst, VkBlendOp blendColorOp,
                                                 VkBlendFactor blendAlphaFactorSrc, VkBlendFactor blendAlphaFactorDst, VkBlendOp blendAlphaOp,
                                                 VkColorComponentFlags colorWriteMask,
                                                 VkSampleCountFlagBits msaaSamples,
                                                 VkPipelineCache vkPipelineCache,
                                                 VkPipeline& vkPipeline)
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
        rasterizationStateInfo.cullMode = cullMode;
        rasterizationStateInfo.frontFace = frontFace;
        rasterizationStateInfo.depthBiasEnable = VK_FALSE;
        rasterizationStateInfo.depthBiasConstantFactor = 0.0f; // Optional
        rasterizationStateInfo.depthBiasClamp = 0.0f; // Optional
        rasterizationStateInfo.depthBiasSlopeFactor = 0.0f; // Optional
        rasterizationStateInfo.lineWidth = lineWidth;

        //5> Pipeline Multisample State
        VkPipelineMultisampleStateCreateInfo multisamplingStateInfo = {};
        multisamplingStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisamplingStateInfo.sampleShadingEnable = VK_FALSE;
        multisamplingStateInfo.rasterizationSamples = msaaSamples;
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

        if (!E_CheckVkResult(vkCreateGraphicsPipelines(this->m_vkDevice, vkPipelineCache, 1, &pipelineInfo, nullptr, &vkPipeline), "vkCreateGraphicsPipelines"))
        {
            F_LogError("*********************** VulkanDevice::CreateVkPipeline_Graphics: Failed to create VkPipeline !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::CreateVkPipeline_Compute(VkShaderModule compShaderModule,
                                                const String& compMain,
                                                VkPipelineLayout pipelineLayout, 
                                                VkPipelineCreateFlags flags,
                                                VkPipelineCache vkPipelineCache,
                                                VkPipeline& vkPipeline)
    {
        VkPipelineShaderStageCreateInfo compShaderStageInfo = {};
        compShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        compShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
        compShaderStageInfo.module = compShaderModule;
        compShaderStageInfo.pName = compMain.c_str();

        return CreateVkPipeline_Compute(compShaderStageInfo,
                                        pipelineLayout,
                                        flags,
                                        vkPipelineCache,
                                        vkPipeline);
    }
    bool VulkanDevice::CreateVkPipeline_Compute(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
                                                VkPipelineLayout pipelineLayout, 
                                                VkPipelineCreateFlags flags,
                                                VkPipelineCache vkPipelineCache,
                                                VkPipeline& vkPipeline)
    {
        VkComputePipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
        pipelineInfo.pNext = nullptr;
        pipelineInfo.flags = flags; 
        pipelineInfo.stage = shaderStageCreateInfo;
        pipelineInfo.layout = pipelineLayout;
        
        if (!E_CheckVkResult(vkCreateComputePipelines(this->m_vkDevice, vkPipelineCache, 1, &pipelineInfo, nullptr, &vkPipeline), "vkCreateComputePipelines"))
        {
            F_LogError("*********************** VulkanDevice::CreateVkPipeline_Compute: Failed to create VkPipeline !");
            return false;
        }
        return true;
    }
    void VulkanDevice::DestroyVkPipeline(const VkPipeline& vkPipeline)
    {
        if (vkPipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(this->m_vkDevice, vkPipeline, nullptr);   
        }
    }


    //////////////////// VkDescriptorSet ////////////////
    bool VulkanDevice::CreateVkDescriptorSet(uint32_t descriptorSetCount,
                                             VkDescriptorSetLayout vkDescriptorSetLayout, 
                                             VkDescriptorPool vkDescriptorPool,
                                             VkDescriptorSet& vkDescriptorSet)
    {
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = vkDescriptorPool;
        allocInfo.descriptorSetCount = descriptorSetCount;
        allocInfo.pSetLayouts = &vkDescriptorSetLayout;

        if (vkAllocateDescriptorSets(this->m_vkDevice, &allocInfo, &vkDescriptorSet) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkDescriptorSet: Failed to allocate VkDescriptorSet !");
            return false;
        }
        return true;
    }
    bool VulkanDevice::CreateVkDescriptorSets(uint32_t countSwapChain, 
                                              VkDescriptorSetLayout vkDescriptorSetLayout,
                                              VkDescriptorPool vkDescriptorPool,
                                              VkDescriptorSetVector& aDescriptorSets)
    {
        VkDescriptorSetLayoutVector layouts(countSwapChain, vkDescriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = vkDescriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(countSwapChain);
        allocInfo.pSetLayouts = layouts.data();

        aDescriptorSets.resize(countSwapChain);
        if (vkAllocateDescriptorSets(this->m_vkDevice, &allocInfo, aDescriptorSets.data()) != VK_SUCCESS) 
        {
            F_LogError("*********************** VulkanDevice::CreateVkDescriptorSets: Failed to allocate VkDescriptorSets !");
            return false;
        }
        return true;
    }

    VkDescriptorSetLayoutBinding VulkanDevice::CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
                                                                                          VkDescriptorType descriptorType,
                                                                                          uint32_t descriptorCount,
                                                                                          VkShaderStageFlags stageFlags)
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
        CreateVkDescriptorSetLayoutBinding_Uniform(binding,
                                                   descriptorType,
                                                   descriptorCount,
                                                   stageFlags,
                                                   descriptorSetLayoutBinding);
        return descriptorSetLayoutBinding;
    }
    void VulkanDevice::CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
                                                                  VkDescriptorType descriptorType,
                                                                  uint32_t descriptorCount,
                                                                  VkShaderStageFlags stageFlags,
                                                                  VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding)
    {
        descriptorSetLayoutBinding.binding = binding;
        descriptorSetLayoutBinding.descriptorType = descriptorType;
        descriptorSetLayoutBinding.descriptorCount = descriptorCount;
        descriptorSetLayoutBinding.stageFlags = stageFlags;
        descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
    }
    VkDescriptorSetLayoutBinding VulkanDevice::CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                                                        VkDescriptorType descriptorType,
                                                                                        uint32_t descriptorCount,
                                                                                        VkShaderStageFlags stageFlags,
                                                                                        VkSampler* pImmutableSamplers)
    {
        VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
        CreateVkDescriptorSetLayoutBinding_Image(binding,
                                                 descriptorType,
                                                 descriptorCount,
                                                 stageFlags,
                                                 pImmutableSamplers,
                                                 descriptorSetLayoutBinding);
        return descriptorSetLayoutBinding;
    }
    void VulkanDevice::CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
                                                                VkDescriptorType descriptorType,
                                                                uint32_t descriptorCount,
                                                                VkShaderStageFlags stageFlags,
                                                                VkSampler* pImmutableSamplers,
                                                                VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding)
    {
        descriptorSetLayoutBinding.binding = binding;
        descriptorSetLayoutBinding.descriptorType = descriptorType;
        descriptorSetLayoutBinding.descriptorCount = descriptorCount;
        descriptorSetLayoutBinding.stageFlags = stageFlags;
        descriptorSetLayoutBinding.pImmutableSamplers = pImmutableSamplers;
    }

    void VulkanDevice::PushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
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
    void VulkanDevice::PushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
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
    void VulkanDevice::UpdateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets)
    {
        if (aWriteDescriptorSets.size() > 0)
        {
            vkUpdateDescriptorSets(this->m_vkDevice, static_cast<uint32_t>(aWriteDescriptorSets.size()), aWriteDescriptorSets.data(), 0, nullptr);
        }
    }


    //////////////////// VkCommandBuffer ////////////////
    VkCommandBuffer VulkanDevice::BeginSingleTimeCommands()
    {
        VkCommandBuffer vkCommandBuffer = AllocateVkCommandBuffer(m_vkCommandPoolTransfer, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        BeginVkCommandBuffer(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        return vkCommandBuffer;
    }
    void VulkanDevice::EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer)
    {
        EndVkCommandBuffer(vkCommandBuffer);
        QueueSubmitVkCommandBuffers(m_pQueueTransfer->GetVkQueue(), 
                                    1, 
                                    &vkCommandBuffer, 
                                    0,
                                    nullptr,
                                    nullptr);
        QueueWaitIdle(m_pQueueTransfer->GetVkQueue());
        FreeVkCommandBuffers(m_vkCommandPoolTransfer, 1, &vkCommandBuffer);
    }


    void VulkanDevice::BeginRenderPass(VkCommandBuffer& commandBuffer, 
                                       const VkRenderPass& renderPass, 
                                       const VkFramebuffer& frameBuffer,
                                       const VkOffset2D& offset,
                                       const VkExtent2D& extent,
                                       const FVector4& clBg,
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
        void VulkanDevice::BindViewport(VkCommandBuffer& commandBuffer, const VkViewport& vkViewport, const VkRect2D& scissor, bool isNegativeViewport /*= true*/)
        {
            VkViewport viewport = vkViewport;
            if (isNegativeViewport)
            {
                viewport.y = viewport.height - viewport.y;
                viewport.height = -viewport.height;
            }   
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
        }
        void VulkanDevice::BindPipeline(VkCommandBuffer& commandBuffer, VkPipelineBindPoint pipelineBindPoint, const VkPipeline& vkPipeline)
        {
            vkCmdBindPipeline(commandBuffer, pipelineBindPoint, vkPipeline);
        }
        void VulkanDevice::BindVertexBuffer(VkCommandBuffer& commandBuffer, uint32_t firstBinding, uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets)
        {
            vkCmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
        }
        void VulkanDevice::BindIndexBuffer(VkCommandBuffer& commandBuffer, const VkBuffer& vkIndexBuffer, VkDeviceSize offset, VkIndexType indexType)
        {
            vkCmdBindIndexBuffer(commandBuffer, vkIndexBuffer, offset, indexType);
        }
        void VulkanDevice::BindDescriptorSets(VkCommandBuffer& commandBuffer, const VkPipelineBindPoint& pipelineBindPoint, const VkPipelineLayout& layout, uint32_t firstSet, uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount, const uint32_t* pDynamicOffsets)
        {
            vkCmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount, pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
        }
        void VulkanDevice::Draw(VkCommandBuffer& commandBuffer, uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
        {
            vkCmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
        }
        void VulkanDevice::DrawIndexed(VkCommandBuffer& commandBuffer, uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance)
        {
            vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
        }
        void VulkanDevice::DrawIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
        {
            vkCmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
        }
        void VulkanDevice::DrawIndexedIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer, VkDeviceSize offset, uint32_t drawCount, uint32_t stride)
        {
            vkCmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
        }
    void VulkanDevice::EndRenderPass(VkCommandBuffer& commandBuffer)
    {
        vkCmdEndRenderPass(commandBuffer);
    }

    void VulkanDevice::Dispatch(VkCommandBuffer& commandBuffer, uint32_t groupCountX,  uint32_t groupCountY,  uint32_t groupCountZ)
    {
        vkCmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
    }
    void VulkanDevice::DispatchIndirect(VkCommandBuffer& commandBuffer, const VkBuffer& buffer,  VkDeviceSize offset)
    {
        vkCmdDispatchIndirect(commandBuffer, buffer, offset);
    }

}; //LostPeterPluginRendererVulkan