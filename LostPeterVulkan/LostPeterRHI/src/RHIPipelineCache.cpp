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

#include "../include/RHIPipelineCache.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIPipelineCache::RHIPipelineCache(RHIDevice* pDevice, const RHIPipelineCacheCreateInfo& createInfo)
        : RHIObject(pDevice)
    {
        m_eObject = RHIObjectType::RHI_Object_PipelineCache;
    }

    RHIPipelineCache::~RHIPipelineCache()
    {
        
    }
    

}; //LostPeterRHI