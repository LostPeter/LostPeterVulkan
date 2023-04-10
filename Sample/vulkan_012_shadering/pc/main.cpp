/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_012_shadering.h"


int main()
{
    Vulkan_012_Shadering sample(1280, 720, "LostPeter - Vulkan_012_Shadering");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}