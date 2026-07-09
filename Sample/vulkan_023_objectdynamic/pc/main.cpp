/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-08-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_023_objectdynamic.h"


int main()
{
    Vulkan_023_ObjectDynamic sample(1280, 720, "LostPeter - Vulkan_023_ObjectDynamic");
    return App::Run(&sample);
}