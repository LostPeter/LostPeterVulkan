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

#include "../include/RenderPassUI3D.h"

namespace LostPeterEngine
{
    RenderPassUI3D::RenderPassUI3D()
        : RenderPass(F_GetRenderPassTypeName(F_RenderPass_UI3D), 
                     F_RenderPass_UI3D, 
                     F_GetRenderPassTypeName(F_RenderPass_UI3D))
    {

    }
    
    RenderPassUI3D::~RenderPassUI3D()
    {

    }

    

}; //LostPeterEngine