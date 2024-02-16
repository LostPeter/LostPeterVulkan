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

#include "../include/ShaderParamPassCustom.h"
#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/ShaderParameter.h"

namespace LostPeterEngine
{
    ShaderParamPassCustom::ShaderParamPassCustom()
        : ShaderParamPass(F_GetShaderPassTypeName(F_ShaderPass_Custom))
    {
        m_eShaderPass = F_ShaderPass_Custom;
    }

    ShaderParamPassCustom::~ShaderParamPassCustom()
    {

    }

    void ShaderParamPassCustom::Destroy()
    {

        ShaderParamPass::Destroy();
    }

    bool ShaderParamPassCustom::createParamPass()
    {
        

        return true;
    }

}; //LostPeterEngine