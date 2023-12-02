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

namespace LostPeter
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
        E_FORCEINLINE VulkanFenceManager* GetFenceManager() { return m_pFenceManager; }
        E_FORCEINLINE const VkFence& GetVkFence() const { return m_vkFence; }
        E_FORCEINLINE VkFence GetVkFence() { return m_vkFence; }
        E_FORCEINLINE bool IsSignaled() const { return m_eFenceState == Vulkan_FenceState_Signaled; }
        E_FORCEINLINE VulkanFenceStateType GetFenceStateType() const { return m_eFenceState; }
        E_FORCEINLINE void SetFenceStateType(VulkanFenceStateType eFenceState) { m_eFenceState = eFenceState; }

    public:
        void Destroy();
    };

}; //LostPeter

#endif