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
    ObjectCloud::ObjectCloud(const String& nameCloud, Scene* pScene)
        : Object(nameCloud, pScene)
    {
        this->m_typeObject = Vulkan_Object_Cloud;
    }
    ObjectCloud::~ObjectCloud()
    {

    }
    
}; //LostPeter