/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_PIPELINE_LAYOUT_H_
#define _RHI_PIPELINE_LAYOUT_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIPipelineLayout : public FNonCopyable
    {
    public:
        RHIPipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo);
        virtual ~RHIPipelineLayout();

    public:
    protected:


    public:
        virtual void Destroy() = 0;

    };
    
}; //LostPeterRHI

#endif