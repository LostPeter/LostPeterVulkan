/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Base.h"
#include "../include/VulkanWindow.h"

namespace LostPeter
{
    VulkanWindow* Base::ms_pVulkanWindow = nullptr;
    Base::Base(const String& _name)
        : name(_name)
        , refCount(0)
        , isInit(false)
    {
       
    }
    Base::~Base()
    {
        
    }

}; //LostPeter