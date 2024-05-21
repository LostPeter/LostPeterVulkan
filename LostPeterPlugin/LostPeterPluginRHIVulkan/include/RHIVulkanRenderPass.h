/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_RENDER_PASS_H_
#define _RHI_VULKAN_RENDER_PASS_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanRenderPass : public RHIRenderPass
    {
    public:
        RHIVulkanRenderPass(RHIVulkanDevice* pVulkanDevice, const RHIRenderPassCreateInfo& createInfo);
        virtual ~RHIVulkanRenderPass();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;


        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();


    protected:
    
    };
    
}; //LostPeterPluginRHIVulkan

#endif