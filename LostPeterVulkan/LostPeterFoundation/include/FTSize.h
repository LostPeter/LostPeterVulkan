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

#ifndef _F_T_SIZE_H_
#define _F_T_SIZE_H_

#include "FTPoint.h"

namespace LostPeterFoundation
{
    template <class T>
	class FTSize
	{
	public:
		FTSize()
            : x(T(0))
            , y(T(0))
		{
			
		}
		FTSize(const T& _x, const T& _y)
            : x(_x)
            , y(_y)
		{

		}
		FTSize(const FTSize& size)
            : x(size.x)
            , y(size.y)
		{

		}
		FTSize(const FTPoint<T>& pt)
            : x(pt.x)
            , y(pt.y)
		{
			
		}
		~FTSize() 
		{

		}

	public:
		T x;
		T y;

	public:
		void SetSize(T& _x, T& _y) 
		{
			x = _x;
			y = _y;	
		} 

	public:
		FTSize operator +(const FTSize& size)
		{
			return FTSize(x + size.x, y + size.y);
		}

		FTSize operator -(const FTSize& size)
		{
			return FTSize(x - size.x, y - size.y);
		}

		bool operator ==(const FTSize& size)
		{
			return (x == size.x && y == size.y);
		}

		bool operator !=(const FTSize& size)
		{
			return (x != size.x || y != size.y);
		}

		void operator +=(const FTSize& size)
		{
			x += size.x;
			y += size.y;
		}

		void operator -=(const FTSize& size)
		{
			x -= size.x;
			y -= size.y;
		}
	};

}; //LostPeterFoundation

#endif