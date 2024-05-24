/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_QUEUE_H_
#define _RHI_DUMMY_QUEUE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyQueue : public RHIQueue
                                       , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyQueue(RHIDummyDevice* pDummyDevice, const RHIQueueCreateInfo& createInfo);
    public:
        virtual ~RHIDummyQueue();

    public:
    protected:


    public:


    public:
        virtual void Destroy();

        virtual void Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal);
        virtual void Wait(RHIFence* pFenceToSignal);
    };
    
}; //LostPeterPluginRHIDummy

#endif