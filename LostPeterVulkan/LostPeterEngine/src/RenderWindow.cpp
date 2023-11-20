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
#include "../include/Renderer.h"

namespace LostPeterEngine
{
    RenderWindow::RenderWindow(const String& nameRenderWindow)
        : RenderTarget(nameRenderWindow)
        , m_bIsFullScreen(false)
		, m_bFakeFullScreen(false)
		, m_bIsPrimary(false)
		, m_bAutoDeactivatedOnFocusChange(true)
		, m_bFocused(false)
		, m_nLeft(0)
		, m_nTop(0)
		, m_nWindowWidth(0)
		, m_nWindowHeight(0)
		, m_nClientWidth(0)
		, m_nClientHeight(0)
    {

    }
    
    RenderWindow::~RenderWindow()
    {

    }

    void RenderWindow::GetMetrics(uint32& nWidth, uint32& nHeight, uint32& nColorDepth, int32& nLeft, int32& nTop)
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
		RenderTarget::Update();
	}

	void RenderWindow::Present(Renderer* pRenderer)
	{
		if(pRenderer->IsEmptyGpuBuffer())
		{
			EmptyGPUCommandBuffer();
		}
        
		SwapBuffers(pRenderer->IsVSync());
	}

}; //LostPeterEngine