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

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanFrameBuffer : public RHIFrameBuffer
    {
    public:
        RHIVulkanFrameBuffer(RHIVulkanDevice* pVulkanDevice, const RHIFrameBufferCreateInfo& createInfo);
        virtual ~RHIVulkanFrameBuffer();

    public:
    protected:
        RHIVulkanDevice* m_pVulkanDevice;
        
    public:
        F_FORCEINLINE RHIVulkanDevice* GetVulkanDevice() { return m_pVulkanDevice; }

    public:
        virtual void Destroy();

    };
    
}; //LostPeterPluginRHIVulkan

#endif