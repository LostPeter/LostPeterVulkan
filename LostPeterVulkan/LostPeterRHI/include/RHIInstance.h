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

#ifndef _RHI_INSTANCE_H_
#define _RHI_INSTANCE_H_

#include "RHIPreDefine.h"

namespace LostPeterRHI
{
    class rhiExport RHIInstance : public FNonCopyable
    {
    public:
        RHIInstance();
        virtual ~RHIInstance();

    public:
    protected:


    public:
        virtual void Destroy() = 0;

        virtual RHIType GetRHIType() = 0;
        virtual uint32_t GetPhysicalDeviceCount() = 0;
        virtual RHIPhysicalDevice* GetPhysicalDevice(uint32 nIndex) = 0;
        virtual RHIDevice* RequestDevice(const RHIDeviceCreateInfo& createInfo) = 0;
        
    };

}; //LostPeterRHI

#endif

