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
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineGraphics : public RHIPipelineGraphics
                                                    , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanPipelineGraphics();

    public:
    protected:
        VkPipeline m_vkPipeline;

        String m_strDebugName;

    public:
        F_FORCEINLINE VkPipeline& GetVkPipeline() { return m_vkPipeline; }

        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipeline();
    };

}; //LostPeterPluginRHIVulkan

#endif