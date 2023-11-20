/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class utilExport ObjectManager : public FSingleton<ObjectManager>
                                   , public Base
    {
    public:
        ObjectManager();
        virtual ~ObjectManager();

    public:


    public:
        static ObjectManager&	GetSingleton();
		static ObjectManager*	GetSingletonPtr();

    public:
        void Destroy();
        bool Init();

    };

}; //LostPeterEngine

#endif