/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_CYLINDER_H_
#define _F_CYLINDER_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FCylinder
	{
	public:
		F_FORCEINLINE FCylinder()
			: m_vCenterTop(0, 1, 0)
			, m_vCenterBottom(0, 0, 0)
			, m_fRadius(0.5f)
		{
			Refresh();
		}
		F_FORCEINLINE FCylinder(const FVector3& vCenterTop, const FVector3& vCenterBottom, float fRadius)
			: m_vCenterTop(vCenterTop)
			, m_vCenterBottom(vCenterBottom)
			, m_fRadius(fRadius)
		{
			Refresh();
		}
		F_FORCEINLINE FCylinder(const FCylinder& cylinder) 
			: m_vCenterTop(cylinder.m_vCenterTop)
			, m_vCenterBottom(cylinder.m_vCenterBottom)
			, m_fRadius(cylinder.m_fRadius)
		{
			Refresh();
		}

	public:
		FVector3 m_vCenterTop;
		FVector3 m_vCenterBottom;
		float m_fRadius;		

		FVector3 m_vCenter;
		FVector3 m_vDirection;
		float m_fHeight;

	public:
		F_FORCEINLINE const FVector3& GetCenterTop() const { return m_vCenterTop; }
        F_FORCEINLINE FVector3& GetCenterTop() { return m_vCenterTop; } 
		F_FORCEINLINE const FVector3& GetCenterBottom() const { return m_vCenterBottom; }
        F_FORCEINLINE FVector3& GetCenterBottom() { return m_vCenterBottom; } 
        F_FORCEINLINE float GetRadius() const { return m_fRadius; }

		FVector3 GetCenter() const;
		F_FORCEINLINE FVector3 GetDirection() const { return m_vCenterTop - m_vCenterBottom; }
		FVector3 GetDirectionNormalized() const;
		float GetHeight() const;

	public:
		void Refresh()
		{
			this->m_vCenter = GetCenter();
			this->m_vDirection = GetDirectionNormalized();
			this->m_fHeight = GetHeight();
		}

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif