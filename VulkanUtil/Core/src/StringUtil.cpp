#include "../include/PreInclude.h"
#include "../include/StringUtil.h"

namespace LostPeter
{
    const std::string StringUtil::BLANK;

    void StringUtil::Trim(std::string& str, bool left /*= true*/, bool right /*= true*/)
    {
        static const std::string delims = " \t\r";
        size_t index;
        if (right)
        {
            index = str.find_last_not_of(delims);
            if (index != std::string::npos)
            {
                str.erase(str.begin(), str.begin() + index + 1); // trim right
            }
        }
        if (left)
        {
            index = str.find_first_not_of(delims);
            if (index != std::string::npos)
            {
                str.erase(str.begin(), str.begin() + index); // trim left
            }
        }
    }

    std::vector<std::string> StringUtil::Split(const std::string& str, const std::string& delims /*= "\t\n "*/, unsigned int maxSplits /*= 0*/)
    {
        std::vector<std::string> ret;
        ret.reserve(maxSplits ? maxSplits+1 : 10);    // 10 is guessed capacity for most case

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
            else if (pos == std::string::npos || (maxSplits && numSplits == maxSplits))
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

        } while (pos != std::string::npos);

        return ret;
    }

    void StringUtil::ToLowerCase(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), tolower);
    }

    void StringUtil::ToUpperCase(std::string& str)
    {
        std::transform(str.begin(), str.end(), str.begin(), toupper);
    }

    bool StringUtil::StartsWith(const std::string& str, const std::string& pattern, bool lowerCase /*= true*/)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        std::string startOfThis = str.substr(0, patternLen);
        if (lowerCase)
            StringUtil::ToLowerCase(startOfThis);

        return (startOfThis == pattern);
    }

    bool StringUtil::EndsWith(const std::string& str, const std::string& pattern, bool lowerCase /*= true*/)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        std::string endOfThis = str.substr(thisLen - patternLen,patternLen);
        if (lowerCase)
            StringUtil::ToLowerCase(endOfThis);

        return (endOfThis == pattern);
    }

    std::string StringUtil::Replace(const std::string& src_str, const std::string& sub_str, const std::string& new_str)
    {
        std::string dst_str = src_str;
        std::string::size_type pos = 0;
        while((pos = dst_str.find(sub_str)) != std::string::npos)
        {
            dst_str.replace(pos, sub_str.length(), new_str);
        }
        return dst_str;
    }

    std::string StringUtil::FormatString(const char* format, ...)
    {
        char szTemp[2048] = {0};
        va_list ap;
        va_start(ap, format);
        vsprintf(szTemp, format, ap);
        va_end(ap);

        return szTemp;
    }

    void StringUtil::AddUnique(std::vector<std::string>& arr, const std::string& val)
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

    void StringUtil::AddUnique(std::vector<const char*>& arr, const char* val)
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


    bool StringUtil::ParserBool(const std::string& strValue)
    {
        bool bValue = false;
        if (strValue == "true")
            bValue = true;

        return bValue;
    }
    int32 StringUtil::ParserInt(const std::string& strValue)
    {
        int32 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    uint32 StringUtil::ParserUInt(const std::string& strValue)
    {
        uint32 nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    size_t StringUtil::ParserSizeT(const std::string& strValue)
    {
        size_t nValue = 0;
        std::istringstream str(strValue.c_str());
        str >> nValue;

        return nValue;
    }
    float StringUtil::ParserFloat(const std::string& strValue)
    {
        float fValue = 0.0f;
        std::istringstream str(strValue.c_str());
        str >> fValue;

        return fValue;
    }
    double StringUtil::ParserDouble(const std::string& strValue)
    {
        double dValue = 0;
        std::istringstream str(strValue.c_str());
        str >> dValue;

        return dValue;
    }
    glm::vec4 StringUtil::ParserColor(const std::string& strValue)
    {
        glm::vec4 clValue = glm::vec4(0, 0, 0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 4)
        {
            clValue = glm::vec4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return clValue;
    }
    glm::vec2 StringUtil::ParserSize(const std::string& strValue)
    {
        glm::vec2 size = glm::vec2(0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 2)
        {
            size = glm::vec2(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return size;
    }
    glm::vec2 StringUtil::ParserPoint(const std::string& strValue)
    {
        glm::vec2 point = glm::vec2(0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 2)
        {
            point = glm::vec2(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return point;
    }
    glm::vec2 StringUtil::ParserVector2(const std::string& strValue)
    {
        glm::vec2 v2Value = glm::vec2(0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 2)
        {
            v2Value = glm::vec2(ParserFloat(vec[0]), ParserFloat(vec[1]));
        }

        return v2Value;
    }
    glm::vec3 StringUtil::ParserVector3(const std::string& strValue)
    {
        glm::vec3 v3Value = glm::vec3(0, 0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 3)
        {
            v3Value = glm::vec3(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]));
        }

        return v3Value;
    }
    glm::vec4 StringUtil::ParserVector4(const std::string& strValue)
    {
        glm::vec4 v4Value = glm::vec4(0, 0, 0, 0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 4)
        {
            v4Value = glm::vec4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return v4Value;
    }
    glm::quat StringUtil::ParserQuaternion(const std::string& strValue)
    {
        glm::quat qValue = glm::quat(0, 0, 0, 1);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 4)
        {
            qValue = glm::quat(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]));
        }

        return qValue;
    }
    glm::mat3 StringUtil::ParserMatrix3(const std::string& strValue)
    {
        glm::mat3 mat3Value = glm::mat3(0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 9)
        {
            mat3Value = glm::mat3(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]),
                                  ParserFloat(vec[3]), ParserFloat(vec[4]), ParserFloat(vec[5]),
                                  ParserFloat(vec[6]), ParserFloat(vec[7]), ParserFloat(vec[8]));
        }

        return mat3Value;
    }
    glm::mat4 StringUtil::ParserMatrix4(const std::string& strValue)
    {
        glm::mat4 mat4Value = glm::mat4(0);
        std::vector<std::string> vec = StringUtil::Split(strValue);
        if (vec.size() == 16)
        {
            mat4Value = glm::mat4(ParserFloat(vec[0]), ParserFloat(vec[1]), ParserFloat(vec[2]), ParserFloat(vec[3]),
                                  ParserFloat(vec[4]), ParserFloat(vec[5]), ParserFloat(vec[6]), ParserFloat(vec[7]),
                                  ParserFloat(vec[8]), ParserFloat(vec[9]), ParserFloat(vec[10]), ParserFloat(vec[11]),
                                  ParserFloat(vec[12]), ParserFloat(vec[13]), ParserFloat(vec[14]), ParserFloat(vec[15]));
        }

        return mat4Value;
    }

    void StringUtil::SaveBool(std::string& strValue, bool bValue)
    {
        strValue = SaveBool(bValue);
    }
    void StringUtil::SaveInt(std::string& strValue, int32 nValue)
    {
    strValue = SaveInt(nValue);
    }
    void StringUtil::SaveUInt(std::string& strValue, uint32 nValue)
    {
        strValue = SaveUInt(nValue);
    }
    void StringUtil::SaveSizeT(std::string& strValue, size_t nValue)
    {
        strValue = SaveSizeT(nValue);
    }
    void StringUtil::SaveFloat(std::string& strValue, float fValue)
    {
        strValue = SaveFloat(fValue);
    }
    void StringUtil::SaveDouble(std::string& strValue, double dValue)
    {
        strValue = SaveDouble(dValue);
    }
    void StringUtil::SaveColor(std::string& strValue, const glm::vec4& clValue)
    {
        strValue = SaveColor(clValue);
    }
    void StringUtil::SaveSize(std::string& strValue, const glm::vec2& size)
    {
        strValue = SaveSize(size);
    }
    void StringUtil::SavePoint(std::string& strValue, const glm::vec2& point)
    {
        strValue = SavePoint(point);
    }
    void StringUtil::SaveVector2(std::string& strValue, const glm::vec2& v2Value)
    {
        strValue = SaveVector2(v2Value);
    }
    void StringUtil::SaveVector3(std::string& strValue, const glm::vec3& v3Value)
    {
        strValue = SaveVector3(v3Value);
    }
    void StringUtil::SaveVector4(std::string& strValue, const glm::vec4& v4Value)
    {
        strValue = SaveVector4(v4Value);
    }
    void StringUtil::SaveQuaternion(std::string& strValue, const glm::quat& qValue)
    {
        strValue = SaveQuaternion(qValue);
    }
    void StringUtil::SaveMatrix3(std::string& strValue, const glm::mat3& mat3Value)
    {
        strValue = SaveMatrix3(mat3Value);
    }
    void StringUtil::SaveMatrix4(std::string& strValue, const glm::mat4& mat4Value)
    {
        strValue = SaveMatrix4(mat4Value);
    }


    std::string StringUtil::SaveBool(bool bValue)
    {
        if (bValue)
            return "true";
        return "false";
    }
    std::string StringUtil::SaveInt(int32 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    std::string StringUtil::SaveUInt(uint32 nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    std::string StringUtil::SaveSizeT(size_t nValue)
    {
        std::ostringstream stream;
        stream << nValue;
        return stream.str();
    }
    std::string StringUtil::SaveFloat(float fValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << fValue;
        return stream.str();
    }
    std::string StringUtil::SaveDouble(double dValue)
    {
        std::ostringstream stream;
        stream.precision(10);
        stream << dValue;
        return stream.str();
    }
    std::string StringUtil::SaveColor(const glm::vec4& clValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << clValue.x << " "
               << clValue.y << " "
               << clValue.z << " "
               << clValue.w;
        return stream.str();
    }
    std::string StringUtil::SaveSize(const glm::vec2& size)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << size.x << " "
               << size.y;
        return stream.str();
    }
    std::string StringUtil::SavePoint(const glm::vec2& point)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << point.x << " "
               << point.y;
        return stream.str();
    }
    std::string StringUtil::SaveVector2(const glm::vec2& v2Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v2Value.x << " "
               << v2Value.y;
        return stream.str();
    }
    std::string StringUtil::SaveVector3(const glm::vec3& v3Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v3Value.x << " "
               << v3Value.y << " "
               << v3Value.z;
        return stream.str();
    }
    std::string StringUtil::SaveVector4(const glm::vec4& v4Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << v4Value.x << " "
               << v4Value.y << " "
               << v4Value.z << " "
               << v4Value.w;
        return stream.str();
    }
    std::string StringUtil::SaveQuaternion(const glm::quat& qValue)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << qValue.x << " "
               << qValue.y << " "
               << qValue.z << " "
               << qValue.w;
        return stream.str();
    }
    std::string StringUtil::SaveMatrix3(const glm::mat3& mat3Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << mat3Value[0][0] << " " << mat3Value[0][1] << " " << mat3Value[0][2] << " " 
               << mat3Value[1][0] << " " << mat3Value[1][1] << " " << mat3Value[1][2] << " " 
               << mat3Value[2][0] << " " << mat3Value[2][1] << " " << mat3Value[2][2];
        return stream.str();
    }
    std::string StringUtil::SaveMatrix4(const glm::mat4& mat4Value)
    {
        std::ostringstream stream;
        stream.precision(6);
        stream << mat4Value[0][0] << " " << mat4Value[0][1] << " " << mat4Value[0][2] << " " << mat4Value[0][3] << " " 
               << mat4Value[1][0] << " " << mat4Value[1][1] << " " << mat4Value[1][2] << " " << mat4Value[1][3] << " " 
               << mat4Value[2][0] << " " << mat4Value[2][1] << " " << mat4Value[2][2] << " " << mat4Value[2][3] << " " 
               << mat4Value[3][0] << " " << mat4Value[3][1] << " " << mat4Value[3][2] << " " << mat4Value[3][3];
        return stream.str();
    }

}; //LostPeter