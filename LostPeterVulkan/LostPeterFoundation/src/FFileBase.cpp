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

#include "../include/FFileBase.h"

namespace LostPeterFoundation
{
    bool FFileBase::PutC(unsigned char c)
	{
		size_t nWrote = Write(&c,1,1);
		return nWrote == 1 ? true: false;
	}
    
}; //LostPeterFoundation