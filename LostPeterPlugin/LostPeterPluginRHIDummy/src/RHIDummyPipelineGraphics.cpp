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
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyPipelineGraphics::RHIDummyPipelineGraphics(RHIDummyDevice* pDummyDevice, const RHIPipelineGraphicsCreateInfo& createInfo)
        : RHIPipelineGraphics(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyPipelineGraphics::RHIDummyPipelineGraphics")
    }

    RHIDummyPipelineGraphics::~RHIDummyPipelineGraphics()
    {
        Destroy();
    }
    
    void RHIDummyPipelineGraphics::Destroy()
    {

    }

}; //LostPeterPluginRHIDummy