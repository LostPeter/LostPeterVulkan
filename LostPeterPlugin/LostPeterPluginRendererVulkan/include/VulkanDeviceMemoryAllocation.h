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

#ifndef _VULKAN_DEVICE_MEMORY_ALLOCATION_H_
#define _VULKAN_DEVICE_MEMORY_ALLOCATION_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDeviceMemoryAllocation
    {
        friend class VulkanDeviceMemoryManager;

    public:
        VulkanDeviceMemoryAllocation();
    protected:
        virtual ~VulkanDeviceMemoryAllocation();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkDeviceSize m_nSize;
        VkDeviceMemory m_vkDeviceMemory;
        void* m_pMappedPointer;
        uint32 m_nMemoryTypeIndex;
        bool m_bCanBeMapped;
        bool m_bIsCoherent;
        bool m_bIsCached;
        bool m_bIsFreedBySystem;

    public:
        F_FORCEINLINE VkDeviceSize GetSize() const { return m_nSize; }
        F_FORCEINLINE VkDeviceMemory GetVkDeviceMemory() const { return m_vkDeviceMemory; }
        F_FORCEINLINE void* GetMappedPointer() { return m_pMappedPointer; }
        F_FORCEINLINE bool IsMapped() const { return m_pMappedPointer != nullptr; }
        F_FORCEINLINE uint32 GetMemoryTypeIndex() const { return m_nMemoryTypeIndex; }
        F_FORCEINLINE bool CanBeMapped() const { return m_bCanBeMapped; }
        F_FORCEINLINE bool IsCoherent() const { return m_bIsCoherent; }
        
    public:
        void Destroy();
        bool Init(VulkanDevice* pDevice);

        void* Map(VkDeviceSize size, VkDeviceSize offset);
	    void Unmap();

	    void FlushMappedMemory(VkDeviceSize offset, VkDeviceSize size);
	    void InvalidateMappedMemory(VkDeviceSize offset, VkDeviceSize size);
    };

}; //LostPeterPluginRendererVulkan

#endif