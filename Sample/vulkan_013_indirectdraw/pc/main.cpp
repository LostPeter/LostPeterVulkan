/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-04-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_013_indirectdraw.h"


int main()
{
    Vulkan_013_IndirectDraw sample(1280, 720, "LostPeter - Vulkan_013_IndirectDraw");
    return App::Run(&sample);
}