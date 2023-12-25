/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include "RenderTarget.h"
#include "WindowBase.h"

namespace LostPeterEngine
{
    class engineExport RenderWindow : public RenderTarget
									, public WindowBase
    {
    public:
        RenderWindow(const String& nameRenderWindow);
        virtual ~RenderWindow();

    public:
    protected:
		bool m_bIsFullScreen;
		bool m_bFakeFullScreen;
		bool m_bIsPrimary;
		bool m_bAutoDeactivatedOnFocusChange;
		bool m_bFocused;

		int32 m_nLeft;
		int32 m_nTop;
		int32 m_nClientWidth;
		int32 m_nClientHeight;

		RenderPassDescriptor* m_pRenderPassDescriptor;

	protected:
		RenderWindowListenerPtrMap m_mapRenderWindowListener;

	public:
		virtual bool IsFullScreen() const { return m_bIsFullScreen; }
		virtual void SetFullScreen(bool bFullScreen, int32 nWidth, int32 nHeight) { }
		virtual void SetFullScreen(bool bFullscreen, int32 nWidth, int32 nHeight, int32 nWinWidth, int32 nWinHeight) { }

		virtual bool IsFakeFullScreen() const { return m_bFakeFullScreen; }
		virtual void SetFakeFullScreen(bool bVal) { m_bFakeFullScreen = bVal; }
			
		virtual bool IsPrimary() const { return m_bIsPrimary; }
		virtual void SetPrimary() { m_bIsPrimary = true; }

		virtual bool IsDeactivatedOnFocusChange() const	{ return m_bAutoDeactivatedOnFocusChange; }
		virtual void SetDeactivateOnFocusChange(bool b)	{ m_bAutoDeactivatedOnFocusChange = b; }

		virtual bool IsFocused() const { return m_bFocused && IsVisible(); }
		virtual void SetFocused(bool bFocused) { m_bFocused = bFocused; }

		virtual void GetMetrics(int32& nWidth, int32& nHeight, uint32& nColorDepth, int32& nLeft, int32& nTop);

	public:
		F_FORCEINLINE int32 GetWindowLeft() const { return m_nLeft; }
		F_FORCEINLINE int32 GetWindowTop() const { return m_nTop; }
		F_FORCEINLINE int32 GetClientWidth() const { return m_nClientWidth; }
		F_FORCEINLINE int32 GetClientHeight() const { return m_nClientHeight; }
		F_FORCEINLINE RenderPassDescriptor* GetRenderPassDescriptor() const { return m_pRenderPassDescriptor; }

	public:
		virtual bool IsVisible() const { return true; }
		virtual void SetVisible(bool visible) {	}
		virtual bool IsActive() const { return m_bActive && IsVisible(); }
		virtual void Update();
		virtual void Update(bool bSwapBuffers);
		
	public:
        virtual void Destroy();
		virtual bool Init(int32 nWidth, 
						  int32 nHeight, 
						  const String2StringMap* pParams) = 0;
    
		virtual void Resize(int32 nWidth, int32 nHeight) = 0;
		virtual void Reposition(int32 nLeft, int32 nTop) = 0;
		virtual bool IsClosed() const = 0;
		virtual void WindowMovedOrResized() = 0;
		virtual bool CanChangeToWindowMode(int32 srcWidth, int32 srcHeight, int32& destWidth, int32& destHeight) = 0;
		
		virtual void EmptyGPUCommandBuffer() = 0;
		
		virtual bool IsDeviceLost() { return false; }
		virtual bool Present(Renderer* pRenderer);

	public:
		void AddRenderWindowListener(RenderWindowListener* pRenderWindowListener);
        void RemoveRenderWindowListener(RenderWindowListener* pRenderWindowListener);
        void RemoveRenderWindowListenerAll();

	public:
        //Window
        virtual void OnResize(int w, int h, bool force);

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