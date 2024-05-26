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
#include "../include/RHIVulkanBindGroupLayoutCache.h"
#include "../include/RHIVulkanBindGroupLayout.h"
#include "../include/RHIVulkanBindGroupPool.h"
#include "../include/RHIVulkanBindGroupCache.h"
#include "../include/RHIVulkanBindGroup.h"
#include "../include/RHIVulkanShaderModule.h"
#include "../include/RHIVulkanShaderModuleCache.h"
#include "../include/RHIVulkanPipelineLayout.h"
#include "../include/RHIVulkanPipelineLayoutCache.h"
#include "../include/RHIVulkanPipelineCache.h"
#include "../include/RHIVulkanPipelineCompute.h"
#include "../include/RHIVulkanPipelineGraphics.h"
#include "../include/RHIVulkanRenderPassCache.h"
#include "../include/RHIVulkanRenderPass.h"
#include "../include/RHIVulkanFrameBuffer.h"
#include "../include/RHIVulkanCommandPool.h"
#include "../include/RHIVulkanCommandBuffer.h"
#include "../include/RHIVulkanFence.h"
#include "../include/RHIVulkanSemaphore.h"
#include "../include/RHIVulkanVolk.h"
#include "../include/RHIVulkanDebug.h"
#include "../include/RHIVulkanConverter.h"
#include "../include/RHIVulkanObjectManager.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image.h>

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanDevice::RHIVulkanDevice(RHIVulkanPhysicalDevice* pVulkanPhysicalDevice, const RHIDeviceCreateInfo& createInfo)
        : RHIDevice(pVulkanPhysicalDevice, createInfo)
        , m_pVulkanPhysicalDevice(pVulkanPhysicalDevice)
        , m_vkDevice(VK_NULL_HANDLE)
        , m_vmaAllocator(VK_NULL_HANDLE)
        , m_pVulkanBindGroupLayoutCache(nullptr)
        , m_pVulkanBindGroupCache(nullptr)
        , m_pVulkanShaderModuleCache(nullptr)
        , m_pVulkanPipelineLayoutCache(nullptr)
        , m_pVulkanPipelineCache(nullptr)
        , m_pVulkanRenderPassCache(nullptr)
        , m_strDebugName(createInfo.strDebugName)
        , m_pCommandPoolTransfer(nullptr)
        , m_pCommandPoolGraphics(nullptr)
        , m_pCommandPoolCompute(nullptr)
        , m_pQueueGraphics(nullptr)
        , m_pQueueCompute(nullptr)
        , m_pQueueTransfer(nullptr)
        , m_pQueuePresent(nullptr)
    {
        F_Assert(m_pVulkanPhysicalDevice && "RHIVulkanDevice::RHIVulkanDevice")
        m_pVulkanObjectManager = new RHIVulkanObjectManager(this);
        m_pObjectManager = m_pVulkanObjectManager;
        init(m_pVulkanPhysicalDevice->GetVulkanInstance()->IsEnableValidationLayers());
    }

    RHIVulkanDevice::~RHIVulkanDevice()
    {
        Destroy();
    }

    void RHIVulkanDevice::Destroy()
    {
        destroyCache();
        destroyPixelFormatInfos();
        destroyVMA();
        destroyVulkanQueue();
        destroyVulkanCommandPool();
        
        DestroyVkDevice(this->m_vkDevice);
        this->m_vkDevice = VK_NULL_HANDLE;
        
        m_pVulkanPhysicalDevice = nullptr;
    }
    void RHIVulkanDevice::destroyCache()
    {
        F_DELETE(m_pVulkanBindGroupLayoutCache)
        F_DELETE(m_pVulkanBindGroupCache)
        F_DELETE(m_pVulkanShaderModuleCache)
        F_DELETE(m_pVulkanPipelineLayoutCache)
        F_DELETE(m_pVulkanPipelineCache)
        F_DELETE(m_pVulkanRenderPassCache)
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
        return new RHIVulkanSurface(this, createInfo);
    }

    RHISwapChain* RHIVulkanDevice::CreateSwapChain(const RHISwapChainCreateInfo& createInfo)
    {
        return new RHIVulkanSwapChain(this, createInfo);
    }

    RHIBuffer* RHIVulkanDevice::CreateBuffer(const RHIBufferCreateInfo& createInfo)
    {
        return new RHIVulkanBuffer(this, createInfo);
    }

    RHITexture* RHIVulkanDevice::CreateTexture(const RHITextureCreateInfo& createInfo)
    {
        return new RHIVulkanTexture(this, createInfo);
    }
    RHITexture* RHIVulkanDevice::CreateTexture(const RHITextureCreateInfo& createInfo, VkImage& vkImage)
    {
        return new RHIVulkanTexture(this, createInfo, vkImage);
    }

    RHISampler* RHIVulkanDevice::CreateSampler(const RHISamplerCreateInfo& createInfo)
    {
        return new RHIVulkanSampler(this, createInfo);
    }

    RHIBindGroupLayoutCache* RHIVulkanDevice::CreateBindGroupLayoutCache(const RHIBindGroupLayoutCacheCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupLayoutCache(this, createInfo);
    }

    RHIBindGroupLayout* RHIVulkanDevice::CreateBindGroupLayout(const RHIBindGroupLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupLayout(this, createInfo);
    }

    RHIBindGroupPool* RHIVulkanDevice::CreateBindGroupPool(const RHIBindGroupPoolCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupPool(this, createInfo);
    }

    RHIBindGroupCache* RHIVulkanDevice::CreateBindGroupCache(const RHIBindGroupCacheCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroupCache(this, createInfo);
    }

    RHIBindGroup* RHIVulkanDevice::CreateBindGroup(const RHIBindGroupCreateInfo& createInfo)
    {
        return new RHIVulkanBindGroup(this, createInfo);
    }

    RHIShaderModuleCache* RHIVulkanDevice::CreateShaderModuleCache(const RHIShaderModuleCacheCreateInfo& createInfo)
    {
        return new RHIVulkanShaderModuleCache(this, createInfo);
    }

    RHIShaderModule* RHIVulkanDevice::CreateShaderModule(const RHIShaderModuleCreateInfo& createInfo)
    {
        return new RHIVulkanShaderModule(this, createInfo);
    }

    RHIPipelineLayoutCache* RHIVulkanDevice::CreatePipelineLayoutCache(const RHIPipelineLayoutCacheCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineLayoutCache(this, createInfo);
    }

    RHIPipelineLayout* RHIVulkanDevice::CreatePipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineLayout(this, createInfo);
    }

    RHIPipelineCache* RHIVulkanDevice::CreatePipelineCache(const RHIPipelineCacheCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineCache(this, createInfo);
    }

    RHIPipelineCompute* RHIVulkanDevice::CreatePipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineCompute(this, createInfo);
    }

    RHIPipelineGraphics* RHIVulkanDevice::CreatePipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
    {
        return new RHIVulkanPipelineGraphics(this, createInfo);
    }

    RHIRenderPassCache* RHIVulkanDevice::CreateRenderPassCache(const RHIRenderPassCacheCreateInfo& createInfo)
    {
        return new RHIVulkanRenderPassCache(this, createInfo);
    }

    RHIRenderPass* RHIVulkanDevice::CreateRenderPass(const RHIRenderPassCreateInfo& createInfo)
    {
        return new RHIVulkanRenderPass(this, createInfo);
    }

    RHIFrameBuffer* RHIVulkanDevice::CreateFrameBuffer(const RHIFrameBufferCreateInfo& createInfo)
    {
        return new RHIVulkanFrameBuffer(this, createInfo);
    }

    RHIFence* RHIVulkanDevice::CreateFence(const RHIFenceCreateInfo& createInfo)
    {
        return new RHIVulkanFence(this, createInfo);
    }

    RHISemaphore* RHIVulkanDevice::CreateSemaphore(const RHISemaphoreCreateInfo& createInfo)
    {
        return new RHIVulkanSemaphore(this, createInfo);
    }

    RHICommandPool* RHIVulkanDevice::CreateCommandPool(const RHICommandPoolCreateInfo& createInfo)
    {
        return new RHIVulkanCommandPool(this, createInfo);
    }

    RHICommandBuffer* RHIVulkanDevice::CreateCommandBuffer(const RHICommandBufferCreateInfo& createInfo)
    {
        RHIVulkanCommandPool* pVulkanCommandPool = (RHIVulkanCommandPool*)GetCommandPool(createInfo.eQueue);
        return new RHIVulkanCommandBuffer(this, pVulkanCommandPool, createInfo);
    }

    RHIQueue* RHIVulkanDevice::CreateQueue(const RHIQueueCreateInfo& createInfo)
    {
        return new RHIVulkanQueue(this, createInfo);
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

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                SetDebugObject(VK_OBJECT_TYPE_DEVICE, reinterpret_cast<uint64_t>(m_vkDevice), m_strDebugName.c_str());
            }
        }
        F_LogInfo("RHIVulkanDevice::init: Init VkDevice success !");
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
        {
            RHIQueueCreateInfo createInfo;
            //RHIVulkanQueue Graphics
            createInfo.eQueue = RHIQueueType::RHI_Queue_Graphics;
            createInfo.nFamilyIndex = queueFamilyIndex_Graphics;
            createInfo.strDebugName = "Debug-Queue-Graphics";
            m_pQueueGraphics = (RHIVulkanQueue*)CreateQueue(createInfo);
            m_aVulkanQueues.push_back(m_pQueueGraphics);
            m_mapVulkanQueues[RHIQueueType::RHI_Queue_Graphics] = m_pQueueGraphics;

            //RHIVulkanQueue Compute
            if (queueFamilyIndex_Compute == -1) 
            {
                queueFamilyIndex_Compute = queueFamilyIndex_Graphics;
            }
            createInfo.eQueue = RHIQueueType::RHI_Queue_Compute;
            createInfo.nFamilyIndex = queueFamilyIndex_Compute;
            createInfo.strDebugName = "Debug-Queue-Compute";
            m_pQueueCompute = (RHIVulkanQueue*)CreateQueue(createInfo);
            m_aVulkanQueues.push_back(m_pQueueCompute);
            m_mapVulkanQueues[RHIQueueType::RHI_Queue_Compute] = m_pQueueGraphics;

            //RHIVulkanQueue Transfer
            if (queueFamilyIndex_Transfer == -1) 
            {
                queueFamilyIndex_Transfer = queueFamilyIndex_Compute;
            }
            createInfo.eQueue = RHIQueueType::RHI_Queue_Transfer;
            createInfo.nFamilyIndex = queueFamilyIndex_Transfer;
            createInfo.strDebugName = "Debug-Queue-Transfer";
            m_pQueueTransfer = (RHIVulkanQueue*)CreateQueue(createInfo);
            m_aVulkanQueues.push_back(m_pQueueTransfer);
            m_mapVulkanQueues[RHIQueueType::RHI_Queue_Transfer] = m_pQueueGraphics;
        }
        
        ////////////////////// RHIVulkanCommandPool ////////////////
        {
            RHICommandPoolCreateInfo createInfo;
            //RHIVulkanCommandPool Transfer
            createInfo.eQueue = RHIQueueType::RHI_Queue_Transfer;
            createInfo.strDebugName = "Debug-CommandPool-Transfer";
            m_pCommandPoolTransfer = (RHIVulkanCommandPool*)CreateCommandPool(createInfo);
            m_aVulkanCommandPools.push_back(m_pCommandPoolTransfer);
            m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Transfer] = m_pCommandPoolTransfer;

            //RHIVulkanCommandPool Graphics
            createInfo.eQueue = RHIQueueType::RHI_Queue_Graphics;
            createInfo.strDebugName = "Debug-CommandPool-Graphics";
            m_pCommandPoolGraphics = (RHIVulkanCommandPool*)CreateCommandPool(createInfo);
            m_aVulkanCommandPools.push_back(m_pCommandPoolGraphics);
            m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Graphics] = m_pCommandPoolGraphics;

            //RHIVulkanCommandPool Compute
            createInfo.eQueue = RHIQueueType::RHI_Queue_Compute;
            createInfo.strDebugName = "Debug-CommandPool-Compute";
            m_pCommandPoolCompute = (RHIVulkanCommandPool*)CreateCommandPool(createInfo);
            m_aVulkanCommandPools.push_back(m_pCommandPoolCompute);
            m_mapVulkanCommandPools[RHIQueueType::RHI_Queue_Compute] = m_pCommandPoolCompute;
        }
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
    bool RHIVulkanDevice::AllocateVkCommandBuffer(VkCommandPool vkCommandPool,
                                                  VkCommandBufferLevel level,
                                                  VkCommandBuffer& vkCommandBuffer)
    {
        return  AllocateVkCommandBuffers(vkCommandPool,
                                         level,
                                         nullptr,
                                         1,
                                         &vkCommandBuffer);
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

    void RHIVulkanDevice::TransitionVkImageLayout(VkCommandBuffer cmdBuffer,
                                                  VkImage vkImage, 
                                                  VkImageLayout oldLayout, 
                                                  VkImageLayout newLayout,
                                                  uint32_t nMipBase,
                                                  uint32_t nMipCount,
                                                  uint32_t nLayerBase,
                                                  uint32_t nLayerCount,
                                                  VkImageAspectFlags typeImageAspectFlags /*= VK_IMAGE_ASPECT_COLOR_BIT*/) 
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

            barrier.subresourceRange.aspectMask = typeImageAspectFlags;
            barrier.subresourceRange.baseMipLevel = nMipBase;
            barrier.subresourceRange.levelCount = nMipCount <= 0 ? 1 : nMipCount;
            barrier.subresourceRange.baseArrayLayer = nLayerBase;
            barrier.subresourceRange.layerCount = nLayerCount;

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
                F_LogError("*********************** RHIVulkanDevice::TransitionVkImageLayout: Unsupported layout transition !");
                throw std::invalid_argument("*********************** RHIVulkanDevice::TransitionVkImageLayout: Unsupported layout transition !");
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
    void RHIVulkanDevice::CopyVkBufferToVkImage(VkCommandBuffer cmdBuffer,
                                                VkBuffer vkBuffer, 
                                                VkImage vkImage, 
                                                uint32_t nWidth, 
                                                uint32_t nHeight,
                                                uint32_t nDepth,
                                                uint32_t nPixelSize,
                                                uint32_t nLayerCount) 
    {
        bool isCreate = false;
        if (cmdBuffer == VK_NULL_HANDLE)
        {
            isCreate = true;
            cmdBuffer = BeginSingleTimeCommands();
        }
        {
            std::vector<VkBufferImageCopy> bufferCopyRegions;
            for (uint32_t i = 0; i < nLayerCount; i++)
            {
                VkBufferImageCopy region = {};
                region.bufferOffset = nWidth * nHeight * nPixelSize * i;
                region.imageExtent.width = nWidth;
                region.imageExtent.height = nHeight;
                region.imageExtent.depth = nDepth;
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
    void RHIVulkanDevice::GenerateVkImageMipMaps(VkCommandBuffer cmdBuffer,
                                                 VkImage vkImage, 
                                                 VkFormat imageFormat, 
                                                 int32_t nWidth, 
                                                 int32_t nHeight, 
                                                 uint32_t nMipMapCount,
                                                 uint32_t nLayerCount,
                                                 bool bIsAutoMipMap)
    {
        bool isCreate = false;
        if (cmdBuffer == VK_NULL_HANDLE)
        {
            isCreate = true;
            cmdBuffer = BeginSingleTimeCommands();
        }
        {
            if (bIsAutoMipMap)
            {
                int32_t mipWidth = nWidth;
                int32_t mipHeight = nHeight;
                for (uint32_t i = 1; i < nMipMapCount; i++) 
                {
                    TransitionVkImageLayout(cmdBuffer,
                                            vkImage, 
                                            VK_IMAGE_LAYOUT_UNDEFINED, 
                                            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                            i,
                                            1,
                                            0,
                                            nLayerCount);
                    {
                        VkImageBlit blit = {};
                        blit.srcOffsets[0] = {0, 0, 0};
                        blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
                        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        blit.srcSubresource.mipLevel = i - 1;
                        blit.srcSubresource.baseArrayLayer = 0;
                        blit.srcSubresource.layerCount = nLayerCount;

                        blit.dstOffsets[0] = {0, 0, 0};
                        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
                        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
                        blit.dstSubresource.mipLevel = i;
                        blit.dstSubresource.baseArrayLayer = 0;
                        blit.dstSubresource.layerCount = nLayerCount;

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
                                            nLayerCount);
                }
            }

            TransitionVkImageLayout(cmdBuffer,
                                    vkImage,
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                                    0,
                                    nMipMapCount,
                                    0,
                                    nLayerCount);
        }
        if (isCreate)
        {
            EndSingleTimeCommands(cmdBuffer);
        }
    }

    bool RHIVulkanDevice::CreateTexture1D(const String& pathAsset, 
                                          RHIPixelFormatType& ePixelFormatRet,
                                          uint32_t& nMipMapCount,
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory)
    {
        VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
        return CreateTexture2D(pathAsset,
                               VK_IMAGE_TYPE_1D,
                               VK_SAMPLE_COUNT_1_BIT,
                               typeFormat,
                               ePixelFormatRet,
                               true,
                               nMipMapCount,
                               vkImage,
                               vkImageMemory);
    }
    
    bool RHIVulkanDevice::CreateTexture2D(const String& pathAsset, 
                                          VkImageType typeImage,
                                          VkSampleCountFlagBits typeSamplesCountFlagBits,
                                          VkFormat& typeFormat,
                                          RHIPixelFormatType& ePixelFormatRet,
                                          bool bIsAutoMipMap, 
                                          uint32_t& nMipMapCount, 
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory,
                                          VkBuffer& vkBuffer, 
                                          VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
        int nWidth, nHeight, texChannels;
        int nPixelSize = 4;
        stbi_uc* pixels = stbi_load(pathTexture.c_str(), &nWidth, &nHeight, &texChannels, STBI_rgb_alpha);
        VkDeviceSize imageSize = nWidth * nHeight * nPixelSize;
        nMipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(nWidth, nHeight)))) + 1;
        if (!pixels) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2D: Failed to load texture image: [%s] !", pathAsset.c_str());
            return false;
        }
        uint8* pData = pixels;
        ePixelFormatRet = RHIPixelFormatType::RHI_PixelFormat_RGBA8UNormSRGB;
        typeFormat = RHIVulkanConverter::TransformToVkFormat(ePixelFormatRet);

        //2> Create Buffer and copy Texture data to buffer
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2D: Failed to create vk buffer: [%s] !", pathTexture.c_str());
            return false;
        }

        WriteVkBuffer(vkBufferMemory, 
                      (void*)pData,
                      (uint32_t)imageSize, 
                      0);

        uint32_t nDepth = 1;
        uint32_t nLayerCount = 1;

        //3> CreateImage
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           typeImage,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           false,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2D: Failed to create vk image: [%s] !", pathTexture.c_str());
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
                                    nLayerCount);
            {
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(nWidth), 
                                      static_cast<uint32_t>(nHeight),
                                      static_cast<uint32_t>(nDepth), 
                                      static_cast<uint32_t>(nPixelSize), 
                                      nLayerCount);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    nLayerCount);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   typeFormat, 
                                   nWidth, 
                                   nHeight, 
                                   nMipMapCount,
                                   nLayerCount,
                                   bIsAutoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool RHIVulkanDevice::CreateTexture2D(const String& pathAsset, 
                                          VkImageType typeImage,
                                          VkSampleCountFlagBits typeSamplesCountFlagBits,
                                          VkFormat& typeFormat,
                                          RHIPixelFormatType& ePixelFormatRet,
                                          bool bIsAutoMipMap, 
                                          uint32_t& nMipMapCount, 
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture2D(pathAsset, 
                                    typeImage, 
                                    typeSamplesCountFlagBits,
                                    typeFormat,
                                    ePixelFormatRet,
                                    bIsAutoMipMap,
                                    nMipMapCount,
                                    vkImage, 
                                    vkImageMemory, 
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool RHIVulkanDevice::CreateTexture2D(const String& pathAsset, 
                                          RHIPixelFormatType& ePixelFormatRet,
                                          uint32_t& nMipMapCount,
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory)
    {
        VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
        return CreateTexture2D(pathAsset,
                               VK_IMAGE_TYPE_2D,
                               VK_SAMPLE_COUNT_1_BIT,
                               typeFormat,
                               ePixelFormatRet,
                               true,
                               nMipMapCount,
                               vkImage,
                               vkImageMemory);
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
    bool RHIVulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                               VkImageType typeImage,
                                               VkSampleCountFlagBits typeSamplesCountFlagBits,
                                               VkFormat& typeFormat,
                                               RHIPixelFormatType& ePixelFormatRet,
                                               bool bIsAutoMipMap, 
                                               uint32_t& nMipMapCount, 
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory,
                                               VkBuffer& vkBuffer, 
                                               VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<int> aPixelSize;
        std::vector<RHIPixelFormatType> aPixelFormatType;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture path count <= 0 !");
            return false;
        }
        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset = aPathAsset[i];
            String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** RHIVulkanDevice::CreateTexture2DArray: Failed to load texture image: " + pathTexture;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            RHIPixelFormatType typePixelFormat = RHIPixelFormatType::RHI_PixelFormat_RGBA8UNormSRGB;
            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
             aPixelSize.push_back(4);
            aPixelFormatType.push_back(typePixelFormat);
        }

        int nWidth = aWidth[0];
        int nHeight = aHeight[0];
        int nPixelSize = aPixelSize[0];
        RHIPixelFormatType typePixelFormat = aPixelFormatType[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != nWidth)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture image's all width must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aHeight[i] != nHeight)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture image's all height must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aPixelSize[i] != nPixelSize)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture image's all pixel size must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aPixelFormatType[i] != typePixelFormat)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture image's all pixel format must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
        }
        ePixelFormatRet = typePixelFormat;
        typeFormat = RHIVulkanConverter::TransformToVkFormat(typePixelFormat);

        uint32_t nDepth = 1;
        uint32_t nLayerCount = (uint32_t)count_tex;
        if (typeImage == VK_IMAGE_TYPE_1D)
        {
            nDepth = 0;
        }

        //2> Create Buffer and copy Texture data to buffer
        nMipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(nWidth, nHeight)))) + 1;
        VkDeviceSize imageSize = nWidth * nHeight * nPixelSize;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Failed to create vk buffer !");
            s_DeletePixels(aPixels);
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
           stbi_uc* pixels = aPixels[i];
            WriteVkBuffer(vkBufferMemory,
                          pixels,
                          (uint32_t)imageSize,
                          (uint32_t)(nWidth * nHeight * nPixelSize * i));
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           typeImage,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           false,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Failed to create vk image 2d array !");
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
                                    nLayerCount);
            {   
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(nWidth), 
                                      static_cast<uint32_t>(nHeight),
                                      static_cast<uint32_t>(nDepth), 
                                      static_cast<uint32_t>(nPixelSize), 
                                      nLayerCount);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    nLayerCount);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   typeFormat, 
                                   nWidth, 
                                   nHeight,
                                   nMipMapCount,
                                   nLayerCount,
                                   bIsAutoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool RHIVulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                               VkImageType typeImage,
                                               VkSampleCountFlagBits typeSamplesCountFlagBits,
                                               VkFormat& typeFormat,
                                               RHIPixelFormatType& ePixelFormatRet,
                                               bool bIsAutoMipMap, 
                                               uint32_t& nMipMapCount, 
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture2DArray(aPathAsset, 
                                         typeImage, 
                                         typeSamplesCountFlagBits,
                                         typeFormat,
                                         ePixelFormatRet,
                                         bIsAutoMipMap,
                                         nMipMapCount,
                                         vkImage, 
                                         vkImageMemory, 
                                         vkStagingBuffer, 
                                         vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool RHIVulkanDevice::CreateTexture2DArray(const StringVector& aPathAsset, 
                                               RHIPixelFormatType& ePixelFormatRet,
                                               uint32_t& nMipMapCount,
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory)
    {
        VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
        return CreateTexture2DArray(aPathAsset,
                                    VK_IMAGE_TYPE_2D,
                                    VK_SAMPLE_COUNT_1_BIT,
                                    typeFormat,
                                    ePixelFormatRet,
                                    true,
                                    nMipMapCount,
                                    vkImage,
                                    vkImageMemory);
    }
    
    bool RHIVulkanDevice::CreateTexture3D(VkFormat typeFormat,
                                          const uint8* pDataRGBA,
                                          uint32_t nWidth,
                                          uint32_t nHeight,
                                          uint32_t nDepth,
                                          uint32_t nPixelSize,
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory,
                                          VkBuffer& vkBuffer, 
                                          VkDeviceMemory& vkBufferMemory)
    {
        VkFormatProperties formatProperties;
		vkGetPhysicalDeviceFormatProperties(m_pVulkanPhysicalDevice->GetVkPhysicalDevice(), typeFormat, &formatProperties);
		if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_TRANSFER_DST_BIT))
		{
            F_LogError("*********************** RHIVulkanDevice::CreateTexture3D: Physical device does not support flag 'VK_FORMAT_FEATURE_TRANSFER_DST_BIT' for selected texture format !");
            return false;
		}
		uint32_t maxImageDimension3D(m_pVulkanPhysicalDevice->GetVkPhysicalDeviceProperties().limits.maxImageDimension3D);
		if (nWidth > maxImageDimension3D || nHeight > maxImageDimension3D || nDepth > maxImageDimension3D)
		{
            F_LogError("*********************** RHIVulkanDevice::CreateTexture3D: Requested texture dimensions is greater than supported 3D texture dimension !");
			return false;
		}

        //1> Create Buffer and copy Texture data to buffer
        VkDeviceSize imageSize = nWidth * nHeight * nDepth * nPixelSize;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture3D: Failed to create vk buffer !");
            return false;
        }

        WriteVkBuffer(vkBufferMemory, 
                      (void*)pDataRGBA, 
                      (uint32_t)imageSize, 
                      0);

        //2> CreateImage
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           1,
                           1, 
                           VK_IMAGE_TYPE_3D,
                           false,
                           VK_SAMPLE_COUNT_1_BIT, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           false,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTexture3D: Failed to create vk image 3d !");
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
                                      static_cast<uint32_t>(nWidth), 
                                      static_cast<uint32_t>(nHeight),
                                      static_cast<uint32_t>(nDepth), 
                                      static_cast<uint32_t>(nPixelSize), 
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
                                   typeFormat, 
                                   nWidth, 
                                   nHeight,
                                   1,
                                   1,
                                   false);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool RHIVulkanDevice::CreateTexture3D(VkFormat typeFormat,
                                          const uint8* pDataRGBA,
                                          uint32_t nWidth,
                                          uint32_t nHeight,
                                          uint32_t nDepth,
                                          uint32_t nPixelSize,
                                          VkImage& vkImage, 
                                          VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTexture3D(typeFormat, 
                                    pDataRGBA, 
                                    nWidth,
                                    nHeight,
                                    nDepth,
                                    nPixelSize,
                                    vkImage, 
                                    vkImageMemory, 
                                    vkStagingBuffer, 
                                    vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset, 
                                               VkSampleCountFlagBits typeSamplesCountFlagBits,
                                               VkFormat& typeFormat,
                                               RHIPixelFormatType& ePixelFormatRet,
                                               bool bIsAutoMipMap, 
                                               uint32_t& nMipMapCount, 
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory,
                                               VkBuffer& vkBuffer, 
                                               VkDeviceMemory& vkBufferMemory)
    {
        //1> Load Texture From File
        std::vector<int> aWidth;
        std::vector<int> aHeight;
        std::vector<int> aPixelSize;
        std::vector<RHIPixelFormatType> aPixelFormatType;
        std::vector<stbi_uc*> aPixels;

        size_t count_tex = aPathAsset.size();
        if (count_tex <= 0)
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Texture path count <= 0 !");
            return false;
        }
        if (count_tex != 6)
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Texture path count != 6 !");
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            const String& pathAsset = aPathAsset[i];
            String pathTexture = FPathManager::GetSingleton().GetFilePath(pathAsset);
            int width, height, texChannels;
            stbi_uc* pixels = stbi_load(pathTexture.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
            if (!pixels) 
            {
                s_DeletePixels(aPixels);
                String msg = "*********************** RHIVulkanDevice::CreateTextureCubeMap: Failed to load texture image: " + pathTexture;
                F_LogError(msg.c_str());
                throw std::runtime_error(msg);
            }
            RHIPixelFormatType typePixelFormat = RHIPixelFormatType::RHI_PixelFormat_RGBA8UNormSRGB;
            aWidth.push_back(width);
            aHeight.push_back(height);
            aPixels.push_back(pixels);
            aPixelSize.push_back(4);
            aPixelFormatType.push_back(typePixelFormat);
        }

        int nWidth = aWidth[0];
        int nHeight = aHeight[0];
        int nPixelSize = aPixelSize[0];
        RHIPixelFormatType typePixelFormat = aPixelFormatType[0];
        for (size_t i = 1; i < count_tex; i++)
        {
            if (aWidth[i] != nWidth)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Texture image's all width must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aHeight[i] != nHeight)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Texture image's all height must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aPixelSize[i] != nPixelSize)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Texture image's all pixel size must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
            if (aPixelFormatType[i] != typePixelFormat)
            {
                F_LogError("*********************** RHIVulkanDevice::CreateTexture2DArray: Texture image's all pixel format must the same !");
                s_DeletePixels(aPixels);
                return false;
            }
        }
        ePixelFormatRet = typePixelFormat;
        typeFormat = RHIVulkanConverter::TransformToVkFormat(typePixelFormat);

        uint32_t nDepth = 1;
        uint32_t nLayerCount = (uint32_t)count_tex;

        //2> Create Buffer and copy Texture data to buffer
        nMipMapCount = static_cast<uint32_t>(std::floor(std::log2(std::max(nWidth, nHeight)))) + 1;
        VkDeviceSize imageSize = nWidth * nHeight * nPixelSize;
        VkDeviceSize imageSizeAll = imageSize * count_tex;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Failed to create vk buffer !");
            s_DeletePixels(aPixels);
            return false;
        }

        for (size_t i = 0; i < count_tex; i++)
        {
            stbi_uc* pixels = aPixels[i];
            WriteVkBuffer(vkBufferMemory,
                          pixels,
                          (uint32_t)imageSize,
                          (uint32_t)(nWidth * nHeight * nPixelSize * i));
        }
        s_DeletePixels(aPixels);

        //3> CreateImage, TransitionImageLayout and CopyBufferToImage
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           VK_IMAGE_TYPE_2D,
                           true,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           false,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureCubeMap: Failed to create vk image cubemap !");
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
                                    nLayerCount);
            {   
                CopyVkBufferToVkImage(cmdBuffer,
                                      vkBuffer, 
                                      vkImage, 
                                      static_cast<uint32_t>(nWidth), 
                                      static_cast<uint32_t>(nHeight),
                                      static_cast<uint32_t>(nDepth), 
                                      static_cast<uint32_t>(nPixelSize), 
                                      nLayerCount);
            }
            TransitionVkImageLayout(cmdBuffer,
                                    vkImage, 
                                    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 
                                    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                                    0,
                                    1,
                                    0,
                                    nLayerCount);

            GenerateVkImageMipMaps(cmdBuffer,
                                   vkImage, 
                                   typeFormat, 
                                   nWidth, 
                                   nHeight,
                                   nMipMapCount,
                                   nLayerCount,
                                   bIsAutoMipMap);
        }
        EndSingleTimeCommands(cmdBuffer);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset, 
                                               VkSampleCountFlagBits typeSamplesCountFlagBits,
                                               VkFormat& typeFormat,
                                               RHIPixelFormatType& ePixelFormatRet,
                                               bool bIsAutoMipMap, 
                                               uint32_t& nMipMapCount, 
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureCubeMap(aPathAsset, 
                                         typeSamplesCountFlagBits, 
                                         typeFormat,
                                         ePixelFormatRet,
                                         bIsAutoMipMap,
                                         nMipMapCount,
                                         vkImage, 
                                         vkImageMemory, 
                                         vkStagingBuffer, 
                                         vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }
    bool RHIVulkanDevice::CreateTextureCubeMap(const StringVector& aPathAsset,
                                               RHIPixelFormatType& ePixelFormatRet,
                                               uint32_t& nMipMapCount, 
                                               VkImage& vkImage, 
                                               VkDeviceMemory& vkImageMemory)
    {
        VkFormat typeFormat = VK_FORMAT_R8G8B8A8_SRGB;
        return CreateTextureCubeMap(aPathAsset, 
                                    VK_SAMPLE_COUNT_1_BIT, 
                                    typeFormat,
                                    ePixelFormatRet,
                                    true,
                                    nMipMapCount,
                                    vkImage, 
                                    vkImageMemory);
    }

    bool RHIVulkanDevice::CreateTextureRenderTarget1D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory,
                                                      VkBuffer& vkBuffer, 
                                                      VkDeviceMemory& vkBufferMemory)
    {
        return CreateTextureRenderTarget2D(clDefault,
                                           isSetColor,
                                           nWidth,
                                           1,
                                           nMipMapCount,
                                           VK_IMAGE_TYPE_1D,
                                           typeSamplesCountFlagBits,
                                           typeFormat,
                                           typeImageUsageFlags,
                                           bIsGraphicsComputeShared,
                                           vkImage,
                                           vkImageMemory,
                                           vkBuffer,
                                           vkBufferMemory);
    }
    bool RHIVulkanDevice::CreateTextureRenderTarget1D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget1D(clDefault, 
                                                isSetColor,
                                                nWidth, 
                                                nMipMapCount,
                                                typeSamplesCountFlagBits,
                                                typeFormat,
                                                typeImageUsageFlags,
                                                bIsGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureRenderTarget2D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nMipMapCount,
                                                      VkImageType typeImage,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory,
                                                      VkBuffer& vkBuffer, 
                                                      VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = nWidth * nHeight * 4;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2D: Failed to create vk buffer !");
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
                for (uint32_t i = 0; i < nWidth * nHeight; i++)
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
        uint32_t nDepth = 1;
        uint32_t nLayerCount = 1;
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           typeImage,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           typeImageUsageFlags, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           bIsGraphicsComputeShared,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2D: Failed to create vk image 2d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                nMipMapCount,
                                0,
                                nLayerCount);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureRenderTarget2D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2D(clDefault, 
                                                isSetColor,
                                                nWidth, 
                                                nHeight,
                                                nMipMapCount,
                                                VK_IMAGE_TYPE_2D,
                                                typeSamplesCountFlagBits,
                                                typeFormat,
                                                typeImageUsageFlags,
                                                bIsGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureRenderTarget2D(uint8* pData,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nMipMapCount,
                                                      VkImageType typeImage,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory,
                                                      VkBuffer& vkBuffer, 
                                                      VkDeviceMemory& vkBufferMemory)
    {
        uint32_t sizeFormat = 4;
        if (typeFormat == VK_FORMAT_R8_UNORM)
        {
            sizeFormat = 1;
        }
        else if (typeFormat == VK_FORMAT_R16_UNORM)
        {
            sizeFormat = 2;
        }
        //1> CreateBuffer
        VkDeviceSize imageSize = nWidth * nHeight * sizeFormat;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2D: Failed to create vk buffer !");
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
        uint32_t nDepth = 1;
        uint32_t nLayerCount = 1;
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           typeImage,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           typeImageUsageFlags, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           bIsGraphicsComputeShared,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2D: Failed to create vk image 2d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                nMipMapCount,
                                0,
                                nLayerCount);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureRenderTarget2D(uint8* pData,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2D(pData,
                                                nWidth, 
                                                nHeight,
                                                nMipMapCount,
                                                VK_IMAGE_TYPE_2D,
                                                typeSamplesCountFlagBits,
                                                typeFormat,
                                                typeImageUsageFlags,
                                                bIsGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureRenderTarget2DArray(const FColor& clDefault,
                                                           bool isSetColor,
                                                           uint32_t nWidth, 
                                                           uint32_t nHeight,
                                                           uint32_t nLayerCount,
                                                           uint32_t nMipMapCount,
                                                           VkImageType typeImage,
                                                           VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                           VkFormat typeFormat,
                                                           VkImageUsageFlags typeImageUsageFlags, 
                                                           bool bIsGraphicsComputeShared,
                                                           VkImage& vkImage, 
                                                           VkDeviceMemory& vkImageMemory,
                                                           VkBuffer& vkBuffer, 
                                                           VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = nWidth * nHeight * 4;
        VkDeviceSize imageSizeAll = imageSize * nLayerCount;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2DArray: Failed to create vk buffer !");
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
                for (uint32_t i = 0; i < nWidth * nHeight * nLayerCount; i++)
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
        uint32_t nDepth = 1;
        if (typeImage == VK_IMAGE_TYPE_1D)
        {
            nDepth = 0;
        }
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           typeImage,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           typeImageUsageFlags, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           bIsGraphicsComputeShared,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget2DArray: Failed to create vk image 2d array !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                nMipMapCount,
                                0,
                                nLayerCount);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureRenderTarget2DArray(const FColor& clDefault,
                                                           bool isSetColor,
                                                           uint32_t nWidth, 
                                                           uint32_t nHeight,
                                                           uint32_t nLayerCount,
                                                           uint32_t nMipMapCount,
                                                           VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                           VkFormat typeFormat,
                                                           VkImageUsageFlags typeImageUsageFlags, 
                                                           bool bIsGraphicsComputeShared,
                                                           VkImage& vkImage, 
                                                           VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget2DArray(clDefault, 
                                                     isSetColor,
                                                     nWidth, 
                                                     nHeight,
                                                     nLayerCount,
                                                     nMipMapCount,
                                                     VK_IMAGE_TYPE_2D,
                                                     typeSamplesCountFlagBits,
                                                     typeFormat,
                                                     typeImageUsageFlags,
                                                     bIsGraphicsComputeShared,
                                                     vkImage, 
                                                     vkImageMemory, 
                                                     vkStagingBuffer, 
                                                     vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureRenderTarget3D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nDepth,
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory,
                                                      VkBuffer& vkBuffer, 
                                                      VkDeviceMemory& vkBufferMemory)
    {
        //1> CreateBuffer
        VkDeviceSize imageSize = nWidth * nHeight * nDepth * 4;
        if (!CreateVkBuffer(imageSize, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget3D: Failed to create vk buffer !");
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
                for (uint32_t i = 0; i < nWidth * nHeight * nDepth; i++)
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
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           1,
                           nMipMapCount, 
                           VK_IMAGE_TYPE_3D,
                           false,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           typeImageUsageFlags, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           bIsGraphicsComputeShared,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTarget3D: Failed to create vk image 3d !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                nMipMapCount,
                                0,
                                1);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureRenderTarget3D(const FColor& clDefault,
                                                      bool isSetColor,
                                                      uint32_t nWidth, 
                                                      uint32_t nHeight,
                                                      uint32_t nDepth,
                                                      uint32_t nMipMapCount,
                                                      VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                      VkFormat typeFormat,
                                                      VkImageUsageFlags typeImageUsageFlags, 
                                                      bool bIsGraphicsComputeShared,
                                                      VkImage& vkImage, 
                                                      VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTarget3D(clDefault, 
                                                isSetColor,
                                                nWidth, 
                                                nHeight,
                                                nDepth,
                                                nMipMapCount,
                                                typeSamplesCountFlagBits,
                                                typeFormat,
                                                typeImageUsageFlags,
                                                bIsGraphicsComputeShared,
                                                vkImage, 
                                                vkImageMemory, 
                                                vkStagingBuffer, 
                                                vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }

    bool RHIVulkanDevice::CreateTextureRenderTargetCubeMap(uint32_t nWidth, 
                                                           uint32_t nHeight,
                                                           uint32_t nMipMapCount,
                                                           VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                           VkFormat typeFormat,
                                                           VkImageUsageFlags typeImageUsageFlags, 
                                                           bool bIsGraphicsComputeShared,
                                                           VkImage& vkImage, 
                                                           VkDeviceMemory& vkImageMemory,
                                                           VkBuffer& vkBuffer, 
                                                           VkDeviceMemory& vkBufferMemory)
    {
        uint32_t nLayerCount = 6;
        //1> CreateBuffer
        VkDeviceSize imageSize = nWidth * nHeight * 4;
        VkDeviceSize imageSizeAll = imageSize * nLayerCount;
        if (!CreateVkBuffer(imageSizeAll, 
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                            vkBuffer, 
                            vkBufferMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTargetCubeMap: Failed to create vk buffer !");
            return false;
        }

        //2> CreateImage
        uint32_t nDepth = 1;
        if (!CreateVkImage(nWidth, 
                           nHeight, 
                           nDepth,
                           nLayerCount,
                           nMipMapCount, 
                           VK_IMAGE_TYPE_2D,
                           true,
                           typeSamplesCountFlagBits, 
                           typeFormat, 
                           VK_IMAGE_TILING_OPTIMAL, 
                           typeImageUsageFlags, 
                           VK_SHARING_MODE_EXCLUSIVE,
                           bIsGraphicsComputeShared,
                           VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                           vkImage, 
                           vkImageMemory))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateTextureRenderTargetCubeMap: Failed to create vk image cubemap !");
            return false;
        }

        //3> TransitionImageLayout
        TransitionVkImageLayout(VK_NULL_HANDLE,
                                vkImage, 
                                VK_IMAGE_LAYOUT_UNDEFINED, 
                                VK_IMAGE_LAYOUT_GENERAL,
                                0,
                                nMipMapCount,
                                0,
                                nLayerCount);
        return true;
    }
    bool RHIVulkanDevice::CreateTextureRenderTargetCubeMap(uint32_t nWidth, 
                                                           uint32_t nHeight,
                                                           uint32_t nMipMapCount,
                                                           VkSampleCountFlagBits typeSamplesCountFlagBits,
                                                           VkFormat typeFormat,
                                                           VkImageUsageFlags typeImageUsageFlags, 
                                                           bool bIsGraphicsComputeShared,
                                                           VkImage& vkImage, 
                                                           VkDeviceMemory& vkImageMemory)
    {
        VkBuffer vkStagingBuffer;
        VkDeviceMemory vkStagingBufferMemory;
        bool bRet = CreateTextureRenderTargetCubeMap(nWidth, 
                                                     nHeight,
                                                     nMipMapCount,
                                                     typeSamplesCountFlagBits,
                                                     typeFormat,
                                                     typeImageUsageFlags,
                                                     bIsGraphicsComputeShared,
                                                     vkImage, 
                                                     vkImageMemory, 
                                                     vkStagingBuffer, 
                                                     vkStagingBufferMemory);
        DestroyVkBuffer(vkStagingBuffer, vkStagingBufferMemory);
        return bRet;
    }     


    bool RHIVulkanDevice::CreateTextureFrameBufferColor(uint32_t nWidth, 
                                                        uint32_t nHeight,
                                                        uint32_t nDepth,
                                                        VkSampleCountFlagBits typeSamplesCountFlagBits, 
                                                        VkFormat typeFormat, 
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory)
    {
        return CreateVkImage(nWidth, 
                             nHeight, 
                             nDepth,
                             1,
                             1,
                             VK_IMAGE_TYPE_2D, 
                             false,
                             typeSamplesCountFlagBits, 
                             typeFormat, 
                             VK_IMAGE_TILING_OPTIMAL, 
                             VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, 
                             VK_SHARING_MODE_EXCLUSIVE,
                             false,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                             vkImage, 
                             vkImageMemory);
    }
    bool RHIVulkanDevice::CreateTextureFrameBufferDepth(uint32_t nWidth, 
                                                        uint32_t nHeight,
                                                        uint32_t nDepth,
                                                        VkSampleCountFlagBits typeSamplesCountFlagBits, 
                                                        VkFormat typeFormat, 
                                                        VkImage& vkImage, 
                                                        VkDeviceMemory& vkImageMemory)
    {
        return CreateVkImage(nWidth, 
                             nHeight, 
                             nDepth,
                             1,
                             1,
                             VK_IMAGE_TYPE_2D, 
                             false,
                             typeSamplesCountFlagBits, 
                             typeFormat, 
                             VK_IMAGE_TILING_OPTIMAL, 
                             VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
                             VK_SHARING_MODE_EXCLUSIVE,
                             false,
                             VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
                             vkImage, 
                             vkImageMemory);
    }


    //////////////////// VkShaderModule /////////////////
    bool RHIVulkanDevice::CreateVkShaderModule(RHIShaderStageBitsType eShaderStageBits, 
                                               const String& pathFile,
                                               VkShaderModule& vkShaderModule)
    {
        const String& strTypeShader = RHI_GetShaderStageBitsTypeName(eShaderStageBits);
        return CreateVkShaderModule(strTypeShader, 
                                    pathFile,
                                    vkShaderModule);
    }
    bool RHIVulkanDevice::CreateVkShaderModule(const String& strTypeShader, 
                                               const String& pathFile,
                                               VkShaderModule& vkShaderModule)
    {
        if (pathFile.empty())
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, path file is empty !");
            return false;
        }

        CharVector code;
        if (!FUtil::LoadAssetFileContent(pathFile.c_str(), code))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, path file: [%s] !", pathFile.c_str());
            return false;
        }
        if (code.size() <= 0)
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule, code size <= 0, path file: [%s] !", pathFile.c_str());
            return false;
        }

        if (!CreateVkShaderModule(code.size(),
                                  code.data(),
                                  vkShaderModule))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule: shader type: [%s], path file: [%s] !", strTypeShader.c_str(), pathFile.c_str());
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::CreateVkShaderModule(size_t nSizeByteCode,
                                               const void* pByteCode,
                                               VkShaderModule& vkShaderModule)
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = nSizeByteCode;
        createInfo.pCode = reinterpret_cast<const uint32_t*>(pByteCode);
        if (vkCreateShaderModule(this->m_vkDevice, &createInfo, nullptr, &vkShaderModule) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkShaderModule: Failed to create VkShaderModule: nSizeByteCode: [%u] !", (uint32)nSizeByteCode);
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkShaderModule(const VkShaderModule& vkShaderModule)
    {
        if (vkShaderModule != VK_NULL_HANDLE)
        {
            vkDestroyShaderModule(this->m_vkDevice, vkShaderModule, nullptr);
        }
    }


    //////////////////// VkDescriptorSetLayout //////////
    bool RHIVulkanDevice::CreateVkDescriptorSetLayout(const VkDescriptorSetLayoutBindingVector& aDescriptorSetLayoutBinding, 
                                                      VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast<uint32_t>(aDescriptorSetLayoutBinding.size());
        layoutInfo.pBindings = aDescriptorSetLayoutBinding.data();
        if (vkCreateDescriptorSetLayout(this->m_vkDevice, &layoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkDescriptorSetLayout: Failed to create VkDescriptorSetLayout !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkDescriptorSetLayout(const VkDescriptorSetLayout& vkDescriptorSetLayout)
    {
        if (vkDescriptorSetLayout != VK_NULL_HANDLE)
        {
            vkDestroyDescriptorSetLayout(this->m_vkDevice, vkDescriptorSetLayout, nullptr);
        }
    }


    //////////////////// VkPipelineLayout ///////////////
    bool RHIVulkanDevice::CreateVkPipelineLayout(const VkDescriptorSetLayoutVector& aDescriptorSetLayout,
                                                 VkPipelineLayout& vkPipelineLayout)
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(aDescriptorSetLayout.size());
        pipelineLayoutInfo.pSetLayouts = aDescriptorSetLayout.data();

        if (vkCreatePipelineLayout(this->m_vkDevice, &pipelineLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkPipelineLayout: Failed to create VkPipelineLayout !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkPipelineLayout(const VkPipelineLayout& vkPipelineLayout)
    {
        if (vkPipelineLayout != VK_NULL_HANDLE)
        {
            vkDestroyPipelineLayout(this->m_vkDevice, vkPipelineLayout, nullptr);
        }
    }


    //////////////////// VkPipelineCache ////////////////
    bool RHIVulkanDevice::CreateVkPipelineCache(VkPipelineCache& vkPipelineCache)
    {
        VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
        pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
        if (vkCreatePipelineCache(this->m_vkDevice, &pipelineCacheCreateInfo, nullptr, &vkPipelineCache) != VK_SUCCESS) 
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkPipelineCache: Failed to create VkPipelineCache !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkPipelineCache(const VkPipelineCache& vkPipelineCache)
    {
        if (vkPipelineCache != VK_NULL_HANDLE)
        {
            vkDestroyPipelineCache(this->m_vkDevice, vkPipelineCache, nullptr);
        }
    }


    //////////////////// VkPipeline /////////////////////
    bool RHIVulkanDevice::CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
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
    bool RHIVulkanDevice::CreateVkPipeline_Graphics(VkShaderModule vertShaderModule, const String& vertMain,
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
    bool RHIVulkanDevice::CreateVkPipeline_Graphics(const VkPipelineShaderStageCreateInfoVector& aShaderStageCreateInfos,
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

        if (!RHI_CheckVkResult(vkCreateGraphicsPipelines(this->m_vkDevice, vkPipelineCache, 1, &pipelineInfo, nullptr, &vkPipeline), "vkCreateGraphicsPipelines"))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkPipeline_Graphics: Failed to create VkPipeline !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::CreateVkPipeline_Compute(VkShaderModule compShaderModule,
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
    bool RHIVulkanDevice::CreateVkPipeline_Compute(const VkPipelineShaderStageCreateInfo& shaderStageCreateInfo,
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
        
        if (!RHI_CheckVkResult(vkCreateComputePipelines(this->m_vkDevice, vkPipelineCache, 1, &pipelineInfo, nullptr, &vkPipeline), "vkCreateComputePipelines"))
        {
            F_LogError("*********************** RHIVulkanDevice::CreateVkPipeline_Compute: Failed to create VkPipeline !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::DestroyVkPipeline(const VkPipeline& vkPipeline)
    {
        if (vkPipeline != VK_NULL_HANDLE)
        {
            vkDestroyPipeline(this->m_vkDevice, vkPipeline, nullptr);   
        }
    }


    //////////////////// VkDescriptorSet ////////////////
    bool RHIVulkanDevice::CreateVkDescriptorSet(uint32_t descriptorSetCount,
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkDescriptorSet: Failed to allocate VkDescriptorSet !");
            return false;
        }
        return true;
    }
    bool RHIVulkanDevice::CreateVkDescriptorSets(uint32_t countSwapChain, 
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
            F_LogError("*********************** RHIVulkanDevice::CreateVkDescriptorSets: Failed to allocate VkDescriptorSets !");
            return false;
        }
        return true;
    }
    void RHIVulkanDevice::FreeVkDescriptorSets(VkDescriptorPool vkDescriptorPool, const VkDescriptorSet& vkDescriptorSet)
    {
        if (vkDescriptorSet != VK_NULL_HANDLE)
        {
            vkFreeDescriptorSets(this->m_vkDevice, vkDescriptorPool, 1, &vkDescriptorSet);
        }
    }
    void RHIVulkanDevice::FreeVkDescriptorSets(VkDescriptorPool vkDescriptorPool, VkDescriptorSetVector& aDescriptorSets)
    {
        if (!aDescriptorSets.empty())
        {
            vkFreeDescriptorSets(this->m_vkDevice, vkDescriptorPool, (uint32_t)aDescriptorSets.size(), aDescriptorSets.data());
        }
    }

    VkDescriptorSetLayoutBinding RHIVulkanDevice::CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
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
    void RHIVulkanDevice::CreateVkDescriptorSetLayoutBinding_Uniform(uint32_t binding,
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
    VkDescriptorSetLayoutBinding RHIVulkanDevice::CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
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
    void RHIVulkanDevice::CreateVkDescriptorSetLayoutBinding_Image(uint32_t binding,
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

    void RHIVulkanDevice::PushVkDescriptorSet_Uniform(VkWriteDescriptorSetVector& aWriteDescriptorSets,
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
    void RHIVulkanDevice::PushVkDescriptorSet_Image(VkWriteDescriptorSetVector& aWriteDescriptorSets,
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
    void RHIVulkanDevice::UpdateVkDescriptorSets(VkWriteDescriptorSetVector& aWriteDescriptorSets)
    {
        if (aWriteDescriptorSets.size() > 0)
        {
            vkUpdateDescriptorSets(this->m_vkDevice, static_cast<uint32_t>(aWriteDescriptorSets.size()), aWriteDescriptorSets.data(), 0, nullptr);
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