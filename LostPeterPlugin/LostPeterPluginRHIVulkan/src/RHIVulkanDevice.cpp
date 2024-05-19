/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanInstance.h"
#include "../include/RHIVulkanPhysicalDevice.h"
#include "../include/RHIVulkanQueue.h"
#include "../include/RHIVulkanSurface.h"
#include "../include/RHIVulkanSwapChain.h"
#include "../include/RHIVulkanBuffer.h"
#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanSampler.h"
#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanBindGroup.h"
#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanPipelineCompute.h"
#include "../include/RHIVulkanPipelineGraphics.h"
#include "../include/RHIVulkanCommandPool.h"
#include "../include/RHIVulkanCommandBuffer.h"
#include "../include/RHIVulkanFence.h"
#include "../include/RHIVulkanSemaphore.h"
#include "../include/RHIVulkanVolk.h"
#include "../include/RHIVulkanDebug.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanDevice::RHIVulkanDevice(RHIVulkanPhysicalDevice* pVulkanPhysicalDevice, const RHIDeviceCreateInfo& createInfo)
        : RHIDevice(pVulkanPhysicalDevice, createInfo)
        , m_pVulkanPhysicalDevice(pVulkanPhysicalDevice)
        , m_vkDevice(VK_NULL_HANDLE)
        , m_vmaAllocator(VK_NULL_HANDLE)
        , m_pCommandPoolTransfer(nullptr)
        , m_pCommandPoolGraphics(nullptr)
        , m_pCommandPoolCompute(nullptr)
        , m_pQueueGraphics(nullptr)
        , m_pQueueCompute(nullptr)
        , m_pQueueTransfer(nullptr)
        , m_pQueuePresent(nullptr)
    {
        F_Assert(m_pVulkanPhysicalDevice && "RHIVulkanDevice::RHIVulkanDevice")

        init(m_pVulkanPhysicalDevice->GetVulkanInstance()->IsEnableValidationLayers());
    }

    RHIVulkanDevice::~RHIVulkanDevice()
    {
        Destroy();
    }

    void RHIVulkanDevice::Destroy()
    {
        destroyPixelFormatInfos();
        destroyVMA();
        destroyVulkanQueue();
        destroyVulkanCommandPool();
        
        DestroyVkDevice(this->m_vkDevice);
        this->m_vkDevice = VK_NULL_HANDLE;
        
        m_pVulkanPhysicalDevice = nullptr;
    }
    void RHIVulkanDevice::destroyVMA()
    {
        if (m_vmaAllocator != VK_NULL_HANDLE)
        {
            vmaDestroyAllocator(m_vmaAllocator);
        }
        m_vmaAllocator = VK_NULL_HANDLE;
    }
    void RHIVulkanDevice::destroyVulkanQueue()
    {
        m_pQueuePresent = nullptr;
        F_DELETE(m_pQueueTransfer)
        F_DELETE(m_pQueueCompute)
        F_DELETE(m_pQueueGraphics)
        m_aVulkanQueues.clear();
        m_mapVulkanQueues.clear();
    }
    void RHIVulkanDevice::destroyVulkanCommandPool()
    {
        F_DELETE(m_pCommandPoolCompute)
        F_DELETE(m_pCommandPoolGraphics)
        F_DELETE(m_pCommandPoolTransfer)
        m_aVulkanCommandPools.clear();
        m_mapVulkanCommandPools.clear();
    }


    uint32 RHIVulkanDevice::GetQueueCount(RHIQueueType eQueue)
    {
        return eQueue == RHIQueueType::RHI_Queue_Graphics ? 1 : 0;
    }

    RHIQueue* RHIVulkanDevice::GetQueue(RHIQueueType eQueue)
    {
        RHIVulkanQueuePtrMap::iterator itFind = m_mapVulkanQueues.find(eQueue);
        if (itFind == m_mapVulkanQueues.end())
            return nullptr;
        return itFind->second;
    }

    RHICommandPool* RHIVulkanDevice::GetCommandPool(RHIQueueType eQueue)
    {
        RHIVulkanCommandPoolPtrMap::iterator itFind = m_mapVulkanCommandPools.find(eQueue);
        if (itFind == m_mapVulkanCommandPools.end())
            return nullptr;
        return itFind->second;
    }

    RHISurface* RHIVulkanDevice::CreateSurface(const RHISurfaceCreateInfo& createInfo)
    {
        return new RHIVulkanSurface(createInfo);
    }

    RHISwapChain* RHIVulkanDevice::CreateSwapChain(const RHISwapChainCreateInfo& createInfo)
    {
        return new RHIVulkanSwapChain(createInfo);
    }

    RHIBuffer* RHIVulkanDevice::CreateBuffer(const RHIBufferCreateInfo& createInfo)
    {
        return new RHIVulkanBuffer(this, createInfo);
    }

    RHITexture* RHIVulkanDevice::CreateTexture(const RHITextureCreateInfo& createInfo)
    {
        return new RHIVulkanTexture(this, createInfo);
    }

    RHISampler* RHIVulkanDevice::CreateSampler(const RHISamplerCreateInfo& createInfo)
    {
        return new RHIVulkanSampler(this, createInfo);
    }

    RHIBindGroupLayout* RHIVulkanDevice::CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupLayout(createInfo);
    }

    RHIBindGroup* RHIVulkanDevice::CreateBindGroup(const RHIBindGroupCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroup(createInfo);
    }

    RHIPipelineLayout* RHIVulkanDevice::CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineLayout(createInfo);
    }

    RHIShaderModule* RHIVulkanDevice::CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo)
    {
        return new RHIVulkanShaderModule(createInfo);
    }

    RHIPipelineCompute* RHIVulkanDevice::CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineCompute(createInfo);
    }

    RHIPipelineGraphics* RHIVulkanDevice::CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineGraphics(createInfo);
    }

    RHICommandPool* RHIVulkanDevice::CreateCommandPool()
    {
        return new RHIVulkanCommandPool(this);
    }
    RHICommandBuffer* RHIVulkanDevice::CreateCommandBuffer()
    {
        return new RHIVulkanCommandBuffer(this);
    }

    RHIFence* RHIVulkanDevice::CreateFence()
    {
        return new RHIVulkanFence(this, false);
    }
    RHIFence* RHIVulkanDevice::CreateFence(bool bIsSignaled)
    {
        return new RHIVulkanFence(this, bIsSignaled);
    }

    bool RHIVulkanDevice::CheckSwapChainFormatSupport(RHISurface* pSurface, RHIPixelFormatType ePixelFormat)
    {
        return true;
    }
    bool RHIVulkanDevice::IsPixelFormatSupported(RHIPixelFormatType ePixelFormat)
    {
        VkFormat typeFormat = RHIVulkanConverter::TransformToVkFormat(ePixelFormat);
        return IsPixelFormatSupported(typeFormat);
    }

    bool RHIVulkanDevice::IsPixelFormatSupported(VkFormat typeFormat)
    {
        VkFormatProperties prop;
        memset(&prop, 0, sizeof(VkFormatProperties));
        vkGetPhysicalDeviceFormatProperties(m_pVulkanPhysicalDevice->GetVkPhysicalDevice(), typeFormat, &prop);

        return (prop.bufferFeatures != 0 ||
                prop.linearTilingFeatures != 0 ||
                prop.optimalTilingFeatures != 0);
    }

    void RHIVulkanDevice::AddAppDeviceExtensions(const char* szNameExtension)
    {
        if (szNameExtension == nullptr)
            return;

        int count = (int)m_aAppDeviceExtensions.size();
        for (int i = 0; i < count; i++)
        {
            if (strcmp(szNameExtension, m_aAppDeviceExtensions[i]) == 0)
                return;
        }
        m_aAppDeviceExtensions.push_back(szNameExtension);
    }
    bool RHIVulkanDevice::HasExtensionName(const char* szNameExtension)
    {
        int count = (int)m_aAppDeviceExtensions.size();
        for (int i = 0; i < count; i++)
        {
            if (strcmp(szNameExtension, m_aAppDeviceExtensions[i]) == 0)
                return true;
        }
        return false;
    }
    
    bool RHIVulkanDevice::IsSupportPresent(RHIVulkanQueue* pQueue, VkSurfaceKHR vkSurfaceKHR)
    {
        const uint32 familyIndex = pQueue->GetFamilyIndex();
        VkBool32 supportsPresent = VK_FALSE;
        if (!RHI_CheckVkResult(vkGetPhysicalDeviceSurfaceSupportKHR(m_pVulkanPhysicalDevice->GetVkPhysicalDevice(), familyIndex, vkSurfaceKHR, &supportsPresent), "vkGetPhysicalDeviceSurfaceSupportKHR"))
        {
            F_LogError("*********************** RHIVulkanDevice::IsSupportPresent: vkGetPhysicalDeviceSurfaceSupportKHR failed !");
            return false;
        }
        if (supportsPresent) 
        {
            F_LogInfo("RHIVulkanDevice::IsSupportPresent: Queue Family: [%d] support Present !", familyIndex);
        }
        return (supportsPresent == VK_TRUE);
    }

    bool RHIVulkanDevice::CreateQueuePresent(VkSurfaceKHR vkSurfaceKHR)
    {
        if (m_pQueuePresent != nullptr)
            return true;

        bool compute = IsSupportPresent(m_pQueueCompute, vkSurfaceKHR);
        if (m_pQueueTransfer->GetFamilyIndex() != m_pQueueGraphics->GetFamilyIndex() && 
            m_pQueueTransfer->GetFamilyIndex() != m_pQueueCompute->GetFamilyIndex()) 
        {
            IsSupportPresent(m_pQueueTransfer, vkSurfaceKHR);
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

    void RHIVulkanDevice::SetDebugObject(VkObjectType objectType, uint64_t objectHandle, const char* objectName)
    {
        RHIVulkanDebug* pDebug = m_pVulkanPhysicalDevice->GetVulkanInstance()->GetDebug();
        if (!pDebug)
            return;
        pDebug->SetDebugObject(m_vkDevice, objectType, objectHandle, objectName);
    }


    bool RHIVulkanDevice::init(bool bIsEnableValidationLayers)
    {   
        const ConstCharPtrVector& aAppInstanceExtensions = m_pVulkanPhysicalDevice->GetVulkanInstance()->GetAppInstanceExtensions();
        size_t count_extension = aAppInstanceExtensions.size(); 
        for (size_t i = 0; i < count_extension; ++i)
        {
            AddAppDeviceExtensions(aAppInstanceExtensions[i]);
        }

        //1> createDevice
        if (!createDevice(bIsEnableValidationLayers))
        {
            F_LogError("*********************** RHIVulkanDevice::init: 1> createDevice failed !");
            return false;
        }
        F_LogInfo("RHIVulkanDevice::init: 1> createDevice success !");

        //2> createVmaAllocator
        if (!createVmaAllocator())
        {
            F_LogError("*********************** RHIVulkanDevice::init: 2> createVmaAllocator failed !");
            return false;
        }
        F_LogInfo("RHIVulkanDevice::init: 2> createVmaAllocator success !");    
        
        //3> checkPixelFormats
        if (!checkPixelFormats())
        {
            F_LogError("*********************** RHIVulkanDevice::init: 3> checkPixelFormats failed !");
            return false;
        }
        F_LogInfo("RHIVulkanDevice::init: 3> checkPixelFormats success !");



        F_LogInfo("RHIVulkanDevice::init: Init vulkan device success !");
        return true;
    }
    bool RHIVulkanDevice::createDevice(bool bIsEnableValidationLayers)
    {
        ConstCharPtrVector validationLayers;
        ConstCharPtrVector deviceExtensions;
        getDeviceLayersAndExtensions(bIsEnableValidationLayers,
                                     validationLayers,
                                     deviceExtensions);

        size_t countAppDeviceExtensions = deviceExtensions.size();
        for (size_t i = 0; i < countAppDeviceExtensions; i++)
        {
            AddAppDeviceExtensions(deviceExtensions[i]);
        }

        countAppDeviceExtensions = m_aAppDeviceExtensions.size();
        F_LogInfo("RHIVulkanDevice::createDevice: Using app device extensions count: [%d] !", (int32)countAppDeviceExtensions);
        for (int32 i = 0; i < countAppDeviceExtensions; ++i)
        {
            F_LogInfo("RHIVulkanDevice::createDevice: Using app device extension: [%s] !", m_aAppDeviceExtensions[i]);
        }

        VkDeviceCreateInfo deviceCreateInfo;
        RHI_ZeroStruct(deviceCreateInfo, VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO);
        deviceCreateInfo.enabledExtensionCount = uint32_t(m_aAppDeviceExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = m_aAppDeviceExtensions.data();
        deviceCreateInfo.enabledLayerCount = uint32_t(validationLayers.size());
        deviceCreateInfo.ppEnabledLayerNames = validationLayers.data();

        VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures = m_pVulkanPhysicalDevice->GetVkPhysicalDeviceFeatures();
        VkPhysicalDeviceFeatures2* pVkPhysicalDeviceFeatures2 = m_pVulkanPhysicalDevice->GetVkPhysicalDeviceFeatures2();
        if (pVkPhysicalDeviceFeatures2) 
        {
            deviceCreateInfo.pNext = pVkPhysicalDeviceFeatures2;
            deviceCreateInfo.pEnabledFeatures = nullptr;
            pVkPhysicalDeviceFeatures2->features = vkPhysicalDeviceFeatures;
        }
        else 
        {
            deviceCreateInfo.pEnabledFeatures = &vkPhysicalDeviceFeatures;
        }

        const VkQueueFamilyPropertiesVector& aVkQueueFamilyProperties = m_pVulkanPhysicalDevice->GetQueueFamilyProperties();
        int32 countQueueFamilyProperties = (int32)aVkQueueFamilyProperties.size();
        F_LogInfo("RHIVulkanDevice::createDevice: Queue Families count: %d", countQueueFamilyProperties);

        std::vector<VkDeviceQueueCreateInfo> queueFamilyInfos;
	
        int32 queueFamilyIndex_Graphics = -1;
        int32 queueFamilyIndex_Compute = -1;
        int32 queueFamilyIndex_Transfer = -1;
        uint32 numPriorities = 0;
        for (int32 i = 0; i < countQueueFamilyProperties; ++i)
        {
            const VkQueueFamilyProperties& currProps = aVkQueueFamilyProperties[i];

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
                F_LogInfo("RHIVulkanDevice::createDevice: Skipping unnecessary Queue Family: [%d : %d], Queues: [%s ]", i, currProps.queueCount, RHIVulkanPhysicalDevice::GetQueueType(currProps).c_str());
                continue;
            }

            VkDeviceQueueCreateInfo queueCreateInfo;
            RHI_ZeroStruct(queueCreateInfo, VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO);
            queueCreateInfo.queueFamilyIndex = i;
            queueCreateInfo.queueCount = currProps.queueCount;
            numPriorities += currProps.queueCount;
            queueFamilyInfos.push_back(queueCreateInfo);
            
            F_LogInfo("RHIVulkanDevice::createDevice: Initializing Queue Family: [%d: %d], Queues: [%s ]", i,  currProps.queueCount, RHIVulkanPhysicalDevice::GetQueueType(currProps).c_str());
        }
        
        FloatVector queuePriorities(numPriorities);
        float* pCurrentPriority = queuePriorities.data();
        for (int32 i = 0; i < queueFamilyInfos.size(); ++i)
        {
            VkDeviceQueueCreateInfo& queueCreateInfo = queueFamilyInfos[i];
            queueCreateInfo.pQueuePriorities = pCurrentPriority;
            const VkQueueFamilyProperties& currProps = aVkQueueFamilyProperties[queueCreateInfo.queueFamilyIndex];
            for (int32 j = 0; j < (int32)currProps.queueCount; ++j) 
            {
                *pCurrentPriority++ = 1.0f;
            }
        }

        deviceCreateInfo.queueCreateInfoCount = uint32_t(queueFamilyInfos.size());
        deviceCreateInfo.pQueueCreateInfos = queueFamilyInfos.data();
        
        VkResult result = vkCreateDevice(m_pVulkanPhysicalDevice->GetVkPhysicalDevice(), &deviceCreateInfo, nullptr, &m_vkDevice);
        if (result == VK_ERROR_INITIALIZATION_FAILED)
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: vkCreateDevice create a Vulkan device failed !");
            return false;
        }
        m_pVulkanPhysicalDevice->GetVulkanInstance()->GetVolk()->VolkLoadDevice(m_vkDevice);

        ////////////////////// RHIVulkanQueue //////////////////////
        //RHIVulkanQueue Graphics
        m_pQueueGraphics = new RHIVulkanQueue(this, queueFamilyIndex_Graphics);
        if (!m_pQueueGraphics->Init())
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanQueue Graphics init failed !");
            return false;
        }
        m_aVulkanQueues.push_back(m_pQueueGraphics);
        m_mapVulkanQueues[RHIQueueType::RHI_Queue_Graphics] = m_pQueueGraphics;

        //RHIVulkanQueue Compute
        if (queueFamilyIndex_Compute == -1) 
        {
            queueFamilyIndex_Compute = queueFamilyIndex_Graphics;
        }
        m_pQueueCompute = new RHIVulkanQueue(this, queueFamilyIndex_Compute);
        if (!m_pQueueCompute->Init())
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanQueue Compute init failed !");
            return false;
        }
        m_aVulkanQueues.push_back(m_pQueueCompute);
        m_mapVulkanQueues[RHIQueueType::RHI_Queue_Compute] = m_pQueueGraphics;

        //RHIVulkanQueue Transfer
        if (queueFamilyIndex_Transfer == -1) 
        {
            queueFamilyIndex_Transfer = queueFamilyIndex_Compute;
        }
        m_pQueueTransfer = new RHIVulkanQueue(this, queueFamilyIndex_Transfer);
        if (!m_pQueueTransfer->Init())
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanQueue Transfer init failed !");
            return false;
        }
        m_aVulkanQueues.push_back(m_pQueueTransfer);
        m_mapVulkanQueues[RHIQueueType::RHI_Queue_Transfer] = m_pQueueGraphics;

        ////////////////////// RHIVulkanCommandPool ////////////////
        //RHIVulkanCommandPool Transfer
        m_pCommandPoolTransfer = new RHIVulkanCommandPool(this);
        if (!m_pCommandPoolTransfer->Init(queueFamilyIndex_Transfer))
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanCommandPool Transfer init failed !");
            return false;
        }
        m_aVulkanCommandPools.push_back(m_pCommandPoolTransfer);
        m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Transfer] = m_pCommandPoolTransfer;

        //RHIVulkanCommandPool Graphics
        m_pCommandPoolGraphics = new RHIVulkanCommandPool(this);
        if (!m_pCommandPoolGraphics->Init(queueFamilyIndex_Graphics))
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanCommandPool Graphics init failed !");
            return false;
        }
        m_aVulkanCommandPools.push_back(m_pCommandPoolGraphics);
        m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Graphics] = m_pCommandPoolGraphics;

        //RHIVulkanCommandPool Compute
        m_pCommandPoolCompute = new RHIVulkanCommandPool(this);
        if (!m_pCommandPoolCompute->Init(queueFamilyIndex_Compute))
        {
            F_LogError("*********************** RHIVulkanDevice::createDevice: RHIVulkanCommandPool Compute init failed !");
            return false;
        }
        m_aVulkanCommandPools.push_back(m_pCommandPoolCompute);
        m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Compute] = m_pCommandPoolCompute;

        return true;
    }
    bool RHIVulkanDevice::createVmaAllocator()
    {   
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo info = {};
        info.vulkanApiVersion = VK_API_VERSION_1_2;
        info.instance = m_pVulkanPhysicalDevice->GetVulkanInstance()->GetVkInstance();
        info.physicalDevice = m_pVulkanPhysicalDevice->GetVkPhysicalDevice();
        info.device = m_vkDevice;
        info.pVulkanFunctions = &vulkanFunctions;

        if (!RHI_CheckVkResult(vmaCreateAllocator(&info, &m_vmaAllocator), "vmaCreateAllocator"))
        {
            F_LogError("*********************** RHIVulkanDevice::createVmaAllocator: vmaCreateAllocator failed !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::checkPixelFormats()
    {   
        int count = (int)RHIPixelFormatType::RHI_PixelFormat_Count;
        for (int i = 0; i < count; i++)
        {
            RHIPixelFormatType ePixelFormat = (RHIPixelFormatType)i;

            RHIPixelFormatInfo* pInfo = new RHIPixelFormatInfo();
            pInfo->strName = RHI_GetPixelFormatTypeName(ePixelFormat);
            pInfo->ePixelFormat = ePixelFormat;
            VkFormat vkFormat = RHIVulkanConverter::TransformToVkFormat(ePixelFormat);
            pInfo->nTypeOriginal = (uint32)vkFormat;
            pInfo->strNameOriginal = RHIVulkanConverter::TransformToVkFormatName(ePixelFormat);
            if (i != count - 1)
            {
                pInfo->bIsSupported = IsPixelFormatSupported(vkFormat);
            }
            else
            {
                pInfo->bIsSupported = false;
            }
            m_aPixelFormatInfos.push_back(pInfo);
            m_mapPixelFormatInfos[(uint32)pInfo->ePixelFormat] = pInfo;
            m_mapPixelFormatInfosOriginal[pInfo->nTypeOriginal] = pInfo;

            F_LogInfo("RHIVulkanDevice::checkPixelFormats: [%d]: [%s] - [%s] is supported [%s] !", 
                      i, 
                      pInfo->strName.c_str(), 
                      pInfo->strNameOriginal.c_str(),
                      pInfo->bIsSupported ? "true" : "false");
        }
        return true;
    }


    /////////////////////////////////////// Vulkan Function Wrapper ///////////////////////////////////////
    //////////////////// VkDevice ///////////////////////
    void RHIVulkanDevice::DestroyVkDevice(const VkDevice& vkDevice)
    {
        if (vkDevice != VK_NULL_HANDLE)
        {
            vkDestroyDevice(vkDevice, nullptr);
        }
    }
    void RHIVulkanDevice::WaitVkDeviceIdle()
    {
        vkDeviceWaitIdle(this->m_vkDevice);
    }


    //////////////////// VkCommandPool //////////////////
    VkCommandPool RHIVulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                                       uint32_t queueFamilyIndex)
    {
        VkCommandPool vkCommandPool = VK_NULL_HANDLE;
        CreateVkCommandPool(flags, 
                            queueFamilyIndex,
                            vkCommandPool);
        return vkCommandPool;
    }                                   
    bool RHIVulkanDevice::CreateVkCommandPool(VkCommandPoolCreateFlags flags,
                                              uint32_t queueFamilyIndex, 
                                              VkCommandPool& vkCommandPool)
    {
        VkCommandPoolCreateInfo commandPoolInfo = {};
        commandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        commandPoolInfo.flags = flags;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        if (!RHI_CheckVkResult(vkCreateCommandPool(this->m_vkDevice, &commandPoolInfo, RHI_CPU_ALLOCATOR, &vkCommandPool), "vkCreateCommandPool")) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkCommandPool: Failed to create command pool !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkCommandPool(const VkCommandPool& vkCommandPool)
    {   
        if (vkCommandPool != VK_NULL_HANDLE)
        {
            vkDestroyCommandPool(this->m_vkDevice, vkCommandPool, RHI_CPU_ALLOCATOR);
        }
    }


    //////////////////// VkCommandBuffer ////////////////
    VkCommandBuffer RHIVulkanDevice::AllocateVkCommandBuffer(VkCommandPool vkCommandPool,
                                                             VkCommandBufferLevel level)
    {
        VkCommandBuffer vkCommandBuffer = VK_NULL_HANDLE;
        AllocateVkCommandBuffers(vkCommandPool,
                                 level,
                                 nullptr,
                                 1,
                                 &vkCommandBuffer);
        return vkCommandBuffer;
    }
    bool RHIVulkanDevice::AllocateVkCommandBuffers(VkCommandPool vkCommandPool,
                                                   VkCommandBufferLevel level,
                                                   const void* pNext,
                                                   uint32_t commandBufferCount,
                                                   VkCommandBuffer* pCommandBuffers)
    {
        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = level;
        allocInfo.pNext = pNext;
        allocInfo.commandPool = vkCommandPool;
        allocInfo.commandBufferCount = commandBufferCount;
        if (!RHI_CheckVkResult(vkAllocateCommandBuffers(this->m_vkDevice, &allocInfo, pCommandBuffers), "vkAllocateCommandBuffers")) 
        {
            F_LogError("*********************** RHIVulkanDevice::AllocateVkCommandBuffers: Failed to allocate VkCommandBuffers !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::FreeVkCommandBuffers(VkCommandPool vkCommandPool, 
                                               uint32_t commandBufferCount, 
                                               VkCommandBuffer* pCommandBuffer)
    {
        if (pCommandBuffer != nullptr)
        {
            vkFreeCommandBuffers(this->m_vkDevice, vkCommandPool, commandBufferCount, pCommandBuffer);
        }
    }
    
    bool RHIVulkanDevice::BeginVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer,
                                               VkCommandBufferUsageFlags flags)
    {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = flags;
        if (!RHI_CheckVkResult(vkBeginCommandBuffer(vkCommandBuffer, &beginInfo), "vkBeginCommandBuffer")) 
        {
            F_LogError("*********************** RHIVulkanDevice::BeginVkCommandBuffer: Failed to call vkBeginCommandBuffer !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::EndVkCommandBuffer(const VkCommandBuffer& vkCommandBuffer)
    {
        if (!RHI_CheckVkResult(vkEndCommandBuffer(vkCommandBuffer), "vkEndCommandBuffer")) 
        {
            F_LogError("*********************** RHIVulkanDevice::EndVkCommandBuffer: Failed to call vkEndCommandBuffer !");
            return false;
        }
        return true;
    }


    //////////////////// VkQueue ////////////////////////
    VkQueue RHIVulkanDevice::GetVkQueue(uint32 queueFamilyIndex, uint32_t queueIndex)
    {
        VkQueue vkQueue;
        vkGetDeviceQueue(this->m_vkDevice, queueFamilyIndex, queueIndex, &vkQueue);
        return vkQueue;
    }
    bool RHIVulkanDevice::QueueSubmitVkCommandBuffers(const VkQueue& vkQueue,
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
        if (!RHI_CheckVkResult(vkQueueSubmit(vkQueue, commandBufferCount, &submitInfo, vkFence), "vkQueueSubmit")) 
        {
            F_LogError("*********************** RHIVulkanDevice::QueueSubmitVkCommandBuffers: Failed to call vkQueueSubmit !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::QueueWaitIdle(const VkQueue& vkQueue)
    {
        if (!RHI_CheckVkResult(vkQueueWaitIdle(vkQueue), "vkQueueWaitIdle")) 
        {
            F_LogError("*********************** RHIVulkanDevice::QueueWaitIdle: Failed to call vkQueueWaitIdle !");
            return false;
        }
        return true;
    }


    //////////////////// VkSemaphore ////////////////////
    VkSemaphore RHIVulkanDevice::CreateVkSemaphore()
    {
        VkSemaphore vkSemaphore;
        CreateVkSemaphore(vkSemaphore);
        return vkSemaphore;
    }
    bool RHIVulkanDevice::CreateVkSemaphore(VkSemaphore& vkSemaphore)
    {
        VkSemaphoreCreateInfo semaphoreCreateInfo;
        RHI_ZeroStruct(semaphoreCreateInfo, VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO);
        if (!RHI_CheckVkResult(vkCreateSemaphore(this->m_vkDevice, &semaphoreCreateInfo, RHI_CPU_ALLOCATOR, &vkSemaphore), "vkCreateSemaphore")) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkSemaphore: Failed to create VkSemaphore !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkSemaphore(const VkSemaphore& vkSemaphore)
    {
        if (vkSemaphore != VK_NULL_HANDLE)
        {
            vkDestroySemaphore(this->m_vkDevice, vkSemaphore, RHI_CPU_ALLOCATOR);
        }
    }

    void RHIVulkanDevice::DestroyVulkanSemaphore(RHIVulkanSemaphore* pVulkanSemaphore)
    {
        F_DELETE(pVulkanSemaphore)
    }
    void RHIVulkanDevice::DestroyVulkanSemaphores(RHIVulkanSemaphorePtrVector& aVulkanSemaphore)
    {
        size_t count = aVulkanSemaphore.size();
        for (size_t i = 0; i < count; i++)
        {
            RHIVulkanSemaphore* pVulkanSemaphore = aVulkanSemaphore[i];
            F_DELETE(pVulkanSemaphore)
        }
        aVulkanSemaphore.clear();
    }


    //////////////////// VkFence ////////////////////////
    VkFence RHIVulkanDevice::CreateVkFence(bool isCreateSignaled)
    {
        VkFence vkFence;
        CreateVkFence(isCreateSignaled, vkFence);
        return vkFence;
    }
    bool RHIVulkanDevice::CreateVkFence(bool isCreateSignaled, VkFence& vkFence)
    {
        VkFenceCreateInfo fenceCreateInfo;
        RHI_ZeroStruct(fenceCreateInfo, VK_STRUCTURE_TYPE_FENCE_CREATE_INFO);
        fenceCreateInfo.flags = isCreateSignaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;
        if (!RHI_CheckVkResult(vkCreateFence(this->m_vkDevice, &fenceCreateInfo, RHI_CPU_ALLOCATOR, &vkFence), "vkCreateFence")) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkFence: Failed to create VkFence !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkFence(const VkFence& vkFence)
    {
        if (vkFence != VK_NULL_HANDLE)
        {
            vkDestroyFence(this->m_vkDevice, vkFence, RHI_CPU_ALLOCATOR);
        }
    }

    bool RHIVulkanDevice::WaitVkFence(const VkFence& vkFence)
    {
        return vkWaitForFences(this->m_vkDevice, 1, &vkFence, VK_TRUE, UINT64_MAX) == VK_SUCCESS;
    }
    bool RHIVulkanDevice::WaitVkFences(const VkFenceVector& aFences)
    {
        return vkWaitForFences(this->m_vkDevice, (uint32_t)aFences.size(), aFences.data(), VK_TRUE, UINT64_MAX) == VK_SUCCESS;
    }
    bool RHIVulkanDevice::ResetVkFence(const VkFence& vkFence)
    {
        return vkResetFences(this->m_vkDevice, 1, &vkFence) == VK_SUCCESS;
    }
    bool RHIVulkanDevice::ResetVkFences(const VkFenceVector& aFences)
    {
        return vkResetFences(this->m_vkDevice, (uint32_t)aFences.size(), aFences.data()) == VK_SUCCESS;
    }

    void RHIVulkanDevice::DestroyVulkanFence(RHIVulkanFence* pVulkanFence)
    {
        F_DELETE(pVulkanFence)
    }
    void RHIVulkanDevice::DestroyVulkanFences(RHIVulkanFencePtrVector& aVulkanFence)
    {
        size_t count = aVulkanFence.size();
        for (size_t i = 0; i < count; i++)
        {
            RHIVulkanFence* pVulkanFence = aVulkanFence[i];
            F_DELETE(pVulkanFence)
        }
        aVulkanFence.clear();
    }

    void RHIVulkanDevice::WaitFence(RHIVulkanFence* pVulkanFence)
    {
        pVulkanFence->Wait();
    }
    void RHIVulkanDevice::WaitFences(RHIVulkanFencePtrVector& aVulkanFence)
    {
        size_t count = aVulkanFence.size();
        for (size_t i = 0; i < count; i++)
        {
            RHIVulkanFence* pVulkanFence = aVulkanFence[i];
            pVulkanFence->Wait();
        }
    }

    void RHIVulkanDevice::ResetFence(RHIVulkanFence* pVulkanFence)
    {
        pVulkanFence->Reset();
    }
    void RHIVulkanDevice::ResetFences(RHIVulkanFencePtrVector& aVulkanFence)
    {
        size_t count = aVulkanFence.size();
        for (size_t i = 0; i < count; i++)
        {
            RHIVulkanFence* pVulkanFence = aVulkanFence[i];
            pVulkanFence->Reset();
        }
    }


    //////////////////// VkDescriptorPool ///////////////
    bool RHIVulkanDevice::CreateVkDescriptorPool(uint32_t descriptorCount,    
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkDescriptorPool: Failed to create descriptor pool, descriptor count: [%u] !", descriptorCount);
            return false;
        }

        F_LogInfo("RHIVulkanDevice::CreateVkDescriptorPool: Success to create descriptor pool, descriptor count: [%u] !", descriptorCount);
        return true;
    }
    void RHIVulkanDevice::DestroyVkDescriptorPool(const VkDescriptorPool& vkDescriptorPool)
    {
        if (vkDescriptorPool != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorPool(this->m_vkDevice, vkDescriptorPool, nullptr);
        }
    }


    //////////////////// VkSwapchainKHR /////////////////
    bool RHIVulkanDevice::CreateVkSwapchainKHR(VkSwapchainCreateInfoKHR& vkSwapChainCreateInfoKHR,
                                               VkSwapchainKHR& vkSwapChainKHR)
    {
        if (!RHI_CheckVkResult(vkCreateSwapchainKHR(this->m_vkDevice, &vkSwapChainCreateInfoKHR, nullptr, &vkSwapChainKHR), "vkCreateSwapchainKHR"))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkSwapchainKHR: Failed to create VkSwapchainKHR !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::GetVkSwapchainImagesKHR(const VkSwapchainKHR& vkSwapChainKHR,
                                                  uint32& numSwapChainImages,
                                                  VkImageVector* pVkImages)
    {
        if (!RHI_CheckVkResult(vkGetSwapchainImagesKHR(this->m_vkDevice, vkSwapChainKHR, &numSwapChainImages, (pVkImages != nullptr ? pVkImages->data() : nullptr)), "vkGetSwapchainImagesKHR"))
        {
            F_LogError("*********************** VulkanSwapChain::GetVkSwapchainImagesKHR: vkGetSwapchainImagesKHR failed !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkSwapchainKHR(const VkSwapchainKHR& vkSwapChainKHR)
    {
        if (vkSwapChainKHR != VK_NULL_HANDLE)
        {
            vkDestroySwapchainKHR(this->m_vkDevice, vkSwapChainKHR, nullptr);
        }
    }

    RHISwapStatusType RHIVulkanDevice::VkAcquireNextImageKHR(VkSwapchainKHR vkSwapChainKHR,
                                                             uint64_t timeout,
                                                             VkSemaphore vkSemaphore,
                                                             VkFence vkFence,
                                                             uint32_t* pImageIndex)
    {
        VkResult result = vkAcquireNextImageKHR(this->m_vkDevice, vkSwapChainKHR, timeout, vkSemaphore, vkFence, pImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            return RHISwapStatusType::RHI_SwapStatus_OutOfDate;
        }
        else if (result == VK_ERROR_SURFACE_LOST_KHR) 
        {
            return RHISwapStatusType::RHI_SwapStatus_Lost;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            F_LogError("*********************** RHIVulkanDevice::VkAcquireNextImageKHR: Failed to call vkAcquireNextImageKHR, error: [%s] !", RHI_VkResult2String(result).c_str());
            throw std::runtime_error("*********************** RHIVulkanDevice::VkAcquireNextImageKHR: Failed to acquire swap chain image index !");
            return RHISwapStatusType::RHI_SwapStatus_Error;
        }
        return RHISwapStatusType::RHI_SwapStatus_Normal;
    }

    RHISwapStatusType RHIVulkanDevice::VkQueuePresentKHR(VkQueue vkQueue,   
                                                         VkSwapchainKHR vkSwapChainKHR,
                                                         const VkSemaphoreVector& aWaitSemaphores,
                                                         uint32_t* pImageIndices)
    {
        VkSwapchainKHRVector aSwapChainKHR;
        aSwapChainKHR.push_back(vkSwapChainKHR);
        return VkQueuePresentKHR(vkQueue,
                                 aSwapChainKHR,
                                 aWaitSemaphores,
                                 pImageIndices);
    }
    RHISwapStatusType RHIVulkanDevice::VkQueuePresentKHR(VkQueue vkQueue,
                                                         const VkSwapchainKHRVector& aSwapChainKHR,
                                                         const VkSemaphoreVector& aWaitSemaphores,
                                                         uint32_t* pImageIndices)
    {
        VkPresentInfoKHR presentInfo;
        RHI_ZeroStruct(presentInfo, VK_STRUCTURE_TYPE_PRESENT_INFO_KHR);
        presentInfo.waitSemaphoreCount = (uint32_t)aWaitSemaphores.size();
        presentInfo.pWaitSemaphores = aWaitSemaphores.data();
        presentInfo.swapchainCount = (uint32_t)aSwapChainKHR.size();
        presentInfo.pSwapchains = aSwapChainKHR.data();
        presentInfo.pImageIndices = pImageIndices;

        VkResult result = vkQueuePresentKHR(vkQueue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR) 
        {
            return RHISwapStatusType::RHI_SwapStatus_OutOfDate;
        }
        else if (result == VK_ERROR_SURFACE_LOST_KHR) 
        {
            return RHISwapStatusType::RHI_SwapStatus_Lost;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) 
        {
            F_LogError("*********************** RHIVulkanDevice::VkQueuePresentKHR: Failed to call vkQueuePresentKHR, error: [%s] !", RHI_VkResult2String(result).c_str());
            throw std::runtime_error("*********************** RHIVulkanDevice::VkQueuePresentKHR: Failed to present swap chain image !");
            return RHISwapStatusType::RHI_SwapStatus_Error;
        }
        return RHISwapStatusType::RHI_SwapStatus_Normal;
    }


    //////////////////// VkViewport /////////////////////
    void RHIVulkanDevice::CreateVkViewport(float nWidth,
                                           float nHeight,
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
        vkViewport.width = nWidth;
        vkViewport.height = nHeight;
        vkViewport.minDepth = minDepth;
        vkViewport.maxDepth = maxDepth;

        vkScissor = {};
        VkOffset2D offset;
        offset.x = (int32_t)offsetX;
        offset.y = (int32_t)offsetY;
        VkExtent2D extent;
        extent.width = (uint32_t)nWidth;
        extent.height = (uint32_t)nHeight;
        vkScissor.offset = offset;
        vkScissor.extent = extent;
    }


    //////////////////// VkAttachmentDescription ////////
    void RHIVulkanDevice::CreateVkAttachmentDescription(VkAttachmentDescriptionFlags flags,
                                                        VkFormat typeFormat,
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
        vkAttachmentDescription.format = typeFormat;
        vkAttachmentDescription.samples = samples;
        vkAttachmentDescription.loadOp = loadOp;
        vkAttachmentDescription.storeOp = storeOp;
        vkAttachmentDescription.stencilLoadOp = stencilLoadOp;
        vkAttachmentDescription.stencilStoreOp = stencilStoreOp;
        vkAttachmentDescription.initialLayout = initialLayout;
        vkAttachmentDescription.finalLayout = finalLayout;
    }


    //////////////////// VkRenderPass ///////////////////
    bool RHIVulkanDevice::CreateVkRenderPass(const String& nameRenderPass,
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkRenderPass: vkCreateRenderPass failed: [%s] !", nameRenderPass.c_str());
            return false;
        }

        F_LogInfo("RHIVulkanDevice::CreateVkRenderPass: vkCreateRenderPass success: [%s] !", nameRenderPass.c_str());
        return true;
    }
    void RHIVulkanDevice::DestroyVkRenderPass(const VkRenderPass& vkRenderPass)
    {
        if (vkRenderPass != VK_NULL_HANDLE)
        {
            vkDestroyRenderPass(this->m_vkDevice, vkRenderPass, nullptr);   
        }
    }
        

    //////////////////// VkFramebuffer //////////////////
    bool RHIVulkanDevice::CreateVkFramebuffer(const String& nameFramebuffer,
                                              const VkImageViewVector& aImageView, 
                                              VkRenderPass& vkRenderPass,
                                              VkFramebufferCreateFlags flags,
                                              uint32_t nWidth,
                                              uint32_t nHeight,
                                              uint32_t layers,
                                              VkFramebuffer& vkFramebuffer)
    {
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(aImageView.size());
        framebufferInfo.pAttachments = &aImageView[0];
        framebufferInfo.width = nWidth;
        framebufferInfo.height = nHeight;
        framebufferInfo.layers = layers;

        if (vkCreateFramebuffer(this->m_vkDevice, &framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkFramebuffer: Failed to create VkFramebuffer: [%s] !", nameFramebuffer.c_str());
            return false;
        }

        F_LogInfo("RHIVulkanDevice::CreateVkFramebuffer: Success to create VkFramebuffer: [%s] !", nameFramebuffer.c_str());
        return true;
    }
    void RHIVulkanDevice::DestroyVkFramebuffer(const VkFramebuffer& vkFramebuffer)
    {
        if (vkFramebuffer != VK_NULL_HANDLE)
        {
            vkDestroyFramebuffer(this->m_vkDevice, vkFramebuffer, nullptr);
        }
    }


    //////////////////// VkBuffer ///////////////////////
    bool RHIVulkanDevice::CreateVkBuffer(VkDeviceSize size, 
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
        if (!RHI_CheckVkResult(vkCreateBuffer(this->m_vkDevice, &bufferInfo, RHI_CPU_ALLOCATOR, &vkBuffer), "vkCreateBuffer")) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkBuffer: Failed to create VkBuffer !");
            return false;
        }

        VkMemoryRequirements memRequirements = { };
        vkGetBufferMemoryRequirements(this->m_vkDevice, vkBuffer, &memRequirements);
        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_pVulkanPhysicalDevice->FindMemoryType(memRequirements.memoryTypeBits, properties);
        if (vkAllocateMemory(this->m_vkDevice, &allocInfo, RHI_CPU_ALLOCATOR, &vkBufferMemory) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkBuffer: Failed to allocate VkDeviceMemory !");
            return false;
        }
        vkBindBufferMemory(this->m_vkDevice, vkBuffer, vkBufferMemory, 0);

        return true;
    }
    bool RHIVulkanDevice::CreateVkBufferVertex(void* pData, 
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferVertex: CreateVkBufferVertex failed !");
            return false;
        }
        DestroyVkBuffer(vkBufferTransfer, vkBufferMemoryTransfer);

        return true;
    }
    bool RHIVulkanDevice::CreateVkBufferVertex(void* pData, 
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferVertex: 1 CreateVkBuffer failed !");
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferVertex: 2 CreateVkBuffer failed !");
            return false;
        }
        CopyVkBuffer(vkBufferTransfer, vkBuffer, bufSize);

        return true;
    }
    bool RHIVulkanDevice::CreateVkBufferIndex(void* pData, 
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferIndex: CreateVkBufferIndex failed !");
            return false;
        }
        DestroyVkBuffer(vkBufferTransfer, vkBufferMemoryTransfer);

        return true;
    }
    bool RHIVulkanDevice::CreateVkBufferIndex(void* pData, 
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferIndex: 1 CreateVkBuffer failed !");
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkBufferIndex: 2 CreateVkBuffer failed !");
            return false;
        }
        CopyVkBuffer(vkBufferTransfer, vkBuffer, bufSize);

        return true;
    }
    bool RHIVulkanDevice::CreateVkUniformBuffer(VkDeviceSize bufSize, 
                                                VkBuffer& vkBuffer, 
                                                VkDeviceMemory& vkBufferMemory)
    {
        if (!CreateVkBuffer(bufSize, 
                            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkUniformBuffer: CreateVkBuffer failed !");
            return false;
        }

        return true;
    }
    bool RHIVulkanDevice::CreateVkUniformBuffers(VkDeviceSize bufSize, 
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
                F_LogError("*********************** RHIVulkanDevice::CreateVkUniformBuffers: CreateVkUniformBuffer failed !");
                return false;
            }
        }

        return true;
    }
    void RHIVulkanDevice::CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                                       const VkBuffer& vkBufferSrc, 
                                       const VkBuffer& vkBufferDst, 
                                       VkDeviceSize size)
    {
        VkBufferCopy copyRegion = { };
        copyRegion.size = size;
        vkCmdCopyBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, 1, &copyRegion);
    }
    void RHIVulkanDevice::CopyVkBuffer(const VkCommandBuffer& vkCommandBuffer, 
                                       const VkBuffer& vkBufferSrc, 
                                       const VkBuffer& vkBufferDst, 
                                       size_t nSrcOffset, 
                                       size_t nDstOffset, 
                                       VkDeviceSize size)
    {
        VkBufferCopy copyRegion = { nSrcOffset, nDstOffset, size };
        vkCmdCopyBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, 1, &copyRegion);
    }
    void RHIVulkanDevice::CopyVkBuffer(const VkBuffer& vkBufferSrc, 
                                       const VkBuffer& vkBufferDst, 
                                       VkDeviceSize size)
    {
        VkCommandBuffer vkCommandBuffer = BeginSingleTimeCommands();
        {
            CopyVkBuffer(vkCommandBuffer, vkBufferSrc, vkBufferDst, size);
        }
        EndSingleTimeCommands(vkCommandBuffer);
    }
    void RHIVulkanDevice::CopyVkBuffer(const VkBuffer& vkBufferSrc, 
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
    void RHIVulkanDevice::WriteVkBuffer(VkDeviceMemory& vkBufferMemory, 
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
    void RHIVulkanDevice::DestroyVkBuffer(const VkBuffer& vkBuffer, const VkDeviceMemory& vkBufferMemory)
    {
        if (vkBuffer != VK_NULL_HANDLE)
        {
            vkDestroyBuffer(this->m_vkDevice, vkBuffer, RHI_CPU_ALLOCATOR);
            vkFreeMemory(this->m_vkDevice, vkBufferMemory, RHI_CPU_ALLOCATOR);
        }
    }
    void RHIVulkanDevice::DestroyVkBuffers(VkBufferVector& aBuffer, VkDeviceMemoryVector& aBufferMemory)
    {
        size_t count = aBuffer.size();
        for (size_t i = 0; i < count; i++) 
        {
            DestroyVkBuffer(aBuffer[i], aBufferMemory[i]);
        }
    }

    void* RHIVulkanDevice::MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
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
            F_LogError("*********************** RHIVulkanDevice::MapVkDeviceMemory: MapVkDeviceMemory failed !");
            return nullptr;
        }

        return pData;
    }
    bool RHIVulkanDevice::MapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory,
                                            uint32_t nDataSize, 
                                            uint32_t nDataOffset,
                                            VkMemoryMapFlags flags,
                                            void** ppData)
    {
        if (!RHI_CheckVkResult(vkMapMemory(this->m_vkDevice, vkBufferMemory, nDataOffset, nDataSize, flags, ppData), "vkMapMemory")) 
        {
            F_LogError("*********************** RHIVulkanDevice::MapVkDeviceMemory: Failed to call vkMapMemory !");
            return false;
        }

        return true;
    }
    void RHIVulkanDevice::UnmapVkDeviceMemory(const VkDeviceMemory& vkBufferMemory)
    {
        vkUnmapMemory(this->m_vkDevice, vkBufferMemory);
    }


     //////////////////// VkImage ////////////////////////
    bool RHIVulkanDevice::CreateVkImage(uint32_t nWidth, 
                                        uint32_t nHeight, 
                                        uint32_t nDepth, 
                                        uint32_t nLayerCount,
                                        uint32_t nMipMapCount, 
                                        VkImageType typeImage, 
                                        bool bIsCubeMap,
                                        VkSampleCountFlagBits typeSamplesCountFlagBits, 
                                        VkFormat typeFormat, 
                                        VkImageTiling typeImageTiling, 
                                        VkImageUsageFlags typeImageUsageFlags, 
                                        VkSharingMode typeSharingMode,
                                        bool bIsGraphicsComputeShared,
                                        VkMemoryPropertyFlags properties, 
                                        VkImage& vkImage, 
                                        VkDeviceMemory& vkImageMemory) 
    {
        VkImageCreateInfo imageCreateInfo = {};
        imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        if (bIsCubeMap)
            imageCreateInfo.flags = VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
        imageCreateInfo.imageType = typeImage;
        imageCreateInfo.format = typeFormat;
        imageCreateInfo.extent.width = nWidth;
        imageCreateInfo.extent.height = nHeight;
        imageCreateInfo.extent.depth = nDepth;
        imageCreateInfo.mipLevels = nMipMapCount <= 0 ? 1 : nMipMapCount;
        imageCreateInfo.arrayLayers = nLayerCount;
        imageCreateInfo.samples = typeSamplesCountFlagBits;
        imageCreateInfo.tiling = typeImageTiling;
        imageCreateInfo.usage = typeImageUsageFlags;
        imageCreateInfo.sharingMode = typeSharingMode;
        if (bIsGraphicsComputeShared)
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkImage: Failed to create image !");
            return false;
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(this->m_vkDevice, vkImage, &memRequirements);

        VkMemoryAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = m_pVulkanPhysicalDevice->FindMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(this->m_vkDevice, &allocInfo, nullptr, &vkImageMemory) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkImage: Failed to allocate image memory !");
            return false;
        }
        vkBindImageMemory(this->m_vkDevice, vkImage, vkImageMemory, 0);
        return true;
    }
    bool RHIVulkanDevice::CreateVkImageView(VkImage vkImage, 
                                            VkImageViewType typeImageView, 
                                            VkFormat typeFormat, 
                                            VkComponentMapping typeComponentMapping,
                                            VkImageAspectFlags typeImageAspectFlags, 
                                            uint32_t nBaseMipLevel,
                                            uint32_t nMipMapCount,
                                            uint32_t nBaseArrayLayer,
                                            uint32_t nLayerCount,
                                            VkImageView& vkImageView) 
    {
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = vkImage;
        viewInfo.viewType = typeImageView;
        viewInfo.format = typeFormat;
        viewInfo.components = typeComponentMapping;
        viewInfo.subresourceRange.aspectMask = typeImageAspectFlags;
        viewInfo.subresourceRange.baseMipLevel = nBaseMipLevel;
        viewInfo.subresourceRange.levelCount = nMipMapCount <= 0 ? 1 : nMipMapCount;
        viewInfo.subresourceRange.baseArrayLayer = nBaseArrayLayer;
        viewInfo.subresourceRange.layerCount = nLayerCount;

        if (vkCreateImageView(this->m_vkDevice, &viewInfo, nullptr, &vkImageView) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkImageView: Failed to create texture image view !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::CreateVkSampler(uint32_t nMipMapCount, 
                                          VkSampler& vkSampler)
    {
        return CreateVkSampler(RHIAddressType::RHI_Address_ClampToEdge,
                               RHIAddressType::RHI_Address_ClampToEdge,
                               RHIAddressType::RHI_Address_ClampToEdge,
                               RHIBorderColorType::RHI_BorderColor_OpaqueBlack,
                               RHIFilterType::RHI_Filter_Linear,
                               RHIFilterType::RHI_Filter_Linear,
                               RHIFilterType::RHI_Filter_Linear,
                               RHIComparisonFuncType::RHI_ComparisonFunc_Never,
                               true,
                               m_pVulkanPhysicalDevice->GetVkPhysicalDeviceProperties().limits.maxSamplerAnisotropy,
                               0.0f,
                               static_cast<float>(nMipMapCount),
                               0.0f,
                               vkSampler);
    }
    bool RHIVulkanDevice::CreateVkSampler(RHIAddressType eAddressingU,
                                          RHIAddressType eAddressingV,
                                          RHIAddressType eAddressingW,
                                          RHIBorderColorType eBorderColor,
                                          RHIFilterType eFilterMin,
                                          RHIFilterType eFilterMag,
                                          RHIFilterType eFilterMip,
                                          RHIComparisonFuncType eComparisonFunc,
                                          bool bIsEnableAnisotropy,
                                          float fMaxAnisotropy,
                                          float fMinLod, 
                                          float fMaxLod, 
                                          float fMipLodBias,
                                          VkSampler& vkSampler)
    {
        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.addressModeU = RHIVulkanConverter::TransformToVkSamplerAddressMode(eAddressingU);
        samplerInfo.addressModeV = RHIVulkanConverter::TransformToVkSamplerAddressMode(eAddressingV);
        samplerInfo.addressModeW = RHIVulkanConverter::TransformToVkSamplerAddressMode(eAddressingW);
        samplerInfo.minFilter = RHIVulkanConverter::TransformToVkFilter(eFilterMin);
        samplerInfo.magFilter = RHIVulkanConverter::TransformToVkFilter(eFilterMag);
        samplerInfo.mipmapMode = RHIVulkanConverter::TransformToVkSamplerMipmapMode(eFilterMip);
         samplerInfo.borderColor = RHIVulkanConverter::TransformToVkBorderColor(eBorderColor);
        samplerInfo.minLod = fMinLod;
        samplerInfo.maxLod = fMaxLod;
        samplerInfo.mipLodBias = fMipLodBias;
        samplerInfo.anisotropyEnable = bIsEnableAnisotropy ? VK_TRUE : VK_FALSE;
        samplerInfo.maxAnisotropy = fMaxAnisotropy > m_pVulkanPhysicalDevice->GetVkPhysicalDeviceProperties().limits.maxSamplerAnisotropy ? m_pVulkanPhysicalDevice->GetVkPhysicalDeviceProperties().limits.maxSamplerAnisotropy : fMaxAnisotropy;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = eComparisonFunc != RHIComparisonFuncType::RHI_ComparisonFunc_Never;
        samplerInfo.compareOp = RHIVulkanConverter::TransformToVkCompareOp(eComparisonFunc);

        if (vkCreateSampler(this->m_vkDevice, &samplerInfo, nullptr, &vkSampler) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkSampler: Failed to create texture sampler !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkImage(const VkImage& vkImage, const VkDeviceMemory& vkImageMemory, const VkImageView& vkImageView)
    {
        if (vkImage != VK_NULL_HANDLE)
        {
            vkDestroyImage(this->m_vkDevice, vkImage, nullptr);
            vkFreeMemory(this->m_vkDevice, vkImageMemory, nullptr);
        }
        DestroyVkImageView(vkImageView);
    }
    void RHIVulkanDevice::DestroyVkImageView(const VkImageView& vkImageView)
    {   
        if (vkImageView != VK_NULL_HANDLE)
        {
            vkDestroyImageView(this->m_vkDevice, vkImageView, nullptr);
        }
    }
    void RHIVulkanDevice::DestroyVkSampler(const VkSampler& vkSampler)
    {
        if (vkSampler != VK_NULL_HANDLE)
        {
            vkDestroySampler(this->m_vkDevice, vkSampler, nullptr);
        }
    }


    //////////////////// VkCommandBuffer ////////////////
    VkCommandBuffer RHIVulkanDevice::BeginSingleTimeCommands()
    {
        VkCommandBuffer vkCommandBuffer = AllocateVkCommandBuffer(m_pCommandPoolTransfer->GetVkCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY);
        BeginVkCommandBuffer(vkCommandBuffer, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        return vkCommandBuffer;
    }
    void RHIVulkanDevice::EndSingleTimeCommands(VkCommandBuffer& vkCommandBuffer)
    {
        EndVkCommandBuffer(vkCommandBuffer);
        QueueSubmitVkCommandBuffers(m_pQueueTransfer->GetVkQueue(), 
                                    1, 
                                    &vkCommandBuffer, 
                                    0,
                                    nullptr,
                                    nullptr);
        QueueWaitIdle(m_pQueueTransfer->GetVkQueue());
        FreeVkCommandBuffers(m_pCommandPoolTransfer->GetVkCommandPool(), 1, &vkCommandBuffer);
    }


}; //LostPeterPluginRHIVulkan