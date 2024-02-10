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

#include "../include/VulkanDescriptorPoolManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanDescriptorPool.h"

template<> LostPeterPluginRendererVulkan::VulkanDescriptorPoolManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanDescriptorPoolManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanDescriptorPoolManager* VulkanDescriptorPoolManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanDescriptorPoolManager& VulkanDescriptorPoolManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanDescriptorPoolManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanDescriptorPoolManager::VulkanDescriptorPoolManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanDescriptorPoolManager::VulkanDescriptorPoolManager")
    }   

    VulkanDescriptorPoolManager::~VulkanDescriptorPoolManager()
    {
        Destroy();
    }

    void VulkanDescriptorPoolManager::Destroy()
    {
        DeleteDescriptorPoolAll();
    }

    bool VulkanDescriptorPoolManager::HasDescriptorPool(const String& strName)
    {
        return GetDescriptorPool(strName) != nullptr;
    }
    VulkanDescriptorPool* VulkanDescriptorPoolManager::GetDescriptorPool(const String& strName)
    {
        VulkanDescriptorPoolPtrMap::iterator itFind = m_mapDescriptorPool.find(strName);
        if (itFind == m_mapDescriptorPool.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanDescriptorPoolManager::AddDescriptorPool(VulkanDescriptorPool* pDescriptorPool)
    {
        const String& strName = pDescriptorPool->GetName();
        VulkanDescriptorPoolPtrMap::iterator itFind = m_mapDescriptorPool.find(strName);
        if (itFind != m_mapDescriptorPool.end())
        {
            F_LogError("*********************** VulkanDescriptorPoolManager::AddDescriptorPool: DescriptorPool name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapDescriptorPool.insert(VulkanDescriptorPoolPtrMap::value_type(strName, pDescriptorPool));
        m_aDescriptorPool.push_back(pDescriptorPool);
        return true;
    }
    VulkanDescriptorPool* VulkanDescriptorPoolManager::CreateDescriptorPool(const String& nameDescriptorPool,
                                                                            uint32_t descriptorCount)
    {
        VulkanDescriptorPool* pDescriptorPool = GetDescriptorPool(nameDescriptorPool);
        if (pDescriptorPool != nullptr)
            return pDescriptorPool;
        
        pDescriptorPool = new VulkanDescriptorPool(nameDescriptorPool, this->m_pDevice);
        if (!pDescriptorPool->Init(descriptorCount))
        {
            F_LogError("*********************** VulkanDescriptorPoolManager::CreateDescriptorPool: Failed to init VulkanDescriptorPool, name: [%s] !", nameDescriptorPool.c_str());
            F_DELETE(pDescriptorPool)
            return nullptr;
        }
        AddDescriptorPool(pDescriptorPool);
        return pDescriptorPool;
    }
    void VulkanDescriptorPoolManager::DeleteDescriptorPool(const String& strName)
    {
        VulkanDescriptorPoolPtrMap::iterator itFind = m_mapDescriptorPool.find(strName);
        if (itFind == m_mapDescriptorPool.end())
        {
            return;
        }

        VulkanDescriptorPoolPtrVector::iterator itFindA = std::find(m_aDescriptorPool.begin(), m_aDescriptorPool.end(), itFind->second);
        if (itFindA != m_aDescriptorPool.end())
            m_aDescriptorPool.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapDescriptorPool.erase(itFind);
    }
    void VulkanDescriptorPoolManager::DeleteDescriptorPool(VulkanDescriptorPool* pDescriptorPool)
    {
        if (!pDescriptorPool)
            return;
        DeleteDescriptorPool(pDescriptorPool->GetName());
    }
    void VulkanDescriptorPoolManager::DeleteDescriptorPoolAll()
    {
        for (VulkanDescriptorPoolPtrVector::iterator it = m_aDescriptorPool.begin();
             it != m_aDescriptorPool.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aDescriptorPool.clear();
        m_mapDescriptorPool.clear();
    }

}; //LostPeterPluginRendererVulkan