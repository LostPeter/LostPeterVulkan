/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
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
#include "../include/VulkanPixelFormat.h"

namespace LostPeter
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
        assert(m_vkPhysicalDevice != VK_NULL_HANDLE && "VulkanDevice::VulkanDevice");
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
    bool VulkanDevice::IsPixelFormatSupported(VulkanPixelFormatType format)
    {
        return VulkanPixelFormat::GetPixelFormatDes(format).isSupported;
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
        Util_ZeroStruct(deviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
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
            Util_ZeroStruct(queueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
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
        int count = (int)Vulkan_PixelFormat_Count;
        for (int i = 1; i < count; i++)
        {
            VulkanPixelFormatType format = (VulkanPixelFormatType)i;
            VulkanPixelFormatDes& des = VulkanPixelFormat::GetPixelFormatDesRef(format);
            des.isSupported = IsPixelFormatSupported(Util_Transform2VkFormat(format));
            F_LogInfo("VulkanDevice::createDevice: [%d]: [%s] is supported [%s]", i, des.name.c_str(), des.isSupported ? "true" : "false");
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
        if (!Util_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_vkPhysicalDevice, familyIndex, surface, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
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
    //VkSemaphore
    VkSemaphore VulkanDevice::CreateVkSemaphore()
    {
        VkSemaphore vkSemaphore;
        CreateVkSemaphore(vkSemaphore);
        return vkSemaphore;
    }
    void VulkanDevice::CreateVkSemaphore(VkSemaphore& vkSemaphore)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        Util_ZeroStruct(semaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        if (!Util_CheckVkResult(vkCreateSemaphore(m_vkDevice, &semaphoreCreateInfo, UTIL_CPU_ALLOCATOR, &vkSemaphore), "vkCreateSemaphore")) 
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
            vkDestroySemaphore(m_vkDevice, vkSemaphore, UTIL_CPU_ALLOCATOR);
        }
    }


    //VkFence
    VkFence VulkanDevice::CreateVkFence(bool isCreateSignaled)
    {
        VkFence vkFence;
        CreateVkFence(isCreateSignaled, vkFence);
        return vkFence;
    }
    void VulkanDevice::CreateVkFence(bool isCreateSignaled, VkFence& vkFence)
    {
        VkFenceCreateInfo fenceCreateInfo;
        Util_ZeroStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        fenceCreateInfo.flags = isCreateSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        if (!Util_CheckVkResult(vkCreateFence(m_vkDevice, &fenceCreateInfo, UTIL_CPU_ALLOCATOR, &vkFence), "vkCreateFence")) 
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
            vkDestroyFence(m_vkDevice, vkFence, UTIL_CPU_ALLOCATOR);
        }
    }


    //VkCommandPool
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
        if (!Util_CheckVkResult(vkCreateCommandPool(m_vkDevice, &commandPoolInfo, UTIL_CPU_ALLOCATOR, &vkCommandPool), "vkCreateCommandPool")) 
        {
            String msg = "*********************** VulkanWindow::CreateVkCommandPool: Failed to create command pool !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::DestroyVkCommandPool(const VkCommandPool& vkCommandPool)
    {   
        if (vkCommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(m_vkDevice, vkCommandPool, UTIL_CPU_ALLOCATOR);
        }
    }


    //VkCommandBuffer
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
        if (!Util_CheckVkResult(vkAllocateCommandBuffers(m_vkDevice, &allocInfo, pCommandBuffers), "vkAllocateCommandBuffers")) 
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
        if (!Util_CheckVkResult(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo), "vkBeginCommandBuffer")) 
        {
            String msg = "*********************** VulkanDevice::BeginVkCommandBuffer: Failed to call vkBeginCommandBuffer !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer)
    {
        if (!Util_CheckVkResult(vkEndCommandBuffer(vkCommandBuffer), "vkEndCommandBuffer")) 
        {
            String msg = "*********************** VulkanDevice::EndVkCommandBuffer: Failed to call vkEndCommandBuffer !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }


    //VkQueue
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
        if (!Util_CheckVkResult(vkQueueSubmit(vkQueue, commandBufferCount, &submitInfo, vkFence), "vkQueueSubmit")) 
        {
            String msg = "*********************** VulkanDevice::QueueSubmitVkCommandBuffers: Failed to call vkQueueSubmit !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }
    void VulkanDevice::QueueWaitIdle(const VkQueue& vkQueue)
    {
        if (!Util_CheckVkResult(vkQueueWaitIdle(vkQueue), "vkQueueWaitIdle")) 
        {
            String msg = "*********************** VulkanDevice::QueueWaitIdle: Failed to call vkQueueWaitIdle !";
            F_LogError(msg.c_str());
            throw std::runtime_error(msg);
        }
    }


    //VkBuffer
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
        if (!Util_CheckVkResult(vkCreateBuffer(m_vkDevice, &bufferInfo, UTIL_CPU_ALLOCATOR, &vkBuffer), "vkCreateBuffer")) 
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
        if (vkAllocateMemory(m_vkDevice, &allocInfo, UTIL_CPU_ALLOCATOR, &vkBufferMemory) != VK_SUCCESS) 
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
        WriteVkBuffer(vkBufferMemoryTransfer, pData, bufSize, 0);

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
        WriteVkBuffer(vkBufferMemoryTransfer, pData, bufSize, 0);
        
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
        VkBufferCopy copyRegion = {};
        copyRegion.size = size;
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
            vkDestroyBuffer(m_vkDevice, vkBuffer, UTIL_CPU_ALLOCATOR);
            vkFreeMemory(m_vkDevice, vkBufferMemory, UTIL_CPU_ALLOCATOR);
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
        if (!Util_CheckVkResult(vkMapMemory(m_vkDevice, vkBufferMemory, nDataOffset, nDataSize, flags, ppData), "vkMapMemory")) 
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

}; //LostPeter