/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/CullUnitObject.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    CullUnitObject::CullUnitObject(const String& nameUnit)
        : CullUnit(nameUnit)
    {
        this->typeCullUnit = Vulkan_CullUnit_Object;
    }

    CullUnitObject::~CullUnitObject()
    {

    }
    
}; //LostPeterVulkan