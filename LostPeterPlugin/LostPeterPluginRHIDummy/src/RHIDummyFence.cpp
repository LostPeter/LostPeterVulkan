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
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyFence::RHIDummyFence(RHIDummyDevice* pDummyDevice, const RHIFenceCreateInfo& createInfo)
        : RHIFence(pDummyDevice, createInfo)
        , RHIDummyObject(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyFence::RHIDummyFence")
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