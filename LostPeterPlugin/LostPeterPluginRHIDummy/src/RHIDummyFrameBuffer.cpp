/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyFrameBuffer.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyFrameBuffer::RHIDummyFrameBuffer(RHIDummyDevice* pDummyDevice, const RHIFrameBufferCreateInfo& createInfo)
        : RHIFrameBuffer(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyFrameBuffer::RHIDummyFrameBuffer")
    }

    RHIDummyFrameBuffer::~RHIDummyFrameBuffer()
    {
        
    }
    
    void RHIDummyFrameBuffer::Destroy()
    {

    }
    

}; //LostPeterPluginRHIDummy