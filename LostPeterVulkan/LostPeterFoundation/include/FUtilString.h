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

#ifndef _F_UTIL_STRING_H_
#define _F_UTIL_STRING_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FUtilString
    {
    public:
        static const String BLANK;

    public:
        static void	Trim(String& str, bool left = true, bool right = true);
        static StringVector Split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);

		static String StandardizePath(const String& init);
		static void	SplitFileName(const String& qualifiedName, String& outBasename, String& outPath);
		static void	SplitFullFileName(const String& qualifiedName,String& outBasename, String& outExtention,String& outPath);
		static void	SplitBaseFileName(const String& fullName,String& outBasename, String& outExtention);
        
        static void	ToLowerCase(String& str);
        static void	ToUpperCase(String& str);

        static bool	StartsWith(const String& str, const String& pattern, bool lowerCase = true);
        static bool	EndsWith(const String& str, const String& pattern, bool lowerCase = true);

        static String Replace(const String& src_str, const String& sub_str, const String& new_str);
        static bool	Match(const String& str, const String& pattern, bool caseSensitive = true);

        static String FormatString(const char* format, ...);

        static void AddUnique(StringVector& arr, const String& val);
        static void AddUnique(ConstCharPtrVector& arr, const char* val);

    public:
        static bool	ParserBool(const String& strValue);
        static int8 ParserInt8(const String& strValue);
        static uint8 ParserUInt8(const String& strValue);
        static int16 ParserInt16(const String& strValue);
        static uint16 ParserUInt16(const String& strValue);
        static int32 ParserInt(const String& strValue);
        static uint32 ParserUInt(const String& strValue);
        static size_t ParserSizeT(const String& strValue);
        static float ParserFloat(const String& strValue);
        static double ParserDouble(const String& strValue);
        static FColor ParserColor(const String& strValue);
        static FPointI ParserPointI(const String& strValue);
        static FPointF ParserPointF(const String& strValue);
        static FSizeI ParserSizeI(const String& strValue);
        static FSizeF ParserSizeF(const String& strValue);
        static FRectI ParserRectI(const String& strValue);
        static FRectF ParserRectF(const String& strValue);
        static FVector2 ParserVector2(const String& strValue);
        static FVector3 ParserVector3(const String& strValue);
        static FVector4 ParserVector4(const String& strValue);
        static FQuaternion ParserQuaternion(const String& strValue);
        static FMatrix3 ParserMatrix3(const String& strValue);
        static FMatrix4 ParserMatrix4(const String& strValue);

        static void	SaveBool(String& strValue, bool bValue);
        static void	SaveInt8(String& strValue, int8 nValue);
        static void	SaveUInt8(String& strValue, uint8 nValue);
        static void	SaveInt16(String& strValue, int16 nValue);
        static void	SaveUInt16(String& strValue, uint16 nValue);
        static void	SaveInt(String& strValue, int32 nValue);
        static void	SaveUInt(String& strValue, uint32 nValue);
        static void	SaveSizeT(String& strValue, size_t nValue);
        static void	SaveFloat(String& strValue, float fValue);
        static void	SaveDouble(String& strValue, double dValue);
        static void	SaveColor(String& strValue, const FColor& clValue);
        static void	SavePointI(String& strValue, const FPointI& point);
        static void	SavePointF(String& strValue, const FPointF& point);
        static void	SaveSizeI(String& strValue, const FSizeI& size);
        static void	SaveSizeF(String& strValue, const FSizeF& size);
        static void	SaveRectI(String& strValue, const FRectI& rect);
        static void	SaveRectF(String& strValue, const FRectF& rect);
        static void	SaveVector2(String& strValue, const FVector2& v2Value);
        static void	SaveVector3(String& strValue, const FVector3& v3Value);
        static void	SaveVector4(String& strValue, const FVector4& v4Value);
        static void	SaveQuaternion(String& strValue, const FQuaternion& qValue);
        static void	SaveMatrix3(String& strValue, const FMatrix3& mat3Value);
        static void	SaveMatrix4(String& strValue, const FMatrix4& mat4Value);

        static String SaveBool(bool bValue);
        static String SaveInt8(int8 nValue);
        static String SaveUInt8(uint8 nValue);
        static String SaveInt16(int16 nValue);
        static String SaveUInt16(uint16 nValue);
        static String SaveInt(int32 nValue);
        static String SaveUInt(uint32 nValue);
        static String SaveSizeT(size_t nValue);
        static String SaveFloat(float fValue);
        static String SaveDouble(double dValue);
        static String SaveColor(const FColor& clValue);
        static String SavePointI(const FPointI& point);
        static String SavePointF(const FPointF& point);
        static String SaveSizeI(const FSizeI& size);
        static String SaveSizeF(const FSizeF& size);
        static String SaveRectI(const FRectI& rect);
        static String SaveRectF(const FRectF& rect);
        static String SaveVector2(const FVector2& v2Value);
        static String SaveVector3(const FVector3& v3Value);
        static String SaveVector4(const FVector4& v4Value);
        static String SaveQuaternion(const FQuaternion& qValue);
        static String SaveMatrix3(const FMatrix3& mat3Value);
        static String SaveMatrix4(const FMatrix4& mat4Value);
    };

}; //LostPeterFoundation

#endif