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

#include "../include/RHIDummyRenderPassCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyRenderPassCache::RHIDummyRenderPassCache(RHIDummyDevice* pDummyDevice, const RHIRenderPassCacheCreateInfo& createInfo)
        : RHIRenderPassCache(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyRenderPassCache::RHIDummyRenderPassCache")
    }

    RHIDummyRenderPassCache::~RHIDummyRenderPassCache()
    {
        Destroy();
    }
    
    void RHIDummyRenderPassCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy