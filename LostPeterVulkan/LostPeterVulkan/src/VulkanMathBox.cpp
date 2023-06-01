/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-05-19
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanMathBox.h"

namespace LostPeter
{
    bool VulkanMathBox::Contains(const VulkanMathBox &def) const
	{
		return (def.m_nLeft >= m_nLeft && 
                def.m_nTop >= m_nTop && 
                def.m_nFront >= m_nFront &&
			    def.m_nRight <= m_nRight && 
                def.m_nBottom <= m_nBottom && 
                def.m_nBack <= m_nBack);
	}

}; //LostPeter