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

#ifndef _RHI_QUEUE_H_
#define _RHI_QUEUE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIQueue : public FNonCopyable
    {
    public:
        RHIQueue();
        virtual ~RHIQueue();

    public:
    protected:


    public:
        virtual void Submit(RHICommandBuffer* pCommandBuffer, RHIFence* pFenceToSignal) = 0;
        virtual void Wait(RHIFence* pFenceToSignal) = 0;

    };
    
}; //LostPeterRHI

#endif