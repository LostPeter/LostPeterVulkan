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

#ifndef _RHI_DUMMY_RENDER_PASS_H_
#define _RHI_DUMMY_RENDER_PASS_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyRenderPass : public RHIRenderPass
                                            , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyRenderPass(RHIDummyDevice* pDummyDevice, const RHIRenderPassCreateInfo& createInfo);
    public:
        virtual ~RHIDummyRenderPass();

    public:
    protected:


    public:


    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIDummy

#endif