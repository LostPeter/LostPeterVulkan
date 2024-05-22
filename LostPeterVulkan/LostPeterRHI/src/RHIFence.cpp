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

#include "../include/RHIFence.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIFence::RHIFence(RHIDevice* pDevice, const RHIFenceCreateInfo& createInfo)
        : RHIObject(pDevice)
    {
        m_eObject = RHIObjectType::RHI_Object_Fence;
    }

    RHIFence::~RHIFence()
    {
        
    }
    

}; //LostPeterRHI