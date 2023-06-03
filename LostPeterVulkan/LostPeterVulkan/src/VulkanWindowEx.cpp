/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanWindowEx.h"

namespace LostPeter
{
    VulkanWindowEx::VulkanWindowEx()
        : m_strNameTitle("")
        , m_nWidth(0)
        , m_nHeight(0)
        , m_pWindow(nullptr)
        , m_bIsWindowShow(true)
    {

    }   

    VulkanWindowEx::~VulkanWindowEx()
    {
        Destroy();
    }

    void VulkanWindowEx::Destroy()
    {
        if (m_pWindow != nullptr)
        {
            glfwDestroyWindow(m_pWindow);
        }
        m_pWindow = nullptr;
    }

    bool VulkanWindowEx::Init(const String& nameTitle, int nWidth, int nHeight)
    {
        m_strNameTitle = nameTitle;
        m_nWidth = nWidth;
        m_nHeight = nHeight;

        m_pWindow = glfwCreateWindow(m_nWidth, m_nHeight, m_strNameTitle.c_str(), NULL, NULL);
        if (!m_pWindow) {
            F_LogError("VulkanWindowEx::Init: glfwCreateWindow failed, [%s]", m_strNameTitle.c_str());
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

    void VulkanWindowEx::OnResize(int w, int h, bool force)
    {
        
    }   

    void VulkanWindowEx::OnMouseInput()
    {

    }
    void VulkanWindowEx::OnMouseLeftDown(double x, double y)
    {

    }
    void VulkanWindowEx::OnMouseLeftUp(double x, double y)
    {

    }
    void VulkanWindowEx::OnMouseRightDown(double x, double y)
    {

    }
    void VulkanWindowEx::OnMouseRightUp(double x, double y)
    {

    }
    void VulkanWindowEx::OnMouseMove(int button, double x, double y)
    {

    }
    void VulkanWindowEx::OnMouseWheel(double x, double y)
    {

    }

    void VulkanWindowEx::OnKeyboardInput()
    {

    }
    void VulkanWindowEx::OnKeyDown(int key)
    {

    }
    void VulkanWindowEx::OnKeyUp(int key)
    {

    }

    void VulkanWindowEx::callback_key(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        VulkanWindowEx* pWindow = (VulkanWindowEx*)glfwGetWindowUserPointer(window);
        if (action == GLFW_PRESS)
        {
            pWindow->OnKeyDown(key);
        }
        else if (action == GLFW_RELEASE)
        {
            pWindow->OnKeyUp(key);
        }
    }
    void VulkanWindowEx::callback_framebuffer_size(GLFWwindow* window, int width, int height)
    {
        VulkanWindowEx* pWindow = (VulkanWindowEx*)glfwGetWindowUserPointer(window);
        pWindow->OnResize(width, height, true);
    }
    void VulkanWindowEx::callback_mouse_button(GLFWwindow* window, int button, int action, int mods)
    {

    }
    void VulkanWindowEx::callback_cursor_position(GLFWwindow* window, double x, double y)
    {

    }
    void VulkanWindowEx::callback_scroll(GLFWwindow* window, double x, double y)
    {
        VulkanWindowEx* pWindow = (VulkanWindowEx*)glfwGetWindowUserPointer(window);
        pWindow->OnMouseWheel(x, y);
    }

}; //LostPeter