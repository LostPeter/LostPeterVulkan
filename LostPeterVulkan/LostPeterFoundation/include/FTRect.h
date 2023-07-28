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

#ifndef _F_T_RECT_H_
#define _F_T_RECT_H_

#include "FTPoint.h"

namespace LostPeterFoundation
{
    template <class T>
	class FTRect
	{
	public:
		FTRect() 
            : left(T(0))
            , top(T(0))
            , right(T(0))
            , bottom(T(0))
		{

		}
		FTRect(const T& _left, const T& _top, const T& _right, const T& _bottom)
            : left(_left)
            , top(_top)
            , right(_right)
            , bottom(_bottom)
		{ 
			
		}
		FTRect(const FTRect& rc)
            : left(rc.left)
            , top(rc.top)
            , right(rc.right)
            , bottom(rc.bottom)
		{ 
			
		}
        ~FTRect() 
		{

		}

	public:		
		T left;
		T top;
		T right;
		T bottom;

	public:	
		friend bool operator !=(const FTRect& rc1, const FTRect& rc2) 
		{ 
			return rc1.left != rc2.left || rc1.top != rc2.top || rc1.right != rc2.right || rc1.bottom != rc2.bottom; 
		}

		friend bool operator ==(const FTRect& rc1, const FTRect& rc2) 
		{ 
			return rc1.left == rc2.left && rc1.top == rc2.top && rc1.right == rc2.right && rc1.bottom == rc2.bottom;
		}

		friend FTRect operator +(const FTRect& rc1, const FTRect& rc2) 
		{ 
			return FTRect(rc1.left + rc2.left, rc1.top + rc2.top, rc1.right + rc2.right, rc1.bottom + rc2.bottom); 
		}

		friend FTRect operator -(const FTRect& rc1, const FTRect& rc2) 
		{
			return FTRect(rc1.left - rc2.left, rc1.top - rc2.top, rc1.right - rc2.right, rc1.bottom - rc2.bottom); 
		}

		friend FTRect operator +(const FTRect& rc1, const FTPoint<T>& pt)
		{ 
			return FTRect(rc1.left + pt.x, rc1.top + pt.y, rc1.right + pt.x, rc1.bottom + pt.y); 
		}

		friend FTRect operator -(const FTRect& rc1, const FTPoint<T>& pt) 
		{ 
			return FTRect(rc1.left - pt.x, rc1.top - pt.y, rc1.right - pt.x, rc1.bottom - pt.y); 
		}

		const FTRect& operator &=(const FTRect& rc) 
		{ 
			*this = *this & rc; 
			return *this;
		}

		const FTRect& operator |=(const FTRect& rc) 
		{ 
			*this = *this | rc; 
			return *this; 
		}

		FTRect operator +() const 
		{
			return *this; 
		}

		FTRect operator -() const 
		{
			return FTRect(-left, -top, -right, -bottom); 
		}

		const FTRect& operator =(const FTRect& rc) 
		{ 
			left = rc.left; 
			top	= rc.top; 
			right = rc.right; 
			bottom = rc.bottom; 
			return *this; 
		}

		const FTRect& operator +=(const FTRect& rc) 
		{ 
			left += rc.left; 
			top	+= rc.top; 
			right += rc.right;
			bottom += rc.bottom; 
			return *this; 
		}

		const FTRect& operator -=(const FTRect& rc) 
		{ 
			left -= rc.left; 
			top	-= rc.top; 
			right -= rc.right;
			bottom -= rc.bottom; 
			return *this; 
		}

		const FTRect& operator +=(const FTPoint<T>& pt) 
		{ 
			left += pt.x; 
			top	+= pt.y; 
			right += pt.x;
			bottom += pt.y; 
			return *this; 
		}

		const FTRect& operator -=(const FTPoint<T>& pt)
		{ 
			left -= pt.x; 
			top	-= pt.y;
			right -= pt.x; 
			bottom -= pt.y; 
			return *this; 
		}

		T Width() const
		{ 
			return right - left;
		}

		T Height() const
		{ 
			return bottom - top;
		}

		FTPoint<T> CenterPoint() const 
		{ 
			return FTPoint<T>((left + right) / 2, (top + bottom) / 2); 
		}

		void SetRect(const T& _left, const T& _top, const T& _right, const T& _bottom)
		{ 
			left = _left; 
			top	= _top;
			right = _right;
			bottom = _bottom; 
		}

		bool PtInRect(const T& x, const T& y) const 
		{ 
			return (x >= left && x < right && y >= top && y < bottom) ? true : false; 
		}

		bool PtInRect(const FTPoint<T>& pt) const 
		{ 
			return PtInRect(pt.x, pt.y); 
		}

		void Normalize()
		{
			if (left > right)
			{
                T temp = right;
                right = left;
                left = temp;
            }

			if (top > bottom) 
            {
                T temp = top;
                top = bottom;
                bottom = temp;
            }
		}

		bool IsRectNull() const 
		{ 
			return (left == 0 && top == 0 && right == 0 && bottom == 0); 
		}

		bool IsEmpty() const 
		{ 
			return (Width() == 0 || Height() == 0); 
		}

		void Clear()
		{ 
			left = top = right = bottom = 0; 
		}

		void Deflate(const T& x, const T& y) 
		{ 
			left += x; 
			top	+= y; 
			right -= x; 
			bottom -= y; 
		}

		void Deflate(const FTRect<T>& rc) 
		{ 
			left += rc.left; 
			top	+= rc.top; 
			right -= rc.right; 
			bottom -= rc.bottom; 
		}

		void Deflate(const T& l, const T& t, const T& r, const T& b) 
		{ 
			left += l; 
			top	+= t; 
			right -= r; 
			bottom -= b;
		}

		void Inflate(const T& x, const T& y) 
		{ 
			left -= x; 
			top	-= y; 
			right += x;
			bottom += y;
		}

		void Inflate(const FTRect& rc)
		{ 
			left -= rc.left; 
			top	-= rc.top;
			right += rc.right;
			bottom += rc.bottom; 
		}

		void Inflate(const T& l, const T& t, const T& r, const T& b) 
		{ 
			left -= l; 
			top	-= t; 
			right += r; 
			bottom += b;
		}

		void Offset(const T& x, const T& y) 
		{ 
			left += x;
			top	+= y;
			right += x; 
			bottom += y; 
		}

		void Offset(const FTPoint<T>& pt)
		{ 
			*this += pt; 
		}

		void SetUnion(const FTRect<T>& rc1, const FTRect<T>& rc2) 
		{ 
			*this = rc1 | rc2; 
		}

		void SetIntersect(const FTRect<T>& rc1, const FTRect<T>& rc2) 
		{ 
			*this = rc1 & rc2;
		}
	};

	template <class T>
	FTRect<T> operator & (const FTRect<T>& rc1, const FTRect<T>& rc2)
	{
		if (rc1.IsEmpty() || rc2.IsEmpty())
			return FTRect<T>(0, 0, 0, 0);

		if (rc1.left >= rc2.right || rc2.left >= rc1.right ||
			rc1.top >= rc2.bottom || rc2.top >= rc1.bottom)
			return FTRect<T>(0, 0, 0, 0);

		return FTRect<T>(rc1.left > rc2.left ? rc1.left : rc2.left,
                         rc1.top > rc2.top ? rc1.top : rc2.top,
                         rc1.right < rc2.right ? rc1.right : rc2.right,
                         rc1.bottom < rc2.bottom ? rc1.bottom : rc2.bottom);
	}

	template <class T>
	FTRect<T> operator | (const FTRect<T>& rc1, const FTRect<T>& rc2)
	{
		if (rc1.IsEmpty())
			return rc2;

		if (rc2.IsEmpty())
			return rc1;

		return FTRect<T>(rc1.left < rc2.left ? rc1.left : rc2.left,
                         rc1.top < rc2.top ? rc1.top : rc2.top,
                         rc1.right > rc2.right ? rc1.right : rc2.right,
                         rc1.bottom > rc2.bottom ? rc1.bottom : rc2.bottom);
	}

}; //LostPeterFoundation

#endif