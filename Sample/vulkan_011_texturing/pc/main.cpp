/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-01-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_011_texturing.h"


int main()
{
    Vulkan_011_Texturing sample(1280, 720, "LostPeter - Vulkan_011_Texturing");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}