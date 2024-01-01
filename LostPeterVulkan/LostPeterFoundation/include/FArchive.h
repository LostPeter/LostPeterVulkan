/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_ARCHIVE_H_
#define _F_ARCHIVE_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
	struct foundationExport FFileInfo		
	{
		FArchive* pArchive;
		String strFileName;		
		String strPath;			
		String strBaseName;		
		size_t nCompressedSize;		
		size_t nUncompressedSize;
	};
	typedef std::vector<FFileInfo> FFileInfoVector;


	class foundationExport FArchive 
	{
	public:
		FArchive(const String& strName, const String& strType);
		virtual ~FArchive();

	public:
	protected:
		String m_strName; 
		String m_strType;
		bool m_bIsReadOnly;

	public:
		F_FORCEINLINE const String& GetName(void) const { return m_strName; }
		F_FORCEINLINE const String& GetType(void) const { return m_strType; }

	public:
		virtual bool IsCaseSensitive(void) const = 0;
		virtual bool IsReadOnly() const	{ return m_bIsReadOnly; }

		virtual void Load()	= 0;
		virtual void Unload() = 0;

		virtual FFileBase* Open(const String& strFileName, bool bIsReadOnly = true) const = 0;
		virtual FFileBase* Create(const String& strFileName) const = 0;
		virtual void Remove(const String& strFileName) const = 0;

		virtual StringVector* ListFile(bool bIsRecursive = true, bool bDirs = false) = 0;
		virtual FFileInfoVector* ListFileInfo(bool bIsRecursive = true, bool bDirs = false) = 0;
		virtual StringVector* Find(const String& strPattern, bool bIsRecursive = true, bool bDirs = false) = 0;
		virtual bool Exists(const String& strFileName) = 0; 
		virtual time_t GetModifiedTime(const String& strFileName) = 0; 
		virtual FFileInfoVector* FindFileInfo(const String& strPattern, bool bIsRecursive = true, bool bDirs = false) = 0;
	};

}; //LostPeterFoundation

#endif