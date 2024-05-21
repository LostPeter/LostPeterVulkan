/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIBindGroupCache.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIBindGroupCache::RHIBindGroupCache(RHIDevice* pDevice, const RHIBindGroupCacheCreateInfo& createInfo)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHIBindGroupCache::RHIBindGroupCache")
    }

    RHIBindGroupCache::~RHIBindGroupCache()
    {
        
    }
    

}; //LostPeterRHI