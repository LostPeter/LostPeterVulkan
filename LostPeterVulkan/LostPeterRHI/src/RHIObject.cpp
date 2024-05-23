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
#include "../include/RHIObjectManager.h"
#include "../include/RHIDevice.h"

namespace LostPeterRHI
{
    RHIObject::RHIObject(RHIDevice* pDevice)
        : m_pDevice(pDevice)
        , m_eObject(RHIObjectType::RHI_Object_UnKnown)
        , m_nID(RHIObjectManager::GetNextID())
        , m_strName("")
    {
        F_Assert(m_pDevice && "RHIObject::RHIObject")
    }

    RHIObject::~RHIObject()
    {
        
    }
    
    void RHIObject::setObjectType(RHIObjectType eObject)
    {
        m_eObject = eObject;
        m_strName = RHIObjectManager::GetAutoName(m_eObject, m_nID);
        RHIObjectManager::GetSingleton().AddObject(this);
    }

}; //LostPeterRHI