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

#ifndef _RHI_RENDER_PASS_CACHE_H_
#define _RHI_RENDER_PASS_CACHE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIRenderPassCache : public RHIObject
    {
    public:
        RHIRenderPassCache(RHIDevice* pDevice, const RHIRenderPassCacheCreateInfo& createInfo);
        virtual ~RHIRenderPassCache();

    public:
    protected:


    public:
        

    };
    
}; //LostPeterRHI

#endif