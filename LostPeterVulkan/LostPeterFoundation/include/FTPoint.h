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

#ifndef _F_T_POINT_H_
#define _F_T_POINT_H_

namespace LostPeterFoundation
{
    template <class T>
	class FTPoint
	{
	public:		
		FTPoint() 
		{

		}
		FTPoint(const T& _x, const T& _y)
            : x(_x)
            , y(_y)
		{ 
			
		}
		FTPoint(const FTPoint& pt) 
            : x(pt.x)
            , y(pt.y)
		{ 
            
		}

	public:		
		T x;
		T y;

	public:		
		void Offset(const T& ox, const T& oy)
		{ 
			x += ox; 
			y += oy;
		}

		void Set(const T& _x, const T& _y) 
		{ 
			x = _x; 
			y = _y; 
		}

		friend bool operator !=(const FTPoint& pt1, const FTPoint& pt2) 
		{ 
			return pt1.x != pt2.x || pt1.y != pt2.y; 
		}

		friend bool operator ==(const FTPoint& pt1, const FTPoint& pt2) 
		{ 
			return pt1.x == pt2.x && pt1.y == pt2.y; 
		}

		friend FTPoint operator +(const FTPoint& pt1, const FTPoint& pt2) 
		{
			return FTPoint(pt1.x + pt2.x, pt1.y + pt2.y); 
		}

		friend FTPoint operator -(const FTPoint& pt1, const FTPoint& pt2) 
		{ 
			return FTPoint(pt1.x - pt2.x, pt1.y - pt2.y); 
		}

		FTPoint operator +() const 
		{
			return *this;
		}

		FTPoint operator -() const 
		{
			return FTPoint(-x, -y); 
		}

		const FTPoint& operator =(const FTPoint& pt) 
		{
			x = pt.x; y = pt.y; 
			return *this; 
		}

		const FTPoint& operator +=(const FTPoint& pt) 
		{ 
			x += pt.x; 
			y += pt.y; 
			return *this; 
		}

		const FTPoint& operator -=(const FTPoint& pt) 
		{
			x -= pt.x; 
			y -= pt.y; 
			return *this;
		}
	};

}; //LostPeterFoundation

#endif