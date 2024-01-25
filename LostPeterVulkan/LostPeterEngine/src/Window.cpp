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

#include "../include/Window.h"
#include "../include/WindowListener.h"

namespace LostPeterEngine
{
    Window::Window()
    {

    }   

    Window::~Window()
    {
        Destroy();
    }

    void Window::Destroy()
    {
        WindowBase::Destroy();

        RemoveWindowListenerAll();
    }

    void Window::AddWindowListener(WindowListener* pWindowListener)
    {
        WindowListenerPtrMap::iterator itFind = m_mapWindowListener.find(pWindowListener->GetName());
        if (itFind != m_mapWindowListener.end())
            return;
        m_mapWindowListener[pWindowListener->GetName()] = pWindowListener;
    }
    void Window::RemoveWindowListener(WindowListener* pWindowListener)
    {
        WindowListenerPtrMap::iterator itFind = m_mapWindowListener.find(pWindowListener->GetName());
        if (itFind == m_mapWindowListener.end())
            return;
        m_mapWindowListener.erase(itFind);
    }
    void Window::RemoveWindowListenerAll()
    {
        m_mapWindowListener.clear();
    }

    //Common/Window
    void Window::OnInit()
    {

    }

    void Window::OnLoad()
    {

    }

    bool Window::OnIsInit()
    {

        return true;
    }

    void Window::OnResize(int w, int h, bool force)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnResize(w, h, force);
        }
    }   

    void Window::OnDestroy()
    {

    }

    //Compute/Render
    bool Window::OnBeginCompute()
    {
        return true;
    }
        void Window::OnUpdateCompute()
        {

        }
        void Window::OnCompute()
        {

        }
    void Window::OnEndCompute()
    {

    }
    bool Window::OnBeginRender()
    {
        return true;
    }
        void Window::OnUpdateRender()
        {

        }
        void Window::OnRender()
        {

        }
    void Window::OnEndRender()
    {

    }

    //Mouse Input
    void Window::OnMouseLeftDown(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseLeftDown(x, y);
        }
    }   
    void Window::OnMouseLeftUp(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;
        
        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseLeftUp(x, y);
        }
    }
    void Window::OnMouseRightDown(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseRightDown(x, y);
        }
    }
    void Window::OnMouseRightUp(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseRightUp(x, y);
        }
    }
    void Window::OnMouseMiddleDown(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseMiddleDown(x, y);
        }
    }
    void Window::OnMouseMiddleUp(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseMiddleUp(x, y);
        }
    }
    void Window::OnMouseMove(int button, double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseMove(button, x, y);
        }
    }
    void Window::OnMouseHover(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseHover(x, y);
        }
    }
    void Window::OnMouseWheel(double x, double y)
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnMouseWheel(x, y);
        }
    }

    //Keyboard Input
    void Window::OnKeyboardInput()
    {
        if (m_mapWindowListener.size() <= 0)
            return;

        for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
             it != m_mapWindowListener.end(); ++it)
        {
            it->second->OnKeyboardInput();
        }
    }
        void Window::OnKeyDown(int key)
        {
            if (m_mapWindowListener.size() <= 0)
                return;

            for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
                it != m_mapWindowListener.end(); ++it)
            {
                it->second->OnKeyDown(key);
            }
        }
        void Window::OnKeyUp(int key)
        {
            if (m_mapWindowListener.size() <= 0)
                return;

            for (WindowListenerPtrMap::iterator it = m_mapWindowListener.begin();
                it != m_mapWindowListener.end(); ++it)
            {
                it->second->OnKeyUp(key);
            }
        }

}; //LostPeterEngine