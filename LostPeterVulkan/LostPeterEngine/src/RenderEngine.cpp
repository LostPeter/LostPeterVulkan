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
#include "../include/ShaderProgramManager.h"
#include "../include/TextureManager.h"
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
        , m_pMaterialDataManager(nullptr)
        , m_pMaterialManager(nullptr)
        
		, m_nNextMovableTypeFlag(1)
		, m_bIsInit(false)
    {
		initRendererCfgItems();
    }

    RenderEngine::~RenderEngine()
    {
        Destroy();
    }

    void RenderEngine::initRendererCfgItems()
    {
		m_mapRendererCfgItem.clear();

		//0> RendererName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_RendererName);
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererNull"));	//0
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererVulkan"));	//1
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererD3D12"));	//2
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererMetal"));	//3
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererGL33"));	//4
			cfgItem.aPossibleValues.push_back(String("LostPeterPluginRendererGLES3"));	//5
			cfgItem.strCurValue = cfgItem.aPossibleValues[1];
			cfgItem.bImmutable = false;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}

		//1> PipelineName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_PipelineName);
			cfgItem.aPossibleValues.push_back(F_GetRenderPipelineTypeName(F_RenderPipeline_Forward));	//0
			cfgItem.aPossibleValues.push_back(F_GetRenderPipelineTypeName(F_RenderPipeline_Deferred));	//1
			cfgItem.strCurValue = cfgItem.aPossibleValues[0];
			cfgItem.bImmutable = false;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}

		//2> ShaderGroup
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_ShaderGroup);
			cfgItem.strCurValue = FUtilString::SaveUInt(0);
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}
		//3> ShaderConfigName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_ShaderConfigName);
			cfgItem.strCurValue = ShaderProgramManager::ms_strShaderProgramConfigName;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}

		//4> MeshGroup
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_MeshGroup);
			cfgItem.strCurValue = FUtilString::SaveUInt(0);
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}
		//5> MeshConfigName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_MeshConfigName);
			cfgItem.strCurValue = MeshManager::ms_strMeshConfigName;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}

		//6> TextureGroup
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_TextureGroup);
			cfgItem.strCurValue = FUtilString::SaveUInt(0);
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}
		//7> TextureConfigName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_TextureConfigName);
			cfgItem.strCurValue = TextureManager::ms_strTextureConfigName;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}

		//8> MaterialGroup
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_MaterialGroup);
			cfgItem.strCurValue = FUtilString::SaveUInt(0);
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}
		//9> MaterialConfigName
		{
			ConfigItem cfgItem;
			cfgItem.strName	= E_GetEngineConfigTypeName(E_EngineConfig_Render_MaterialConfigName);
			cfgItem.strCurValue = MaterialManager::ms_strMaterialConfigName;
			m_mapRendererCfgItem[cfgItem.strName] = cfgItem;
		}
    }

    void RenderEngine::Destroy()
    {
        RemoveRenderEngineListenerAll();
		//m_pSceneManagerEnumerator->ClearAllSceneManager();
		F_DELETE(m_pSceneManagerEnumerator)
		F_DELETE(m_pRenderPipeLineManager)
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
		m_mapRenderer.clear();

        m_bIsInit = false;
    }
    
    bool RenderEngine::Init(bool bAutoCreateWindow)
    {
		//1> Find current renderer
		{
			const String& nameItem = E_GetEngineConfigTypeName(E_EngineConfig_Render_RendererName);
			ConfigItemMap::iterator itFind = m_mapRendererCfgItem.find(nameItem);
			if (itFind == m_mapRendererCfgItem.end())
			{
				F_LogError("*********************** RenderEngine::Init: 1> Can not find renderer config item: [%s] !", nameItem.c_str());
				return false;
			}
			String strRendererName(itFind->second.strCurValue);
			m_pRendererCurrent = GetRendererByName(strRendererName);
			if (!m_pRendererCurrent)
			{
				F_LogError("*********************** RenderEngine::Init: 1> Can not find renderer: [%s] !", strRendererName.c_str());
				return false;
			}
			F_LogInfo("RenderEngine::Init: 1> Get renderer: [%s] success !", strRendererName.c_str());
		}

		//2> Set renderer config items
		{

		}

		//3> Init renderer and create main window
		{
			m_pRenderWindowAuto = m_pRendererCurrent->Init(bAutoCreateWindow);
		}

		//4> Init managers
		{
			if (!initRenderPipelineManager())
			{
				F_LogError("*********************** RenderEngine::Init: 4> Init RenderPipelineManager failed !");
				return false;
			}
			if (!initSceneManager())
			{
				F_LogError("*********************** RenderEngine::Init: 4> Init SceneManager failed !");
				return false;
			}
			if (!initRenderQueueManager())
			{
				F_LogError("*********************** RenderEngine::Init: 4> Init RenderQueueManager failed !");
				return false;
			}
			if (!initMaterialManager())
			{
				F_LogError("*********************** RenderEngine::Init: 4> Init MaterialManager failed !");
				return false;
			}
		}

        return true;
    }
		bool RenderEngine::initRenderPipelineManager()
		{
			m_pRenderPipeLineManager = new RenderPipelineManager();
			const String& nameItem = E_GetEngineConfigTypeName(E_EngineConfig_Render_PipelineName);
			ConfigItemMap::iterator itFind = m_mapRendererCfgItem.find(nameItem);
			if (itFind == m_mapRendererCfgItem.end())
			{
				F_LogError("*********************** RenderEngine::initRenderPipelineManager: Can not find renderer config item: [%s] !", nameItem.c_str());
				return false;
			}
			const String& strNamePipeline = itFind->second.strCurValue;
			if (!m_pRenderPipeLineManager->Init(strNamePipeline))
			{
				F_LogError("*********************** RenderEngine::initRenderPipelineManager: Failed to init pipeline manager, pipeline Name: [%s] !", strNamePipeline.c_str());
            	return false;
			}
			return true;
		}
		bool RenderEngine::initSceneManager()
		{
			m_pSceneManagerEnumerator = new SceneManagerEnumerator();
			return true;
		}
		bool RenderEngine::initRenderQueueManager()
		{
			m_pRenderQueueManager = new RenderQueueManager();
			return true;
		}
		bool RenderEngine::initMaterialManager()
		{
			m_pMaterialDataManager = new MaterialDataManager();
			m_pMaterialManager = new MaterialManager();
			uint32 nGroup = GetCfgGroup(E_EngineConfig_Render_MaterialGroup);
        	const String& strNameCfg = GetCfgConfigName(E_EngineConfig_Render_MaterialConfigName);
			if (!m_pMaterialManager->Init(nGroup, strNameCfg))
			{
				F_LogError("*********************** RenderEngine::initMaterialManager: Failed to init material manager, group: [%u], configName: [%s] !", nGroup, strNameCfg.c_str());
            	return false;
			}
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

	uint32 RenderEngine::GetCfgGroup(EEngineConfigType typeEngineConfig) const
	{
		if (typeEngineConfig != E_EngineConfig_Render_ShaderGroup &&
			typeEngineConfig != E_EngineConfig_Render_MeshGroup &&
			typeEngineConfig != E_EngineConfig_Render_TextureGroup &&
			typeEngineConfig != E_EngineConfig_Render_MaterialGroup)
		{
			return 0;
		}

		const String& strKey = E_GetEngineConfigTypeName(typeEngineConfig);
		ConfigItemMap::const_iterator itFind = m_mapRendererCfgItem.find(strKey);
		F_Assert(itFind != m_mapRendererCfgItem.end() && "RenderEngine::GetCfgGroup")
		const String& strValue = itFind->second.strCurValue;
		return FUtilString::ParserUInt(strValue);
	}
	const String& RenderEngine::GetCfgConfigName(EEngineConfigType typeEngineConfig) const
	{
		if (typeEngineConfig != E_EngineConfig_Render_ShaderConfigName &&
			typeEngineConfig != E_EngineConfig_Render_MeshConfigName &&
			typeEngineConfig != E_EngineConfig_Render_TextureConfigName &&
			typeEngineConfig != E_EngineConfig_Render_MaterialConfigName)
		{
			return FUtilString::BLANK;
		}

		const String& strKey = E_GetEngineConfigTypeName(typeEngineConfig);
		ConfigItemMap::const_iterator itFind = m_mapRendererCfgItem.find(strKey);
		F_Assert(itFind != m_mapRendererCfgItem.end() && "RenderEngine::GetCfgConfigName")
		return itFind->second.strCurValue;
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

	RenderWindow* RenderEngine::CreateRenderWindow(const String& strName, 
												   int32 nWidth, 
												   int32 nHeight, 
												   const String2StringMap* pParams /*= 0*/)
	{
		if (!m_pRendererCurrent)
		{
			F_LogError("*********************** RenderEngine::CreateRenderWindow: No renderer !");
			return nullptr;
		}
		RenderWindow* pRenderWindow = m_pRendererCurrent->CreateRenderWindow(strName, 
																			 nWidth, 
																			 nHeight, 
																			 pParams);
		if (!pRenderWindow)
		{
			F_LogError("*********************** RenderEngine::CreateRenderWindow: Create render window failed !");
			return nullptr;
		}
		
		return pRenderWindow;
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
		RenderWindow* pRenderWindow = m_pRendererCurrent->GetActiveRenderWindow();
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