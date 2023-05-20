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

#ifndef _APP_EX_H_
#define _APP_EX_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport AppEx
    {
    public:
        static VulkanSample* s_pSample;

    public:
        static int Run(VulkanSample* pSample);
        
    protected:
        static void error_callback(int error, const char* description);
    };

}; //LostPeter

#endif