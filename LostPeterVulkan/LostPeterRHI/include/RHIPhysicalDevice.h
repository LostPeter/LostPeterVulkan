/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-05
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_PHYSICAL_DEVICE_H_
#define _RHI_PHYSICAL_DEVICE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIPhysicalDevice : public FNonCopyable
    {
    public:
        RHIPhysicalDevice();
        virtual ~RHIPhysicalDevice();

    public:
    protected:


    public:
        virtual RHIPhysicalDeviceProperty GetPhysicalDeviceProperty() = 0;
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo) = 0;
    };
    
}; //LostPeterRHI

#endif