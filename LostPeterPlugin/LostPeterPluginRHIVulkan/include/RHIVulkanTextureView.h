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

#ifndef _RHI_VULKAN_TEXTURE_VIEW_H_
#define _RHI_VULKAN_TEXTURE_VIEW_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanTextureView : public RHITextureView
    {
    public:
        RHIVulkanTextureView(const RHITextureViewCreateInfo& createInfo);
        virtual ~RHIVulkanTextureView();

    public:
    protected:


    public:
        virtual void Destroy();

    };

}; //LostPeterPluginRHIVulkan

#endif