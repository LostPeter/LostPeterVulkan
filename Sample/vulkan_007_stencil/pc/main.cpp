// author: LostPeter
// time:   2022-12-08

#include "PreInclude.h"
#include "vulkan_007_stencil.h"


int main()
{
    Vulkan_007_Stencil sample(1280, 720, "LostPeter - Vulkan_007_Stencil");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}