/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_BUFFER_H_
#define _RHI_VULKAN_BUFFER_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanBuffer : public RHIBuffer
    {
    public:
        RHIVulkanBuffer(const RHIBufferCreateInfo& createInfo);
        virtual ~RHIVulkanBuffer();

    public:
    protected:
        std::vector<uint8> m_aVulkanData;

    public:
        virtual void Destroy();

        virtual void* Map(RHIMapType eMap, uint32 nOffset, uint32 nLength);
        virtual void UnMap();
        virtual RHIBufferView* CreateBufferView(const RHIBufferViewCreateInfo& createInfo);
    };
    
}; //LostPeterPluginRHIVulkan

#endif