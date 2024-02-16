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

#include "../include/ShaderParamPassMain.h"
#include "../include/ShaderParamPassManager.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/ShaderParameter.h"

namespace LostPeterEngine
{
    ShaderParamPassMain::ShaderParamPassMain()
        : ShaderParamPass(F_GetShaderPassTypeName(F_ShaderPass_Main))
    {
        m_eShaderPass = F_ShaderPass_Main;
    }

    ShaderParamPassMain::~ShaderParamPassMain()
    {

    }

    void ShaderParamPassMain::Destroy()
    {

        ShaderParamPass::Destroy();
    }

    bool ShaderParamPassMain::createParamPass()
    {
        m_pShaderParameter = new ShaderParameter();
        

        return true;
    }

}; //LostPeterEngine