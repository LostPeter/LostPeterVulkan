/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "Base.h"
#include "EngineListener.h"

namespace LostPeterEngine
{
    class engineExport Engine : public FSingleton<Engine>
                              , public Base
    {
    public:
        Engine();
        virtual ~Engine();

    public:
        static FPlatformType ms_ePlatform;
        static FPlatformType GetPlatformType() { return ms_ePlatform; }

        static bool	ms_bUseBinaryResource;
        static bool GetUseBinaryResource() { return ms_bUseBinaryResource; }
		static void SetUseBinaryResource(bool b) { ms_bUseBinaryResource = b; }

        static String ms_strPathCfgEngine;
		static String ms_strPathCfgPlugin;

    public:
    protected:
        SystemCapabilities* m_pSystemCapabilities;
        FPathManager* m_pPathManager;
        FFileManager* m_pFileManager;
        FCodecManager* m_pCodecManager;
        FPluginManager* m_pPluginManager;

        RenderEngine* m_pRenderEngine;


    private:
        FTimer* m_pTimer;
        EngineFrameProfiler* m_pEngineFrameProfiler;

        uint64 m_nFrameCurrent;
        uint64 m_nFrameCount;
        uint64 m_nTimeLast;

        bool m_bRenderAutoStop;

    private:
    ////config file path
        String m_strNamePlatform;
        String m_pathWorkFolder;
        String m_folder_Plugin;
		bool m_bEngineIsInit;
        bool m_bCodecIsInit;

        ConfigItemMap m_mapEngineCommonCfgItem;

        EngineListenerPtrMultiSet m_setEngineListeners;
		EngineListenerPtrMultiSet m_setRemovedEngineListeners;

    public:
        static Engine& GetSingleton();
		static Engine* GetSingletonPtr();

    public:
        F_FORCEINLINE SystemCapabilities* GetSystemCapabilities() { return m_pSystemCapabilities; }
        F_FORCEINLINE FPathManager* GetPathManager() { return m_pPathManager; }
        F_FORCEINLINE FFileManager* GetFileManager() { return m_pFileManager; }
        F_FORCEINLINE FCodecManager* GetCodecManager() { return m_pCodecManager; }
        F_FORCEINLINE FPluginManager* GetPluginManager() { return m_pPluginManager; }
        F_FORCEINLINE RenderEngine* GetRenderEngine() { return m_pRenderEngine; }

        F_FORCEINLINE FTimer* GetTimer() const { return m_pTimer; }
        F_FORCEINLINE EngineFrameProfiler* GetEngineFrameProfiler() { return m_pEngineFrameProfiler; }

        F_FORCEINLINE uint64 GetFrameCurrentNumber() const { return m_nFrameCurrent; }
        F_FORCEINLINE uint64 GetTimeLast() const { return m_nTimeLast; }

        F_FORCEINLINE const String& GetNamePlatform() const { return m_strNamePlatform; }
        F_FORCEINLINE const String& GetPathWorkFolder() const { return m_pathWorkFolder; }
        F_FORCEINLINE bool IsEngineInit() const { return m_bEngineIsInit; }
        F_FORCEINLINE ConfigItemMap* GetEngineCommonCfgItems() { return &m_mapEngineCommonCfgItem; }

    public:
        void Destroy();
		bool Init(const String& strWorkFolder, bool bAutoCreateWindow);

        void RenderAuto();
		void RenderStop();

		bool RenderOneFrame();

		bool SaveConfig();
		bool RestoreConfig();

		void AddEngineListener(EngineListener* pEngineListener);
		void RemoveEngineListener(EngineListener* pEngineListener);
		void RemoveEngineListenerAll();

    private:
        void initCommonCfgItems();
		bool loadEngineConfig(const String& strPath);
        
        void clearEventTimes();

        bool fireFrameStarted(RenderFrameEvent& event);
		bool fireFrameRenderingQueued(RenderFrameEvent& event);
		bool fireFrameEnded(RenderFrameEvent& event);

        void updateStats();
		void resetStatistics();
    };

}; //LostPeterEngine

#endif