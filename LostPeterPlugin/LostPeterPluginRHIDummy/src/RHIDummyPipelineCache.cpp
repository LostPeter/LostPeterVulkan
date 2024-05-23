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

#include "../include/RHIDummyPipelineCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPipelineCache::RHIDummyPipelineCache(RHIDummyDevice* pDummyDevice, const RHIPipelineCacheCreateInfo& createInfo)
        : RHIPipelineCache(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyPipelineCache::RHIDummyPipelineCache")
    }

    RHIDummyPipelineCache::~RHIDummyPipelineCache()
    {
        Destroy();
    }
    
    void RHIDummyPipelineCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy