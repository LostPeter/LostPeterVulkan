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

#include "../include/RHIVulkanBuffer.h"
#include "../include/RHIVulkanBufferView.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBuffer::RHIVulkanBuffer(const RHIBufferCreateInfo& createInfo)
        : RHIBuffer(createInfo)
        , m_aVulkanData(1)
    {

    }

    RHIVulkanBuffer::~RHIVulkanBuffer()
    {
        Destroy();
    }

    void RHIVulkanBuffer::Destroy()
    {

    }

    void* RHIVulkanBuffer::Map(RHIMapType eMap, uint32 nOffset, uint32 nLength)
    {
        return m_aVulkanData.data();
    }

    void RHIVulkanBuffer::UnMap()
    {

    }

    RHIBufferView* RHIVulkanBuffer::CreateBufferView(const RHIBufferViewCreateInfo& createInfo)
    {
        return new RHIVulkanBufferView(createInfo);
    }
    
}; //LostPeterPluginRHIVulkan