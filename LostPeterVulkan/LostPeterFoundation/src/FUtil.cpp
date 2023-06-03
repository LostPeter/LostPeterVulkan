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

#include "../include/FUtil.h"
#include "../include/FUtilOC.h"

namespace LostPeterFoundation
{
#if LP_PLATFORM == LP_PLATFORM_WIN32
    char* Unicode2Utf8(wchar_t* unicodeStr) {
        int cStrLen = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, NULL, 0, NULL, NULL);
        char* cStr = (char*)malloc(sizeof(char) * (cStrLen + 1));
        WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, cStr, cStrLen + 1, NULL, NULL);
        *(cStr + cStrLen) = '\0';
        return cStr;
    }
#endif


////Path
    String FUtil::GetPathExecute()
    {
        String path;

    #if LP_PLATFORM == LP_PLATFORM_WIN32
        wchar_t szBuf[512];
        ::GetModuleFileNameW(NULL, szBuf, 512);
        ::PathRemoveFileSpecW(szBuf);
        char* utf8 = Unicode2Utf8(szBuf);

        path.append(utf8);
        free(utf8); 

        std::replace(path.begin(), path.end(), '\\', '/');

    #elif LP_PLATFORM == LP_PLATFORM_MAC
        path = F_OCGetPathExecute();
        
    #endif
        if (path[path.size() - 1] == '.')
        {
            path = path.substr(0, path.size() - 2);
        }
        size_t index = path.find_last_of('/');
        path = path.substr(0, index);
        path += "/";

        return path;
    }
    String FUtil::GetPathBin()
    {
        String pathBin = GetPathExecute();
        String::size_type pos = pathBin.find_last_of('/');
        if (pos != String::npos)
            pathBin = pathBin.substr(0, pos);
        return pathBin;
    }
    String FUtil::GetPathAssets()
    {
        const String& pathBin = GetPathBin();
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC
        String pathAssets = pathBin + "/Assets/";
    #else
        String pathAssets = pathBin + "/";
    #endif
        return pathAssets;
    }
    String FUtil::GetPathReal(const char* szFile)
    {
        return GetPathReal(String(szFile));
    }
    String FUtil::GetPathReal(const String& strPath)
    {
        return GetPathBin() + "/" + strPath;
    }


////File
    bool FUtil::FileIsExist(const String& strPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC
        String pathReal = GetPathReal(strPath);
        std::ifstream file(pathReal.c_str(), std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            return false;
        }
        file.close();
        return true;
    #else
        
    #endif
    return false;
    }
    bool FUtil::DeleteFile(const String& strPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else

    #endif
    return false;
    }
    bool FUtil::ClearFile(const String& strPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }
    bool FUtil::CopyFile(const String& strSrcPath, const String& strDstPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }


////Folder
    bool FUtil::IsDirectory(const String& strPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }
    bool FUtil::CreateDirectory(const String& strPath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }
    bool FUtil::EnumFiles(const String& strFolderPath, StringVector& aFiles, bool bFilePath)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }
    bool FUtil::EnumFiles(const String& strFolderPath, String2StringMap& mapFiles, bool bIsRecursive)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }
    bool FUtil::EnumFolders(const String& strFolderPath, StringVector& aFolders, bool bFolderPath, bool bIsRecursive)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32 || LP_PLATFORM == LP_PLATFORM_MAC

    #else
    
    #endif
    return false;
    }


////LoadFile
    bool FUtil::LoadFileContent(const char* szFile, CharVector& content, bool addEnd0 /*= false*/)
    {
        std::ifstream file(szFile, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            F_LogError("*********************** FUtil::LoadFileContent: Read file failed: [%s] !", szFile);
            return false;
        }

        size_t fileSize = (size_t)file.tellg();
        if (addEnd0)
            content.resize(fileSize + 1);
        else
            content.resize(fileSize);
        file.seekg(0);
        file.read(content.data(), fileSize);
        file.close();
        if (addEnd0)
            content[fileSize] = 0;
        return true;
    }
    bool FUtil::LoadFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 /*= false*/)
    {
        std::ifstream file(szFile, std::ios::ate | std::ios::binary);
        if (!file.is_open())
        {
            F_LogError("*********************** FUtil::LoadFileToBuffer: Read file failed: [%s] !", szFile);
            return false;
        }

        size_t fileSize = (size_t)file.tellg();
        if (addEnd0)
            fileSize += 1;
        uint8* pData = new uint8[fileSize];
        file.seekg(0);
        file.read((char*)pData, fileSize);
        file.close();
        if (addEnd0)
            pData[fileSize] = 0;

        *ppData = pData;
        sizeData = (int32)fileSize;
        return true;
    }
    bool FUtil::LoadFileToString(const char* szFile, String& contentFile)
    {
        std::ifstream file;
        file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            file.open(szFile);
            std::stringstream fileStream;
            fileStream << file.rdbuf();
            file.close();
            contentFile = fileStream.str();
        }
        catch(std::ifstream::failure e)
        {
            F_LogError("*********************** FUtil::LoadFileToString: Read file failed: [%s] !", szFile);
            return false;
        }
        return true;
    }

    bool FUtil::LoadAssetFileContent(const char* szFile, CharVector& content, bool addEnd0 /*= false*/)
    {
        String pathReal = GetPathReal(szFile);
        return LoadFileContent(pathReal.c_str(), content, addEnd0); 
    }
    bool FUtil::LoadAssetFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 /*= false*/)
    {
        String pathReal = GetPathReal(szFile);
        return LoadFileToBuffer(pathReal.c_str(), ppData, sizeData, addEnd0);
    }
    bool FUtil::LoadAssetFileToString(const char* szFile, String& contentFile)
    {
        String pathReal = GetPathReal(szFile);
        return LoadFileToString(pathReal.c_str(), contentFile); 
    }

}; //LostPeterFoundation