/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyBindGroup.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBindGroup::RHIDummyBindGroup(const RHIBindGroupCreateInfo& createInfo)
        : RHIBindGroup(createInfo)
    {

    }

    RHIDummyBindGroup::~RHIDummyBindGroup()
    {
        Destroy();
    }
    
    void RHIDummyBindGroup::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy