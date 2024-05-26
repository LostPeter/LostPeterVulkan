/****************************************************************************
* LostPeterRHI - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-12
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIBindGroup.h"
#include "../include/RHIDevice.h"
#include "../include/RHIBindGroupPool.h"
#include "../include/RHIBindGroupLayout.h"

namespace LostPeterRHI
{
    RHIBindGroup::RHIBindGroup(RHIDevice* pDevice, const RHIBindGroupCreateInfo& createInfo)
        : RHIObject(pDevice)
        , m_pBindGroupPool(createInfo.pBindGroupPool)
        , m_pBindGroupLayout(createInfo.pBindGroupLayout)
    {
        F_Assert(m_pBindGroupPool && m_pBindGroupLayout && "RHIBindGroup::RHIBindGroup")

        setObjectType(RHIObjectType::RHI_Object_BindGroup);
    }

    RHIBindGroup::~RHIBindGroup()
    {
        
    }
    

}; //LostPeterRHI