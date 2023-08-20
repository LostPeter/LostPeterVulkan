/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-08-20
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FCapsule.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
    bool FCapsule::Intersects_Point(const FVector3& point) const
	{
		
		return true;	
	}

	bool FCapsule::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayCapsule_Test(ray, *this, true);
	}
    
}; //LostPeterFoundation