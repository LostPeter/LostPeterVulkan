/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_SAMPLE_H_
#define _VULKAN_SAMPLE_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanSample
    {
    public:
        VulkanSample(const String& nameSample, int nWidth, int nHeight);
        virtual ~VulkanSample();

    public:
        static VulkanLogManager* ms_pLogManager;

    protected:
        String m_strNameSample;
        String m_strPathBin; 
        int m_nWidth;
        int m_nHeight;
        float m_fAspectRatio;

        VulkanTimer* m_pTimer;
        float m_fTimeLastFPS;
        float m_fFPS;
        int m_nFrameFPS;
        uint64 m_nFrameTotal;

        bool m_bIsWindowGameUsed;
        bool m_bIsWindowSceneUsed;
        
    protected:
        VulkanInstance* m_pInstance;
        VulkanDevice* m_pDevice;

        VulkanWindowExPtrVector m_aWindows;
        VulkanWindowExPtrMap m_mapWindows;
        VulkanWindowEx* m_pWindow_Main;
        VulkanWindowEx* m_pWindow_Game;
        VulkanWindowEx* m_pWindow_Scene;

    public:
        UTIL_FORCEINLINE const String& GetNameSample() const { return m_strNameSample; }
        UTIL_FORCEINLINE const String& GetPathBin() const { return m_strPathBin; }
        UTIL_FORCEINLINE int GetWidth() const { return m_nWidth; }
        UTIL_FORCEINLINE int GetHeight() const { return m_nHeight; }
        UTIL_FORCEINLINE String GetAssetFullPath(const String& strAssetName) { return m_strPathBin + strAssetName; }

        UTIL_FORCEINLINE VulkanInstance* GetInstance() const { return m_pInstance; }
        UTIL_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        UTIL_FORCEINLINE VulkanWindowEx* GetWindowMain() const { return m_pWindow_Main; }
        UTIL_FORCEINLINE VulkanWindowEx* GetWindowGame() const { return m_pWindow_Game; }
        UTIL_FORCEINLINE VulkanWindowEx* GetWindowScene() const { return m_pWindow_Scene; }

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
            virtual VulkanWindowEx* createWindow(const String& nameTitle);

        virtual bool init();
            virtual bool initVulkan();

        virtual void destroy();
            virtual void destroyVulkan();
            virtual void destroyWindows();
    };

}; //LostPeter

#endif