/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _RHI_SEMAPHORE_H_
#define _RHI_SEMAPHORE_H_

#include "RHIObject.h"

namespace LostPeterRHI
{
    class rhiExport RHISemaphore : public RHIObject
    {
    public:
        RHISemaphore(RHIDevice* pDevice, const RHISemaphoreCreateInfo& createInfo);
        virtual ~RHISemaphore();

    public:
    protected:


    public:
        

    };

}; //LostPeterRHI

#endif