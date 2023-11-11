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

#include "../include/ObjectCamera.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    const String ObjectCamera::ms_strMovableType = "Movable_Camera";

    ObjectCamera::ObjectCamera(const String& nameCamera, Scene* pScene)
        : ObjectFrustum(nameCamera, pScene)
    {
        this->m_typeObject = Vulkan_Object_Camera;
    }

    ObjectCamera::~ObjectCamera()
    {

    }

    const String& ObjectCamera::GetMovableType() const
    {
        return ms_strMovableType;
    }

	float ObjectCamera::GetBoundingRadius() const
    {
        return 0.0f;
    }

}; //LostPeter