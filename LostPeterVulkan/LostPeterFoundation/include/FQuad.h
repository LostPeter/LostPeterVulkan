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

#ifndef _F_QUAD_H_
#define _F_QUAD_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FQuad
	{
	public:
		LP_FORCEINLINE FQuad()
			: m_pt0(-1, -1, 0)
			, m_pt1(-1,  1, 0)
			, m_pt2( 1,  1, 0)
			, m_pt3( 1, -1, 0)
		{

		}
		LP_FORCEINLINE FQuad(const FVector3& pt0, const FVector3& pt1, const FVector3& pt2, const FVector3& pt3)
			: m_pt0(pt0)
			, m_pt1(pt1)
			, m_pt2(pt2)
			, m_pt3(pt3)
		{

		}

	public:
		FVector3 m_pt0;
		FVector3 m_pt1;
		FVector3 m_pt2;
		FVector3 m_pt3;

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif