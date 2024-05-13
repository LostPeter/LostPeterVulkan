/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_PIPELINE_COMPUTE_H_
#define _RHI_PIPELINE_COMPUTE_H_

#include "RHIPipeline.h"

namespace LostPeterRHI
{
    class rhiExport RHIPipelineCompute : public RHIPipeline
    {
    public:
        RHIPipelineCompute(const RHIPipelineComputeCreateInfo& createInfo);
        virtual ~RHIPipelineCompute();

    public:
    protected:


    public:


    };
    
}; //LostPeterRHI

#endif