/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _WINDOW_BASE_H_
#define _WINDOW_BASE_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport WindowBase 
    {
    public:
        WindowBase();
        virtual ~WindowBase();

    public:
    protected:
        String m_strNameWindow;
        String m_strNameTitle;
        int32 m_nWindowWidth;
        int32 m_nWindowHeight;

        GLFWwindow* m_pWindow;
        bool m_bIsWindowShow;

        FVector2 m_vFramebufferSize;
        FVector2 m_vWindowContentScale;

    protected:
        FVector2 m_vMousePosLast;
        bool m_bIsMouseButtonDownLeft;
        bool m_bIsMouseButtonDownRight;
        bool m_bIsMouseButtonDownMiddle;

    public:
        F_FORCEINLINE const String& GetNameWindow() const { return m_strNameWindow; }
        F_FORCEINLINE const String& GetNameTitle() const { return m_strNameTitle; }
        F_FORCEINLINE int32 GetWindowWidth() const { return m_nWindowWidth; }
        F_FORCEINLINE int32 GetWindowHeight() const { return m_nWindowHeight; }
        F_FORCEINLINE GLFWwindow* GetGLFWwindow() { return m_pWindow; }
        F_FORCEINLINE bool IsWindowShow() const { return m_bIsWindowShow; }

        F_FORCEINLINE bool IsMouseButtonDownLeft() const { return m_bIsMouseButtonDownLeft; }
        F_FORCEINLINE bool IsMouseButtonDownRight() const { return m_bIsMouseButtonDownRight; }
        F_FORCEINLINE bool IsMouseButtonDownMiddle() const { return m_bIsMouseButtonDownMiddle; }

    public:
        virtual void Destroy();
        virtual bool Init(const String& nameWindow, int32 nWindowWidth, int32 nWindowHeight);

        virtual void SetWindowTitle(const String& nameTitle);
        virtual void SetIsWindowShow(bool bIsWindowShow);

        virtual void RefreshWindowSize(int32& nWindowWidth, int32& nWindowHeight);
        virtual void RefreshFramebufferSize();
        virtual void RefreshWindowContentScale();

    protected:
        virtual void destroyWindowGLFW();

        virtual bool createWindowGLFW(const String& nameWindow, int32 nWindowWidth, int32 nWindowHeight);
        virtual void registerCallbacks();

    public:
        //Common/Window
        virtual void OnInit() = 0;
        virtual void OnLoad() = 0;
        virtual bool OnIsInit() = 0;
        virtual void OnResize(int w, int h, bool force) = 0;
        virtual void OnDestroy() = 0;

        //Compute/Render
        virtual bool OnBeginCompute() = 0;
            virtual void OnUpdateCompute() = 0;
            virtual void OnCompute() = 0;
        virtual void OnEndCompute() = 0;
        virtual bool OnBeginRender() = 0;
            virtual void OnUpdateRender() = 0;
            virtual void OnRender() = 0;
        virtual void OnEndRender() = 0;

        //Mouse Input
        virtual void OnMouseInput();
            virtual void OnMouseLeftDown(double x, double y) = 0;
            virtual void OnMouseLeftUp(double x, double y) = 0;
            virtual void OnMouseRightDown(double x, double y) = 0;
            virtual void OnMouseRightUp(double x, double y) = 0;
            virtual void OnMouseMiddleDown(double x, double y) = 0;
            virtual void OnMouseMiddleUp(double x, double y) = 0;
            virtual void OnMouseMove(int button, double x, double y) = 0;
            virtual void OnMouseHover(double x, double y) = 0;
            virtual void OnMouseWheel(double x, double y) = 0;

        //Keyboard Input
        virtual void OnKeyboardInput() = 0;
            virtual void OnKeyDown(int key) = 0;
            virtual void OnKeyUp(int key) = 0;

    private:
        static void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void callback_framebuffer_size(GLFWwindow* window, int width, int height);
        static void callback_mouse_button(GLFWwindow* window, int button, int action, int mods);
        static void callback_cursor_position(GLFWwindow* window, double x, double y);
        static void callback_scroll(GLFWwindow* window, double x, double y);
    };

}; //LostPeterEngine

#endif