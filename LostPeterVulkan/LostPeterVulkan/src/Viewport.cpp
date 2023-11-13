/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Viewport.h"
#include "../include/VulkanWindow.h"
#include "../include/ObjectCamera.h"
#include "../include/RenderTarget.h"

namespace LostPeter
{
    Viewport::Viewport(const String& nameViewport,
                       ObjectCamera* pObjectCamera,
                       RenderTarget* pRenderTarget,
                       float fLeft,
                       float fTop,
                       float fWidth,
                       float fHeight,
                       int nZOrder)
        : Base(nameViewport)
        , m_pObjectCamera(pObjectCamera)
		, m_pRenderTarget(pRenderTarget)
		, m_fLeft(fLeft)
		, m_fTop(fTop)
		, m_fWidth(fWidth)
		, m_fHeight(fHeight)
		, m_nActLeft(0)
		, m_nActTop(0)
		, m_nActWidth(0)
		, m_nActHeight(0)
		, m_nZOrder(nZOrder)
		, m_clBackColor(FMath::ms_clBlack)
		, m_bClearEveryFrame(true)
		, m_nClearBuffers(F_FrameBuffer_Color | F_FrameBuffer_Depth)
		, m_bUpdated(false)
		, m_bShowOverlays(true)
		, m_bShowSkies(true)
		, m_bShowShadows(true)
		, m_nVisibilityMask(0xFFFFFFFF)
		, m_fRangeMinZ(0.0f)
		, m_fRangeMaxZ(1.0f)
    {
		F_LogInfo("Viewport::Viewport: RenderTarget: [%s], ObjectCamera: [%s], Pos: [%f, %f], Size: [%f, %f], ZOrder: [%d] !",
			      pRenderTarget->GetName().c_str(), 
                  !pObjectCamera ? "Null" : pObjectCamera->GetName().c_str(),
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

			F_LogInfo("Viewport::UpdateDimensions: ObjectCamera: [%s], Pos: [%d,%d], Size: [%d,%d] !",
				      m_pObjectCamera ? "Null" : m_pObjectCamera->GetName().c_str(),
                      m_nActLeft, m_nActTop, m_nActWidth, m_nActHeight);
		}
		m_bUpdated = true;
	}


    // bool Viewport::Init(float vpStartX, 
    //                     float vpStartY, 
    //                     float vpWidth, 
    //                     float vpHeight,
    //                     float vpMinDepth,
    //                     float vpMaxDepth,
    //                     VkOffset2D scOffset,
    //                     VkExtent2D scExtent)
    // {
    //     AddViewport(vpStartX,
    //                 vpStartY,
    //                 vpWidth,
    //                 vpHeight,
    //                 vpMinDepth,
    //                 vpMaxDepth,
    //                 scOffset,
    //                 scExtent);

    //     return true;
    // }

    // void Viewport::AddViewport(float vpStartX, 
    //                            float vpStartY, 
    //                            float vpWidth, 
    //                            float vpHeight,
    //                            float vpMinDepth,
    //                            float vpMaxDepth,
    //                            VkOffset2D scOffset,
    //                            VkExtent2D scExtent)
    // {
    //     VkViewport vkViewport = {};
    //     vkViewport.x = vpStartX;
    //     vkViewport.y = vpStartY;
    //     vkViewport.width = vpWidth;
    //     vkViewport.height = vpHeight;
    //     vkViewport.minDepth = vpMinDepth;
    //     vkViewport.maxDepth = vpMaxDepth;

    //     VkRect2D vkScissor = {};
    //     vkScissor.offset = scOffset;
    //     vkScissor.extent = scExtent;

    //     m_aViewports.push_back(vkViewport);
    //     m_aScissors.push_back(vkScissor);
    // }

    // void Viewport::RefreshViewport(int index, const VkExtent2D& vkExtent)
    // {
    //     if (index < 0 || index >= (int)m_aViewports.size())
    //         return;

    //     m_aViewports[index].width = (float)vkExtent.width;
    //     m_aViewports[index].height = (float)vkExtent.height;
    //     m_aScissors[index].extent = vkExtent;
    // }

}; //LostPeter