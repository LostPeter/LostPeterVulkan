// author: LostPeter
// time:   2022-11-05

#include "PreInclude.h"
#include "vulkan_001_triangle.h"


int main()
{
    Vulkan_001_Triangle sample(1280, 720, "LostPeter - Vulkan_001_Triangle");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}