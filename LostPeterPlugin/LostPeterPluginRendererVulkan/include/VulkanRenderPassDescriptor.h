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

    public:

    public:
        virtual void Destroy();
        virtual bool Init();


    };

}; //LostPeterPluginRendererVulkan

#endif