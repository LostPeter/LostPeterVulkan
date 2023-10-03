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

#include "../include/FTorus.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
    bool FTorus::Intersects_Point(const FVector3& point) const
	{
		float fDisR2 = FMath::Distance2(point, m_vCenter);
		if (fDisR2 < (m_fRadius - m_fSectionRadius) * (m_fRadius - m_fSectionRadius) ||
		    fDisR2 > (m_fRadius + m_fSectionRadius) * (m_fRadius + m_fSectionRadius))
		{
			return false;
		}

		FVector3 vD = FMath::Normalize(point - m_vCenter) - m_vDirection;
		FVector3 vR = FMath::Normalize(vD) * m_fRadius;
		float fDis2 = FMath::Distance2(vR, point);
		if (fDis2 > m_fSectionRadius * m_fSectionRadius)
		{
			return false;
		}

		return false;	
	}

	bool FTorus::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RayTorus_Test(ray, *this, true);
	}
    
}; //LostPeterFoundation