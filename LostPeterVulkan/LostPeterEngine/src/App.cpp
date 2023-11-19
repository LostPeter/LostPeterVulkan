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

#include "../include/App.h"
#include "../include/Sample.h"

namespace LostPeter
{
    Sample* App::s_pSample = nullptr;

    int App::Run(Sample* pSample)
    {
        s_pSample = pSample;

        //1> Parse the command line parameters

        F_LogInfo("********** 1> App::Run: Parse command success ************");

        //2> glfw initialize and configure 
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) 
        {
            F_LogError("*********************** App::Run: glfwInit failed !");
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        F_LogInfo("********** 2> App::Run: Glfw initialize success **********");

        //3> OnCreateWindow
        if (!s_pSample->OnCreateWindow())
        {
            F_LogError("*********************** App::Run: s_pSample->OnCreateWindow() failed !");
            return -1;
        }
        F_LogInfo("********** 3> App::Run: OnCreateWindow success ***********");

        //4> OnInit
        if (!s_pSample->OnInit())
        {
            F_LogError("*********************** App::Run: s_pSample->OnInit() failed !");
            return -1;
        }
        F_LogInfo("********** 4> App::Run: OnInit success *******************");

        //5> OnResize
        s_pSample->OnResize(s_pSample->GetWidth(), s_pSample->GetHeight(), true);
        F_LogInfo("********** 5> App::Run: OnResize success *****************");

        //6> OnLoad
        s_pSample->OnLoad();
        F_LogInfo("********** 6> App::Run: OnLoad success *******************");

        //7> OnIsInit
        bool isInit = s_pSample->OnIsInit();

        //8> Main loop
        while (!s_pSample->OnIsWindowsClosed()) 
        {
            //0) timer
            s_pSample->UpdateTimer();
            
            //1) input
            //pBase->OnMouseInput();
            //pBase->OnKeyboardInput();

            //2) poll IO events (keys pressed/released, mouse moved etc)
            glfwPollEvents();

            //3) 


        }

        //9> OnDestroy
        s_pSample->OnDestroy();

        //10> Cleanup
        glfwTerminate();

        return 0;
    }

    void App::error_callback(int error, const char* description)
    {
        F_LogError("App::error_callback: [%s]", description);
    }

}; //LostPeter