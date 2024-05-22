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

#include "../include/RHIObject.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIObject::RHIObject(RHIDevice* pDevice)
        : m_pDevice(pDevice)
        , m_eObject(RHIObjectType::RHI_Object_UnKnown)
    {
        F_Assert(m_pDevice && "RHIObject::RHIObject")
    }

    RHIObject::~RHIObject()
    {
        
    }
    

}; //LostPeterRHI