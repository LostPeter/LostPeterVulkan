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

#include "../include/RHIVulkanTexture.h"
#include "../include/RHIVulkanTextureView.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanTexture::RHIVulkanTexture(const RHITextureCreateInfo& createInfo)
        : RHITexture(createInfo)
    {

    }

    RHIVulkanTexture::~RHIVulkanTexture()
    {
        Destroy();
    }

    void RHIVulkanTexture::Destroy()
    {

    }

    RHITextureView* RHIVulkanTexture::CreateTextureView(const RHITextureViewCreateInfo& createInfo)
    {
        return new RHIVulkanTextureView(createInfo);
    }
    
}; //LostPeterPluginRHIVulkan