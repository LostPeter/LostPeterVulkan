// author: LostPeter
// time:   2022-11-08

#include "PreInclude.h"
#include "vulkan_002_imgui.h"


int main()
{
    Vulkan_002_Imgui sample(1280, 720, "LostPeter - Vulkan_002_Imgui");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}