/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/AppEx.h"
#include "../include/VulkanSample.h"

namespace LostPeter
{
    VulkanSample* AppEx::s_pSample = nullptr;

    int AppEx::Run(VulkanSample* pSample)
    {
        s_pSample = pSample;

        //1> Parse the command line parameters

        Util_LogInfo("********** 1> AppEx::Run: Parse command success ************");

        //2> glfw initialize and configure 
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) 
        {
            Util_LogError("*********************** AppEx::Run: glfwInit failed !");
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        Util_LogInfo("********** 2> AppEx::Run: Glfw initialize success **********");

        //3> OnCreateWindow
        if (!s_pSample->OnCreateWindow())
        {
            Util_LogError("*********************** AppEx::Run: s_pSample->OnCreateWindow() failed !");
            return -1;
        }
        Util_LogInfo("********** 3> AppEx::Run: OnCreateWindow success ***********");

        //4> OnInit
        if (!s_pSample->OnInit())
        {
            Util_LogError("*********************** AppEx::Run: s_pSample->OnInit() failed !");
            return -1;
        }
        Util_LogInfo("********** 4> AppEx::Run: OnInit success *******************");

        //5> OnResize
        s_pSample->OnResize(s_pSample->GetWidth(), s_pSample->GetHeight(), true);
        Util_LogInfo("********** 5> AppEx::Run: OnResize success *****************");

        //6> OnLoad
        s_pSample->OnLoad();
        Util_LogInfo("********** 6> AppEx::Run: OnLoad success *******************");

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

    void AppEx::error_callback(int error, const char* description)
    {
        Util_LogError("AppEx::error_callback: [%s]", description);
    }

}; //LostPeter