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

#ifndef _RHI_DUMMY_BIND_GROUP_LAYOUT_CACHE_H_
#define _RHI_DUMMY_BIND_GROUP_LAYOUT_CACHE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyBindGroupLayoutCache : public RHIBindGroupLayoutCache
                                                      , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyBindGroupLayoutCache(RHIDummyDevice* pDummyDevice, const RHIBindGroupLayoutCacheCreateInfo& createInfo);
    public:
        virtual ~RHIDummyBindGroupLayoutCache();

    public:
    protected:


    public:


    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIDummy

#endif