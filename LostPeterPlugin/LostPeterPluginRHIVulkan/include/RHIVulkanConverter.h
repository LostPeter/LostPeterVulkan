/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-15
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_VULKAN_CONVERTER_H_
#define _RHI_VULKAN_CONVERTER_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanConverter 
    {
    public:
        ////////////////////// TransformFromXXXX //////////////////////
        static RHIPhysicalDeviceType TransformFromVkPhysicalDeviceType(VkPhysicalDeviceType ePhysicalDevice);

        static RHIPixelFormatType TransformFromVkFormat(VkFormat eFormat);

    public:
        ////////////////////// TransformToXXXX ////////////////////////
        static VkPhysicalDeviceType TransformToVkPhysicalDeviceType(RHIPhysicalDeviceType ePhysicalDevice);

        static VkFormat TransformToVkFormat(RHIPixelFormatType ePixelFormat);
    };

}; //LostPeterPluginRHIVulkan

#endif