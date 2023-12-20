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

#include "../include/RenderPipelineManager.h"
#include "../include/RenderPipeline.h"
#include "../include/RenderPipelineForward.h"
#include "../include/RenderPassManager.h"

template<> LostPeterEngine::RenderPipelineManager* LostPeterFoundation::FSingleton<LostPeterEngine::RenderPipelineManager>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    RenderPipelineManager* RenderPipelineManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderPipelineManager& RenderPipelineManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderPipelineManager::GetSingleton")
		return (*ms_Singleton);     
	}

    RenderPipelineManager::RenderPipelineManager()
        : Base("RenderPipelineManager")
        , m_pPipeLineCurrent(nullptr)
    {
        m_pRenderPassManager = new RenderPassManager;
		m_mapRenderPipeLine[F_GetRenderPipelineTypeName(F_RenderPipeline_Forward)] = new RenderPipelineForward(this);
		//m_mapRenderPipeLine[F_GetRenderPipelineTypeName(F_RenderPipeline_Deferred)] = new RenderPipelineDeferred(this);
    }

    RenderPipelineManager::~RenderPipelineManager()
    {
        Destroy();
    }

    void RenderPipelineManager::Destroy()
	{
		for (RenderPipelinePtrMap::iterator it = m_mapRenderPipeLine.begin(); 
		     it != m_mapRenderPipeLine.end(); ++it)
		{
			delete it->second;
		}
		m_mapRenderPipeLine.clear();
		F_DELETE(m_pRenderPassManager)
	}

    bool RenderPipelineManager::Init(const String& strPipeLineName)
	{
		F_Assert(!strPipeLineName.empty() && "RenderPipelineManager::Init")
		RenderPipelinePtrMap::iterator itFind = m_mapRenderPipeLine.find(strPipeLineName);
		if (itFind == m_mapRenderPipeLine.end())
		{
			F_Assert(false && "RenderPipelineManager::Init: Wrong pipeline type !")
			return false;
		}
		m_pPipeLineCurrent = itFind->second;

		if (!m_pPipeLineCurrent->Init())
		{
			F_LogError("*********************** RenderPipelineManager::Init: Init pipe line failed, name: [%s] !", strPipeLineName.c_str());
			F_Assert(false && "RenderPipelineManager::Init: Init pipe line failed !")
			return false;
		}

		return true;
	}

	bool RenderPipelineManager::RenderOneFrame(RenderFrameEvent& event)
	{
		return m_pPipeLineCurrent->RenderOneFrame(event);
	}

	bool RenderPipelineManager::ChangePipeLineTo(FRenderPipelineType eRenderPipeline)
	{
		

		return true;
	}
    

}; //LostPeterEngine