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

#include "../include/VulkanShaderModule.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderModule::VulkanShaderModule(const String& nameShaderModule, VulkanDevice* pDevice)
        : Base(nameShaderModule)
        , m_pDevice(pDevice)
        , m_vkShaderModule(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanShaderModule::VulkanShaderModule")
    }

    VulkanShaderModule::~VulkanShaderModule()
    {
        Destroy();
    }

    void VulkanShaderModule::Destroy()
    {
        if (this->m_vkShaderModule != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkShaderModule(this->m_vkShaderModule);
        }
        this->m_vkShaderModule = VK_NULL_HANDLE;
    }

    bool VulkanShaderModule::Init(FShaderType eShader, 
                                  const String& pathFile)
    {
        Destroy();
        if (!m_pDevice->CreateVkShaderModule(eShader,
                                             pathFile,
                                             this->m_vkShaderModule))
        {
            F_LogError("*********************** VulkanShaderModule::Init: Failed to CreateVkShaderModule, path: [%s] !", pathFile.c_str());
            return false;
        }

        return true;
    }

}; //LostPeterPluginRendererVulkan