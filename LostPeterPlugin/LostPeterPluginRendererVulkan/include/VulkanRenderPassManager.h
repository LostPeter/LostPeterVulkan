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

#ifndef _VULKAN_RENDER_PASS_MANAGER_H_
#define _VULKAN_RENDER_PASS_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderPassManager : public RenderPassManager
    {
    public:
        VulkanRenderPassManager(VulkanDevice* pDevice);
        virtual ~VulkanRenderPassManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanRenderPassDescriptorPtrVector m_aRenderPassDescriptor;
        VulkanRenderPassDescriptorPtrMap m_mapRenderPassDescriptor;

    public:
        virtual void Destroy();
        virtual bool Init();

    public:
        VulkanRenderPassDescriptor* NewRenderPassDescriptor(const String& nameRenderPassDescriptor);

    public:
        virtual RenderPassDescriptor* GetRenderPassDescriptor(const String& nameRenderPassDescriptor);
        virtual RenderPassDescriptor* CreateRenderPassDescriptor(const String& nameRenderPassDescriptor, FRenderPassType eRenderPass);
        virtual void DestroyRenderPassDescriptor(RenderPassDescriptor* pRenderPassDescriptor);
        virtual void DestroyRenderPassDescriptorAll();
    };

}; //LostPeterPluginRendererVulkan

#endif