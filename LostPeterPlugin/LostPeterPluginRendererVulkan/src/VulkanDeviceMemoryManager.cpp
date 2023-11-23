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

#include "../include/VulkanDeviceMemoryManager.h"
#include "../include/VulkanDevice.h"

namespace LostPeter
{
    VulkanDeviceMemoryManager::VulkanDeviceMemoryManager()
        : m_pDevice(nullptr)
        , m_bHasUnifiedMemory(false)
        , m_nNumAllocations(0)
        , m_nNumPeakAllocations(0)
    {
        memset(&m_vkPhysicalDeviceMemoryProperties, 0, sizeof(VkPhysicalDeviceMemoryProperties));
    }

    VulkanDeviceMemoryManager::~VulkanDeviceMemoryManager()
    {
        Destroy();
    }

    void VulkanDeviceMemoryManager::Destroy()
    {
        size_t count = m_aHeapInfos.size();
        for (size_t i = 0; i < count; ++i)
        {
            if (m_aHeapInfos[i].allocations.size() > 0)
            {
                F_LogInfo("VulkanDeviceMemoryManager::Destroy: Found [%d] freed allocations !", (int32)m_aHeapInfos[i].allocations.size());

                DumpMemory();
            }
        }
        m_nNumAllocations = 0;
    }

    bool VulkanDeviceMemoryManager::Init(VulkanDevice* pDevice)
    {
        m_pDevice = pDevice;
        F_Assert(m_pDevice && "VulkanDeviceMemoryAllocation::Init")

        m_nNumAllocations = 0;
        m_nNumPeakAllocations = 0;

        vkGetPhysicalDeviceMemoryProperties(m_pDevice->GetVkPhysicalDevice(), &m_vkPhysicalDeviceMemoryProperties);
        m_aHeapInfos.resize(m_vkPhysicalDeviceMemoryProperties.memoryHeapCount);

        setupAndPrintMemInfo();

        return true;
    }

    uint64 VulkanDeviceMemoryManager::GetTotalMemory(bool gpu) const
    {
        uint64 totalMemory = 0;
        for (uint32 i = 0; i < m_vkPhysicalDeviceMemoryProperties.memoryHeapCount; ++i)
        {
            const bool isGPUHeap = ((m_vkPhysicalDeviceMemoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT);
            if (isGPUHeap == gpu) 
            {
                totalMemory += m_aHeapInfos[i].totalSize;
            }
        }
        return totalMemory;
    }

    bool VulkanDeviceMemoryManager::SupportsMemoryType(VkMemoryPropertyFlags properties) const
    {
        for (uint32 i = 0; i < m_vkPhysicalDeviceMemoryProperties.memoryTypeCount; ++i)
        {
            if (m_vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags == properties) 
            {
                return true;
            }
        }
        return false;
    }

    VulkanDeviceMemoryAllocation* VulkanDeviceMemoryManager::Alloc(bool canFail, 
                                                                   VkDeviceSize allocationSize, 
                                                                   uint32 memoryTypeIndex, 
                                                                   void* dedicatedAllocateInfo, 
                                                                   const char* file,
                                                                   uint32 line)
    {
        VkMemoryAllocateInfo allocInfo;
        Util_ZeroStruct(allocInfo, VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO);
        allocInfo.allocationSize  = allocationSize;
        allocInfo.memoryTypeIndex = memoryTypeIndex;
        allocInfo.pNext           = dedicatedAllocateInfo;
        
        VulkanDeviceMemoryAllocation* newAllocation = new VulkanDeviceMemoryAllocation();
        newAllocation->Init(m_pDevice);
        newAllocation->m_nSize            = allocationSize;
        newAllocation->m_nMemoryTypeIndex = memoryTypeIndex;
        newAllocation->m_bCanBeMapped     = ((m_vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)  == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        newAllocation->m_bIsCoherent      = ((m_vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
        newAllocation->m_bIsCached        = ((m_vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT)   == VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
        
        VkResult result = vkAllocateMemory(m_pDevice->GetVkDevice(), &allocInfo, nullptr, &newAllocation->m_vkDeviceMemory);
        if (result == VK_ERROR_OUT_OF_DEVICE_MEMORY)
        {
            if (canFail)
            {
                F_LogError("*********************** VulkanDeviceMemoryManager::Alloc: Failed to allocate Device Memory, Requested: [%fKb], MemTypeIndex: [%d]", (float)allocInfo.allocationSize / 1024.0f, allocInfo.memoryTypeIndex);
                return nullptr;
            }
            F_LogInfo("VulkanDeviceMemoryManager::Alloc: Out of Device Memory, Requested: [%fKb], MemTypeIndex: [%d]", (float)allocInfo.allocationSize / 1024.0f, allocInfo.memoryTypeIndex);

            DumpMemory();
        }
        else if (result == VK_ERROR_OUT_OF_HOST_MEMORY)
        {
            if (canFail)
            {
                F_LogError("*********************** VulkanDeviceMemoryManager::Alloc: Failed to allocate Host Memory, Requested: [%fKb], MemTypeIndex: [%d]", (float)allocInfo.allocationSize / 1024.0f, allocInfo.memoryTypeIndex);
                return nullptr;
            }
            F_LogInfo("VulkanDeviceMemoryManager::Alloc: Out of Host Memory, Requested: [%fKb], MemTypeIndex: [%d]", (float)allocInfo.allocationSize / 1024.0f, allocInfo.memoryTypeIndex);
  
            DumpMemory();
        }
        else
        {
            Util_CheckVkResult(result, "vkAllocateMemory");
        }
        
        m_nNumAllocations     += 1;
        m_nNumPeakAllocations = FMath::Max(m_nNumAllocations, m_nNumPeakAllocations);
        if (m_nNumAllocations == m_pDevice->GetVkPhysicalDeviceLimits().maxMemoryAllocationCount) 
        {
            F_LogInfo("VulkanDeviceMemoryManager::Alloc: Hit Maximum # of allocations: [%d] reported by device !", m_nNumAllocations);
        }
        
        uint32 heapIndex = m_vkPhysicalDeviceMemoryProperties.memoryTypes[memoryTypeIndex].heapIndex;
        m_aHeapInfos[heapIndex].allocations.push_back(newAllocation);
        m_aHeapInfos[heapIndex].usedSize += allocationSize;
        m_aHeapInfos[heapIndex].peakSize = FMath::Max(m_aHeapInfos[heapIndex].peakSize, m_aHeapInfos[heapIndex].usedSize);
        
        return newAllocation;
    }

    void VulkanDeviceMemoryManager::Free(VulkanDeviceMemoryAllocation*& allocation)
    {
        m_nNumAllocations -= 1;

        vkFreeMemory(m_pDevice->GetVkDevice(), allocation->m_vkDeviceMemory, nullptr);
        uint32 heapIndex = m_vkPhysicalDeviceMemoryProperties.memoryTypes[allocation->m_nMemoryTypeIndex].heapIndex;
        m_aHeapInfos[heapIndex].usedSize -= allocation->m_nSize;
        
        auto it = std::find(m_aHeapInfos[heapIndex].allocations.begin(), m_aHeapInfos[heapIndex].allocations.end(), allocation);
        if (it != m_aHeapInfos[heapIndex].allocations.end()) 
        {
            m_aHeapInfos[heapIndex].allocations.erase(it);
        }
        
        allocation->m_bIsFreedBySystem = true;
        delete allocation;
        allocation = nullptr;
    }

    void VulkanDeviceMemoryManager::DumpMemory()
    {
        setupAndPrintMemInfo();

        int32 count = (int32)m_aHeapInfos.size();
        F_LogInfo("VulkanDeviceMemoryManager::DumpMemory: Device Memory: [%d], allocations on [%lu] heaps", m_nNumAllocations, (uint32)count);
        for (int32 index = 0; index < count; ++index)
        {
            HeapInfo& heapInfo = m_aHeapInfos[index];
            F_LogInfo("*********** VulkanDeviceMemoryManager::DumpMemory: \tHeap: [%d], [%lu] allocations", index, (uint32)heapInfo.allocations.size());
            uint64 totalSize = 0;
            for (int32 subIndex = 0; subIndex < heapInfo.allocations.size(); ++subIndex)
            {
                VulkanDeviceMemoryAllocation* allocation = heapInfo.allocations[subIndex];
                F_LogInfo("VulkanDeviceMemoryManager::DumpMemory: \t\t[%d], Size [%llu], Handle [%p]", subIndex, (uint64)allocation->m_nSize, (void*)allocation->m_vkDeviceMemory);
                totalSize += allocation->m_nSize;
            }
            F_LogInfo("*********** VulkanDeviceMemoryManager::DumpMemory: \t\tTotal Allocated: [%.2f MB], Peak: [%.2f MB]", totalSize / 1024.0f / 1024.0f, heapInfo.peakSize / 1024.0f / 1024.0f);
        }
    }

    void VulkanDeviceMemoryManager::setupAndPrintMemInfo()
    {
        const uint32 maxAllocations = m_pDevice->GetVkPhysicalDeviceLimits().maxMemoryAllocationCount;
        F_LogInfo("VulkanDeviceMemoryManager::setupAndPrintMemInfo: [%d] Device Memory Heaps, Max memory allocations [%d]", m_vkPhysicalDeviceMemoryProperties.memoryHeapCount, maxAllocations);
        for (uint32 index = 0; index < m_vkPhysicalDeviceMemoryProperties.memoryHeapCount; ++index)
        {
            bool isGPUHeap = ((m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT);
            F_LogInfo("VulkanDeviceMemoryManager::setupAndPrintMemInfo: [%d]: Flags: [0x%x], Size: [%llu] [%.2f MB] [%s]",
                index,
                m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].flags,
                (uint64)(m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].size),
                (float)((double)m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].size / 1024.0 / 1024.0),
                isGPUHeap ? "GPU" : ""
            );
            m_aHeapInfos[index].totalSize = m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].size;
        }
        
        m_bHasUnifiedMemory = false;
        F_LogInfo("VulkanDeviceMemoryManager::setupAndPrintMemInfo: [%d] Device Memory Types: [%sunified]", m_vkPhysicalDeviceMemoryProperties.memoryTypeCount, m_bHasUnifiedMemory ? "" : "Not");
        for (uint32 index = 0; index < m_vkPhysicalDeviceMemoryProperties.memoryTypeCount; ++index)
        {
            auto GetFlagsString = [](VkMemoryPropertyFlags flags)
            {
                String str;
                if ((flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
                    str += " Local";
                }
                if ((flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
                    str += " HostVisible";
                }
                if ((flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
                    str += " HostCoherent";
                }
                if ((flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) == VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
                    str += " HostCached";
                }
                if ((flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) == VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
                    str += " Lazy";
                }
                return str;
            };
            
            F_LogInfo("VulkanDeviceMemoryManager::setupAndPrintMemInfo: [%d]: Flags: [0x%x], Heap: [%d] [%s]",
                index,
                m_vkPhysicalDeviceMemoryProperties.memoryTypes[index].propertyFlags,
                m_vkPhysicalDeviceMemoryProperties.memoryTypes[index].heapIndex,
                GetFlagsString(m_vkPhysicalDeviceMemoryProperties.memoryTypes[index].propertyFlags).c_str()
            );
        }
        
        for (uint32 index = 0; index < m_vkPhysicalDeviceMemoryProperties.memoryHeapCount; ++index)
        {
            bool isGPUHeap = ((m_vkPhysicalDeviceMemoryProperties.memoryHeaps[index].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) == VK_MEMORY_HEAP_DEVICE_LOCAL_BIT);
            if (isGPUHeap) {
                m_aHeapInfos[index].totalSize = (uint64)((float)m_aHeapInfos[index].totalSize * 0.95f);
            }
        }
    }

}; //LostPeter