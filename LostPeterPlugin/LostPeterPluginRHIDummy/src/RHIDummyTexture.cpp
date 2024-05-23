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

#include "../include/RHIDummyTexture.h"
#include "../include/RHIDummyTextureView.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyTexture::RHIDummyTexture(RHIDummyDevice* pDummyDevice, const RHITextureCreateInfo& createInfo)
        : RHITexture(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
        , m_pDummyTextureView(nullptr)
    {
        F_Assert(m_pDummyDevice && "RHIDummyTexture::RHIDummyTexture")
    }

    RHIDummyTexture::~RHIDummyTexture()
    {
        Destroy();
    }
    
    void RHIDummyTexture::Destroy()
    {
        DestroyTextureView();
    }

    void RHIDummyTexture::DestroyTextureView()
    {
        F_DELETE(m_pDummyTextureView)
        m_pTextureView = nullptr;
    }

    RHITextureView* RHIDummyTexture::CreateTextureView(const RHITextureViewCreateInfo& createInfo)
    {
        if (m_pDummyTextureView != nullptr)
        {
            return m_pDummyTextureView;
        }
        m_pDummyTextureView = new RHIDummyTextureView(this, createInfo);
        m_pTextureView = m_pDummyTextureView;
        return m_pDummyTextureView;
    }

}; //LostPeterPluginRHIDummy