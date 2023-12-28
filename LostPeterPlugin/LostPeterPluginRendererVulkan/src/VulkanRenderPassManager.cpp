/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderPassManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderPass.h"

template<> LostPeterPluginRendererVulkan::VulkanRenderPassManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanRenderPassManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPassManager* VulkanRenderPassManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanRenderPassManager& VulkanRenderPassManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanRenderPassManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanRenderPassManager::VulkanRenderPassManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanRenderPassManager::VulkanRenderPassManager")
    }

    VulkanRenderPassManager::~VulkanRenderPassManager()
    {
        Destroy();
    }

    void VulkanRenderPassManager::Destroy()
    {
        DeleteRenderPassAll();
    }

    bool VulkanRenderPassManager::HasRenderPass(const String& strName)
    {
        return GetRenderPass(strName) != nullptr;
    }
    VulkanRenderPass* VulkanRenderPassManager::GetRenderPass(const String& strName)
    {
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind == m_mapRenderPass.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanRenderPassManager::AddRenderPass(VulkanRenderPass* pRenderPass)
    {
        const String& strName = pRenderPass->GetName();
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind != m_mapRenderPass.end())
        {
            F_LogError("*********************** VulkanRenderPassManager::AddRenderPass: RenderPass name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapRenderPass.insert(VulkanRenderPassPtrMap::value_type(strName, pRenderPass));
        m_aRenderPass.push_back(pRenderPass);
        return true;
    }
    bool VulkanRenderPassManager::CreateRenderPass(const String& nameRenderPass
                                                    )
    {
        VulkanRenderPass* pRenderPass = GetRenderPass(nameRenderPass);
        if (pRenderPass != nullptr)
            return true;
        
        pRenderPass = new VulkanRenderPass(nameRenderPass, this->m_pDevice);
        if (!pRenderPass->Init())
        {
            return false;
        }
        AddRenderPass(pRenderPass);
        return pRenderPass;
    }
    void VulkanRenderPassManager::DeleteRenderPass(const String& strName)
    {
        VulkanRenderPassPtrMap::iterator itFind = m_mapRenderPass.find(strName);
        if (itFind == m_mapRenderPass.end())
        {
            return;
        }

        VulkanRenderPassPtrVector::iterator itFindA = std::find(m_aRenderPass.begin(), m_aRenderPass.end(), itFind->second);
        if (itFindA != m_aRenderPass.end())
            m_aRenderPass.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapRenderPass.erase(itFind);
    }
    void VulkanRenderPassManager::DeleteRenderPassAll()
    {
        for (VulkanRenderPassPtrVector::iterator it = m_aRenderPass.begin();
             it != m_aRenderPass.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aRenderPass.clear();
        m_mapRenderPass.clear();
    }

}; //LostPeterPluginRendererVulkan