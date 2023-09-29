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

#include "../include/FCone.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
	FVector3 FCone::GetRealTop() const
	{
		if (m_fRadiusTop == 0.0f)
			return m_vTop;

		//tan(a) = rb / x = ra / (x + h)
		//x = rb*h / (ra - rb)
		float x = m_fRadiusTop * m_fHeight / (m_fRadius - m_fRadiusTop);
		return m_vCenter + FMath::Normalize(m_vTop - m_vCenter) * (m_fHeight + x);
	}

    bool FCone::Intersects_Point(const FVector3& point) const
	{
		float fDistance = FMath::GetDistanceFromPointLine(point, m_vCenter, m_vTop);
		if (fDistance < m_fRadiusTop || fDistance > m_fRadius)
		{
			return false;
		}
		
		FVector3 vTop = GetRealTop();
		float fLen2 = FMath::Distance2(vTop, point) - fDistance * fDistance;
		float fHeight = FMath::Distance2(vTop, m_vCenter);
		if (fDistance * fDistance / fLen2 > m_fRadius * m_fRadius / fHeight)
		{
			return false;
		}

		return true;	
	}

	bool FCone::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayCone_Test(ray, *this, true);
	}
    
}; //LostPeterFoundation