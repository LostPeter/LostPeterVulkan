/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_PIPELINE_GRAPHICS_H_
#define _RHI_VULKAN_PIPELINE_GRAPHICS_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineGraphics : public RHIPipelineGraphics
    {
    public:
        RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo);
        virtual ~RHIVulkanPipelineGraphics();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;


    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        

    public:
        virtual void Destroy();

    };

}; //LostPeterPluginRHIVulkan

#endif