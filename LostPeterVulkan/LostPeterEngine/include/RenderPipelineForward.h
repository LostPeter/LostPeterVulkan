/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PIPELINE_FORWARD_H_
#define _RENDER_PIPELINE_FORWARD_H_

#include "RenderPipeline.h"

namespace LostPeterEngine
{
    class engineExport RenderPipelineForward : public RenderPipeline
    {
    public:
        RenderPipelineForward(RenderPipelineManager* pRenderPipelineManager);
        virtual ~RenderPipelineForward();

    public:
        virtual bool Init();

    };

}; //LostPeterEngine

#endif