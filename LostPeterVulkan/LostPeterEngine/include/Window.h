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

#include "WindowBase.h"

namespace LostPeterEngine
{
    class engineExport Window : public WindowBase
    {
    public:
        Window();
        virtual ~Window();

    public:
    protected:
        WindowListenerPtrMap m_mapWindowListener;

    public:
        virtual void Destroy();

    public:
        void AddWindowListener(WindowListener* pWindowListener);
        void RemoveWindowListener(WindowListener* pWindowListener);
        void RemoveWindowListenerAll();

    public:
        //Common/Window
        virtual void OnInit();
        virtual void OnLoad();
        virtual bool OnIsInit();
        virtual void OnResize(int w, int h, bool force);
        virtual void OnDestroy();

        //Compute/Render
        virtual bool OnBeginCompute();
            virtual void OnUpdateCompute();
            virtual void OnCompute();
        virtual void OnEndCompute();
        virtual bool OnBeginRender();
            virtual void OnUpdateRender();
            virtual void OnRender();
        virtual void OnEndRender();

        //Mouse Input
        virtual void OnMouseLeftDown(double x, double y);
        virtual void OnMouseLeftUp(double x, double y);
        virtual void OnMouseRightDown(double x, double y);
        virtual void OnMouseRightUp(double x, double y);
        virtual void OnMouseMiddleDown(double x, double y);
        virtual void OnMouseMiddleUp(double x, double y);
        virtual void OnMouseMove(int button, double x, double y);
        virtual void OnMouseHover(double x, double y);
        virtual void OnMouseWheel(double x, double y);

        //Keyboard Input
        virtual void OnKeyboardInput();
            virtual void OnKeyDown(int key);
            virtual void OnKeyUp(int key);
    };

}; //LostPeterEngine

#endif