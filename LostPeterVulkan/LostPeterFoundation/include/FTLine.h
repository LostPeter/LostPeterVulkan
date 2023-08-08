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

#ifndef _F_T_LINE_H_
#define _F_T_LINE_H_

#include "FTPoint.h"

namespace LostPeterFoundation
{
    template <class T>
	class FTLine
	{
	public:
		FTLine() 
            : m_ptStart(T(0), T(0))
            , m_ptEnd(T(0), T(0))
		{
			
		}
		FTLine(const FTPoint<T>& ptStart,const FTPoint<T>& ptEnd)
            : m_ptStart(ptStart)
            , m_ptEnd(ptEnd)
		{
			
		}
		FTLine(const FTLine& line)
            : m_ptStart(line.m_ptStart)
            , m_ptEnd(line.m_ptEnd)
		{
			
		}
		~FTLine() 
		{

		}

	public:
		FTPoint<T>	m_ptStart;
		FTPoint<T>	m_ptEnd;

	public:
		T Min(T x, T y) const
		{
			if (x <= y)
				return x;
			return y;
		}

		T Max(T x, T y) const
		{
			if (x >= y)
				return x;
			return y;
		}

        bool PtInLine(const FTPoint<T>& pt) const 
		{
			if ((pt.x - m_ptStart.x) * (m_ptEnd.y - m_ptStart.y) == (pt.y - m_ptStart.y) * (m_ptEnd.x - m_ptStart.x)
				&& Min(m_ptStart.x, m_ptEnd.x) <= pt.x && pt.x <= Max(m_ptStart.x, m_ptEnd.x)
				&& Min(m_ptStart.y, m_ptEnd.y) <= pt.y && pt.y <= Max(m_ptStart.y, m_ptEnd.y))
				return true;
			return false;
		}
	};

}; //LostPeterFoundation

#endif