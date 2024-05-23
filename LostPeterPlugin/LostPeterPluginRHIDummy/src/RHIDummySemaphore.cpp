/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummySemaphore.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummySemaphore::RHIDummySemaphore(RHIDummyDevice* pDummyDevice, const RHISemaphoreCreateInfo& createInfo)
        : RHISemaphore(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummySemaphore::RHIDummySemaphore")
    }

    RHIDummySemaphore::~RHIDummySemaphore()
    {
        
    }
    
    void RHIDummySemaphore::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy