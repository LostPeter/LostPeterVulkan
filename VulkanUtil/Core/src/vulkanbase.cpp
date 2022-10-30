#include "../include/preinclude.h"
#include "../include/vulkanbase.h"

namespace LibUtil
{
     VulkanBase::VulkanBase(int width, int height, std::string name)
        : width(width)
        , height(height)
        , nameTitle(name)
        , pWindow(nullptr)

        , isAppPaused(false)
        , isMinimized(false)
        , isMaximized(false)
        , isResizing(false)
        , isFullscreenState(false)
        , isMinimizedWindowNeedClose(false)

        , isCreateDevice(false)
        , isLoadAsset(false)

    {
        this->pathAssets = VulkanUtil::GetAssetsPath();
        std::cout << "Path GetAssetsPath: " << this->pathAssets << std::endl; 

        RefreshAspectRatio();
    }

    VulkanBase::~VulkanBase()
    {
        
    }

    float VulkanBase::RefreshAspectRatio()
    {
        this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        return this->aspectRatio;
    }

    void VulkanBase::CalculateFrameStats(GLFWwindow* s_pWindow)
    {

    }

    std::string VulkanBase::GetAssetFullPath(const std::string& assetName)
    {
        return this->pathAssets + assetName;
    }

}; //LibUtil