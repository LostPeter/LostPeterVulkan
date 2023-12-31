/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-31
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RESOURCE_GROUP_MANAGER_H_
#define _RESOURCE_GROUP_MANAGER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport ResourceGroupManager : public FSingleton<ResourceGroupManager>
                                            , public Base
    {
    public:
        ResourceGroupManager();
        virtual ~ResourceGroupManager();

    public:

    public:
        static ResourceGroupManager& GetSingleton();
		static ResourceGroupManager* GetSingletonPtr();
        

    public:
        void Destroy();


    };

}; //LostPeterEngine

#endif