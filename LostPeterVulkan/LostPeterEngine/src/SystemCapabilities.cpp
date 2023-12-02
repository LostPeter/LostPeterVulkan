/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/SystemCapabilities.h"

template<> LostPeterEngine::SystemCapabilities* LostPeterFoundation::FSingleton<LostPeterEngine::SystemCapabilities>::ms_Singleton = nullptr;

namespace LostPeterEngine
{
    SystemCapabilities* SystemCapabilities::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	SystemCapabilities& SystemCapabilities::GetSingleton()
	{  
		F_Assert(ms_Singleton && "SystemCapabilities::GetSingleton")
		return (*ms_Singleton);     
	}


    SystemCapabilities::SystemCapabilities()
        : Base("SystemCapabilities")
    {

    }

    SystemCapabilities::~SystemCapabilities()
    {
        Destroy();
    }

    void SystemCapabilities::Destroy()
    {

    }

    void SystemCapabilities::Init()
    {

    }


}; //LostPeterEngine