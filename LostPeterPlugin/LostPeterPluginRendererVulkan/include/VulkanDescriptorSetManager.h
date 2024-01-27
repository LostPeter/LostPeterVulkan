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

#ifndef _VULKAN_DESCRIPTOR_SET_MANAGER_H_
#define _VULKAN_DESCRIPTOR_SET_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanDescriptorSetManager : public FSingleton<VulkanDescriptorSetManager>
    {
    public:
        VulkanDescriptorSetManager(VulkanDevice* pDevice);
        ~VulkanDescriptorSetManager();

    public:
    protected:
        VulkanDevice* m_pDevice;

    public:
        static VulkanDescriptorSetManager& GetSingleton();
		static VulkanDescriptorSetManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }

    public:
        void Destroy();

    public:


    };

}; //LostPeterPluginRendererVulkan

#endif