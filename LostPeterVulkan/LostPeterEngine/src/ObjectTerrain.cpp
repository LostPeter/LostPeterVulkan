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

#include "../include/ObjectTerrain.h"

namespace LostPeterEngine
{
    const String ObjectTerrain::ms_strMovableType = "Movable_Terrain";

    ObjectTerrain::ObjectTerrain(const String& nameObjectTerrain, Scene* pScene)
        : Object(nameObjectTerrain, pScene)
    {
        this->m_typeObject = Vulkan_Object_Terrain;
    }
    ObjectTerrain::~ObjectTerrain()
    {

    }




    const String& ObjectTerrain::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectTerrain::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectTerrain::GetBoundingRadius() const
    {
        return 0.0f;
    }
    
}; //LostPeterEngine