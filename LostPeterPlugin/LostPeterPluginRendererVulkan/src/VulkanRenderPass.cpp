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

#include "../include/VulkanRenderPass.h"
#include "../include/VulkanDevice.h"

namespace LostPeterPluginRendererVulkan
{
    VulkanRenderPass::VulkanRenderPass(const String& nameRenderPass, VulkanDevice* pDevice)
        : Base(nameRenderPass)
        , m_pDevice(pDevice)
        , m_vkRenderPass(VK_NULL_HANDLE)
    {
        F_Assert(m_pDevice && "VulkanRenderPass::VulkanRenderPass")
    }

    VulkanRenderPass::~VulkanRenderPass()
    {
        Destroy();
    }

    void VulkanRenderPass::Destroy()
    {
        if (this->m_vkRenderPass != VK_NULL_HANDLE)
        {
            m_pDevice->DestroyVkRenderPass(this->m_vkRenderPass);
        }
        this->m_vkRenderPass = VK_NULL_HANDLE;
    }

    bool VulkanRenderPass::Init(const VkAttachmentDescriptionVector& aAttachmentDescription,
                                const VkSubpassDescriptionVector& aSubpassDescription,
                                const VkSubpassDependencyVector& aSubpassDependency,
                                VkRenderPassMultiviewCreateInfo* pMultiviewCI)
    {
        Destroy();
        const String& nameRenderPass = GetName();
        if (!m_pDevice->CreateVkRenderPass(nameRenderPass,
                                           aAttachmentDescription,
                                           aSubpassDescription,
                                           aSubpassDependency,
                                           pMultiviewCI,
                                           this->m_vkRenderPass))
        {
            F_LogError("*********************** VulkanRenderPass::Init: Failed to CreateVkRenderPass, nameRenderPass: [%s] !", nameRenderPass.c_str());
            return false;
        }

        return true;
    }




}; //LostPeterPluginRendererVulkan