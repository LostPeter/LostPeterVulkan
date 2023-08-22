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

#include "../include/FSegment.h"
#include "../include/FMath.h"

namespace LostPeterFoundation
{
	FVector3 FSegment::GetDirectionNormalized() const
	{
		return FMath::Normalize(m_P1 - m_P0);
	}

	float FSegment::Length() const 
	{ 
		return FMath::Distance(m_P1, m_P0);
	}

	float FSegment::Length2() const	
	{ 
		return FMath::Distance2(m_P1, m_P0);
	}

	float FSegment::Distance2(const FVector3& point, float* t /*= 0*/) const
	{
		FVector3 Diff = point - m_P0;
		FVector3 Dir = m_P1 - m_P0;
		float fT = FMath::Dot(Diff, Dir);

		if(fT <= 0.0f)
		{
			fT = 0.0f;
		}
		else
		{
			float SqrLen= FMath::Length2(Dir);
			if(fT >= SqrLen)
			{
				fT = 1.0f;
				Diff -= Dir;
			}
			else
			{
				fT /= SqrLen;
				Diff -= fT*Dir;
			}
		}

		if(t)	
		{
			*t = fT;
		}

		return FMath::Length2(Diff);
	}

	float FSegment::Distance(const FVector3& point, float* t /*= 0*/) const			
	{ 
		return FMath::Sqrt(Distance2(point, t));
	}

    bool FSegment::Intersects_Point(const FVector3& point) const
	{
		return FMath::Intersects_PointInLine(point, *this, true);
	}

	bool FSegment::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RaySegment_Test(ray, *this);
	}
    
}; //LostPeterFoundation