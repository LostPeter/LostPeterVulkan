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

#include "../include/RHIInstance.h"

template<> LostPeterRHI::RHIInstance* LostPeterFoundation::FSingleton<LostPeterRHI::RHIInstance>::ms_Singleton = nullptr;

namespace LostPeterRHI
{
    RHIInstance* RHIInstance::GetSingletonPtr()
	{
		return ms_Singleton;
	}
	RHIInstance& RHIInstance::GetSingleton()
	{  
		F_Assert(ms_Singleton && "RHIInstance::GetSingleton")
		return (*ms_Singleton);     
	}


    RHIInstance::RHIInstance()
    {

    }

    RHIInstance::~RHIInstance()
    {

    }


}; //LostPeterRHI

