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

#include "../include/RHIBufferView.h"
#include "../include/RHIBuffer.h"

namespace LostPeterRHI
{
    RHIBufferView::RHIBufferView(RHIBuffer* pBuffer, const RHIBufferViewCreateInfo& createInfo)
        : m_pBuffer(pBuffer)
    {
        F_Assert(m_pBuffer && "RHIBufferView::RHIBufferView")
    }

    RHIBufferView::~RHIBufferView()
    {
        
    }
    

}; //LostPeterRHI