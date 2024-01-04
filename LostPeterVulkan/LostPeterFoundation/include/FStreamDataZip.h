/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_STREAM_DATA_ZIP_H_
#define _F_STREAM_DATA_ZIP_H_

#include "FStreamData.h"

typedef struct zzip_dir	ZZIP_DIR;
typedef struct zzip_file ZZIP_FILE;

namespace LostPeterFoundation
{
	class foundationExport FStreamDataZip : public FStreamData
	{
	public:
		FStreamDataZip(ZZIP_FILE* pZzipFile, size_t nUncompressedSize);
		FStreamDataZip(const String& strName, ZZIP_FILE* pZzipFile, size_t nUncompressedSize);
		virtual ~FStreamDataZip();

	public:
	protected:
		ZZIP_FILE* m_pZzipFile;

	public:
		virtual size_t Read(void* pBuffer, size_t nCount);
		virtual void Skip(long nCount);
		virtual void Seek(size_t nPos);
		virtual size_t Tell() const;
		virtual bool Eof() const;
		virtual void Close();
	};

}; //LostPeterFoundation

#endif