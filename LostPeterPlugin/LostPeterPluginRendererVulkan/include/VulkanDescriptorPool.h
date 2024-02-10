/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-10
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_DESCRIPTOR_POOL_H_
#define _VULKAN_DESCRIPTOR_POOL_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDescriptorPool : public Base
                               , public FNonCopyable
    {
        friend class VulkanDescriptorPoolManager;

    private:
        VulkanDescriptorPool(const String& strNameDescriptorPool, VulkanDevice* pDevice);
    public:
        ~VulkanDescriptorPool();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkDescriptorPool m_vkDescriptorPool;

    public:
        F_FORCEINLINE VkDescriptorPool GetVkDescriptorPool() const { return m_vkDescriptorPool; }
        

    public:
        void Destroy();
        bool Init(uint32_t descriptorCount);
    };

}; //LostPeterPluginRendererVulkan

#endif