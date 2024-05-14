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

#ifndef _RHI_VULKAN_TEXTURE_H_
#define _RHI_VULKAN_TEXTURE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanTexture : public RHITexture
    {
    public:
        RHIVulkanTexture(const RHITextureCreateInfo& createInfo);
        virtual ~RHIVulkanTexture();

    public:
    protected:


    public:
        virtual void Destroy();

        virtual RHITextureView* CreateTextureView(const RHITextureViewCreateInfo& createInfo);
    };
    
}; //LostPeterPluginRHIVulkan

#endif