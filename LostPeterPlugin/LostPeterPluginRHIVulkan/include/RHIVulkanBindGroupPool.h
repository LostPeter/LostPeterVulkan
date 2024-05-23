/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_BIND_GROUP_POOL_H_
#define _RHI_VULKAN_BIND_GROUP_POOL_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBindGroupPool : public RHIBindGroupPool
                                                 , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanBindGroupPool(RHIVulkanDevice* pVulkanDevice, const RHIBindGroupPoolCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanBindGroupPool();

    public:
    protected:
        VkDescriptorPool m_vkDescriptorPool;

        uint32 m_nDescriptorCount;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkDescriptorPool& GetVkDescriptorPool() { return m_vkDescriptorPool; }

        F_FORCEINLINE uint32 GetDescriptorCount() const { return m_nDescriptorCount; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();


    protected:
        void createVkDescriptorPool();
    };
    
}; //LostPeterPluginRHIVulkan

#endif