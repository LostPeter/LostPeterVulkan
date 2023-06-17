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


    //////////////////////// Common ////////////////////////
    //Path
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


    //////////////////////// Thread ////////////////////////
    //int8/16/32/64 [+-] 1
    int8 FUtil::InterlockedIncrement(volatile int8* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)_InterlockedExchangeAdd8((char*)pValue, 1) + 1;
    #else
        return __sync_fetch_and_add(pValue, 1) + 1;
    #endif
    }
    int8 FUtil::InterlockedDecrement(volatile int8* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)::_InterlockedExchangeAdd8((char*)pValue, -1) - 1;
    #else
        return __sync_fetch_and_sub(pValue, 1) - 1;
    #endif
    }
    int16 FUtil::InterlockedIncrement(volatile int16* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedIncrement16((short*)pValue);
    #else
        return __sync_fetch_and_add(pValue, 1) + 1;
    #endif
    }
    int16 FUtil::InterlockedDecrement(volatile int16* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedDecrement16((short*)pValue);
    #else
        return __sync_fetch_and_sub(pValue, 1) - 1;
    #endif
    }
    int32 FUtil::InterlockedIncrement(volatile int32* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedIncrement((long*)pValue);
    #else
        return __sync_fetch_and_add(pValue, 1) + 1;
    #endif
    }
    int32 FUtil::InterlockedDecrement(volatile int32* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedDecrement((long*)pValue);
    #else
        return __sync_fetch_and_sub(pValue, 1) - 1;
    #endif
    }
    int64 FUtil::InterlockedIncrement(volatile int64* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (int64)::_InterlockedIncrement64((long long*)pValue);
        #else
            while (true)
            {
                int64 oldValue = *pValue;
                if (_InterlockedCompareExchange64(pValue, oldValue + 1, oldValue) == oldValue)
                {
                    return oldValue + 1;
                }
            }
        #endif
    #else
        return __sync_fetch_and_add(pValue, 1) + 1;
    #endif
    }
    int64 FUtil::InterlockedDecrement(volatile int64* pValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (int64)::_InterlockedDecrement64((long long*)pValue);
        #else
            while (true)
            {
                int64 oldValue = *pValue;
                if (_InterlockedCompareExchange64(pValue, oldValue - 1, oldValue) == oldValue)
                {
                    return oldValue - 1;
                }
            }
        #endif
    #else
        return __sync_fetch_and_sub(pValue, 1) - 1;
    #endif
    }

    //int8/16/32/64 [+-] nAmount
    int8 FUtil::InterlockedAdd(volatile int8* pValue, int8 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)::_InterlockedExchangeAdd8((char*)pValue, (char)nAmount) + nAmount;
    #else
        return __sync_fetch_and_add(pValue, nAmount) + nAmount;
    #endif
    }
    int8 FUtil::InterlockedSub(volatile int8* pValue, int8 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)::_InterlockedExchangeAdd8((char*)pValue, (char)-nAmount) - nAmount;
    #else
        return __sync_fetch_and_sub(pValue, nAmount) - nAmount;
    #endif
    }
    int16 FUtil::InterlockedAdd(volatile int16* pValue, int16 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedExchangeAdd16((short*)pValue, (short)nAmount) + nAmount;
    #else
        return __sync_fetch_and_add(pValue, nAmount) + nAmount;
    #endif
    }
    int16 FUtil::InterlockedSub(volatile int16* pValue, int16 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedExchangeAdd16((short*)pValue, (short)-nAmount) - nAmount;
    #else
        return __sync_fetch_and_sub(pValue, nAmount) - nAmount;
    #endif
    }
    int32 FUtil::InterlockedAdd(volatile int32* pValue, int32 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedExchangeAdd((long*)pValue, (long)nAmount) + nAmount;
    #else
        return __sync_fetch_and_add(pValue, nAmount) + nAmount;
    #endif
    }
    int32 FUtil::InterlockedSub(volatile int32* pValue, int32 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedExchangeAdd((long*)pValue, (long)-nAmount) - nAmount;
    #else
        return __sync_fetch_and_sub(pValue, nAmount) - nAmount;
    #endif
    }
    int64 FUtil::InterlockedAdd(volatile int64* pValue, int64 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (int64)::_InterlockedExchangeAdd64((int64*)pValue, (int64)nAmount) + nAmount;
        #else
            while (true)
            {
                int64 oldValue = *pValue;
                if (_InterlockedCompareExchange64(pValue, oldValue + amount, oldValue) == oldValue)
                {
                    return oldValue + amount;
                }
            }
        #endif
    #else
        return __sync_fetch_and_add(pValue, nAmount) + nAmount;
    #endif
    }
    int64 FUtil::InterlockedSub(volatile int64* pValue, int64 nAmount)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (int64)::_InterlockedExchangeAdd64((int64*)pValue, (int64)-nAmount) - nAmount;
        #else
            while (true)
            {
                int64 oldValue = *pValue;
                if (_InterlockedCompareExchange64(pValue, oldValue - amount, oldValue) == oldValue)
                {
                    return oldValue - amount;
                }
            }
        #endif
    #else
        return __sync_fetch_and_sub(pValue, nAmount) - nAmount;
    #endif
    }

    //int8/16/32/64 exchange nExchange
    int8 FUtil::InterlockedExchange(volatile int8* pValue, int8 nExchange)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)::_InterlockedExchange8((char*)pValue, (char)nExchange);
    #else
        return __sync_lock_test_and_set(pValue, nExchange);
    #endif
    }
    int16 FUtil::InterlockedExchange(volatile int16* pValue, int16 nExchange)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedExchange16((short*)pValue, (short)nExchange);
    #else
        return __sync_lock_test_and_set(pValue, nExchange);
    #endif
    }
    int32 FUtil::InterlockedExchange(volatile int32* pValue, int32 nExchange)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedExchange((long*)pValue, (long)nExchange);
    #else
        return __sync_lock_test_and_set(pValue, nExchange);
    #endif
    }
    int64 FUtil::InterlockedExchange(volatile int64* pValue, int64 nExchange)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (int64)::_InterlockedExchange64((long long*)pValue, (long long)nExchange);
        #else
                while (true)
                {
                    int64 oldValue = *pValue;
                    if (_InterlockedCompareExchange64(pValue, exchange, oldValue) == oldValue)
                    {
                        return oldValue;
                    }
                }
        #endif
    #else
        return __sync_lock_test_and_set(pValue, nExchange);
    #endif
    }

    //int8/16/32/64 compare nComperand and exchange nExchange
    int8 FUtil::InterlockedCompareExchange(volatile int8* pDest, int8 nExchange, int8 nComperand)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int8)::_InterlockedCompareExchange8((char*)pDest, (char)nExchange, (char)nComperand);
    #else
        return __sync_val_compare_and_swap(pDest, nComperand, nExchange);
    #endif
    }
    int16 FUtil::InterlockedCompareExchange(volatile int16* pDest, int16 nExchange, int16 nComperand)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int16)::_InterlockedCompareExchange16((short*)pDest, (short)nExchange, (short)nComperand);
    #else
        return __sync_val_compare_and_swap(pDest, nComperand, nExchange);
    #endif
    }
    int32 FUtil::InterlockedCompareExchange(volatile int32* pDest, int32 nExchange, int32 nComperand)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int32)::_InterlockedCompareExchange((long*)pDest, (long)nExchange, (long)nComperand);
    #else
        return __sync_val_compare_and_swap(pDest, nComperand, nExchange);
    #endif
    }
    int64 FUtil::InterlockedCompareExchange(volatile int64* pDest, int64 nExchange, int64 nComperand)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (int64)::_InterlockedCompareExchange64(pDest, nExchange, nComperand);
    #else
        return __sync_val_compare_and_swap(pDest, nComperand, nExchange);
    #endif
    }

    //exchange Ptr
    void* FUtil::InterlockedExchangePtr(void** ppDest, void* pExchange)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return (void*)::_InterlockedExchange64((int64*)(ppDest), (int64)(pExchange));
        #else
            return (void*)::_InterlockedExchange((long*)(ppDest), (long)(pExchange));
        #endif
    #else
        return __sync_lock_test_and_set(ppDest, pExchange);
    #endif
    }
    void* FUtil::InterlockedCompareExchangePtr(void** ppDest, void* pExchange, void* pComperand)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return (void*)::_InterlockedCompareExchange64((int64*)ppDest, (int64)pExchange, (int64)pComperand);
    #else
        return __sync_val_compare_and_swap(ppDest, pComperand, pExchange);
    #endif
    }

    //int8/16/32/64 read pSrc
    int8 FUtil::AtomicRead(volatile const int8* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return InterlockedCompareExchange((int8*)pSrc, 0, 0);
    #else
        int8 nResult;
        __atomic_load((volatile int8*)pSrc, &nResult, __ATOMIC_SEQ_CST);
        return nResult;
    #endif
    }
    int16 FUtil::AtomicRead(volatile const int16* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return InterlockedCompareExchange((int16*)pSrc, 0, 0);
    #else
        int16 nResult;
        __atomic_load((volatile int16*)pSrc, &nResult, __ATOMIC_SEQ_CST);
        return nResult;
    #endif
    }
    int32 FUtil::AtomicRead(volatile const int32* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return InterlockedCompareExchange((int32*)pSrc, 0, 0);
    #else
        int32 nResult;
        __atomic_load((volatile int32*)pSrc, &nResult, __ATOMIC_SEQ_CST);
        return nResult;
    #endif
    }
    int64 FUtil::AtomicRead(volatile const int64* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return InterlockedCompareExchange((int64*)pSrc, 0, 0);
    #else
        int64 nResult;
        __atomic_load((volatile int64*)pSrc, &nResult, __ATOMIC_SEQ_CST);
        return nResult;
    #endif        
    }

    //int8/16/32/64 read relaxed pSrc
    int8 FUtil::AtomicRead_Relaxed(volatile const int8* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return *pSrc;
    #else
        int8 nResult;
        __atomic_load((volatile int8*)pSrc, &nResult, __ATOMIC_RELAXED);
        return nResult;
    #endif
    }
    int16 FUtil::AtomicRead_Relaxed(volatile const int16* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return *pSrc;
    #else
        int16 nResult;
        __atomic_load((volatile int16*)pSrc, &nResult, __ATOMIC_RELAXED);
        return nResult;
    #endif
    }
    int32 FUtil::AtomicRead_Relaxed(volatile const int32* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        return *pSrc;
    #else
        int32 nResult;
        __atomic_load((volatile int32*)pSrc, &nResult, __ATOMIC_RELAXED);
        return nResult;
    #endif
    }
    int64 FUtil::AtomicRead_Relaxed(volatile const int64* pSrc)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            return *pSrc;
        #else
            return InterlockedCompareExchange((volatile int64*)pSrc, 0, 0);
        #endif
    #else
        int64 nResult;
        __atomic_load((volatile int64*)pSrc, &nResult, __ATOMIC_RELAXED);
        return nResult;
    #endif
    }

    //int8/16/32/64 store nValue to pSrc
    void FUtil::AtomicStore(volatile int8* pSrc, int8 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        InterlockedExchange(pSrc, nValue);
    #else
        __atomic_store((volatile int8*)pSrc, &nValue, __ATOMIC_SEQ_CST);
    #endif
    }
    void FUtil::AtomicStore(volatile int16* pSrc, int16 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        InterlockedExchange(pSrc, nValue);
    #else
        __atomic_store((volatile int16*)pSrc, &nValue, __ATOMIC_SEQ_CST);
    #endif
    }
    void FUtil::AtomicStore(volatile int32* pSrc, int32 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        InterlockedExchange(pSrc, nValue);
    #else
        __atomic_store((volatile int32*)pSrc, &nValue, __ATOMIC_SEQ_CST);
    #endif
    }
    void FUtil::AtomicStore(volatile int64* pSrc, int64 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        InterlockedExchange(pSrc, nValue);
    #else
        __atomic_store((volatile int64*)pSrc, &nValue, __ATOMIC_SEQ_CST);
    #endif
    }

    //int8/16/32/64 store relaxed nValue to pSrc
    void FUtil::AtomicStore_Relaxed(volatile int8* pSrc, int8 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        *pSrc = nValue;
    #else
        __atomic_store((volatile int8*)pSrc, &nValue, __ATOMIC_RELAXED);
    #endif
    }
    void FUtil::AtomicStore_Relaxed(volatile int16* pSrc, int16 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        *pSrc = nValue;
    #else
        __atomic_store((volatile int16*)pSrc, &nValue, __ATOMIC_RELAXED);
    #endif
    }
    void FUtil::AtomicStore_Relaxed(volatile int32* pSrc, int32 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        *pSrc = nValue;
    #else
        __atomic_store((volatile int32*)pSrc, &nValue, __ATOMIC_RELAXED);
    #endif
    }
    void FUtil::AtomicStore_Relaxed(volatile int64* pSrc, int64 nValue)
    {
    #if LP_PLATFORM == LP_PLATFORM_WIN32
        #if LP_ARCHITECTURE == LP_ARCHITECTURE_64
            *pSrc = nValue;
        #else
            InterlockedExchange(pSrc, nValue);
        #endif
    #else
        __atomic_store((volatile int64*)pSrc, &nValue, __ATOMIC_RELAXED);
    #endif
    }


}; //LostPeterFoundation