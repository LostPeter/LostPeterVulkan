/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-09-02
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FQuad.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
    bool FQuad::Intersects_Point(const FVector3& point) const
	{
		
		return true;	
	}

	bool FQuad::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayQuad_Test(ray, *this);
	}
    
}; //LostPeterFoundation