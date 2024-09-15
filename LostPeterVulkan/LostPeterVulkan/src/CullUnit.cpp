/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-09-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/CullUnit.h"
#include "../include/VulkanWindow.h"

namespace LostPeterVulkan
{
    CullUnit::CullUnit(const String& nameUnit)
        : Base(nameUnit)
    {

    }
    CullUnit::~CullUnit()
    {
        Destroy();
    }

    void CullUnit::Destroy()
    {
       
    }
    
}; //LostPeterVulkan