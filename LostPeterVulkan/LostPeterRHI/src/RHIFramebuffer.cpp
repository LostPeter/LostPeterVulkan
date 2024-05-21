/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIFrameBuffer.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIFrameBuffer::RHIFrameBuffer(RHIDevice* pDevice, const RHIFrameBufferCreateInfo& createInfo)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHIFrameBuffer::RHIFrameBuffer")
    }

    RHIFrameBuffer::~RHIFrameBuffer()
    {
        
    }
    

}; //LostPeterRHI