// author: LostPeter
// time:   2022-12-17

#include "PreInclude.h"
#include "vulkan_008_blend.h"


int main()
{
    Vulkan_008_Blend sample(1280, 720, "LostPeter - Vulkan_008_Blend");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}