/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-18
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHICommandPool.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHICommandPool::RHICommandPool(RHIDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHICommandPool::RHICommandPool")
    }

    RHICommandPool::~RHICommandPool()
    {
        
    }  
    

}; //LostPeterRHI