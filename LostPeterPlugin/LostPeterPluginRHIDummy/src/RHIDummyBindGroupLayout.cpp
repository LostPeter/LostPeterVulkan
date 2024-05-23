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

#include "../include/RHIDummyBindGroupLayout.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroupLayout::RHIDummyBindGroupLayout(RHIDummyDevice* pDummyDevice, const RHIBindGroupLayoutCreateInfo& createInfo)
        : RHIBindGroupLayout(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBindGroupLayout::RHIDummyBindGroupLayout")
    }

    RHIDummyBindGroupLayout::~RHIDummyBindGroupLayout()
    {
        Destroy();
    }
    
    void RHIDummyBindGroupLayout::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy