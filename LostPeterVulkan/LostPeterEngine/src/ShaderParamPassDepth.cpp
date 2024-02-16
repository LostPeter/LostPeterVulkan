/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderParamPassDepth.h"
#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/ShaderParameter.h"

namespace LostPeterEngine
{
    ShaderParamPassDepth::ShaderParamPassDepth()
        : ShaderParamPass(F_GetShaderPassTypeName(F_ShaderPass_Depth))
    {
        m_eShaderPass = F_ShaderPass_Depth;
    }

    ShaderParamPassDepth::~ShaderParamPassDepth()
    {

    }

    void ShaderParamPassDepth::Destroy()
    {

        ShaderParamPass::Destroy();
    }

    bool ShaderParamPassDepth::createParamPass()
    {
        

        return true;
    }

}; //LostPeterEngine