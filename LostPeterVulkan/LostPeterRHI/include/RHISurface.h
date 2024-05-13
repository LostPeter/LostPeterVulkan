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

#ifndef _RHI_SURFACE_H_
#define _RHI_SURFACE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHISurface : public FNonCopyable
    {
    public:
        RHISurface(const RHISurfaceCreateInfo& createInfo);
        virtual ~RHISurface();

    public:
    protected:


    public:
        virtual void Destroy() = 0;

    };
    
}; //LostPeterRHI

#endif