/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-09
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummySampler.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummySampler::RHIDummySampler(RHIDummyDevice* pDummyDevice, const RHISamplerCreateInfo& createInfo)
        : RHISampler(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummySampler::RHIDummySampler")
    }

    RHIDummySampler::~RHIDummySampler()
    {
        Destroy();
    }
    
    void RHIDummySampler::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy