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

#include "../include/RHIDummyBufferView.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBufferView::RHIDummyBufferView(const RHIBufferViewCreateInfo& createInfo)
        : RHIBufferView(createInfo)
    {

    }

    RHIDummyBufferView::~RHIDummyBufferView()
    {
        Destroy();
    }
    
    void RHIDummyBufferView::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy