// author: LostPeter
// time:   2022-11-28

#include "PreInclude.h"
#include "vulkan_006_depth.h"


int main()
{
    Vulkan_006_Depth sample(1280, 720, "LostPeter - Vulkan_006_Depth");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}