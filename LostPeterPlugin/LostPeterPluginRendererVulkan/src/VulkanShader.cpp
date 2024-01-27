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

#include "../include/VulkanShader.h"
#include "../include/VulkanShaderManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShader::VulkanShader(VulkanDevice* pDevice,
                               ResourceManager* pResourceManager,
                               uint32 nGroup, 
                               const String& strName,
                               const String& strGroupName,
                               ResourceHandle nHandle,
                               bool bIsManualLoad /*= false*/,
                               ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Shader(pResourceManager,
                 nGroup, 
                 strName,
                 strGroupName,
                 nHandle,
                 bIsManualLoad,
                 pResourceManualLoader)
        , m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanShader::VulkanShader")
    }

    VulkanShader::~VulkanShader()
    {
        Destroy();
    }

    void VulkanShader::Destroy()
    {
        Unload();
        Shader::Destroy();
    }

    void VulkanShader::addParameterInherit()
    {

    }

    void VulkanShader::destroyInternalResourcesImpl()
    {

    }

    void VulkanShader::createInternalResourcesImpl()
    {
        
    }

}; //LostPeterPluginRendererVulkan