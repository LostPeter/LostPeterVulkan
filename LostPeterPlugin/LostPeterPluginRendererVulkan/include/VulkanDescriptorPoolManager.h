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

#ifndef _VULKAN_DESCRIPTOR_POOL_MANAGER_H_
#define _VULKAN_DESCRIPTOR_POOL_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDescriptorPoolManager : public FSingleton<VulkanDescriptorPoolManager>
    {
    public:
        VulkanDescriptorPoolManager(VulkanDevice* pDevice);
        ~VulkanDescriptorPoolManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanDescriptorPoolPtrVector m_aDescriptorPool;
        VulkanDescriptorPoolPtrMap m_mapDescriptorPool;

    public:
        static VulkanDescriptorPoolManager& GetSingleton();
		static VulkanDescriptorPoolManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanDescriptorPoolPtrVector& GetDescriptorPoolPtrVector() const { return m_aDescriptorPool; }
        F_FORCEINLINE VulkanDescriptorPoolPtrVector& GetDescriptorPoolPtrVector() { return m_aDescriptorPool; }
        F_FORCEINLINE const VulkanDescriptorPoolPtrMap& GetDescriptorPoolPtrMap() const { return m_mapDescriptorPool; }
        F_FORCEINLINE VulkanDescriptorPoolPtrMap& GetDescriptorPoolPtrMap() { return m_mapDescriptorPool; }

    public:
        void Destroy();

    public:
        bool HasDescriptorPool(const String& strName);
        VulkanDescriptorPool* GetDescriptorPool(const String& strName);
        bool AddDescriptorPool(VulkanDescriptorPool* pDescriptorPool);
        VulkanDescriptorPool* CreateDescriptorPool(const String& nameDescriptorPool,
                                                   uint32_t descriptorCount);

        void DeleteDescriptorPool(const String& strName);
        void DeleteDescriptorPool(VulkanDescriptorPool* pDescriptorPool);
        void DeleteDescriptorPoolAll();

    };

}; //LostPeterPluginRendererVulkan

#endif