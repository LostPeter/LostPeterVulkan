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

#ifndef _VULKAN_COMMAND_POOL_MANAGER_H_
#define _VULKAN_COMMAND_POOL_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanCommandPoolManager : public FSingleton<VulkanCommandPoolManager>
    {
    public:
        VulkanCommandPoolManager(VulkanDevice* pDevice);
        ~VulkanCommandPoolManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanCommandPoolPtrVector m_aCommandPool;
        VulkanCommandPoolPtrMap m_mapCommandPool;

    public:
        static VulkanCommandPoolManager& GetSingleton();
		static VulkanCommandPoolManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanCommandPoolPtrVector& GetCommandPoolPtrVector() const { return m_aCommandPool; }
        F_FORCEINLINE VulkanCommandPoolPtrVector& GetCommandPoolPtrVector() { return m_aCommandPool; }
        F_FORCEINLINE const VulkanCommandPoolPtrMap& GetCommandPoolPtrMap() const { return m_mapCommandPool; }
        F_FORCEINLINE VulkanCommandPoolPtrMap& GetCommandPoolPtrMap() { return m_mapCommandPool; }

    public:
        void Destroy();

    public:
        bool HasCommandPool(const String& strName);
        VulkanCommandPool* GetCommandPool(const String& strName);
        bool AddCommandPool(VulkanCommandPool* pCommandPool);
        VulkanCommandPool* CreateCommandPool(const String& nameCommandPool,
                                             VkCommandPoolCreateFlags flags,
                                             uint32_t queueFamilyIndex);

        void DeleteCommandPool(const String& strName);
        void DeleteCommandPool(VulkanCommandPool* pCommandPool);
        void DeleteCommandPoolAll();

    };

}; //LostPeterPluginRendererVulkan

#endif