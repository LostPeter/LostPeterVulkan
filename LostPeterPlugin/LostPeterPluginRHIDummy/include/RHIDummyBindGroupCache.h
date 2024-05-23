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

#ifndef _RHI_DUMMY_BIND_GROUP_CACHE_H_
#define _RHI_DUMMY_BIND_GROUP_CACHE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyBindGroupCache : public RHIBindGroupCache
                                                , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyBindGroupCache(RHIDummyDevice* pDummyDevice, const RHIBindGroupCacheCreateInfo& createInfo);
    public:
        virtual ~RHIDummyBindGroupCache();

    public:
    protected:


    public:


    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIDummy

#endif