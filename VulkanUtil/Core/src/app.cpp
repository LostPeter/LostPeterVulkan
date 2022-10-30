#include "../include/preinclude.h"
#include "../include/preinclude.h"
#include "../include/app.h"
#include "../include/vulkanbase.h"

namespace LibUtil
{
    GLFWwindow* App::s_pWindow = nullptr;
    VulkanBase* App::s_pBase = nullptr;

    int App::Run(VulkanBase* pBase, int vulkanVersionMajor, int vulkanVersionMinor)
    {
        s_pBase = pBase;

        //1> Parse the command line parameters


        //2> glfw initialize and configure 
        if (!glfwInit()) {
            std::cout << "App::Run: glfwInit failed !" << std::endl;
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //3> glfw window creation
        s_pWindow = glfwCreateWindow(pBase->width, pBase->height, pBase->GetTitle().c_str(), NULL, NULL);
        if (!s_pWindow) {
            std::cout << "App::Run: glfwCreateWindow failed !" << std::endl;
            glfwTerminate();
            return -1;
        }
        pBase->pWindow = s_pWindow;

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
            //1) input
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

}; //LibUtil