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

#ifndef _VULKAN_PIXEL_FORMAT_H_
#define _VULKAN_PIXEL_FORMAT_H_

#include "PreDefine.h"
#include "VulkanPixelBox.h"

namespace LostPeter
{
    class VulkanPixelFormat
    {
    private:
		static VulkanPixelFormatDes ms_PixelFormats[Vulkan_PixelFormat_Count];
		
	public:
		static const VulkanPixelFormatDes& GetPixelFormatDes(const VulkanPixelFormatType& format);
		static VulkanPixelFormatDes& GetPixelFormatDesRef(VulkanPixelFormatType format);
        static const String& GetPixelFormatName(VulkanPixelFormatType format);
		static size_t GetPixelFormatElemBytes(VulkanPixelFormatType format);
		static size_t GetPixelFormatElemBits(VulkanPixelFormatType format);
        static uint32 GetPixelFormatFlags(VulkanPixelFormatType format);
        static VulkanPixelFormatComponentType GetPixelFormatComponentType(VulkanPixelFormatType format);
		static size_t GetPixelFormatComponentCount(VulkanPixelFormatType format);
        static void	GetPixelFormatBitDepths(VulkanPixelFormatType format, int32 rgba[4]);
		static void	GetPixelFormatBitMasks(VulkanPixelFormatType format, uint64 rgba[4]);
		static void	GetPixelFormatBitShifts(VulkanPixelFormatType format, uint8 rgba[4]);
		static size_t GetPixelFormatMemorySize(size_t width, size_t height, size_t depth, VulkanPixelFormatType format);

		static bool	IsNative(VulkanPixelFormatType format);
		static bool	IsCompressed(VulkanPixelFormatType format);
		static bool IsInteger(VulkanPixelFormatType format);
		static bool	IsFloat(VulkanPixelFormatType format);
		static bool	IsLuminance(VulkanPixelFormatType format);
		static bool	IsStencil(VulkanPixelFormatType format);
		static bool	IsDepth(VulkanPixelFormatType format);
		static bool	IsDepthStencil(VulkanPixelFormatType format);
		static bool	HasAlpha(VulkanPixelFormatType format);
       
		static bool	IsValidExtent(size_t width, size_t height, size_t depth, VulkanPixelFormatType format);
		static bool	IsAccessible(VulkanPixelFormatType format);

    public:
		static VulkanPixelFormatType ParsePixelFormatFromName(const String& strName, bool accessibleOnly = false, bool caseSensitive = false);
		static VulkanPixelFormatType ParsePixelFormatForBitDepths(VulkanPixelFormatType format, uint16 integerBits, uint16 floatBits);

		static void	PackColor(const FColor &color, const VulkanPixelFormatType format, void* dest);
		static void	PackColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a, const VulkanPixelFormatType format, void* dest);
		static void	PackColor(const float r, const float g, const float b, const float a, const VulkanPixelFormatType format, void* dest);

		static void UnpackColor(FColor& color, VulkanPixelFormatType format, const void* src);
		static void	UnpackColor(FColor* color, VulkanPixelFormatType format, const void* src);
		static void	UnpackColor(uint8* r, uint8* g, uint8* b, uint8* a, VulkanPixelFormatType format, const void* src);
		static void	UnpackColor(float* r, float* g, float* b, float* a, VulkanPixelFormatType format, const void* src); 

		static void	BulkPixelConversion(void* src, VulkanPixelFormatType srcFormat, void* dest, VulkanPixelFormatType dstFormat, uint32 count);
		static void	BulkPixelConversion(const VulkanPixelBox& src, const VulkanPixelBox& dst);
		static void BulkPixelVerticalFlip(const VulkanPixelBox& box);
    };

}; //LostPeter

#endif