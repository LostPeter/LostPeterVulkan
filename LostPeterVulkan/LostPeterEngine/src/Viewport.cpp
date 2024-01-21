/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Viewport.h"
#include "../include/ObjectCamera.h"
#include "../include/RenderTarget.h"

namespace LostPeterEngine
{
    Viewport::Viewport(const String& nameViewport,
                       ObjectCamera* pObjectCamera,
                       RenderTarget* pRenderTarget,
                       int nZOrder,
					   float fLeft /*= 0.0f*/,
					   float fTop /*= 0.0f*/,
					   float fWidth /*= 1.0f*/,
					   float fHeight /*= 1.0f*/,
					   float fDepthMin /*= 0.0f*/,
					   float fDepthMax /*= 1.0f*/)
        : Base(nameViewport)
        , m_pObjectCamera(pObjectCamera)
		, m_pRenderTarget(pRenderTarget)
		, m_nZOrder(nZOrder)
		, m_fLeft(fLeft)
		, m_fTop(fTop)
		, m_fWidth(fWidth)
		, m_fHeight(fHeight)
		, m_nActLeft(0)
		, m_nActTop(0)
		, m_nActWidth(0)
		, m_nActHeight(0)
		, m_fDepthMin(fDepthMin)
		, m_fDepthMax(fDepthMax)
		, m_clBackGroundColor(FMath::ms_clBlack)
		, m_bClearEveryFrame(true)
		, m_nClearBuffers(F_FrameBuffer_Color | F_FrameBuffer_Depth)
		, m_bUpdated(false)
		, m_bShowOverlays(true)
		, m_bShowSkies(true)
		, m_bShowShadows(true)
		, m_nVisibilityMask(0xFFFFFFFF)
    {
		F_Assert(pRenderTarget && "Viewport::Viewport")

		F_LogInfo("Viewport::Viewport: [%s], ObjectCamera: [%s], RenderTarget: [%s], Offset: [%f, %f], Size: [%f, %f], ZOrder: [%d] !",
				  nameViewport.c_str(),
                  pObjectCamera ? pObjectCamera->GetName().c_str() : "Null",
				  pRenderTarget->GetName().c_str(), 
                  fLeft, fTop, fWidth, fHeight,
                  nZOrder);

		UpdateDimensions();
		if(pObjectCamera) 
		{
            pObjectCamera->NotifyViewport(this);
        }
    }

    Viewport::~Viewport()
    {
        Destroy();
    }

    void Viewport::Destroy()
    {

    }

    void Viewport::SetObjectCamera(ObjectCamera* pObjectCamera)
	{
		m_pObjectCamera = pObjectCamera;
		UpdateDimensions();
		if (m_pObjectCamera)
		{
            m_pObjectCamera->NotifyViewport(this);
        }
	}

	void Viewport::SetDimensions(float fLeft, float fTop, float fWidth, float fHeight)
	{
		m_fLeft	= fLeft;
		m_fTop = fTop;
		m_fWidth = fWidth;
		m_fHeight = fHeight;
		UpdateDimensions();
	}

	uint32 Viewport::GetNumRenderedFaces() const
	{
		return 0; //m_pObjectCamera ? m_pObjectCamera->GetNumRenderedFaces() : 0;
	}

	uint32 Viewport::GetNumRenderedBatches() const
	{
		return 0; //m_pObjectCamera ? m_pObjectCamera->GetNumRenderedBatches() : 0;
	}

	void Viewport::Update()
	{
		if (m_pObjectCamera)
		{
			//m_pObjectCamera->RenderScene(this, m_bShowOverlays);
		}
	}

	void Viewport::UpdateDimensions()
	{
		float fWidth = (float)m_pRenderTarget->GetWidth();
		float fHeight = (float)m_pRenderTarget->GetHeight();
		
		m_nActLeft = (int)(m_fLeft * fWidth);
		m_nActTop = (int)(m_fTop * fHeight);
		m_nActWidth = (int)(m_fWidth * fWidth);
		m_nActHeight = (int)(m_fHeight * fHeight);

		if (m_pObjectCamera) 
		{
			// if(m_pObjectCamera->GetAutoAspectRatio())
			// {
            //     m_pObjectCamera->SetAspectRatio((float)m_nActWidth/(float)m_nActHeight);
            // }

			F_LogInfo("Viewport::UpdateDimensions: ObjectCamera: [%s], Offset: [%d, %d], Size: [%d, %d] !",
				      m_pObjectCamera ? m_pObjectCamera->GetName().c_str() : "Null",
                      m_nActLeft, m_nActTop, m_nActWidth, m_nActHeight);
		}
		m_bUpdated = true;
	}

}; //LostPeterEngine