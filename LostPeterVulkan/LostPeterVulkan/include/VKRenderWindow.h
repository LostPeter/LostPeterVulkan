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

#ifndef _VK_RENDER_WINDOW_H_
#define _VK_RENDER_WINDOW_H_

#include "RenderWindow.h"

namespace LostPeter
{
    class utilExport VKRenderWindow : public RenderWindow
    {
    public:
        VKRenderWindow(const String& nameRenderWindow);
        virtual ~VKRenderWindow();

    public:


    };

}; //LostPeter

#endif