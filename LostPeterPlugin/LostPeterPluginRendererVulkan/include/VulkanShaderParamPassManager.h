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

#ifndef _VULKAN_SHADER_PARAM_PASS_MANAGER_H_
#define _VULKAN_SHADER_PARAM_PASS_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderParamPassManager : public ShaderParamPassManager 
    {
    public:
        VulkanShaderParamPassManager(VulkanDevice* pDevice);
        virtual ~VulkanShaderParamPassManager();

    public:
    protected:
        VulkanDevice* m_pDevice;

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        virtual void Destroy();
    
    public:
        virtual bool GenerateShaderProgramParams(ShaderProgram* pShaderProgram);

    protected:

    };

}; //LostPeterPluginRendererVulkan

#endif