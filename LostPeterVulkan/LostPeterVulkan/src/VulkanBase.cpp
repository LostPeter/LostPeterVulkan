/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-10-30
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanBase.h"

namespace LostPeterVulkan
{
    FLogManager* VulkanBase::ms_pLogManager = FLogManager::GetInstance();
    VulkanBase::VulkanBase(int width, int height, String name)
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

        , pTimer(new FTimer())
        , fFPS(0.0f)
        , nFrameFPS(0)
        , nFrameTotal(0)
    {
        this->pathBin = FUtil::GetPathBin() + "/";
        F_LogInfo("Path Bin: [%s] !", this->pathBin.c_str());

        RefreshAspectRatio();
        fTimeLastFPS = pTimer->GetTimeSinceStart();
    }

    VulkanBase::~VulkanBase()
    {
        F_DELETE(pTimer)
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

    String VulkanBase::GetAssetFullPath(const String& assetName)
    {
        return this->pathBin + assetName;
    }

}; //LostPeterVulkan