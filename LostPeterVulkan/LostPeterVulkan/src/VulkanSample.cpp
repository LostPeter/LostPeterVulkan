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

#include "../include/PreInclude.h"
#include "../include/VulkanSample.h"
#include "../include/VulkanWindowEx.h"
#include "../include/VulkanInstance.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanTimer.h"
#include "../include/VulkanLogManager.h"

namespace LostPeter
{
    VulkanLogManager* VulkanSample::ms_pLogManager = VulkanLogManager::GetInstance();
    VulkanSample::VulkanSample(const String& nameSample, int nWidth, int nHeight)
        : m_strNameSample(nameSample)
        , m_nWidth(nWidth)
        , m_nHeight(nHeight)
        , m_fAspectRatio(1.0f)
        
        , m_pTimer(new VulkanTimer())
        , m_fFPS(0.0f)
        , m_nFrameFPS(0)
        , m_nFrameTotal(0)

        , m_bIsWindowGameUsed(true)
        , m_bIsWindowSceneUsed(false)
        
        , m_pInstance(nullptr)
        , m_pDevice(nullptr)
        , m_pWindow_Main(nullptr)
        , m_pWindow_Game(nullptr)
        , m_pWindow_Scene(nullptr)
    {
        m_strPathBin = VulkanUtil::GetPathBin() + "/";
        Util_LogInfo("Path Bin: [%s] !", m_strPathBin.c_str());

        RefreshAspectRatio();
        m_fTimeLastFPS = m_pTimer->GetTimeSinceStart();
    }

    VulkanSample::~VulkanSample()
    {
        destroy();
    }

    float VulkanSample::RefreshAspectRatio()
    {
        m_fAspectRatio = static_cast<float>(m_nWidth) / static_cast<float>(m_nHeight);
        return m_fAspectRatio;
    }

    void VulkanSample::UpdateTimer()
    {
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


    bool VulkanSample::OnCreateWindow()
    {
        return createWindows();
    }
        bool VulkanSample::createWindows()
        {
            //0> Window Main
            {
                String nameMain = m_strNameSample + " - " + Util_GetWindowTypeName(Vulkan_Window_Main);
                m_pWindow_Main = createWindow(nameMain);
                if (m_pWindow_Main == nullptr)
                {
                    Util_LogError("*********************** VulkanSample::createWindows: Create window main failed !");
                    return false;
                }
                Util_LogInfo("VulkanSample::createWindows: Create window main success !");
            }
            //1> Window Game
            if (m_bIsWindowGameUsed)
            {
                String nameGame = m_strNameSample + " - " + Util_GetWindowTypeName(Vulkan_Window_Game);
                m_pWindow_Game = createWindow(nameGame);
                if (m_pWindow_Game == nullptr)
                {
                    Util_LogError("*********************** VulkanSample::createWindows: Create window game failed !");
                    return false;
                }
                Util_LogInfo("VulkanSample::createWindows: Create window game success !");
            }
            //2> Window Scene
            if (m_bIsWindowSceneUsed)
            {
                String nameScene = m_strNameSample + " - " + Util_GetWindowTypeName(Vulkan_Window_Scene);
                m_pWindow_Scene = createWindow(nameScene);
                if (m_pWindow_Scene == nullptr)
                {
                    Util_LogError("*********************** VulkanSample::createWindows: Create window scene failed !");
                    return false;
                }
                Util_LogInfo("VulkanSample::createWindows: Create window scene success !");
            }

            return true;
        }
            VulkanWindowEx* VulkanSample::createWindow(const String& nameTitle)
            {
                VulkanWindowEx* pWindow = new VulkanWindowEx();
                if (!pWindow->Init(nameTitle, m_nWidth, m_nHeight))
                {
                    Util_LogError("*********************** VulkanSample::createWindow: pWindow->Init failed !");
                    UTIL_DELETE(pWindow)
                    return nullptr;
                }
                m_aWindows.push_back(pWindow);
                m_mapWindows[nameTitle] = pWindow;
                return pWindow;
            }
            
    bool VulkanSample::OnInit()
    {
        return init();
    }
        bool VulkanSample::init()
        {
            return true;
        }
            bool VulkanSample::initVulkan()
            {
                return true;
            }

    void VulkanSample::OnDestroy()
    {
        destroy();
    }
        void VulkanSample::destroy()
        {
            destroyVulkan();
            destroyWindows();
        }
            void VulkanSample::destroyVulkan()
            {
                m_pDevice = nullptr;
                UTIL_DELETE(m_pInstance)
            }
            void VulkanSample::destroyWindows()
            {
                size_t count = m_aWindows.size();
                for (size_t i = 0; i < count; i++)
                {
                    VulkanWindowEx* pWindow = m_aWindows[i];
                    UTIL_DELETE(pWindow)
                }
                m_aWindows.clear();
                m_mapWindows.clear();
            }

    void VulkanSample::OnLoad()
    {

    }

    void VulkanSample::OnResize(int w, int h, bool force)
    {

    }

    bool VulkanSample::OnIsInit()
    {
        if (m_pDevice != nullptr)
        {
            return true;
        }
        return false;
    }

    bool VulkanSample::OnIsWindowsClosed()
    {
        size_t count = m_aWindows.size();
        for (size_t i = 0; i < count; i++)
        {
            VulkanWindowEx* pWindow = m_aWindows[i];
            if (!glfwWindowShouldClose(pWindow->GetGLFWwindow()))
            {
                return false;
            }
        }
        return true;
    }   

}; //LostPeter