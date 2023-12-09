/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-11-21
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_PIXEL_BOX_H_
#define _F_PIXEL_BOX_H_

#include "FBox.h"

namespace LostPeterFoundation
{
    class foundationExport FPixelBox : public FBox
    {
    public:
        FPixelBox();
        FPixelBox(const FBox& extents, FPixelFormatType ePixelFormat, void* pPixelData = nullptr);
		FPixelBox(size_t width, size_t height, size_t depth, FPixelFormatType ePixelFormat, void* pPixelData = nullptr);
        virtual ~FPixelBox();

    public:
        uint8* m_pData;			
		FPixelFormatType m_ePixelFormat;		
		size_t m_nRowPitch;		
		size_t m_nSlicePitch;
        
    public:
        F_FORCEINLINE size_t GetRowSkip() const
		{ 
			return m_nRowPitch - GetWidth();
		}

		F_FORCEINLINE size_t GetSliceSkip() const 
		{ 
			return m_nSlicePitch - (GetHeight() * m_nRowPitch); 
		}

        F_FORCEINLINE bool IsConsecutive() const 
		{ 
			return m_nRowPitch == GetWidth() && m_nSlicePitch == GetWidth() * GetHeight(); 
		}
        F_FORCEINLINE void SetConsecutive()
		{
			m_nRowPitch = GetWidth();
			m_nSlicePitch = GetWidth() * GetHeight();
		}

    public:
		size_t GetConsecutiveSize() const;
		FPixelBox GetSubVolume(const FBox& def, bool resetOrigin = true) const;
		uint8* GetTopLeftFrontPixelPtr() const;

		FColor GetColorAt(size_t x, size_t y, size_t z) const;
		void SetColorAt(size_t x, size_t y, size_t z, const FColor &cv);
    };

}; //LostPeterFoundation

#endif