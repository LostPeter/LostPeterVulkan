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

#ifndef _VULKAN_FRAME_BUFFER_MANAGER_H_
#define _VULKAN_FRAME_BUFFER_MANAGER_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanFrameBufferManager : public FSingleton<VulkanFrameBufferManager>
    {
    public:
        VulkanFrameBufferManager(VulkanDevice* pDevice);
        ~VulkanFrameBufferManager();

    public:
    protected:
        VulkanDevice* m_pDevice;
        VulkanFrameBufferPtrVector m_aFrameBuffer;
        VulkanFrameBufferPtrMap m_mapFrameBuffer;

    public:
        static VulkanFrameBufferManager& GetSingleton();
		static VulkanFrameBufferManager* GetSingletonPtr();

    public:
        F_FORCEINLINE VulkanDevice* GetDevice() const { return m_pDevice; }
        F_FORCEINLINE const VulkanFrameBufferPtrVector& GetFrameBufferPtrVector() const { return m_aFrameBuffer; }
        F_FORCEINLINE VulkanFrameBufferPtrVector& GetFrameBufferPtrVector() { return m_aFrameBuffer; }
        F_FORCEINLINE const VulkanFrameBufferPtrMap& GetFrameBufferPtrMap() const { return m_mapFrameBuffer; }
        F_FORCEINLINE VulkanFrameBufferPtrMap& GetFrameBufferPtrMap() { return m_mapFrameBuffer; }

    public:
        void Destroy();

    public:
        bool HasFrameBuffer(const String& strName);
        VulkanFrameBuffer* GetFrameBuffer(const String& strName);
        bool AddFrameBuffer(VulkanFrameBuffer* pFrameBuffer);
        VulkanFrameBuffer* CreateFrameBuffer(const String& nameFrameBuffer,
                                             const VkImageViewVector& aImageView, 
                                             VkRenderPass vkRenderPass,
                                             VkFramebufferCreateFlags flags,
                                             uint32_t nWidth,
                                             uint32_t nHeight,
                                             uint32_t layers);

        void DeleteFrameBuffer(const String& strName);
        void DeleteFrameBuffer(VulkanFrameBuffer* pFrameBuffer);
        void DeleteFrameBufferAll();

    };

}; //LostPeterPluginRendererVulkan

#endif