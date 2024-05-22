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

#ifndef _RHI_VULKAN_PIPELINE_COMPUTE_H_
#define _RHI_VULKAN_PIPELINE_COMPUTE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineCompute : public RHIPipelineCompute
    {
    public:
        RHIVulkanPipelineCompute(RHIVulkanDevice* pVulkanDevice, const RHIPipelineComputeCreateInfo& createInfo);
        virtual ~RHIVulkanPipelineCompute();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        VkPipeline m_vkPipeline;

        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }
        F_FORCEINLINE VkPipeline& GetVkPipeline() { return m_vkPipeline; }

        F_FORCEINLINE const String& GetDebugName() { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipeline();
    };
    
}; //LostPeterPluginRHIVulkan

#endif