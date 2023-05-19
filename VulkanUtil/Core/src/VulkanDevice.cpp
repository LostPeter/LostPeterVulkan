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

#include "../include/PreInclude.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanQueue.h"
#include "../include/VulkanFenceManager.h"
#include "../include/VulkanDeviceMemoryManager.h"
#include "../include/VulkanPixelFormat.h"

namespace LostPeter
{
    VulkanDevice::VulkanDevice(VkPhysicalDevice vkPhysicalDevice)
        : m_vkDevice(VK_NULL_HANDLE)
        , m_vkPhysicalDevice(vkPhysicalDevice)
        , m_pVkPhysicalDeviceFeatures2(nullptr)
        , m_vkMaxMSAASamples(VK_SAMPLE_COUNT_1_BIT)
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
        UTIL_DELETE(m_pQueueTransfer)
        UTIL_DELETE(m_pQueueCompute)
        UTIL_DELETE(m_pQueueGraphics)

        UTIL_DELETE(m_pFenceManager)
        UTIL_DELETE(m_pDeviceMemoryManager)

        if (m_vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_vkDevice, nullptr);
        }
        m_vkDevice = VK_NULL_HANDLE;
    }

    bool VulkanDevice::Init(int32 deviceIndex, bool bIsEnableValidationLayers)
    {
        vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);
        Util_LogInfo("VulkanDevice::Init: Using device: [%d], Support Geometry Shader: [%d], Support Tessellation Shader: [%d]", 
                    deviceIndex, m_vkPhysicalDeviceFeatures.geometryShader, m_vkPhysicalDeviceFeatures.tessellationShader);

        //1> createDevice
        if (!createDevice(bIsEnableValidationLayers))
        {
            Util_LogError("*********************** VulkanDevice::Init: 1> createDevice failed !");
            return false;
        }
        Util_LogInfo("VulkanDevice::Init: 1> createDevice success !");
        
        //2> checkPixelFormats
        if (!checkPixelFormats())
        {
            Util_LogError("*********************** VulkanDevice::Init: 2> checkPixelFormats failed !");
            return false;
        }
        Util_LogInfo("VulkanDevice::Init: 2> checkPixelFormats success !");
       
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

        Util_LogInfo("VulkanDevice::QueryGPU: Device index: [%d], name: [%s]", deviceIndex, m_vkPhysicalDeviceProperties.deviceName);
        Util_LogInfo("VulkanDevice::QueryGPU: API: [%d.%d.%d(0x%x)], Driver: [0x%x VendorId 0x%x]", VK_VERSION_MAJOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_MINOR(m_vkPhysicalDeviceProperties.apiVersion), VK_VERSION_PATCH(m_vkPhysicalDeviceProperties.apiVersion), m_vkPhysicalDeviceProperties.apiVersion, m_vkPhysicalDeviceProperties.driverVersion, m_vkPhysicalDeviceProperties.vendorID);
        Util_LogInfo("VulkanDevice::QueryGPU: DeviceID: [0x%x], Type: [%s]", m_vkPhysicalDeviceProperties.deviceID, deviceType.c_str());
        Util_LogInfo("VulkanDevice::QueryGPU: Max Descriptor Sets Bound: [%d], Timestamps: [%d]", m_vkPhysicalDeviceProperties.limits.maxBoundDescriptorSets, m_vkPhysicalDeviceProperties.limits.timestampComputeAndGraphics);
        Util_LogInfo("VulkanDevice::QueryGPU: Max MSAA Samples: [%d]", (int32)m_vkMaxMSAASamples);

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
            Util_LogInfo("VulkanDevice::createDevice: Using app device extensions count: %d", (int32)countAppDeviceExtensions);
            for (int32 i = 0; i < countAppDeviceExtensions; ++i)
            {
                deviceExtensions.push_back(m_aAppDeviceExtensions[i]);
                Util_LogInfo("VulkanDevice::createDevice: Using app device extension: %s", m_aAppDeviceExtensions[i]);
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
        Util_LogInfo("VulkanDevice::createDevice: Queue Families count: %d", countQueueFamilyProperties);

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
                Util_LogInfo("VulkanDevice::createDevice: Skipping unnecessary Queue Family: [%d : %d], Queues: [%s]", i, currProps.queueCount, getQueueType(currProps).c_str());
                continue;
            }

            VkDeviceQueueCreateInfo queueCreateInfo;
            Util_ZeroStruct(queueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
            queueCreateInfo.queueFamilyIndex = i;
            queueCreateInfo.queueCount = currProps.queueCount;
            numPriorities += currProps.queueCount;
            queueFamilyInfos.push_back(queueCreateInfo);
            
            Util_LogInfo("VulkanDevice::createDevice: Initializing Queue Family: [%d: %d], Queues: [%s]", i,  currProps.queueCount, getQueueType(currProps).c_str());
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
            Util_LogError("*********************** VulkanDevice::createDevice: vkCreateDevice create a Vulkan device failed !");
            return false;
        }
        //volkLoadDevice(m_vkDevice);


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
            Util_LogInfo("VulkanDevice::createDevice: [%d]: [%s] is supported [%s]", i, des.name.c_str(), des.isSupported ? "true" : "false");
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
            Util_LogError("*********************** VulkanDevice::isSupportPresent: vkGetPhysicalDeviceSurfaceSupportKHR failed !");
            return false;
        }
        if (supportsPresent) 
        {
            Util_LogInfo("VulkanDevice::isSupportPresent: Queue Family: [%d] support Present !", familyIndex);
        }
        return (supportsPresent == VK_TRUE);
    }

}; //LostPeter