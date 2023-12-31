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

#ifndef _IMAGE_CODEC_H_
#define _IMAGE_CODEC_H_

#include "PreDefine.h"

namespace LostPeterEngine
{
    class engineExport ImageCodec : public FCodec
	{
	public:
        ImageCodec();
		virtual ~ImageCodec();

	protected:
		static void FlipEndian(void* pData, size_t size, size_t count)
        {
#if F_ENDIAN == F_ENDIAN_BIG
            FBitwise::SwapChunks(pData, size, count);
#endif
        }
        static void FlipEndian(void* pData, size_t size)
        {
#if F_ENDIAN == F_ENDIAN_BIG
            FBitwise::SwapBuffer(pData, size);
#endif
        }

	public:
		class engineExport ImageData : public FCodec::FCodecData
		{
		public:
			ImageData();
            virtual ~ImageData();

		public:
			int32 nHeight;
			int32 nWidth;
			int32 nDepth;
			int32 nSize;
			
			uint16 nMipmapsCount;
			uint32 nFlags;
			FPixelFormatType ePixelFormat;

		public:
			virtual String GetDataType() const;
		};

	public:
		virtual String GetDataType() const;
	};

}; //LostPeterEngine

#endif