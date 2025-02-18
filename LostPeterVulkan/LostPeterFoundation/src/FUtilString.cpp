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

#include "../include/FUtilString.h"

namespace LostPeterFoundation
{
     const String FUtilString::BLANK;

    void FUtilString::Trim(String& str, bool left /*= true*/, bool right /*= true*/)
    {
        static const String delims = " \t\r";
        size_t index;
        if (right)
        {
            index = str.find_last_not_of(delims);
            if (index != String::npos)
            {
                str.erase(str.begin(), str.begin() + index + 1); // trim right
            }
        }
        if (left)
        {
            index = str.find_first_not_of(delims);
            if (index != String::npos)
            {
                str.erase(str.begin(), str.begin() + index); // trim left
            }
        }
    }

    StringVector FUtilString::Split(const String& str, const String& delims /*= "\t\n "*/, unsigned int maxSplits /*= 0*/)
    {
        StringVector ret;
        ret.reserve(maxSplits ? maxSplits + 1 : 10);    // 10 is guessed capacity for most case

        unsigned int numSplits = 0;
        // Use STL methods 
        size_t start, pos;
        start = 0;
        do 
        {
            pos = str.find_first_of(delims, start);
            if (pos == start)
            {
                // Do nothing
                start = pos + 1;
            }
            else if (pos == String::npos || (maxSplits && numSplits == maxSplits))
            {
                // Copy the rest of the string
                ret.push_back(str.substr(start));
                break;
            }
            else
            {
                // Copy up to delimiter
                ret.push_back(str.substr(start, pos - start));
                start = pos + 1;
            }
            // parse up to next real data
            start = str.find_first_not_of(delims, start);
            ++numSplits;

        } while (pos != String::npos);

        return ret;
    }

    String FUtilString::StandardizePath(const String &init)
	{
		String path = init;
		std::replace(path.begin(), path.end(),'\\', '/');
		if(path[path.length() - 1] != '/')
			path += '/';
		return path;
	}

	void FUtilString::SplitFileName(const String& qualifiedName, String& outBasename, String& outPath)
	{
		String path = qualifiedName;
		// Replace \ with / first
		std::replace(path.begin(), path.end(), '\\', '/');
		// split based on final /
		size_t i = path.find_last_of('/');

		if (i == String::npos)
		{
			outPath.clear();
			outBasename = qualifiedName;
		}
		else
		{
			outBasename = path.substr(i+1, path.size() - i - 1);
			outPath = path.substr(0, i+1);
		}
	}

	void FUtilString::SplitFullFileName(const String& qualifiedName, String& outBasename, String& outExtention, String& outPath)
	{
		String fullName;
		SplitFileName(qualifiedName, fullName, outPath);
		SplitBaseFileName(fullName, outBasename, outExtention);
	}

	void FUtilString::SplitBaseFileName(const String& fullName, String& outBasename, String& outExtention)
	{
		size_t i = fullName.find_last_of(".");
		if (i == String::npos)
		{
			outExtention.clear();
			outBasename = fullName;
		}
		else
		{
			outExtention = fullName.substr(i + 1);
			outBasename = fullName.substr(0, i);
		}
	}

    void FUtilString::ToLowerCase(String& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void FUtilString::ToUpperCase(String& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    bool FUtilString::StartsWith(const String& str, const String& pattern, bool lowerCase /*= true*/)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        String startOfThis = str.substr(0, patternLen);
        if (lowerCase)
            FUtilString::ToLowerCase(startOfThis);

        return (startOfThis == pattern);
    }

    bool FUtilString::EndsWith(const String& str, const String& pattern, bool lowerCase /*= true*/)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        String endOfThis = str.substr(thisLen - patternLen,patternLen);
        if (lowerCase)
            FUtilString::ToLowerCase(endOfThis);

        return (endOfThis == pattern);
    }

    String FUtilString::Replace(const String& src_str, const String& sub_str, const String& new_str)
    {
        String dst_str = src_str;
        String::size_type pos = 0;
        while((pos = dst_str.find(sub_str)) != String::npos)
        {
            dst_str.replace(pos, sub_str.length(), new_str);
        }
        return dst_str;
    }

    bool FUtilString::Match(const String& str, const String& pattern, bool caseSensitive /*= true*/)
    {
        String tmpStr = str;
		String tmpPattern = pattern;
		if (!caseSensitive)
		{
			FUtilString::ToLowerCase(tmpStr);
			FUtilString::ToLowerCase(tmpPattern);
		}

		String::const_iterator strIt = tmpStr.begin();
		String::const_iterator patIt = tmpPattern.begin();
		String::const_iterator lastWildCardIt = tmpPattern.end();
		while (strIt != tmpStr.end() && patIt != tmpPattern.end())
		{
			if (*patIt == '*')
			{
				lastWildCardIt = patIt;
				++patIt;
				if (patIt == tmpPattern.end())
				{
					strIt = tmpStr.end();
				}
				else
				{
					while(strIt != tmpStr.end() && *strIt != *patIt)
						++strIt;
				}
			}
			else
			{
				if (*patIt != *strIt)
				{
					if (lastWildCardIt != tmpPattern.end())
					{
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						return false;
					}
				}
				else
				{
					++patIt;
					++strIt;
				}
			}
		}

		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
			return true;
		}
		
        return false;
    }

    String FUtilString::FormatString(const char* format, ...)
    {
        char szTemp[2048] = {0};
        va_list ap;
        va_start(ap, format);
        vsprintf(szTemp, format, ap);
        va_end(ap);

        return szTemp;
    }

    void FUtilString::AddUnique(StringVector& arr, const String& val)
    {
        bool found = false;
		for (int32 i = 0; i < arr.size(); ++i)
        {
			if (arr[i] == val) 
            {
				found = true;
				break;
			}
		}
        
		if (!found) 
        {
			arr.push_back(val);
		}
    }

    void FUtilString::AddUnique(ConstCharPtrVector& arr, const char* val)
	{
		bool found = false;
		for (int32 i = 0; i < arr.size(); ++i) 
        {
			if (strcmp(arr[i], val) == 0) 
            {
				found = true;
				break;
			}
		}
		
        if (!found) 
        {
			arr.push_back(val);
		}
	}


    bool FUtilString::ParserBool(const String& strValue)
    {
        bool bValue = false;
        if (strValue == "true")
            bValue = true;

        return bValue;
    }
    int8 FUtilString::ParserInt8(const String& strValue)
    {
        int8 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    uint8 FUtilString::ParserUInt8(const String& strValue)
    {
        uint8 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    int16 FUtilString::ParserInt16(const String& strValue)
    {
        int16 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    uint16 FUtilString::ParserUInt16(const String& strValue)
    {
        uint16 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    int32 FUtilString::ParserInt(const String& strValue)
    {
        int32 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    uint32 FUtilString::ParserUInt(const String& strValue)
    {
        uint32 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    size_t FUtilString::ParserSizeT(const String& strValue)
    {
        size_t nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    float FUtilString::ParserFloat(const String& strValue)
    {
        float fValue = 0.0f;
        std::istringstream str(strValue.c_str());
        str >> fValue;

        return fValue;
    }
    double FUtilString::ParserDouble(const String& strValue)
    {
        double dValue = 0;
        std::istringstream str(strValue.c_str());
        str >> dValue;

        return dValue;
    }
    FVector4 FUtilString::ParserColor(const String& strValue)
    {
        FVector4 clValue = FVector4(0, 0, 0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 4)
        {
            clValue = FVector4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return clValue;
    }
    FPointI FUtilString::ParserPointI(const String& strValue)
    {
        FPointI point = FPointI(0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 2)
        {
            point = FPointI(ParserInt(vec[0]), ParserFloat(vec[1]));
        }

        return point;
    }
    FPointF FUtilString::ParserPointF(const String& strValue)
    {
        FPointF point = FPointF(0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 2)
        {
            point = FPointF(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return point;
    }
    FSizeI FUtilString::ParserSizeI(const String& strValue)
    {
        FSizeI size = FSizeI(0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 2)
        {
            size = FSizeI(ParserInt(vec[0]), ParserInt(vec[1]));
        }

        return size;
    }
    FSizeF FUtilString::ParserSizeF(const String& strValue)
    {
        FSizeF size = FSizeF(0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 2)
        {
            size = FSizeF(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return size;
    }
    FRectI FUtilString::ParserRectI(const String& strValue)
    {
        FRectI size = FRectI(0, 0, 0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 4)
        {
            size = FRectI(ParserInt(vec[0]), ParserInt(vec[1]), ParserInt(vec[2]), ParserInt(vec[3]));
        }

        return size;
    }
    FRectF FUtilString::ParserRectF(const String& strValue)
    {
        FRectF size = FRectF(0, 0, 0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 4)
        {
            size = FRectF(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return size;
    }
    FVector2 FUtilString::ParserVector2(const String& strValue)
    {
        FVector2 v2Value = FVector2(0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 2)
        {
            v2Value = FVector2(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return v2Value;
    }
    FVector3 FUtilString::ParserVector3(const String& strValue)
    {
        FVector3 v3Value = FVector3(0, 0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 3)
        {
            v3Value = FVector3(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]));
        }

        return v3Value;
    }
    FVector4 FUtilString::ParserVector4(const String& strValue)
    {
        FVector4 v4Value = FVector4(0, 0, 0, 0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 4)
        {
            v4Value = FVector4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return v4Value;
    }
    FQuaternion FUtilString::ParserQuaternion(const String& strValue)
    {
        FQuaternion qValue = FQuaternion(0, 0, 0, 1);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 4)
        {
            qValue = FQuaternion(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return qValue;
    }
    FMatrix3 FUtilString::ParserMatrix3(const String& strValue)
    {
        FMatrix3 mat3Value = FMatrix3(0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 9)
        {
            mat3Value = FMatrix3(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]),
                                 ParserFloat(vec[3]), ParserFloat(vec[4]), ParserFloat(vec[5]),
                                 ParserFloat(vec[6]), ParserFloat(vec[7]), ParserFloat(vec[8]));
        }

        return mat3Value;
    }
    FMatrix4 FUtilString::ParserMatrix4(const String& strValue)
    {
        FMatrix4 mat4Value = FMatrix4(0);
        StringVector vec = FUtilString::Split(strValue);
        if (vec.size() == 16)
        {
            mat4Value = FMatrix4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]),
                                 ParserFloat(vec[4]), ParserFloat(vec[5]), ParserFloat(vec[6]), ParserFloat(vec[7]),
                                 ParserFloat(vec[8]), ParserFloat(vec[9]), ParserFloat(vec[10]), ParserFloat(vec[11]),
                                 ParserFloat(vec[12]), ParserFloat(vec[13]), ParserFloat(vec[14]), ParserFloat(vec[15]));
        }

        return mat4Value;
    }

    void FUtilString::SaveBool(String& strValue, bool bValue)
    {
        strValue = SaveBool(bValue);
    }
    void FUtilString::SaveInt8(String& strValue, int8 nValue)
    {
        strValue = SaveInt8(nValue);
    }
    void FUtilString::SaveUInt8(String& strValue, uint8 nValue)
    {
        strValue = SaveUInt8(nValue);
    }
    void FUtilString::SaveInt16(String& strValue, int16 nValue)
    {
        strValue = SaveInt16(nValue);
    }
    void FUtilString::SaveUInt16(String& strValue, uint16 nValue)
    {
        strValue = SaveUInt16(nValue);
    }
    void FUtilString::SaveInt(String& strValue, int32 nValue)
    {
        strValue = SaveInt(nValue);
    }
    void FUtilString::SaveUInt(String& strValue, uint32 nValue)
    {
        strValue = SaveUInt(nValue);
    }
    void FUtilString::SaveSizeT(String& strValue, size_t nValue)
    {
        strValue = SaveSizeT(nValue);
    }
    void FUtilString::SaveFloat(String& strValue, float fValue)
    {
        strValue = SaveFloat(fValue);
    }
    void FUtilString::SaveDouble(String& strValue, double dValue)
    {
        strValue = SaveDouble(dValue);
    }
    void FUtilString::SaveColor(String& strValue, const FColor& clValue)
    {
        strValue = SaveColor(clValue);
    }
    void FUtilString::SavePointI(String& strValue, const FPointI& point)
    {
        strValue = SavePointI(point);
    }
    void FUtilString::SavePointF(String& strValue, const FPointF& point)
    {
        strValue = SavePointF(point);
    }
    void FUtilString::SaveSizeI(String& strValue, const FSizeI& size)
    {
        strValue = SaveSizeI(size);
    }
    void FUtilString::SaveSizeF(String& strValue, const FSizeF& size)
    {
        strValue = SaveSizeF(size);
    }
    void FUtilString::SaveRectI(String& strValue, const FRectI& rect)
    {
        strValue = SaveRectI(rect);
    }
    void FUtilString::SaveRectF(String& strValue, const FRectF& rect)
    {
        strValue = SaveRectF(rect);
    }
    void FUtilString::SaveVector2(String& strValue, const FVector2& v2Value)
    {
        strValue = SaveVector2(v2Value);
    }
    void FUtilString::SaveVector3(String& strValue, const FVector3& v3Value)
    {
        strValue = SaveVector3(v3Value);
    }
    void FUtilString::SaveVector4(String& strValue, const FVector4& v4Value)
    {
        strValue = SaveVector4(v4Value);
    }
    void FUtilString::SaveQuaternion(String& strValue, const FQuaternion& qValue)
    {
        strValue = SaveQuaternion(qValue);
    }
    void FUtilString::SaveMatrix3(String& strValue, const FMatrix3& mat3Value)
    {
        strValue = SaveMatrix3(mat3Value);
    }
    void FUtilString::SaveMatrix4(String& strValue, const FMatrix4& mat4Value)
    {
        strValue = SaveMatrix4(mat4Value);
    }


    String FUtilString::SaveBool(bool bValue)
    {
        if (bValue)
            return "true";
        return "false";
    }
    String FUtilString::SaveInt8(int8 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveUInt8(uint8 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveInt16(int16 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveUInt16(uint16 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveInt(int32 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveUInt(uint32 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveSizeT(size_t nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    String FUtilString::SaveFloat(float fValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << fValue;
        return stream.str();
    }
    String FUtilString::SaveDouble(double dValue)
    {
        std::ostringstream stream;
        stream.precision(10);
        stream << dValue;
        return stream.str();
    }
    String FUtilString::SaveColor(const FColor& clValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << clValue.x << " "
               << clValue.y << " "
               << clValue.z << " "
               << clValue.w;
        return stream.str();
    }
    String FUtilString::SavePointI(const FPointI& point)
    {
        std::ostringstream stream;
        stream << point.x << " "
               << point.y;
        return stream.str();
    }
    String FUtilString::SavePointF(const FPointF& point)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << point.x << " "
               << point.y;
        return stream.str();
    }
    String FUtilString::SaveSizeI(const FSizeI& size)
    {
        std::ostringstream stream;
        stream << size.x << " "
               << size.y;
        return stream.str();
    }
    String FUtilString::SaveSizeF(const FSizeF& size)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << size.x << " "
               << size.y;
        return stream.str();
    }
    String FUtilString::SaveRectI(const FRectI& rect)
    {
        std::ostringstream stream;
        stream << rect.left << " "
               << rect.top << " "
               << rect.right << " "
               << rect.bottom;
        return stream.str();
    }
    String FUtilString::SaveRectF(const FRectF& rect)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << rect.left << " "
               << rect.top << " "
               << rect.right << " "
               << rect.bottom;
        return stream.str();
    }
    String FUtilString::SaveVector2(const FVector2& v2Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v2Value.x << " "
               << v2Value.y;
        return stream.str();
    }
    String FUtilString::SaveVector3(const FVector3& v3Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v3Value.x << " "
               << v3Value.y << " "
               << v3Value.z;
        return stream.str();
    }
    String FUtilString::SaveVector4(const FVector4& v4Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v4Value.x << " "
               << v4Value.y << " "
               << v4Value.z << " "
               << v4Value.w;
        return stream.str();
    }
    String FUtilString::SaveQuaternion(const FQuaternion& qValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << qValue.x << " "
               << qValue.y << " "
               << qValue.z << " "
               << qValue.w;
        return stream.str();
    }
    String FUtilString::SaveMatrix3(const FMatrix3& mat3Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << mat3Value[0][0] << " " << mat3Value[0][1] << " " << mat3Value[0][2] << " " 
               << mat3Value[1][0] << " " << mat3Value[1][1] << " " << mat3Value[1][2] << " " 
               << mat3Value[2][0] << " " << mat3Value[2][1] << " " << mat3Value[2][2];
        return stream.str();
    }
    String FUtilString::SaveMatrix4(const FMatrix4& mat4Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << mat4Value[0][0] << " " << mat4Value[0][1] << " " << mat4Value[0][2] << " " << mat4Value[0][3] << " " 
               << mat4Value[1][0] << " " << mat4Value[1][1] << " " << mat4Value[1][2] << " " << mat4Value[1][3] << " " 
               << mat4Value[2][0] << " " << mat4Value[2][1] << " " << mat4Value[2][2] << " " << mat4Value[2][3] << " " 
               << mat4Value[3][0] << " " << mat4Value[3][1] << " " << mat4Value[3][2] << " " << mat4Value[3][3];
        return stream.str();
    }

}; //LostPeterFoundation