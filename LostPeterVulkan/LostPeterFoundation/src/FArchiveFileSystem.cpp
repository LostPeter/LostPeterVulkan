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

#include "../include/FArchiveFileSystem.h"
#include "../include/FStreamDataFileStream.h"
#include "../include/FUtilString.h"

#if F_PLATFORM != F_PLATFORM_WINDOW
	#include "../include/FUtilFile.h"
#endif

namespace LostPeterFoundation
{
    static bool is_reserved_dir(const char* fn)
	{
		return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
	}
	static bool is_absolute_path(const char* path)
	{
#if F_PLATFORM == F_PLATFORM_WINDOW
		if (isalpha(unsigned char(path[0])) && path[1] == ':')
			return true;
#endif
		return path[0] == '/' || path[0] == '\\';
	}
    static String concatenate_path(const String& strBase, const String& strName)
	{
		if (strBase.empty() || is_absolute_path(strName.c_str()))
			return strName;
		else
			return strBase + '/' + strName;
	}


    bool FArchiveFileSystem::ms_bIsIgnoreHidden = true;
	FArchiveFileSystem::FArchiveFileSystem(const String& strName, const String& archType)
		: FArchive(strName, archType)
	{

	}

	FArchiveFileSystem::~FArchiveFileSystem()
	{
		Unload();
	}

	bool FArchiveFileSystem::IsCaseSensitive() const
	{
#if F_PLATFORM == F_PLATFORM_WINDOW
		return false;
#else
		return true;
#endif
	}

	void FArchiveFileSystem::Load()
	{
		String testPath = concatenate_path(m_strName, "__testwrite.test");
		std::ofstream writeStream;
		writeStream.open(testPath.c_str());
		if (writeStream.fail())
			m_bIsReadOnly = true;
		else
		{
			m_bIsReadOnly = false;
			writeStream.close();
			::remove(testPath.c_str());
		}
	}

	void FArchiveFileSystem::Unload()
	{
        
	}

	FStreamData* FArchiveFileSystem::Open(const String& strFileName, bool bIsReadOnly /*= true*/) const
	{
		String strFullPath = concatenate_path(m_strName, strFileName);
		struct stat tagStat;
		int ret = stat(strFullPath.c_str(), &tagStat);
		F_Assert("FArchiveFileSystem::Open: Problem getting file size !" && ret == 0)
		(void)ret; 
        
		std::ios::openmode mode = std::ios::in | std::ios::binary;
		std::istream* pBaseStream = nullptr;
		std::ifstream* pRoStream = nullptr;
		std::fstream* pRwStream = nullptr;
		if (!bIsReadOnly && IsReadOnly())
		{
			mode |= std::ios::out;
			pRwStream = new std::fstream();
			pRwStream->open(strFullPath.c_str(), mode);
			pBaseStream = pRwStream;
		}
		else
		{
			pRoStream = new std::ifstream;
			pRoStream->open(strFullPath.c_str(), mode);
			pBaseStream = pRoStream;
		}

		if (pBaseStream->fail())
		{
			delete pRoStream;
			delete pRwStream;

            F_LogError("*********************** FArchiveFileSystem::Open: Can not open file: [%s] !", strFileName.c_str());
            return nullptr;
		}

		FStreamDataFileStream* pStream = nullptr;
		if (pRwStream)
		{
			pStream = new FStreamDataFileStream(strFileName, pRwStream, (size_t)tagStat.st_size, true);
		}
		else
		{
			pStream = new FStreamDataFileStream(strFileName, pRoStream, (size_t)tagStat.st_size,true);
		}
		return pStream;
	}

	FStreamData* FArchiveFileSystem::Create(const String& strFileName) const
	{
		if (IsReadOnly())
		{
            F_LogError("*********************** FArchiveFileSystem::Create: Can not create a file in a read-only archive, file: [%s] !", strFileName.c_str());
            return nullptr;
		}

		String strFullPath = concatenate_path(m_strName, strFileName);
		std::ios::openmode mode = std::ios::out | std::ios::binary;
		std::fstream* pRwStream = new std::fstream;
		pRwStream->open(strFullPath.c_str(), mode);
		if (pRwStream->fail())
		{
			delete pRwStream;

            F_LogError("*********************** FArchiveFileSystem::Create: Can not open file: [%s] !", strFileName.c_str());
            return nullptr;
		}

		FStreamDataFileStream* pStream = new FStreamDataFileStream(strFileName, pRwStream, 0, true);
		return pStream;
	}
	
	void FArchiveFileSystem::Remove(const String& strFileName) const
	{	
		if (IsReadOnly())
		{
            F_LogError("*********************** FArchiveFileSystem::Remove: Can not remove a file from a read-only archive, file: [%s] !", strFileName.c_str());
            return;
		}

		String strFullPath = concatenate_path(m_strName, strFileName);
		::remove(strFullPath.c_str());
	}

	StringVector* FArchiveFileSystem::ListFile(bool bIsRecursive, bool bDirs)
	{
		StringVector* pFileNames = new StringVector;
		FindFiles("*", bIsRecursive, bDirs, pFileNames, nullptr);
		return pFileNames;
	}

	FFileInfoVector* FArchiveFileSystem::ListFileInfo(bool bIsRecursive, bool bDirs)
	{
		FFileInfoVector* pFileInfos = new FFileInfoVector;
		FindFiles("*", bIsRecursive, bDirs, nullptr, pFileInfos);
		return pFileInfos;
	}

	StringVector* FArchiveFileSystem::Find(const String& strPattern, bool bIsRecursive, bool bDirs)
	{
		StringVector* pFileNames = new StringVector;
		FindFiles(strPattern, bIsRecursive, bDirs, pFileNames, nullptr);
		return pFileNames;
	}

	FFileInfoVector* FArchiveFileSystem::FindFileInfo(const String& strPattern, bool bIsRecursive, bool bDirs)
	{
		FFileInfoVector* pFileInfos = new FFileInfoVector;
		FindFiles(strPattern, bIsRecursive, bDirs, nullptr, pFileInfos);
		return pFileInfos;
	}

	bool FArchiveFileSystem::Exists(const String& strFileName)
	{
		String strFullPath = concatenate_path(m_strName, strFileName);

		struct stat tagStat;
		bool ret = (stat(strFullPath.c_str(), &tagStat) == 0);
		if (ret && is_absolute_path(strFileName.c_str()))
		{
#if F_PLATFORM == F_PLATFORM_WINDOW
			String lowerCaseName = m_strName;
			FUtilString::ToLowerCase(lowerCaseName);
			ret = FUtilString::StartsWith(strFullPath, lowerCaseName, true);
#else
			ret = FUtilString::StartsWith(strFullPath, m_strName, false);
#endif
		}

		return ret;
	}

	time_t FArchiveFileSystem::GetModifiedTime(const String& strFileName)
	{
		String strFullPath = concatenate_path(m_strName, strFileName);
		struct stat tagStat;
		bool ret = (stat(strFullPath.c_str(), &tagStat) == 0);
		if (ret)
		{
			return tagStat.st_mtime;
		}
		return 0;
	}

    void FArchiveFileSystem::FindFiles(const String& strPattern, 
                                       bool bIsRecursive, 
		                               bool bDirs, 
                                       StringVector* pFileNames, 
                                       FFileInfoVector* pFileInfos)
	{
		long lHandle, res;
		struct _finddata_t tagData;

		size_t pos1 = strPattern.rfind ('/');
		size_t pos2 = strPattern.rfind ('\\');
		if (pos1 == strPattern.npos || ((pos2 != strPattern.npos) && (pos1 < pos2)))
			pos1 = pos2;
		String directory;
		if (pos1 != strPattern.npos)
			directory = strPattern.substr (0, pos1 + 1);

		String full_pattern = concatenate_path(m_strName, strPattern);

		lHandle = (long)_findfirst(full_pattern.c_str(), &tagData);
		res = 0;
		while (lHandle != -1 && res != -1)
		{
			if ((bDirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
				( !ms_bIsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
				(!bDirs || !is_reserved_dir (tagData.name)))
			{
				if (pFileNames)
				{
					pFileNames->push_back(directory + tagData.name);
				}
				else if (pFileInfos)
				{
					FFileInfo fi;
					fi.pArchive	= this;
					fi.strFileName = directory + tagData.name;
					fi.strBaseName = tagData.name;
					fi.strPath = directory;
					fi.nCompressedSize = tagData.size;
					fi.nUncompressedSize = tagData.size;
					pFileInfos->push_back(fi);
				}
			}
			res = _findnext(lHandle, &tagData);
		}

		if(lHandle != -1)
			_findclose(lHandle);

		if (bIsRecursive)
		{
			String base_dir = m_strName;
			if (!directory.empty ())
			{
				base_dir = concatenate_path(m_strName, directory);
				base_dir.erase (base_dir.length () - 1);
			}
			base_dir.append ("/*");

			String mask ("/");
			if (pos1 != strPattern.npos)
				mask.append (strPattern.substr (pos1 + 1));
			else
				mask.append (strPattern);

			lHandle = (long)_findfirst(base_dir.c_str (), &tagData);
			res = 0;
			while (lHandle != -1 && res != -1)
			{
				if ((tagData.attrib & _A_SUBDIR) &&
					( !ms_bIsIgnoreHidden || (tagData.attrib & _A_HIDDEN) == 0 ) &&
					!is_reserved_dir (tagData.name))
				{
					base_dir = directory;
					base_dir.append (tagData.name).append (mask);
					FindFiles(base_dir, bIsRecursive, bDirs, pFileNames, pFileInfos);
				}
				res = _findnext( lHandle, &tagData );
			}

			if(lHandle != -1)
				_findclose(lHandle);
		}
	}

}; //LostPeterFoundation