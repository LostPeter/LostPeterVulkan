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

#ifndef _VULKAN_FENCE_MANAGER_H_
#define _VULKAN_FENCE_MANAGER_H_

#include "VulkanFence.h"

namespace LostPeter
{
    class utilExport VulkanFenceManager
    {
    public:
        VulkanFenceManager();
        ~VulkanFenceManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanFencePtrVector m_aFences_Free;
        VulkanFencePtrVector m_aFences_Used;

    public:
        UTIL_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

        UTIL_FORCEINLINE bool IsFenceSignaled(VulkanFence* pFence)
        {
            if (pFence->IsSignaled()) {
                return true;
            }
            return checkFenceState(pFence);
        }

    public:
        void Destroy();
        bool Init(VulkanDevice* pDevice);
        
    public:
        VulkanFence* CreateFence(bool bCreateSignaled = false);
        void DestoryFence(VulkanFence* pFence);

        bool WaitForFence(VulkanFence* pFence, uint64 timeInNanoseconds);
        void ResetFence(VulkanFence* pFence);

        void RecoveryFence(VulkanFence*& pFence);
        void WaitAndRecoveryFence(VulkanFence*& pFence, uint64 timeInNanoseconds);

    protected:
        bool checkFenceState(VulkanFence* pFence);
    };

}; //LostPeter

#endif