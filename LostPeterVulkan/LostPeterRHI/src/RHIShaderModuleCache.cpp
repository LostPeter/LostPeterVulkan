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

#include "../include/RHIShaderModuleCache.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIShaderModuleCache::RHIShaderModuleCache(RHIDevice* pDevice, const RHIShaderModuleCacheCreateInfo& createInfo)   
        : RHIObject(pDevice)
    {
        setObjectType(RHIObjectType::RHI_Object_ShaderModuleCache);
    }

    RHIShaderModuleCache::~RHIShaderModuleCache()
    {
        
    }
    

}; //LostPeterRHI