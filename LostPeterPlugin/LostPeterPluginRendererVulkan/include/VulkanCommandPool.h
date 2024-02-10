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

#ifndef _VULKAN_COMMAND_POOL_H_
#define _VULKAN_COMMAND_POOL_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanCommandPool : public Base
                            , public FNonCopyable
    {
        friend class VulkanCommandPoolManager;

    private:
        VulkanCommandPool(const String& strNameCommandPool, VulkanDevice* pDevice);
    public:
        ~VulkanCommandPool();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkCommandPool m_vkCommandPool;

    public:
        F_FORCEINLINE VkCommandPool GetVkCommandPool() const { return m_vkCommandPool; }
        

    public:
        void Destroy();
        bool Init(VkCommandPoolCreateFlags flags,
                  uint32_t queueFamilyIndex);
    };

}; //LostPeterPluginRendererVulkan

#endif