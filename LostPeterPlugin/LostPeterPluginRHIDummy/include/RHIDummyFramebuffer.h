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
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyFrameBuffer : public RHIFrameBuffer
                                             , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyFrameBuffer(RHIDummyDevice* pDummyDevice, const RHIFrameBufferCreateInfo& createInfo);
    public:
        virtual ~RHIDummyFrameBuffer();

    public:
    protected:


    public:

        
    public:
        virtual void Destroy();

    
    };
    
}; //LostPeterPluginRHIDummy

#endif