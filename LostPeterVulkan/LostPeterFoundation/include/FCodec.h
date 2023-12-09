/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-12-06
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_CODEC_H_
#define _F_CODEC_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	class foundationExport FCodec
	{
	public:
        FCodec();
		virtual ~FCodec();

	public:
		class foundationExport FCodecData 
		{
		public:
			FCodecData();
			virtual ~FCodecData();

		public:
			virtual String GetDataType() const;
		};

    public:
        typedef std::pair<FFileMemory*, FCodecData*> FDecodeResult;
		
	public:
		virtual String GetCodecType() const = 0;
		virtual String GetDataType() const = 0;
		
		virtual String MagicNumberToFileExt(const char* szMagicNum, size_t nMaxbytes) const = 0;
		virtual bool MagicNumberMatch(const char* szMagicNum, size_t nMaxbytes) const 
		{ 
			return !MagicNumberToFileExt(szMagicNum,nMaxbytes).empty();
		}

		virtual FFileMemory* Code(FFileMemory* pInput, FCodecData* pCodecData) const = 0;
		virtual bool CodeToFile(FFileMemory* pInput, const String& outFilePath, FCodecData* pCodecData) const = 0;

		virtual FDecodeResult Decode(FFileMemory* pInput) const = 0;
	};

}; //LostPeterFoundation

#endif