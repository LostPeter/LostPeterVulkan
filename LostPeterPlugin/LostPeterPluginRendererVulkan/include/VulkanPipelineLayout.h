/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_PIPELINE_LAYOUT_H_
#define _VULKAN_PIPELINE_LAYOUT_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanPipelineLayout : public Base 
                               , public FNonCopyable
    {
        friend class VulkanPipelineLayoutManager;

    private:
        VulkanPipelineLayout(const String& namePipelineLayout, VulkanDevice* pDevice);
    public:
        ~VulkanPipelineLayout();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkPipelineLayout m_vkPipelineLayout;

    public:
        F_FORCEINLINE VkPipelineLayout GetVkPipelineLayout() const { return m_vkPipelineLayout; }

    public:
        void Destroy();
        bool Init(const VkDescriptorSetLayoutVector& aDescriptorSetLayout);

    };

}; //LostPeterPluginRendererVulkan

#endif