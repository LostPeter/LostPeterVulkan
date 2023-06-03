/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_UTIL_H_
#define _F_UTIL_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class LPF_Export FUtil
    {
    public:
    ////Path
        static String GetPathExecute();
        static String GetPathBin();
        static String GetPathAssets();
        static String GetPathReal(const char* szFile);
        static String GetPathReal(const String& strPath);

    ////File
        static bool FileIsExist(const String& strPath);
        static bool DeleteFile(const String& strPath);
        static bool ClearFile(const String& strPath);
        static bool CopyFile(const String& strSrcPath, const String& strDstPath);

    ////Folder
        static bool IsDirectory(const String& strPath);
		static bool CreateDirectory(const String& strPath);
		static bool EnumFiles(const String& strFolderPath, StringVector& aFiles, bool bFilePath);
		static bool EnumFiles(const String& strFolderPath, String2StringMap& mapFiles, bool bIsRecursive);
		static bool EnumFolders(const String& strFolderPath, StringVector& aFolders, bool bFolderPath, bool bIsRecursive);

    ////LoadFile
        static bool LoadFileContent(const char* szFile, CharVector& content, bool addEnd0 = false);
        static bool LoadFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadFileToString(const char* szFile, String& contentFile);

        static bool LoadAssetFileContent(const char* szFile, CharVector& content, bool addEnd0 = false);
        static bool LoadAssetFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadAssetFileToString(const char* szFile, String& contentFile);
    };

}; //LostPeterFoundation

#endif