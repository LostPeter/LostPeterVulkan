/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-07
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanShaderModuleManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShaderModule.h"

template<> LostPeterPluginRendererVulkan::VulkanShaderModuleManager* LostPeterFoundation::FSingleton<LostPeterPluginRendererVulkan::VulkanShaderModuleManager>::ms_Singleton = nullptr;

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderModuleManager* VulkanShaderModuleManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	VulkanShaderModuleManager& VulkanShaderModuleManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "VulkanShaderModuleManager::GetSingleton")
		return (*ms_Singleton);     
	}


    VulkanShaderModuleManager::VulkanShaderModuleManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanShaderModuleManager::VulkanShaderModuleManager")
    }   

    VulkanShaderModuleManager::~VulkanShaderModuleManager()
    {
        Destroy();
    }

    void VulkanShaderModuleManager::Destroy()
    {
        DeleteShaderModuleAll();
    }

    bool VulkanShaderModuleManager::HasShaderModule(const String& strName)
    {
        return GetShaderModule(strName) != nullptr;
    }
    VulkanShaderModule* VulkanShaderModuleManager::GetShaderModule(const String& strName)
    {
        VulkanShaderModulePtrMap::iterator itFind = m_mapShaderModule.find(strName);
        if (itFind == m_mapShaderModule.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanShaderModuleManager::AddShaderModule(VulkanShaderModule* pShaderModule)
    {
        const String& strName = pShaderModule->GetName();
        VulkanShaderModulePtrMap::iterator itFind = m_mapShaderModule.find(strName);
        if (itFind != m_mapShaderModule.end())
        {
            F_LogError("*********************** VulkanShaderModuleManager::AddShaderModule: ShaderModule name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapShaderModule.insert(VulkanShaderModulePtrMap::value_type(strName, pShaderModule));
        m_aShaderModule.push_back(pShaderModule);
        return true;
    }
    VulkanShaderModule* VulkanShaderModuleManager::CreateShaderModule(const String& nameShaderModule,
                                                                      FShaderType eShader, 
                                                                      const String& pathFile)
    {
        VulkanShaderModule* pShaderModule = GetShaderModule(nameShaderModule);
        if (pShaderModule != nullptr)
            return pShaderModule;
        
        pShaderModule = new VulkanShaderModule(nameShaderModule, this->m_pDevice);
        if (!pShaderModule->Init(eShader,
                                 pathFile))
        {
            F_LogError("*********************** VulkanShaderModuleManager::CreateShaderModule: Failed to init VulkanShaderModule, name: [%s] !", nameShaderModule.c_str());
            F_DELETE(pShaderModule)
            return nullptr;
        }
        AddShaderModule(pShaderModule);
        return pShaderModule;
    }
    void VulkanShaderModuleManager::DeleteShaderModule(const String& strName)
    {
        VulkanShaderModulePtrMap::iterator itFind = m_mapShaderModule.find(strName);
        if (itFind == m_mapShaderModule.end())
        {
            return;
        }

        VulkanShaderModulePtrVector::iterator itFindA = std::find(m_aShaderModule.begin(), m_aShaderModule.end(), itFind->second);
        if (itFindA != m_aShaderModule.end())
            m_aShaderModule.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapShaderModule.erase(itFind);
    }
    void VulkanShaderModuleManager::DeleteShaderModule(VulkanShaderModule* pShaderModule)
    {
        if (!pShaderModule)
            return;
        DeleteShaderModule(pShaderModule->GetName());
    }
    void VulkanShaderModuleManager::DeleteShaderModuleAll()
    {
        for (VulkanShaderModulePtrVector::iterator it = m_aShaderModule.begin();
             it != m_aShaderModule.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aShaderModule.clear();
        m_mapShaderModule.clear();
    }

}; //LostPeterPluginRendererVulkan