// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_UTIL_H_
#define _VULKAN_UTIL_H_

#include "predefine.h"

namespace LostPeter
{
    class utilExport VulkanUtil
    {
    public:
    ////Path
        static std::string GetPathExecute();
        static std::string GetPathBin();
        static std::string GetPathAssets();
        static std::string GetPathReal(const char* szFile);
        static std::string GetPathReal(const std::string& strPath);

    ////File
        static bool FileIsExist(const std::string& strPath);
        static bool DeleteFile(const std::string& strPath);
        static bool ClearFile(const std::string& strPath);
        static bool CopyFile(const std::string& strSrcPath, const std::string& strDstPath);

    ////Folder
        static bool IsDirectory(const std::string& strPath);
		static bool CreateDirectory(const std::string& strPath);
		static bool EnumFiles(const std::string& strFolderPath, std::vector<std::string>& aFiles, bool bFilePath);
		static bool EnumFiles(const std::string& strFolderPath, std::map<std::string, std::string>& mapFiles, bool bIsRecursive);
		static bool EnumFolders(const std::string& strFolderPath, std::vector<std::string>& aFolders, bool bFolderPath, bool bIsRecursive);

    ////LoadFile
        static bool LoadFileContent(const char* szFile, std::vector<char>& content, bool addEnd0 = false);
        static bool LoadFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadFileToString(const char* szFile, std::string& contentFile);

        static bool LoadAssetFileContent(const char* szFile, std::vector<char>& content, bool addEnd0 = false);
        static bool LoadAssetFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadAssetFileToString(const char* szFile, std::string& contentFile);
    };

}; //LostPeter

#endif