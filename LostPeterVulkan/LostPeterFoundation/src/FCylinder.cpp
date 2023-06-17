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
    bool FCylinder::IsPointIn(const FVector3& vPos)
	{
		if (FMath::Abs(vPos.y) > m_fHalfLenY ||
			FMath::Abs(vPos.x) > m_fRadius ||
			FMath::Abs(vPos.z) > m_fRadius)
		{
            return false;
        }

		return true;	
	}
    
}; //LostPeterFoundation