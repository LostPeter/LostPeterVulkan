/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHITextureView.h"
#include "../include/RHITexture.h"

namespace LostPeterRHI
{
    RHITextureView::RHITextureView(RHITexture* pTexture, const RHITextureViewCreateInfo& createInfo)
        : m_pTexture(pTexture)
    {
        F_Assert(m_pTexture && "RHITextureView::RHITextureView")
    }

    RHITextureView::~RHITextureView()
    {
        
    }
    

}; //LostPeterRHI