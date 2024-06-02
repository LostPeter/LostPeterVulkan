/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_RENDER_PASS_H_
#define _RHI_VULKAN_RENDER_PASS_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanRenderPass : public RHIRenderPass
                                              , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanRenderPass(RHIVulkanDevice* pVulkanDevice, const RHIRenderPassCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanRenderPass();

    public:
    protected:
        VkRenderPass m_vkRenderPass;

        RHIVulkanFrameBuffer* m_pVulkanFrameBuffer;
        RHIGraphicsPassColorAttachmentVector m_aColorAttachment;
        RHIGraphicsPassDepthStencilAttachmentVector m_aDepthStencilAttachment;
        RHIGraphicsSubpassDescriptionVector m_aSubpassDescription;
        RHIGraphicsSubpassDependencyVector m_aSubpassDependency;
        String m_strDebugName;

    public:
        F_FORCEINLINE VkRenderPass& GetVkRenderPass() { return m_vkRenderPass; }

        F_FORCEINLINE RHIVulkanFrameBuffer* GetVulkanFrameBuffer() const { return m_pVulkanFrameBuffer; }
        F_FORCEINLINE const RHIGraphicsPassColorAttachmentVector& GetColorAttachmentVector() const { return m_aColorAttachment; }
        F_FORCEINLINE const RHIGraphicsPassDepthStencilAttachmentVector& GetDepthStencilAttachmentVector() const { return m_aDepthStencilAttachment; }
        F_FORCEINLINE const RHIGraphicsSubpassDescriptionVector& GetSubpassDescriptionVector() const { return m_aSubpassDescription; }
        F_FORCEINLINE const RHIGraphicsSubpassDependencyVector& GetSubpassDependencyVector() const { return m_aSubpassDependency; }
        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

        
    protected:
        void createVkRenderPass();    
    };
    
}; //LostPeterPluginRHIVulkan

#endif