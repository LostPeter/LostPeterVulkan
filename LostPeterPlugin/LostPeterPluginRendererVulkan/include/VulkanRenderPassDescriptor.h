/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-24
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_RENDER_PASS_DESCRIPTOR_H_
#define _VULKAN_RENDER_PASS_DESCRIPTOR_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderPassDescriptor : public RenderPassDescriptor
    {
    public:
        VulkanRenderPassDescriptor(const String& nameRenderPassDescriptor, VulkanDevice* pDevice);
        virtual ~VulkanRenderPassDescriptor();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanFrameBufferAttachmentPtrVector m_aFrameBufferAttachment;
        VulkanFrameBufferAttachmentPtrMap m_mapFrameBufferAttachment;
        VkSampler m_vkSampler;
        VkDescriptorImageInfo m_vkDescriptorImageInfo;
        

    public:
        F_FORCEINLINE const VulkanFrameBufferAttachmentPtrVector& GetFrameBufferAttachmentPtrVector() const { return m_aFrameBufferAttachment; }
        F_FORCEINLINE VulkanFrameBufferAttachmentPtrVector& GetFrameBufferAttachmentPtrVector() { return m_aFrameBufferAttachment; }
        F_FORCEINLINE const VulkanFrameBufferAttachmentPtrMap& GetFrameBufferAttachmentPtrMap() const { return m_mapFrameBufferAttachment; }
        F_FORCEINLINE VulkanFrameBufferAttachmentPtrMap& GetFrameBufferAttachmentPtrMap() { return m_mapFrameBufferAttachment; }



    public:
        virtual void Destroy();
        virtual bool Init();



    public:
        bool HasFrameBufferAttachment(const String& strName);
        VulkanFrameBufferAttachment* GetFrameBufferAttachment(const String& strName);
        bool AddFrameBufferAttachment(VulkanFrameBufferAttachment* pFrameBufferAttachment);
        void DeleteFrameBufferAttachment(const String& strName);
        void DeleteFrameBufferAttachmentAll();

    };

}; //LostPeterPluginRendererVulkan

#endif