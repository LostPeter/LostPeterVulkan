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
		return FMath::Normalize(m_pt1 - m_pt0);
	}

	float FSegment::Length() const 
	{ 
		return FMath::Distance(m_pt1, m_pt0);
	}

	float FSegment::Length2() const	
	{ 
		return FMath::Distance2(m_pt1, m_pt0);
	}

	float FSegment::Distance2(const FVector3& point, float* t /*= 0*/) const
	{
		FVector3 Diff = point - m_pt0;
		FVector3 Dir = m_pt1 - m_pt0;
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

	//Point - InLineSameSide/NotInLineSameSide
	bool FSegment::IsPoint2InSameSide(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::Points2_InLineSameSide(pt1, pt2, m_pt0, m_pt1);
	}
	bool FSegment::IsPoint2NotInSameSide(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::Points2_NotInLineSameSide(pt1, pt2, m_pt0, m_pt1);
	}

	//Line - Line Parallel/NotParallel
	bool FSegment::IsLineLineParallel(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::LineLine_Parallel(pt1, pt2, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLineParallel(const FSegment& segment)
	{
		return FMath::LineLine_Parallel(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLineNotParallel(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::LineLine_NotParallel(pt1, pt2, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLineNotParallel(const FSegment& segment)
	{
		return FMath::LineLine_NotParallel(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1);
	}

	//Line - Line Perpendicular/NotPerpendicular
	bool FSegment::IsLineLinePerpendicular(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::LineLine_Perpendicular(pt1, pt2, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLinePerpendicular(const FSegment& segment)
	{
		return FMath::LineLine_Perpendicular(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLineNotPerpendicular(const FVector3& pt1, const FVector3& pt2)
	{
		return FMath::LineLine_NotPerpendicular(pt1, pt2, m_pt0, m_pt1);
	}
	bool FSegment::IsLineLineNotPerpendicular(const FSegment& segment)
	{
		return FMath::LineLine_NotPerpendicular(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1);
	}

	//Line - Line Intersect/NotIntersect
	bool FSegment::IsLineLineIntersect(const FVector3& pt1, const FVector3& pt2, float fEpsilon, bool includeBorder /*= true*/)
	{
		return FMath::LineLine_Intersect(m_pt0, m_pt1, pt1, pt2, includeBorder, fEpsilon);
	}
	bool FSegment::IsLineLineIntersect(const FSegment& segment, float fEpsilon, bool includeBorder /*= true*/)
	{
		return FMath::LineLine_Intersect(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1, includeBorder, fEpsilon);
	}
	bool FSegment::IsLineLineNotIntersect(const FVector3& pt1, const FVector3& pt2, float fEpsilon, bool includeBorder /*= true*/)
	{
		return FMath::LineLine_NotIntersect(m_pt0, m_pt1, pt1, pt2, includeBorder, fEpsilon);
	}
	bool FSegment::IsLineLineNotIntersect(const FSegment& segment, float fEpsilon, bool includeBorder /*= true*/)
	{
		return FMath::LineLine_NotIntersect(segment.m_pt0, segment.m_pt1, m_pt0, m_pt1, includeBorder, fEpsilon);
	}

	//Line - Plane Parallel/NotParallel
	bool FSegment::IsLinePlaneParallel(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
	{
		return FMath::LinePlane_Parallel(m_pt0, m_pt1, pt1, pt2, pt3);
	}
	bool FSegment::IsLinePlaneParallel(const FPlane& plane)
	{
		return FMath::LinePlane_Parallel(*this, plane);
	}
	bool FSegment::IsLinePlaneNotParallel(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
	{
		return FMath::LinePlane_NotParallel(m_pt0, m_pt1, pt1, pt2, pt3);
	}
	bool FSegment::IsLinePlaneNotParallel(const FPlane& plane)
	{
		return FMath::LinePlane_NotParallel(*this, plane);
	}

	//Line - Plane Perpendicular/NotPerpendicular
	bool FSegment::IsLinePlanePerpendicular(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
	{
		return FMath::LinePlane_Perpendicular(m_pt0, m_pt1, pt1, pt2, pt3);
	}
	bool FSegment::IsLinePlanePerpendicular(const FPlane& plane)
	{
		return FMath::LinePlane_Perpendicular(*this, plane);
	}
	bool FSegment::IsLinePlaneNotPerpendicular(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
	{
		return FMath::LinePlane_NotPerpendicular(m_pt0, m_pt1, pt1, pt2, pt3);
	}
	bool FSegment::IsLinePlaneNotPerpendicular(const FPlane& plane)
	{
		return FMath::LinePlane_NotPerpendicular(*this, plane);
	}

	//Line - Triangle Intersect/NotIntersect
	bool FSegment::IsLineTriangleIntersect(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
	{
		return FMath::LineTriangle_Intersect(m_pt0, m_pt1, pt1, pt2, pt3, includeBorder);
	}
	bool FSegment::IsLineTriangleNotIntersect(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder /*= true*/)
	{
		return FMath::LineTriangle_NotIntersect(m_pt0, m_pt1, pt1, pt2, pt3, includeBorder);
	}


	//Intersects
    bool FSegment::Intersects_Point(const FVector3& point) const
	{
		return FMath::Intersects_PointInLine(point, *this, true);
	}
	bool FSegment::Intersects_Ray(const FRay& ray) const
	{
		return FMath::Intersects_RaySegment_Test(ray, *this);
	}
    
}; //LostPeterFoundation