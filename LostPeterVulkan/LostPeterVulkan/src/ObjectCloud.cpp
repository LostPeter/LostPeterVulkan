/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ObjectCloud.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    const String ObjectCloud::ms_strMovableType = "Movable_Cloud";

    ObjectCloud::ObjectCloud(const String& nameCloud, Scene* pScene)
        : Object(nameCloud, pScene)
    {
        this->m_typeObject = Vulkan_Object_Cloud;
    }
    ObjectCloud::~ObjectCloud()
    {

    }



    const String& ObjectCloud::GetMovableType() const
    {
        return ms_strMovableType;
    }
    const FAABB& ObjectCloud::GetBoundingAABB() const
    {
        return m_aabb;
    }
    float ObjectCloud::GetBoundingRadius() const
    {
        return 0.0f;
    }
    
}; //LostPeter