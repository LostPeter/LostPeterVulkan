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

#ifndef _APP_H_
#define _APP_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport App
    {
    public:
        static Sample* s_pSample;

    public:
        static int Run(Sample* pSample);
        
    protected:
        static void error_callback(int error, const char* description);
    };

}; //LostPeterEngine

#endif