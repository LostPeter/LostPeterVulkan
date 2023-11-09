/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineLayoutManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKPipelineLayout.h"
#include "../include/VKDescriptorSetLayoutManager.h"
#include "../include/VKDescriptorSetLayout.h"

template<> LostPeter::VKPipelineLayoutManager* LostPeterFoundation::FSingleton<LostPeter::VKPipelineLayoutManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKPipelineLayoutManager* VKPipelineLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKPipelineLayoutManager& VKPipelineLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKPipelineLayoutManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKPipelineLayoutManager::VKPipelineLayoutManager()
        : Base("VKPipelineLayoutManager")
    {

    }
    VKPipelineLayoutManager::~VKPipelineLayoutManager()
    {
        Destroy();
    }
    
    void VKPipelineLayoutManager::Destroy()
    {
        DeleteVKPipelineLayoutAll();
    }
    bool VKPipelineLayoutManager::Init()
    {
        

        return true;
    }

    bool VKPipelineLayoutManager::LoadVKPipelineLayoutAll()
    {
        DeleteVKPipelineLayoutAll();
        VKDescriptorSetLayoutPtrVector& aDescriptorSetLayout = VKDescriptorSetLayoutManager::GetSingleton().GetVKDescriptorSetLayoutPtrVector();
        for (VKDescriptorSetLayoutPtrVector::iterator it = aDescriptorSetLayout.begin();
             it != aDescriptorSetLayout.end(); ++it)
        {
            VKDescriptorSetLayout* pVKDescriptorSetLayout = (*it);
            if (!loadVKPipelineLayout(pVKDescriptorSetLayout->GetName()))
                continue;
        }

        return true;
    }

    VKPipelineLayout* VKPipelineLayoutManager::LoadVKPipelineLayout(const String& strName)
    {
        VKPipelineLayout* pVKPipelineLayout = GetVKPipelineLayout(strName);
        if (pVKPipelineLayout == nullptr)
        {
            if (!loadVKPipelineLayout(strName))
            {
                return nullptr;
            }
        }
        pVKPipelineLayout->AddRef();
        return pVKPipelineLayout;
    }
    VKPipelineLayout* VKPipelineLayoutManager::loadVKPipelineLayout(const String& strName)
    {
        VKPipelineLayout* pVKPipelineLayout = new VKPipelineLayout(strName);
        pVKPipelineLayout->LoadPipelineLayout();
        if (AddVKPipelineLayout(pVKPipelineLayout))
        {
            F_LogInfo("VKPipelineLayoutManager::loadVKPipelineLayout: Load pipeline layout success, [%s] !", 
                      strName.c_str());
        }
        return pVKPipelineLayout;
    }
    void VKPipelineLayoutManager::UnloadVKPipelineLayout(VKPipelineLayout* pVKPipelineLayout)
    {
        if (pVKPipelineLayout == nullptr)
            return;
        pVKPipelineLayout->DelRef();
        if (!HasRef())
        {
            DeleteVKPipelineLayout(pVKPipelineLayout->GetName());
        }
    }

    bool VKPipelineLayoutManager::HasVKPipelineLayout(const String& strName)
    {
        return GetVKPipelineLayout(strName) != nullptr;
    }

    VKPipelineLayout* VKPipelineLayoutManager::GetVKPipelineLayout(const String& strName)
    {
        VKPipelineLayoutPtrMap::iterator itFind = m_mapVKPipelineLayout.find(strName);
        if (itFind == m_mapVKPipelineLayout.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    bool VKPipelineLayoutManager::AddVKPipelineLayout(VKPipelineLayout* pVKPipelineLayout)
    {
        const String& strName = pVKPipelineLayout->GetName();
        VKPipelineLayoutPtrMap::iterator itFind = m_mapVKPipelineLayout.find(strName);
        if (itFind != m_mapVKPipelineLayout.end())
        {
            F_LogError("*********************** VKPipelineLayoutManager::AddVKPipelineLayout: VKPipelineLayout name already exist: [%s] !", strName.c_str());
            F_DELETE(pVKPipelineLayout)
            return false;
        }

        m_aVKPipelineLayout.push_back(pVKPipelineLayout);
        m_mapVKPipelineLayout.insert(VKPipelineLayoutPtrMap::value_type(strName, pVKPipelineLayout));
        return true;
    }

    void VKPipelineLayoutManager::DeleteVKPipelineLayout(const String& strName)
    {
        VKPipelineLayoutPtrMap::iterator itFind = m_mapVKPipelineLayout.find(strName);
        if (itFind == m_mapVKPipelineLayout.end())
        {
            return;
        }

        VKPipelineLayoutPtrVector::iterator itFindA = std::find(m_aVKPipelineLayout.begin(), m_aVKPipelineLayout.end(), itFind->second);
        if (itFindA != m_aVKPipelineLayout.end())
            m_aVKPipelineLayout.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapVKPipelineLayout.erase(itFind);
    }

    void VKPipelineLayoutManager::DeleteVKPipelineLayoutAll()
    {
        for (VKPipelineLayoutPtrMap::iterator it = m_mapVKPipelineLayout.begin();
             it != m_mapVKPipelineLayout.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aVKPipelineLayout.clear();
        m_mapVKPipelineLayout.clear();
    }

}; //LostPeter