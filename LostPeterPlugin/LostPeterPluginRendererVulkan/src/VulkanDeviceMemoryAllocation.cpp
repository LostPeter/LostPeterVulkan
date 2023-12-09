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

#include "../include/VulkanDeviceMemoryAllocation.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanDeviceMemoryAllocation::VulkanDeviceMemoryAllocation()
        : m_pDevice(nullptr)
        , m_nSize(0) 
        , m_vkDeviceMemory(VK_NULL_HANDLE)
        , m_pMappedPointer(nullptr)
        , m_nMemoryTypeIndex(0)
        , m_bCanBeMapped(false)
        , m_bIsCoherent(false)
        , m_bIsCached(false)
        , m_bIsFreedBySystem(false)
    {
        
    }

    VulkanDeviceMemoryAllocation::~VulkanDeviceMemoryAllocation()
    {
        
    }

    void VulkanDeviceMemoryAllocation::Destroy()
    {

    }

    bool VulkanDeviceMemoryAllocation::Init(VulkanDevice* pDevice)
    {
        m_pDevice = pDevice;
        F_Assert(m_pDevice && "VulkanDeviceMemoryAllocation::Init")


        return true;
    }

    void* VulkanDeviceMemoryAllocation::Map(VkDeviceSize size, VkDeviceSize offset)
    {
        vkMapMemory(m_pDevice->GetVkDevice(), m_vkDeviceMemory, offset, size, 0, &m_pMappedPointer);
	    return m_pMappedPointer;
    }

    void VulkanDeviceMemoryAllocation::Unmap()
    {
        vkUnmapMemory(m_pDevice->GetVkDevice(), m_vkDeviceMemory);
    }

    void VulkanDeviceMemoryAllocation::FlushMappedMemory(VkDeviceSize offset, VkDeviceSize size)
    {
        if (!IsCoherent())
        {
            VkMappedMemoryRange range;
            E_ZeroStruct(range, VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE);
            range.memory = m_vkDeviceMemory;
            range.offset = offset;
            range.size   = size;
            vkFlushMappedMemoryRanges(m_pDevice->GetVkDevice(), 1, &range);
        }
    }

    void VulkanDeviceMemoryAllocation::InvalidateMappedMemory(VkDeviceSize offset, VkDeviceSize size)
    {
        if (!IsCoherent())
        {
            VkMappedMemoryRange range;
            E_ZeroStruct(range, VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE);
            range.memory = m_vkDeviceMemory;
            range.offset = offset;
            range.size   = size;
            vkInvalidateMappedMemoryRanges(m_pDevice->GetVkDevice(), 1, &range);
        }
    }

}; //LostPeterPluginRendererVulkan