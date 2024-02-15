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

#ifndef _VULKAN_PIPELINE_CACHE_H_
#define _VULKAN_PIPELINE_CACHE_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanPipelineCache : public FSingleton<VulkanPipelineCache>
    {
    public:
        VulkanPipelineCache(VulkanDevice* pDevice);
        ~VulkanPipelineCache();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VkPipelineCache m_vkPipelineCache;

    public:
        static VulkanPipelineCache& GetSingleton();
		static VulkanPipelineCache* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE VkPipelineCache GetVkPipelineCache() const { return m_vkPipelineCache; }

    public:
        void Destroy();
        bool Init();
    };

}; //LostPeterPluginRendererVulkan

#endif