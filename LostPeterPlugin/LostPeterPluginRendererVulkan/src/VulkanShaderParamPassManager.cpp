/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanShaderParamPassManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanShaderProgram.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanShaderParamPassManager::VulkanShaderParamPassManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanShaderParamPassManager::VulkanShaderParamPassManager")
    }

    VulkanShaderParamPassManager::~VulkanShaderParamPassManager()
    {
        Destroy();
    }

    void VulkanShaderParamPassManager::Destroy()
    {

        ShaderParamPassManager::Destroy();
    }

    
    bool VulkanShaderParamPassManager::GenerateShaderProgramParams(ShaderProgram* pShaderProgram)
    {
        VulkanShaderProgram* p = (VulkanShaderProgram*)pShaderProgram;
        

        return true;
    }

}; //LostPeterPluginRendererVulkan