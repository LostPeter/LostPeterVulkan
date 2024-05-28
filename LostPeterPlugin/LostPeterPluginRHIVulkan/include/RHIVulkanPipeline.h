/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_PIPELINE_H_
#define _RHI_VULKAN_PIPELINE_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipeline
    {  
    public:
        RHIVulkanPipeline();
        virtual ~RHIVulkanPipeline();

    public:
    protected:
        VkPipeline m_vkPipeline;


    public:
        F_FORCEINLINE VkPipeline& GetVkPipeline() { return m_vkPipeline; }

    protected:
        void destroyVkPipeline(RHIVulkanDevice* pVulkanDevice);

    };
    
}; //LostPeterPluginRHIVulkan

#endif