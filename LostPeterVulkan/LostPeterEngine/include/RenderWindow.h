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

namespace LostPeterEngine
{
    class utilExport RenderWindow : public RenderTarget
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
		int32 m_nWindowWidth;
		int32 m_nWindowHeight;
		int32 m_nClientWidth;
		int32 m_nClientHeight;
		
	public:
		virtual bool IsFullScreen() const { return m_bIsFullScreen; }
		virtual void SetFullScreen(bool bFullScreen, uint32 nWidth, uint32 nHeight) { }
		virtual void SetFullScreen(bool bFullscreen, uint32 nWidth, uint32 nHeight, uint32 nWinWidth, uint32 nWinHeight) { }

		virtual bool IsFakeFullScreen() const { return m_bFakeFullScreen; }
		virtual void SetFakeFullScreen(bool bVal) { m_bFakeFullScreen = bVal; }
			
		virtual bool IsPrimary() const { return m_bIsPrimary; }
		virtual void SetPrimary() { m_bIsPrimary = true; }

		virtual bool IsDeactivatedOnFocusChange() const	{ return m_bAutoDeactivatedOnFocusChange; }
		virtual void SetDeactivateOnFocusChange(bool b)	{ m_bAutoDeactivatedOnFocusChange = b; }

		virtual bool IsFocused() const { return m_bFocused && IsVisible(); }
		virtual void SetFocused(bool bFocused) { m_bFocused = bFocused; }


		virtual void GetMetrics(uint32& nWidth,uint32& nHeight,uint32& nColorDepth,int32& nLeft,int32& nTop);
		inline int32 GetWindowLeft() const { return m_nLeft; }
		inline int32 GetWindowTop() const { return m_nTop; }
		inline int32 GetWindowWidth() const	{ return m_nWindowWidth; }
		inline int32 GetWindowHeight() const { return m_nWindowHeight; }
		inline int32 GetClientWidth() const	{ return m_nClientWidth; }
		inline int32 GetClientHeight() const { return m_nClientHeight; }

	public:
		virtual bool IsVisible() const { return true; }
		virtual void SetVisible(bool visible) {	}
		virtual bool IsActive() const { return m_bActive && IsVisible(); }
		virtual void Update();
		virtual void Update(bool bSwapBuffers);
		virtual void SwapBuffers(bool bVSync = true) = 0;
		
	public:
        virtual bool Destroy() = 0;
		virtual bool Create(const String& strName, uint32 nWidth, uint32 nHeight, bool bFullScreen, const String2StringMap* pParams, bool bShowWindow = true) = 0;
    
		virtual void Resize(uint32 nWidth,uint32 nHeight) = 0;
		virtual void Reposition(int32 nLeft,int32 nTop) = 0;
		virtual bool IsClosed() const = 0;
		virtual void WindowMovedOrResized() = 0;
		virtual void SetWindowTitle(const String& strName) = 0;
		virtual bool CanChangeToWindowMode(int32 srcWidth,int32 srcHeight,int32 &destWidth,int32 &destHeight) = 0;
		
		virtual void EmptyGPUCommandBuffer() = 0;
		
		virtual bool IsDeviceLost() { return false; }
		virtual void Present(Renderer* pRenderer);
    };

}; //LostPeterEngine

#endif