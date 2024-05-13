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

#ifndef _RHI_PIPELINE_GRAPHICS_H_
#define _RHI_PIPELINE_GRAPHICS_H_

#include "RHIPipeline.h"

namespace LostPeterRHI
{
    class rhiExport RHIPipelineGraphics : public RHIPipeline
    {
    public:
        RHIPipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo);
        virtual ~RHIPipelineGraphics();

    public:
    protected:


    public:


    };
    
}; //LostPeterRHI

#endif