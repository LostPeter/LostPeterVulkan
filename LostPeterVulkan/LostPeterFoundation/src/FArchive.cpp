/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FArchive.h"

namespace LostPeterFoundation
{
    FArchive::FArchive(const String& strName, const String& strType)
        : m_strName(strName)
        , m_strType(strType) 
        , m_bIsReadOnly(true)
    {

    }

    FArchive::~FArchive()
    {

    }
    
}; //LostPeterFoundation