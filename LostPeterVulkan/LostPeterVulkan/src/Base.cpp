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

namespace LostPeterVulkan
{
    VulkanWindow* Base::ms_pWindow = nullptr;
    Base::Base(const String& _name)
        : group(0)
        , name(_name)
        , refCount(0)
        , isInit(false)
    {
       
    }
    Base::Base(uint32 _group, const String& _name)
        : group(_group)
        , name(_name)
        , refCount(0)
        , isInit(false)
    {

    }
    Base::~Base()
    {
        
    }

    bool Base::IsGroupNameValid() const
    {
        if (name.empty() || group <= 0)
			return false;
        return true;
    }

}; //LostPeterVulkan