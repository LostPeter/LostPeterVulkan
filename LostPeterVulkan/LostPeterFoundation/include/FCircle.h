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

#ifndef _F_CIRCLE_H_
#define _F_CIRCLE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FCircle
	{
	public:
		LP_FORCEINLINE FCircle()
			: m_vNormal(0, 1, 0)
			, m_fDistance(0)
			, m_vCenter(0, 0, 0)
			, m_fRadius(1)
		{

		}
		LP_FORCEINLINE FCircle(const FVector3& vNormal, float fDistance, const FVector3& vCenter, float fRadius)
			: m_vNormal(vNormal)
			, m_fDistance(fDistance)
			, m_vCenter(vCenter)
			, m_fRadius(fRadius)
		{

		}

	public:
		FVector3 m_vNormal;			
        float m_fDistance;		
		FVector3 m_vCenter;
		float m_fRadius;

	public:
		LP_FORCEINLINE const FVector3& GetNormal() const { return m_vNormal; }
        LP_FORCEINLINE FVector3& GetNormal() { return m_vNormal; }
        LP_FORCEINLINE float GetDistance() const { return m_fDistance; }
		LP_FORCEINLINE const FVector3& GetCenter() const { return m_vCenter; }
        LP_FORCEINLINE FVector3& GetCenter() { return m_vCenter; }
		LP_FORCEINLINE float GetRadius() const { return m_fRadius; }

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif