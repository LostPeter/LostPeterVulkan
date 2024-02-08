/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanShaderProgram.h"
#include "../include/VulkanShaderProgramManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShaderModuleManager.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderProgram::VulkanShaderProgram(VulkanDevice* pDevice,
                                             ResourceManager* pResourceManager,
                                             uint32 nGroup, 
                                             const String& strName,
                                             const String& strGroupName,
                                             ResourceHandle nHandle,
                                             bool bIsManualLoad /*= false*/,
                                             ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : ShaderProgram(pResourceManager,
                        nGroup, 
                        strName,
                        strGroupName,
                        nHandle,
                        bIsManualLoad,
                        pResourceManualLoader)
        , m_pDevice(pDevice)
        , m_pShaderModule(nullptr)
    {
        F_Assert(m_pDevice && "VulkanShaderProgram::VulkanShaderProgram")
    }

    VulkanShaderProgram::~VulkanShaderProgram()
    {
        Destroy();
    }

    void VulkanShaderProgram::Destroy()
    {
        Unload();
        ShaderProgram::Destroy();
    }

    void VulkanShaderProgram::addParameterInherit()
    {

    }

    void VulkanShaderProgram::destroyInternalResourcesImpl()
    {
        VulkanShaderModuleManager::GetSingleton().DeleteShaderModule(m_pShaderModule);
        m_pShaderModule = nullptr;
    }

    void VulkanShaderProgram::createInternalResourcesImpl()
    {
        const String& nameShaderModule = GetName();
        m_pShaderModule = VulkanShaderModuleManager::GetSingleton().GetShaderModule(nameShaderModule);
        if (m_pShaderModule == nullptr)
        {
            m_pShaderModule = VulkanShaderModuleManager::GetSingleton().CreateShaderModule(nameShaderModule, 
                                                                                           m_eShader,
                                                                                           m_strPath);
            if (m_pShaderModule == nullptr)
            {
                F_LogError("*********************** VulkanShaderProgram::createInternalResourcesImpl: Failed to create shader module: group: [%u], name: [%s], path: [%s] !", GetGroup(), nameShaderModule.c_str(), m_strPath.c_str());
                return;
            }
        }
        m_pShaderModule->AddRef();
    }

}; //LostPeterPluginRendererVulkan