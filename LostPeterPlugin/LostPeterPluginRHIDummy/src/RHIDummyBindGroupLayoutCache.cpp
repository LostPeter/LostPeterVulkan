/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyBindGroupLayoutCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroupLayoutCache::RHIDummyBindGroupLayoutCache(RHIDummyDevice* pDummyDevice, const RHIBindGroupLayoutCacheCreateInfo& createInfo)
        : RHIBindGroupLayoutCache(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBindGroupLayoutCache::RHIDummyBindGroupLayoutCache")
    }

    RHIDummyBindGroupLayoutCache::~RHIDummyBindGroupLayoutCache()
    {
        Destroy();
    }
    
    void RHIDummyBindGroupLayoutCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy