// author: LostPeter
// time:   2022-11-15

#include "preinclude.h"
#include "vulkan_004_model.h"


int main()
{
    Vulkan_004_Model sample(1280, 720, "LostPeter - Vulkan_004_Model");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}