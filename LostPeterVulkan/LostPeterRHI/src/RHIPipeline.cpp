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

#include "../include/RHIPipeline.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIPipeline::RHIPipeline(RHIDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHIPipeline::RHIPipeline")
    }

    RHIPipeline::~RHIPipeline()
    {
        
    }
    

}; //LostPeterRHI