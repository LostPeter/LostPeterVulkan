/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Renderer.h"
#include "../include/RendererListener.h"
#include "../include/RenderTargetManager.h"
#include "../include/RenderTarget.h"
#include "../include/RenderWindow.h"
#include "../include/Viewport.h"
#include "../include/ObjectCamera.h"
#include "../include/VertexElement.h"

namespace LostPeterEngine
{
    Renderer::Renderer(const String& nameRenderer)
        : Base(nameRenderer)
        , m_pRenderWindow(nullptr)
        , m_pActiveRenderTarget(nullptr)
        , m_pActiveViewport(nullptr)
        , m_pActiveObjectCamera(nullptr)
        , m_bRendererIsInit(false)
        , m_bFullScreen(false)
		, m_bFakeFullScreen(false)
		, m_bUseNvPerfHud(false)
		, m_bVSync(true)
		, m_bWBuffer(false)
		, m_bEmptyGpuBuffer(false)
		, m_nCurWidth(0)
		, m_nCurHeight(0)
    {
        m_pRenderTargetManager = new RenderTargetManager(this);
    }
    
    Renderer::~Renderer()
    {

    }

    void Renderer::Destroy()
    {

        F_DELETE(m_pRenderTargetManager)
    }

	RenderTarget* Renderer::GetRenderTarget(const String& strName)
	{
		return m_pRenderTargetManager->GetRenderTarget(strName);
	}
	void Renderer::AttachRenderTarget(RenderTarget* pRenderTarget)
	{
		m_pRenderTargetManager->AttachRenderTarget(pRenderTarget);
	}
	RenderTarget* Renderer::DetachRenderTarget(const String& strName)
	{
		return m_pRenderTargetManager->DetachRenderTarget(strName);
	}
	void Renderer::DestroyRenderTarget(const String& strName)
	{
		m_pRenderTargetManager->DestroyRenderTarget(strName);
	}
	void Renderer::DestroyRenderWindow(const String& strName)
	{
		m_pRenderTargetManager->DestroyRenderWindow(strName);
	}
	void Renderer::DestroyRenderTexture(const String& strName)
	{
		m_pRenderTargetManager->DestroyRenderTexture(strName);
	}

    void Renderer::UpdateRenderTargetPriority(uint8 nPriority, RenderTarget* pRenderTarget)
    {
        RenderTargetPtrPriorityMap& mapRenderTargetPtrPriority = m_pRenderTargetManager->GetRenderTargetPtrPriorityMap();
        for(RenderTargetPtrPriorityMap::iterator it = mapRenderTargetPtrPriority.begin(); 
			it != mapRenderTargetPtrPriority.end(); ++it)
		{
			if (it->second == pRenderTarget && it->first != nPriority)
			{
				mapRenderTargetPtrPriority.erase(it);
				mapRenderTargetPtrPriority.insert(RenderTargetPtrPriorityMap::value_type(pRenderTarget->GetPriority(), pRenderTarget));
				break;
			}
		}
    }

    void Renderer::UpdateRenderTargetAll()
    {
        uint32 nCount = 0;
		BeginFrame();
        RenderTargetPtrPriorityMap& mapRenderTargetPtrPriority = m_pRenderTargetManager->GetRenderTargetPtrPriorityMap();
		RenderTargetPtrPriorityMap::iterator it,itEnd;
		itEnd = mapRenderTargetPtrPriority.end();
		for (it = mapRenderTargetPtrPriority.begin(); it != itEnd; ++it)
		{
			RenderTarget* pRenderTarget = it->second;
			if(pRenderTarget->IsActive() && pRenderTarget->IsAutoUpdated())
			{
				pRenderTarget->Update();
				++nCount;
			}
		}
		EndFrame();
		m_pRenderWindow->Present(this);
    }

    void Renderer::NotifyObjectCameraRemoved(const ObjectCamera* pObjectCamera)
    {
        RenderTargetPtrMap& mapRenderTarget = m_pRenderTargetManager->GetRenderTargetPtrMap();
        for (RenderTargetPtrMap::iterator it = mapRenderTarget.begin();
			it != mapRenderTarget.end();++it)
		{
			RenderTarget* pRenderTarget = it->second;
			pRenderTarget->NotifyObjectCameraRemoved(pObjectCamera);
		}
    }

    void Renderer::AddRendererListener(RendererListener* pRendererListener) 
    { 
        m_listRendererListeners.push_back(pRendererListener); 
    }	

    void Renderer::RemoveRendererListener(RendererListener* pRendererListener) 
    { 
        m_listRendererListeners.remove(pRendererListener); 
    }

	void Renderer::ConvertColorValue(const FColor& color, uint32* pDest)
	{
		*pDest = VertexElement::ConvertColorValue(color, GetColorVertexElementType());
	}

    void Renderer::Present()
	{
		if (m_pRenderWindow)
		{
			m_pRenderWindow->Present(this);
		}
	}

    bool Renderer::SetViewProjectionMatrix(ObjectCamera* pObjectCamera)
	{
		m_pActiveObjectCamera = pObjectCamera;
		if (m_pActiveRenderTarget)
		{
			//SetViewMatrix(pObjectCamera->GetViewMatrix(true));
			//SetProjectionMatrix(pObjectCamera->GetProjectionMatrixRS());
		}
		return true;
	}

}; //LostPeterEngine