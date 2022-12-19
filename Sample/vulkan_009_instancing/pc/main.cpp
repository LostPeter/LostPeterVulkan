// author: LostPeter
// time:   2022-12-20

#include "PreInclude.h"
#include "vulkan_009_instancing.h"


int main()
{
    Vulkan_009_Instancing sample(1280, 720, "LostPeter - Vulkan_009_Instancing");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}