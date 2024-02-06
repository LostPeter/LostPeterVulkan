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

#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ShaderManager : public FSingleton<ShaderManager>
                                     , public Base
    {
    public:
        ShaderManager();
        virtual ~ShaderManager();

    public:
    protected:


    public:
        static ShaderManager& GetSingleton();
		static ShaderManager* GetSingletonPtr();

    public:
        virtual void Destroy();

    public:

    public:
        
    };

}; //LostPeterEngine

#endif