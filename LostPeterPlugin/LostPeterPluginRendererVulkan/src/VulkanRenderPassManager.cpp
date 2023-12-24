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

#include "../include/VulkanRenderPassManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderPassDescriptor.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPassManager::VulkanRenderPassManager()
    {
        
    }

    VulkanRenderPassManager::~VulkanRenderPassManager()
    {
        Destroy();
    }

    void VulkanRenderPassManager::Destroy()
    {
        DestroyRenderPassDescriptorAll();

        RenderPassManager::Destroy();
    }

    bool VulkanRenderPassManager::Init()
    {

        return true;
    }

    RenderPassDescriptor* VulkanRenderPassManager::GetRenderPassDescriptor(FRenderPassType eRenderPass)
    {
        return nullptr;
    }

    RenderPassDescriptor* VulkanRenderPassManager::CreateRenderPassDescriptor(FRenderPassType eRenderPass)
    {
        return nullptr;
    }

    void VulkanRenderPassManager::DestroyRenderPassDescriptor(RenderPassDescriptor* pRenderPassDescriptor)
    {

    }

    void VulkanRenderPassManager::DestroyRenderPassDescriptorAll()
    {
        int count = (int)m_aRenderPassDescriptor.size();
        for (int i = 0; i < count; i++)
        {
            VulkanRenderPassDescriptor* pRenderPassDescriptor = m_aRenderPassDescriptor[i];
            F_DELETE(pRenderPassDescriptor)
        }
        m_aRenderPassDescriptor.clear();
        m_mapRenderPassDescriptor.clear();
    }

}; //LostPeterPluginRendererVulkan