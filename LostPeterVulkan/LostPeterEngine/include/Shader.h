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

#ifndef _SHADER_H_
#define _SHADER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Shader : public Base
    {
    public:
        Shader(uint32 _group,
               const String& _nameShader,
               const String& _pathShader,
               FShaderType _typeShader);
        virtual ~Shader();

    public:
        String pathShader;
        FShaderType typeShader;

    public:
        

    public:
        void Destroy();

        virtual bool LoadShader();

    };

}; //LostPeterEngine

#endif