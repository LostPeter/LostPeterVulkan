/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_VIEWPORT_H_
#define _VULKAN_VIEWPORT_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanViewport
    {
    public:
        VulkanViewport();
        ~VulkanViewport();

    public:
    protected:
        Viewport* m_pViewport;

        VkViewport m_vkViewport;
        VkOffset2D m_vkOffset2D;
        VkExtent2D m_vkExtent2D;
        VkRect2D m_vkScissor;

    public:
        F_FORCEINLINE Viewport* GetViewport() const { return m_pViewport; }
        F_FORCEINLINE const VkViewport& GetVkViewport() const { return m_vkViewport; }
        F_FORCEINLINE const VkOffset2D& GetVkOffset2D() const { return m_vkOffset2D; }
        F_FORCEINLINE const VkExtent2D& GetVkExtent2D() const { return m_vkExtent2D; }
        F_FORCEINLINE const VkRect2D& GetScissor() const { return m_vkScissor; }

    public:
        void Destroy();
        bool Init(Viewport* pViewport);

    public:
        void UpdateViewport();
    };  

}; //LostPeterPluginRendererVulkan

#endif