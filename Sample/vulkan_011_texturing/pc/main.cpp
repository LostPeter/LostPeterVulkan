// author: LostPeter
// time:   2022-01-02

#include "PreInclude.h"
#include "vulkan_011_texturing.h"


int main()
{
    Vulkan_011_Texturing sample(1280, 720, "LostPeter - Vulkan_011_Texturing");
    return App::Run(&sample, VulkanWindow::versionVulkan_Major, VulkanWindow::versionVulkan_Minor);
}