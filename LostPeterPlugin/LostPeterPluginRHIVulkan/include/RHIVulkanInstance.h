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

#ifndef _RHI_VULKAN_INSTANCE_H_
#define _RHI_VULKAN_INSTANCE_H_

#include "RHIVulkanPreDefine.h"

namespace LostPeterPluginRHIVulkan
{
    class rhiVulkanExport RHIVulkanInstance : public RHIInstance
    {
    public:
        RHIVulkanInstance();
        virtual ~RHIVulkanInstance();

    public:
    protected:
        RHIVulkanPhysicalDevice* m_pPhysicalDevice;

    public:
        virtual void Destroy();

        virtual RHIType GetRHIType();
        virtual uint32_t GetPhysicalDeviceCount();
        virtual RHIPhysicalDevice* GetPhysicalDevice(uint32 nIndex);
    };

}; //LostPeterPluginRHIVulkan

#endif

