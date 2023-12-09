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
    class foundationExport FCone
	{
	public:
		F_FORCEINLINE FCone()
			: m_vCenter(0, 0, 0)
			, m_vTop(0, 1, 0)
			, m_fRadiusTop(0.0f)
			, m_fRadius(0.5f)
			, m_fHeight(1.0f)
		{

		}
		F_FORCEINLINE FCone(const FVector3& vCenter, const FVector3& vTop, float fRadius, float fHeight)
			: m_vCenter(vCenter)
			, m_vTop(vTop)
			, m_fRadiusTop(0.0f)
			, m_fRadius(fRadius)
			, m_fHeight(fHeight)
		{

		}
		F_FORCEINLINE FCone(const FVector3& vCenter, const FVector3& vTop, float fRadiusTop, float fRadius, float fHeight)
			: m_vCenter(vCenter)
			, m_vTop(vTop)
			, m_fRadiusTop(fRadiusTop)
			, m_fRadius(fRadius)
			, m_fHeight(fHeight)
		{

		}
		F_FORCEINLINE FCone(const FCone& cone) 
			: m_vCenter(cone.m_vCenter)
			, m_vTop(cone.m_vTop)
			, m_fRadiusTop(cone.m_fRadiusTop)
			, m_fRadius(cone.m_fRadius)
			, m_fHeight(cone.m_fHeight)
		{
			
		}

	public:
		FVector3 m_vCenter;
		FVector3 m_vTop;
		float m_fRadiusTop;		
		float m_fRadius;		
		float m_fHeight;

	public:
		F_FORCEINLINE const FVector3& GetCenter() const { return m_vCenter; }
		F_FORCEINLINE const FVector3& GetTop() const { return m_vTop; }
		F_FORCEINLINE float GetRadiusTop() const { return m_fRadiusTop; }
        F_FORCEINLINE float GetRadius() const { return m_fRadius; }
		F_FORCEINLINE float GetHeight() const { return m_fHeight; }

		F_FORCEINLINE void SetConeParam(const FVector3& vCenter, const FVector3& vTop, float fRadius, float fHeight)
		{
			m_vCenter = vCenter;
			m_vTop = vTop;
			m_fRadiusTop = 0.0f;
			m_fRadius = fRadius;
			m_fHeight = fHeight;
		}

		FVector3 GetRealTop() const;

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif