/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Shader.h"

namespace LostPeter
{
    Shader::Shader(uint32 _group,
                   const String& _nameShader,
                   const String& _pathShader,
                   FShaderType _typeShader)
        : Base(_group, _nameShader)
        , pathShader(_pathShader)
        , typeShader(_typeShader)
    {

    }
    Shader::~Shader()
    {

    }   

    void Shader::Destroy()
    {
       
    }

    bool Shader::LoadShader()
    {
        
        
        return true;
    }

}; //LostPeter