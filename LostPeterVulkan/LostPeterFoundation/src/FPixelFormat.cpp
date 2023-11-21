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

#include "../include/FPixelFormat.h"
#include "../include/FBitwise.h"
#include "../include/FMath.h"
#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
    FPixelFormatDes FPixelFormat::ms_PixelFormats[F_PixelFormat_Count] = 
	{
		//0		F_PixelFormat_Unknown
		{
			"Unknown", 0, 0, F_PixelFormatComponent_ByteU, 0, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0
		},

		//1		F_PixelFormat_BYTE_R4G4_UNORM_PACK8
		{
			"BYTE_R4G4_UNORM_PACK8", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			4,      	4,      	0,     	 	0,
			0xF0,   	0x0F,      	0,     	 	0,
			4,      	0,      	0,      	0
		},

		//2		F_PixelFormat_BYTE_L8_UNORM
		{
			"BYTE_L8_UNORM", 1, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,      	0,      	0,     	 	0,
			0xFF,   	0,      	0,     	 	0,
			0,      	0,      	0,      	0
		},

		//3		F_PixelFormat_BYTE_A8_UNORM
		{
			"BYTE_A8_UNORM", 1, F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 1, false,
			0,      	0,     	 	0,      	8,
			0,      	0,      	0,      	0xFF,
			0,      	0,      	0,      	0
		},

		//4		F_PixelFormat_BYTE_R8_UNORM
		{
			"BYTE_R8_UNORM", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//5		F_PixelFormat_BYTE_R8_SNORM
		{
			"BYTE_R8_SNORM", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//6		F_PixelFormat_BYTE_R8_USCALED
		{
			"BYTE_R8_USCALED", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//7		F_PixelFormat_BYTE_R8_SSCALED
		{
			"BYTE_R8_SSCALED", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//8		F_PixelFormat_BYTE_R8_UINT
		{
			"BYTE_R8_UINT", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//9		F_PixelFormat_BYTE_R8_SINT
		{
			"BYTE_R8_SINT", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//10	F_PixelFormat_BYTE_R8_SRGB
		{
			"BYTE_R8_SRGB", 1, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,      	0,      	0,      	0,
			0xFF,   	0,      	0,      	0,
			0,      	0,      	0,      	0		
		},

		//11	F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16
		{
			"BYTE_R4G4B4A4_UNORM_PACK16", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			4,      	4,      	4,      	4,
			0xF000,		0x0F00, 	0x00F0, 	0x000F,
			12,      	8,      	4,      	0
		},

		//12	F_PixelFormat_BYTE_B4G4R4A4_UNORM_PACK16
		{
			"BYTE_B4G4R4A4_UNORM_PACK16", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			4,      	4,      	4,      	4,
			0x00F0, 	0x0F00, 	0xF000, 	0x000F,
			4,      	8,      	12,      	0
		},

		//13	F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16
		{		
			"BYTE_R5G6B5_UNORM_PACK16", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			5,      	6,      	5,      	0,
			0xF800, 	0x07E0, 	0x001F, 	0,
			11,     	5,     		0,      	0
		},

		//14	F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16
		{
			"BYTE_B5G6R5_UNORM_PACK16", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			5,      	6,      	5,      	0,
			0x001F, 	0x07E0, 	0xF800, 	0,
			0,      	5,      	11,     	0
		},

		//15	F_PixelFormat_BYTE_R5G5B5A1_UNORM_PACK16
		{
			"BYTE_R5G5B5A1_UNORM_PACK16", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			5,      	5,      	5,      	1,
			0xF800, 	0x07C0, 	0x003E, 	0x0001,
			11,     	6,      	1,      	0,
		},

		//16	F_PixelFormat_BYTE_B5G5R5A1_UNORM_PACK16
		{
			"BYTE_B5G5R5A1_UNORM_PACK16", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			5,      	5,      	5,      	1,
			0x003E, 	0x07C0, 	0xF800,		0x0001,
			1,     		6,      	11,      	0,
		},

		//17	F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16
		{
			"BYTE_A1R5G5B5_UNORM_PACK16", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			5,      	5,      	5,      	1,
			0x7C00, 	0x03E0, 	0x001F, 	0x8000,
			10,     	5,      	0,      	15,
		},

		//18	F_PixelFormat_BYTE_AL_UNORM
		{
			"BYTE_AL_UNORM", 2, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsLuminance, F_PixelFormatComponent_ByteU, 2, false,
			8,      	0,      	0,      	8,
			0x00FF,     0,      	0,     	0xFF00,
			0,      	0,      	0,      	8
		},

		//19	F_PixelFormat_BYTE_G8R8_UNORM
		{
			"BYTE_G8R8_UNORM", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//20	F_PixelFormat_BYTE_G8R8_SNORM
		{
			"BYTE_G8R8_SNORM", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//21	F_PixelFormat_BYTE_G8R8_USCALED
		{
			"BYTE_G8R8_USCALED", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//22	F_PixelFormat_BYTE_G8R8_SSCALED
		{
			"BYTE_G8R8_SSCALED", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//23	F_PixelFormat_BYTE_G8R8_UINT
		{
			"BYTE_G8R8_UINT", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//24	F_PixelFormat_BYTE_G8R8_SINT
		{
			"BYTE_G8R8_SINT", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//25	F_PixelFormat_BYTE_G8R8_SRGB
		{
			"BYTE_G8R8_SRGB", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 2, false,
			8,      	8,      	0,      	0,
			0xFF0000, 	0x00FF00,   0,      	0,
			0,      	8,      	0,      	0
		},

		//26	F_PixelFormat_BYTE_R8G8B8_UNORM
		{
			"BYTE_R8G8B8_UNORM", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//27	F_PixelFormat_BYTE_R8G8B8_SNORM
		{
			"BYTE_R8G8B8_SNORM", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//28	F_PixelFormat_BYTE_R8G8B8_USCALED
		{
			"BYTE_R8G8B8_USCALED", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//29	F_PixelFormat_BYTE_R8G8B8_SSCALED
		{
			"BYTE_R8G8B8_SSCALED", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//30	F_PixelFormat_BYTE_R8G8B8_UINT
		{
			"BYTE_R8G8B8_UINT", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//31	F_PixelFormat_BYTE_R8G8B8_SINT
		{
			"BYTE_R8G8B8_SINT", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//32	F_PixelFormat_BYTE_R8G8B8_SRGB
		{
			"BYTE_R8G8B8_SRGB", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0,
			0xFF0000,	0x00FF00,	0x0000FF,	0,
			16,      	8,      	0,     		0
		},

		//33	F_PixelFormat_BYTE_B8G8R8_UNORM
		{
			"BYTE_B8G8R8_UNORM", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//34	F_PixelFormat_BYTE_B8G8R8_SNORM
		{
			"BYTE_B8G8R8_SNORM", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//35	F_PixelFormat_BYTE_B8G8R8_USCALED
		{
			"BYTE_B8G8R8_USCALED", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//36	F_PixelFormat_BYTE_B8G8R8_SSCALED
		{
			"BYTE_B8G8R8_SSCALED", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//37	F_PixelFormat_BYTE_B8G8R8_UINT
		{
			"BYTE_B8G8R8_UINT", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//38	F_PixelFormat_BYTE_B8G8R8_SINT
		{
			"BYTE_B8G8R8_SINT", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//39	F_PixelFormat_BYTE_B8G8R8_SRGB
		{
			"BYTE_B8G8R8_SRGB", 3, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 3, false,
			8,      	8,      	8,      	0, 
			0x0000FF,	0x00FF00,	0xFF0000,	0,
			0,     		8,      	16,      	0
		},

		//40 	F_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32
		{
			"BYTE_A8B8G8R8_UNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//41 	F_PixelFormat_BYTE_A8B8G8R8_SNORM_PACK32
		{
			"BYTE_A8B8G8R8_SNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//42 	F_PixelFormat_BYTE_A8B8G8R8_USCALED_PACK32
		{
			"BYTE_A8B8G8R8_USCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//43 	F_PixelFormat_BYTE_A8B8G8R8_SSCALED_PACK32
		{
			"BYTE_A8B8G8R8_SSCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//44 	F_PixelFormat_BYTE_A8B8G8R8_UINT_PACK32
		{
			"BYTE_A8B8G8R8_UINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//45 	F_PixelFormat_BYTE_A8B8G8R8_SINT_PACK32
		{
			"BYTE_A8B8G8R8_SINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//46 	F_PixelFormat_BYTE_A8B8G8R8_SRGB_PACK32
		{
			"BYTE_A8B8G8R8_SRGB_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//47 	F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32
		{
			"BYTE_A2R10G10B10_UNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},
		
		//48 	F_PixelFormat_BYTE_A2R10G10B10_SNORM_PACK32
		{
			"BYTE_A2R10G10B10_SNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},

		//49 	F_PixelFormat_BYTE_A2R10G10B10_USCALED_PACK32
		{
			"BYTE_A2R10G10B10_USCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},

		//50 	F_PixelFormat_BYTE_A2R10G10B10_SSCALED_PACK32
		{
			"BYTE_A2R10G10B10_SSCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},

		//51 	F_PixelFormat_BYTE_A2R10G10B10_UINT_PACK32
		{
			"BYTE_A2R10G10B10_UINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},

		//52 	F_PixelFormat_BYTE_A2R10G10B10_SINT_PACK32
		{
			"BYTE_A2R10G10B10_SINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	10,     	10,     	2,
			0x3FF00000,	0x000FFC00,	0x000003FF,	0xC0000000,
			20,      	10,     	0,     		30
		},

		//53	F_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32
		{
			"BYTE_A2B10G10R10_UNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//54	F_PixelFormat_BYTE_A2B10G10R10_SNORM_PACK32
		{
			"BYTE_A2B10G10R10_SNORM_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//55	F_PixelFormat_BYTE_A2B10G10R10_USCALED_PACK32
		{
			"BYTE_A2B10G10R10_USCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//56	F_PixelFormat_BYTE_A2B10G10R10_SSCALED_PACK32
		{
			"BYTE_A2B10G10R10_SSCALED_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//57	F_PixelFormat_BYTE_A2B10G10R10_UINT_PACK32
		{
			"BYTE_A2B10G10R10_UINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//58	F_PixelFormat_BYTE_A2B10G10R10_SINT_PACK32
		{
			"BYTE_A2B10G10R10_SINT_PACK32", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			10,     	0,      	10,     	2,
			0x000003FF,	0x000FFC00,	0x3FF00000,	0xC0000000,
			0,     		10,     	20,      	30
		},

		//59	F_PixelFormat_BYTE_A8R8G8B8_UNORM
		{
			"BYTE_A8R8G8B8_UNORM", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//60	F_PixelFormat_BYTE_A8R8G8B8_SNORM
		{
			"BYTE_A8R8G8B8_SNORM", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//61	F_PixelFormat_BYTE_A8R8G8B8_USCALED
		{
			"BYTE_A8R8G8B8_USCALED", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//62	F_PixelFormat_BYTE_A8R8G8B8_SSCALED
		{
			"BYTE_A8R8G8B8_SSCALED", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//63	F_PixelFormat_BYTE_A8R8G8B8_UINT
		{
			"BYTE_A8R8G8B8_UINT", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//64	F_PixelFormat_BYTE_A8R8G8B8_SINT
		{
			"BYTE_A8R8G8B8_SINT", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//65	F_PixelFormat_BYTE_A8R8G8B8_SRGB
		{
			"BYTE_A8R8G8B8_SRGB", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x00FF0000,	0x0000FF00,	0x000000FF,	0xFF000000,
			16,      	8,     		0,     		24
		},

		//66	F_PixelFormat_BYTE_A8B8G8R8_UNORM
		{
			"BYTE_A8B8G8R8_UNORM", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//67	F_PixelFormat_BYTE_A8B8G8R8_SNORM
		{
			"BYTE_A8B8G8R8_SNORM", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//68	F_PixelFormat_BYTE_A8B8G8R8_USCALED
		{
			"BYTE_A8B8G8R8_USCALED", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//69	F_PixelFormat_BYTE_A8B8G8R8_SSCALED
		{
			"BYTE_A8B8G8R8_SSCALED", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//70	F_PixelFormat_BYTE_A8B8G8R8_UINT
		{
			"BYTE_A8B8G8R8_UINT", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//71	F_PixelFormat_BYTE_A8B8G8R8_SINT
		{
			"BYTE_A8B8G8R8_SINT", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteS, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//72	F_PixelFormat_BYTE_A8B8G8R8_SRGB
		{
			"BYTE_A8B8G8R8_SRGB", 4, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 4, false,
			8,      	8,      	8,      	8,
			0x000000FF, 0x0000FF00, 0x00FF0000,	0xFF000000,
			0,     		8,     		16,      	24
		},

		//73	F_PixelFormat_SHORT_L16_UNORM
		{
			"SHORT_L16_UNORM", 2, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//74	F_PixelFormat_SHORT_R16_UNORM
		{
			"SHORT_R16_UNORM", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},
		
		//75	F_PixelFormat_SHORT_R16_SNORM
		{
			"SHORT_R16_SNORM", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//76	F_PixelFormat_SHORT_R16_USCALED
		{
			"SHORT_R16_USCALED", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//77	F_PixelFormat_SHORT_R16_SSCALED
		{
			"SHORT_R16_SSCALED", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//78	F_PixelFormat_SHORT_R16_UINT
		{
			"SHORT_R16_UINT", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//79	F_PixelFormat_SHORT_R16_SINT
		{
			"SHORT_R16_SINT", 2, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//80	F_PixelFormat_SHORT_R16_SFLOAT
		{
			"SHORT_R16_SFLOAT", 2, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float16, 1, false,
			16,     	0,      	0,      	0,
			0xFFFF, 	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//81	F_PixelFormat_SHORT_G16R16_UNORM
		{
			"SHORT_G16R16_UNORM", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//82	F_PixelFormat_SHORT_G16R16_SNORM
		{
			"SHORT_G16R16_SNORM", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//83	F_PixelFormat_SHORT_G16R16_USCALED
		{
			"SHORT_G16R16_USCALED", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//84	F_PixelFormat_SHORT_G16R16_SSCALED
		{
			"SHORT_G16R16_SSCALED", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//85	F_PixelFormat_SHORT_G16R16_UINT
		{
			"SHORT_G16R16_UINT", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//86	F_PixelFormat_SHORT_G16R16_SINT
		{
			"SHORT_G16R16_SINT", 4, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//87	F_PixelFormat_SHORT_G16R16_SFLOAT
		{
			"SHORT_G16R16_SFLOAT", 4, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float16, 2, false,
			16,     	16,     	0,      	0,
			0x0000FFFF,	0xFFFF0000,	0,			0, 
			0,      	16,     	0,      	0
		},

		//88	F_PixelFormat_SHORT_B16G16R16_UNORM
		{
			"SHORT_B16G16R16_UNORM", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//89	F_PixelFormat_SHORT_B16G16R16_SNORM
		{
			"SHORT_B16G16R16_SNORM", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//90	F_PixelFormat_SHORT_B16G16R16_USCALED
		{
			"SHORT_B16G16R16_USCALED", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//91	F_PixelFormat_SHORT_B16G16R16_SSCALED
		{
			"SHORT_B16G16R16_SSCALED", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//92	F_PixelFormat_SHORT_B16G16R16_UINT
		{
			"SHORT_B16G16R16_UINT", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//93	F_PixelFormat_SHORT_B16G16R16_SINT
		{
			"SHORT_B16G16R16_SINT", 6, F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//94	F_PixelFormat_SHORT_B16G16R16_SFLOAT
		{
			"SHORT_B16G16R16_SFLOAT", 6, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float16, 3, false,
			16,     	16,     	16,     	0,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0,  
			0,      	16,      	32,      	0
		},

		//95	F_PixelFormat_SHORT_A16B16G16R16_UNORM
		{
			"SHORT_A16B16G16R16_UNORM", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//96	F_PixelFormat_SHORT_A16B16G16R16_SNORM
		{
			"SHORT_A16B16G16R16_SNORM", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//97	F_PixelFormat_SHORT_A16B16G16R16_USCALED
		{
			"SHORT_A16B16G16R16_USCALED", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//98	F_PixelFormat_SHORT_A16B16G16R16_SSCALED
		{
			"SHORT_A16B16G16R16_SSCALED", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//99	F_PixelFormat_SHORT_A16B16G16R16_UINT
		{
			"SHORT_A16B16G16R16_UINT", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//100	F_PixelFormat_SHORT_A16B16G16R16_SINT
		{
			"SHORT_A16B16G16R16_SINT", 8, F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortS, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//101	F_PixelFormat_SHORT_A16B16G16R16_SFLOAT
		{
			"SHORT_A16B16G16R16_SFLOAT", 8, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float16, 4, false,
			16,     	16,     	16,     	16,
			0x0000FFFF,	0xFFFF0000,	0xFFFF00000000,			0xFFFF000000000000,  
			0,      	16,      	32,      	48
		},

		//102	F_PixelFormat_INT_R32_UINT
		{
			"INT_R32_UINT", 4, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntU, 1, false,
			32,     	0,      	0,      	0,
			0xFFFFFFFF, 0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//103	F_PixelFormat_INT_R32_SINT
		{
			"INT_R32_SINT", 4, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntS, 1, false,
			32,     	0,      	0,      	0,
			0xFFFFFFFF, 0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//104	F_PixelFormat_INT_R32_SFLOAT
		{
			"INT_R32_SFLOAT", 4, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 1, false,
			32,     	0,      	0,      	0,
			0xFFFFFFFF, 0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//105	F_PixelFormat_INT_G32R32_UINT
		{
			"INT_G32R32_UINT", 8, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntU, 2, false,
			32,     	32,      	0,      	0,
			0xFFFFFFFF, 0xFFFFFFFF, 0, 			0,
            0, 			32, 		0, 			0
		},

		//106	F_PixelFormat_INT_G32R32_SINT
		{
			"INT_G32R32_SINT", 8, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntS, 2, false,
			32,     	32,      	0,      	0,
			0xFFFFFFFF, 0xFFFFFFFF, 0, 			0,
            0, 			32, 		0, 			0
		},

		//107	F_PixelFormat_INT_G32R32_SFLOAT
		{
			"INT_G32R32_SFLOAT", 8, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 2, false,
			32,     	32,      	0,      	0,
			0xFFFFFFFF, 0xFFFFFFFF, 0, 			0,
            0, 			32, 		0, 			0
		},

		//108	F_PixelFormat_INT_B32G32R32_UINT
		{
			"INT_B32G32R32_UINT", 12, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntU, 3, false,
			32,     	32,      	32,      	0,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		0
		},

		//109	F_PixelFormat_INT_B32G32R32_SINT
		{
			"INT_B32G32R32_SINT", 12, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntS, 3, false,
			32,     	32,      	32,      	0,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		0
		},

		//110	F_PixelFormat_INT_B32G32R32_SFLOAT
		{
			"INT_B32G32R32_SFLOAT", 12, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 3, false,
			32,     	32,      	32,      	0,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		0
		},

		//111	F_PixelFormat_INT_A32B32G32R32_UINT
		{
			"INT_A32B32G32R32_UINT", 16, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntU, 4, false,
			32,     	32,      	32,      	32,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		96
		},

		//112	F_PixelFormat_INT_A32B32G32R32_SINT
		{
			"INT_A32B32G32R32_SINT", 16, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntS, 4, false,
			32,     	32,      	32,      	32,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		96
		},

		//113	F_PixelFormat_INT_A32B32G32R32_SFLOAT
		{
			"INT_A32B32G32R32_SFLOAT", 16, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 4, false,
			32,     	32,      	32,      	32,
			0, 			0, 			0, 			0,
            0, 			32, 		64, 		96
		},
		
		//114	F_PixelFormat_LONG_R64_UINT
		{
			"LONG_R64_UINT", 8, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongU, 1, false,
			64,     	0,      	0,      	0,
			0, 			0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//115	F_PixelFormat_LONG_R64_SINT
		{
			"LONG_R64_SINT", 8, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongS, 1, false,
			64,     	0,      	0,      	0,
			0, 			0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//116	F_PixelFormat_LONG_R64_SFLOAT
		{
			"LONG_R64_SFLOAT", 8, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Double, 1, false,
			64,     	0,      	0,      	0,
			0, 			0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//117	F_PixelFormat_LONG_G64R64_UINT
		{
			"LONG_G64R64_UINT", 16, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongU, 2, false,
			64,     	64,      	0,      	0,
			0,			0,			0,			0,
            0, 			64, 		0, 			0
		},

		//118	F_PixelFormat_LONG_G64R64_SINT
		{
			"LONG_G64R64_SINT", 16, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongS, 2, false,
			64,     	64,      	0,      	0,
			0,			0,			0,			0,
            0, 			64, 		0, 			0
		},

		//119	F_PixelFormat_LONG_G64R64_SFLOAT
		{
			"LONG_G64R64_SFLOAT", 16, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Double, 2, false,
			64,     	64,      	0,      	0,
			0,			0,			0,			0,
            0, 			64, 		0, 			0
		},

		//120	F_PixelFormat_LONG_B64G64R64_UINT
		{
			"LONG_B64G64R64_UINT", 24, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongU, 3, false,
			64,     	64,      	64,      	0,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		0
		},

		//121	F_PixelFormat_LONG_B64G64R64_SINT
		{
			"LONG_B64G64R64_SINT", 24, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongS, 3, false,
			64,     	64,      	64,      	0,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		0
		},

		//122	F_PixelFormat_LONG_B64G64R64_SFLOAT
		{
			"LONG_B64G64R64_SFLOAT", 24, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Double, 3, false,
			64,     	64,      	64,      	0,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		0
		},

		//123	F_PixelFormat_LONG_A64B64G32R64_UINT
		{
			"LONG_A64B64G32R64_UINT", 32, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongU, 4, false,
			64,     	64,      	64,      	64,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		192
		},

		//124	F_PixelFormat_LONG_A64B64G32R64_SINT
		{
			"LONG_A64B64G32R64_SINT", 32, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_LongS, 4, false,
			64,     	64,      	64,      	64,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		192
		},

		//125	F_PixelFormat_LONG_A64B64G32R64_SFLOAT
		{
			"LONG_A64B64G32R64_SFLOAT", 32, F_PixelFormatFlag_IsInteger | F_PixelFormatFlag_HasAlpha | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Double, 4, false,
			64,     	64,      	64,      	64,
			0, 			0, 			0, 			0,
            0, 			64, 		128, 		192
		},

		//126	F_PixelFormat_FLOAT_B10G11R11_UFLOAT_PACK32
		{
			"FLOAT_B10G11R11_UFLOAT_PACK32", 4, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 4, false,
			11,     	11,      	10,      	0,
			0, 			0, 			0, 			0,
            0, 			0, 			0, 			0
		},

		//127	F_PixelFormat_FLOAT_E5B9G9R9_UFLOAT_PACK32
		{
			"FLOAT_E5B9G9R9_UFLOAT_PACK32", 4, F_PixelFormatFlag_IsFloat | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 4, false,
			9,     		9,      	9,      	0,
			0xFF000000,	0x00FF0000, 0x0000FF00, 0x000000FF,
            24, 		16,	 		8, 			0
		},

		//128	F_PixelFormat_STENCIL_S8_UINT
		{
			"STENCIL_S8_UINT", 1, F_PixelFormatFlag_IsStencil | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ByteU, 1, false,
			8,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//129	F_PixelFormat_DEPTH_D16_UNORM
		{
			"DEPTH_D16_UNORM", 2, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepth | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_ShortU, 1, false,
			16,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//130	F_PixelFormat_DEPTH_D24_UNORM
		{
			"DEPTH_D24_UNORM", 4, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepth | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_IntU, 1, false,
			32,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//131	F_PixelFormat_DEPTH_D32_SFLOAT
		{
			"DEPTH_D32_SFLOAT", 4, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepth | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 1, false,
			32,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//132	F_PixelFormat_DEPTHSTENCIL_D16_UNORM_S8_UINT
		{
			"DEPTHSTENCIL_D16_UNORM_S8_UINT", 4, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepthStencil | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 1, false,
			32,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//133	F_PixelFormat_DEPTHSTENCIL_D24_UNORM_S8_UINT
		{
			"DEPTHSTENCIL_D24_UNORM_S8_UINT", 4, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepthStencil | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 1, false,
			32,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//134	F_PixelFormat_DEPTHSTENCIL_D32_SFLOAT_S8_UINT
		{
			"DEPTHSTENCIL_D32_SFLOAT_S8_UINT", 4, F_PixelFormatFlag_IsLuminance | F_PixelFormatFlag_IsDepthStencil | F_PixelFormatFlag_IsNative, F_PixelFormatComponent_Float32, 1, false,
			32,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},
		
		//135	F_PixelFormat_DXT1
		{
			"DXT1", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 3, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//136	F_PixelFormat_DXT2
		{
			"DXT2", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//137	F_PixelFormat_DXT3
		{	
			"DXT3", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//138	F_PixelFormat_DXT4
		{
			"DXT4", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//139	F_PixelFormat_DXT5
		{
			"DXT5", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,      	0,      	0,      	0,
			0,      	0,      	0,      	0, 
			0,      	0,      	0,      	0
		},

		//140	F_PixelFormat_BC4_UNORM
		{
			"BC4_UNORM", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 1, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//141	F_PixelFormat_BC4_SNORM
		{
			"BC4_SNORM", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 1, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//142	F_PixelFormat_BC5_UNORM
		{
			"BC5_UNORM", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 2, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//143	F_PixelFormat_BC5_SNORM
		{
			"BC5_SNORM", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 2, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//144	F_PixelFormat_BC6H_UF16
		{
			"BC6H_UF16", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//145	F_PixelFormat_BC6H_SF16
		{
			"BC6H_SF16", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//146	F_PixelFormat_BC7_UNORM
		{
			"BC7_UNORM", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//147	F_PixelFormat_PVRTC_RGB2
		{
			"PVRTC_RGB2", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//148	F_PixelFormat_PVRTC_RGBA2
		{
			"PVRTC_RGBA2", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//149	F_PixelFormat_PVRTC_RGB4
		{
			"PVRTC_RGB4", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//150	F_PixelFormat_PVRTC_RGBA4
		{
			"PVRTC_RGBA4", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//151	F_PixelFormat_PVRTC2_2BPP
		{
			"PVRTC2_2BPP", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//152	F_PixelFormat_PVRTC2_4BPP
		{
			"PVRTC2_4BPP", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//153	F_PixelFormat_ETC1_RGB8
		{
			"ETC1_RGB8", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//154	F_PixelFormat_ETC2_RGB8
		{
			"ETC2_RGB8", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//155	F_PixelFormat_ETC2_RGBA8
		{
			"ETC2_RGBA8", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//156	F_PixelFormat_ETC2_RGB8A1
		{
			"ETC2_RGB8A1", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//157	F_PixelFormat_ATC_RGB
		{
			"ATC_RGB", 0, F_PixelFormatFlag_IsCompressed, F_PixelFormatComponent_ByteU, 3, false,
			0,			0,			0,			0, 
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//158	F_PixelFormat_ATC_RGBA_EXPLICIT_ALPHA
		{
			"ATC_RGBA_EXPLICIT_ALPHA", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//159	F_PixelFormat_ATC_RGBA_INTERPOLATED_ALPHA
		{
			"ATC_RGBA_INTERPOLATED_ALPHA", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//160	F_PixelFormat_ASTC_RGBA_4X4_LDR
		{
			"ASTC_RGBA_4X4_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//161	F_PixelFormat_ASTC_RGBA_5X4_LDR
		{
			"ASTC_RGBA_5X4_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//162	F_PixelFormat_ASTC_RGBA_5X5_LDR
		{
			"ASTC_RGBA_5X5_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//163	F_PixelFormat_ASTC_RGBA_6X5_LDR
		{
			"ASTC_RGBA_6X5_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//164	F_PixelFormat_ASTC_RGBA_6X6_LDR
		{
			"ASTC_RGBA_6X6_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//165	F_PixelFormat_ASTC_RGBA_8X5_LDR
		{
			"ASTC_RGBA_8X5_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//166	F_PixelFormat_ASTC_RGBA_8X6_LDR
		{
			"ASTC_RGBA_8X6_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//167	F_PixelFormat_ASTC_RGBA_8X8_LDR
		{
			"ASTC_RGBA_8X8_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//168	F_PixelFormat_ASTC_RGBA_10X5_LDR
		{
			"ASTC_RGBA_10X5_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//169	F_PixelFormat_ASTC_RGBA_10X6_LDR
		{
			"ASTC_RGBA_10X6_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//170	F_PixelFormat_ASTC_RGBA_10X8_LDR
		{
			"ASTC_RGBA_10X8_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//171	F_PixelFormat_ASTC_RGBA_10X10_LDR
		{
			"ASTC_RGBA_10X10_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//172	F_PixelFormat_ASTC_RGBA_12X10_LDR
		{
			"ASTC_RGBA_12X10_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

		//173	F_PixelFormat_ASTC_RGBA_12X12_LDR
		{
			"ASTC_RGBA_12X12_LDR", 0, F_PixelFormatFlag_IsCompressed | F_PixelFormatFlag_HasAlpha, F_PixelFormatComponent_ByteU, 4, false,
			0,			0,			0,			0,
			0,			0,			0,			0,
			0,			0,			0,			0
		},

	};
	/////////////////////////////////PixelFormat/////////////////////////////////

	const FPixelFormatDes& FPixelFormat::GetPixelFormatDes(const FPixelFormatType& format)
	{
		const int32 type = (int32)format;
		assert(type >= F_PixelFormat_Unknown && type < F_PixelFormat_Count && "FPixelFormat::GetPixelFormatDes");
		return ms_PixelFormats[type];
	}
	FPixelFormatDes& FPixelFormat::GetPixelFormatDesRef(FPixelFormatType format)
	{
		const int32 type = (int32)format;
		assert(type >= F_PixelFormat_Unknown && type < F_PixelFormat_Count && "FPixelFormat::GetPixelFormatDesRef");
		return ms_PixelFormats[type];
	}

    const String& FPixelFormat::GetPixelFormatName(FPixelFormatType format)
	{
		return GetPixelFormatDes(format).name;
	}
	size_t FPixelFormat::GetPixelFormatElemBytes(FPixelFormatType format)
	{
		return GetPixelFormatDes(format).nElemBytes;
	}
	size_t FPixelFormat::GetPixelFormatElemBits(FPixelFormatType format)
	{
		return GetPixelFormatDes(format).nElemBytes * 8;
	}
    uint32 FPixelFormat::GetPixelFormatFlags(FPixelFormatType format)
	{	
		return GetPixelFormatDes(format).eFlags;
	}
    FPixelFormatComponentType FPixelFormat::GetPixelFormatComponentType(FPixelFormatType format)
	{
		return (FPixelFormatComponentType)GetPixelFormatDes(format).eComponentType;
	}
	size_t FPixelFormat::GetPixelFormatComponentCount(FPixelFormatType format)
	{
		return GetPixelFormatDes(format).nComponentCount;
	}
    void FPixelFormat::GetPixelFormatBitDepths(FPixelFormatType format, int32 rgba[4])
	{
		const FPixelFormatDes &des = GetPixelFormatDes(format);
		rgba[0] = des.nRbits;
		rgba[1] = des.nGbits;
		rgba[2] = des.nBbits;
		rgba[3] = des.nAbits;
	}
	void FPixelFormat::GetPixelFormatBitMasks(FPixelFormatType format, uint64 rgba[4])
	{		
		const FPixelFormatDes &des = GetPixelFormatDes(format);
		rgba[0] = des.nRmask;
		rgba[1] = des.nGmask;
		rgba[2] = des.nBmask;
		rgba[3] = des.nAmask;
	}
	void FPixelFormat::GetPixelFormatBitShifts(FPixelFormatType format, uint8 rgba[4])
	{
		const FPixelFormatDes &des = GetPixelFormatDes(format);
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
	size_t FPixelFormat::GetPixelFormatMemorySize(size_t width, size_t height, size_t depth, FPixelFormatType format)
	{
		if (IsCompressed(format))
		{
			switch((int32)format)
			{
			//DXT
			case F_PixelFormat_DXT1:
				return ((width + 3) / 4) * ((height + 3) / 4) * 8 * depth;
			case F_PixelFormat_DXT2:
			case F_PixelFormat_DXT3:
			case F_PixelFormat_DXT4:
			case F_PixelFormat_DXT5:
				return ((width + 3) / 4) * ((height + 3) / 4) * 16 * depth;

			case F_PixelFormat_BC4_UNORM:
			case F_PixelFormat_BC4_SNORM:
				return ((width + 3) / 4) * ((height + 3) / 4) * 8 * depth;
			case F_PixelFormat_BC5_UNORM:
			case F_PixelFormat_BC5_SNORM:
			case F_PixelFormat_BC6H_UF16:
			case F_PixelFormat_BC6H_SF16:
			case F_PixelFormat_BC7_UNORM:
				return ((width + 3) / 4) * ((height + 3) / 4) * 16 * depth;

            //ASTC
			case F_PixelFormat_ASTC_RGBA_4X4_LDR:
				return astc_slice_size(width, height, 4, 4) * depth;
			case F_PixelFormat_ASTC_RGBA_5X4_LDR:
				return astc_slice_size(width, height, 5, 4) * depth;
			case F_PixelFormat_ASTC_RGBA_5X5_LDR:
				return astc_slice_size(width, height, 5, 5) * depth;
			case F_PixelFormat_ASTC_RGBA_6X5_LDR:
				return astc_slice_size(width, height, 6, 5) * depth;
			case F_PixelFormat_ASTC_RGBA_6X6_LDR:
				return astc_slice_size(width, height, 6, 6) * depth;
			case F_PixelFormat_ASTC_RGBA_8X5_LDR:
				return astc_slice_size(width, height, 8, 5) * depth;
			case F_PixelFormat_ASTC_RGBA_8X6_LDR:
				return astc_slice_size(width, height, 8, 6) * depth;
			case F_PixelFormat_ASTC_RGBA_8X8_LDR:
				return astc_slice_size(width, height, 8, 8) * depth;
			case F_PixelFormat_ASTC_RGBA_10X5_LDR:
				return astc_slice_size(width, height, 10, 5) * depth;
			case F_PixelFormat_ASTC_RGBA_10X6_LDR:
				return astc_slice_size(width, height, 10, 6) * depth;
			case F_PixelFormat_ASTC_RGBA_10X8_LDR:
				return astc_slice_size(width, height, 10, 8) * depth;
			case F_PixelFormat_ASTC_RGBA_10X10_LDR:
				return astc_slice_size(width, height, 10, 10) * depth;
			case F_PixelFormat_ASTC_RGBA_12X10_LDR:
				return astc_slice_size(width, height, 12, 10) * depth;
			case F_PixelFormat_ASTC_RGBA_12X12_LDR:
				return astc_slice_size(width, height, 12, 12) * depth;

			default:
				{
					assert(false && "FPixelFormat::GetPixelFormatMemorySize, Invalid compressed pixel format");
					return 0;
				}
			}
		}
		
		return width * height * depth * GetPixelFormatElemBytes(format);
	}   


	bool FPixelFormat::IsNative(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsNative) > 0;
	}
	bool FPixelFormat::IsCompressed(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsCompressed) > 0;
	}
	bool FPixelFormat::IsInteger(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsInteger) > 0;
	}
	bool FPixelFormat::IsFloat(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsFloat) > 0;
	}
	bool FPixelFormat::IsLuminance(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsLuminance) > 0;
	}
	bool FPixelFormat::IsStencil(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsStencil) > 0;
	}
	bool FPixelFormat::IsDepth(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsDepth) > 0;
	}
	bool FPixelFormat::IsDepthStencil(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_IsDepthStencil) > 0;
	}
	bool FPixelFormat::HasAlpha(FPixelFormatType format)
	{
		return (FPixelFormat::GetPixelFormatFlags(format) & F_PixelFormatFlag_HasAlpha) > 0;
	}
	

	bool FPixelFormat::IsValidExtent(size_t width, size_t height, size_t depth, FPixelFormatType format)
	{
		if (IsCompressed(format))
		{
			switch(format)
			{
			case F_PixelFormat_DXT1:
			case F_PixelFormat_DXT2:
			case F_PixelFormat_DXT3:
			case F_PixelFormat_DXT4:
			case F_PixelFormat_DXT5:
				return ((width & 3) == 0 && (height & 3) == 0 && depth == 1);
			default:
				return true;
			}
		}

		return true;
	}
	bool FPixelFormat::IsAccessible(FPixelFormatType format)
	{
		return (format != F_PixelFormat_Unknown) && !IsCompressed(format);
	}

	FPixelFormatType FPixelFormat::ParsePixelFormatFromName(const String& strName, bool accessibleOnly /*= false*/, bool caseSensitive /*= false*/)
	{
		String tmp = strName;
		if (!caseSensitive)
		{
			FUtilString::ToUpperCase(tmp);
		}

        int32 count = (int32)F_PixelFormat_Count;
		for (int32 i = 0; i < count; ++i)
		{
			FPixelFormatType format = static_cast<FPixelFormatType>(i);
			if (!accessibleOnly || IsAccessible(format))
			{
				if (tmp == GetPixelFormatName(format))
					return format;
			}
		}
        if(tmp == String("BYTE_RGB_UNORM"))
            return F_PixelFormat_BYTE_RGB_UNORM;
        if(tmp == String("BYTE_RGBA_UNORM"))
            return F_PixelFormat_BYTE_RGBA_UNORM;
        if(tmp == String("BYTE_BGR_UNORM"))
            return F_PixelFormat_BYTE_BGR_UNORM;
        if(tmp == String("BYTE_BGRA_UNORM"))
            return F_PixelFormat_BYTE_BGRA_UNORM;

		return F_PixelFormat_Unknown;
	}

	FPixelFormatType FPixelFormat::ParsePixelFormatForBitDepths(FPixelFormatType format, uint16 integerBits, uint16 floatBits)
	{
		switch (integerBits)
		{
		case 16:
			{
				switch ((int32)format)
				{
				case F_PixelFormat_BYTE_R8G8B8_UNORM:
					return F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16;

				case F_PixelFormat_BYTE_B8G8R8_UNORM:
					return F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16;

				case F_PixelFormat_BYTE_A8B8G8R8_UNORM_PACK32:
				case F_PixelFormat_BYTE_A8R8G8B8_UNORM:
				case F_PixelFormat_BYTE_A8B8G8R8_UNORM:
					return F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16;

				case F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:
				case F_PixelFormat_BYTE_A2B10G10R10_UNORM_PACK32:
					return F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16;

				default:
					break;
				}
			}
			break;

		case 32:
			{
				switch ((int32)format)
				{
				case F_PixelFormat_BYTE_R5G6B5_UNORM_PACK16:
					return F_PixelFormat_BYTE_A8R8G8B8_UNORM;

				case F_PixelFormat_BYTE_B5G6R5_UNORM_PACK16:
					return F_PixelFormat_BYTE_A8B8G8R8_UNORM;

				case F_PixelFormat_BYTE_R4G4B4A4_UNORM_PACK16:
					return F_PixelFormat_BYTE_A8R8G8B8_UNORM;

				case F_PixelFormat_BYTE_A1R5G5B5_UNORM_PACK16:
					return F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32;

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
				case F_PixelFormat_INT_R32_SFLOAT:
					return F_PixelFormat_SHORT_R16_SFLOAT;

				case F_PixelFormat_INT_B32G32R32_SFLOAT:
					return F_PixelFormat_SHORT_B16G16R16_SFLOAT; 

				case F_PixelFormat_INT_A32B32G32R32_SFLOAT:
					return F_PixelFormat_SHORT_A16B16G16R16_SFLOAT;

				default:
					break;
				}
			}
			break;

		case 32:
			{
				switch ((int32)format)
				{
				case F_PixelFormat_SHORT_R16_SFLOAT:
					return F_PixelFormat_INT_R32_SFLOAT;

				case F_PixelFormat_SHORT_B16G16R16_SFLOAT:
					return F_PixelFormat_INT_B32G32R32_SFLOAT;

				case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
					return F_PixelFormat_INT_A32B32G32R32_SFLOAT;

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

	void FPixelFormat::PackColor(const FColor &color, const FPixelFormatType format, void* dest)
	{
		PackColor(color.r, color.g, color.b, color.a, format, dest);
	}	

	void FPixelFormat::PackColor(const uint8 r, const uint8 g, const uint8 b, const uint8 a, const FPixelFormatType format, void* dest)
	{
		const FPixelFormatDes& des = GetPixelFormatDes(format);
		if(des.eFlags & F_PixelFormatFlag_IsNative) 
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

	void FPixelFormat::PackColor(const float r, const float g, const float b, const float a, const FPixelFormatType format, void* dest)
	{
		const FPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & F_PixelFormatFlag_IsNative) 
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
			case F_PixelFormat_INT_R32_SFLOAT:
				((float*)dest)[0] = r;
				break;
			case F_PixelFormat_INT_G32R32_SFLOAT:
				((float*)dest)[0] = r;
				((float*)dest)[1] = g;
				break;
			case F_PixelFormat_INT_B32G32R32_SFLOAT:
				((float*)dest)[0] = r;
				((float*)dest)[1] = g;
				((float*)dest)[2] = b;
				break;
			case F_PixelFormat_INT_A32B32G32R32_SFLOAT:
				((float*)dest)[0] = r;
				((float*)dest)[1] = g;
				((float*)dest)[2] = b;
				((float*)dest)[3] = a;
				break;
			case F_PixelFormat_DEPTH_D16_UNORM:
			case F_PixelFormat_SHORT_R16_SFLOAT:
				((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
				break;
			case F_PixelFormat_SHORT_G16R16_SFLOAT:
				((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
				((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
				break;
			case F_PixelFormat_SHORT_B16G16R16_SFLOAT:
				((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
				((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
				((uint16*)dest)[2] = FBitwise::FloatToHalf(b);
				break;
			case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
				((uint16*)dest)[0] = FBitwise::FloatToHalf(r);
				((uint16*)dest)[1] = FBitwise::FloatToHalf(g);
				((uint16*)dest)[2] = FBitwise::FloatToHalf(b);
				((uint16*)dest)[3] = FBitwise::FloatToHalf(a);
				break;
			case F_PixelFormat_SHORT_B16G16R16_UNORM:
				((uint16*)dest)[0] = (uint16)FBitwise::FloatToFixed(r, 16);
				((uint16*)dest)[1] = (uint16)FBitwise::FloatToFixed(g, 16);
				((uint16*)dest)[2] = (uint16)FBitwise::FloatToFixed(b, 16);
				break;
			case F_PixelFormat_SHORT_A16B16G16R16_UNORM:
				((uint16*)dest)[0] = (uint16)FBitwise::FloatToFixed(r, 16);
				((uint16*)dest)[1] = (uint16)FBitwise::FloatToFixed(g, 16);
				((uint16*)dest)[2] = (uint16)FBitwise::FloatToFixed(b, 16);
				((uint16*)dest)[3] = (uint16)FBitwise::FloatToFixed(a, 16);
				break;
			case F_PixelFormat_BYTE_AL_UNORM:
				((uint8*)dest)[0] = (uint8)FBitwise::FloatToFixed(r, 8);
				((uint8*)dest)[1] = (uint8)FBitwise::FloatToFixed(a, 8);
				break;
			case F_PixelFormat_BYTE_A8_UNORM:
				((uint8*)dest)[0] = (uint8)FBitwise::FloatToFixed(r, 8);
                break;
            case F_PixelFormat_BYTE_A2R10G10B10_UNORM_PACK32:
            {
                const uint16 ir = static_cast<uint16>(FMath::Saturate(r) * 1023.0f + 0.5f);
                const uint16 ig = static_cast<uint16>(FMath::Saturate(g) * 1023.0f + 0.5f);
                const uint16 ib = static_cast<uint16>(FMath::Saturate(b) * 1023.0f + 0.5f);
                const uint16 ia = static_cast<uint16>(FMath::Saturate(a) * 3.0f + 0.5f);

                ((uint32*)dest)[0] = (ia << 30u) | (ir << 20u) | (ig << 10u) | (ib);
                break;
            }
			default:
				{
					F_LogError("*********************** FPixelFormat::PackColor: pack to [%s] not implemented", GetPixelFormatName(format).c_str());
					F_Assert(false && "FPixelFormat::PackColor")
				}
				break;
			}
		}
	}

	void FPixelFormat::UnpackColor(FColor& color, FPixelFormatType format, const void* src)
	{
		UnpackColor(&color.r, &color.g, &color.b, &color.a, format, src);
	}

	void FPixelFormat::UnpackColor(FColor* color, FPixelFormatType format, const void* src)
	{
		UnpackColor(&color->r, &color->g, &color->b, &color->a, format, src);
	}

	void FPixelFormat::UnpackColor(uint8 *r, uint8 *g, uint8 *b, uint8 *a, FPixelFormatType format, const void* src)
	{
		const FPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & F_PixelFormatFlag_IsNative) 
		{
			const uint32 value = FBitwise::IntRead(src, des.nElemBytes);
			if (des.eFlags & F_PixelFormatFlag_IsLuminance)
			{
				*r = *g = *b = (uint8)FBitwise::FixedToFixed((value & des.nRmask)>>des.nRshift, des.nRbits, 8);
			}
			else
			{
				*r = (uint8)FBitwise::FixedToFixed((value & des.nRmask)>>des.nRshift, des.nRbits, 8);
				*g = (uint8)FBitwise::FixedToFixed((value & des.nGmask)>>des.nGshift, des.nGbits, 8);
				*b = (uint8)FBitwise::FixedToFixed((value & des.nBmask)>>des.nBshift, des.nBbits, 8);
			}
			if (des.eFlags & F_PixelFormatFlag_HasAlpha)
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

	void FPixelFormat::UnpackColor(float *r, float *g, float *b, float *a, FPixelFormatType format, const void* src)
	{
		const FPixelFormatDes& des = GetPixelFormatDes(format);
		if (des.eFlags & F_PixelFormatFlag_IsNative) 
		{
			const uint32 value = FBitwise::IntRead(src, des.nElemBytes);
			if (des.eFlags & F_PixelFormatFlag_IsLuminance)
			{
				*r = *g = *b = FBitwise::FixedToFloat((value & des.nRmask)>>des.nRshift, des.nRbits);
			}
			else
			{
				*r = FBitwise::FixedToFloat((value & des.nRmask)>>des.nRshift, des.nRbits);
				*g = FBitwise::FixedToFloat((value & des.nGmask)>>des.nGshift, des.nGbits);
				*b = FBitwise::FixedToFloat((value & des.nBmask)>>des.nBshift, des.nBbits);
			}
			if (des.eFlags & F_PixelFormatFlag_HasAlpha)
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
			case F_PixelFormat_INT_R32_SFLOAT:
				*r = *g = *b = ((float*)src)[0];
				*a = 1.0f;
				break;
			case F_PixelFormat_INT_G32R32_SFLOAT:
				*r = ((float*)src)[0];
				*g = *b = ((float*)src)[1];
				*a = 1.0f;
				break;
			case F_PixelFormat_INT_B32G32R32_SFLOAT:
				*r = ((float*)src)[0];
				*g = ((float*)src)[1];
				*b = ((float*)src)[2];
				*a = 1.0f;
				break;
			case F_PixelFormat_INT_A32B32G32R32_SFLOAT:
				*r = ((float*)src)[0];
				*g = ((float*)src)[1];
				*b = ((float*)src)[2];
				*a = ((float*)src)[3];
				break;
			case F_PixelFormat_SHORT_R16_SFLOAT:
				*r = *g = *b = FBitwise::HalfToFloat(((uint16*)src)[0]);
				*a = 1.0f;
				break;
			case F_PixelFormat_SHORT_G16R16_SFLOAT:
				*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
				*g = *b = FBitwise::HalfToFloat(((uint16*)src)[1]);
				*a = 1.0f;
				break;
			case F_PixelFormat_SHORT_B16G16R16_SFLOAT:
				*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
				*g = FBitwise::HalfToFloat(((uint16*)src)[1]);
				*b = FBitwise::HalfToFloat(((uint16*)src)[2]);
				*a = 1.0f;
				break;
			case F_PixelFormat_SHORT_A16B16G16R16_SFLOAT:
				*r = FBitwise::HalfToFloat(((uint16*)src)[0]);
				*g = FBitwise::HalfToFloat(((uint16*)src)[1]);
				*b = FBitwise::HalfToFloat(((uint16*)src)[2]);
				*a = FBitwise::HalfToFloat(((uint16*)src)[3]);
				break;
			case F_PixelFormat_SHORT_B16G16R16_UNORM:
				*r = FBitwise::FixedToFloat(((uint16*)src)[0], 16);
				*g = FBitwise::FixedToFloat(((uint16*)src)[1], 16);
				*b = FBitwise::FixedToFloat(((uint16*)src)[2], 16);
				*a = 1.0f;
				break;
			case F_PixelFormat_SHORT_A16B16G16R16_UNORM:
				*r = FBitwise::FixedToFloat(((uint16*)src)[0], 16);
				*g = FBitwise::FixedToFloat(((uint16*)src)[1], 16);
				*b = FBitwise::FixedToFloat(((uint16*)src)[2], 16);
				*a = FBitwise::FixedToFloat(((uint16*)src)[3], 16);
				break;
			case F_PixelFormat_BYTE_AL_UNORM:
				*r = *g = *b = FBitwise::FixedToFloat(((uint8*)src)[0], 8);
				*a = FBitwise::FixedToFloat(((uint8*)src)[1], 8);
				break;
			default:
				{
					F_LogError("*********************** FPixelFormat::UnpackColor: unpack from [%s] not implemented", GetPixelFormatName(format).c_str());
					F_Assert(false && "FPixelFormat::UnpackColor")
				}
				break;
			}
		}
	}

	void FPixelFormat::BulkPixelConversion(void *src, FPixelFormatType srcFormat, void *dest, FPixelFormatType dstFormat, uint32 count)
	{
		FPixelBox src_box(count, 1, 1, srcFormat, src);
		FPixelBox dst_box(count, 1, 1, dstFormat, dest);
		BulkPixelConversion(src_box, dst_box);
	}

	void FPixelFormat::BulkPixelConversion(const FPixelBox &src, const FPixelBox &dst)
	{
		assert(src.GetWidth() == dst.GetWidth() &&
			   src.GetHeight() == dst.GetHeight() &&
			   src.GetDepth() == dst.GetDepth() && 
               "FPixelFormat::BulkPixelConversion");

		if (FPixelFormat::IsCompressed(src.m_ePixelFormat) || FPixelFormat::IsCompressed(dst.m_ePixelFormat))
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
				F_LogError("*********************** FPixelFormat::BulkPixelConversion: This method can not be used to compress or decompress images");
				F_Assert(false && "FPixelFormat::BulkPixelConversion")
			}
		}

		if (src.m_ePixelFormat == dst.m_ePixelFormat) 
		{
			if (src.IsConsecutive() && dst.IsConsecutive())
			{
				memcpy(dst.GetTopLeftFrontPixelPtr(), src.GetTopLeftFrontPixelPtr(), src.GetConsecutiveSize());
                return;
			}

			const size_t srcPixelSize = FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat);
			const size_t dstPixelSize = FPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat);
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
        
		const size_t srcPixelSize = FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat);
		const size_t dstPixelSize = FPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat);
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

	void FPixelFormat::BulkPixelVerticalFlip(const FPixelBox& box)
	{
        if (FPixelFormat::IsCompressed(box.m_ePixelFormat))
        {
            assert(false && "FPixelFormat::BulkPixelVerticalFlip: This method can not be used for compressed format !");
            return;
        }
        
        const size_t pixelSize = FPixelFormat::GetPixelFormatElemBytes(box.m_ePixelFormat);
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
    
}; //LostPeterFoundation