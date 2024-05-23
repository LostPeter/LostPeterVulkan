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

#include "../include/RHIVulkanFrameBuffer.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanFrameBuffer::RHIVulkanFrameBuffer(RHIVulkanDevice* pVulkanDevice, const RHIFrameBufferCreateInfo& createInfo)
        : RHIFrameBuffer(pVulkanDevice, createInfo)
        , RHIVulkanObject(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanFrameBuffer::RHIVulkanFrameBuffer")

    }   

    RHIVulkanFrameBuffer::~RHIVulkanFrameBuffer()
    {
        Destroy();
    }

    void RHIVulkanFrameBuffer::Destroy()
    {
        
    }
    

}; //LostPeterPluginRHIVulkan