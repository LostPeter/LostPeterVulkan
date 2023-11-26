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

#ifndef _RENDER_ENGINE_H_
#define _RENDER_ENGINE_H_

#include "RenderEngineListener.h"

namespace LostPeterEngine
{
    class utilExport RenderEngine : public FSingleton<RenderEngine>
                                  , public Base
    {
    public:
        RenderEngine();
        virtual ~RenderEngine();

    public:
    protected:
        ConfigItemMap m_mapRendererCfgItem;

		RendererPtrMap m_mapRenderer;
		Renderer* m_pRendererCurrent;
        RenderWindow* m_pRenderWindowAuto;
		RenderPipelineManager* m_pRenderPipeLineManager;
        SceneManagerEnumerator* m_pSceneManagerEnumerator;
        SceneManager* m_pSceneManagerCurrent;
        RenderQueueManager* m_pRenderQueueManager;
        MeshManager* m_pMeshManager;
        TextureManager* m_pTextureManager;
        ShaderManager* m_pShaderManager;
        
        MaterialDataManager* m_pMaterialDataManager;
        MaterialManager* m_pMaterialManager;

        MovableFactoryPtrMap m_mapMovableFactory;
		uint32 m_nNextMovableTypeFlag;
		
		bool m_bIsInit;
		bool m_bFirstInitAfterWndCreated;

		RenderEngineListenerPtrVector m_aRenderEngineListener;
        


    public:
        static RenderEngine& GetSingleton();
		static RenderEngine* GetSingletonPtr();

    public:
		LP_FORCEINLINE ConfigItemMap* GetRendererCfgItems() { return &m_mapRendererCfgItem; }

		LP_FORCEINLINE RendererPtrMap&	GetRendererPtrMap() { return m_mapRenderer; }
		LP_FORCEINLINE Renderer* GetRendererCurrent() const { return m_pRendererCurrent; }
		bool SetRendererCurrent(Renderer* pRenderer);
		LP_FORCEINLINE RenderWindow* GetRenderWindowAuto() const { return m_pRenderWindowAuto; }
		LP_FORCEINLINE RenderPipelineManager* GetRenderPipeLineManager() const { return m_pRenderPipeLineManager; }
		LP_FORCEINLINE SceneManager* GetSceneManagerCurrent() const { return m_pSceneManagerCurrent; }
		LP_FORCEINLINE bool	IsInit() const { return m_bIsInit; }

		Renderer* GetRendererByName(const String& strName);
		bool AddRenderer(Renderer* pRenderer);
		void RemoveRenderer(Renderer* pRenderer);
		
		LP_FORCEINLINE const MovableFactoryPtrMap& GetMovableFactoryPtrMap() const { return m_mapMovableFactory; }
		MovableFactory* GetMovableFactory(const String& strTypeName);
		bool HasMovableFactory(const String& strTypeName) const;
		void AddMovableFactory(MovableFactory* pFactory, bool bOverrideExisting = false);
		void DeleteMovableFactory(const String& strTypeName);
		void DeleteMovableFactoryAll();

		void AddRenderEngineListener(RenderEngineListener* pListener);
		void RemoveRenderEngineListener(RenderEngineListener* pListener);
		void RemoveRenderEngineListenerAll();

    public:
        void Destroy();
        virtual bool Init();

		RenderWindow* CreateRenderWindow(const String& strName, uint32 nWidth, uint32 nHeight, bool bFullScreen, const String2StringMap* pParams = nullptr);
		void DestroyRenderWindow(RenderWindow* pRenderWindow);
		void DestroyRenderWindow(const String& strName);

		void TickOneFrame(RenderFrameEvent& event);
		bool RenderOneFrame(RenderFrameEvent& event);
        
        void RenderScene(SceneManager* pSceneManager, Viewport* pViewport);

    private:
		void initRendererCfgItems();
		void firstInitAfterWndCreated();

		uint32 allocateNextMovableTypeFlag();
    };

}; //LostPeterEngine

#endif