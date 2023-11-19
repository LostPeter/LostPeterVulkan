/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _WINDOW_H_
#define _WINDOW_H_

namespace LostPeter
{
    class utilExport Window
    {
    public:
        Window();
        virtual ~Window();

    public:
    protected:
        String m_strNameTitle;
        int m_nWidth;
        int m_nHeight;

        GLFWwindow* m_pWindow;
        bool m_bIsWindowShow;

    public:
        const String& GetNameTitle() const { return m_strNameTitle; }
        int GetWidth() const { return m_nWidth; }
        int GetHeight() const { return m_nHeight; }
        GLFWwindow* GetGLFWwindow() { return m_pWindow; }
        bool IsWindowShow() const { return m_bIsWindowShow; }

    public:
        void Destroy();
        bool Init(const String& nameTitle, int nWidth, int nHeight);

    public:


    public:
        // Window
        virtual void OnResize(int w, int h, bool force);

        // Mouse Input
        virtual void OnMouseInput();
        virtual void OnMouseLeftDown(double x, double y);
        virtual void OnMouseLeftUp(double x, double y);
        virtual void OnMouseRightDown(double x, double y);
        virtual void OnMouseRightUp(double x, double y);
        virtual void OnMouseMove(int button, double x, double y);
        virtual void OnMouseWheel(double x, double y);

        // Keyboard Input
        virtual void OnKeyboardInput();
        virtual void OnKeyDown(int key);
        virtual void OnKeyUp(int key);

    private:
        static void callback_key(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void callback_framebuffer_size(GLFWwindow* window, int width, int height);
        static void callback_mouse_button(GLFWwindow* window, int button, int action, int mods);
        static void callback_cursor_position(GLFWwindow* window, double x, double y);
        static void callback_scroll(GLFWwindow* window, double x, double y);
    };

}; //LostPeter

#endif