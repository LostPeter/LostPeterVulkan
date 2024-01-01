/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_FACTORY_H_
#define _F_FACTORY_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    template <typename T>
	class FFactory
	{
	public:
		virtual ~FFactory();

	public:
	private:


    public:
        virtual const String& GetType() const = 0;
        virtual T* CreateInstance(const String& strName) = 0;
        virtual void DestroyInstance(T* pInstance) = 0;
	};	

}; //LostPeterFoundation

#endif