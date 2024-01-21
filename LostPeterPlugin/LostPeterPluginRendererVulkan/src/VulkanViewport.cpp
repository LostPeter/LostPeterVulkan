/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanViewport.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanViewport::VulkanViewport()
    {

    }

    VulkanViewport::~VulkanViewport()
    {
        Destroy();
    }

    void VulkanViewport::Destroy()
    {
        m_pViewport = nullptr;
    }

    bool VulkanViewport::Init(Viewport* pViewport)
    {
        F_Assert(pViewport && "VulkanViewport::Init")
        m_pViewport = pViewport;
        pViewport->UpdateDimensions();
        UpdateViewport();

        return true;
    }

    void VulkanViewport::UpdateViewport()
    {
        F_Assert(m_pViewport && "VulkanViewport::UpdateViewport")

        int nLeft = m_pViewport->GetActualLeft();
        int nTop = m_pViewport->GetActualTop();
        int nWidth = m_pViewport->GetActualWidth();
        int nHeight = m_pViewport->GetActualHeight();
        float fDepthMin = m_pViewport->GetDepthMin();
        float fDepthMax = m_pViewport->GetDepthMax();

        this->m_vkOffset2D.x = nLeft;
        this->m_vkOffset2D.y = nTop;
        this->m_vkExtent2D.width = nWidth;
        this->m_vkExtent2D.height = nHeight;

        this->m_vkViewport = {};
        this->m_vkViewport.x = (float)nLeft;
        this->m_vkViewport.y = (float)nTop;
        this->m_vkViewport.width = (float)nWidth;
        this->m_vkViewport.height = (float)nHeight;
        this->m_vkViewport.minDepth = fDepthMin;
        this->m_vkViewport.maxDepth = fDepthMax;

        this->m_vkScissor = {};
        this->m_vkScissor.offset = this->m_vkOffset2D;
        this->m_vkScissor.extent = this->m_vkExtent2D;
    }

}; //LostPeterPluginRendererVulkan