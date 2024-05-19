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

#include "../include/RHIDummyTextureView.h"
#include "../include/RHIDummyTexture.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyTextureView::RHIDummyTextureView(RHIDummyTexture* pDummyTexture, const RHITextureViewCreateInfo& createInfo)
        : RHITextureView(pDummyTexture, createInfo)
        , m_pDummyTexture(pDummyTexture)
    {
        F_Assert(m_pDummyTexture && "RHIDummyTextureView::RHIDummyTextureView")
    }

    RHIDummyTextureView::~RHIDummyTextureView()
    {
        Destroy();
    }
    
    void RHIDummyTextureView::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy