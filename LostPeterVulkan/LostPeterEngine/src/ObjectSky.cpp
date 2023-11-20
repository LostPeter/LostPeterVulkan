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

#include "../include/ObjectSky.h"

namespace LostPeterEngine
{
    const String ObjectSky::ms_strMovableType = "Movable_Sky";

    ObjectSky::ObjectSky(const String& nameSky, Scene* pScene)
        : Object(nameSky, pScene)
    {
        this->m_typeObject = Vulkan_Object_Sky;
    }
    ObjectSky::~ObjectSky()
    {

    }

    

    const String& ObjectSky::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectSky::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectSky::GetBoundingRadius() const
    {
        return 0.0f;
    }
    
}; //LostPeterEngine