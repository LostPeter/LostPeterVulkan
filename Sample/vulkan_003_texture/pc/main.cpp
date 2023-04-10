/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-11-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "PreInclude.h"
#include "vulkan_003_texture.h"


int main()
{
    Vulkan_003_Texture sample(1280, 720, "LostPeter - Vulkan_003_Texture");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}