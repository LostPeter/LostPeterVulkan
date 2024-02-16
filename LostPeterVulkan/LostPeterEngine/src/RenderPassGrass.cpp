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

#include "../include/RenderPassGrass.h"

namespace LostPeterEngine
{
    RenderPassGrass::RenderPassGrass()
        : RenderPass(F_GetRenderPassTypeName(F_RenderPass_Grass), 
                     F_RenderPass_Grass, 
                     F_GetRenderPassTypeName(F_RenderPass_Grass))
    {

    }
    
    RenderPassGrass::~RenderPassGrass()
    {

    }

    

}; //LostPeterEngine