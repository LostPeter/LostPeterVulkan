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
#include "../include/RenderEngine.h"
#include "../include/RenderWindow.h"

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

        , m_cfg_bIsWindowSceneUsed(true)
        , m_cfg_bIsWindowGameUsed(false)
        

        , m_pWindow_Main(nullptr)
        , m_pRenderWindow_Scene(nullptr)
        , m_pRenderWindow_Game(nullptr)
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

            return true;
        }
            Window* Sample::createWindow(const String& nameWindow)
            {
                Window* pWindow = new Window();
                if (!pWindow->Init(nameWindow, m_nWidth, m_nHeight))
                {
                    F_LogError("*********************** Sample::createWindow: pWindow->Init failed !");
                    F_DELETE(pWindow)
                    return nullptr;
                }
                m_aWindows.push_back(pWindow);
                m_mapWindows[nameWindow] = pWindow;
                return pWindow;
            }
            
    bool Sample::OnInit()
    {
        return init();
    }
        bool Sample::init()
        {
            //1> createEngine
            if (!createEngine())
            {
                return false;
            }

            //2> createRenderWindows
            if (!createRenderWindows())
            {
                return false;
            }

            return true;
        }
            bool Sample::createEngine()
            {
                m_pEngine = new Engine();
                if (!m_pEngine->Init(m_strPathBin, false))
                {
                    F_LogError("*********************** Sample::createEngine: init engine failed, path bin: [%s] !", m_strPathBin.c_str());
                    return false;
                }
                m_pTimer = m_pEngine->GetTimer();
                m_fTimeLastFPS = m_pTimer->GetTimeSinceStart();

                return true;
            }
            bool Sample::createRenderWindows()
            {
                //Scene
                if (m_cfg_bIsWindowSceneUsed)
                {
                    String nameScene = m_strNameSample + " - " + E_GetWindowTypeName(E_Window_Scene);
                    m_pRenderWindow_Scene = createRenderWindow(nameScene, 1080, 720);
                    if (m_pRenderWindow_Scene == nullptr)
                    {
                        F_LogError("*********************** Sample::createRenderWindows: Create render window scene failed !");
                        return false;
                    }
                    F_LogInfo("Sample::createRenderWindows: Create render window scene success !");
                }

                //Game
                if (m_cfg_bIsWindowGameUsed)
                {
                    String nameGame = m_strNameSample + " - " + E_GetWindowTypeName(E_Window_Game);
                    m_pRenderWindow_Game = createRenderWindow(nameGame, 1080, 720);
                    if (m_pRenderWindow_Game == nullptr)
                    {
                        F_LogError("*********************** Sample::createRenderWindows: Create render window game failed !");
                        return false;
                    }
                    F_LogInfo("Sample::createRenderWindows: Create render window game success !");
                }

                return true;
            }
                RenderWindow* Sample::createRenderWindow(const String& nameWindow, int nWidth, int nHeight)
                {
                    RenderWindow* pRenderWindow = RenderEngine::GetSingleton().CreateRenderWindow(nameWindow, nWidth, nHeight);
                    if (pRenderWindow == nullptr)
                    {
                        F_LogError("*********************** Sample::createRenderWindow: Create render window failed, name: [%s], w-h: [%d]-[%d] ", nameWindow.c_str(), nWidth, nHeight);
                        return nullptr;
                    }

                    m_aWindows.push_back(pRenderWindow);
                    m_mapWindows[nameWindow] = pRenderWindow;
                    return pRenderWindow;
                }

    bool Sample::OnRunning()
    {
        if (OnIsWindowMainClosed())
            return false;
        
        UpdateTimer();
        int count = (int)m_aWindows.size();
        for (int i = 0; i < count; i++)
        {
            WindowBase* pWindowBase = m_aWindows[i];
            if (pWindowBase && pWindowBase->IsWindowShow())
            {
                pWindowBase->OnMouseInput();
                pWindowBase->OnKeyboardInput();
            }
        }

        return true;
    }

    void Sample::OnDestroy()
    {
        destroy();
    }
        void Sample::destroy()
        {
            destroyRenderWindows();
            destroyEngine();

            destroyWindows();
        }
            void Sample::destroyRenderWindows()
            {
                destroyRenderWindow(m_pRenderWindow_Scene);
                m_pRenderWindow_Scene = nullptr;
                destroyRenderWindow(m_pRenderWindow_Game);
                m_pRenderWindow_Game = nullptr;
            }
                void Sample::destroyRenderWindow(RenderWindow* pRenderWindow)
                {
                    if (pRenderWindow != nullptr)
                    {
                        RenderEngine::GetSingleton().DestroyRenderWindow(pRenderWindow);
                    }   
                }
            void Sample::destroyEngine()
            {
                F_DELETE(m_pEngine)
            }

            void Sample::destroyWindows()
            {
                F_DELETE(m_pWindow_Main)
                m_aWindows.clear();
                m_mapWindows.clear();
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

    bool Sample::OnIsWindowMainClosed()
    {
        if (glfwWindowShouldClose(m_pWindow_Main->GetGLFWwindow()))
        {
            return true;
        }
        return false;
    }

}; //LostPeterEngine