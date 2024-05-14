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

#include "../include/RHIVulkanSwapChain.h"
#include "../include/RHIVulkanTexture.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanSwapChain::RHIVulkanSwapChain(const RHISwapChainCreateInfo& createInfo)
        : RHISwapChain(createInfo)
        , m_bIsPingPong(true)
    {

    }

    RHIVulkanSwapChain::~RHIVulkanSwapChain()
    {
        Destroy();
    }

    void RHIVulkanSwapChain::Destroy()
    {
        size_t count = m_aVulkanTextures.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aVulkanTextures[i])
        }
        m_aVulkanTextures.clear();
    }

    RHITexture* RHIVulkanSwapChain::GetTexture(uint8 nIndex)
    {
        F_Assert("RHIVulkanSwapChain::GetTexture" && nIndex < (uint8)m_aVulkanTextures.size());
        return m_aVulkanTextures[nIndex];
    }

    uint8 RHIVulkanSwapChain::AcquireBackTexture()
    {
        m_bIsPingPong = !m_bIsPingPong;
        return m_bIsPingPong ? 0 : 1;
    }

    void RHIVulkanSwapChain::Present()
    {

    }
    
}; //LostPeterPluginRHIVulkan