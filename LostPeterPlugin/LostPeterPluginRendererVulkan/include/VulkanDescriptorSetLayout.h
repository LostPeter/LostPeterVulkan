/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_DESCRIPTOR_SET_LAYOUT_H_
#define _VULKAN_DESCRIPTOR_SET_LAYOUT_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDescriptorSetLayout : public Base
                                    , public FNonCopyable
    {
        friend class VulkanDescriptorSetLayoutManager;

    private:
        VulkanDescriptorSetLayout(const String& strNameDescriptorSetLayout, VulkanDescriptorSetLayoutManager* pDescriptorSetLayoutManager);
    public:
        ~VulkanDescriptorSetLayout();

    public:
    protected:
        VulkanDescriptorSetLayoutManager* m_pDescriptorSetLayoutManager;
        VkDescriptorSetLayout m_vkDescriptorSetLayout;

    public:
        F_FORCEINLINE VulkanDescriptorSetLayoutManager* GetDescriptorSetLayoutManager() { return m_pDescriptorSetLayoutManager; }
        F_FORCEINLINE const VkDescriptorSetLayout& GetVkDescriptorSetLayout() const { return m_vkDescriptorSetLayout; }
        F_FORCEINLINE VkDescriptorSetLayout GetVkDescriptorSetLayout() { return m_vkDescriptorSetLayout; }

    public:
        void Destroy();
    };

}; //LostPeterPluginRendererVulkan

#endif