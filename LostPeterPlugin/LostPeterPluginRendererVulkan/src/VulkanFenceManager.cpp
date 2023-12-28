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

#include "../include/VulkanFenceManager.h"
#include "../include/VulkanDevice.h"

template<> LostPeterPluginRendererVulkan::VulkanFenceManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanFenceManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanFenceManager* VulkanFenceManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanFenceManager& VulkanFenceManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanFenceManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanFenceManager::VulkanFenceManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanFenceManager::VulkanFenceManager")
    }   

    VulkanFenceManager::~VulkanFenceManager()
    {
        Destroy();
    }

    void VulkanFenceManager::Destroy()
    {
        if (m_aFences_Used.size() > 0) 
        {
		    F_LogError("VulkanFenceManager::Destroy: Not all fences are done !");
        }

        size_t count = m_aFences_Free.size();
        for (size_t i = 0; i < count; ++i) 
        {
            DestoryFence(m_aFences_Free[i]);
        }
        m_aFences_Free.clear();
        m_aFences_Used.clear();
    }

    VulkanFence* VulkanFenceManager::CreateFence(bool bCreateSignaled /*= false*/)
    {
        if (m_aFences_Free.size() > 0)
        {
            VulkanFence* pFence = m_aFences_Free.back();
            m_aFences_Free.pop_back();
            m_aFences_Used.push_back(pFence);
            if (bCreateSignaled) 
            {
                pFence->SetFenceStateType(Vulkan_FenceState_Signaled);
            }
            return pFence;
        }

        VulkanFence* pFenceNew = new VulkanFence(this, bCreateSignaled);
        m_aFences_Used.push_back(pFenceNew);
        return pFenceNew;
    }

    void VulkanFenceManager::DestoryFence(VulkanFence* pFence)
    {   
        F_DELETE(pFence)
    }

    bool VulkanFenceManager::WaitForFence(VulkanFence* pFence, uint64 timeInNanoseconds)
    {
        VkResult result = vkWaitForFences(m_pDevice->GetVkDevice(), 1, &pFence->m_vkFence, true, timeInNanoseconds);
        switch ((int)result)
        {
        case VK_SUCCESS:
            pFence->SetFenceStateType(Vulkan_FenceState_Signaled);
            return true;
        case VK_TIMEOUT:
            return false;
        }
        F_LogError("VulkanFenceManager::WaitForFence: Unkow error: [%d]", (int32)result);
        return false;
    }

    void VulkanFenceManager::ResetFence(VulkanFence* pFence)
    {
        if (pFence->GetFenceStateType() != Vulkan_FenceState_NotReady)
        {
            vkResetFences(m_pDevice->GetVkDevice(), 1, &pFence->m_vkFence);
            pFence->SetFenceStateType(Vulkan_FenceState_NotReady);
        }
    }

    void VulkanFenceManager::RecoveryFence(VulkanFence*& pFence)
    {
        ResetFence(pFence);

        size_t count = m_aFences_Used.size();
        for (size_t i = 0; i < count; ++i) 
        {
            if (m_aFences_Used[i] == pFence)
            {
                m_aFences_Used.erase(m_aFences_Used.begin() + i);
                break;
            }
        }
        m_aFences_Free.push_back(pFence);
        pFence = nullptr;
    }

    void VulkanFenceManager::WaitAndRecoveryFence(VulkanFence*& pFence, uint64 timeInNanoseconds)
    {
        if (!pFence->IsSignaled()) 
        {
            WaitForFence(pFence, timeInNanoseconds);
        }
        RecoveryFence(pFence);
    }

    bool VulkanFenceManager::checkFenceState(VulkanFence* pFence)
    {
        VkResult result = vkGetFenceStatus(m_pDevice->GetVkDevice(), pFence->GetVkFence());
        switch (result)
        {
        case VK_SUCCESS:
            pFence->SetFenceStateType(Vulkan_FenceState_Signaled);
            return true;
        case VK_NOT_READY:
            break;
        default:
            break;
        }
        return false;
    }

}; //LostPeterPluginRendererVulkan