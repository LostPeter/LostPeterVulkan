/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_DUMMY_PHYSICAL_DEVICE_H_
#define _RHI_DUMMY_PHYSICAL_DEVICE_H_

#include "RHIDummyPreDefine.h"

namespace LostPeterPluginRHIDummy
{
    class rhiDummyExport RHIDummyPhysicalDevice : public RHIPhysicalDevice
    {
    public:
        RHIDummyPhysicalDevice();
        virtual ~RHIDummyPhysicalDevice();

    public:
    protected:


    public:
        virtual RHIPhysicalDeviceProperty GetPhysicalDeviceProperty();
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo);
    };
    

}; //LostPeterPluginRHIDummy

#endif