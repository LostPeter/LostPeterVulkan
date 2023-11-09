/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VKPipelineManager.h"
#include "../include/VulkanWindow.h"
#include "../include/VKPipeline.h"

template<> LostPeter::VKPipelineManager* LostPeterFoundation::FSingleton<LostPeter::VKPipelineManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    VKPipelineManager* VKPipelineManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VKPipelineManager& VKPipelineManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VKPipelineManager::GetSingleton")
		return (*ms_Singleton);  
	}

    VKPipelineManager::VKPipelineManager()
        : Base("VKPipelineManager")
    {

    }
    VKPipelineManager::~VKPipelineManager()
    {
        Destroy();
    }
    
    void VKPipelineManager::Destroy()
    {
        DeleteVKPipelineAll();
    }
    bool VKPipelineManager::Init()
    {
        

        return true;
    }

    VKPipeline* VKPipelineManager::LoadVKPipeline(const String& strName)
    {
        VKPipeline* pVKPipeline = GetVKPipeline(strName);
        if (pVKPipeline == nullptr)
        {
            if (!loadVKPipeline(strName))
            {
                return nullptr;
            }
        }
        pVKPipeline->AddRef();
        return pVKPipeline;
    }
    VKPipeline* VKPipelineManager::loadVKPipeline(const String& strName)
    {
        VKPipeline* pVKPipeline = new VKPipeline(strName);
        pVKPipeline->LoadPipeline();
        if (AddVKPipeline(pVKPipeline))
        {
            F_LogInfo("VKPipelineManager::loadVKPipeline: Load pipeline success, [%s] !", 
                      strName.c_str());
        }
        return pVKPipeline;
    }
    void VKPipelineManager::UnloadVKPipeline(VKPipeline* pVKPipeline)
    {
        if (pVKPipeline == nullptr)
            return;
        pVKPipeline->DelRef();
        if (!HasRef())
        {
            DeleteVKPipeline(pVKPipeline->GetName());
        }
    }

    bool VKPipelineManager::HasVKPipeline(const String& strName)
    {
        return GetVKPipeline(strName) != nullptr;
    }

    VKPipeline* VKPipelineManager::GetVKPipeline(const String& strName)
    {
        VKPipelinePtrMap::iterator itFind = m_mapVKPipeline.find(strName);
        if (itFind == m_mapVKPipeline.end())
        {
            return nullptr;
        }
        return itFind->second;
    }

    bool VKPipelineManager::AddVKPipeline(VKPipeline* pVKPipeline)
    {
        const String& strName = pVKPipeline->GetName();
        VKPipelinePtrMap::iterator itFind = m_mapVKPipeline.find(strName);
        if (itFind != m_mapVKPipeline.end())
        {
            F_LogError("*********************** VKPipelineManager::AddVKPipeline: VKPipeline name already exist: [%s] !", strName.c_str());
            F_DELETE(pVKPipeline)
            return false;
        }

        m_aVKPipeline.push_back(pVKPipeline);
        m_mapVKPipeline.insert(VKPipelinePtrMap::value_type(strName, pVKPipeline));
        return true;
    }

    void VKPipelineManager::DeleteVKPipeline(const String& strName)
    {
        VKPipelinePtrMap::iterator itFind = m_mapVKPipeline.find(strName);
        if (itFind == m_mapVKPipeline.end())
        {
            return;
        }

        VKPipelinePtrVector::iterator itFindA = std::find(m_aVKPipeline.begin(), m_aVKPipeline.end(), itFind->second);
        if (itFindA != m_aVKPipeline.end())
            m_aVKPipeline.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapVKPipeline.erase(itFind);
    }

    void VKPipelineManager::DeleteVKPipelineAll()
    {
        for (VKPipelinePtrMap::iterator it = m_mapVKPipeline.begin();
             it != m_mapVKPipeline.end(); ++it)
        {
            F_DELETE(it->second)
        }
        m_aVKPipeline.clear();
        m_mapVKPipeline.clear();
    }

}; //LostPeter