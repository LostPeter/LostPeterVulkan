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

#include "../include/RenderPassVolumeFog.h"

namespace LostPeterEngine
{
    RenderPassVolumeFog::RenderPassVolumeFog()
        : RenderPass(F_GetRenderPassTypeName(F_RenderPass_VolumeFog), 
                     F_RenderPass_VolumeFog, 
                     F_GetRenderPassTypeName(F_RenderPass_VolumeFog))
    {

    }
    
    RenderPassVolumeFog::~RenderPassVolumeFog()
    {

    }

    

}; //LostPeterEngine