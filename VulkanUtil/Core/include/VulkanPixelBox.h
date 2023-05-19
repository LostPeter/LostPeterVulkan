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

#ifndef _VULKAN_PIXEL_BOX_H_
#define _VULKAN_PIXEL_BOX_H_

#include "VulkanMathBox.h"

namespace LostPeter
{
    class VulkanPixelBox : public VulkanMathBox
    {
    public:
        VulkanPixelBox();
        VulkanPixelBox(const VulkanMathBox& extents, VulkanPixelFormatType ePixelFormat, void* pPixelData = nullptr);
		VulkanPixelBox(size_t width, size_t height, size_t depth, VulkanPixelFormatType ePixelFormat, void* pPixelData = nullptr);
        virtual ~VulkanPixelBox();

    public:
        uint8* m_pData;			
		VulkanPixelFormatType m_ePixelFormat;		
		size_t m_nRowPitch;		
		size_t m_nSlicePitch;
        
    public:
        UTIL_FORCEINLINE size_t GetRowSkip() const
		{ 
			return m_nRowPitch - GetWidth();
		}

		UTIL_FORCEINLINE size_t GetSliceSkip() const 
		{ 
			return m_nSlicePitch - (GetHeight() * m_nRowPitch); 
		}

        UTIL_FORCEINLINE bool IsConsecutive() const 
		{ 
			return m_nRowPitch == GetWidth() && m_nSlicePitch == GetWidth() * GetHeight(); 
		}
        UTIL_FORCEINLINE void SetConsecutive()
		{
			m_nRowPitch = GetWidth();
			m_nSlicePitch = GetWidth() * GetHeight();
		}

    public:
		size_t GetConsecutiveSize() const;
		VulkanPixelBox GetSubVolume(const VulkanMathBox& def, bool resetOrigin = true) const;
		uint8* GetTopLeftFrontPixelPtr() const;

		Color GetColorAt(size_t x, size_t y, size_t z) const;
		void SetColorAt(size_t x, size_t y, size_t z, const Color &cv);
    };

}; //LostPeter

#endif