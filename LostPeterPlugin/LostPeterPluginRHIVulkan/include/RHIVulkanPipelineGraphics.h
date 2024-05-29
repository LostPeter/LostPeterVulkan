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

#ifndef _RHI_VULKAN_PIPELINE_GRAPHICS_H_
#define _RHI_VULKAN_PIPELINE_GRAPHICS_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"
#include "RHIVulkanPipeline.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanPipelineGraphics : public RHIPipelineGraphics
                                                    , public RHIVulkanObject
                                                    , public RHIVulkanPipeline
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanPipelineGraphics(RHIVulkanDevice* pVulkanDevice, const RHIPipelineGraphicsCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanPipelineGraphics();

    public:
    protected:
        RHIVulkanPipelineCache* m_pVulkanPipelineCache;
        RHIVulkanPipelineLayout* m_pVulkanPipelineLayout;
        RHIVulkanShaderModule* m_pShaderVertex;
        RHIVulkanShaderModule* m_pShaderPixel;
        RHIVulkanShaderModule* m_pShaderGeometry;
        RHIVulkanShaderModule* m_pShaderDomain;
        RHIVulkanShaderModule* m_pShaderHull;
        RHIVertexState m_sVertexState;
        RHIPrimitiveState m_sPrimitiveState;
        RHIDepthStencilState m_sDepthStencilState;
        RHIMultiSampleState m_sMultiSampleState;
        RHIFragmentState m_sFragmentState;

        String m_strDebugName;

    public:
        F_FORCEINLINE RHIVulkanPipelineCache* GetVulkanPipelineCache() const { return m_pVulkanPipelineCache; }
        F_FORCEINLINE RHIVulkanPipelineLayout* GetVulkanPipelineLayout() const { return m_pVulkanPipelineLayout; }
        F_FORCEINLINE RHIVulkanShaderModule* GetVulkanShaderVertex() const { return m_pShaderVertex; }
        F_FORCEINLINE RHIVulkanShaderModule* GetVulkanShaderPixel() const { return m_pShaderPixel; }
        F_FORCEINLINE RHIVulkanShaderModule* GetVulkanShaderGeometry() const { return m_pShaderGeometry; }
        F_FORCEINLINE RHIVulkanShaderModule* GetVulkanShaderDomain() const { return m_pShaderDomain; }
        F_FORCEINLINE RHIVulkanShaderModule* GetVulkanShaderHull() const { return m_pShaderHull; }
        F_FORCEINLINE RHIVertexState& GetVertexState() { return m_sVertexState; }
        F_FORCEINLINE RHIPrimitiveState& GetPrimitiveState() { return m_sPrimitiveState; }
        F_FORCEINLINE RHIDepthStencilState& GetDepthStencilState() { return m_sDepthStencilState; }
        F_FORCEINLINE RHIMultiSampleState& GetMultiSampleState() { return m_sMultiSampleState; }
        F_FORCEINLINE RHIFragmentState& GetFragmentState() { return m_sFragmentState; }

        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkPipelineGraphics();
    };

}; //LostPeterPluginRHIVulkan

#endif