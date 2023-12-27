/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanFrameBufferDescriptor.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanConverter.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanFrameBufferDescriptor::VulkanFrameBufferDescriptor(const String& nameFrameBufferAttachment,
                                                             VulkanDevice* pDevice)
        : Base(nameFrameBufferAttachment)
        , m_pDevice(pDevice)
    {
        F_Assert(m_pDevice != nullptr && "VulkanFrameBufferDescriptor::VulkanFrameBufferDescriptor");
    }

    VulkanFrameBufferDescriptor::~VulkanFrameBufferDescriptor()
    {
        Destroy();
    }
    
    void VulkanFrameBufferDescriptor::Destroy()
    {
        
    }

    bool VulkanFrameBufferDescriptor::Init()
    {
        

        return true;
    }

}; //LostPeterPluginRendererVulkan