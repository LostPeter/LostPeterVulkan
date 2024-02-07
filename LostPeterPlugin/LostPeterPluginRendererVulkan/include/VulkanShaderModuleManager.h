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

#ifndef _VULKAN_SHADER_MODULE_MANAGER_H_
#define _VULKAN_SHADER_MODULE_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanShaderModuleManager : public FSingleton<VulkanShaderModuleManager>
    {
    public:
        VulkanShaderModuleManager(VulkanDevice* pDevice);
        ~VulkanShaderModuleManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanShaderModulePtrVector m_aShaderModule;
        VulkanShaderModulePtrMap m_mapShaderModule;

    public:
        static VulkanShaderModuleManager& GetSingleton();
		static VulkanShaderModuleManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanShaderModulePtrVector& GetShaderModulePtrVector() const { return m_aShaderModule; }
        F_FORCEINLINE VulkanShaderModulePtrVector& GetShaderModulePtrVector() { return m_aShaderModule; }
        F_FORCEINLINE const VulkanShaderModulePtrMap& GetShaderModulePtrMap() const { return m_mapShaderModule; }
        F_FORCEINLINE VulkanShaderModulePtrMap& GetShaderModulePtrMap() { return m_mapShaderModule; }

    public:
        void Destroy();

    public:
        bool HasShaderModule(const String& strName);
        VulkanShaderModule* GetShaderModule(const String& strName);
        bool AddShaderModule(VulkanShaderModule* pShaderModule);
        VulkanShaderModule* CreateShaderModule(const String& nameShaderModule,
                                               FShaderType eShader, 
                                               const String& pathFile);

        void DeleteShaderModule(const String& strName);
        void DeleteShaderModule(VulkanShaderModule* pShaderModule);
        void DeleteShaderModuleAll();
    };

}; //LostPeterPluginRendererVulkan

#endif