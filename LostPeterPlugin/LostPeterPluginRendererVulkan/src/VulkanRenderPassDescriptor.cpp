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

#include "../include/VulkanRenderPassDescriptor.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPassDescriptor::VulkanRenderPassDescriptor(const String& nameRenderPassDescriptor, VulkanDevice* pDevice)
        : RenderPassDescriptor(nameRenderPassDescriptor)
        , m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanRenderPassDescriptor::VulkanRenderPassDescriptor")
    }

    VulkanRenderPassDescriptor::~VulkanRenderPassDescriptor()
    {
        Destroy();
    }

    void VulkanRenderPassDescriptor::Destroy()
    {
        
    }

    bool VulkanRenderPassDescriptor::Init()
    {
        
        return true;
    }

}; //LostPeterPluginRendererVulkan