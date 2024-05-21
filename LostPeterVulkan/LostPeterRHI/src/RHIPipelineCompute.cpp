/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIPipelineCompute.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIPipelineCompute::RHIPipelineCompute(RHIDevice* pDevice, const RHIPipelineComputeCreateInfo& createInfo)
        : RHIPipeline(pDevice)
    {
        F_Assert(m_pDevice && "RHIPipelineCompute::RHIPipelineCompute")
    }

    RHIPipelineCompute::~RHIPipelineCompute()
    {
        
    }
    

}; //LostPeterRHI