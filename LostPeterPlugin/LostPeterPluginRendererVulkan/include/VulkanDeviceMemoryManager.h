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

#ifndef _VULKAN_DEVICE_MEMORY_MANAGER_H_
#define _VULKAN_DEVICE_MEMORY_MANAGER_H_

#include "VulkanPreDefine.h"
#include "VulkanDeviceMemoryAllocation.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDeviceMemoryManager
    {
    public:
        VulkanDeviceMemoryManager();
        virtual ~VulkanDeviceMemoryManager();

    public:
    protected:
        struct HeapInfo
        {
            HeapInfo()
                : totalSize(0)
                , usedSize(0)
                , peakSize(0)
            {
                
            }
            
            VkDeviceSize totalSize;
            VkDeviceSize usedSize;
            VkDeviceSize peakSize;
            VulkanDeviceMemoryAllocationPtrVector allocations;
        };
        typedef std::vector<HeapInfo> HeapInfoVector;

    protected:
        VulkanDevice* m_pDevice;
        VkPhysicalDeviceMemoryProperties m_vkPhysicalDeviceMemoryProperties;
        bool m_bHasUnifiedMemory;
        uint32 m_nNumAllocations;
        uint32 m_nNumPeakAllocations;
        HeapInfoVector m_aHeapInfos;

    public: 
        F_FORCEINLINE const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return m_vkPhysicalDeviceMemoryProperties; }
        F_FORCEINLINE bool HasUnifiedMemory() const { return m_bHasUnifiedMemory; }
        F_FORCEINLINE uint32 GetNumMemoryTypes() const { return m_vkPhysicalDeviceMemoryProperties.memoryTypeCount; }
        
        F_FORCEINLINE VkResult GetMemoryTypeFromProperties(uint32 typeBits, VkMemoryPropertyFlags properties, uint32* outTypeIndex)
        {
            for (uint32 i = 0; i < m_vkPhysicalDeviceMemoryProperties.memoryTypeCount && typeBits; ++i)
            {
                if ((typeBits & 1) == 1)
                {
                    if ((m_vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
                    {
                        *outTypeIndex = i;
                        return VK_SUCCESS;
                    }
                }
                typeBits >>= 1;
            }
            
            return VK_ERROR_FEATURE_NOT_PRESENT;
        }
        
        F_FORCEINLINE VkResult GetMemoryTypeFromPropertiesExcluding(uint32 typeBits, VkMemoryPropertyFlags properties, uint32 excludeTypeIndex, uint32* outTypeIndex)
        {
            for (uint32 i = 0; i < m_vkPhysicalDeviceMemoryProperties.memoryTypeCount && typeBits; ++i)
            {
                if ((typeBits & 1) == 1)
                {
                    if ((m_vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties && excludeTypeIndex != i)
                    {
                        *outTypeIndex = i;
                        return VK_SUCCESS;
                    }
                }
                typeBits >>= 1;
            }
            
            return VK_ERROR_FEATURE_NOT_PRESENT;
        }
        
        F_FORCEINLINE VulkanDeviceMemoryAllocation* Alloc(bool canFail, VkDeviceSize allocationSize, uint32 memoryTypeBits, VkMemoryPropertyFlags memoryPropertyFlags, void* dedicatedAllocateInfo, const char* file, uint32 line)
        {
            uint32 memoryTypeIndex = ~0;
            E_VK_CHECK(this->GetMemoryTypeFromProperties(memoryTypeBits, memoryPropertyFlags, &memoryTypeIndex))
            return Alloc(canFail, allocationSize, memoryTypeIndex, dedicatedAllocateInfo, file, line);
        }

    public:
        void Destroy();
        bool Init(VulkanDevice* pDevice);

        uint64 GetTotalMemory(bool gpu) const;

        bool SupportsMemoryType(VkMemoryPropertyFlags properties) const;
    
        VulkanDeviceMemoryAllocation* Alloc(bool canFail, 
                                            VkDeviceSize allocationSize, 
                                            uint32 memoryType, 
                                            void* dedicatedAllocateInfo, 
                                            const char* file, 
                                            uint32 line);
        void Free(VulkanDeviceMemoryAllocation*& allocation);
        
        void DumpMemory();

    protected:
        void setupAndPrintMemInfo();
    };

}; //LostPeterPluginRendererVulkan

#endif