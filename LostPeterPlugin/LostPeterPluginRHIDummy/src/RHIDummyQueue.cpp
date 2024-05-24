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

#include "../include/RHIDummyQueue.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyQueue::RHIDummyQueue(RHIDummyDevice* pDummyDevice, const RHIQueueCreateInfo& createInfo)
        : RHIQueue(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyQueue::RHIDummyQueue")
    }

    RHIDummyQueue::~RHIDummyQueue()
    {
        
    }

    void RHIDummyQueue::Destroy()
    {

    }
    
    void RHIDummyQueue::Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal)
    {

    }

    void RHIDummyQueue::Wait(RHIFence* pFenceToSignal)
    {

    }

}; //LostPeterPluginRHIDummy