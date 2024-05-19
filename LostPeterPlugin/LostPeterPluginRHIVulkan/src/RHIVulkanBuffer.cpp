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

#include "../include/RHIVulkanBuffer.h"
#include "../include/RHIVulkanBufferView.h"
#include "../include/RHIVulkanDevice.h"
#include "../include/RHIVulkanConverter.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBuffer::RHIVulkanBuffer(RHIVulkanDevice* pVulkanDevice, const RHIBufferCreateInfo& createInfo)
        : RHIBuffer(pVulkanDevice, createInfo)
        , m_pVulkanDevice(pVulkanDevice)
        , m_vkBuffer(VK_NULL_HANDLE)
        , m_vmaAllocation(VK_NULL_HANDLE)
        , m_nSize(createInfo.nSize)
        , m_flagsBufferUsages(createInfo.eUsages)
        , m_strDebugName(createInfo.strDebugName)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanBuffer::RHIVulkanBuffer")

        createVkBuffer();
    }

    RHIVulkanBuffer::~RHIVulkanBuffer()
    {
        Destroy();
    }

    void RHIVulkanBuffer::Destroy()
    {
        if (m_vkBuffer != VK_NULL_HANDLE) 
        {
            vmaDestroyBuffer(m_pVulkanDevice->GetVmaAllocator(), m_vkBuffer, m_vmaAllocation);
        }
        m_vkBuffer = VK_NULL_HANDLE;
        m_vmaAllocation = VK_NULL_HANDLE;
    }

    void* RHIVulkanBuffer::Map(RHIMapType eMap, uint32 nOffset, uint32 nLength)
    {
        void* pData = nullptr;
        if (vmaMapMemory(m_pVulkanDevice->GetVmaAllocator(), m_vmaAllocation, &pData) != VK_SUCCESS)
        {
            F_LogError("*********************** RHIVulkanBuffer::Map: vmaMapMemory failed, name: [%s] !", m_strDebugName.c_str());
        }
        return pData;
    }

    void RHIVulkanBuffer::UnMap()
    {
        if (m_vmaAllocation != VK_NULL_HANDLE)
        {
            vmaUnmapMemory(m_pVulkanDevice->GetVmaAllocator(), m_vmaAllocation);
        }
    }

    RHIBufferView* RHIVulkanBuffer::CreateBufferView(const RHIBufferViewCreateInfo& createInfo)
    {
        return new RHIVulkanBufferView(this, createInfo);
    }

    void RHIVulkanBuffer::createVkBuffer()
    {
        VkBufferCreateInfo bufferInfo = {};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        bufferInfo.usage = RHIVulkanConverter::TransformToVkBufferUsageFlagsFromBufferUsageFlags(m_flagsBufferUsages);
        bufferInfo.size = m_nSize;

        VmaAllocationCreateInfo allocInfo = {};
        allocInfo.usage = VMA_MEMORY_USAGE_AUTO;
        if (m_flagsBufferUsages | RHIBufferUsageBitsType::RHI_BufferUsageBits_MapWrite) 
        {
            allocInfo.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
        }

        if (vmaCreateBuffer(m_pVulkanDevice->GetVmaAllocator(), &bufferInfo, &allocInfo, &m_vkBuffer, &m_vmaAllocation, nullptr) != VK_SUCCESS)
        {
            F_LogError("*********************** RHIVulkanBuffer::createVkBuffer: vmaCreateBuffer failed, name: [%s] !", m_strDebugName.c_str());
        }

        if (RHI_IsDebug())
        {
            if (!m_strDebugName.empty())
            {
                m_pVulkanDevice->SetDebugObject(VK_OBJECT_TYPE_BUFFER, reinterpret_cast<uint64_t>(m_vkBuffer), m_strDebugName.c_str());
            }
        }
    }
    
}; //LostPeterPluginRHIVulkan