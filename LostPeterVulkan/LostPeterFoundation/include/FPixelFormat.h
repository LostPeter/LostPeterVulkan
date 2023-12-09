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

#ifndef _F_PIXEL_FORMAT_H_
#define _F_PIXEL_FORMAT_H_

#include "FPixelBox.h"

namespace LostPeterFoundation
{
    class foundationExport FPixelFormat
    {
    private:
		static FPixelFormatDes ms_PixelFormats[F_PixelFormat_Count];
		
	public:
		static const FPixelFormatDes& GetPixelFormatDes(const FPixelFormatType& format);
		static FPixelFormatDes& GetPixelFormatDesRef(FPixelFormatType format);
        static const String& GetPixelFormatName(FPixelFormatType format);
		static size_t GetPixelFormatElemBytes(FPixelFormatType format);
		static size_t GetPixelFormatElemBits(FPixelFormatType format);
        static uint32 GetPixelFormatFlags(FPixelFormatType format);
        static FPixelFormatComponentType GetPixelFormatComponentType(FPixelFormatType format);
		static size_t GetPixelFormatComponentCount(FPixelFormatType format);
        static void	GetPixelFormatBitDepths(FPixelFormatType format, int32 rgba[4]);
		static void	GetPixelFormatBitMasks(FPixelFormatType format, uint64 rgba[4]);
		static void	GetPixelFormatBitShifts(FPixelFormatType format, uint8 rgba[4]);
		static size_t GetPixelFormatMemorySize(size_t width, size_t height, size_t depth, FPixelFormatType format);

		static bool	IsNative(FPixelFormatType format);
		static bool	IsCompressed(FPixelFormatType format);
		static bool IsInteger(FPixelFormatType format);
		static bool	IsFloat(FPixelFormatType format);
		static bool	IsLuminance(FPixelFormatType format);
		static bool	IsStencil(FPixelFormatType format);
		static bool	IsDepth(FPixelFormatType format);
		static bool	IsDepthStencil(FPixelFormatType format);
		static bool	HasAlpha(FPixelFormatType format);
       
		static bool	IsValidExtent(size_t width, size_t height, size_t depth, FPixelFormatType format);
		static bool	IsAccessible(FPixelFormatType format);

    public:
		static FPixelFormatType ParsePixelFormatFromName(const String& strName, bool accessibleOnly = false, bool caseSensitive = false);
		static FPixelFormatType ParsePixelFormatForBitDepths(FPixelFormatType format, uint16 integerBits, uint16 floatBits);

		static void	PackColor(const FColor &color, const FPixelFormatType format, void* dest);
		static void	PackColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a, const FPixelFormatType format, void* dest);
		static void	PackColor(const float r, const float g, const float b, const float a, const FPixelFormatType format, void* dest);

		static void UnpackColor(FColor& color, FPixelFormatType format, const void* src);
		static void	UnpackColor(FColor* color, FPixelFormatType format, const void* src);
		static void	UnpackColor(uint8* r, uint8* g, uint8* b, uint8* a, FPixelFormatType format, const void* src);
		static void	UnpackColor(float* r, float* g, float* b, float* a, FPixelFormatType format, const void* src); 

		static void	BulkPixelConversion(void* src, FPixelFormatType srcFormat, void* dest, FPixelFormatType dstFormat, uint32 count);
		static void	BulkPixelConversion(const FPixelBox& src, const FPixelBox& dst);
		static void BulkPixelVerticalFlip(const FPixelBox& box);
    };

}; //LostPeterFoundation

#endif