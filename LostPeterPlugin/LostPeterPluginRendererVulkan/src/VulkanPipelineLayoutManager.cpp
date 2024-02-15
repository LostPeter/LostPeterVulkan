/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanPipelineLayoutManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanPipelineLayout.h"

template<> LostPeterPluginRendererVulkan::VulkanPipelineLayoutManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanPipelineLayoutManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanPipelineLayoutManager* VulkanPipelineLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanPipelineLayoutManager& VulkanPipelineLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanPipelineLayoutManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanPipelineLayoutManager::VulkanPipelineLayoutManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanPipelineLayoutManager::VulkanPipelineLayoutManager")
    }   

    VulkanPipelineLayoutManager::~VulkanPipelineLayoutManager()
    {
        Destroy();
    }

    void VulkanPipelineLayoutManager::Destroy()
    {
        DeletePipelineLayoutAll();
    }

    bool VulkanPipelineLayoutManager::HasPipelineLayout(const String& strName)
    {
        return GetPipelineLayout(strName) != nullptr;
    }
    VulkanPipelineLayout* VulkanPipelineLayoutManager::GetPipelineLayout(const String& strName)
    {
        VulkanPipelineLayoutPtrMap::iterator itFind = m_mapPipelineLayout.find(strName);
        if (itFind == m_mapPipelineLayout.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanPipelineLayoutManager::AddPipelineLayout(VulkanPipelineLayout* pPipelineLayout)
    {
        const String& strName = pPipelineLayout->GetName();
        VulkanPipelineLayoutPtrMap::iterator itFind = m_mapPipelineLayout.find(strName);
        if (itFind != m_mapPipelineLayout.end())
        {
            F_LogError("*********************** VulkanPipelineLayoutManager::AddPipelineLayout: PipelineLayout name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapPipelineLayout.insert(VulkanPipelineLayoutPtrMap::value_type(strName, pPipelineLayout));
        m_aPipelineLayout.push_back(pPipelineLayout);
        return true;
    }
    VulkanPipelineLayout* VulkanPipelineLayoutManager::CreatePipelineLayout(const String& namePipelineLayout,
                                                                            const VkDescriptorSetLayoutVector& aDescriptorSetLayout)
    {
        VulkanPipelineLayout* pPipelineLayout = GetPipelineLayout(namePipelineLayout);
        if (pPipelineLayout != nullptr)
        {
            pPipelineLayout->AddRef();
            return pPipelineLayout;
        }

        pPipelineLayout = new VulkanPipelineLayout(namePipelineLayout, this->m_pDevice);
        if (!pPipelineLayout->Init(aDescriptorSetLayout))
        {
            F_LogError("*********************** VulkanPipelineLayoutManager::CreatePipelineLayout: Failed to init VulkanPipelineLayout, name: [%s] !", namePipelineLayout.c_str());
            F_DELETE(pPipelineLayout)
            return nullptr;
        }
        AddPipelineLayout(pPipelineLayout);
        pPipelineLayout->AddRef();
        return pPipelineLayout;
    }
    void VulkanPipelineLayoutManager::DeletePipelineLayout(const String& strName)
    {
        VulkanPipelineLayoutPtrMap::iterator itFind = m_mapPipelineLayout.find(strName);
        if (itFind == m_mapPipelineLayout.end())
        {
            return;
        }

        itFind->second->DelRef();
        if (itFind->second->CanDel())
        {
            VulkanPipelineLayoutPtrVector::iterator itFindA = std::find(m_aPipelineLayout.begin(), m_aPipelineLayout.end(), itFind->second);
            if (itFindA != m_aPipelineLayout.end())
                m_aPipelineLayout.erase(itFindA);
            F_DELETE(itFind->second)
            m_mapPipelineLayout.erase(itFind);
        }
    }
    void VulkanPipelineLayoutManager::DeletePipelineLayout(VulkanPipelineLayout* pPipelineLayout)
    {
        if (!pPipelineLayout)
            return;
        DeletePipelineLayout(pPipelineLayout->GetName());
    }
    void VulkanPipelineLayoutManager::DeletePipelineLayoutAll()
    {
        for (VulkanPipelineLayoutPtrVector::iterator it = m_aPipelineLayout.begin();
             it != m_aPipelineLayout.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aPipelineLayout.clear();
        m_mapPipelineLayout.clear();
    }

}; //LostPeterPluginRendererVulkan