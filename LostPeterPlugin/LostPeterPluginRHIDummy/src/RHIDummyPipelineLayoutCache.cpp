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

#include "../include/RHIDummyPipelineLayoutCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPipelineLayoutCache::RHIDummyPipelineLayoutCache(RHIDummyDevice* pDummyDevice, const RHIPipelineLayoutCacheCreateInfo& createInfo)
        : RHIPipelineLayoutCache(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyPipelineLayoutCache::RHIDummyPipelineLayoutCache")
    }

    RHIDummyPipelineLayoutCache::~RHIDummyPipelineLayoutCache()
    {
        Destroy();
    }
    
    void RHIDummyPipelineLayoutCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy