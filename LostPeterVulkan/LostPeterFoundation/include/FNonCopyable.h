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

#ifndef _F_NON_COPYABLE_H_
#define _F_NON_COPYABLE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FNonCopyable
	{
	public:
		FNonCopyable();
		~FNonCopyable();

	public:
	private:
		FNonCopyable(const FNonCopyable&);
		const FNonCopyable& operator=(const FNonCopyable&);
	};	

}; //LostPeterFoundation

#endif