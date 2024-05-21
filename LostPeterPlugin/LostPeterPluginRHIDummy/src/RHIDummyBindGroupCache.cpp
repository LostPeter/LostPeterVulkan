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

#include "../include/RHIDummyBindGroupCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroupCache::RHIDummyBindGroupCache(RHIDummyDevice* pDummyDevice, const RHIBindGroupCacheCreateInfo& createInfo)
        : RHIBindGroupCache(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBindGroupCache::RHIDummyBindGroupCache")
    }

    RHIDummyBindGroupCache::~RHIDummyBindGroupCache()
    {
        Destroy();
    }
    
    void RHIDummyBindGroupCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy