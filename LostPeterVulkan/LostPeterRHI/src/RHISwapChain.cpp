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

#include "../include/RHISwapChain.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHISwapChain::RHISwapChain(RHIDevice* pDevice, const RHISwapChainCreateInfo& createInfo)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHISwapChain::RHISwapChain")
    }

    RHISwapChain::~RHISwapChain()
    {
        
    }   
    

}; //LostPeterRHI