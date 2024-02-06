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

#include "../include/VulkanShaderProgramManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShaderProgram.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderProgramManager::VulkanShaderProgramManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanShaderProgramManager::VulkanShaderProgramManager")
    }

    VulkanShaderProgramManager::~VulkanShaderProgramManager()
    {
        Destroy();
    }

    Resource* VulkanShaderProgramManager::createImpl(uint32 nGroup,
                                              const String& strName,
                                              const String& strGroupName,
                                              ResourceHandle nHandle, 
                                              bool bIsManualLoad,
                                              ResourceManualLoader* pManualLoader, 
                                              const NameValuePairMap* pLoadParams)
    {
        return new VulkanShaderProgram(m_pDevice,
                                       this,
                                       nGroup, 
                                       strName,
                                       strGroupName,
                                       nHandle,
                                       bIsManualLoad,
                                       pManualLoader);
    }

}; //LostPeterPluginRendererVulkan