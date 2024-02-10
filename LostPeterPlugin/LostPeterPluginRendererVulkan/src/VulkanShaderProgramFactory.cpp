/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-10
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanShaderProgramFactory.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShaderProgram.h"

namespace LostPeterPluginRendererVulkan
{
    const String VulkanShaderProgramFactory::m_strShaderLanguage = "spirv";
    VulkanShaderProgramFactory::VulkanShaderProgramFactory(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanShaderProgramFactory::VulkanShaderProgramFactory")
    }

    VulkanShaderProgramFactory::~VulkanShaderProgramFactory()
    {

    }

    const String& VulkanShaderProgramFactory::GetShaderLanguage() const
    {
        return m_strShaderLanguage;
    }

    ShaderProgram* VulkanShaderProgramFactory::Create(ResourceManager* pResourceManager, 
                                                      uint32 nGroup, 
                                                      const String& strName, 
                                                      const String& strGroupName, 
                                                      ResourceHandle nHandle,
                                                      bool bIsManualLoad,
                                                      ResourceManualLoader* pResourceManualLoader)
    {
         return new VulkanShaderProgram(m_pDevice,
                                        pResourceManager,
                                        nGroup, 
                                        strName,
                                        strGroupName,
                                        nHandle,
                                        bIsManualLoad,
                                        pResourceManualLoader);
    }

}; //LostPeterPluginRendererVulkan