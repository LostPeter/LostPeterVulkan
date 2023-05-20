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


        //2> glfw initialize and configure 
        glfwSetErrorCallback(error_callback);
        if (!glfwInit()) {

            Util_LogError("AppEx::Run: glfwInit failed !");
            return -1;
        }
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //3> glfw window creation


        //9> OnDestroy

        //10> Cleanup
        glfwTerminate();

        return 0;
    }

    void AppEx::error_callback(int error, const char* description)
    {
        Util_LogError("AppEx::error_callback: [%s]", description);
    }

}; //LostPeter