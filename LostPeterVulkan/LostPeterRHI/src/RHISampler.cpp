/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-09
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHISampler.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHISampler::RHISampler(RHIDevice* pDevice, const RHISamplerCreateInfo& createInfo)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHISampler::RHISampler")
    }

    RHISampler::~RHISampler()
    {
        
    }
    

}; //LostPeterRHI