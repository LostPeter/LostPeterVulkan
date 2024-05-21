/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyBindGroupPool.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroupPool::RHIDummyBindGroupPool(RHIDummyDevice* pDummyDevice, const RHIBindGroupPoolCreateInfo& createInfo)
        : RHIBindGroupPool(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBindGroupPool::RHIDummyBindGroupPool")
    }

    RHIDummyBindGroupPool::~RHIDummyBindGroupPool()
    {
        Destroy();
    }
    
    void RHIDummyBindGroupPool::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy