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

#ifndef _VULKAN_MATH_BOX_H_
#define _VULKAN_MATH_BOX_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanMathBox
    {
    public:
        UTIL_FORCEINLINE VulkanMathBox()
			: m_nLeft(0)
			, m_nTop(0)
			, m_nRight(1)
			, m_nBottom(1)
			, m_nFront(0)
			, m_nBack(1)
		{

		}
		UTIL_FORCEINLINE VulkanMathBox(size_t l, size_t t, size_t r, size_t b)
			: m_nLeft(l)
			, m_nTop(t)
			, m_nRight(r)
			, m_nBottom(b)
			, m_nFront(0)
			, m_nBack(1)
		{
			assert(m_nRight >= m_nLeft && m_nBottom >= m_nTop && m_nBack >= m_nFront && "VulkanMathBox::VulkanMathBox");
		}
		UTIL_FORCEINLINE VulkanMathBox(size_t l, size_t t, size_t ff, size_t r, size_t b, size_t bb)
			: m_nLeft(l)
			, m_nTop(t)
			, m_nRight(r)
			, m_nBottom(b)
			, m_nFront(ff)
			, m_nBack(bb)
		{
			assert(m_nRight >= m_nLeft && m_nBottom >= m_nTop && m_nBack >= m_nFront && "VulkanMathBox::VulkanMathBox");
		}
        virtual ~VulkanMathBox()
        {
            
        }

    public:
        size_t m_nLeft;			
		size_t m_nTop;				
		size_t m_nRight;			
		size_t m_nBottom;			
		size_t m_nFront;			
		size_t m_nBack;

    public:
		UTIL_FORCEINLINE size_t GetWidth() const { return m_nRight - m_nLeft; }
		UTIL_FORCEINLINE size_t GetHeight() const { return m_nBottom - m_nTop; }
		UTIL_FORCEINLINE size_t GetDepth() const { return m_nBack - m_nFront; }

    public:
        bool Contains(const VulkanMathBox &def) const;
    };

}; //LostPeter

#endif