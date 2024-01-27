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

namespace LostPeterEngine
{
    ShaderParamPassDepth::ShaderParamPassDepth(const String& strNameShaderParamPassDepth)
        : ShaderParamPass(strNameShaderParamPassDepth)
    {
        m_eShaderPass = F_ShaderPass_Depth;
    }

    ShaderParamPassDepth::~ShaderParamPassDepth()
    {

    }

    

}; //LostPeterEngine