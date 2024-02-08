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

#ifndef _VULKAN_RENDER_PASS_H_
#define _VULKAN_RENDER_PASS_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderPass : public Base
                           , public FNonCopyable
    {
        friend class VulkanRenderPassManager;

    private:
        VulkanRenderPass(const String& nameRenderPass, VulkanDevice* pDevice);
    public:
        ~VulkanRenderPass();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkRenderPass m_vkRenderPass;

    public:
        F_FORCEINLINE VkRenderPass GetVkRenderPass() const { return m_vkRenderPass; }

    public:
        void Destroy();
        bool Init(const VkAttachmentDescriptionVector& aAttachmentDescription,
                  const VkSubpassDescriptionVector& aSubpassDescription,
                  const VkSubpassDependencyVector& aSubpassDependency,
                  VkRenderPassMultiviewCreateInfo* pMultiviewCI);

    public:
        
    };

}; //LostPeterPluginRendererVulkan

#endif