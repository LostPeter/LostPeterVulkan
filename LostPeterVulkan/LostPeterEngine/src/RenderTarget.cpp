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

#include "../include/RenderTarget.h"
#include "../include/RenderTargetListener.h"
#include "../include/Viewport.h"
#include "../include/ObjectCamera.h"

namespace LostPeterEngine
{
    RenderTarget::RenderTarget(const String& nameRenderTarget)
        : Base(nameRenderTarget)
        , m_nPriority(0)
		, m_nWidth(0)
		, m_nHeight(0)
		, m_nColorDepth(32)
		, m_bActive(true)
		, m_bAutoUpdate(true)
		, m_bHwGamma(false)
		, m_nMSAA(0)
		, m_bIsDepthBuffered(false)
		, m_bEnableDepthBuf(true)
		, m_pShareDepthStencilRT(nullptr)
    {

    }
	
    RenderTarget::~RenderTarget()
    {
        for (ViewportPtrOrderMap::iterator it = m_mapViewport.begin();
			it != m_mapViewport.end(); ++it)
		{
			fireViewportRemoved(it->second);
            F_DELETE(it->second)
		}
		F_LogInfo("RenderTarget::~RenderTarget: Render Target: [%s] is deleted !", GetName().c_str());
    }

    void RenderTarget::GetMetrics(uint32& nWidth, uint32& nHeight, uint32& nColorDepth)
	{
		nWidth = m_nWidth;
		nHeight	= m_nHeight;
		nColorDepth = m_nColorDepth;
	}

	void RenderTarget::AddRenderTargetListener(RenderTargetListener* pRenderTargetListener)
	{
		m_aRenderTargetListener.push_back(pRenderTargetListener);
	}

	void RenderTarget::RemoveRenderTargetListener(RenderTargetListener* pRenderTargetListener)
	{
		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			if (*it == pRenderTargetListener)
			{
				m_aRenderTargetListener.erase(it);
				break;
			}
		}
	}

	void RenderTarget::RemoveAllRenderTargetListeners()
	{
		m_aRenderTargetListener.clear();
	}
	
	int RenderTarget::GetViewportCount() const
	{		
		return (int)m_mapViewport.size();
	}
	Viewport* RenderTarget::GetViewport(int nIndex)
	{
		F_Assert(nIndex < m_mapViewport.size() && "RenderTarget::GetViewport: Index out of bounds !");
		ViewportPtrOrderMap::iterator itFind = m_mapViewport.begin();
		while (nIndex--)
			++itFind;
		return itFind->second;
	}
	Viewport* RenderTarget::GetViewportByZOrder(int nZOrder)
	{
		ViewportPtrOrderMap::iterator itFind = m_mapViewport.find(nZOrder);
        if (itFind == m_mapViewport.end())
        {
			F_LogError("*********************** RenderTarget::GetViewportByZOrder: No viewport with given Z-Order: [%d] !", nZOrder);
			return nullptr;
        }
        return itFind->second;
	}
	Viewport* RenderTarget::AddViewport(const String& nameViewport,
                                        ObjectCamera* pObjectCamera,
                                        int nZOrder /*= 0*/, 
										float fLeft /*= 0.0f*/,
                                        float fTop /*= 0.0f*/,
										float fWidth /*= 1.0f*/,
                                        float fHeight /*= 1.0f*/)
	{
		ViewportPtrOrderMap::iterator itFind = m_mapViewport.find(nZOrder);
		if (itFind != m_mapViewport.end())
		{
			F_LogError("*********************** RenderTarget::AddViewport: Can't create another viewport for [%s], because a viewport exists with this Z-Order already !", GetName().c_str());
			return nullptr;
		}
		Viewport* pViewport = new Viewport(nameViewport, pObjectCamera, this, fLeft, fTop, fWidth, fHeight, nZOrder);
		m_mapViewport.insert(ViewportPtrOrderMap::value_type(nZOrder, pViewport));

		fireViewportAdded(pViewport);
		return pViewport;
	}
	void RenderTarget::RemoveViewport(int nZOrder)
	{
		ViewportPtrOrderMap::iterator itFind = m_mapViewport.find(nZOrder);
		if (itFind != m_mapViewport.end())
		{
			fireViewportRemoved(itFind->second);
            F_DELETE(itFind->second)
			m_mapViewport.erase(itFind);
		}
	}
	void RenderTarget::RemoveAllViewport()
	{
		for (ViewportPtrOrderMap::iterator it = m_mapViewport.begin();
			 it != m_mapViewport.end(); ++it)
		{
			fireViewportRemoved(it->second);
            F_DELETE(it->second)
		}
		m_mapViewport.clear();
	}

	void RenderTarget::Update(bool bSwapBuffers /*= true*/)
	{
		updateImpl();

		if (bSwapBuffers)
        {
            SwapBuffers();
        }
	}

	void RenderTarget::NotifyObjectCameraRemoved(const ObjectCamera* pObjectCamera)
	{
		for (ViewportPtrOrderMap::iterator it = m_mapViewport.begin(); 
			 it != m_mapViewport.end(); ++it)
		{
			Viewport* pViewport = it->second;
			if (pViewport->GetObjectCamera() == pObjectCamera)
			{
				pViewport->SetObjectCamera(nullptr);
			}
		}
	}

	void RenderTarget::GetCustomAttribute(const String& strName, void* pData)
	{
		F_Assert(false && "RenderTarget::GetCustomAttribute")

	}

	void RenderTarget::firePreUpdate()
	{
		RenderTargetEvent evt;
		evt.pRenderTargetSrc = this;

		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			(*it)->PreRenderTargetUpdate(evt);
		}
	}

	void RenderTarget::firePostUpdate()
	{
		RenderTargetEvent evt;
		evt.pRenderTargetSrc = this;

		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			(*it)->PostRenderTargetUpdate(evt);
		}
	}		

	void  RenderTarget::fireViewportPreUpdate(Viewport* pViewport)
	{
		RenderTargetViewportEvent evt;
		evt.pViewportSrc = pViewport;

		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			(*it)->PreViewportUpdate(evt);
		}
	}

	void RenderTarget::fireViewportPostUpdate(Viewport* pViewport)
	{
		RenderTargetViewportEvent evt;
		evt.pViewportSrc = pViewport;

		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			(*it)->PostViewportUpdate(evt);
		}
	}

	void RenderTarget::fireViewportAdded(Viewport* pViewport)
	{	
		RenderTargetViewportEvent evt;
		evt.pViewportSrc = pViewport;

		for (RenderTargetListenerPtrVector::iterator it = m_aRenderTargetListener.begin(); 
			 it != m_aRenderTargetListener.end(); ++it)
		{
			(*it)->ViewportAdded(evt);
		}
	}

	void RenderTarget::fireViewportRemoved(Viewport* pViewport)
	{
		RenderTargetViewportEvent evt;
		evt.pViewportSrc = pViewport;
		
		RenderTargetListenerPtrVector tempList = m_aRenderTargetListener;
		for (RenderTargetListenerPtrVector::iterator it = tempList.begin(); 
			 it != tempList.end(); ++it)
		{
			(*it)->ViewportRemoved(evt);
		}
	}

	void RenderTarget::updateImpl()
	{
		firePreUpdate();
        {
            ViewportPtrOrderMap::iterator it = m_mapViewport.begin();
            while (it != m_mapViewport.end())
            {
                fireViewportPreUpdate(it->second);
                it->second->Update();
                fireViewportPostUpdate(it->second);
                ++it;
            }
        }
		firePostUpdate();
	}

}; //LostPeterEngine