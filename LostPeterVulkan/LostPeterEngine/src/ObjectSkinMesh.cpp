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

#include "../include/ObjectSkinMesh.h"

namespace LostPeter
{
    const String ObjectSkinMesh::ms_strMovableType = "Movable_SkinMesh";

    ObjectSkinMesh::ObjectSkinMesh(const String& nameSkinMesh, Scene* pScene)
        : Object(nameSkinMesh, pScene)
    {
        this->m_typeObject = Vulkan_Object_SkinMesh;
    }
    ObjectSkinMesh::~ObjectSkinMesh()
    {

    }



    const String& ObjectSkinMesh::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectSkinMesh::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectSkinMesh::GetBoundingRadius() const
    {
        return 0.0f;
    }

}; //LostPeter