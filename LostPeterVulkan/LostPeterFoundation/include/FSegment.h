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

#ifndef _F_SEGMENT_H_
#define _F_SEGMENT_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FSegment
	{
	public:
		F_FORCEINLINE FSegment()
		{

		}
		F_FORCEINLINE FSegment(const FVector3& p0, const FVector3& p1)
			: m_pt0(p0)
			, m_pt1(p1)
		{

		}
		F_FORCEINLINE FSegment(const FSegment& src)
			: m_pt0(src.m_pt0)
			, m_pt1(src.m_pt1)
		{

		}

	public:
		FVector3 m_pt0;		
		FVector3 m_pt1;	

	public:
		F_FORCEINLINE const FVector3& GetOrigin() const { return m_pt0; }
		F_FORCEINLINE void	GetPoint(FVector3& pt, float t) const
		{
			pt = m_pt0 + t * (m_pt1 - m_pt0);		
		}

		F_FORCEINLINE FVector3 GetDirection() const { return m_pt1 - m_pt0; }
		F_FORCEINLINE void	 GetDirection(FVector3& dir) const	{ dir = m_pt1 - m_pt0; }
		FVector3 GetDirectionNormalized() const;
		F_FORCEINLINE void	SetOriginDirection(const FVector3& origin, const FVector3& direction)
		{
			m_pt0 = m_pt1 = origin;
			m_pt1 += direction;
		}

		float Length() const;
		float Length2() const;

		float Distance2(const FVector3& point, float* t = 0) const;
		float Distance(const FVector3& point, float* t = 0) const;

		//Point - InLineSameSide/NotInLineSameSide
		bool IsPoint2InSameSide(const FVector3& pt1, const FVector3& pt2);
		bool IsPoint2NotInSameSide(const FVector3& pt1, const FVector3& pt2);

		//Line - Line Parallel/NotParallel
		bool IsLineLineParallel(const FVector3& pt1, const FVector3& pt2);
		bool IsLineLineParallel(const FSegment& segment);
		bool IsLineLineNotParallel(const FVector3& pt1, const FVector3& pt2);
		bool IsLineLineNotParallel(const FSegment& segment);

		//Line - Line Perpendicular/NotPerpendicular
		bool IsLineLinePerpendicular(const FVector3& pt1, const FVector3& pt2);
		bool IsLineLinePerpendicular(const FSegment& segment);
		bool IsLineLineNotPerpendicular(const FVector3& pt1, const FVector3& pt2);
		bool IsLineLineNotPerpendicular(const FSegment& segment);

		//Line - Line Intersect/NotIntersect
		bool IsLineLineIntersect(const FVector3& pt1, const FVector3& pt2, float fEpsilon, bool includeBorder = true);
		bool IsLineLineIntersect(const FSegment& segment, float fEpsilon, bool includeBorder = true);
		bool IsLineLineNotIntersect(const FVector3& pt1, const FVector3& pt2, float fEpsilon, bool includeBorder = true);
		bool IsLineLineNotIntersect(const FSegment& segment, float fEpsilon, bool includeBorder = true);

		//Line - Plane Parallel/NotParallel
		bool IsLinePlaneParallel(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
		bool IsLinePlaneParallel(const FPlane& plane);
		bool IsLinePlaneNotParallel(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
		bool IsLinePlaneNotParallel(const FPlane& plane);

		//Line - Plane Perpendicular/NotPerpendicular
		bool IsLinePlanePerpendicular(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
		bool IsLinePlanePerpendicular(const FPlane& plane);
		bool IsLinePlaneNotPerpendicular(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3);
		bool IsLinePlaneNotPerpendicular(const FPlane& plane);

		//Line - Triangle Intersect/NotIntersect
		bool IsLineTriangleIntersect(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true);
		bool IsLineTriangleNotIntersect(const FVector3& pt1, const FVector3& pt2, const FVector3& pt3, bool includeBorder = true);

	public:
		//Intersects
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif