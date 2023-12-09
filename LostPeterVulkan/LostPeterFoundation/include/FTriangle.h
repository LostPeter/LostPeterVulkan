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

#ifndef _F_TRIANGLE_H_
#define _F_TRIANGLE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FTriangle
	{
	public:
		F_FORCEINLINE FTriangle()
			: m_pt0(-1, 0, 0)
			, m_pt1( 0, 1, 0)
			, m_pt2( 1, 0, 0)
		{

		}
		F_FORCEINLINE FTriangle(const FVector3& pt0, const FVector3& pt1, const FVector3& pt2)
			: m_pt0(pt0)
			, m_pt1(pt1)
			, m_pt2(pt2)
		{

		}

	public:
		FVector3 m_pt0;
		FVector3 m_pt1;
		FVector3 m_pt2;

	public:
		bool Intersects_Point(const FVector3& point) const;
		bool Intersects_Ray(const FRay& ray) const;
	};

}; //LostPeterFoundation

#endif