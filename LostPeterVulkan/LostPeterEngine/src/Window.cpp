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

#include "../include/PreInclude.h"

namespace LostPeter
{
    Window::Window()
        : m_strNameTitle("")
        , m_nWidth(0)
        , m_nHeight(0)
        , m_pWindow(nullptr)
        , m_bIsWindowShow(true)
    {

    }   

    Window::~Window()
    {
        Destroy();
    }

    void Window::Destroy()
    {
        if (m_pWindow != nullptr)
        {
            glfwDestroyWindow(m_pWindow);
        }
        m_pWindow = nullptr;
    }

    bool Window::Init(const String& nameTitle, int nWidth, int nHeight)
    {
        m_strNameTitle = nameTitle;
        m_nWidth = nWidth;
        m_nHeight = nHeight;

        m_pWindow = glfwCreateWindow(m_nWidth, m_nHeight, m_strNameTitle.c_str(), NULL, NULL);
        if (!m_pWindow) {
            F_LogError("Window::Init: glfwCreateWindow failed, [%s]", m_strNameTitle.c_str());
            return false;
        }

        glfwSetWindowUserPointer(m_pWindow, this);
        glfwSetKeyCallback(m_pWindow, callback_key);
        glfwSetFramebufferSizeCallback(m_pWindow, callback_framebuffer_size);
        glfwSetMouseButtonCallback(m_pWindow, callback_mouse_button);
        glfwSetCursorPosCallback(m_pWindow, callback_cursor_position);
        glfwSetScrollCallback(m_pWindow, callback_scroll);

        return true;
    }

    void Window::OnResize(int w, int h, bool force)
    {
        
    }   

    void Window::OnMouseInput()
    {

    }
    void Window::OnMouseLeftDown(double x, double y)
    {

    }
    void Window::OnMouseLeftUp(double x, double y)
    {

    }
    void Window::OnMouseRightDown(double x, double y)
    {

    }
    void Window::OnMouseRightUp(double x, double y)
    {

    }
    void Window::OnMouseMove(int button, double x, double y)
    {

    }
    void Window::OnMouseWheel(double x, double y)
    {

    }

    void Window::OnKeyboardInput()
    {

    }
    void Window::OnKeyDown(int key)
    {

    }
    void Window::OnKeyUp(int key)
    {

    }

    void Window::callback_key(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        Window* pWindow = (Window*)glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS)
        {
            pWindow->OnKeyDown(key);
        }
        else if (action == GLFW_RELEASE)
        {
            pWindow->OnKeyUp(key);
        }
    }
    void Window::callback_framebuffer_size(GLFWwindow* window, int width, int height)
    {
        Window* pWindow = (Window*)glfwGetWindowUserPointer(window);
        pWindow->OnResize(width, height, true);
    }
    void Window::callback_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {

    }
    void Window::callback_cursor_position(GLFWwindow* window, double x, double y)
    {

    }
    void Window::callback_scroll(GLFWwindow* window, double x, double y)
    {
        Window* pWindow = (Window*)glfwGetWindowUserPointer(window);
        pWindow->OnMouseWheel(x, y);
    }

}; //LostPeter