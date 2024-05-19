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

#include "../include/RHISurface.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHISurface::RHISurface(RHIDevice* pDevice, const RHISurfaceCreateInfo& createInfo)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHISurface::RHISurface")
    }

    RHISurface::~RHISurface()
    {
        
    }   
    

}; //LostPeterRHI