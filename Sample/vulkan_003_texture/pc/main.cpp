// author: LostPeter
// time:   2022-11-12

#include "PreInclude.h"
#include "vulkan_003_texture.h"


int main()
{
    Vulkan_003_Texture sample(1280, 720, "LostPeter - Vulkan_003_Texture");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}