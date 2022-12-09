// author: LostPeter
// time:   2022-11-21

#include "PreInclude.h"
#include "vulkan_005_camera.h"


int main()
{
    Vulkan_005_Camera sample(1280, 720, "LostPeter - Vulkan_005_Camera");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}