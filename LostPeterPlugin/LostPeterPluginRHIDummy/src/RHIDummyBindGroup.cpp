/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyBindGroup.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroup::RHIDummyBindGroup(RHIDummyDevice* pDummyDevice, const RHIBindGroupCreateInfo& createInfo)
        : RHIBindGroup(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBindGroup::RHIDummyBindGroup")
    }

    RHIDummyBindGroup::~RHIDummyBindGroup()
    {
        Destroy();
    }
    
    void RHIDummyBindGroup::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy