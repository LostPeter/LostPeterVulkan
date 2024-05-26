/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_BIND_GROUP_LAYOUT_H_
#define _RHI_VULKAN_BIND_GROUP_LAYOUT_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBindGroupLayout : public RHIBindGroupLayout
                                                   , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanBindGroupLayout(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupLayoutCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanBindGroupLayout();

    public:
    protected:
        VkDescriptorSetLayout m_vkDescriptorSetLayout;

        uint8 m_nLayoutIndex;
        RHIBindGroupLayoutEntryVector m_aBindGroupLayoutEntries;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkDescriptorSetLayout& GetVkDescriptorSetLayout() { return m_vkDescriptorSetLayout; }

        F_FORCEINLINE uint8 GetLayoutIndex() const { return m_nLayoutIndex; }
        F_FORCEINLINE const RHIBindGroupLayoutEntryVector& GetBindGroupLayoutEntries() const { return m_aBindGroupLayoutEntries; }
        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkDescriptorSetLayout();
    };
    
}; //LostPeterPluginRHIVulkan

#endif