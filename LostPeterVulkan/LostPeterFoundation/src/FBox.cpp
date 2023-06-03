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

#include "../include/FPreInclude.h"
#include "../include/FBox.h"

namespace LostPeterFoundation
{
    bool FBox::Contains(const FBox& box) const
	{
		return (box.m_nLeft >= m_nLeft && 
                box.m_nTop >= m_nTop && 
                box.m_nFront >= m_nFront &&
			    box.m_nRight <= m_nRight && 
                box.m_nBottom <= m_nBottom && 
                box.m_nBack <= m_nBack);
	}
    
}; //LostPeterFoundation