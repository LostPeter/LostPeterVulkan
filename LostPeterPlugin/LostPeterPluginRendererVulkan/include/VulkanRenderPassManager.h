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

#ifndef _VULKAN_RENDER_PASS_MANAGER_H_
#define _VULKAN_RENDER_PASS_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanRenderPassManager : public FSingleton<VulkanRenderPassManager>
    {
    public:
        VulkanRenderPassManager(VulkanDevice* pDevice);
        virtual ~VulkanRenderPassManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanRenderPassPtrVector m_aRenderPass;
        VulkanRenderPassPtrMap m_mapRenderPass;

     public:
        static VulkanRenderPassManager& GetSingleton();
		static VulkanRenderPassManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanRenderPassPtrVector& GetRenderPassPtrVector() const { return m_aRenderPass; }
        F_FORCEINLINE VulkanRenderPassPtrVector& GetRenderPassPtrVector() { return m_aRenderPass; }
        F_FORCEINLINE const VulkanRenderPassPtrMap& GetRenderPassPtrMap() const { return m_mapRenderPass; }
        F_FORCEINLINE VulkanRenderPassPtrMap& GetRenderPassPtrMap() { return m_mapRenderPass; }

    public:
        void Destroy();

    public:
        bool HasRenderPass(const String& strName);
        VulkanRenderPass* GetRenderPass(const String& strName);
        bool AddRenderPass(VulkanRenderPass* pRenderPass);
        VulkanRenderPass* CreateRenderPass(const String& nameRenderPass,
                                           const VkAttachmentDescriptionVector& aAttachmentDescription,
                                           const VkSubpassDescriptionVector& aSubpassDescription,
                                           const VkSubpassDependencyVector& aSubpassDependency,
                                           VkRenderPassMultiviewCreateInfo* pMultiviewCI);
        VulkanRenderPass* CreateRenderPass_KhrDepth(const String& nameRenderPass,
                                                    VkFormat formatSwapChain, 
                                                    VkFormat formatDepth);
        VulkanRenderPass* CreateRenderPass_KhrDepthImgui(const String& nameRenderPass,
                                                         VkFormat formatColor, 
                                                         VkFormat formatDepth, 
                                                         VkFormat formatSwapChain);
        VulkanRenderPass* CreateRenderPass_ColorDepthMSAA(const String& nameRenderPass,
                                                          VkFormat formatColor, 
                                                          VkFormat formatDepth, 
                                                          VkFormat formatSwapChain, 
                                                          VkSampleCountFlagBits samples);
        VulkanRenderPass* CreateRenderPass_ColorDepthImguiMSAA(const String& nameRenderPass,
                                                               VkFormat formatColor,     
                                                               VkFormat formatDepth, 
                                                               VkFormat formatSwapChain, 
                                                               VkSampleCountFlagBits samples);
        
        void DeleteRenderPass(const String& strName);
        void DeleteRenderPass(VulkanRenderPass* pRenderPass);
        void DeleteRenderPassAll();

    };

}; //LostPeterPluginRendererVulkan

#endif