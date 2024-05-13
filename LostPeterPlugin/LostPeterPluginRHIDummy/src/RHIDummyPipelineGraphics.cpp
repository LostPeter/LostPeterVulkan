/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyPipelineGraphics.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPipelineGraphics::RHIDummyPipelineGraphics(const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(createInfo)
    {

    }

    RHIDummyPipelineGraphics::~RHIDummyPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIDummyPipelineGraphics::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy