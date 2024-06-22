/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-26
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_019_shadowmap.h"


int main()
{
    Vulkan_019_ShadowMap sample(1280, 720, "LostPeter - Vulkan_019_ShadowMap");
    return App::Run(&sample);
}