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
    class LPF_Export FSegment
	{
	public:
		LP_FORCEINLINE FSegment()
		{

		}
		LP_FORCEINLINE FSegment(const FVector3& p0, const FVector3& p1)
			: m_P0(p0)
			, m_P1(p1)
		{

		}
		LP_FORCEINLINE FSegment(const FSegment& src)
			: m_P0(src.m_P0)
			, m_P1(src.m_P1)
		{

		}

	public:
		FVector3 m_P0;		
		FVector3 m_P1;	

	public:
		LP_FORCEINLINE const FVector3& GetOrigin() const { return m_P0; }
		LP_FORCEINLINE void	GetPoint(FVector3& pt, float t) const
		{
			pt = m_P0 + t * (m_P1 - m_P0);		
		}

		LP_FORCEINLINE FVector3 GetDirection() const { return m_P1 - m_P0; }
		LP_FORCEINLINE void	 GetDirection(FVector3& dir) const	{ dir = m_P1 - m_P0; }
		FVector3 GetDirectionNormalized() const;
		LP_FORCEINLINE void	SetOriginDirection(const FVector3& origin, const FVector3& direction)
		{
			m_P0 = m_P1 = origin;
			m_P1 += direction;
		}

		float Length() const;
		float Length2() const;

		float Distance2(const FVector3& point, float* t = 0) const;
		float Distance(const FVector3& point, float* t = 0) const;

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif