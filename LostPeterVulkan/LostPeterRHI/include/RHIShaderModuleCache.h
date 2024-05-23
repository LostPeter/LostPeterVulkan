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

#ifndef _RHI_SHADER_MODULE_CACHE_H_
#define _RHI_SHADER_MODULE_CACHE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIShaderModuleCache : public RHIObject
    {
    public:
        RHIShaderModuleCache(RHIDevice* pDevice, const RHIShaderModuleCacheCreateInfo& createInfo);
        virtual ~RHIShaderModuleCache();

    public:
    protected:


    public:
        

    };

}; //LostPeterRHI

#endif