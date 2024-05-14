/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-14
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_VOLK_H_
#define _RHI_VULKAN_VOLK_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class RHIVulkanVolk
    {
    public:
        RHIVulkanVolk();
        virtual ~RHIVulkanVolk();

    public:
        virtual bool VolkInitialize();
        virtual void VolkLoadInstanceOnly(VkInstance vkInstance);
        virtual void VolkLoadDevice(VkDevice vkDevice);
    };

}; //LostPeterPluginRHIVulkan

#endif