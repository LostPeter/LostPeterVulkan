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

#include "../include/RHIDummyFence.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyFence::RHIDummyFence(RHIDevice& device)
        : RHIFence(device)
    {

    }

    RHIDummyFence::~RHIDummyFence()
    {
        
    }
    
    void RHIDummyFence::Destroy()
    {

    }

    RHIFenceStatusType RHIDummyFence::GetStatus()
    {
        return RHIFenceStatusType::RHI_FenceStatus_Signaled;
    }

    void RHIDummyFence::Reset()
    {

    }

    void RHIDummyFence::Wait()
    {

    }

}; //LostPeterPluginRHIDummy