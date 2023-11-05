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

#include "../include/VKDescriptorSetLayoutManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKDescriptorSetLayoutSerializer.h"
#include "../include/VKDescriptorSetLayout.h"

template<> LostPeter::VKDescriptorSetLayoutManager* LostPeterFoundation::FSingleton<LostPeter::VKDescriptorSetLayoutManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKDescriptorSetLayoutManager* VKDescriptorSetLayoutManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKDescriptorSetLayoutManager& VKDescriptorSetLayoutManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKDescriptorSetLayoutManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKDescriptorSetLayoutManager::VKDescriptorSetLayoutManager()
        : Base("VKDescriptorSetLayoutManager")
        , m_pVKDescriptorSetLayoutSerializer(nullptr)
    {

    }
    VKDescriptorSetLayoutManager::~VKDescriptorSetLayoutManager()
    {
        Destroy();
    }
    
    void VKDescriptorSetLayoutManager::Destroy()
    {
        F_DELETE(m_pVKDescriptorSetLayoutSerializer)
        DeleteVKDescriptorSetLayoutAll();
    }
    bool VKDescriptorSetLayoutManager::Init(uint nGroup, const String& strNameCfg)
    {
        //1> VKDescriptorSetLayout Cfg Path 
        String strPathCfgShader = FPathManager::GetSingleton().GetFilePath(nGroup, strNameCfg);
        if (strPathCfgShader.empty())
        {
            F_LogError("*********************** VKDescriptorSetLayoutManager::Init: Can not get file path from group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        //2> VKDescriptorSetLayout Serializer
        m_pVKDescriptorSetLayoutSerializer = new VKDescriptorSetLayoutSerializer();
        if (!m_pVKDescriptorSetLayoutSerializer->LoadFile(strPathCfgShader))
        {
            F_LogError("*********************** VKDescriptorSetLayoutManager::Init: Load file descriptor set cfg failed, group: [%u], name: [%s] !", nGroup, strNameCfg.c_str());
            return false;
        }

        return true;
    }

    bool VKDescriptorSetLayoutManager::LoadVKDescriptorSetLayoutAll()
    {
        if (m_pVKDescriptorSetLayoutSerializer == nullptr)
            return false;

        DeleteVKDescriptorSetLayoutAll();
        DescriptorSetLayoutInfoPtrVector& aDescriptorSetLayoutInfos = m_pVKDescriptorSetLayoutSerializer->GetDescriptorSetLayoutInfoPtrVector();
        for (DescriptorSetLayoutInfoPtrVector::iterator it = aDescriptorSetLayoutInfos.begin();
             it != aDescriptorSetLayoutInfos.end(); ++it)
        {
            if (!loadVKDescriptorSetLayout(*it))
                continue;
        }

        return true;
    }

    VKDescriptorSetLayout* VKDescriptorSetLayoutManager::LoadVKDescriptorSetLayout(const String& strName)
    {
        if (m_pVKDescriptorSetLayoutSerializer == nullptr)
            return nullptr;

        VKDescriptorSetLayout* pVKDescriptorSetLayout = GetVKDescriptorSetLayout(strName);
        if (pVKDescriptorSetLayout == nullptr)
        {
            DescriptorSetLayoutInfo* pDescriptorSetInfo = m_pVKDescriptorSetLayoutSerializer->GetDescriptorSetLayoutInfo(strName);
            if (pDescriptorSetInfo == nullptr)
            {
                F_LogError("*********************** VKDescriptorSetLayoutManager::LoadVKDescriptorSetLayout: Can not find descriptor set info, name: [%s] !", strName.c_str());
                return nullptr;
            }
            if (!loadVKDescriptorSetLayout(pDescriptorSetInfo))
            {
                return nullptr;
            }
        }
        pVKDescriptorSetLayout->AddRef();
        return pVKDescriptorSetLayout;
    }
    VKDescriptorSetLayout* VKDescriptorSetLayoutManager::loadVKDescriptorSetLayout(DescriptorSetLayoutInfo* pDSLI)
    {
        VKDescriptorSetLayout* pVKDescriptorSetLayout = new VKDescriptorSetLayout(pDSLI->nameDescriptorSetLayoutInfo);
        pVKDescriptorSetLayout->LoadDescriptorSetLayout();
        if (AddVKDescriptorSetLayout(pVKDescriptorSetLayout))
        {
            F_LogInfo("VKDescriptorSetLayoutManager::loadVKDescriptorSetLayout: Load descriptor set layout success, [%s] !", 
                      pDSLI->nameDescriptorSetLayoutInfo.c_str());
        }
        return pVKDescriptorSetLayout;
    }
    void VKDescriptorSetLayoutManager::UnloadVKDescriptorSetLayout(VKDescriptorSetLayout* pVKDescriptorSetLayout)
    {
        if (pVKDescriptorSetLayout == nullptr)
            return;
        pVKDescriptorSetLayout->DelRef();
        if (!HasRef())
        {
            DeleteVKDescriptorSetLayout(pVKDescriptorSetLayout->GetName());
        }
    }

    bool VKDescriptorSetLayoutManager::HasVKDescriptorSetLayout(const String& strName)
    {
        return GetVKDescriptorSetLayout(strName) != nullptr;
    }

    VKDescriptorSetLayout* VKDescriptorSetLayoutManager::GetVKDescriptorSetLayout(const String& strName)
    {
        VKDescriptorSetLayoutPtrMap::iterator itFind = m_mapVKDescriptorSetLayout.find(strName);
        if (itFind == m_mapVKDescriptorSetLayout.end())
        {
            F_LogError("*********************** VKDescriptorSetLayoutManager::GetVKDescriptorSetLayout: Can not find, name: [%s]", strName.c_str());
            return nullptr;
        }
        return itFind->second;
    }

    bool VKDescriptorSetLayoutManager::AddVKDescriptorSetLayout(VKDescriptorSetLayout* pVKDescriptorSetLayout)
    {
        const String& strName = pVKDescriptorSetLayout->GetName();
        VKDescriptorSetLayoutPtrMap::iterator itFind = m_mapVKDescriptorSetLayout.find(strName);
        if (itFind != m_mapVKDescriptorSetLayout.end())
        {
            F_LogError("*********************** VKDescriptorSetLayoutManager::AddVKDescriptorSetLayout: VKDescriptorSetLayout name already exist: [%s] !", strName.c_str());
            F_DELETE(pVKDescriptorSetLayout)
            return false;
        }

        m_aVKDescriptorSetLayout.push_back(pVKDescriptorSetLayout);
        m_mapVKDescriptorSetLayout.insert(VKDescriptorSetLayoutPtrMap::value_type(strName, pVKDescriptorSetLayout));
        return true;
    }

    void VKDescriptorSetLayoutManager::DeleteVKDescriptorSetLayout(const String& strName)
    {
        VKDescriptorSetLayoutPtrMap::iterator itFind = m_mapVKDescriptorSetLayout.find(strName);
        if (itFind == m_mapVKDescriptorSetLayout.end())
        {
            return;
        }

        VKDescriptorSetLayoutPtrVector::iterator itFindA = std::find(m_aVKDescriptorSetLayout.begin(), m_aVKDescriptorSetLayout.end(), itFind->second);
        if (itFindA != m_aVKDescriptorSetLayout.end())
            m_aVKDescriptorSetLayout.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapVKDescriptorSetLayout.erase(itFind);
    }

    void VKDescriptorSetLayoutManager::DeleteVKDescriptorSetLayoutAll()
    {
        for (VKDescriptorSetLayoutPtrMap::iterator it = m_mapVKDescriptorSetLayout.begin();
             it != m_mapVKDescriptorSetLayout.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aVKDescriptorSetLayout.clear();
        m_mapVKDescriptorSetLayout.clear();
    }

}; //LostPeter