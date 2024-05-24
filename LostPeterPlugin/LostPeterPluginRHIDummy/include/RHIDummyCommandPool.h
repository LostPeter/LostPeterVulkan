/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_COMMAND_POOL_H_
#define _RHI_DUMMY_COMMAND_POOL_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyCommandPool : public RHICommandPool
                                             , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyCommandPool(RHIDummyDevice* pDummyDevice, const RHICommandPoolCreateInfo& createInfo);
    public:
        virtual ~RHIDummyCommandPool();

    public:
    protected:


    public:


    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIDummy

#endif