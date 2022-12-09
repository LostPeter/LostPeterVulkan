// author: LostPeter
// time:   2022-10-30

#include "PreInclude.h"
#include "vulkan_000_window.h"


int main()
{
    Vulkan_000_Window sample(1280, 720, "LostPeter - Vulkan_000_Window");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}