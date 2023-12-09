/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_FENCE_H_
#define _VULKAN_FENCE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanFence
    {
        friend class VulkanFenceManager;

    public:
        VulkanFence(VulkanFenceManager* pFenceManager, bool bCreateSignaled);
        ~VulkanFence();

    public:
    protected:
        VulkanFenceManager* m_pFenceManager;
        VkFence m_vkFence;
        VulkanFenceStateType m_eFenceState;

    public:
        F_FORCEINLINE VulkanFenceManager* GetFenceManager() { return m_pFenceManager; }
        F_FORCEINLINE const VkFence& GetVkFence() const { return m_vkFence; }
        F_FORCEINLINE VkFence GetVkFence() { return m_vkFence; }
        F_FORCEINLINE bool IsSignaled() const { return m_eFenceState == Vulkan_FenceState_Signaled; }
        F_FORCEINLINE VulkanFenceStateType GetFenceStateType() const { return m_eFenceState; }
        F_FORCEINLINE void SetFenceStateType(VulkanFenceStateType eFenceState) { m_eFenceState = eFenceState; }

    public:
        void Destroy();
    };

}; //LostPeterPluginRendererVulkan

#endif