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

#include "../include/RHIDummyGraphicsState.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyGraphicsState::RHIDummyGraphicsState(RHIDummyDevice* pDummyDevice)
        : RHIGraphicsState(pDummyDevice)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyGraphicsState::RHIDummyGraphicsState")
    }

    RHIDummyGraphicsState::~RHIDummyGraphicsState()
    {
        Destroy();
    }
    
    void RHIDummyGraphicsState::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy