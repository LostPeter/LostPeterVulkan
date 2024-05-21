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

#ifndef _RHI_DUMMY_FRAME_BUFFER_H_
#define _RHI_DUMMY_FRAME_BUFFER_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyFrameBuffer : public RHIFrameBuffer
    {
    public:
        RHIDummyFrameBuffer(RHIDummyDevice* pDummyDevice, const RHIFrameBufferCreateInfo& createInfo);
        virtual ~RHIDummyFrameBuffer();

    public:
    protected:
        RHIDummyDevice* m_pDummyDevice;

    public:
        F_FORCEINLINE RHIDummyDevice* GetDummyDevice() { return m_pDummyDevice; }

        
    public:
        virtual void Destroy();

    
    };
    
}; //LostPeterPluginRHIDummy

#endif