/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_SWAP_CHAIN_H_
#define _RHI_VULKAN_SWAP_CHAIN_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanSwapChain : public RHISwapChain
    {
    public:
        RHIVulkanSwapChain(const RHISwapChainCreateInfo& createInfo);
        virtual ~RHIVulkanSwapChain();

    public:
    protected:
        RHIVulkanTexturePtrVector m_aVulkanTextures;
        bool m_bIsPingPong;

    public:
        virtual void Destroy();

        virtual RHITexture* GetTexture(uint8 nIndex);
        virtual uint8 AcquireBackTexture();
        virtual void Present();
    };
    
}; //LostPeterPluginRHIVulkan

#endif