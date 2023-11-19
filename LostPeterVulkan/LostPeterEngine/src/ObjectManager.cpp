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

#include "../include/ObjectManager.h"
#include "../include/Object.h"

template<> LostPeter::ObjectManager* LostPeterFoundation::FSingleton<LostPeter::ObjectManager>::ms_Singleton = nullptr;

namespace LostPeter
{
    ObjectManager* ObjectManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	ObjectManager& ObjectManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "ObjectManager::GetSingleton")
		return (*ms_Singleton);     
	}

    ObjectManager::ObjectManager()
        : Base("ObjectManager")
    {

    }
    ObjectManager::~ObjectManager()
    {
        Destroy();
    }
    void ObjectManager::Destroy()
    {

    }

    bool ObjectManager::Init()
    {

        return true;
    }

}; //LostPeter