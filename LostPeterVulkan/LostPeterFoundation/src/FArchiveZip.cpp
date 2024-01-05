/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FArchiveZip.h"
#include "../include/FStreamDataZip.h"
#include "../include/FUtilString.h"

#include <zzip/zzip.h>

namespace LostPeterFoundation
{
	String s_GetZzipErrorDescription(zzip_error_t zzipError) 
	{
		String errorMsg;
		switch (zzipError)
		{
		case ZZIP_NO_ERROR:
			break;
		case ZZIP_OUTOFMEM:
			errorMsg = "Out of memory.";
			break;            
		case ZZIP_DIR_OPEN:
		case ZZIP_DIR_STAT: 
		case ZZIP_DIR_SEEK:
		case ZZIP_DIR_READ:
			errorMsg = "Unable to read zip file.";
			break;            
		case ZZIP_UNSUPP_COMPR:
			errorMsg = "Unsupported compression format.";
			break;            
		case ZZIP_CORRUPTED:
			errorMsg = "Corrupted archive.";
			break;            
		default:
			errorMsg = "Unknown error.";
			break;            
		};

		return errorMsg;
	}

    FArchiveZip::FArchiveZip(const String& strName, const String& strType)
        : FArchive(strName, strType)
        , m_pZzipDir(nullptr)
    {

    }

	FArchiveZip::~FArchiveZip()
    {
        Unload();
    }

    void FArchiveZip::Load()
	{
		if (!m_pZzipDir)
		{
			zzip_error_t zzipError;
			m_pZzipDir = zzip_dir_open(m_strName.c_str(), &zzipError);
            if (zzipError != ZZIP_NO_ERROR)
            {
                String errorMsg = s_GetZzipErrorDescription(static_cast<zzip_error_t>(zzipError));
                F_LogError("*********************** FArchiveZip::Load: Error to load file: [%s], error: [%s] !", m_strName.c_str(), errorMsg.c_str());
                return;
            }

			ZZIP_DIRENT zzipEntry;
			while (zzip_dir_read(m_pZzipDir, &zzipEntry))
			{
				FFileInfo info;
				info.pArchive = this;
				FUtilString::SplitFileName(zzipEntry.d_name, info.strBaseName, info.strPath);
				info.strFileName = zzipEntry.d_name;
				info.nCompressedSize = static_cast<size_t>(zzipEntry.d_csize);
				info.nUncompressedSize = static_cast<size_t>(zzipEntry.st_size);
				if (info.strBaseName.empty())
				{
					info.strFileName	= info.strFileName.substr (0, info.strFileName.length () - 1);
					FUtilString::SplitFileName(info.strFileName, info.strBaseName, info.strPath);
					info.nCompressedSize = size_t (-1);
				}
				m_aFileInfo.push_back(info);
			}
		}
	}

	void FArchiveZip::Unload()
	{
		if (m_pZzipDir)
		{
			zzip_dir_close(m_pZzipDir);
			m_pZzipDir = nullptr;
			m_aFileInfo.clear();
		}
	}
	
	FStreamData* FArchiveZip::Open(const String& strFileName, bool bIsReadOnly /*= true*/) const
	{
		ZZIP_FILE* zzipFile = zzip_file_open(m_pZzipDir, strFileName.c_str(), ZZIP_ONLYZIP | ZZIP_CASELESS);
		if (!zzipFile)
		{
			int zerr = zzip_error(m_pZzipDir);
			String zzDesc = s_GetZzipErrorDescription((zzip_error_t)zerr);

            F_LogError("*********************** FArchiveZip::Open: Unable to open file: [%s], error: [%s] !", strFileName.c_str(), zzDesc.c_str());
            return nullptr;
		}

		ZZIP_STAT zstat;
		zzip_dir_stat(m_pZzipDir, strFileName.c_str(), &zstat, ZZIP_CASEINSENSITIVE);
		return new FStreamDataZip(strFileName, zzipFile, static_cast<size_t>(zstat.st_size));
	}	

	FStreamData* FArchiveZip::Create(const String& strFileName) const
	{
        F_LogError("*********************** FArchiveZip::Create: Modification of zipped archives is not supported, file: [%s] !", strFileName.c_str());
        return nullptr;
	}

	void FArchiveZip::Remove(const String& strFileName) const
	{

	}

	StringVector* FArchiveZip::ListFile(bool bIsRecursive /*= true*/, bool bDirs /*= false*/)
	{
		StringVector* pFileNames = new StringVector;
		FFileInfoVector::iterator it, itEnd;
		itEnd = m_aFileInfo.end();
		for (it = m_aFileInfo.begin(); it != itEnd; ++it)
		{
			if ((bDirs == (it->nCompressedSize == size_t (-1))) &&
				(bIsRecursive || it->strPath.empty()))
				pFileNames->push_back(it->strFileName);
		}
		return pFileNames;
	}
	
	FFileInfoVector* FArchiveZip::ListFileInfo(bool bIsRecursive /*= true*/, bool bDirs /*= false*/)
	{
		FFileInfoVector* pFileInfos = new FFileInfoVector;
		FFileInfoVector::const_iterator it, itEnd;
		itEnd = m_aFileInfo.end();
		for (it = m_aFileInfo.begin(); it != itEnd; ++it)
		{
			if ((bDirs == (it->nCompressedSize == size_t (-1))) &&
				(bIsRecursive || it->strPath.empty()))
				pFileInfos->push_back(*it);
		}
		return pFileInfos;
	}
	
	StringVector* FArchiveZip::Find(const String& strPattern, bool bIsRecursive /*= true*/, bool bDirs /*= false*/)
	{
		StringVector* pFileNames = new StringVector;
		bool bIsFullMatch = (strPattern.find ('/') != String::npos) || (strPattern.find ('\\') != String::npos);
		FFileInfoVector::iterator it, itEnd;
		itEnd = m_aFileInfo.end();
		for (it = m_aFileInfo.begin(); it != itEnd; ++it)
		{
			if ((bDirs == (it->nCompressedSize == size_t (-1))) &&
				(bIsRecursive || bIsFullMatch || it->strPath.empty()))
			{
				if (FUtilString::Match(bIsFullMatch ? it->strFileName : it->strBaseName, strPattern, false))
					pFileNames->push_back(it->strFileName);
			}
		}
		return pFileNames;
	}
	
	FFileInfoVector* FArchiveZip::FindFileInfo(const String& strPattern, bool bIsRecursive /*= true*/, bool bDirs /*= false*/)
	{
		FFileInfoVector* pFileInfos = new FFileInfoVector;
		bool bIsFullMatch = (strPattern.find ('/') != String::npos) || (strPattern.find ('\\') != String::npos);
		FFileInfoVector::iterator it, itEnd;
		itEnd = m_aFileInfo.end();
		for (it = m_aFileInfo.begin(); it != itEnd; ++it)
		{
			if ((bDirs == (it->nCompressedSize == size_t (-1))) &&
				(bIsRecursive || bIsFullMatch || it->strPath.empty()))
			{
				if (FUtilString::Match(bIsFullMatch ? it->strFileName : it->strBaseName, strPattern, false))
					pFileInfos->push_back(*it);
			}
		}
		return pFileInfos;
	}
	
	bool FArchiveZip::Exists(const String& strFileName)
	{
		ZZIP_STAT zstat;
		int res = zzip_dir_stat(m_pZzipDir, strFileName.c_str(), &zstat, ZZIP_CASEINSENSITIVE);
		return (res == ZZIP_NO_ERROR);
	}
	
	time_t FArchiveZip::GetModifiedTime(const String& strFileName)
	{
		struct stat tagStat;
		bool ret = (stat(m_strName.c_str(), &tagStat) == 0);
		if (ret)
		{
			return tagStat.st_mtime;
		}
		return 0;
	}
    
}; //LostPeterFoundation