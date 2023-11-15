/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RENDER_WINDOW_H_
#define _RENDER_WINDOW_H_

#include "RenderTarget.h"

namespace LostPeter
{
    class utilExport RenderWindow : public RenderTarget
    {
    public:
        RenderWindow(const String& nameRenderWindow);
        virtual ~RenderWindow();

    public:
        

    };

}; //LostPeter

#endif