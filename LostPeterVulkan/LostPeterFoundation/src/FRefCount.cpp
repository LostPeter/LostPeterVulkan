/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FRefCount.h"

namespace LostPeterFoundation
{
    FRefCount::FRefCount()
    {

    }

    FRefCount::~FRefCount()
    {
        if (m_refCounter.GetRefCount() != 0)
        {
            F_LogError("*********************** FRefCount::~FRefCount: RefCount is not 0 !");
        }
    }
    
}; //LostPeterFoundation