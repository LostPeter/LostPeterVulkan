/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderPassPostProcess.h"

namespace LostPeterEngine
{
    RenderPassPostProcess::RenderPassPostProcess()
        : RenderPass(F_GetRenderPassTypeName(F_RenderPass_PostProcess), 
                     F_RenderPass_PostProcess, 
                     F_GetRenderPassTypeName(F_RenderPass_PostProcess))
    {

    }
    
    RenderPassPostProcess::~RenderPassPostProcess()
    {

    }

    

}; //LostPeterEngine