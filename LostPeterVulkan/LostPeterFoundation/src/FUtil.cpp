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
#include "../include/FMath.h"
#include "../include/FCamera.h"
#include "../include/FRay.h"

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


////Screen - World Transform
    bool FUtil::TransformScreenPos2ToWorldRay(const FVector2& vPosSceen2, FCamera* pCamera, const FVector4& vViewport, FRay* pOutRay)
    {
        F_Assert("FUtil::TransformScreenPos2ToWorldRay" && pCamera != nullptr)
        return pCamera->ConvertScreenPos2ToWorldRay(vPosSceen2.x, vPosSceen2.y, vViewport, pOutRay);
    }
    bool FUtil::TransformScreenPos2ToWorldRay(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FRay* pOutRay)
    {
        F_Assert("FUtil::TransformScreenPos2ToWorldRay" && pCamera != nullptr)
        return pCamera->ConvertScreenPos2ToWorldRay(screenX, screenY, vViewport, pOutRay);
    }

    bool FUtil::TransformScreenPos3ToWorldPos3(const FVector3& vPosScreen3, FCamera* pCamera, const FVector4& vViewport, FVector3& vPosWorld)
    {
        F_Assert("FUtil::TransformScreenPos3ToWorldPos3" && pCamera != nullptr)
        FRay ray; 
        pCamera->ConvertScreenPos2ToWorldRay(vPosScreen3.x, vPosScreen3.y, vViewport, &ray);
        vPosWorld = ray.GetPoint(vPosScreen3.z);

        return true;
    }
    bool FUtil::TransformScreenPos2ToWorldPos3StartEnd(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FVector3& vStart, FVector3& vEnd)
    {
        FVector3 vScreenStart = FVector3(screenX, screenY, 0.0f);
        if (!TransformScreenPos3ToWorldPos3(vScreenStart, pCamera, vViewport, vStart))
        {
            vStart = FMath::ms_v3Zero;
            vEnd = FMath::ms_v3Zero;
            return false;
        }

        FVector3 vScreenEnd = FVector3(screenX, screenY, 1.0f);
        if (!TransformScreenPos3ToWorldPos3(vScreenEnd, pCamera, vViewport, vEnd))
        {
            vStart = FMath::ms_v3Zero;
            vEnd = FMath::ms_v3Zero;
            return false;
        }

        return true;
    }   

    bool FUtil::TransformWorldPos3ToScreenPos2(const FVector3& vPosWorld, FCamera* pCamera, const FVector4& vViewport, FVector2& vPosScreen)
    {
        FVector3 vPosScreen3;
        if (!TransformWorldPos3ToScreenPos3(vPosWorld, pCamera, vViewport, vPosScreen3))
        {
            vPosScreen.x = 0.0f;
            vPosScreen.y = 0.0f;
            return false;
        }
        vPosScreen.x = vPosScreen3.x;
        vPosScreen.y = vPosScreen3.y;
        return true;
    }
    bool FUtil::TransformWorldPos3ToScreenPos3(const FVector3& vPosWorld, FCamera* pCamera, const FVector4& vViewport, FVector3& vPosScreen)
    {
        F_Assert("FUtil::TransformWorldPos3ToScreenPos3" && pCamera != nullptr)

        const FMatrix4& mat4View = pCamera->GetMatrix4View();
        const FMatrix4& mat4Proj = pCamera->GetMatrix4Projection();

        FVector4 in	= FVector4(vPosWorld.x, vPosWorld.y, vPosWorld.z, 1.0f);
        FVector4 out = mat4View * in;
        out = mat4Proj * out;

        if (out.w <= 0.0)
            return false;    

        out.x /= out.w;
        out.y /= out.w;
        out.z /= out.w;

        //Map x, y and z to range 0-1
        out.x = out.x * 0.5f + 0.5f;
        out.y = out.y * 0.5f + 0.5f;
        out.z = out.z * 0.5f + 0.5f;

        //Map x,y to viewport
        out.x = vViewport.x + out.x * vViewport.z;
        out.y = (1.0f - out.y - vViewport.y) * vViewport.w;

        vPosScreen.x = out.x;
        vPosScreen.y = out.y;
        vPosScreen.z = out.z;
        return true;
    }

    bool FUtil::IsBetween(float fl, float fr, float ft)
    {   
        float fBigger  = fl >= fr? fl : fr;
        float fSmaller = fl < fr? fl : fr;
        return (ft <= fBigger && ft >= fSmaller);
    }

    bool FUtil::IntersectLine(float x, float y, const FVector3& vStart, const FVector3& vEnd, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter)
    {
        int nRadius = 5;
        FVector3 vScreenStart, vScreenEnd;
        TransformWorldPos3ToScreenPos3(vStart, pCamera, vViewport, vScreenStart);
        TransformWorldPos3ToScreenPos3(vEnd, pCamera, vViewport, vScreenEnd);

        float x1,x2,y1,y2;
        x1 = vScreenStart.x;
        y1 = vScreenStart.y;
        x2 = vScreenEnd.x;
        y2 = vScreenEnd.y;

        //if not acute triangle
        if (!IsBetween(x1, x2, x) && !IsBetween(y1, y2, y))
        {
            return false;
        }

        //the line is a dot on screen
        if (fabsf(x1 - x2) < 1e-6 && fabsf(y1 - y2) < 1e-6)
        {
            return false;
        }

        //the line is vertical on screen
        float fPitch, fb, fb2, fDist;
        if (fabsf(x1 - x2) < 1e-6)
        {
            if(fabsf(x - x1) < nRadius)
            {
                goto _succeed;
            }

            return false;
        }

        //calculate the distance
        fPitch = (y2 - y1) / (x2 - x1);
        fb = y2 - x2 * fPitch;
        fb2 = y1 - x1 * fPitch;
        fDist = fabsf(fPitch * x - y + fb) * fabsf(fPitch * x - y + fb) / (fPitch * fPitch + 1);
        if(fDist < nRadius * nRadius)
        {
            goto _succeed;
        }
        return false;

    _succeed:	
        FVector3 vCursorPos(x, y, 0);
        FVector3 vScreenStart0(vScreenStart);
        FVector3 vScreenEnd0(vScreenEnd);
        vScreenStart0.z = vScreenEnd0.z = 0;	
        FVector3 u0 = vCursorPos - vScreenStart0;
        FVector3 v0 = FMath::Normalize(vScreenEnd0 - vScreenStart0);

        float t0 = FMath::Dot(u0, v0) / FMath::Length(vScreenEnd0 - vScreenStart0);
        vCursorPos.z = vScreenStart.z + t0 * (vScreenEnd.z - vScreenStart.z);
        TransformScreenPos3ToWorldPos3(vCursorPos, pCamera, vViewport, vInter);
        FVector3 vRayStart,vRayEnd;
        if (!TransformScreenPos2ToWorldPos3StartEnd(x, y, pCamera, vViewport, vRayStart, vRayEnd))
        {
            return false;
        }

        FVector3 vRayDir = FMath::Normalize(vRayEnd - vRayStart);
        FVector3 vInterDir = FMath::Normalize(vInter - vRayStart);
        float f = FMath::Dot(vRayDir, vInterDir);
        if (f < 0.9f)
        {
            return false;
        }

        return true;
    }
    bool FUtil::IntersectLines(float x, float y, FVector3* pvArray, int nCnt, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter, bool bLoop)
    {
        int nOffset = 0;
        if (!bLoop)
            nOffset = 1;
        FVector3 vStart,vEnd;
        for (int i = 0; i < nCnt - nOffset; ++i)
        {
            vStart = pvArray[i % nCnt];
            vEnd = pvArray[(i + 1) % nCnt];
            if (IntersectLine(x, y, vStart, vEnd, pCamera, vViewport, vInter))		
            {
                return true;	
            }	
        }

        return false;
    }
    bool FUtil::IntersectLines(float x, float y, FVector3* pvArray, int nTotalCnt, int nCnt, int nStart, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter, bool bLoop)
    {
        int nOffset = 0;
        if (!bLoop)
            nOffset = 1;
        FVector3 vStart,vEnd;
        for (int i = nStart; i < nCnt - nOffset + nStart; ++i)
        {
            vStart = pvArray[i % nTotalCnt];
            vEnd   = pvArray[(i + 1) % nTotalCnt];
            if (IntersectLine(x, y, vStart,vEnd, pCamera, vViewport, vInter))
            {
                return true;
            }
        }
        
        return false;
    }
    

////Sort
    void FUtil::Sort3(float f1, float f2, float f3, int* pIndex)
    {
        float aV[3] = { f1, f2, f3};
        SortBubble(3, aV, pIndex);
    }
    void FUtil::SortBubble(int count, float* pFloat, int* pIndex)
    {   

        for (int i = 0; i < count -1; i++)
        {
            for (int j = 0; j < count - 1 - i; j++)
            {
                if (pFloat[j] > pFloat[j + 1])
                {
                    float f = pFloat[j];
                    pFloat[j] = pFloat[j + 1];
                    pFloat[j + 1] = f;

                    int index = pIndex[j];
                    pIndex[j] = pIndex[j + 1];
                    pIndex[j + 1] = index;
                }
            }
        }
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