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

namespace LostPeterPluginRHIDummy
{
    RHIDummyTextureView::RHIDummyTextureView(const RHITextureViewCreateInfo& createInfo)
        : RHITextureView(createInfo)
    {

    }

    RHIDummyTextureView::~RHIDummyTextureView()
    {
        Destroy();
    }
    
    void RHIDummyTextureView::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy