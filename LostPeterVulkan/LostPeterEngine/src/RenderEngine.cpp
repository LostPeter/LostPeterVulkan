/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-16
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RenderEngine.h"
#include "../include/MeshManager.h"
#include "../include/TextureManager.h"
#include "../include/ShaderManager.h"
#include "../include/MaterialDataManager.h"
#include "../include/MaterialManager.h"
#include "../include/SceneManagerEnumerator.h"
#include "../include/SceneManager.h"
#include "../include/Viewport.h"
#include "../include/Renderer.h"
#include "../include/RenderTarget.h"
#include "../include/RenderPipelineManager.h"
#include "../include/RenderPipeline.h"
#include "../include/RenderWindow.h"
#include "../include/RenderQueueManager.h"
#include "../include/RenderTargetManager.h"
#include "../include/MovableFactory.h"

template<> LostPeterEngine::RenderEngine* LostPeterFoundation::FSingleton<LostPeterEngine::RenderEngine>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    RenderEngine* RenderEngine::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RenderEngine& RenderEngine::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RenderEngine::GetSingleton")
		return (*ms_Singleton);     
	}


    RenderEngine::RenderEngine()
        : Base("RenderEngine")
        , m_pRendererCurrent(nullptr)
		, m_pRenderWindowAuto(nullptr)
		, m_pRenderPipeLineManager(nullptr)
		, m_pSceneManagerEnumerator(nullptr)
		, m_pSceneManagerCurrent(nullptr)
		, m_pRenderQueueManager(nullptr)
        , m_pMeshManager(nullptr)
        , m_pTextureManager(nullptr)
        , m_pShaderManager(nullptr)
        , m_pMaterialDataManager(nullptr)
        , m_pMaterialManager(nullptr)
        
		, m_nNextMovableTypeFlag(1)
		, m_bIsInit(false)
		, m_bFirstInitAfterWndCreated(false)
    {

    }

    RenderEngine::~RenderEngine()
    {
         Destroy();
    }

    void RenderEngine::initRendererCfgItems()
    {
       
    }

    void RenderEngine::Destroy()
    {
        RemoveRenderEngineListenerAll();
		//m_pSceneManagerEnumerator->ClearAllSceneManager();
		F_DELETE(m_pSceneManagerEnumerator)
		F_DELETE(m_pRenderPipeLineManager)
		F_DELETE(m_pMeshManager)
		F_DELETE(m_pMaterialManager)
		F_DELETE(m_pMaterialDataManager)
		DeleteMovableFactoryAll();
		if (m_pRendererCurrent != nullptr)
		{
			m_pRendererCurrent->Destroy();
			m_pRendererCurrent = nullptr;
		}
		m_pRenderWindowAuto = nullptr;
		F_DELETE(m_pRenderQueueManager)
		m_bFirstInitAfterWndCreated = false;
		m_mapRenderer.clear();

        m_bIsInit = false;
    }
    
    bool RenderEngine::Init(bool bAutoCreateWindow)
    {

        return true;
    }

    bool RenderEngine::SetRendererCurrent(Renderer* pRenderer)
	{
		if (m_pRendererCurrent && m_pRendererCurrent != pRenderer)
		{
			m_pRendererCurrent->Destroy();
		}
		m_pRendererCurrent = pRenderer;
		return true;
	}

	Renderer* RenderEngine::GetRendererByName(const String& strName)
	{
		RendererPtrMap::iterator itFind = m_mapRenderer.find(strName);
		if (itFind == m_mapRenderer.end())
		{
			F_LogError("*********************** RenderEngine::GetRendererByName: Can not find renderer by name: [%s] !", strName.c_str());
			return nullptr;
		}

		return itFind->second;
	}

	bool RenderEngine::AddRenderer(Renderer* pRenderer)
	{
		if (pRenderer == nullptr)
			return false;
		const String& strName = pRenderer->GetName();
		RendererPtrMap::iterator itFind = m_mapRenderer.find(strName);
		if (itFind != m_mapRenderer.end())
			return false;
		
		m_mapRenderer[strName] = pRenderer;
		return true;
	}

	void RenderEngine::RemoveRenderer(Renderer* pRenderer)
	{
		if (pRenderer == nullptr)
			return;
		const String& strName = pRenderer->GetName();
		RendererPtrMap::iterator itFind = m_mapRenderer.find(strName);
		if (itFind == m_mapRenderer.end())
			return;
		m_mapRenderer.erase(strName);
	}

	MovableFactory* RenderEngine::GetMovableFactory(const String& strTypeName)
	{
		MovableFactoryPtrMap::iterator itFind = m_mapMovableFactory.find(strTypeName);
		if (itFind != m_mapMovableFactory.end())
			return itFind->second;
	
		F_LogError("*********************** RenderEngine::GetMovableFactory: Movable type: [%s] does not exist !", strTypeName.c_str());
		return nullptr;
	}

	bool RenderEngine::HasMovableFactory(const String& strTypeName) const
	{
		return !(m_mapMovableFactory.find(strTypeName) == m_mapMovableFactory.end());
	}

	void RenderEngine::AddMovableFactory(MovableFactory* pFactory, bool bOverrideExisting /*= false*/)
	{
		const String& strTypeName = pFactory->GetMovableType();
		MovableFactoryPtrMap::iterator itFind = m_mapMovableFactory.find(strTypeName);
		if (!bOverrideExisting && itFind != m_mapMovableFactory.end())
		{
			F_LogError("*********************** RenderEngine::AddMovableFactory: Movable type: [%s] already exist !", strTypeName.c_str());
			return;
		}

		if (pFactory->RequestTypeFlags())
		{
			if (itFind != m_mapMovableFactory.end() && itFind->second->RequestTypeFlags())
			{
				pFactory->SetTypeFlags(itFind->second->GetTypeFlags());
			}
			else
			{
				pFactory->SetTypeFlags(allocateNextMovableTypeFlag());
			}
		}
		m_mapMovableFactory[strTypeName] = pFactory;
		F_LogInfo("RenderEngine::AddMovableFactory: Movable type: [%s] registered !", strTypeName.c_str());
	}

	void RenderEngine::DeleteMovableFactory(const String& strTypeName)
	{
		MovableFactoryPtrMap::iterator itFind = m_mapMovableFactory.find(strTypeName);
		if (itFind != m_mapMovableFactory.end())
		{
			MovableFactory* pFactory = itFind->second;
			if (!pFactory->GetIsDelExternal())
				delete itFind->second;
			m_mapMovableFactory.erase(itFind);
			F_LogInfo("RenderEngine::DeleteMovableFactory: Movable type: [%s] unregistered !", strTypeName.c_str());
		}
	}

	void RenderEngine::DeleteMovableFactoryAll()
	{
		MovableFactoryPtrMap::iterator it,itEnd;
		itEnd = m_mapMovableFactory.end();
		for (it = m_mapMovableFactory.begin(); it != itEnd; ++it)
		{
			MovableFactory* pFactory = it->second;
			F_LogInfo("RenderEngine::DeleteMovableFactoryAll: Movable type: [%s] unregistered !", pFactory->GetMovableType().c_str());
			if (!pFactory->GetIsDelExternal())
				delete it->second;
		}
		m_mapMovableFactory.clear();
	}

	void RenderEngine::AddRenderEngineListener(RenderEngineListener* pListener)
	{
		m_aRenderEngineListener.push_back(pListener);
	}

	void RenderEngine::RemoveRenderEngineListener(RenderEngineListener* pListener)
	{
		RenderEngineListenerPtrVector::iterator itFind = std::find(m_aRenderEngineListener.begin(), m_aRenderEngineListener.end(), pListener);
		if (itFind != m_aRenderEngineListener.end())
			m_aRenderEngineListener.erase(itFind);
	}

	void RenderEngine::RemoveRenderEngineListenerAll()
	{
		m_aRenderEngineListener.clear();
	}

	RenderWindow* RenderEngine::CreateRenderWindow(const String& strName, uint32 nWidth, uint32 nHeight, bool bFullScreen, const String2StringMap* pParams /*= 0*/)
	{
		if (!m_pRendererCurrent)
		{
			F_LogError("*********************** RenderEngine::CreateRenderWindow: No renderer !");
			return nullptr;
		}
		RenderWindow* pRet = m_pRendererCurrent->CreateRenderWindow(strName, nWidth, nHeight, bFullScreen, pParams);
		if (pRet && !m_bFirstInitAfterWndCreated)
		{
			firstInitAfterWndCreated();
			pRet->SetPrimary();
		}

		return pRet;
	}

	void RenderEngine::DestroyRenderWindow(RenderWindow* pRenderWindow)
	{
		if (!m_pRendererCurrent)
			return;
		m_pRendererCurrent->DestroyRenderWindow(pRenderWindow->GetName());
	}

	void RenderEngine::DestroyRenderWindow(const String& strName)
	{
		if (!m_pRendererCurrent)
			return;
		m_pRendererCurrent->DestroyRenderWindow(strName);
	}

	void RenderEngine::TickOneFrame(RenderFrameEvent& event)
	{
		RenderWindow* pRenderWindow = m_pRendererCurrent->GetRenderWindow();
		if (pRenderWindow && !pRenderWindow->IsDeviceLost())
		{

		}
	}

	bool RenderEngine::RenderOneFrame(RenderFrameEvent& event)
	{
		RenderEngineListenerPtrVector::iterator it,itEnd;
		//1> Pre RenderOneFrame
		itEnd = m_aRenderEngineListener.end();
		for (it = m_aRenderEngineListener.begin(); it != itEnd; ++it)
		{
			if(!(*it)->PreRenderOneFrame())
				return false;
		}
        {
            //2> RenderOneFrame
            if (!m_pRenderPipeLineManager->RenderOneFrame(event))
            {
                F_LogError("*********************** RenderEngine::RenderOneFrame: Pipeline render one frame failed !");
                return false;
            }
        }
		//3> Post RenderOneFrame
		itEnd = m_aRenderEngineListener.end();
		for (it = m_aRenderEngineListener.begin(); it != itEnd; ++it)
		{
			if(!(*it)->PostRenderOneFrame())
				return false;
		}

		return true;
	}

    void RenderEngine::RenderScene(SceneManager* pSceneManager, Viewport* pViewport)
    {
        if (!pSceneManager || !pViewport)
            return;

        RenderTarget* pRenderTarget = pViewport->GetRenderTarget();
        if (pRenderTarget)
        {
            
        }
    }

    void RenderEngine::firstInitAfterWndCreated()
	{	
		if (!m_bFirstInitAfterWndCreated)
		{
			//m_pMaterialManager->InitDefaultMaterials();

			m_bFirstInitAfterWndCreated = true;
		}
	}

	uint32 RenderEngine::allocateNextMovableTypeFlag()
	{
		// if (m_nNextMovableTypeFlag == SceneManager::ms_nUserTypeMask)
		// {
		// 	F_LogError("*********************** RenderEngine::allocateNextMovableTypeFlag: All the user flag have been used !");
		// 	return 0;
		// }
		uint32 nRet = m_nNextMovableTypeFlag;
		m_nNextMovableTypeFlag <<= 1;
		return nRet;
	}

}; //LostPeterEngine