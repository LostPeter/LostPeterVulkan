/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
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

namespace LostPeter
{
    class utilExport ShaderManager : public FSingleton<ShaderManager>
                                   , public Base
    {
    public:
        ShaderManager();
        virtual ~ShaderManager();

    public:

    public:
        static ShaderManager& GetSingleton();
		static ShaderManager* GetSingletonPtr();

    };

}; //LostPeter

#endif