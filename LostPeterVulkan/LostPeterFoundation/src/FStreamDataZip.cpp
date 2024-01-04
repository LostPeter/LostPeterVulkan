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

#include "../include/FStreamDataZip.h"
#include <zzip/zzip.h>

namespace LostPeterFoundation
{
    FStreamDataZip::FStreamDataZip(ZZIP_FILE* pZzipFile, size_t nUncompressedSize)
		: m_pZzipFile(pZzipFile)
	{
		m_nSize = nUncompressedSize;
	}

	FStreamDataZip::FStreamDataZip(const String& strName, ZZIP_FILE* pZzipFile, size_t nUncompressedSize)
		: FStreamData(strName)
		, m_pZzipFile(pZzipFile)
	{
		m_nSize = nUncompressedSize;
	}

	FStreamDataZip::~FStreamDataZip()
	{
		Close();
	}

	size_t FStreamDataZip::Read(void* pBuffer, size_t nCount)
	{	
		zzip_ssize_t r = zzip_file_read(m_pZzipFile, (char*)pBuffer, nCount);
		if (r < 0) 
		{
			ZZIP_DIR* dir = zzip_dirhandle(m_pZzipFile);
            F_LogError("*********************** FStreamDataZip::Read: File: [%s], Error from zziplib: [%s] !", m_strName.c_str(), zzip_strerror_of(dir));
            return 0;
		}
		return (size_t)r;
	}
	
	void FStreamDataZip::Skip(long nCount)
	{
		zzip_seek(m_pZzipFile, static_cast<zzip_off_t>(nCount), SEEK_CUR);
	}
	
	void FStreamDataZip::Seek(size_t nPos)
	{
		zzip_seek(m_pZzipFile, static_cast<zzip_off_t>(nPos), SEEK_SET);
	}
	
	size_t FStreamDataZip::Tell() const
	{
		return zzip_tell(m_pZzipFile);
	}
	
	bool FStreamDataZip::Eof() const
	{
		return (zzip_tell(m_pZzipFile) >= static_cast<zzip_off_t>(m_nSize));
	}
	
	void FStreamDataZip::Close()
	{	
		zzip_file_close(m_pZzipFile);
	}

}; //LostPeterFoundation