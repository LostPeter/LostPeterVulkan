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

#include "../include/RenderWindow.h"
#include "../include/RenderWindowListener.h"
#include "../include/Renderer.h"
#include "../include/RenderPassDescriptor.h"

namespace LostPeterEngine
{
    RenderWindow::RenderWindow(const String& nameRenderWindow)
        : RenderTarget(nameRenderWindow)
        , m_bIsFullScreen(false)
		, m_bIsFullScreenFake(false)
		, m_bIsPrimary(false)
		, m_bAutoDeactivatedOnFocusChange(true)
		, m_bFocused(false)
		, m_nLeft(0)
		, m_nTop(0)
		, m_nClientWidth(0)
		, m_nClientHeight(0)
        , m_pRenderFrameBufferDescriptor(nullptr)
        , m_pRenderPassDescriptor(nullptr)
    {

    }
    
    RenderWindow::~RenderWindow()
    {

    }

	void RenderWindow::Destroy()
	{
        RemoveRenderWindowListenerAll();

		WindowBase::Destroy();
	}

    void RenderWindow::GetMetrics(int32& nWidth, int32& nHeight, uint32& nColorDepth, int32& nLeft, int32& nTop)
	{
		nWidth = m_nWidth;
		nHeight = m_nHeight;
		nColorDepth = m_nColorDepth;
		nLeft = m_nLeft;
		nTop = m_nTop;
	}

	void RenderWindow::Update()
	{
		Update(true);
	}

	void RenderWindow::Update(bool bSwapBuffers /*= true*/)
	{
		RenderTarget::Update(bSwapBuffers);
	}

	bool RenderWindow::Present(Renderer* pRenderer)
	{
		if(pRenderer->IsEmptyGpuBuffer())
		{
			EmptyGPUCommandBuffer();
		}
		return SwapBuffers(pRenderer->IsVSync());
	}

	void RenderWindow::AddRenderWindowListener(RenderWindowListener* pRenderWindowListener)
    {
        RenderWindowListenerPtrMap::iterator itFind = m_mapRenderWindowListener.find(pRenderWindowListener->GetName());
        if (itFind != m_mapRenderWindowListener.end())
            return;
        m_mapRenderWindowListener[pRenderWindowListener->GetName()] = pRenderWindowListener;
    }
    void RenderWindow::RemoveRenderWindowListener(RenderWindowListener* pRenderWindowListener)
    {
        RenderWindowListenerPtrMap::iterator itFind = m_mapRenderWindowListener.find(pRenderWindowListener->GetName());
        if (itFind == m_mapRenderWindowListener.end())
            return;
        m_mapRenderWindowListener.erase(itFind);
    }
    void RenderWindow::RemoveRenderWindowListenerAll()
    {
        m_mapRenderWindowListener.clear();
    }

    //Common/Window
    void RenderWindow::OnInit()
    {

    }

    void RenderWindow::OnLoad()
    {

    }

    bool RenderWindow::OnIsInit()
    {

        return true;
    }

    void RenderWindow::OnResize(int w, int h, bool force)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnResize(w, h, force);
        }
    }   

    void RenderWindow::OnDestroy()
    {

    }

    //Compute/Render
    bool RenderWindow::OnBeginCompute()
    {
        return true;
    }
        void RenderWindow::OnUpdateCompute()
        {

        }
        void RenderWindow::OnCompute()
        {

        }
    void RenderWindow::OnEndCompute()
    {

    }
    
    bool RenderWindow::OnBeginRender()
    {
        return true;
    }
        void RenderWindow::OnUpdateRender()
        {

        }
        void RenderWindow::OnRender()
        {

        }
    void RenderWindow::OnEndRender()
    {
        
    }

    //Mouse Input
    void RenderWindow::OnMouseLeftDown(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseLeftDown(x, y);
        }
    }   
    void RenderWindow::OnMouseLeftUp(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;
        
        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseLeftUp(x, y);
        }
    }
    void RenderWindow::OnMouseRightDown(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseRightDown(x, y);
        }
    }
    void RenderWindow::OnMouseRightUp(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseRightUp(x, y);
        }
    }
    void RenderWindow::OnMouseMiddleDown(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseMiddleDown(x, y);
        }
    }
    void RenderWindow::OnMouseMiddleUp(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseMiddleUp(x, y);
        }
    }
    void RenderWindow::OnMouseMove(int button, double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseMove(button, x, y);
        }
    }
    void RenderWindow::OnMouseHover(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseHover(x, y);
        }
    }
    void RenderWindow::OnMouseWheel(double x, double y)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnMouseWheel(x, y);
        }
    }

    void RenderWindow::OnKeyboardInput()
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnKeyboardInput();
        }
    }
    void RenderWindow::OnKeyDown(int key)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnKeyDown(key);
        }
    }
    void RenderWindow::OnKeyUp(int key)
    {
        if (m_mapRenderWindowListener.size() <= 0)
            return;

        for (RenderWindowListenerPtrMap::iterator it = m_mapRenderWindowListener.begin();
             it != m_mapRenderWindowListener.end(); ++it)
        {
            it->second->OnKeyUp(key);
        }
    }

}; //LostPeterEngine