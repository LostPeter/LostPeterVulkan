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

#include "../include/RHIDevice.h"
#include "../include/RHIPhysicalDevice.h"
#include "../include/RHIObject.h"
#include "../include/RHIObjectManager.h"

namespace LostPeterRHI
{
    RHIDevice::RHIDevice(RHIPhysicalDevice* pPhysicalDevice, const RHIDeviceCreateInfo& createInfo)
        : m_pPhysicalDevice(pPhysicalDevice)
        , m_pObjectManager(nullptr)
    {
        F_Assert(m_pPhysicalDevice && "RHIDevice::RHIDevice")
    }

    RHIDevice::~RHIDevice()
    {
        
    }

    void RHIDevice::DestroyObject(RHIObject* pObject)
    {
        m_pObjectManager->DestroyObject(pObject);
    }
    
    void RHIDevice::destroyPixelFormatInfos()
    {
        size_t count = m_aPixelFormatInfos.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(m_aPixelFormatInfos[i]);
        }
        m_aPixelFormatInfos.clear();
        m_mapPixelFormatInfos.clear();
        m_mapPixelFormatInfosOriginal.clear();
    }

}; //LostPeterRHI