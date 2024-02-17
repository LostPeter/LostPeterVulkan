/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_PASS_H_
#define _RENDER_PASS_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport RenderPass : public Base
                                  , public FNonCopyable
    {
    public:
        RenderPass(FRenderPassType eRenderPass, FRenderPassSortType eRenderPassSort);
        virtual ~RenderPass();

    public:
    protected:
        FRenderPassType m_eRenderPass;
        FRenderPassSortType m_eRenderPassSort;
        RenderPassDescriptor* m_pRenderPassDescriptor;

        RenderTarget* m_pRenderTarget;
		SceneManager* m_pSceneManager;
		ShaderParamSourceAuto* m_pShaderParamSourceAuto;

        bool m_bIsEnabled;
        bool m_bIsEnableCustomClear;
		bool m_bIsClearEveryFrame;
		uint32 m_nClearBuffers;
		bool m_bOriClearEveryFrame;
		uint32 m_nOriClearBuffers;

        RenderPassListenerPtrVector m_aRenderPassListener;

    public:
        F_FORCEINLINE FRenderPassType GetRenderPassType() const { return m_eRenderPass; }
        F_FORCEINLINE FRenderPassSortType GetRenderPassSortType() const { return m_eRenderPassSort; }
        F_FORCEINLINE RenderPassDescriptor* GetRenderPassDescriptor() const { return m_pRenderPassDescriptor; }

        F_FORCEINLINE bool GetIsEnabled() const { return m_bIsEnabled; }
		F_FORCEINLINE void SetIsEnabled(bool bIsEnabled) { m_bIsEnabled = bIsEnabled; }
        F_FORCEINLINE bool GetIsEnableCustomClear() const { return m_bIsEnableCustomClear; }
		F_FORCEINLINE void SetIsEnableCustomClear(bool bIsEnableCustomClear) { m_bIsEnableCustomClear = bIsEnableCustomClear; }
		F_FORCEINLINE bool GetClearEveryFrame() const { return m_bIsClearEveryFrame; }
		F_FORCEINLINE uint32 GetClearBuffers() const { return m_nClearBuffers; }
		F_FORCEINLINE void SetClearEveryFrame(bool bIsClearEveryFrame,
                                              uint32 nClearBuffers = F_FrameBuffer_Color | F_FrameBuffer_Depth | F_FrameBuffer_Stencil)
		{
			m_bIsClearEveryFrame = bIsClearEveryFrame;
			m_nClearBuffers = nClearBuffers;
		}

    public:
        virtual void Destroy();
        virtual bool Init();

        virtual RenderTarget* GetRenderTarget() const { return m_pRenderTarget; }
        virtual void SetRenderTarget(RenderTarget* pRenderTarget) { m_pRenderTarget = pRenderTarget; }
		virtual SceneManager* GetSceneManager() const { return m_pSceneManager; }
		virtual void SetSceneManager(SceneManager* pSceneManager) { m_pSceneManager = pSceneManager; }
        virtual ShaderParamSourceAuto* GetShaderParamAutoSource() const { return m_pShaderParamSourceAuto; }
		virtual void SetShaderParamAutoSource(ShaderParamSourceAuto* pShaderParamAutoSource) { m_pShaderParamSourceAuto = pShaderParamAutoSource; }

        void AddLRenderPassListener(RenderPassListener* pRenderPassListener);
		void RemoveRenderPassListener(RenderPassListener* pRenderPassListener);
		void RemoveRenderPassListenerAll();

    public:
		virtual void AddQueue(Renderable* pRenderable) = 0;
		virtual void ClearQueue() = 0;

		virtual void Render(Renderer* pRenderer) = 0;
		
	protected:
		virtual void preRender(Renderer* pRenderer);
		virtual void postRender(Renderer* pRenderer);
    
    protected:
        virtual void destroyRenderPassDescriptor();

        virtual bool createRenderPassDescriptor();
    };

}; //LostPeterEngine

#endif