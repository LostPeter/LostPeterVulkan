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

#include "../include/RenderPassForwardLit.h"

namespace LostPeterEngine
{
    RenderPassForwardLit::RenderPassForwardLit()
        : RenderPass(F_GetRenderPassTypeName(F_RenderPass_ForwardLit), 
                     F_RenderPass_ForwardLit, 
                     F_GetRenderPassTypeName(F_RenderPass_ForwardLit))
    {

    }
    
    RenderPassForwardLit::~RenderPassForwardLit()
    {

    }

    

}; //LostPeterEngine