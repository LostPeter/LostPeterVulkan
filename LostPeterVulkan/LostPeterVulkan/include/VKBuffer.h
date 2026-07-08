/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2026-07-08
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VK_BUFFER_H_
#define _VK_BUFFER_H_

#include "Base.h"

namespace LostPeterVulkan
{
    class vulkanExport VKBuffer : public Base
    {
    public:
        VKBuffer(const String& nameBuffer);
        virtual ~VKBuffer();

    public:
        

    public:
        virtual void Destroy() = 0;
        
    };

}; //LostPeterVulkan

#endif