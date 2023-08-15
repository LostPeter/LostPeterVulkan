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

#include "../include/FCylinder.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
    bool FCylinder::Intersects_Point(const FVector3& point) const
	{
		if (FMath::Abs(point.y) > m_fHalfLenY ||
			FMath::Abs(point.x) > m_fRadius ||
			FMath::Abs(point.z) > m_fRadius)
		{
            return false;
        }

		return true;	
	}
    
}; //LostPeterFoundation