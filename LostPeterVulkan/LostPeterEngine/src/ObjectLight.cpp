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

#include "../include/ObjectLight.h"

namespace LostPeterEngine
{
    const String ObjectLight::ms_strMovableType = "Movable_Light";

    ObjectLight::ObjectLight(const String& nameLight, Scene* pScene)
        : Object(nameLight, pScene)
    {
        this->m_typeObject = E_Object_Light;
    }

    ObjectLight::~ObjectLight()
    {

    }

    

    const String& ObjectLight::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectLight::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectLight::GetBoundingRadius() const
    {
        return 0.0f;
    }
    
}; //LostPeterEngine