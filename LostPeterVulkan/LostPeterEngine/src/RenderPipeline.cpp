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

#include "../include/RenderPipeline.h"

namespace LostPeterEngine
{
    RenderPipeline::RenderPipeline(const String& nameRenderPipeline, RenderPipelineManager* pRenderPipelineManager, FRenderPipelineType eRenderPipelineType)
        : Base(nameRenderPipeline)
        , m_pRenderPipelineManager(pRenderPipelineManager)
		, m_eRenderPipelineType(eRenderPipelineType)
    {

    }

    RenderPipeline::~RenderPipeline()
    {

    }

    bool RenderPipeline::RenderOneFrame(RenderFrameEvent& event)
    {



        return true;
    }

}; //LostPeterEngine