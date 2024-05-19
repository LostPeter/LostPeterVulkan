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

#ifndef _RHI_VULKAN_BUFFER_H_
#define _RHI_VULKAN_BUFFER_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBuffer : public RHIBuffer
    {
    public:
        RHIVulkanBuffer(RHIVulkanDevice* pVulkanDevice, const RHIBufferCreateInfo& createInfo);
        virtual ~RHIVulkanBuffer();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkBuffer m_vkBuffer;
        VmaAllocation m_vmaAllocation;
        RHIVulkanBufferView* m_pVulkanBufferView;

        uint32 m_nSize;
        RHIBufferUsageFlags m_flagsBufferUsages;
        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkBuffer& GetVkBuffer() { return m_vkBuffer; }
        F_FORCEINLINE VmaAllocation& GetVmaAllocation() { return m_vmaAllocation; }
        F_FORCEINLINE RHIVulkanBufferView* GetVulkanBufferView() { return m_pVulkanBufferView; }
        
        F_FORCEINLINE uint32 GetSize() { return m_nSize; }
        F_FORCEINLINE const RHIBufferUsageFlags& GetBufferUsageFlags() { return m_flagsBufferUsages; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();
        virtual void DestroyBufferView();
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewCreateInfo& createInfo);

        virtual void* Map(RHIMapType eMap, uint32 nOffset, uint32 nLength);
        virtual void UnMap();

    protected:
        void createVkBuffer();
    };
    
}; //LostPeterPluginRHIVulkan

#endif