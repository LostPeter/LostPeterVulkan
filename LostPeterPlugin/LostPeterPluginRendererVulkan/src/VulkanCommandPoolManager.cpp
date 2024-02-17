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

#include "../include/VulkanCommandPoolManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanCommandPool.h"

template<> LostPeterPluginRendererVulkan::VulkanCommandPoolManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanCommandPoolManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
     VulkanCommandPoolManager* VulkanCommandPoolManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanCommandPoolManager& VulkanCommandPoolManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanCommandPoolManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanCommandPoolManager::VulkanCommandPoolManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanCommandPoolManager::VulkanCommandPoolManager")
    }   

    VulkanCommandPoolManager::~VulkanCommandPoolManager()
    {
        Destroy();
    }

    void VulkanCommandPoolManager::Destroy()
    {
        DeleteCommandPoolAll();
    }

    bool VulkanCommandPoolManager::HasCommandPool(const String& strName)
    {
        return GetCommandPool(strName) != nullptr;
    }
    VulkanCommandPool* VulkanCommandPoolManager::GetCommandPool(const String& strName)
    {
        VulkanCommandPoolPtrMap::iterator itFind = m_mapCommandPool.find(strName);
        if (itFind == m_mapCommandPool.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanCommandPoolManager::AddCommandPool(VulkanCommandPool* pCommandPool)
    {
        const String& strName = pCommandPool->GetName();
        VulkanCommandPoolPtrMap::iterator itFind = m_mapCommandPool.find(strName);
        if (itFind != m_mapCommandPool.end())
        {
            F_LogError("*********************** VulkanCommandPoolManager::AddCommandPool: CommandPool name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapCommandPool.insert(VulkanCommandPoolPtrMap::value_type(strName, pCommandPool));
        m_aCommandPool.push_back(pCommandPool);
        return true;
    }
    VulkanCommandPool* VulkanCommandPoolManager::CreateCommandPool(const String& nameCommandPool,
                                                                   VkCommandPoolCreateFlags flags,
                                                                   uint32_t queueFamilyIndex)
    {
        VulkanCommandPool* pCommandPool = GetCommandPool(nameCommandPool);
        if (pCommandPool != nullptr)
        {
            pCommandPool->AddRef();
            return pCommandPool;
        }

        pCommandPool = new VulkanCommandPool(nameCommandPool, this->m_pDevice);
        if (!pCommandPool->Init(flags,
                                queueFamilyIndex))
        {
            F_LogError("*********************** VulkanCommandPoolManager::CreateCommandPool: Failed to init VulkanCommandPool, name: [%s] !", nameCommandPool.c_str());
            F_DELETE(pCommandPool)
            return nullptr;
        }
        AddCommandPool(pCommandPool);
        pCommandPool->AddRef();
        return pCommandPool;
    }
    void VulkanCommandPoolManager::DeleteCommandPool(const String& strName)
    {
        VulkanCommandPoolPtrMap::iterator itFind = m_mapCommandPool.find(strName);
        if (itFind == m_mapCommandPool.end())
        {
            return;
        }

        itFind->second->DelRef();
        if (itFind->second->CanDel())
        {
            VulkanCommandPoolPtrVector::iterator itFindA = std::find(m_aCommandPool.begin(), m_aCommandPool.end(), itFind->second);
            if (itFindA != m_aCommandPool.end())
                m_aCommandPool.erase(itFindA);
            F_DELETE(itFind->second)
            m_mapCommandPool.erase(itFind);
        }
    }
    void VulkanCommandPoolManager::DeleteCommandPool(VulkanCommandPool* pCommandPool)
    {
        if (!pCommandPool)
            return;
        DeleteCommandPool(pCommandPool->GetName());
    }
    void VulkanCommandPoolManager::DeleteCommandPoolAll()
    {
        for (VulkanCommandPoolPtrVector::iterator it = m_aCommandPool.begin();
             it != m_aCommandPool.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aCommandPool.clear();
        m_mapCommandPool.clear();
    }

}; //LostPeterPluginRendererVulkan