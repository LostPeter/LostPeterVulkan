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

#include "../include/RHIDummyPipelineLayout.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPipelineLayout::RHIDummyPipelineLayout(const RHIPipelineLayoutCreateInfo& createInfo)
        : RHIPipelineLayout(createInfo)
    {

    }

    RHIDummyPipelineLayout::~RHIDummyPipelineLayout()
    {
        Destroy();
    }
    
    void RHIDummyPipelineLayout::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy