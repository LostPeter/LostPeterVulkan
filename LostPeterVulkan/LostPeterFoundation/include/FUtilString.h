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
    class LPF_Export FUtilString
    {
    public:
        static const String BLANK;

    public:
        static void	Trim(String& str, bool left = true, bool right = true);
        static StringVector Split(const String& str, const String& delims = "\t\n ", unsigned int maxSplits = 0);
        
        static void	ToLowerCase(String& str);
        static void	ToUpperCase(String& str);

        static bool	StartsWith(const String& str, const String& pattern, bool lowerCase = true);
        static bool	EndsWith(const String& str, const String& pattern, bool lowerCase = true);

        static String Replace(const String& src_str, const String& sub_str, const String& new_str);

        static String FormatString(const char* format, ...);

        static void AddUnique(StringVector& arr, const String& val);
        static void AddUnique(ConstCharPtrVector& arr, const char* val);

    public:
        static bool	ParserBool(const String& strValue);
        static int32 ParserInt(const String& strValue);
        static uint32 ParserUInt(const String& strValue);
        static size_t ParserSizeT(const String& strValue);
        static float ParserFloat(const String& strValue);
        static double ParserDouble(const String& strValue);
        static FColor ParserColor(const String& strValue);
        static FSize ParserSize(const String& strValue);
        static FPoint ParserPoint(const String& strValue);
        static FVector2 ParserVector2(const String& strValue);
        static FVector3 ParserVector3(const String& strValue);
        static FVector4 ParserVector4(const String& strValue);
        static FQuaternion ParserQuaternion(const String& strValue);
        static FMatrix3 ParserMatrix3(const String& strValue);
        static FMatrix4 ParserMatrix4(const String& strValue);

        static void	SaveBool(String& strValue, bool bValue);
        static void	SaveInt(String& strValue, int32 nValue);
        static void	SaveUInt(String& strValue, uint32 nValue);
        static void	SaveSizeT(String& strValue, size_t nValue);
        static void	SaveFloat(String& strValue, float fValue);
        static void	SaveDouble(String& strValue, double dValue);
        static void	SaveColor(String& strValue, const FColor& clValue);
        static void	SaveSize(String& strValue, const FSize& size);
        static void	SavePoint(String& strValue, const FPoint& point);
        static void	SaveVector2(String& strValue, const FVector2& v2Value);
        static void	SaveVector3(String& strValue, const FVector3& v3Value);
        static void	SaveVector4(String& strValue, const FVector4& v4Value);
        static void	SaveQuaternion(String& strValue, const FQuaternion& qValue);
        static void	SaveMatrix3(String& strValue, const FMatrix3& mat3Value);
        static void	SaveMatrix4(String& strValue, const FMatrix4& mat4Value);

        static String SaveBool(bool bValue);
        static String SaveInt(int32 nValue);
        static String SaveUInt(uint32 nValue);
        static String SaveSizeT(size_t nValue);
        static String SaveFloat(float fValue);
        static String SaveDouble(double dValue);
        static String SaveColor(const FColor& clValue);
        static String SaveSize(const FSize& size);
        static String SavePoint(const FPoint& point);
        static String SaveVector2(const FVector2& v2Value);
        static String SaveVector3(const FVector3& v3Value);
        static String SaveVector4(const FVector4& v4Value);
        static String SaveQuaternion(const FQuaternion& qValue);
        static String SaveMatrix3(const FMatrix3& mat3Value);
        static String SaveMatrix4(const FMatrix4& mat4Value);
    };

}; //LostPeterFoundation

#endif