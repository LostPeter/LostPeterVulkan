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

#include "../include/ObjectWater.h"

namespace LostPeterEngine
{
    const String ObjectWater::ms_strMovableType = "Movable_Water";

    ObjectWater::ObjectWater(const String& nameWater, Scene* pScene)
        : Object(nameWater, pScene)
    {
        this->m_typeObject = E_Object_Water;
    }
    ObjectWater::~ObjectWater()
    {

    }
    

    const String& ObjectWater::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectWater::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectWater::GetBoundingRadius() const
    {
        return 0.0f;
    }

}; //LostPeterEngine