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

#ifndef _VULKAN_SHADER_MODULE_H_
#define _VULKAN_SHADER_MODULE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderModule : public Base 
                             , public FNonCopyable
    {
        friend class VulkanShaderModuleManager;

    private:
        VulkanShaderModule(const String& nameShaderModule, VulkanDevice* pDevice);
    public:
        ~VulkanShaderModule();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkShaderModule m_vkShaderModule;

    public:
        F_FORCEINLINE VkShaderModule GetVkShaderModule() const { return m_vkShaderModule; }

    public:
        void Destroy();
        bool Init(FShaderType eShader, 
                  const String& pathFile);

    };

}; //LostPeterPluginRendererVulkan

#endif