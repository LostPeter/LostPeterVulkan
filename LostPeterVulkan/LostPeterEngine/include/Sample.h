/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport Sample
    {
    public:
        Sample(const String& nameSample, int nWidth, int nHeight);
        virtual ~Sample();

    public:
        static FLogManager* ms_pLogManager;

    protected:
        String m_strNameSample;
        String m_strPathBin; 
        int m_nWidth;
        int m_nHeight;
        float m_fAspectRatio;

        Engine* m_pEngine;
        FTimer* m_pTimer;
        float m_fTimeLastFPS;
        float m_fFPS;
        int m_nFrameFPS;
        uint64 m_nFrameTotal;
        
    protected:
        WindowBasePtrVector m_aWindows;
        WindowBasePtrMap m_mapWindows;
        Window* m_pWindow_Main;
        RenderWindow* m_pRenderWindow_Scene;
        RenderWindow* m_pRenderWindow_Game;

    protected:
        bool m_cfg_bIsWindowSceneUsed;
        bool m_cfg_bIsWindowGameUsed;

    public:
        F_FORCEINLINE const String& GetNameSample() const { return m_strNameSample; }
        F_FORCEINLINE const String& GetPathBin() const { return m_strPathBin; }
        F_FORCEINLINE int GetWidth() const { return m_nWidth; }
        F_FORCEINLINE int GetHeight() const { return m_nHeight; }
        F_FORCEINLINE String GetAssetFullPath(const String& strAssetName) { return m_strPathBin + strAssetName; }

        F_FORCEINLINE Window* GetWindowMain() const { return m_pWindow_Main; }
        F_FORCEINLINE RenderWindow* GetRenderWindowScene() const { return m_pRenderWindow_Scene; }
        F_FORCEINLINE RenderWindow* GetRenderWindowGame() const { return m_pRenderWindow_Game; }

    public:
        F_FORCEINLINE bool GetCfg_IsWindowSceneUsed() const { return m_cfg_bIsWindowSceneUsed; }
        F_FORCEINLINE void SetCfg_IsWindowSceneUsed(bool bIsWindowSceneUsed) { m_cfg_bIsWindowSceneUsed = bIsWindowSceneUsed; }
        F_FORCEINLINE bool GetCfg_IsWindowGameUsed() const { return m_cfg_bIsWindowGameUsed; }
        F_FORCEINLINE void SetCfg_IsWindowGameUsed(bool bIsWindowGameUsed) { m_cfg_bIsWindowGameUsed = bIsWindowGameUsed; }

    public:
        float RefreshAspectRatio();
        void UpdateTimer();

    public:
        virtual bool OnCreateWindow();
        virtual bool OnInit();
        virtual bool OnRunning();
        virtual void OnDestroy();

        virtual void OnLoad();
        virtual void OnResize(int w, int h, bool force);
        virtual bool OnIsInit();
        virtual bool OnIsWindowMainClosed();

    protected:
        virtual bool createWindows();
            virtual Window* createWindow(const String& nameTitle);

        virtual bool init();
            virtual bool createEngine();
            virtual bool createRenderWindows();
                virtual RenderWindow* createRenderWindow(const String& nameTitle, int nWidth, int nHeight);

        virtual void destroy();
            virtual void destroyRenderWindows();
                virtual void destroyRenderWindow(RenderWindow* pRenderWindow);
            virtual void destroyEngine();
                
            virtual void destroyWindows();
            
    };

}; //LostPeterEngine

#endif