/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-28
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/VulkanRenderPassDescriptorManager.h"
#include "../include/VulkanDevice.h"
#include "../include/VulkanRenderPassDescriptor.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPassDescriptorManager::VulkanRenderPassDescriptorManager(VulkanDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "VulkanRenderPassDescriptorManager::VulkanRenderPassDescriptorManager")
    }

    VulkanRenderPassDescriptorManager::~VulkanRenderPassDescriptorManager()
    {
        Destroy();
    }

    void VulkanRenderPassDescriptorManager::Destroy()
    {
        DestroyRenderPassDescriptorAll();

        RenderPassDescriptorManager::Destroy();
    }

    bool VulkanRenderPassDescriptorManager::Init()
    {

        return true;
    }

    VulkanRenderPassDescriptor* VulkanRenderPassDescriptorManager::NewRenderPassDescriptor(const String& nameRenderPassDescriptor)
    {
        return new VulkanRenderPassDescriptor(nameRenderPassDescriptor, m_pDevice);
    }

    RenderPassDescriptor* VulkanRenderPassDescriptorManager::GetRenderPassDescriptor(const String& nameRenderPassDescriptor)
    {
        VulkanRenderPassDescriptorPtrMap::iterator itFind = m_mapRenderPassDescriptor.find(nameRenderPassDescriptor);
        if (itFind == m_mapRenderPassDescriptor.end())
            return nullptr;
        return itFind->second;
    }

    RenderPassDescriptor* VulkanRenderPassDescriptorManager::CreateRenderPassDescriptor(const String& nameRenderPassDescriptor, FRenderPassType eRenderPass)
    {
        RenderPassDescriptor* pRenderPassDescriptor = GetRenderPassDescriptor(nameRenderPassDescriptor);
        if (pRenderPassDescriptor != nullptr)
        {
            return pRenderPassDescriptor;
        }

        VulkanRenderPassDescriptor* pVulkanRenderPassDescriptor = NewRenderPassDescriptor(nameRenderPassDescriptor);
        m_aRenderPassDescriptor.push_back(pVulkanRenderPassDescriptor);
        m_mapRenderPassDescriptor[nameRenderPassDescriptor] = pVulkanRenderPassDescriptor;
        return pVulkanRenderPassDescriptor;
    }

    void VulkanRenderPassDescriptorManager::DestroyRenderPassDescriptor(RenderPassDescriptor* pRenderPassDescriptor)
    {
        if (pRenderPassDescriptor == nullptr)
            return;

        VulkanRenderPassDescriptorPtrVector::iterator itF = std::find(m_aRenderPassDescriptor.begin(), m_aRenderPassDescriptor.end(), (VulkanRenderPassDescriptor*)pRenderPassDescriptor);
        if (itF != m_aRenderPassDescriptor.end())
            m_aRenderPassDescriptor.erase(itF);
        VulkanRenderPassDescriptorPtrMap::iterator itFind = m_mapRenderPassDescriptor.find(pRenderPassDescriptor->GetName());
        if (itFind != m_mapRenderPassDescriptor.end())
            m_mapRenderPassDescriptor.erase(itFind);
        F_DELETE(pRenderPassDescriptor)
    }

    void VulkanRenderPassDescriptorManager::DestroyRenderPassDescriptorAll()
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