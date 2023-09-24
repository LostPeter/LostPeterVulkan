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
	FVector3 FCapsule::GetCenter() const
	{
		return (m_vCenterTop + m_vCenterBottom) / 2.0f;
	}
	FVector3 FCapsule::GetDirectionNormalized() const
	{
		return FMath::Normalize(m_vCenterTop - m_vCenterBottom);
	}
	float FCapsule::GetHeight() const
	{
		return FMath::Distance(m_vCenterTop, m_vCenterBottom);
	}

    bool FCapsule::Intersects_Point(const FVector3& point) const
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
		{
			if (fDot > 0)
			{
				if (FMath::Distance(point, m_vCenterTop) <= m_fRadius)
					return true;
			}
			else if (fDot < 0)
			{
				if (FMath::Distance(point, m_vCenterBottom) <= m_fRadius)
					return true;
			}
			return false;
		}
		return true;	
	}

	bool FCapsule::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayCapsule_Test(ray, *this, true);
	}
    
}; //LostPeterFoundation