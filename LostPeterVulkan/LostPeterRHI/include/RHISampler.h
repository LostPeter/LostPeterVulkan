/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-09
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_SAMPLER_H_
#define _RHI_SAMPLER_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHISampler : public FNonCopyable
    {
    public:
        RHISampler(const RHISamplerCreateInfo& createInfo);
        virtual ~RHISampler();

    public:
    protected:


    public:
        virtual void Destroy() = 0;

    };

}; //LostPeterRHI

#endif