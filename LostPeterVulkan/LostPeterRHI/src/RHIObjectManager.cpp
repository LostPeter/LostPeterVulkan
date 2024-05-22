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

#include "../include/RHIObjectManager.h"
#include "../include/RHIDevice.h"
#include "../include/RHIObject.h"

template<> LostPeterRHI::RHIObjectManager* LostPeterFoundation::FSingleton<LostPeterRHI::RHIObjectManager>::ms_Singleton = nullptr;

namespace LostPeterRHI
{
    RHIObjectManager* RHIObjectManager::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RHIObjectManager& RHIObjectManager::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RHIObjectManager::GetSingleton")
		return (*ms_Singleton);     
	}

    
    RHIObjectManager::RHIObjectManager(RHIDevice* pDevice)
        : m_pDevice(pDevice)
    {
        F_Assert(m_pDevice && "RHIObjectManager::RHIObjectManager")
    }

    RHIObjectManager::~RHIObjectManager()
    {
        
    }
    

}; //LostPeterRHI