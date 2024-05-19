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
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummySurface::RHIDummySurface(RHIDummyDevice* pDummyDevice, const RHISurfaceCreateInfo& createInfo)
        : RHISurface(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummySurface::RHIDummySurface")
    }

    RHIDummySurface::~RHIDummySurface()
    {
        Destroy();
    }   

    void RHIDummySurface::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy