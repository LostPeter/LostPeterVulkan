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
#include "../include/VulkanFrameBufferAttachment.h"
#include "../include/VulkanRenderPassDescriptor.h"

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
        DeleteFrameBufferAttachmentAll();
    }

    bool VulkanRenderPassDescriptor::Init()
    {
        
        return true;
    }

    bool VulkanRenderPassDescriptor::HasFrameBufferAttachment(const String& strName)
    {
        return GetFrameBufferAttachment(strName) != nullptr;
    }
    VulkanFrameBufferAttachment* VulkanRenderPassDescriptor::GetFrameBufferAttachment(const String& strName)
    {
        VulkanFrameBufferAttachmentPtrMap::iterator itFind = m_mapFrameBufferAttachment.find(strName);
        if (itFind == m_mapFrameBufferAttachment.end())
        {
            return nullptr;
        }
        return itFind->second;
    }
    bool VulkanRenderPassDescriptor::AddFrameBufferAttachment(VulkanFrameBufferAttachment* pFrameBufferAttachment)
    {
        const String& strName = pFrameBufferAttachment->GetName();
        VulkanFrameBufferAttachmentPtrMap::iterator itFind = m_mapFrameBufferAttachment.find(strName);
        if (itFind != m_mapFrameBufferAttachment.end())
        {
            F_LogError("*********************** VulkanRenderPassDescriptor::AddFrameBufferAttachment: FrameBufferAttachment name already exist: [%s] !", strName.c_str());
            return false;
        }
        
        m_mapFrameBufferAttachment.insert(VulkanFrameBufferAttachmentPtrMap::value_type(strName, pFrameBufferAttachment));
        m_aFrameBufferAttachment.push_back(pFrameBufferAttachment);
        return true;
    }
    void VulkanRenderPassDescriptor::DeleteFrameBufferAttachment(const String& strName)
    {
        VulkanFrameBufferAttachmentPtrMap::iterator itFind = m_mapFrameBufferAttachment.find(strName);
        if (itFind == m_mapFrameBufferAttachment.end())
        {
            return;
        }

        VulkanFrameBufferAttachmentPtrVector::iterator itFindA = std::find(m_aFrameBufferAttachment.begin(), m_aFrameBufferAttachment.end(), itFind->second);
        if (itFindA != m_aFrameBufferAttachment.end())
            m_aFrameBufferAttachment.erase(itFindA);
        F_DELETE(itFind->second)
        m_mapFrameBufferAttachment.erase(itFind);
    }
    void VulkanRenderPassDescriptor::DeleteFrameBufferAttachmentAll()
    {
        for (VulkanFrameBufferAttachmentPtrVector::iterator it = m_aFrameBufferAttachment.begin();
             it != m_aFrameBufferAttachment.end(); ++it)
        {
            F_DELETE(*it)
        }
        m_aFrameBufferAttachment.clear();
        m_mapFrameBufferAttachment.clear();
    }


    void VulkanRenderPassDescriptor::SetClearColor(uint8 nIndex, const FColor& clearColor)
    {

    }
    void VulkanRenderPassDescriptor::SetClearColor(const FColor& clearColor)
    {

    }
        
    void VulkanRenderPassDescriptor::SetClearDepth(float clearDepth)
    {

    }
    void VulkanRenderPassDescriptor::SetClearStencil(uint32 clearStencil)
    {
        
    }

}; //LostPeterPluginRendererVulkan