/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_FENCE_H_
#define _VULKAN_FENCE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanFence
    {
        friend class VulkanFenceManager;

    public:
        VulkanFence(VulkanDevice* pDevice, VulkanFenceManager* pFenceManager, bool bCreateSignaled);
        ~VulkanFence();

    public:
    protected:
        VkFence m_vkFence;
        VulkanFenceStateType m_eFenceState;
        VulkanFenceManager* m_pFenceManager;

    public:
        UTIL_FORCEINLINE const VkFence& GetVkFence() const { return m_vkFence; }
        UTIL_FORCEINLINE VkFence GetVkFence() { return m_vkFence; }
        UTIL_FORCEINLINE bool IsSignaled() const { return m_eFenceState == Vulkan_FenceState_Signaled; }
        UTIL_FORCEINLINE VulkanFenceStateType GetFenceStateType() const { return m_eFenceState; }
        UTIL_FORCEINLINE void SetFenceStateType(VulkanFenceStateType eFenceState) { m_eFenceState = eFenceState; }
        UTIL_FORCEINLINE VulkanFenceManager* GetFenceManager() { return m_pFenceManager; }

    public:
        void Destroy();
        
    };

}; //LostPeter

#endif