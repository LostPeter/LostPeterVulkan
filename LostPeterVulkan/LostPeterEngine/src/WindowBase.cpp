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

#include "../include/WindowBase.h"

namespace LostPeterEngine
{
    WindowBase::WindowBase()
        : m_strNameTitle("")
        , m_nWindowWidth(0)
        , m_nWindowHeight(0)
        , m_pWindow(nullptr)
        , m_bIsWindowShow(true)

        , m_bIsMouseButtonDownLeft(false)
        , m_bIsMouseButtonDownRight(false)
        , m_bIsMouseButtonDownMiddle(false)
    {

    }   

    WindowBase::~WindowBase()
    {
        
    }

    void WindowBase::Destroy()
    {
        destroyWindowGLFW();
    }
    void WindowBase::destroyWindowGLFW()
    {
        if (m_pWindow != nullptr)
        {
            glfwDestroyWindow(m_pWindow);
        }
        m_pWindow = nullptr;
    }

    bool WindowBase::Init(const String& nameTitle, int nWindowWidth, int nWindowHeight)
    {
        //1> createWindowGLFW
        if (!createWindowGLFW(nameTitle, nWindowWidth, nWindowHeight))
        {
            F_LogError("*********************** WindowBase::Init: createWindowGLFW failed, nameTitle: [%s], w-h: [%d]-[%d] !", m_strNameTitle.c_str(), nWindowWidth, nWindowHeight);
            return false;
        }
        RefreshFramebufferSize();
        RefreshWindowContentScale();

        //2> registerCallbacks()
        registerCallbacks();

        return true;
    }
    bool WindowBase::createWindowGLFW(const String& nameTitle, int nWindowWidth, int nWindowHeight)
    {
        m_strNameTitle = nameTitle;
        m_nWindowWidth = nWindowWidth;
        m_nWindowHeight = nWindowHeight;

        m_pWindow = glfwCreateWindow(m_nWindowWidth, m_nWindowHeight, m_strNameTitle.c_str(), NULL, NULL);
        if (!m_pWindow) 
        {
            F_LogError("*********************** WindowBase::createWindowGLFW: glfwCreateWindow failed, nameTitle: [%s], w-h: [%d]-[%d] !", m_strNameTitle.c_str(), nWindowWidth, nWindowHeight);
            return false;
        }
        glfwSetWindowUserPointer(m_pWindow, this);

        return true;
    }
    void WindowBase::registerCallbacks()
    {
        //1> callback_key
        glfwSetKeyCallback(m_pWindow, callback_key);

        //2> callback_framebuffer_size
        glfwSetFramebufferSizeCallback(m_pWindow, callback_framebuffer_size);

        //3> callback_mouse_button
        glfwSetMouseButtonCallback(m_pWindow, callback_mouse_button);

        //4> callback_cursor_position
        glfwSetCursorPosCallback(m_pWindow, callback_cursor_position);

        //5> callback_scroll
        glfwSetScrollCallback(m_pWindow, callback_scroll);
    }
    void WindowBase::callback_key(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowBase* pWindow = (WindowBase*)glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS)
        {
            pWindow->OnKeyDown(key);
        }
        else if (action == GLFW_RELEASE)
        {
            pWindow->OnKeyUp(key);
        }
    }
    void WindowBase::callback_framebuffer_size(GLFWwindow* window, int width, int height)
    {
        WindowBase* pWindow = (WindowBase*)glfwGetWindowUserPointer(window);
        pWindow->OnResize(width, height, true);
    }
    void WindowBase::callback_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {

    }
    void WindowBase::callback_cursor_position(GLFWwindow* window, double x, double y)
    {

    }
    void WindowBase::callback_scroll(GLFWwindow* window, double x, double y)
    {
        WindowBase* pWindow = (WindowBase*)glfwGetWindowUserPointer(window);
        pWindow->OnMouseWheel(x, y);
    }

    void WindowBase::RefreshFramebufferSize()
    {
        int width, height;
        glfwGetFramebufferSize(this->m_pWindow, &width, &height);
        this->m_vFramebufferSize.x = width;
        this->m_vFramebufferSize.y = height;
    }
    void WindowBase::RefreshWindowContentScale()
    {
        float scaleX, scaleY;
        glfwGetWindowContentScale(this->m_pWindow, &scaleX, &scaleY);
        this->m_vWindowContentScale.x = scaleX;
        this->m_vWindowContentScale.y = scaleY;
    }

    void WindowBase::OnMouseInput()
    {
        double cursorX; double cursorY;
        glfwGetCursorPos(this->m_pWindow, &cursorX, &cursorY);
        cursorX *= this->m_vWindowContentScale.x;
        cursorY *= this->m_vWindowContentScale.y;

        //Mouse Left
        int actionLeft = glfwGetMouseButton(this->m_pWindow, GLFW_MOUSE_BUTTON_LEFT);
        if (actionLeft == GLFW_PRESS)
        {
            if (!this->m_bIsMouseButtonDownLeft)
            {
                OnMouseLeftDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_LEFT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->m_bIsMouseButtonDownLeft)
            {
                OnMouseLeftUp(cursorX, cursorY);
            }
        }
        
        //Mouse Right
        int actionRight = glfwGetMouseButton(this->m_pWindow, GLFW_MOUSE_BUTTON_RIGHT);
        if (actionRight == GLFW_PRESS)
        {
            if (!this->m_bIsMouseButtonDownRight)
            {
                OnMouseRightDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_RIGHT, cursorX, cursorY);
            }
        }
        else
        {
            if (this->m_bIsMouseButtonDownRight)
            {
                OnMouseRightUp(cursorX, cursorY);
            }
        }

        //Mouse Middle
        int actionMiddle = glfwGetMouseButton(this->m_pWindow, GLFW_MOUSE_BUTTON_MIDDLE);
        if (actionMiddle == GLFW_PRESS)
        {
            if (!this->m_bIsMouseButtonDownMiddle)
            {
                OnMouseMiddleDown(cursorX, cursorY);
            }
            else
            {
                OnMouseMove(GLFW_MOUSE_BUTTON_MIDDLE, cursorX, cursorY);
            }
        }
        else
        {
            if (this->m_bIsMouseButtonDownMiddle)
            {
                OnMouseMiddleUp(cursorX, cursorY);
            }
        }

        //Mouse Hover
        if (!this->m_bIsMouseButtonDownLeft &&
            !this->m_bIsMouseButtonDownRight &&
            !this->m_bIsMouseButtonDownMiddle)
        {
            OnMouseHover(cursorX, cursorY);
        }

        this->m_vMousePosLast.x = (float)cursorX;
        this->m_vMousePosLast.y = (float)cursorY;
    }

}; //LostPeterEngine