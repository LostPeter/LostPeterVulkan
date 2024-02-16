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

#ifndef _SHADER_PARAM_PASS_DEPTH_H_
#define _SHADER_PARAM_PASS_DEPTH_H_

#include "ShaderParamPass.h"

namespace LostPeterEngine
{
    class engineExport ShaderParamPassDepth : public ShaderParamPass
    {
        friend class ShaderParamPassManager;

    private:
        ShaderParamPassDepth();
    public:
        virtual ~ShaderParamPassDepth();

    public: 
    protected:

    public:
        virtual void Destroy();

    protected:
        virtual bool createParamPass();
    };

}; //LostPeterEngine

#endif