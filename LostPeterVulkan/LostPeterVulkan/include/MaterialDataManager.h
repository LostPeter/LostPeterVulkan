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

#ifndef _MATERIAL_DATA_MANAGER_H_
#define _MATERIAL_DATA_MANAGER_H_

#include "Base.h"

namespace LostPeter
{
    class utilExport MaterialDataManager : public FSingleton<MaterialDataManager>
                                         , public Base
    {
    public:
        MaterialDataManager();
        virtual ~MaterialDataManager();

    public:


    public:
        static MaterialDataManager&	GetSingleton();
		static MaterialDataManager*	GetSingletonPtr();

    };

}; //LostPeter

#endif