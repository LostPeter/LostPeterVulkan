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

#ifndef _VULKAN_PIPELINE_LAYOUT_MANAGER_H_
#define _VULKAN_PIPELINE_LAYOUT_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanPipelineLayoutManager : public FSingleton<VulkanPipelineLayoutManager>
    {
    public:
        VulkanPipelineLayoutManager(VulkanDevice* pDevice);
        ~VulkanPipelineLayoutManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanPipelineLayoutPtrVector m_aPipelineLayout;
        VulkanPipelineLayoutPtrMap m_mapPipelineLayout;

    public:
        static VulkanPipelineLayoutManager& GetSingleton();
		static VulkanPipelineLayoutManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanPipelineLayoutPtrVector& GetPipelineLayoutPtrVector() const { return m_aPipelineLayout; }
        F_FORCEINLINE VulkanPipelineLayoutPtrVector& GetPipelineLayoutPtrVector() { return m_aPipelineLayout; }
        F_FORCEINLINE const VulkanPipelineLayoutPtrMap& GetPipelineLayoutPtrMap() const { return m_mapPipelineLayout; }
        F_FORCEINLINE VulkanPipelineLayoutPtrMap& GetPipelineLayoutPtrMap() { return m_mapPipelineLayout; }

    public:
        void Destroy();

    public:
        bool HasPipelineLayout(const String& strName);
        VulkanPipelineLayout* GetPipelineLayout(const String& strName);
        bool AddPipelineLayout(VulkanPipelineLayout* pPipelineLayout);
        VulkanPipelineLayout* CreatePipelineLayout(const String& namePipelineLayout,
                                                   const VkDescriptorSetLayoutVector& aDescriptorSetLayout);
        void DeletePipelineLayout(const String& strName);
        void DeletePipelineLayout(VulkanPipelineLayout* pPipelineLayout);
        void DeletePipelineLayoutAll();
    };

}; //LostPeterPluginRendererVulkan

#endif