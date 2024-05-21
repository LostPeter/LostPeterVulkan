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

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBindGroupLayout : public RHIBindGroupLayout
    {
    public:
        RHIVulkanBindGroupLayout(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupLayoutCreateInfo& createInfo);
        virtual ~RHIVulkanBindGroupLayout();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkDescriptorSetLayout m_vkDescriptorSetLayout;


        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkDescriptorSetLayout& GetVkDescriptorSetLayout() { return m_vkDescriptorSetLayout; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkDescriptorSetLayout();
    };
    
}; //LostPeterPluginRHIVulkan

#endif