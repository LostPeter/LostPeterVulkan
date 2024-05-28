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

#include "../include/RHIVulkanPipeline.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipeline::RHIVulkanPipeline()
        : m_vkPipeline(VK_NULL_HANDLE)
    {
        
    }

    RHIVulkanPipeline::~RHIVulkanPipeline()
    {

    }
    
    void RHIVulkanPipeline::destroyVkPipeline(RHIVulkanDevice* pVulkanDevice)
    {
        if (m_vkPipeline != VK_NULL_HANDLE)
        {
            pVulkanDevice->DestroyVkPipeline(m_vkPipeline);
        }
        m_vkPipeline = VK_NULL_HANDLE;
    }

}; //LostPeterPluginRHIVulkan