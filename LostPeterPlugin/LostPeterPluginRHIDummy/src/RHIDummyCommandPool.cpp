/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyCommandPool.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyCommandPool::RHIDummyCommandPool(RHIDummyDevice* pDummyDevice, const RHICommandPoolCreateInfo& createInfo)
        : RHICommandPool(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyCommandPool::RHIDummyCommandPool")
    }

    RHIDummyCommandPool::~RHIDummyCommandPool()
    {
        Destroy();
    }  
    
    void RHIDummyCommandPool::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy