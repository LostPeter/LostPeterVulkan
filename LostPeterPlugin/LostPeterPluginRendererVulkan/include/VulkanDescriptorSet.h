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

#ifndef _VULKAN_DESCRIPTOR_SET_H_
#define _VULKAN_DESCRIPTOR_SET_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDescriptorSet : public Base
                              , public FNonCopyable
    {
        friend class VulkanDescriptorSetManager;

    private:
        VulkanDescriptorSet(const String& strNameDescriptorSet, VulkanDescriptorSetManager* pDescriptorSetManager);
    public:
        ~VulkanDescriptorSet();

    public:
    protected:
        VulkanDescriptorSetManager* m_pDescriptorSetManager;
        VkDescriptorSet m_vkDescriptorSet;

    public:
        F_FORCEINLINE VulkanDescriptorSetManager* GetDescriptorSetManager() { return m_pDescriptorSetManager; }
        F_FORCEINLINE VkDescriptorSet GetVkDescriptorSet() { return m_vkDescriptorSet; }

    public:
        void Destroy();

        bool Init();
    };

}; //LostPeterPluginRendererVulkan

#endif