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

#ifndef _RHI_VULKAN_BUFFER_VIEW_H_
#define _RHI_VULKAN_BUFFER_VIEW_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBufferView : public RHIBufferView
    {
    public:
        RHIVulkanBufferView(RHIVulkanBuffer* pVulkanBuffer, const RHIBufferViewCreateInfo& createInfo);
        virtual ~RHIVulkanBufferView();

    public:
    protected:
        RHIVulkanBuffer* m_pVulkanBuffer;
        VkDescriptorBufferInfo m_vkDescriptorBufferInfo;

        uint32 m_nStride;
        uint32 m_nSize;
        uint32 m_nOffset;
        RHIIndexFormatType m_eIndexFormat;

    public:
        F_FORCEINLINE RHIVulkanBuffer* GetVulkanBuffer() const { return m_pVulkanBuffer; }
        F_FORCEINLINE const VkDescriptorBufferInfo& GetVkDescriptorBufferInfo() const { return m_vkDescriptorBufferInfo; }
        F_FORCEINLINE VkDescriptorBufferInfo& GetVkDescriptorBufferInfo() { return m_vkDescriptorBufferInfo; }

        F_FORCEINLINE uint32 GetStride() const { return m_nStride; }
        F_FORCEINLINE uint32 GetSize() const { return m_nSize; }
        F_FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
        F_FORCEINLINE RHIIndexFormatType GetIndexFormatType() const { return m_eIndexFormat; }

    public:
        virtual void Destroy();


    protected:
        void createVkDescriptorBufferInfo();
    };
    
}; //LostPeterPluginRHIVulkan

#endif