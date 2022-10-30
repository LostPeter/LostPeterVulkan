// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_BASE_H_
#define _VULKAN_BASE_H_

#include "predefine.h"
#include "timer.h"

namespace LibUtil
{
    class utilExport VulkanBase
    {
    public:
        VulkanBase(int width, int height, std::string name);
        virtual ~VulkanBase();

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

        Timer timer;

    protected:
        std::string nameTitle;
        std::string pathAssets; 

    public:
        // Accessors.
        int GetWidth() const { return this->width; }
        int GetHeight() const { return this->height; }
        float RefreshAspectRatio();

        const std::string& GetTitle() const { return this->nameTitle; }


    public:
        virtual void OnInit() = 0;
        virtual void OnLoad() = 0;
        virtual bool OnIsInit() = 0;
        virtual void OnResize(int w, int h, bool force) = 0;
        virtual bool OnBeginRender() = 0;
            virtual void OnUpdate() = 0;
            virtual void OnRender() = 0;
        virtual void OnEndRender() = 0;
        virtual void OnDestroy() = 0;

        // Samples override the event handlers to handle specific messages.
        virtual void OnKeyDown(int /*key*/) {}
        virtual void OnKeyUp(int /*key*/) {}

        // Mouse Input
        virtual void OnMouseDown(int btnState, int x, int y) { }
        virtual void OnMouseUp(int btnState, int x, int y) { }
        virtual void OnMouseMove(int btnState, int x, int y) { }

        // Keyboard Input
        // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
        virtual void OnKeyboardInput() { }

    public:
        virtual void CalculateFrameStats(GLFWwindow* s_pWindow);

    protected:
        std::string GetAssetFullPath(const std::string& assetName);

    };

}; //LibUtil

#endif