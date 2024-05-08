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

#include "../include/RHIPreDefine.h"

namespace LostPeterRHI
{
    ////////////////////////////// Enum ////////////////////////////////
    //RHIType
    static const String s_nameRHITypes[] = 
    {
        "RHI-Dummy",                    //0: RHI-Dummy
        "RHI-Vulkan",                   //1: RHI-Vulkan
        "RHI-DirectX12",                //2: RHI-DirectX12
        "RHI-Metal",                    //3: RHI-Metal
    };
    const String& RHI_GetRHITypeName(RHIType type)
    {
        return s_nameRHITypes[(int)type];
    }
    const String& RHI_GetRHITypeName(int type)
    {
        return s_nameRHITypes[(int)type];
    }
    RHIType RHI_ParseRHIType(const String& strName)
    {
        for (int i = 0; i < (int)RHIType::RHI_Count; i++)
        {
            if (s_nameRHITypes[i] == strName)
                return (RHIType)(i);
        }
        F_Assert(false && "RHI_ParseRHIType: Wrong type name !")
        return RHIType::RHI_Vulkan;
    }

    

}; //LostPeterRHI