/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_010_lighting.h"


int main()
{
    Vulkan_010_Lighting sample(1280, 720, "LostPeter - Vulkan_010_Lighting");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}