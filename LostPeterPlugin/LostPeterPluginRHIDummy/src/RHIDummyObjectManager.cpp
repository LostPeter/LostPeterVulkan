/****************************************************************************
* LostPeterPluginRHIDummy - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-05-22
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/RHIDummyObjectManager.h"
#include "../include/RHIDummyDevice.h"

namespace LostPeterPluginRHIDummy
{
    RHIDummyObjectManager::RHIDummyObjectManager(RHIDummyDevice* pDummyDevice)
        : RHIObjectManager(pDummyDevice)
        , m_pDummyDevice(pDummyDevice)
    {
        F_Assert(m_pDummyDevice && "RHIDummyObjectManager::RHIDummyObjectManager")
    }

    RHIDummyObjectManager::~RHIDummyObjectManager()
    {
        
    }
    
    void RHIDummyObjectManager::Destroy()
    {

    }


}; //LostPeterPluginRHIDummy