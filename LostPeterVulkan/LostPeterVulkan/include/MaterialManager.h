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

#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport MaterialManager : public FSingleton<MaterialManager>
                                     , public Base
    {
    public:
        MaterialManager();
        virtual ~MaterialManager();

    public:

    public:
        static MaterialManager&	GetSingleton();
		static MaterialManager*	GetSingletonPtr();

    };

}; //LostPeter

#endif