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

#ifndef _RHI_VULKAN_BIND_GROUP_H_
#define _RHI_VULKAN_BIND_GROUP_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBindGroup : public RHIBindGroup
    {
    public:
        RHIVulkanBindGroup(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupCreateInfo& createInfo);
        virtual ~RHIVulkanBindGroup();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkDescriptorSet m_vkDescriptorSet;


        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkDescriptorSet& GetVkDescriptorSet() { return m_vkDescriptorSet; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();


    protected:
        void createVkDescriptorSet();
    };
    
}; //LostPeterPluginRHIVulkan

#endif