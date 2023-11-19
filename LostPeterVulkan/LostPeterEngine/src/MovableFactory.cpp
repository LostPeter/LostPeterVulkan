/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-11
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/MovableFactory.h"
#include "../include/Movable.h"

namespace LostPeter
{
    MovableFactory::MovableFactory()
        : m_nTypeFlag(0xFFFFFFFF)
		, m_bDelExtern(false)
    {

    }

    MovableFactory::~MovableFactory()
    {

    }

    Movable* MovableFactory::CreateInstance(const String& strName, const String2StringMap* pParams /*= nullptr*/)
	{
		Movable* pMovable = createInstanceImpl(strName, pParams);
		pMovable->SetMovableFactory(this);

		return pMovable;
	}
    
}; //LostPeter