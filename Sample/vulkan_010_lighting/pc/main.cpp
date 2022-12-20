// author: LostPeter
// time:   2022-12-24

#include "PreInclude.h"
#include "vulkan_010_lighting.h"


int main()
{
    Vulkan_010_Lighting sample(1280, 720, "LostPeter - Vulkan_010_Lighting");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}