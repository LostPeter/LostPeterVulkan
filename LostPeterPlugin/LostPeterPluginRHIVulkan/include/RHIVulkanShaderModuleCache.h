/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_SHADER_MODULE_CACHE_H_
#define _RHI_VULKAN_SHADER_MODULE_CACHE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanShaderModuleCache : public RHIShaderModuleCache 
                                                     , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanShaderModuleCache(RHIVulkanDevice* pVulkanDevice, const RHIShaderModuleCacheCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanShaderModuleCache();

    public:
    protected:
        
        String m_strDebugName;

    public:

        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:

    };
    
}; //LostPeterPluginRHIVulkan

#endif