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

#include "../include/RenderPipelineForward.h"

namespace LostPeterEngine
{
    RenderPipelineForward::RenderPipelineForward(RenderPipelineManager* pRenderPipelineManager)
        : RenderPipeline("RenderPipelineForward", pRenderPipelineManager, E_RenderPipeline_Forward)
    {

    }
    
    RenderPipelineForward::~RenderPipelineForward()
    {

    }

    bool RenderPipelineForward::Init()
    {
        return true;
    }

}; //LostPeterEngine