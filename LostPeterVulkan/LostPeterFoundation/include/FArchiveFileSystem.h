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

#ifndef _F_ARCHIVE_FILE_SYSTEM_H_
#define _F_ARCHIVE_FILE_SYSTEM_H_

#include "FArchive.h"

namespace LostPeterFoundation
{
    class foundationExport FArchiveFileSystem : public FArchive
    {
    public:
		FArchiveFileSystem(const String& strName, const String& strType);
		virtual ~FArchiveFileSystem();

    public:
		static bool	ms_bIsIgnoreHidden;

	public:
		static bool	GetIsIgnoreHidden() { return ms_bIsIgnoreHidden; }
		static void	SetIsIgnoreHidden(bool bIsIgnoreHidden) { ms_bIsIgnoreHidden = bIsIgnoreHidden; }

	public:
		virtual bool IsCaseSensitive() const;

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

	protected:
		void FindFiles(const String& strPattern, 
					   bool bIsRecursive, 
					   bool bDirs,
					   StringVector* pFileNames, 
					   FFileInfoVector* pFileInfos);
    };

}; //LostPeterFoundation

#endif