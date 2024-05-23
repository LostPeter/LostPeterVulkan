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

#include "../include/RHIDummyShaderModuleCache.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyShaderModuleCache::RHIDummyShaderModuleCache(RHIDummyDevice* pDummyDevice, const RHIShaderModuleCacheCreateInfo& createInfo)
        : RHIShaderModuleCache(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyShaderModuleCache::RHIDummyShaderModuleCache")
    }

    RHIDummyShaderModuleCache::~RHIDummyShaderModuleCache()
    {
        Destroy();
    }
    
    void RHIDummyShaderModuleCache::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy