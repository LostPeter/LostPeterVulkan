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

#include "../include/RHIDummySurface.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummySurface::RHIDummySurface(const RHISurfaceCreateInfo& createInfo)
        : RHISurface(createInfo)
    {

    }

    RHIDummySurface::~RHIDummySurface()
    {
        Destroy();
    }   

    void RHIDummySurface::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy