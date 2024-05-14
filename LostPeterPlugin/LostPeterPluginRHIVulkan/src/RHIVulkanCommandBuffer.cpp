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

#include "../include/RHIVulkanCommandBuffer.h"
#include "../include/RHIVulkanCommandEncoder.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanCommandBuffer::RHIVulkanCommandBuffer()
    {

    }

    RHIVulkanCommandBuffer::~RHIVulkanCommandBuffer()
    {
        Destroy();
    }

    void RHIVulkanCommandBuffer::Destroy()
    {

    }

    RHICommandEncoder* RHIVulkanCommandBuffer::Begin()
    {
        return new RHIVulkanCommandEncoder(*this);
    }
    
}; //LostPeterPluginRHIVulkan