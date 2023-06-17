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
#include "../include/VulkanPixelFormat.h"

namespace LostPeter
{
    VulkanPixelFormatDes VulkanPixelFormat::ms_PixelFormats[Vulkan_PixelFormat_Count] = 
	{
		//0		Vulkan_PixelFormat_Unknown
		{
			"Unknown", 0, 0, Vulkan_PixelFormatComponent_ByteU, 0, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0
		},

		//1	    Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM
		{
			"BYTE_A8R8G8B8_UNORM", 4, Vulkan_PixelFormatFlag_HasAlpha | Vulkan_PixelFormatFlag_IsNative, Vulkan_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

	};
	/////////////////////////////////PixelFormat/////////////////////////////////

	const VulkanPixelFormatDes& VulkanPixelFormat::GetPixelFormatDes(const VulkanPixelFormatType& format)
	{
		const int32 type = (int32)format;
		F_Assert(type >= Vulkan_PixelFormat_Unknown && type < Vulkan_PixelFormat_Count && "VulkanPixelFormat::GetPixelFormatDes")
		return ms_PixelFormats[type];
	}
	VulkanPixelFormatDes& VulkanPixelFormat::GetPixelFormatDesRef(VulkanPixelFormatType format)
	{
		const int32 type = (int32)format;
		F_Assert(type >= Vulkan_PixelFormat_Unknown && type < Vulkan_PixelFormat_Count && "VulkanPixelFormat::GetPixelFormatDesRef")
		return ms_PixelFormats[type];
	}

    const String& VulkanPixelFormat::GetPixelFormatName(VulkanPixelFormatType format)
	{
		return GetPixelFormatDes(format).name;
	}
	size_t VulkanPixelFormat::GetPixelFormatElemBytes(VulkanPixelFormatType format)
	{
		return GetPixelFormatDes(format).nElemBytes;
	}
	size_t VulkanPixelFormat::GetPixelFormatElemBits(VulkanPixelFormatType format)
	{
		return GetPixelFormatDes(format).nElemBytes * 8;
	}
    uint32 VulkanPixelFormat::GetPixelFormatFlags(VulkanPixelFormatType format)
	{	
		return GetPixelFormatDes(format).eFlags;
	}
    VulkanPixelFormatComponentType VulkanPixelFormat::GetPixelFormatComponentType(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormatComponentType)GetPixelFormatDes(format).eComponentType;
	}
	size_t VulkanPixelFormat::GetPixelFormatComponentCount(VulkanPixelFormatType format)
	{
		return GetPixelFormatDes(format).nComponentCount;
	}
    void VulkanPixelFormat::GetPixelFormatBitDepths(VulkanPixelFormatType format, int32 rgba[4])
	{
		const VulkanPixelFormatDes &des = GetPixelFormatDes(format);
		rgba[0] = des.nRbits;
		rgba[1] = des.nGbits;
		rgba[2] = des.nBbits;
		rgba[3] = des.nAbits;
	}
	void VulkanPixelFormat::GetPixelFormatBitMasks(VulkanPixelFormatType format, uint64 rgba[4])
	{		
		const VulkanPixelFormatDes &des = GetPixelFormatDes(format);
		rgba[0] = des.nRmask;
		rgba[1] = des.nGmask;
		rgba[2] = des.nBmask;
		rgba[3] = des.nAmask;
	}
	void VulkanPixelFormat::GetPixelFormatBitShifts(VulkanPixelFormatType format, uint8 rgba[4])
	{
		const VulkanPixelFormatDes &des = GetPixelFormatDes(format);
		rgba[0] = des.nRshift;
		rgba[1] = des.nGshift;
		rgba[2] = des.nBshift;
		rgba[3] = des.nAshift;
	}
	static size_t astc_slice_size(size_t width, size_t height, size_t blockWidth, size_t blockHeight)
    {
        return ((width + blockWidth - 1) / blockWidth) *
               ((height + blockHeight - 1) / blockHeight) * 16;
    }
	size_t VulkanPixelFormat::GetPixelFormatMemorySize(size_t width, size_t height, size_t depth, VulkanPixelFormatType format)
	{
		if (IsCompressed(format))
		{
			switch((int32)format)
			{
			// //DXT
			// case Vulkan_PixelFormat_DXT1:
			// 	return ((width + 3) / 4) * ((height + 3) / 4) * 8 * depth;
			// case Vulkan_PixelFormat_DXT2:
			// case Vulkan_PixelFormat_DXT3:
			// case Vulkan_PixelFormat_DXT4:
			// case Vulkan_PixelFormat_DXT5:
			// 	return ((width + 3) / 4) * ((height + 3) / 4) * 16 * depth;

			// case Vulkan_PixelFormat_BC4_UNORM:
			// case Vulkan_PixelFormat_BC4_SNORM:
			// 	return ((width + 3) / 4) * ((height + 3) / 4) * 8 * depth;
			// case Vulkan_PixelFormat_BC5_UNORM:
			// case Vulkan_PixelFormat_BC5_SNORM:
			// case Vulkan_PixelFormat_BC6H_UF16:
			// case Vulkan_PixelFormat_BC6H_SF16:
			// case Vulkan_PixelFormat_BC7_UNORM:
			// 	return ((width + 3) / 4) * ((height + 3) / 4) * 16 * depth;

            // //ASTC
			// case Vulkan_PixelFormat_ASTC_RGBA_4X4_LDR:
			// 	return astc_slice_size(width, height, 4, 4) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_5X4_LDR:
			// 	return astc_slice_size(width, height, 5, 4) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_5X5_LDR:
			// 	return astc_slice_size(width, height, 5, 5) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_6X5_LDR:
			// 	return astc_slice_size(width, height, 6, 5) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_6X6_LDR:
			// 	return astc_slice_size(width, height, 6, 6) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_8X5_LDR:
			// 	return astc_slice_size(width, height, 8, 5) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_8X6_LDR:
			// 	return astc_slice_size(width, height, 8, 6) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_8X8_LDR:
			// 	return astc_slice_size(width, height, 8, 8) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_10X5_LDR:
			// 	return astc_slice_size(width, height, 10, 5) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_10X6_LDR:
			// 	return astc_slice_size(width, height, 10, 6) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_10X8_LDR:
			// 	return astc_slice_size(width, height, 10, 8) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_10X10_LDR:
			// 	return astc_slice_size(width, height, 10, 10) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_12X10_LDR:
			// 	return astc_slice_size(width, height, 12, 10) * depth;
			// case Vulkan_PixelFormat_ASTC_RGBA_12X12_LDR:
			// 	return astc_slice_size(width, height, 12, 12) * depth;

			default:
				{
					F_Assert(false && "VulkanPixelFormat::GetPixelFormatMemorySize, Invalid compressed pixel format")
					return 0;
				}
			}
		}
		
		return width * height * depth * GetPixelFormatElemBytes(format);
	}   


	bool VulkanPixelFormat::IsNative(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsNative) > 0;
	}
	bool VulkanPixelFormat::IsCompressed(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsCompressed) > 0;
	}
	bool VulkanPixelFormat::IsInteger(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsInteger) > 0;
	}
	bool VulkanPixelFormat::IsFloat(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsFloat) > 0;
	}
	bool VulkanPixelFormat::IsLuminance(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsLuminance) > 0;
	}
	bool VulkanPixelFormat::IsStencil(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsStencil) > 0;
	}
	bool VulkanPixelFormat::IsDepth(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsDepth) > 0;
	}
	bool VulkanPixelFormat::IsDepthStencil(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_IsDepthStencil) > 0;
	}
	bool VulkanPixelFormat::HasAlpha(VulkanPixelFormatType format)
	{
		return (VulkanPixelFormat::GetPixelFormatFlags(format) & Vulkan_PixelFormatFlag_HasAlpha) > 0;
	}
	

	bool VulkanPixelFormat::IsValidExtent(size_t width, size_t height, size_t depth, VulkanPixelFormatType format)
	{
		if (IsCompressed(format))
		{
			switch(format)
			{
			// case Vulkan_PixelFormat_DXT1:
			// case Vulkan_PixelFormat_DXT2:
			// case Vulkan_PixelFormat_DXT3:
			// case Vulkan_PixelFormat_DXT4:
			// case Vulkan_PixelFormat_DXT5:
			// 	return ((width & 3) == 0 && (height & 3) == 0 && depth == 1);
			default:
				return true;
			}
		}

		return true;
	}
	bool VulkanPixelFormat::IsAccessible(VulkanPixelFormatType format)
	{
		return (format != Vulkan_PixelFormat_Unknown) && !IsCompressed(format);
	}

	VulkanPixelFormatType VulkanPixelFormat::ParsePixelFormatFromName(const String& strName, bool accessibleOnly /*= false*/, bool caseSensitive /*= false*/)
	{
		String tmp = strName;
		if (!caseSensitive)
		{
			FUtilString::ToUpperCase(tmp);
		}

        int32 count = (int32)Vulkan_PixelFormat_Count;
		for (int32 i = 0; i < count; ++i)
		{
			VulkanPixelFormatType format = static_cast<VulkanPixelFormatType>(i);
			if (!accessibleOnly || IsAccessible(format))
			{
				if (tmp == GetPixelFormatName(format))
					return format;
			}
		}
        // if(tmp == String("BYTE_RGB_UNORM"))
        //     return Vulkan_PixelFormat_BYTE_RGB_UNORM;
        // if(tmp == String("BYTE_RGBA_UNORM"))
        //     return Vulkan_PixelFormat_BYTE_RGBA_UNORM;
        // if(tmp == String("BYTE_BGR_UNORM"))
        //     return Vulkan_PixelFormat_BYTE_BGR_UNORM;
        // if(tmp == String("BYTE_BGRA_UNORM"))
        //     return Vulkan_PixelFormat_BYTE_BGRA_UNORM;

		return Vulkan_PixelFormat_Unknown;
	}

	VulkanPixelFormatType VulkanPixelFormat::ParsePixelFormatForBitDepths(VulkanPixelFormatType format, uint16 integerBits, uint16 floatBits)
	{
		switch (integerBits)
		{
		case 16:
			{
				switch ((int32)format)
				{
				// case Vulkan_PixelFormat_BYTE_R8G8B8_UNORM:
				// 	return Vulkan_PixelFormat_BYTE_R5G6B5_UNORM_PACK16;

				// case Vulkan_PixelFormat_BYTE_B8G8R8_UNORM:
				// 	return Vulkan_PixelFormat_BYTE_B5G6R5_UNORM_PACK16;

				// case Vulkan_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32:
				case Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM:
				// case Vulkan_PixelFormat_BYTE_A8B8G8R8_UNORM:
				// 	return Vulkan_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16;

				// case Vulkan_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:
				// case Vulkan_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32:
				// 	return Vulkan_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16;

				default:
					break;
				}
			}
			break;

		case 32:
			{
				switch ((int32)format)
				{
				// case Vulkan_PixelFormat_BYTE_R5G6B5_UNORM_PACK16:
				// 	return Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM;

				// case Vulkan_PixelFormat_BYTE_B5G6R5_UNORM_PACK16:
				// 	return Vulkan_PixelFormat_BYTE_A8B8G8R8_UNORM;

				// case Vulkan_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16:
				// 	return Vulkan_PixelFormat_BYTE_A8R8G8B8_UNORM;

				// case Vulkan_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16:
				// 	return Vulkan_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32;

				default:
					break;
				}
			}
			break;

		default:
			break;
		}

		switch (floatBits)
		{
		case 16:
			{
				switch ((int32)format)
				{
				// case Vulkan_PixelFormat_INT_R32_SFLOAT:
				// 	return Vulkan_PixelFormat_SHORT_R16_SFLOAT;

				// case Vulkan_PixelFormat_INT_B32G32R32_SFLOAT:
				// 	return Vulkan_PixelFormat_SHORT_B16G16R16_SFLOAT; 

				// case Vulkan_PixelFormat_INT_A32B32G32R32_SFLOAT:
				// 	return Vulkan_PixelFormat_SHORT_A16B16G16R16_SFLOAT;

				default:
					break;
				}
			}
			break;

		case 32:
			{
				switch ((int32)format)
				{
				// case Vulkan_PixelFormat_SHORT_R16_SFLOAT:
				// 	return Vulkan_PixelFormat_INT_R32_SFLOAT;

				// case Vulkan_PixelFormat_SHORT_B16G16R16_SFLOAT:
				// 	return Vulkan_PixelFormat_INT_B32G32R32_SFLOAT;

				// case Vulkan_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
				// 	return Vulkan_PixelFormat_INT_A32B32G32R32_SFLOAT;

				default:
					break;
				}
			}
			break;

		default:
			break;
		}

		return format;
	}

	void VulkanPixelFormat::PackColor(const FColor &color, const VulkanPixelFormatType format, void* dest)
	{
		PackColor(color.r, color.g, color.b, color.a, format, dest);
	}	

	void VulkanPixelFormat::PackColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a, const VulkanPixelFormatType format, void* dest)
	{
		const VulkanPixelFormatDes& des = GetPixelFormatDes(format);
		if(des.eFlags & Vulkan_PixelFormatFlag_IsNative) 
		{
			uint32 value = ((FBitwise::FixedToFixed(r, 8, des.nRbits)<<des.nRshift) & des.nRmask) |
						   ((FBitwise::FixedToFixed(g, 8, des.nGbits)<<des.nGshift) & des.nGmask) |
						   ((FBitwise::FixedToFixed(b, 8, des.nBbits)<<des.nBshift) & des.nBmask) |
						   ((FBitwise::FixedToFixed(a, 8, des.nAbits)<<des.nAshift) & des.nAmask);
			FBitwise::IntWrite(dest,des.nElemBytes,value);
		}
		else
		{
			PackColor((float)r/255.0f, (float)g/255.0f, (float)b/255.0f, (float)a/255.0f, format, dest);
		}
	}

	void VulkanPixelFormat::PackColor(const float r, const float g, const float b, const float a, const VulkanPixelFormatType format, void* dest)
	{
		const VulkanPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & Vulkan_PixelFormatFlag_IsNative) 
		{
			const uint32 value = ((FBitwise::FloatToFixed(r, des.nRbits)<<des.nRshift) & des.nRmask) |
								 ((FBitwise::FloatToFixed(g, des.nGbits)<<des.nGshift) & des.nGmask) |
								 ((FBitwise::FloatToFixed(b, des.nBbits)<<des.nBshift) & des.nBmask) |
								 ((FBitwise::FloatToFixed(a, des.nAbits)<<des.nAshift) & des.nAmask);
			FBitwise::IntWrite(dest, des.nElemBytes, value);
		} 
		else 
		{
			switch ((int32)format)
			{
			// case Vulkan_PixelFormat_INT_R32_SFLOAT:
			// 	((float*)dest)[0] = r;
			// 	break;
			// case Vulkan_PixelFormat_INT_G32R32_SFLOAT:
			// 	((float*)dest)[0] = r;
			// 	((float*)dest)[1] = g;
			// 	break;
			// case Vulkan_PixelFormat_INT_B32G32R32_SFLOAT:
			// 	((float*)dest)[0] = r;
			// 	((float*)dest)[1] = g;
			// 	((float*)dest)[2] = b;
			// 	break;
			// case Vulkan_PixelFormat_INT_A32B32G32R32_SFLOAT:
			// 	((float*)dest)[0] = r;
			// 	((float*)dest)[1] = g;
			// 	((float*)dest)[2] = b;
			// 	((float*)dest)[3] = a;
			// 	break;
			// case Vulkan_PixelFormat_DEPTH_D16_UNORM:
			// case Vulkan_PixelFormat_SHORT_R16_SFLOAT:
			// 	((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_G16R16_SFLOAT:
			// 	((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
			// 	((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_B16G16R16_SFLOAT:
			// 	((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
			// 	((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
			// 	((uint16*)dest)[2] = FBitwise::FloatToHalf(b);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
			// 	((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
			// 	((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
			// 	((uint16*)dest)[2] = FBitwise::FloatToHalf(b);
			// 	((uint16*)dest)[3] = FBitwise::FloatToHalf(a);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_B16G16R16_UNORM:
			// 	((uint16*)dest)[0] = (uint16)FBitwise::FloatToFixed(r, 16);
			// 	((uint16*)dest)[1] = (uint16)FBitwise::FloatToFixed(g, 16);
			// 	((uint16*)dest)[2] = (uint16)FBitwise::FloatToFixed(b, 16);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_A16B16G16R16_UNORM:
			// 	((uint16*)dest)[0] = (uint16)FBitwise::FloatToFixed(r, 16);
			// 	((uint16*)dest)[1] = (uint16)FBitwise::FloatToFixed(g, 16);
			// 	((uint16*)dest)[2] = (uint16)FBitwise::FloatToFixed(b, 16);
			// 	((uint16*)dest)[3] = (uint16)FBitwise::FloatToFixed(a, 16);
			// 	break;
			// case Vulkan_PixelFormat_BYTE_AL_UNORM:
			// 	((uint8*)dest)[0] = (uint8)FBitwise::FloatToFixed(r, 8);
			// 	((uint8*)dest)[1] = (uint8)FBitwise::FloatToFixed(a, 8);
			// 	break;
			// case Vulkan_PixelFormat_BYTE_A8_UNORM:
			// 	((uint8*)dest)[0] = (uint8)FBitwise::FloatToFixed(r, 8);
            //     break;
            // case Vulkan_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:
            // {
            //     const uint16 ir = static_cast<uint16>(FMath::Saturate(r) * 1023.0f + 0.5f);
            //     const uint16 ig = static_cast<uint16>(FMath::Saturate(g) * 1023.0f + 0.5f);
            //     const uint16 ib = static_cast<uint16>(FMath::Saturate(b) * 1023.0f + 0.5f);
            //     const uint16 ia = static_cast<uint16>(FMath::Saturate(a) * 3.0f + 0.5f);

            //     ((uint32*)dest)[0] = (ia << 30u) | (ir << 20u) | (ig << 10u) | (ib);
            //     break;
            // }
			default:
				{
					F_LogError("VulkanPixelFormat::PackColor: pack to [%s] not implemented", GetPixelFormatName(format).c_str());
					F_Assert(false && "VulkanPixelFormat::PackColor")
				}
				break;
			}
		}
	}

	void VulkanPixelFormat::UnpackColor(FColor& color, VulkanPixelFormatType format, const void* src)
	{
		UnpackColor(&color.r, &color.g, &color.b, &color.a, format, src);
	}

	void VulkanPixelFormat::UnpackColor(FColor* color, VulkanPixelFormatType format, const void* src)
	{
		UnpackColor(&color->r, &color->g, &color->b, &color->a, format, src);
	}

	void VulkanPixelFormat::UnpackColor(uint8 *r, uint8 *g, uint8 *b, uint8 *a, VulkanPixelFormatType format, const void* src)
	{
		const VulkanPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & Vulkan_PixelFormatFlag_IsNative) 
		{
			const uint32 value = FBitwise::IntRead(src, des.nElemBytes);
			if (des.eFlags & Vulkan_PixelFormatFlag_IsLuminance)
			{
				*r = *g = *b = (uint8)FBitwise::FixedToFixed((value & des.nRmask)>>des.nRshift, des.nRbits, 8);
			}
			else
			{
				*r = (uint8)FBitwise::FixedToFixed((value & des.nRmask)>>des.nRshift, des.nRbits, 8);
				*g = (uint8)FBitwise::FixedToFixed((value & des.nGmask)>>des.nGshift, des.nGbits, 8);
				*b = (uint8)FBitwise::FixedToFixed((value & des.nBmask)>>des.nBshift, des.nBbits, 8);
			}
			if (des.eFlags & Vulkan_PixelFormatFlag_HasAlpha)
			{
				*a = (uint8)FBitwise::FixedToFixed((value & des.nAmask)>>des.nAshift, des.nAbits, 8);
			}
			else
			{
				*a = 255;
			}
		}
		else
		{
			float rr, gg, bb, aa;
			UnpackColor(&rr, &gg, &bb, &aa, format, src);
			*r = (uint8)FBitwise::FloatToFixed(rr, 8);
			*g = (uint8)FBitwise::FloatToFixed(gg, 8);
			*b = (uint8)FBitwise::FloatToFixed(bb, 8);
			*a = (uint8)FBitwise::FloatToFixed(aa, 8);
		}
	}

	void VulkanPixelFormat::UnpackColor(float *r, float *g, float *b, float *a, VulkanPixelFormatType format, const void* src)
	{
		const VulkanPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & Vulkan_PixelFormatFlag_IsNative) 
		{
			const uint32 value = FBitwise::IntRead(src, des.nElemBytes);
			if (des.eFlags & Vulkan_PixelFormatFlag_IsLuminance)
			{
				*r = *g = *b = FBitwise::FixedToFloat((value & des.nRmask)>>des.nRshift, des.nRbits);
			}
			else
			{
				*r = FBitwise::FixedToFloat((value & des.nRmask)>>des.nRshift, des.nRbits);
				*g = FBitwise::FixedToFloat((value & des.nGmask)>>des.nGshift, des.nGbits);
				*b = FBitwise::FixedToFloat((value & des.nBmask)>>des.nBshift, des.nBbits);
			}
			if (des.eFlags & Vulkan_PixelFormatFlag_HasAlpha)
			{
				*a = FBitwise::FixedToFloat((value & des.nAmask)>>des.nAshift, des.nAbits);
			}
			else
			{
				*a = 1.0f;
			}
		} 
		else 
		{
			switch ((int32)format)
			{
			// case Vulkan_PixelFormat_INT_R32_SFLOAT:
			// 	*r = *g = *b = ((float*)src)[0];
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_INT_G32R32_SFLOAT:
			// 	*r = ((float*)src)[0];
			// 	*g = *b = ((float*)src)[1];
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_INT_B32G32R32_SFLOAT:
			// 	*r = ((float*)src)[0];
			// 	*g = ((float*)src)[1];
			// 	*b = ((float*)src)[2];
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_INT_A32B32G32R32_SFLOAT:
			// 	*r = ((float*)src)[0];
			// 	*g = ((float*)src)[1];
			// 	*b = ((float*)src)[2];
			// 	*a = ((float*)src)[3];
			// 	break;
			// case Vulkan_PixelFormat_SHORT_R16_SFLOAT:
			// 	*r = *g = *b = FBitwise::HalfToFloat(((uint16*)src)[0]);
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_SHORT_G16R16_SFLOAT:
			// 	*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
			// 	*g = *b = FBitwise::HalfToFloat(((uint16*)src)[1]);
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_SHORT_B16G16R16_SFLOAT:
			// 	*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
			// 	*g = FBitwise::HalfToFloat(((uint16*)src)[1]);
			// 	*b = FBitwise::HalfToFloat(((uint16*)src)[2]);
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
			// 	*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
			// 	*g = FBitwise::HalfToFloat(((uint16*)src)[1]);
			// 	*b = FBitwise::HalfToFloat(((uint16*)src)[2]);
			// 	*a = FBitwise::HalfToFloat(((uint16*)src)[3]);
			// 	break;
			// case Vulkan_PixelFormat_SHORT_B16G16R16_UNORM:
			// 	*r = FBitwise::FixedToFloat(((uint16*)src)[0], 16);
			// 	*g = FBitwise::FixedToFloat(((uint16*)src)[1], 16);
			// 	*b = FBitwise::FixedToFloat(((uint16*)src)[2], 16);
			// 	*a = 1.0f;
			// 	break;
			// case Vulkan_PixelFormat_SHORT_A16B16G16R16_UNORM:
			// 	*r = FBitwise::FixedToFloat(((uint16*)src)[0], 16);
			// 	*g = FBitwise::FixedToFloat(((uint16*)src)[1], 16);
			// 	*b = FBitwise::FixedToFloat(((uint16*)src)[2], 16);
			// 	*a = FBitwise::FixedToFloat(((uint16*)src)[3], 16);
			// 	break;
			// case Vulkan_PixelFormat_BYTE_AL_UNORM:
			// 	*r = *g = *b = FBitwise::FixedToFloat(((uint8*)src)[0], 8);
			// 	*a = FBitwise::FixedToFloat(((uint8*)src)[1], 8);
			// 	break;
			default:
				{
					F_LogError("VulkanPixelFormat::UnpackColor: unpack from [%s] not implemented", GetPixelFormatName(format).c_str());
					F_Assert(false && "VulkanPixelFormat::UnpackColor")
				}
				break;
			}
		}
	}

	void VulkanPixelFormat::BulkPixelConversion(void *src, VulkanPixelFormatType srcFormat, void *dest, VulkanPixelFormatType dstFormat, uint32 count)
	{
		VulkanPixelBox src_box(count, 1, 1, srcFormat, src);
		VulkanPixelBox dst_box(count, 1, 1, dstFormat, dest);
		BulkPixelConversion(src_box, dst_box);
	}

	void VulkanPixelFormat::BulkPixelConversion(const VulkanPixelBox &src, const VulkanPixelBox &dst)
	{
		F_Assert(src.GetWidth() == dst.GetWidth() &&
			     src.GetHeight() == dst.GetHeight() &&
			     src.GetDepth() == dst.GetDepth() && 
                 "VulkanPixelFormat::BulkPixelConversion")

		if (VulkanPixelFormat::IsCompressed(src.m_ePixelFormat) || VulkanPixelFormat::IsCompressed(dst.m_ePixelFormat))
		{
			if (src.m_ePixelFormat == dst.m_ePixelFormat &&
			    src.IsConsecutive() && dst.IsConsecutive())
			{
                size_t bytesPerSlice = GetPixelFormatMemorySize(src.GetWidth(), src.GetHeight(), 1, src.m_ePixelFormat);
                memcpy(dst.m_pData + bytesPerSlice * dst.m_nFront,
                       src.m_pData + bytesPerSlice * src.m_nFront,
                       bytesPerSlice * src.GetDepth());
                return;
			}
			else
			{
				F_LogError("VulkanPixelFormat::BulkPixelConversion: This method can not be used to compress or decompress images");
				F_Assert(false && "VulkanPixelFormat::BulkPixelConversion")
			}
		}

		if (src.m_ePixelFormat == dst.m_ePixelFormat) 
		{
			if (src.IsConsecutive() && dst.IsConsecutive())
			{
				memcpy(dst.GetTopLeftFrontPixelPtr(), src.GetTopLeftFrontPixelPtr(), src.GetConsecutiveSize());
                return;
			}

			const size_t srcPixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat);
			const size_t dstPixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat);
			uint8 *srcptr = static_cast<uint8*>(src.m_pData)
				+ (src.m_nLeft + src.m_nTop * src.m_nRowPitch + src.m_nFront * src.m_nSlicePitch) * srcPixelSize;
			uint8 *dstptr = static_cast<uint8*>(dst.m_pData)
				+ (dst.m_nLeft + dst.m_nTop * dst.m_nRowPitch + dst.m_nFront * dst.m_nSlicePitch) * dstPixelSize;

			const size_t srcRowPitchBytes = src.m_nRowPitch*srcPixelSize;
			const size_t srcSliceSkipBytes = src.GetSliceSkip()*srcPixelSize;

			const size_t dstRowPitchBytes = dst.m_nRowPitch*dstPixelSize;
			const size_t dstSliceSkipBytes = dst.GetSliceSkip()*dstPixelSize;

			const size_t rowSize = src.GetWidth()*srcPixelSize;
			for (size_t z = src.m_nFront; z < src.m_nBack; z++)
			{
				for (size_t y = src.m_nTop; y < src.m_nBottom; y++)
				{
					memcpy(dstptr, srcptr, rowSize);
					srcptr += srcRowPitchBytes;
					dstptr += dstRowPitchBytes;
				}
				srcptr += srcSliceSkipBytes;
				dstptr += dstSliceSkipBytes;
			}
			return;
		}
        
		const size_t srcPixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat);
		const size_t dstPixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat);
		uint8 *srcptr = static_cast<uint8*>(src.m_pData)
			+ (src.m_nLeft + src.m_nTop * src.m_nRowPitch + src.m_nFront * src.m_nSlicePitch) * srcPixelSize;
		uint8 *dstptr = static_cast<uint8*>(dst.m_pData)
			+ (dst.m_nLeft + dst.m_nTop * dst.m_nRowPitch + dst.m_nFront * dst.m_nSlicePitch) * dstPixelSize;

		const size_t srcRowSkipBytes	= src.GetRowSkip()*srcPixelSize;
		const size_t srcSliceSkipBytes	= src.GetSliceSkip()*srcPixelSize;
		const size_t dstRowSkipBytes	= dst.GetRowSkip()*dstPixelSize;
		const size_t dstSliceSkipBytes	= dst.GetSliceSkip()*dstPixelSize;

		float r,g,b,a;
		for (size_t z = src.m_nFront; z < src.m_nBack; z++)
		{
			for (size_t y = src.m_nTop; y < src.m_nBottom; y++)
			{
				for (size_t x = src.m_nLeft; x < src.m_nRight; x++)
				{
					UnpackColor(&r, &g, &b, &a, src.m_ePixelFormat, srcptr);
					PackColor(r, g, b, a, dst.m_ePixelFormat, dstptr);
					srcptr += srcPixelSize;
					dstptr += dstPixelSize;
				}
				srcptr += srcRowSkipBytes;
				dstptr += dstRowSkipBytes;
			}
			srcptr += srcSliceSkipBytes;
			dstptr += dstSliceSkipBytes;
		}
	}

	void VulkanPixelFormat::BulkPixelVerticalFlip(const VulkanPixelBox& box)
	{
        if (VulkanPixelFormat::IsCompressed(box.m_ePixelFormat))
        {
            F_Assert(false && "VulkanPixelFormat::BulkPixelVerticalFlip: This method can not be used for compressed format !")
            return;
        }
        
        const size_t pixelSize = VulkanPixelFormat::GetPixelFormatElemBytes(box.m_ePixelFormat);
        const size_t copySize = (box.m_nRight - box.m_nLeft) * pixelSize;

        const size_t rowPitchBytes = box.m_nRowPitch * pixelSize;
        const size_t slicePitchBytes = box.m_nSlicePitch * pixelSize;

        uint8 *basesrcptr = box.m_pData
            + (box.m_nLeft + box.m_nTop * box.m_nRowPitch + box.m_nFront * box.m_nSlicePitch) * pixelSize;
        uint8 *basedstptr = basesrcptr + (box.m_nBottom - box.m_nTop - 1) * rowPitchBytes;
        uint8* tmpptr = new uint8[copySize];
        {
            const size_t halfRowCount = (box.m_nBottom - box.m_nTop) >> 1;
            for(size_t z = box.m_nFront; z < box.m_nBack; z++)
            {
                uint8* srcptr = basesrcptr;
                uint8* dstptr = basedstptr;
                for(size_t y = 0; y < halfRowCount; y++)
                {
                    memcpy(tmpptr, dstptr, copySize);
                    memcpy(dstptr, srcptr, copySize);
                    memcpy(srcptr, tmpptr, copySize);
                    srcptr += rowPitchBytes;
                    dstptr -= rowPitchBytes;
                }
                basesrcptr += slicePitchBytes;
                basedstptr += slicePitchBytes;
            }
        }
		delete []tmpptr;
	}

}; //LostPeter