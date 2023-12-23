/****************************************************************************
* LostPeterPluginRendererVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_VOLK_H_
#define _VULKAN_VOLK_H_

#include "VulkanPreDefine.h"

namespace LostPeterPluginRendererVulkan
{
    class VulkanVolk
    {
    public:
        VulkanVolk();
        virtual ~VulkanVolk();

    public:
        virtual bool VolkInitialize();
        virtual void VolkLoadInstanceOnly(VkInstance vkInstance);
        virtual void VolkLoadDevice(VkDevice vkDevice);
    };

}; //LostPeterPluginRendererVulkan

#endif