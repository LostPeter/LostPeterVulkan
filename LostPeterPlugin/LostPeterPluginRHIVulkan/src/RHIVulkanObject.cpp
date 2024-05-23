/****************************************************************************
* LostPeterPluginRHIVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-23
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIVulkanObject.h"
#include "../include/RHIVulkanDevice.h"

namespace LostPeterPluginRHIVulkan
{
    RHIVulkanObject::RHIVulkanObject(RHIVulkanDevice* pVulkanDevice)
        : m_pVulkanDevice(pVulkanDevice)
    {
        F_Assert(m_pVulkanDevice && "RHIVulkanObject::RHIVulkanObject")
    }   

    RHIVulkanObject::~RHIVulkanObject()
    {
        
    }


}; //LostPeterPluginRHIVulkan