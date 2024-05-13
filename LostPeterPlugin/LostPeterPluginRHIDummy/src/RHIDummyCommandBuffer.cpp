/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyCommandBuffer.h"
#include "../include/RHIDummyCommandEncoder.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyCommandBuffer::RHIDummyCommandBuffer()
    {

    }

    RHIDummyCommandBuffer::~RHIDummyCommandBuffer()
    {
        Destroy();
    }  
    
    void RHIDummyCommandBuffer::Destroy()
    {

    }

    RHICommandEncoder* RHIDummyCommandBuffer::Begin()
    {
        return new RHIDummyCommandEncoder(*this);
    }

}; //LostPeterPluginRHIDummy