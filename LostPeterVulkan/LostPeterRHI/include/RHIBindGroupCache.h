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

#ifndef _RHI_BIND_GROUP_CACHE_H_
#define _RHI_BIND_GROUP_CACHE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHIBindGroupCache : public RHIObject
    {
    public:
        RHIBindGroupCache(RHIDevice* pDevice, const RHIBindGroupCacheCreateInfo& createInfo);
        virtual ~RHIBindGroupCache();

    public:
    protected:


    public:
        

    };
    
}; //LostPeterRHI

#endif