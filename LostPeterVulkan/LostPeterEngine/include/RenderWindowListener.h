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

#ifndef _RENDER_WINDOW_LISTENER_H_
#define _RENDER_WINDOW_LISTENER_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderWindowListener : public Base
    {
    public:
        RenderWindowListener(const String& nameRenderWindowListener, RenderWindow* pRenderWindow);
        virtual ~RenderWindowListener();

    public:
    protected:
        RenderWindow* m_pRenderWindow;

    public:
        F_FORCEINLINE RenderWindow* GetRenderWindow() { return m_pRenderWindow; }

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