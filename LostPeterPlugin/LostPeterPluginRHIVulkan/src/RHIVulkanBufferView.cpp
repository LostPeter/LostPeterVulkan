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

#include "../include/RHIVulkanBufferView.h"
#include "../include/RHIVulkanBuffer.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanBufferView::RHIVulkanBufferView(RHIVulkanBuffer* pBuffer, const RHIBufferViewCreateInfo& createInfo)
        : RHIBufferView(createInfo)
        , m_pBuffer(pBuffer)
        , m_nStride(createInfo.eVertexBufferViewInfo.nStride)
        , m_nSize(createInfo.nSize)
        , m_nOffset(createInfo.nOffset)
        , m_eIndexFormat(createInfo.eBufferView == RHIBufferViewType::RHI_BufferView_Index ? createInfo.eIndexBufferViewInfo.eIndexFormat : RHIIndexFormatType::RHI_IndexFormat_16Bit)
    {
        F_Assert(m_pBuffer && "RHIVulkanBufferView::RHIVulkanBufferView")
    }

    RHIVulkanBufferView::~RHIVulkanBufferView()
    {
        Destroy();
    }

    void RHIVulkanBufferView::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan