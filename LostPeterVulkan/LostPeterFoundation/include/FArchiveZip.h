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

#ifndef _F_ARCHIVE_ZIP_H_
#define _F_ARCHIVE_ZIP_H_

#include "FArchive.h"

typedef struct zzip_dir	ZZIP_DIR;
typedef struct zzip_file ZZIP_FILE;

namespace LostPeterFoundation
{
	class foundationExport FArchiveZip : public FArchive
    {
    public:
		FArchiveZip(const String& strName, const String& strType);
		virtual ~FArchiveZip();

    public:
	protected:
		ZZIP_DIR* m_pZzipDir;			
		FFileInfoVector	m_aFileInfo;
	
	public:
		virtual bool IsCaseSensitive() const { return false; }

		virtual void Load();
		virtual void Unload();

		virtual FStreamData* Open(const String& strFileName, bool bIsReadOnly = true) const;
		virtual FStreamData* Create(const String& strFileName) const;
		virtual void Remove(const String& strFileName) const;

		virtual StringVector* ListFile(bool bIsRecursive = true, bool bDirs = false);
		virtual FFileInfoVector* ListFileInfo(bool bIsRecursive = true, bool bDirs = false);
		virtual StringVector* Find(const String& strPattern, bool bIsRecursive = true, bool bDirs = false);
		virtual bool Exists(const String& strFileName);
		virtual time_t GetModifiedTime(const String& strFileName);
		virtual FFileInfoVector* FindFileInfo(const String& strPattern, bool bIsRecursive = true, bool bDirs = false);
    };

}; //LostPeterFoundation

#endif