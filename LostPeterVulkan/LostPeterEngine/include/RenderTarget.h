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

#ifndef _RENDER_TARGET_H_
#define _RENDER_TARGET_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderTarget : public Base
                                    , public FNonCopyable
    {
    public:
        RenderTarget(const String& nameRenderTarget);
        virtual ~RenderTarget();

    public:
    protected:
        uint32 m_nPriority;				

		uint32 m_nWidth;					
		uint32 m_nHeight;					
		uint32 m_nColorDepth;	

        bool m_bActive;					
		bool m_bAutoUpdate;				
		bool m_bHwGamma;

		uint32 m_nMSAA;	
	
		ViewportPtrOrderMap m_mapViewport;
		RenderTargetListenerPtrVector m_aRenderTargetListener;
		
		bool m_bIsDepthBuffered;
		bool m_bEnableDepthBuf;
		RenderTarget* m_pShareDepthStencilRT;

    public:
		virtual uint32 GetPriority() const { return m_nPriority; }
		virtual void SetPriority(uint32 nPriority) { m_nPriority = nPriority; }

		virtual uint32 GetWidth() const	{ return m_nWidth; }
		virtual uint32 GetHeight() const { return m_nHeight; }
		virtual uint32 GetColorDepth() const { return m_nColorDepth; }
		virtual void GetMetrics(uint32& nWidth, uint32& nHeight, uint32& nColorDepth);
		
		virtual bool IsPrimary() const { return false; }

		virtual bool GetEnableDepthBuf() { return m_bEnableDepthBuf; }
		virtual void SetEnableDepthBuf(bool bEnable) { m_bEnableDepthBuf = bEnable; }

		virtual bool IsActive() const { return m_bActive; }
		virtual void SetActive(bool bState)	{ m_bActive = bState; }

		virtual bool IsAutoUpdated() const { return m_bAutoUpdate; }
		virtual void SetAutoUpdated(bool bAutoUpdate) { m_bAutoUpdate = bAutoUpdate; }
		
		virtual bool IsHardwareGammaEnabled() const	{ return m_bHwGamma; }
		virtual uint32 GetMSAA() const { return m_nMSAA; }
		
		virtual void AddRenderTargetListener(RenderTargetListener* pRenderTargetListener);
		virtual void RemoveRenderTargetListener(RenderTargetListener* pRenderTargetListener);
		virtual void RemoveAllRenderTargetListeners();

    public:
		virtual int GetViewportCount() const;
		virtual Viewport* GetViewport(int nIndex);
		virtual Viewport* GetViewportByZOrder(int nZOrder);
		virtual Viewport* AddViewport(const String& nameViewport,
                                      ObjectCamera* pObjectCamera,
                                      int nZOrder = 0, 
									  float fLeft = 0.0f,
                                      float fTop = 0.0f,
									  float fWidth = 1.0f,
                                      float fHeight = 1.0f);
		virtual void RemoveViewport(int nZOrder);
		virtual void RemoveAllViewport();

        virtual void Update(bool bSwapBuffers = true);
		virtual bool SwapBuffers(bool bSwapBuffers = true) = 0;
			
		virtual void NotifyObjectCameraRemoved(const ObjectCamera* pCamera);

		virtual bool RequiresTextureFlipping() const = 0;

		virtual bool DestroyDepthStencil() { return false; }
		virtual bool CreateDepthStencil() { return false; }
		virtual bool SetDepthStencil(void* pData) { return false; }
		virtual void ShareDepthStencil(RenderTarget* pRT) { m_pShareDepthStencilRT = pRT; }

		virtual void GetCustomAttribute(const String& strName, void* pData);

    protected:
		virtual void firePreUpdate();
		virtual void firePostUpdate();
		virtual void fireViewportPreUpdate(Viewport* pViewport);
		virtual void fireViewportPostUpdate(Viewport* pViewport);
		virtual void fireViewportAdded(Viewport* pViewport);
		virtual void fireViewportRemoved(Viewport* pViewport);

		virtual void updateImpl();
    };

}; //LostPeterEngine

#endif