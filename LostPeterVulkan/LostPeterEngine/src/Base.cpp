/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/Base.h"

namespace LostPeterEngine
{
    Base::Base(const String& strName)
        : m_nGroup(0)
        , m_strName(strName)
        , m_nRefCount(0)
        , m_bIsInit(false)
    {
       
    }
    Base::Base(uint32 nGroup, const String& strName)
        : m_nGroup(nGroup)
        , m_strName(strName)
        , m_nRefCount(0)
        , m_bIsInit(false)
    {

    }
    Base::~Base()
    {
        
    }

    bool Base::IsGroupNameValid() const
    {
        if (m_strName.empty() || m_nGroup <= 0)
			return false;
        return true;
    }

}; //LostPeterEngine