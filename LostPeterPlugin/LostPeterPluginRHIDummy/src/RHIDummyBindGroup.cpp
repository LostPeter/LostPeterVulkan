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
#include "../include/RHIDummyBindGroupPool.h"
#include "../include/RHIDummyBindGroupLayout.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroup::RHIDummyBindGroup(RHIDummyDevice* pDummyDevice, const RHIBindGroupCreateInfo& createInfo)
        : RHIBindGroup(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
        , m_pDummyBindGroupPool((RHIDummyBindGroupPool*)createInfo.pBindGroupPool)
        , m_pDummyBindGroupLayout((RHIDummyBindGroupLayout*)createInfo.pBindGroupLayout)
    {
        F_Assert(m_pDummyDevice && m_pDummyBindGroupPool && m_pDummyBindGroupLayout && "RHIDummyBindGroup::RHIDummyBindGroup")
    }

    RHIDummyBindGroup::~RHIDummyBindGroup()
    {
        Destroy();
    }
    
    void RHIDummyBindGroup::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy