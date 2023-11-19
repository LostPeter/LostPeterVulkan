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

#include "../include/ObjectMesh.h"

namespace LostPeter
{
    const String ObjectMesh::ms_strMovableType = "Movable_Mesh";

    ObjectMesh::ObjectMesh(const String& nameMesh, Scene* pScene)
        : Object(nameMesh, pScene)
    {
        this->m_typeObject = Vulkan_Object_Mesh;
    }
    ObjectMesh::~ObjectMesh()
    {

    }



    const String& ObjectMesh::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectMesh::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectMesh::GetBoundingRadius() const
    {
        return 0.0f;
    }
    
}; //LostPeter