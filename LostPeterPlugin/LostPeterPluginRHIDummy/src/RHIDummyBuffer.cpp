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

#include "../include/RHIDummyBuffer.h"
#include "../include/RHIDummyBufferView.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyBuffer::RHIDummyBuffer(RHIDummyDevice* pDummyDevice, const RHIBufferCreateInfo& createInfo)
        : RHIBuffer(pDummyDevice, createInfo)
        , m_pDummyDevice(pDummyDevice)
        , m_aDummyData(1)
    {
        F_Assert(m_pDummyDevice && "RHIDummyBuffer::RHIDummyBuffer")
    }   

    RHIDummyBuffer::~RHIDummyBuffer()
    {
        Destroy();
    }
    
    void RHIDummyBuffer::Destroy()
    {

    }

    void* RHIDummyBuffer::Map(RHIMapType eMap, uint32 nOffset, uint32 nLength)
    {
        return m_aDummyData.data();
    }

    void RHIDummyBuffer::UnMap()
    {

    }

    RHIBufferView* RHIDummyBuffer::CreateBufferView(const RHIBufferViewCreateInfo& createInfo)
    {
        return new RHIDummyBufferView(this, createInfo);
    }

}; //LostPeterPluginRHIDummy