/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-13
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanPipelineCompute.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanPipelineCompute::RHIVulkanPipelineCompute(const RHIPipelineComputeCreateInfo& createInfo)
        : RHIPipelineCompute(createInfo)
    {

    }

    RHIVulkanPipelineCompute::~RHIVulkanPipelineCompute()
    {
        Destroy();
    }
    
    void RHIVulkanPipelineCompute::Destroy()
    {

    }

}; //LostPeterPluginRHIVulkan