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
	FVector3 FCylinder::GetCenter() const
	{
		return (m_vCenterTop + m_vCenterBottom) / 2.0f;
	}
	FVector3 FCylinder::GetDirectionNormalized() const
	{
		return FMath::Normalize(m_vCenterTop - m_vCenterBottom);
	}
	float FCylinder::GetHeight() const
	{
		return FMath::Distance(m_vCenterTop, m_vCenterBottom);
	}

    bool FCylinder::Intersects_Point(const FVector3& point) const
	{
		float fDistance = FMath::GetDistanceFromPointLine(point, m_vCenterBottom, m_vCenterTop);
		if (fDistance > m_fRadius)
		{
			return false;
		}
		float fHeight = GetHeight();
		FVector3 vDir = GetDirectionNormalized();
		FVector3 vCenter = GetCenter();
		float fDot = FMath::Dot((point - vCenter), vDir);
		if (FMath::Abs(fDot) > fHeight / 2.0f)
			return false;
		return true;	
	}

	bool FCylinder::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayCylinder_Test(ray, *this, true);
	}
    
}; //LostPeterFoundation