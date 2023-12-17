/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/WindowListener.h"
#include "../include/Window.h"

namespace LostPeterEngine
{
    WindowListener::WindowListener(const String& nameWindowListener, Window* pWindow)
        : Base(nameWindowListener)
        , m_pWindow(pWindow)
    {

    }   

    WindowListener::~WindowListener()
    {
        
    }

}; //LostPeterEngine