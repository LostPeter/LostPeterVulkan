/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIPipelineLayoutCache.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIPipelineLayoutCache::RHIPipelineLayoutCache(RHIDevice* pDevice, const RHIPipelineLayoutCacheCreateInfo& createInfo)
        : RHIObject(pDevice)
    {
        setObjectType(RHIObjectType::RHI_Object_PipelineLayoutCache);
    }

    RHIPipelineLayoutCache::~RHIPipelineLayoutCache()
    {
        
    }
    

}; //LostPeterRHI