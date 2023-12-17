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

#ifndef _WINDOW_LISTENER_H_
#define _WINDOW_LISTENER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport WindowListener : public Base
    {
    public:
        WindowListener(const String& nameWindowListener, Window* pWindow);
        virtual ~WindowListener();

    public:
    protected:
        Window* m_pWindow;

    public:
        F_FORCEINLINE Window* GetWindow() { return m_pWindow; }

    public:
        //Window
        virtual void OnResize(int w, int h, bool force) { }

        //Mouse Input
        virtual void OnMouseLeftDown(double x, double y) { }
        virtual void OnMouseLeftUp(double x, double y) { }
        virtual void OnMouseRightDown(double x, double y) { }
        virtual void OnMouseRightUp(double x, double y) { }
        virtual void OnMouseMiddleDown(double x, double y) { }
        virtual void OnMouseMiddleUp(double x, double y) { }
        virtual void OnMouseMove(int button, double x, double y) { }
        virtual void OnMouseHover(double x, double y) { }
        virtual void OnMouseWheel(double x, double y) { }

        //Keyboard Input
        virtual void OnKeyboardInput() { }
        virtual void OnKeyDown(int key) { }
        virtual void OnKeyUp(int key) { }
    };

}; //LostPeterEngine

#endif