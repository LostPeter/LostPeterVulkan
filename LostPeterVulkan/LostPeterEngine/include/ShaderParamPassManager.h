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

#ifndef _SHADER_PARAM_PASS_MANAGER_H_
#define _SHADER_PARAM_PASS_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderParamPassManager : public FSingleton<ShaderParamPassManager>
    {
    public:
        ShaderParamPassManager();
        virtual ~ShaderParamPassManager();

    public: 
    protected:
        
        
    public:
        static ShaderParamPassManager& GetSingleton();
		static ShaderParamPassManager* GetSingletonPtr();

    public:
        virtual void Destroy();
        bool Init();

        
    protected:



    };

}; //LostPeterEngine

#endif