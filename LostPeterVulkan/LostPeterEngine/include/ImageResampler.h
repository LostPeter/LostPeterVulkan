/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _IMAGE_RESAMPLER_H_
#define _IMAGE_RESAMPLER_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
	template<unsigned int elemsize> 
	struct ImageNearestResampler 
	{
		static void Scale(const FPixelBox& src, const FPixelBox& dst) 
		{
			uint8* srcdata = (uint8*)src.m_pData;
			uint8* pdst = (uint8*)dst.m_pData;
			uint64 stepx = ((uint64)src.GetWidth() << 48) / dst.GetWidth();
			uint64 stepy = ((uint64)src.GetHeight() << 48) / dst.GetHeight();
			uint64 stepz = ((uint64)src.GetDepth() << 48) / dst.GetDepth();
			uint64 sz_48 = (stepz >> 1) - 1;
			for (size_t z = dst.m_nFront; z < dst.m_nBack; z++, sz_48 += stepz) 
			{
				size_t srczoff = (size_t)(sz_48 >> 48) * src.m_nSlicePitch;
				uint64 sy_48 = (stepy >> 1) - 1;
				for (size_t y = dst.m_nTop; y < dst.m_nBottom; y++, sy_48 += stepy)
				{
					size_t srcyoff = (size_t)(sy_48 >> 48) * src.m_nRowPitch;

					uint64 sx_48 = (stepx >> 1) - 1;
					for (size_t x = dst.m_nLeft; x < dst.m_nRight; x++, sx_48 += stepx)
					{
						uint8* psrc = srcdata + elemsize*((size_t)(sx_48 >> 48) + srcyoff + srczoff);
						memcpy(pdst, psrc, elemsize);
						pdst += elemsize;
					}
					pdst += elemsize * dst.GetRowSkip();
				}
				pdst += elemsize * dst.GetSliceSkip();
			}
		}
	};


	struct ImageLinearResampler 
	{
		static void Scale(const FPixelBox& src, const FPixelBox& dst)
		{
			size_t srcelemsize = FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat);
			size_t dstelemsize = FPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat);
			uint8* srcdata = (uint8*)src.m_pData;
			uint8* pdst = (uint8*)dst.m_pData;
			uint64 stepx = ((uint64)src.GetWidth() << 48) / dst.GetWidth();
			uint64 stepy = ((uint64)src.GetHeight() << 48) / dst.GetHeight();
			uint64 stepz = ((uint64)src.GetDepth() << 48) / dst.GetDepth();

			unsigned int temp;
			uint64 sz_48 = (stepz >> 1) - 1;
			for (size_t z = dst.m_nFront; z < dst.m_nBack; z++, sz_48+=stepz) 
			{
				temp = sz_48 >> 32;
				temp = (temp > 0x8000)? temp - 0x8000 : 0;
				size_t sz1 = temp >> 16;				 
				size_t sz2 = FMath::Min(sz1 + 1, src.GetDepth()-1);
				float szf = (temp & 0xFFFF) / 65536.f;

				uint64 sy_48 = (stepy >> 1) - 1;
				for (size_t y = dst.m_nTop; y < dst.m_nBottom; y++, sy_48+=stepy) 
				{
					temp = sy_48 >> 32;
					temp = (temp > 0x8000)? temp - 0x8000 : 0;
					size_t sy1 = temp >> 16;					
					size_t sy2 = FMath::Min(sy1 + 1, src.GetHeight() - 1);
					float syf = (temp & 0xFFFF) / 65536.f; 

					uint64 sx_48 = (stepx >> 1) - 1;
					for (size_t x = dst.m_nLeft; x < dst.m_nRight; x++, sx_48+=stepx) 
					{
						temp = sx_48 >> 32;
						temp = (temp > 0x8000)? temp - 0x8000 : 0;
						size_t sx1 = temp >> 16;					
						size_t sx2 = FMath::Min(sx1 + 1, src.GetWidth() - 1);
						float sxf = (temp & 0xFFFF) / 65536.f; 

						FColor x1y1z1, x2y1z1, x1y2z1, x2y2z1;
						FColor x1y1z2, x2y1z2, x1y2z2, x2y2z2;

#define UNPACK(dst,x,y,z) FPixelFormat::UnpackColor(&dst, src.m_ePixelFormat, \
	srcdata + srcelemsize*((x)+(y)*src.m_nRowPitch+(z)*src.m_nSlicePitch))

						UNPACK(x1y1z1,sx1,sy1,sz1); UNPACK(x2y1z1,sx2,sy1,sz1);
						UNPACK(x1y2z1,sx1,sy2,sz1); UNPACK(x2y2z1,sx2,sy2,sz1);
						UNPACK(x1y1z2,sx1,sy1,sz2); UNPACK(x2y1z2,sx2,sy1,sz2);
						UNPACK(x1y2z2,sx1,sy2,sz2); UNPACK(x2y2z2,sx2,sy2,sz2);
#undef UNPACK

						FColor accum =
							x1y1z1 * ((1.0f - sxf)*(1.0f - syf)*(1.0f - szf)) +
							x2y1z1 * (        sxf *(1.0f - syf)*(1.0f - szf)) +
							x1y2z1 * ((1.0f - sxf)*        syf *(1.0f - szf)) +
							x2y2z1 * (        sxf *        syf *(1.0f - szf)) +
							x1y1z2 * ((1.0f - sxf)*(1.0f - syf)*        szf ) +
							x2y1z2 * (        sxf *(1.0f - syf)*        szf ) +
							x1y2z2 * ((1.0f - sxf)*        syf *        szf ) +
							x2y2z2 * (        sxf *        syf *        szf );

						FPixelFormat::PackColor(accum, dst.m_ePixelFormat, pdst);

						pdst += dstelemsize;
					}
					pdst += dstelemsize * dst.GetRowSkip();
				}
				pdst += dstelemsize * dst.GetSliceSkip();
			}
		}
	};


	struct ImageLinearResampler_Float32 
	{
		static void Scale(const FPixelBox& src, const FPixelBox& dst)
		{
			size_t srcchannels = FPixelFormat::GetPixelFormatElemBytes(src.m_ePixelFormat) / sizeof(float);
			size_t dstchannels = FPixelFormat::GetPixelFormatElemBytes(dst.m_ePixelFormat) / sizeof(float);
			float* srcdata = (float*)src.m_pData;
			float* pdst = (float*)dst.m_pData;
			uint64 stepx = ((uint64)src.GetWidth() << 48) / dst.GetWidth();
			uint64 stepy = ((uint64)src.GetHeight() << 48) / dst.GetHeight();
			uint64 stepz = ((uint64)src.GetDepth() << 48) / dst.GetDepth();

			unsigned int temp;
			uint64 sz_48 = (stepz >> 1) - 1;
			for (size_t z = dst.m_nFront; z < dst.m_nBack; z++, sz_48+=stepz) 
			{
				temp = sz_48 >> 32;
				temp = (temp > 0x8000)? temp - 0x8000 : 0;
				size_t sz1 = temp >> 16;				 
				size_t sz2 = FMath::Min(sz1 + 1, src.GetDepth() - 1);
				float szf = (temp & 0xFFFF) / 65536.f; 

				uint64 sy_48 = (stepy >> 1) - 1;
				for (size_t y = dst.m_nTop; y < dst.m_nBottom; y++, sy_48+=stepy)
				{
					temp = sy_48 >> 32;
					temp = (temp > 0x8000)? temp - 0x8000 : 0;
					size_t sy1 = temp >> 16;					
					size_t sy2 = FMath::Min(sy1 + 1, src.GetHeight() - 1);
					float syf = (temp & 0xFFFF) / 65536.f; 

					uint64 sx_48 = (stepx >> 1) - 1;
					for (size_t x = dst.m_nLeft; x < dst.m_nRight; x++, sx_48+=stepx)
					{
						temp = sx_48 >> 32;
						temp = (temp > 0x8000)? temp - 0x8000 : 0;
						size_t sx1 = temp >> 16;					
						size_t sx2 = FMath::Min(sx1 + 1, src.GetWidth() - 1);
						float sxf = (temp & 0xFFFF) / 65536.f; 
						float accum[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

#define ACCUM3(x,y,z,factor) \
						{ float f = factor; \
						size_t off = (x+y*src.m_nRowPitch+z*src.m_nSlicePitch)*srcchannels; \
						accum[0]+=srcdata[off+0]*f; accum[1]+=srcdata[off+1]*f; \
						accum[2]+=srcdata[off+2]*f; }

#define ACCUM4(x,y,z,factor) \
						{ float f = factor; \
						size_t off = (x+y*src.m_nRowPitch+z*src.m_nSlicePitch)*srcchannels; \
						accum[0]+=srcdata[off+0]*f; accum[1]+=srcdata[off+1]*f; \
						accum[2]+=srcdata[off+2]*f; accum[3]+=srcdata[off+3]*f; }

						if (srcchannels == 3 || dstchannels == 3) 
						{
							// RGB, no alpha
							ACCUM3(sx1,sy1,sz1,(1.0f-sxf)*(1.0f-syf)*(1.0f-szf));
							ACCUM3(sx2,sy1,sz1,      sxf *(1.0f-syf)*(1.0f-szf));
							ACCUM3(sx1,sy2,sz1,(1.0f-sxf)*      syf *(1.0f-szf));
							ACCUM3(sx2,sy2,sz1,      sxf *      syf *(1.0f-szf));
							ACCUM3(sx1,sy1,sz2,(1.0f-sxf)*(1.0f-syf)*      szf );
							ACCUM3(sx2,sy1,sz2,      sxf *(1.0f-syf)*      szf );
							ACCUM3(sx1,sy2,sz2,(1.0f-sxf)*      syf *      szf );
							ACCUM3(sx2,sy2,sz2,      sxf *      syf *      szf );
							accum[3] = 1.0f;
						} 
						else
						{
							// RGBA
							ACCUM4(sx1,sy1,sz1,(1.0f-sxf)*(1.0f-syf)*(1.0f-szf));
							ACCUM4(sx2,sy1,sz1,      sxf *(1.0f-syf)*(1.0f-szf));
							ACCUM4(sx1,sy2,sz1,(1.0f-sxf)*      syf *(1.0f-szf));
							ACCUM4(sx2,sy2,sz1,      sxf *      syf *(1.0f-szf));
							ACCUM4(sx1,sy1,sz2,(1.0f-sxf)*(1.0f-syf)*      szf );
							ACCUM4(sx2,sy1,sz2,      sxf *(1.0f-syf)*      szf );
							ACCUM4(sx1,sy2,sz2,(1.0f-sxf)*      syf *      szf );
							ACCUM4(sx2,sy2,sz2,      sxf *      syf *      szf );
						}

						memcpy(pdst, accum, sizeof(float) * dstchannels);

#undef ACCUM3
#undef ACCUM4

						pdst += dstchannels;
					}
					pdst += dstchannels * dst.GetRowSkip();
				}
				pdst += dstchannels * dst.GetSliceSkip();
			}
		}
	};


	template<unsigned int channels> 
	struct ImageLinearResampler_Byte 
	{
		static void Scale(const FPixelBox& src,const FPixelBox& dst)
		{
			if (src.GetDepth() > 1 || dst.GetDepth() > 1)
			{
				ImageLinearResampler::Scale(src, dst);
				return;
			}
			uint8* srcdata = (uint8*)src.m_pData;
			uint8* pdst = (uint8*)dst.m_pData;
			uint64 stepx = ((uint64)src.GetWidth() << 48) / dst.GetWidth();
			uint64 stepy = ((uint64)src.GetHeight() << 48) / dst.GetHeight();
			unsigned int temp;

			uint64 sy_48 = (stepy >> 1) - 1;
			for (size_t y = dst.m_nTop; y < dst.m_nBottom; y++, sy_48+=stepy)
			{
				temp = sy_48 >> 36;
				temp = (temp > 0x800)? temp - 0x800: 0;
				unsigned int syf = temp & 0xFFF;
				size_t sy1 = temp >> 12;
				size_t sy2 = FMath::Min(sy1 + 1, src.m_nBottom - src.m_nTop - 1);
				size_t syoff1 = sy1 * src.m_nRowPitch;
				size_t syoff2 = sy2 * src.m_nRowPitch;

				uint64 sx_48 = (stepx >> 1) - 1;
				for (size_t x = dst.m_nLeft; x < dst.m_nRight; x++, sx_48+=stepx)
				{
					temp = sx_48 >> 36;
					temp = (temp > 0x800)? temp - 0x800 : 0;
					unsigned int sxf = temp & 0xFFF;
					size_t sx1 = temp >> 12;
					size_t sx2 = FMath::Min(sx1+1, src.m_nRight-src.m_nLeft-1);

					unsigned int sxfsyf = sxf*syf;
					for (unsigned int k = 0; k < channels; k++) 
					{
						unsigned int accum =
							srcdata[(sx1 + syoff1)*channels+k]*(0x1000000-(sxf<<12)-(syf<<12)+sxfsyf) +
							srcdata[(sx2 + syoff1)*channels+k]*((sxf<<12)-sxfsyf) +
							srcdata[(sx1 + syoff2)*channels+k]*((syf<<12)-sxfsyf) +
							srcdata[(sx2 + syoff2)*channels+k]*sxfsyf;
						*pdst++ = (accum + 0x800000) >> 24;
					}
				}
				pdst += channels*dst.GetRowSkip();
			}
		}
	};	

}; //LostPeterEngine

#endif