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

#ifndef _VIEWPORT_H_
#define _VIEWPORT_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Viewport : public Base
							    , public FNonCopyable
    {
    public:
        Viewport(const String& nameViewport,
                 ObjectCamera* pObjectCamera,
                 RenderTarget* pRenderTarget,
				 int nZOrder,
                 float fLeft = 0.0f,
                 float fTop = 0.0f,
                 float fWidth = 1.0f,
                 float fHeight = 1.0f,
				 float fDepthMin = 0.0f,
				 float fDepthMax = 1.0f);
        virtual ~Viewport();

    public:
    protected:
        ObjectCamera* m_pObjectCamera;			
		RenderTarget* m_pRenderTarget;

		int	m_nZOrder;

		float m_fLeft;
		float m_fTop;
		float m_fWidth;
		float m_fHeight;
		
		int	m_nActLeft;
		int	m_nActTop;
		int	m_nActWidth;
		int	m_nActHeight;

		float m_fDepthMin;
		float m_fDepthMax;

		FColor m_clBackGroundColor;
		bool m_bClearEveryFrame;
		uint32 m_nClearBuffers;
		bool m_bUpdated;
		bool m_bShowOverlays;
		bool m_bShowSkies;
		bool m_bShowShadows;
		uint32 m_nVisibilityMask;

    public:
        void Destroy();

    public:
        F_FORCEINLINE ObjectCamera* GetObjectCamera() const { return m_pObjectCamera; }
		void SetObjectCamera(ObjectCamera* pObjectCamera);
		F_FORCEINLINE RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
		F_FORCEINLINE float GetLeft() const { return m_fLeft; }	
		F_FORCEINLINE float GetTop() const	{ return m_fTop; }
		F_FORCEINLINE float GetWidth() const { return m_fWidth; }
		F_FORCEINLINE float GetHeight() const { return m_fHeight; }
		void SetDimensions(float fLeft, float fTop, float fWidth, float fHeight);
		F_FORCEINLINE int GetActualLeft() const { return m_nActLeft; }
		F_FORCEINLINE int GetActualTop() const	{ return m_nActTop; }
		F_FORCEINLINE int GetActualWidth() const { return m_nActWidth; }
		F_FORCEINLINE int GetActualHeight() const { return m_nActHeight; }
		F_FORCEINLINE void GetActualDimensions(int& nLeft, int& nTop, int& nWidth, int& nHeight) const
		{
			nLeft = m_nActLeft;
			nTop = m_nActTop;
			nWidth = m_nActWidth;
			nHeight	= m_nActHeight;
		}
		F_FORCEINLINE int GetZOrder() const { return m_nZOrder; }

		F_FORCEINLINE float GetDepthMin() { return m_fDepthMin; }
		F_FORCEINLINE float GetDepthMax() { return m_fDepthMax; }
		F_FORCEINLINE void SetDepthMinMax(float fDepthMin,float fDepthMax)
		{
			m_fDepthMin = fDepthMin;
			m_fDepthMax = fDepthMax;
			m_bUpdated = true;
		}

		F_FORCEINLINE const FColor& GetBackGroundColor() const { return m_clBackGroundColor; }
		F_FORCEINLINE void SetBackGroundColor(const FColor& color)	{ m_clBackGroundColor = color; }
		F_FORCEINLINE bool GetClearEveryFrame() const { return m_bClearEveryFrame; }
		F_FORCEINLINE void SetClearEveryFrame(bool bClear, uint32 buffers = F_FrameBuffer_Color | F_FrameBuffer_Depth)
		{
			m_bClearEveryFrame = bClear;
			m_nClearBuffers = buffers;
		}
		F_FORCEINLINE uint32 GetClearBuffers() const { return m_nClearBuffers; }
		F_FORCEINLINE bool IsUpdated() const { return m_bUpdated; }
		F_FORCEINLINE void ClearUpdatedFlag() { m_bUpdated = false; }
		F_FORCEINLINE bool GetOverlaysEnabled() const { return m_bShowOverlays; }
		F_FORCEINLINE void SetOverlaysEnabled(bool b) { m_bShowOverlays = b; }
		F_FORCEINLINE bool GetSkiesEnabled() const { return m_bShowSkies; }
		F_FORCEINLINE void SetSkiesEnabled(bool b)	{ m_bShowSkies = b; }
		F_FORCEINLINE bool GetShadowsEnabled() const { return m_bShowShadows; }
		F_FORCEINLINE void SetShadowsEnabled(bool b) { m_bShowShadows = b; }
		F_FORCEINLINE uint32 GetVisibilityMask() const { return m_nVisibilityMask; }
		F_FORCEINLINE void SetVisibilityMask(uint32 nMask)	{ m_nVisibilityMask = nMask; }
			
		uint32 GetNumRenderedFaces() const;
		uint32 GetNumRenderedBatches() const;
				
	public:
		void Update();
		void UpdateDimensions();
    };

}; //LostPeterEngine

#endif