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

        , pTimer(new Timer())
        , fFPS(0.0f)
        , nFrameFPS(0)
        , nFrameTotal(0)
    {
        this->pathBin = VulkanUtil::GetPathBin() + "/";
        std::cout << "Path Bin: " << this->pathBin << std::endl; 

        RefreshAspectRatio();
        fTimeLastFPS = pTimer->GetTimeSinceStart();
    }

    VulkanBase::~VulkanBase()
    {
        UTIL_DELETE(pTimer)
    }

    float VulkanBase::RefreshAspectRatio()
    {
        this->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
        return this->aspectRatio;
    }

    void VulkanBase::CalculateFrameStats(GLFWwindow* s_pWindow)
    {

    }

    void VulkanBase::UpdateTimer()
    {
        this->pTimer->Tick();
        float timeSinceStart = this->pTimer->GetTimeSinceStart();
        ++ this->nFrameFPS;
        if (timeSinceStart >= this->fTimeLastFPS + 1.0f)
        {
            this->fFPS = this->nFrameFPS / (timeSinceStart - this->fTimeLastFPS);
            this->nFrameFPS = 0;
            this->fTimeLastFPS = timeSinceStart;
        }
    }

    std::string VulkanBase::GetAssetFullPath(const std::string& assetName)
    {
        return this->pathBin + assetName;
    }

}; //LibUtil