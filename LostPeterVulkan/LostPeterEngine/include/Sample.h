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

        bool m_bIsWindowGameUsed;
        bool m_bIsWindowSceneUsed;
        
    protected:
        WindowPtrVector m_aWindows;
        WindowPtrMap m_mapWindows;
        Window* m_pWindow_Main;
        Window* m_pWindow_Game;
        Window* m_pWindow_Scene;

    public:
        E_FORCEINLINE const String& GetNameSample() const { return m_strNameSample; }
        E_FORCEINLINE const String& GetPathBin() const { return m_strPathBin; }
        E_FORCEINLINE int GetWidth() const { return m_nWidth; }
        E_FORCEINLINE int GetHeight() const { return m_nHeight; }
        E_FORCEINLINE String GetAssetFullPath(const String& strAssetName) { return m_strPathBin + strAssetName; }

        E_FORCEINLINE Window* GetWindowMain() const { return m_pWindow_Main; }
        E_FORCEINLINE Window* GetWindowGame() const { return m_pWindow_Game; }
        E_FORCEINLINE Window* GetWindowScene() const { return m_pWindow_Scene; }

    public:
        float RefreshAspectRatio();
        void UpdateTimer();

    public:
        virtual bool OnCreateWindow();
        virtual bool OnInit();
        virtual void OnDestroy();

        virtual void OnLoad();
        virtual void OnResize(int w, int h, bool force);
        virtual bool OnIsInit();
        virtual bool OnIsWindowsClosed();
        
    protected:
        virtual bool createWindows();
            virtual Window* createWindow(const String& nameTitle);

        virtual bool init();
            virtual bool initEngine();

        virtual void destroy();
            virtual void destroyWindows();
            virtual void destroyEngine();
    };

}; //LostPeterEngine

#endif