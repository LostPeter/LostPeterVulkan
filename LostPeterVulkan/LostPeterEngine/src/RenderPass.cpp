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

#include "../include/RenderPass.h"
#include "../include/RenderPassListener.h"
#include "../include/RenderPassDescriptor.h"
#include "../include/RenderPassDescriptorManager.h"
#include "../include/RenderTarget.h"
#include "../include/SceneManager.h"
#include "../include/ShaderParamSourceAuto.h"
#include "../include/Viewport.h"

namespace LostPeterEngine
{
    RenderPass::RenderPass(FRenderPassType eRenderPass, FRenderPassSortType eRenderPassSort)
        : Base(F_GetRenderPassTypeName(eRenderPass))
        , m_eRenderPass(eRenderPass)
        , m_eRenderPassSort(eRenderPassSort)
        , m_pRenderPassDescriptor(nullptr)
        , m_pRenderTarget(nullptr)
        , m_pSceneManager(nullptr)
        , m_pShaderParamSourceAuto(nullptr)
        , m_bIsEnabled(true)
        , m_bIsEnableCustomClear(false)
		, m_bIsClearEveryFrame(false)
		, m_nClearBuffers(F_FrameBuffer_Color | F_FrameBuffer_Depth | F_FrameBuffer_Stencil)
		, m_bOriClearEveryFrame(false)
		, m_nOriClearBuffers(F_FrameBuffer_Color | F_FrameBuffer_Depth | F_FrameBuffer_Stencil)
    {

    }

    RenderPass::~RenderPass()
    {

    }

    void RenderPass::Destroy()
    {
        RemoveRenderPassListenerAll();

        destroyRenderPassDescriptor();
    }
        void RenderPass::destroyRenderPassDescriptor()
        {
            RenderPassDescriptorManager::GetSingleton().DestroyRenderPassDescriptor(m_pRenderPassDescriptor);
            m_pRenderPassDescriptor = nullptr;
        }
    
    bool RenderPass::Init()
    {
        //1> createRenderPassDescriptor
        if (!createRenderPassDescriptor())
        {
            F_LogError("*********************** RenderPass::Init: createRenderPassDescriptor failed, RenderPass Type: [%s] !", GetName().c_str());
            return false;
        }

        return true;
    }
        bool RenderPass::createRenderPassDescriptor()
        {
            String strNameRenderPassDescriptor = "RenderPassDescriptor_" + GetName();
            m_pRenderPassDescriptor = RenderPassDescriptorManager::GetSingleton().CreateRenderPassDescriptor(strNameRenderPassDescriptor, m_eRenderPass);
            if (m_pRenderPassDescriptor == nullptr)
            {
                return false;
            }
            return true;    
        }
    
    void RenderPass::AddLRenderPassListener(RenderPassListener* pRenderPassListener)
    {
        RenderPassListenerPtrVector::iterator itFind = std::find(m_aRenderPassListener.begin(), m_aRenderPassListener.end(), pRenderPassListener);
		if (itFind == m_aRenderPassListener.end())
			m_aRenderPassListener.push_back(pRenderPassListener);
    }
    void RenderPass::RemoveRenderPassListener(RenderPassListener* pRenderPassListener)
    {
        RenderPassListenerPtrVector::iterator itFind = std::find(m_aRenderPassListener.begin(), m_aRenderPassListener.end(), pRenderPassListener);
		if (itFind != m_aRenderPassListener.end())
			m_aRenderPassListener.erase(itFind);
    }
	void RenderPass::RemoveRenderPassListenerAll()
    {
        m_aRenderPassListener.clear();
    }
   
    void RenderPass::preRender(Renderer* pRenderer)
    {
        Viewport* pViewport = m_pRenderTarget->GetViewport(0);
		if (m_bIsEnableCustomClear)
		{
			m_bOriClearEveryFrame = pViewport->GetClearEveryFrame();
			m_nOriClearBuffers    = pViewport->GetClearBuffers(); 
			pViewport->SetClearEveryFrame(m_bIsClearEveryFrame, m_nClearBuffers);
		}
		
		RenderPassListenerPtrVector::iterator it,itEnd;
		itEnd = m_aRenderPassListener.end();
		for (it = m_aRenderPassListener.begin(); it != itEnd; ++it)
		{
			(*it)->PreRenderPassRender(pViewport);
		}
    }
	void RenderPass::postRender(Renderer* pRenderer)
    {
        Viewport* pViewport = m_pRenderTarget->GetViewport(0);
		if (m_bIsEnableCustomClear)
		{
			pViewport->SetClearEveryFrame(m_bOriClearEveryFrame, m_nOriClearBuffers);
		}
	
		RenderPassListenerPtrVector::iterator it,itEnd;
		itEnd = m_aRenderPassListener.end();
		for (it = m_aRenderPassListener.begin(); it != itEnd; ++it)
		{
			(*it)->PostRenderPassRender(pViewport);
		}
    }

}; //LostPeterEngine