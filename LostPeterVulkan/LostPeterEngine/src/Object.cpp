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

#include "../include/Object.h"

namespace LostPeterEngine
{
    Object::Object(const String& nameObject, Scene* pScene)
        : Movable(nameObject, pScene)
    {

    }
    Object::~Object()
    {

    }

}; //LostPeterEngine