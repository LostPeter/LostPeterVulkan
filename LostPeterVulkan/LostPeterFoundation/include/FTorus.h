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

#ifndef _F_TORUS_H_
#define _F_TORUS_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FTorus
	{
	public:
		LP_FORCEINLINE FTorus()
		{

		}
		LP_FORCEINLINE FTorus(const FVector3& vCenter, const FVector3& vDirection, float fRadius, float fSectionRadius)
			: m_vCenter(vCenter)
			, m_vDirection(vDirection)
			, m_fRadius(fRadius)
			, m_fSectionRadius(fSectionRadius)
		{

		}
		LP_FORCEINLINE FTorus(const FTorus& torus) 
			: m_vCenter(torus.m_vCenter)
			, m_vDirection(torus.m_vDirection)
			, m_fRadius(torus.m_fRadius)
			, m_fSectionRadius(torus.m_fSectionRadius)
		{
			
		}

	public:
		FVector3 m_vCenter;
		FVector3 m_vDirection;
		float m_fRadius;		
		float m_fSectionRadius;
		
	public:
		LP_FORCEINLINE const FVector3& GetCenter() const { return m_vCenter; }
		LP_FORCEINLINE const FVector3& GetDirection() const { return m_vDirection; }
        LP_FORCEINLINE float GetRadius() const { return m_fRadius; }
		LP_FORCEINLINE float GetSectionRadius() const { return m_fSectionRadius; }

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif