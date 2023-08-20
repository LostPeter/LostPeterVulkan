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
    class LPF_Export FCylinder
	{
	public:
		LP_FORCEINLINE FCylinder()
		{

		}
		LP_FORCEINLINE FCylinder(FVector3& vCenter, FVector3& vX, FVector3& vY, FVector3& vZ, float fHalfHeight, float fRad)
			: m_vCenter(vCenter)
			, m_vAxisX(vX)
			, m_vAxisY(vY)
			, m_vAxisZ(vZ)
			, m_fHalfLenY(fHalfHeight)
			, m_fRadius(fRad)
		{

		}
		LP_FORCEINLINE FCylinder(const FCylinder& src) 
			: m_vCenter(src.m_vCenter),
			m_vAxisX(src.m_vAxisX),
			m_vAxisY(src.m_vAxisY),
			m_vAxisZ(src.m_vAxisZ)
		{
			m_fHalfLenY	= src.m_fHalfLenY;
			m_fRadius	= src.m_fRadius;
		}

	public:
		FVector3 m_vCenter;		
		FVector3 m_vAxisX;		
		FVector3 m_vAxisY;		
		FVector3 m_vAxisZ;		
		float m_fHalfLenY;	
		float m_fRadius;		

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif