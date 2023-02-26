/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-30
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_BASE_H_
#define _VULKAN_BASE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanBase
    {
    public:
        VulkanBase(int width, int height, std::string name);
        virtual ~VulkanBase();

    public:
        static VulkanLogManager* ms_pLogManager;

    public:
        int width;
        int height;
        float aspectRatio;
        GLFWwindow* pWindow;

        bool isAppPaused;           //is the application paused?
        bool isMinimized;           //is the application minimized?
        bool isMaximized;           //is the application maximized?
        bool isResizing;            //is the resize bars being dragged?
        bool isFullscreenState;     //is fullscreen enabled
        bool isMinimizedWindowNeedClose; //is Minimalized Window Need Close

        bool isCreateDevice;        //is create device
        bool isLoadAsset;           //is load asset

        VulkanTimer* pTimer;
        float fTimeLastFPS;
        float fFPS;
        int nFrameFPS;
        uint64 nFrameTotal;

    protected:
        std::string nameTitle;
        std::string pathBin; 

    public:
        int GetWidth() const { return this->width; }
        int GetHeight() const { return this->height; }
        float RefreshAspectRatio();

        const std::string& GetTitle() const { return this->nameTitle; }


    public:
        virtual void OnInit() = 0;
        virtual void OnLoad() = 0;
        virtual bool OnIsInit() = 0;
        virtual void OnResize(int w, int h, bool force) = 0;
        virtual bool OnBeginCompute() = 0;
            virtual void OnCompute() = 0;
        virtual void OnEndCompute() = 0;
        virtual bool OnBeginRender() = 0;
            virtual void OnUpdate() = 0;
            virtual void OnRender() = 0;
        virtual void OnEndRender() = 0;
        virtual void OnDestroy() = 0;

        // Mouse Input
        virtual void OnMouseInput() { }
        virtual void OnMouseLeftDown(double x, double y) { }
        virtual void OnMouseLeftUp(double x, double y) { }
        virtual void OnMouseRightDown(double x, double y) { }
        virtual void OnMouseRightUp(double x, double y) { }
        virtual void OnMouseMove(int button, double x, double y) { }
        virtual void OnMouseWheel(double x, double y) { }

        // Keyboard Input
        virtual void OnKeyboardInput() { }
        virtual void OnKeyDown(int key) { }
        virtual void OnKeyUp(int key) { }

    public:
        virtual void CalculateFrameStats(GLFWwindow* s_pWindow);

        virtual void UpdateTimer();

    protected:
        std::string GetAssetFullPath(const std::string& assetName);

    };

}; //LostPeter

#endif