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

#include "../include/PreInclude.h"
#include "../include/Sample.h"
#include "../include/Window.h"
#include "../include/Engine.h"

namespace LostPeterEngine
{
    FLogManager* Sample::ms_pLogManager = FLogManager::GetInstance();
    Sample::Sample(const String& nameSample, int nWidth, int nHeight)
        : m_strNameSample(nameSample)
        , m_nWidth(nWidth)
        , m_nHeight(nHeight)
        , m_fAspectRatio(1.0f)
        
        , m_pEngine(nullptr)
        , m_pTimer(nullptr)
        , m_fFPS(0.0f)
        , m_nFrameFPS(0)
        , m_nFrameTotal(0)

        , m_bIsWindowGameUsed(true)
        , m_bIsWindowSceneUsed(false)

        , m_pWindow_Main(nullptr)
        , m_pWindow_Game(nullptr)
        , m_pWindow_Scene(nullptr)
    {
        m_strPathBin = FUtil::GetPathBin() + "/";
        F_LogInfo("Path Bin: [%s] !", m_strPathBin.c_str());

        RefreshAspectRatio();
    }

    Sample::~Sample()
    {
        destroy();
    }

    float Sample::RefreshAspectRatio()
    {
        m_fAspectRatio = static_cast<float>(m_nWidth) / static_cast<float>(m_nHeight);
        return m_fAspectRatio;
    }

    void Sample::UpdateTimer()
    {
        if (m_pTimer == nullptr)
            return;
        m_pTimer->Tick();
        float timeSinceStart = m_pTimer->GetTimeSinceStart();
        ++ m_nFrameFPS;
        if (timeSinceStart >= m_fTimeLastFPS + 1.0f)
        {
            m_fFPS = m_nFrameFPS / (timeSinceStart - m_fTimeLastFPS);
            m_nFrameFPS = 0;
            m_fTimeLastFPS = timeSinceStart;
        }
    }


    bool Sample::OnCreateWindow()
    {
        return createWindows();
    }
        bool Sample::createWindows()
        {
            //0> Window Main
            {
                String nameMain = m_strNameSample + " - " + E_GetWindowTypeName(E_Window_Main);
                m_pWindow_Main = createWindow(nameMain);
                if (m_pWindow_Main == nullptr)
                {
                    F_LogError("*********************** Sample::createWindows: Create window main failed !");
                    return false;
                }
                F_LogInfo("Sample::createWindows: Create window main success !");
            }
            //1> Window Game
            if (m_bIsWindowGameUsed)
            {
                String nameGame = m_strNameSample + " - " + E_GetWindowTypeName(E_Window_Game);
                m_pWindow_Game = createWindow(nameGame);
                if (m_pWindow_Game == nullptr)
                {
                    F_LogError("*********************** Sample::createWindows: Create window game failed !");
                    return false;
                }
                F_LogInfo("Sample::createWindows: Create window game success !");
            }
            //2> Window Scene
            if (m_bIsWindowSceneUsed)
            {
                String nameScene = m_strNameSample + " - " + E_GetWindowTypeName(E_Window_Scene);
                m_pWindow_Scene = createWindow(nameScene);
                if (m_pWindow_Scene == nullptr)
                {
                    F_LogError("*********************** Sample::createWindows: Create window scene failed !");
                    return false;
                }
                F_LogInfo("Sample::createWindows: Create window scene success !");
            }

            return true;
        }
            Window* Sample::createWindow(const String& nameTitle)
            {
                Window* pWindow = new Window();
                if (!pWindow->Init(nameTitle, m_nWidth, m_nHeight))
                {
                    F_LogError("*********************** Sample::createWindow: pWindow->Init failed !");
                    F_DELETE(pWindow)
                    return nullptr;
                }
                m_aWindows.push_back(pWindow);
                m_mapWindows[nameTitle] = pWindow;
                return pWindow;
            }
            
    bool Sample::OnInit()
    {
        return init();
    }
        bool Sample::init()
        {
            //1> initEngine
            if (!initEngine())
            {
                return false;
            }

            return true;
        }
            bool Sample::initEngine()
            {
                m_pEngine = new Engine();
                if (!m_pEngine->Init(m_strPathBin, false))
                {
                    F_LogError("*********************** Sample::initEngine: init engine failed, path bin: [%s] !", m_strPathBin.c_str());
                    return false;
                }
                m_pTimer = m_pEngine->GetTimer();
                m_fTimeLastFPS = m_pTimer->GetTimeSinceStart();

                return true;
            }

    void Sample::OnDestroy()
    {
        destroy();
    }
        void Sample::destroy()
        {
            destroyWindows();
            destroyEngine();
        }
            void Sample::destroyWindows()
            {
                size_t count = m_aWindows.size();
                for (size_t i = 0; i < count; i++)
                {
                    Window* pWindow = m_aWindows[i];
                    F_DELETE(pWindow)
                }
                m_aWindows.clear();
                m_mapWindows.clear();
            }
            void Sample::destroyEngine()
            {
                F_DELETE(m_pEngine)
            }

    void Sample::OnLoad()
    {

    }

    void Sample::OnResize(int w, int h, bool force)
    {

    }

    bool Sample::OnIsInit()
    {
        
        return false;
    }

    bool Sample::OnIsWindowsClosed()
    {
        size_t count = m_aWindows.size();
        for (size_t i = 0; i < count; i++)
        {
            Window* pWindow = m_aWindows[i];
            if (!glfwWindowShouldClose(pWindow->GetGLFWwindow()))
            {
                return false;
            }
        }
        return true;
    }   

}; //LostPeterEngine