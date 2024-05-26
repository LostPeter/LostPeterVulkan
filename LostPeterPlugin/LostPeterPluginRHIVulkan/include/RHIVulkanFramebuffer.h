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

#ifndef _RHI_VULKAN_FRAME_BUFFER_H_
#define _RHI_VULKAN_FRAME_BUFFER_H_

#include "RHIVulkanPreDefine.h"
#include "RHIVulkanObject.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanFrameBuffer : public RHIFrameBuffer
                                               , public RHIVulkanObject
    {   
        friend class RHIVulkanDevice;

    protected:
        RHIVulkanFrameBuffer(RHIVulkanDevice* pVulkanDevice, const RHIFrameBufferCreateInfo& createInfo);
    public:
        virtual ~RHIVulkanFrameBuffer();

    public:
    protected:
        VkFramebuffer m_vkFramebuffer;


        String m_strDebugName;
        
    public:
        F_FORCEINLINE VkFramebuffer& GetVkFramebuffer() { return m_vkFramebuffer; }


        F_FORCEINLINE const String& GetDebugName() const { return m_strDebugName; }

    public:
        virtual void Destroy();

    protected:
        void createVkFramebuffer();
    };
    
}; //LostPeterPluginRHIVulkan

#endif