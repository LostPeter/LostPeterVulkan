#include "../include/PreInclude.h"
#include "../include/PreInclude.h"
#include "../include/App.h"
#include "../include/VulkanBase.h"

namespace LostPeter
{
    GLFWwindow* App::s_pWindow = nullptr;
    VulkanBase* App::s_pBase = nullptr;

    int App::Run(VulkanBase* pBase, int vulkanVersionMajor, int vulkanVersionMinor)
    {
        s_pBase = pBase;

        //1> Parse the command line parameters


        //2> glfw initialize and configure 
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {

            Util_LogError("App::Run: glfwInit failed !");
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //3> glfw window creation
        s_pWindow = glfwCreateWindow(pBase->width, pBase->height, pBase->GetTitle().c_str(), NULL, NULL);
        if (!s_pWindow) {
            Util_LogError("App::Run: glfwCreateWindow failed !");
            glfwTerminate();
            return -1;
        }
        pBase->pWindow = s_pWindow;

        glfwSetKeyCallback(s_pWindow, key_callback);
        glfwSetFramebufferSizeCallback(s_pWindow, framebuffer_size_callback);
        glfwSetMouseButtonCallback(s_pWindow, mouse_button_callback);
        glfwSetCursorPosCallback(s_pWindow, cursor_position_callback);
        glfwSetScrollCallback(s_pWindow, scroll_callback);

        //4> OnInit
        pBase->OnInit();

        //5> OnResize
        pBase->OnResize(pBase->width, pBase->height, true);

        //6> OnLoad
        pBase->OnLoad();

        //7> OnIsInit
        bool isInit = pBase->OnIsInit();

        //8> Main loop
        while (!glfwWindowShouldClose(s_pWindow)) 
        {
            //0) timer
            pBase->UpdateTimer();
            
            //1) input
            pBase->OnMouseInput();
            pBase->OnKeyboardInput();

            //2) poll IO events (keys pressed/released, mouse moved etc)
            glfwPollEvents();

            //3) render
            if (isInit)
            {
                if (!pBase->isAppPaused)
                {
                    pBase->CalculateFrameStats(s_pWindow);

                    if (pBase->OnBeginRender())
                    {
                        pBase->OnUpdate();
                        pBase->OnRender();
                        pBase->OnEndRender();
                    }

                    if (pBase->isMinimizedWindowNeedClose) 
                    {
                        break;
                    }
                }  
            }
        }

        //9> OnDestroy
        pBase->OnDestroy();

        //10> Cleanup
        glfwDestroyWindow(s_pWindow);
        glfwTerminate();

        return 0;
    }

    void App::error_callback(int error, const char* description)
    {
        Util_LogError("App::error_callback: [%s]", description);
    }
    void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS)
        {
            s_pBase->OnKeyDown(key);
        }
        else if (action == GLFW_RELEASE)
        {
            s_pBase->OnKeyUp(key);
        }
    }
    void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        s_pBase->OnResize(width, height, true);
    }
    void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
    {
        
    }
    void App::cursor_position_callback(GLFWwindow* window, double x, double y)
    {
        
    }
    void App::scroll_callback(GLFWwindow* window, double x, double y)
    {
        s_pBase->OnMouseWheel(x, y);
    }

}; //LostPeter