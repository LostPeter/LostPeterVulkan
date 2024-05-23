/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_PIPELINE_LAYOUT_H_
#define _RHI_VULKAN_PIPELINE_LAYOUT_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineLayout : public RHIPipelineLayout
                                                  , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanPipelineLayout(RHIVulkanDevice* pVulkanDevice, const RHIPipelineLayoutCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanPipelineLayout();

    public:
    protected:
        VkPipelineLayout m_vkPipelineLayout;


        String m_strDebugName;

    public:
        F_FORCEINLINE VkPipelineLayout& GetVkPipelineLayout() { return m_vkPipelineLayout; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipelineLayout();
    };
    
}; //LostPeterPluginRHIVulkan

#endif