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

#include "../include/VulkanShaderManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShader.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderManager::VulkanShaderManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanShaderManager::VulkanShaderManager")
    }

    VulkanShaderManager::~VulkanShaderManager()
    {
        Destroy();
    }

    Resource* VulkanShaderManager::createImpl(uint32 nGroup,
                                              const String& strName,
                                              const String& strGroupName,
                                              ResourceHandle nHandle, 
                                              bool bIsManualLoad,
                                              ResourceManualLoader* pManualLoader, 
                                              const NameValuePairMap* pLoadParams)
    {
        return new VulkanShader(m_pDevice,
                                this,
                                nGroup, 
                                strName,
                                strGroupName,
                                nHandle,
                                bIsManualLoad,
                                pManualLoader);
    }

}; //LostPeterPluginRendererVulkan