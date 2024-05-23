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

#ifndef _RHI_DUMMY_PIPELINE_COMPUTE_H_
#define _RHI_DUMMY_PIPELINE_COMPUTE_H_

#include "RHIDummyPreDefine.h"
#include "RHIDummyObject.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyPipelineCompute : public RHIPipelineCompute
                                                 , public RHIDummyObject
    {
        friend class RHIDummyDevice;

    protected:    
        RHIDummyPipelineCompute(RHIDummyDevice* pDummyDevice, const RHIPipelineComputeCreateInfo& createInfo);
    public:
        virtual ~RHIDummyPipelineCompute();

    public:
    protected:


    public:
        

    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIDummy

#endif