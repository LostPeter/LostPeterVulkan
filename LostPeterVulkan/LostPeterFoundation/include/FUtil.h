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
    //////////////////////// Common ////////////////////////
    public:
        static String ms_strPathBin;
        static const String& GetPathBinSaved(); 

    ////Path
        static String GetPathExecute();
        static String GetPathBin();
        static String GetPathAssets();
        static String GetPathReal(const char* szFile);
        static String GetPathReal(const String& strPath);

    ////File
        //Path Absolute
        static bool IsExistFile(const String& strPath);
        static bool DeleteFile(const String& strPath);
        static bool ClearFile(const String& strPath);
        static bool CopyFile(const String& strSrcPath, const String& strDstPath);

        static bool IsExistAssetFile(const String& strPath);
        static bool DeleteAssetFile(const String& strPath);
        static bool ClearAssetFile(const String& strPath);
        static bool CopyAssetFile(const String& strSrcPath, const String& strDstPath);

    ////Directory
        //Path Absolute
        static bool IsDirectory(const String& strPath);
		static bool CreateDirectory(const String& strPath);
        static bool DeleteDirectory(const String& strPath);

        //Path Relative Assets/XXX
        static bool IsAssetDirectory(const String& strPath);
		static bool CreateAssetDirectory(const String& strPath);
        static bool DeleteAssetDirectory(const String& strPath);

    ////File/Folder Op
        //Path Absolute
		static bool EnumFiles(const String& strFolderPath, StringVector& aFiles, bool bFilePath);
		static bool EnumFiles(const String& strFolderPath, String2StringMap& mapFiles, bool bIsRecursive);
		static bool EnumFolders(const String& strFolderPath, StringVector& aFolders, bool bFolderPath, bool bIsRecursive);
        static bool	GenerateFolders(const String& strPath);	
        static bool DeleteFolders(const String& strFolderPath);

        //Path Relative Assets/XXX
        static bool EnumAssetFiles(const String& strFolderPath, StringVector& aFiles, bool bFilePath);
		static bool EnumAssetFiles(const String& strFolderPath, String2StringMap& mapFiles, bool bIsRecursive);
		static bool EnumAssetFolders(const String& strFolderPath, StringVector& aFolders, bool bFolderPath, bool bIsRecursive);
        static bool	GenerateAssetFolders(const String& strPath);	
        static bool DeleteAssetFolders(const String& strFolderPath);
        
    ////LoadFile
        //Path Absolute
        static bool LoadFileContent(const char* szFile, CharVector& content, bool addEnd0 = false);
        static bool LoadFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadFileToString(const char* szFile, String& contentFile);

        //Path Relative Assets/XXX
        static bool LoadAssetFileContent(const char* szFile, CharVector& content, bool addEnd0 = false);
        static bool LoadAssetFileToBuffer(const char* szFile, uint8** ppData, int32& sizeData, bool addEnd0 = false);
        static bool LoadAssetFileToString(const char* szFile, String& contentFile);

    ////SaveFile
        //Path Absolute
        static bool SaveFileContent(const char* szFile, CharVector& content);
        static bool SaveFileContent(const char* szFile, uint8* pData, int32 sizeData);
        static bool SaveFileContent(const char* szFile, String& contentFile);

        //Path Relative Assets/XXX
        static bool SaveAssetFileContent(const char* szFile, CharVector& content);
        static bool SaveAssetFileContent(const char* szFile, uint8* pData, int32 sizeData);
        static bool SaveAssetFileContent(const char* szFile, String& contentFile);

    ////Screen - World Transform
        static bool TransformScreenPos2ToWorldRay(const FVector2& vPosSceen2, FCamera* pCamera, const FVector4& vViewport, FRay* pOutRay);
        static bool TransformScreenPos2ToWorldRay(float screenX, float screenY,  FCamera* pCamera, const FVector4& vViewport, FRay* pOutRay);

        static bool	TransformScreenPos3ToWorldPos3(const FVector3& vPosScreen3, FCamera* pCamera, const FVector4& vViewport, FVector3& vPosWorld);
        static bool	TransformScreenPos2ToWorldPos3StartEnd(float screenX, float screenY, FCamera* pCamera, const FVector4& vViewport, FVector3& vStart, FVector3& vEnd);

        static bool	TransformWorldPos3ToScreenPos2(const FVector3& vPosWorld, FCamera* pCamera, const FVector4& vViewport, FVector2& vPosScreen);
        static bool	TransformWorldPos3ToScreenPos3(const FVector3& vPosWorld, FCamera* pCamera, const FVector4& vViewport, FVector3& vPosScreen);
        
        static bool	IsBetween(float fl, float fr, float ft);
        static bool	IntersectLine(float x, float y, const FVector3& vStart, const FVector3& vEnd, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter);
        static bool	IntersectLines(float x, float y, FVector3* pvArray, int nCnt, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter, bool bLoop);
        static bool	IntersectLines(float x, float y, FVector3* pvArray, int nTotalCnt, int nCnt, int nStart, FCamera* pCamera, const FVector4& vViewport, FVector3& vInter, bool bLoop);

    ////Sort
        static void Sort3(float f1, float f2, float f3, int* pIndex);
        static void SortBubble(int count, float* pFloat, int* pIndex);

    //////////////////////// Thread ////////////////////////
    public:
        //int8/16/32/64 [+-] 1
        static int8 InterlockedIncrement(volatile int8* pValue);
        static int8 InterlockedDecrement(volatile int8* pValue);
        static int16 InterlockedIncrement(volatile int16* pValue);
        static int16 InterlockedDecrement(volatile int16* pValue);
        static int32 InterlockedIncrement(volatile int32* pValue);
        static int32 InterlockedDecrement(volatile int32* pValue);
        static int64 InterlockedIncrement(volatile int64* pValue);
        static int64 InterlockedDecrement(volatile int64* pValue);

        //int8/16/32/64 [+-] nAmount
        static int8 InterlockedAdd(volatile int8* pValue, int8 nAmount);
        static int8 InterlockedSub(volatile int8* pValue, int8 nAmount);
        static int16 InterlockedAdd(volatile int16* pValue, int16 nAmount);
        static int16 InterlockedSub(volatile int16* pValue, int16 nAmount);
        static int32 InterlockedAdd(volatile int32* pValue, int32 nAmount);
        static int32 InterlockedSub(volatile int32* pValue, int32 nAmount);
        static int64 InterlockedAdd(volatile int64* pValue, int64 nAmount);
        static int64 InterlockedSub(volatile int64* pValue, int64 nAmount);

        //int8/16/32/64 exchange nExchange
        static int8 InterlockedExchange(volatile int8* pValue, int8 nExchange);
        static int16 InterlockedExchange(volatile int16* pValue, int16 nExchange);
        static int32 InterlockedExchange(volatile int32* pValue, int32 nExchange);
        static int64 InterlockedExchange(volatile int64* pValue, int64 nExchange);
        
        //int8/16/32/64 compare nComperand and exchange nExchange
        static int8 InterlockedCompareExchange(volatile int8* pDest, int8 nExchange, int8 nComperand);
        static int16 InterlockedCompareExchange(volatile int16* pDest, int16 nExchange, int16 nComperand);
        static int32 InterlockedCompareExchange(volatile int32* pDest, int32 nExchange, int32 nComperand);
        static int64 InterlockedCompareExchange(volatile int64* pDest, int64 nExchange, int64 nComperand);

        //exchange Ptr
        static void* InterlockedExchangePtr(void** ppDest, void* pExchange);
        static void* InterlockedCompareExchangePtr(void** ppDest, void* pExchange, void* pComperand);

        //int8/16/32/64 read pSrc
        static int8 AtomicRead(volatile const int8* pSrc);
        static int16 AtomicRead(volatile const int16* pSrc);
        static int32 AtomicRead(volatile const int32* pSrc);
        static int64 AtomicRead(volatile const int64* pSrc);

        //int8/16/32/64 read relaxed pSrc
        static int8 AtomicRead_Relaxed(volatile const int8* pSrc);
        static int16 AtomicRead_Relaxed(volatile const int16* pSrc);
        static int32 AtomicRead_Relaxed(volatile const int32* pSrc);
        static int64 AtomicRead_Relaxed(volatile const int64* pSrc);

        //int8/16/32/64 store nValue to pSrc
        static void AtomicStore(volatile int8* pSrc, int8 nValue);
        static void AtomicStore(volatile int16* pSrc, int16 nValue);
        static void AtomicStore(volatile int32* pSrc, int32 nValue);
        static void AtomicStore(volatile int64* pSrc, int64 nValue);

        //int8/16/32/64 store relaxed nValue to pSrc
        static void AtomicStore_Relaxed(volatile int8* pSrc, int8 nValue);
        static void AtomicStore_Relaxed(volatile int16* pSrc, int16 nValue);
        static void AtomicStore_Relaxed(volatile int32* pSrc, int32 nValue);
        static void AtomicStore_Relaxed(volatile int64* pSrc, int64 nValue);
    };

}; //LostPeterFoundation

#endif