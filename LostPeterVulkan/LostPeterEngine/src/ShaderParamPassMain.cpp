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

namespace LostPeterEngine
{
    ShaderParamPassMain::ShaderParamPassMain(const String& strNameShaderParamPassMain)
        : ShaderParamPass(strNameShaderParamPassMain)
    {
        m_eShaderPass = F_ShaderPass_Main;
    }

    ShaderParamPassMain::~ShaderParamPassMain()
    {

    }



}; //LostPeterEngine