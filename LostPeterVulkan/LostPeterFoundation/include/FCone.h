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

#ifndef _F_CONE_H_
#define _F_CONE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FCone
	{
	public:
		LP_FORCEINLINE FCone()
			: m_vCenter(0, 0, 0)
			, m_vDirection(0, 1, 0)
			, m_fRadius(0.5f)
			, m_fHeight(1.0f)
		{

		}
		LP_FORCEINLINE FCone(const FVector3& vCenter, const FVector3& vDirection, float fRadius, float fHeight)
			: m_vCenter(vCenter)
			, m_vDirection(vDirection)
			, m_fRadius(fRadius)
			, m_fHeight(fHeight)
		{

		}
		LP_FORCEINLINE FCone(const FCone& cone) 
			: m_vCenter(cone.m_vCenter)
			, m_vDirection(cone.m_vDirection)
			, m_fRadius(cone.m_fRadius)
			, m_fHeight(cone.m_fHeight)
		{
			
		}

	public:
		FVector3 m_vCenter;
		FVector3 m_vDirection;
		float m_fRadius;		
		float m_fHeight;

	public:
		LP_FORCEINLINE const FVector3& GetCenter() const { return m_vCenter; }
		LP_FORCEINLINE const FVector3& GetDirection() const { return m_vDirection; }
        LP_FORCEINLINE float GetRadius() const { return m_fRadius; }
		LP_FORCEINLINE float GetHeight() const { return m_fHeight; }

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif