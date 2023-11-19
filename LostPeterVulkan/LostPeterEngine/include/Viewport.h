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

namespace LostPeter
{
    class utilExport Viewport : public Base
							  , public FNonCopyable
    {
    public:
        Viewport(const String& nameViewport,
                 ObjectCamera* pObjectCamera,
                 RenderTarget* pRenderTarget,
                 float fLeft,
                 float fTop,
                 float fWidth,
                 float fHeight,
                 int nZOrder);
        virtual ~Viewport();

    public:
    protected:
        ObjectCamera* m_pObjectCamera;			
		RenderTarget* m_pRenderTarget;

		float m_fLeft;
		float m_fTop;
		float m_fWidth;
		float m_fHeight;
		
		int	m_nActLeft;
		int	m_nActTop;
		int	m_nActWidth;
		int	m_nActHeight;
		
		int	m_nZOrder;

		FColor m_clBackColor;
		bool m_bClearEveryFrame;
		uint32 m_nClearBuffers;
		bool m_bUpdated;
		bool m_bShowOverlays;
		bool m_bShowSkies;
		bool m_bShowShadows;
		uint32 m_nVisibilityMask;

		float m_fRangeMinZ;
		float m_fRangeMaxZ;

        VkViewportVector m_aViewports;
        VkRect2DVector m_aScissors;

    public:
        LP_FORCEINLINE const VkViewportVector& GetVkViewportVector() const { return this->m_aViewports; }
        LP_FORCEINLINE VkViewportVector& GetVkViewportVector() { return this->m_aViewports; }
        LP_FORCEINLINE const VkRect2DVector& GetVkRect2DVector() const { return this->m_aScissors; }
        LP_FORCEINLINE VkRect2DVector& GetVkRect2DVector() { return this->m_aScissors; }

    public:
        void Destroy();

    public:
        LP_FORCEINLINE ObjectCamera* GetObjectCamera() const { return m_pObjectCamera; }
		void SetObjectCamera(ObjectCamera* pObjectCamera);
		LP_FORCEINLINE RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
		LP_FORCEINLINE float GetLeft() const { return m_fLeft; }	
		LP_FORCEINLINE float GetTop() const	{ return m_fTop; }
		LP_FORCEINLINE float GetWidth() const { return m_fWidth; }
		LP_FORCEINLINE float GetHeight() const { return m_fHeight; }
		void SetDimensions(float fLeft, float fTop, float fWidth, float fHeight);
		LP_FORCEINLINE int GetActualLeft() const { return m_nActLeft; }
		LP_FORCEINLINE int GetActualTop() const	{ return m_nActTop; }
		LP_FORCEINLINE int GetActualWidth() const { return m_nActWidth; }
		LP_FORCEINLINE int GetActualHeight() const { return m_nActHeight; }
		LP_FORCEINLINE void GetActualDimensions(int& nLeft, int& nTop, int& nWidth, int& nHeight) const
		{
			nLeft = m_nActLeft;
			nTop = m_nActTop;
			nWidth = m_nActWidth;
			nHeight	= m_nActHeight;
		}
		LP_FORCEINLINE int GetZOrder() const { return m_nZOrder; }
		LP_FORCEINLINE const FColor& GetBackGroundColor() const { return m_clBackColor; }
		LP_FORCEINLINE void	SetBackGroundColor(const FColor& color)	{ m_clBackColor=color; }
		LP_FORCEINLINE bool	GetClearEveryFrame() const { return m_bClearEveryFrame; }
		LP_FORCEINLINE void	SetClearEveryFrame(bool bClear, uint32 buffers = F_FrameBuffer_Color | F_FrameBuffer_Depth)
		{
			m_bClearEveryFrame = bClear;
			m_nClearBuffers = buffers;
		}
		LP_FORCEINLINE uint32 GetClearBuffers() const { return m_nClearBuffers; }
		LP_FORCEINLINE bool IsUpdated() const { return m_bUpdated; }
		LP_FORCEINLINE void ClearUpdatedFlag() { m_bUpdated = false; }
		LP_FORCEINLINE bool	GetOverlaysEnabled() const { return m_bShowOverlays; }
		LP_FORCEINLINE void	SetOverlaysEnabled(bool b) { m_bShowOverlays = b; }
		LP_FORCEINLINE bool	GetSkiesEnabled() const { return m_bShowSkies; }
		LP_FORCEINLINE void	SetSkiesEnabled(bool b)	{ m_bShowSkies = b; }
		LP_FORCEINLINE bool	GetShadowsEnabled() const { return m_bShowShadows; }
		LP_FORCEINLINE void SetShadowsEnabled(bool b) { m_bShowShadows = b; }
		LP_FORCEINLINE uint32 GetVisibilityMask() const { return m_nVisibilityMask; }
		LP_FORCEINLINE void	SetVisibilityMask(uint32 nMask)	{ m_nVisibilityMask = nMask; }
		LP_FORCEINLINE float GetRangeMinZ() { return m_fRangeMinZ; }
		LP_FORCEINLINE float GetRangeMaxZ() { return m_fRangeMaxZ; }
		LP_FORCEINLINE void	SetRangeZ(float fMinZ,float fMaxZ)
		{
			m_fRangeMinZ = fMinZ;
			m_fRangeMaxZ = fMaxZ;
			m_bUpdated = true;
		}
			
		uint32 GetNumRenderedFaces() const;
		uint32 GetNumRenderedBatches() const;
				
	public:
		void Update();
		void UpdateDimensions();
    };

}; //LostPeter

#endif