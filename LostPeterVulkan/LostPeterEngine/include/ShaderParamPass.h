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

#ifndef _SHADER_PARAM_PASS_H_
#define _SHADER_PARAM_PASS_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderParamPass : public Base
    {
    public:
        ShaderParamPass(const String& strNameShaderParamPass);
        virtual ~ShaderParamPass();

    public: 
    protected:
        FShaderPassType m_eShaderPass;

    public:
        F_FORCEINLINE FShaderPassType GetShaderPassType() const { return m_eShaderPass; }


    public: 
        

    protected:

    };

}; //LostPeterEngine

#endif