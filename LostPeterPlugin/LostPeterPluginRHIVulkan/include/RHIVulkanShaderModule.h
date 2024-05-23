/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_SHADER_MODULE_H_
#define _RHI_VULKAN_SHADER_MODULE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanShaderModule : public RHIShaderModule
                                                , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanShaderModule(RHIVulkanDevice* pVulkanDevice, const RHIShaderModuleCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanShaderModule();

    public:
    protected:
        VkShaderModule m_vkShaderModule;

        RHIShaderStageBitsType m_eShaderStageBits;
        String m_strPath;
        String m_strContent;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkShaderModule& GetVkShaderModule() { return m_vkShaderModule; }

        F_FORCEINLINE RHIShaderStageBitsType GetShaderStageBitsType() const { return m_eShaderStageBits; }
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
        F_FORCEINLINE const String& GetContent() const { return m_strContent; }
        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkShaderModule();
    };
    
}; //LostPeterPluginRHIVulkan

#endif