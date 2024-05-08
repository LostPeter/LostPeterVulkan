/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DEVICE_H_
#define _RHI_DEVICE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIDevice
    {
    public:
        RHIDevice();
        virtual ~RHIDevice();

    public:
    protected:


    public:
        virtual void Destroy() = 0;
        virtual uint32 GetQueueCount(RHIQueueType eQueue) = 0;
        virtual RHIQueue* GetQueue(RHIQueueType eQueue, uint32 nIndex) = 0;
        virtual RHISurface* CreateSurface(const RHISurfaceCreateInfo& createInfo) = 0;
        virtual RHISwapChain* CreateSwapChain(const RHISwapChainCreateInfo& createInfo) = 0;
    };

}; //LostPeterRHI

#endif