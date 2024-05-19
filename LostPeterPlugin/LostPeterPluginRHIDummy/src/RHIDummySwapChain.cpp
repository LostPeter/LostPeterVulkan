/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummySwapChain.h"
#include "../include/RHIDummyTexture.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummySwapChain::RHIDummySwapChain(RHIDummyDevice* pDummyDevice, const RHISwapChainCreateInfo& createInfo)
        : RHISwapChain(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
        , m_bIsPingPong(true)
    {
        F_Assert(m_pDummyDevice && "RHIDummySwapChain::RHIDummySwapChain")
    }

    RHIDummySwapChain::~RHIDummySwapChain()
    {
        Destroy();
    }   

    void RHIDummySwapChain::Destroy()
    {
        size_t count = m_aDummyTextures.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aDummyTextures[i])
        }
        m_aDummyTextures.clear();
    }

    RHITexture* RHIDummySwapChain::GetTexture(uint8 nIndex)
    {
        F_Assert("RHIDummySwapChain::GetTexture" && nIndex < (uint8)m_aDummyTextures.size());
        return m_aDummyTextures[nIndex];
    }

    uint8 RHIDummySwapChain::AcquireBackTexture()
    {
        m_bIsPingPong = !m_bIsPingPong;
        return m_bIsPingPong ? 0 : 1;
    }

    void RHIDummySwapChain::Present()
    {

    }

}; //LostPeterPluginRHIDummy