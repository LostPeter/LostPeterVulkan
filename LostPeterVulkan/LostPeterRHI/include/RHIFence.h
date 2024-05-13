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

#ifndef _RHI_FENCE_H_
#define _RHI_FENCE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIFence : public FNonCopyable
    {
    public:
        RHIFence(RHIDevice& device);
        virtual ~RHIFence();

    public:
    protected:


    public:
        virtual void Destroy() = 0;

        virtual RHIFenceStatusType GetStatus() = 0;
        virtual void Reset() = 0;
        virtual void Wait() = 0;
    };

}; //LostPeterRHI

#endif