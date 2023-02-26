/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-11-20
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _VULKAN_UTIL_STRING_H_
#define _VULKAN_UTIL_STRING_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanUtilString
    {
    public:
        static const std::string BLANK;

    public:
        static void	Trim(std::string& str, bool left = true, bool right = true);
        static std::vector<std::string> Split(const std::string& str, const std::string& delims = "\t\n ", unsigned int maxSplits = 0);
        
        static void	ToLowerCase(std::string& str);
        static void	ToUpperCase(std::string& str);

        static bool	StartsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);
        static bool	EndsWith(const std::string& str, const std::string& pattern, bool lowerCase = true);

        static std::string Replace(const std::string& src_str, const std::string& sub_str, const std::string& new_str);

        static std::string FormatString(const char* format, ...);

        static void AddUnique(std::vector<std::string>& arr, const std::string& val);
        static void AddUnique(std::vector<const char*>& arr, const char* val);

    public:
        static bool	ParserBool(const std::string& strValue);
        static int32 ParserInt(const std::string& strValue);
        static uint32 ParserUInt(const std::string& strValue);
        static size_t ParserSizeT(const std::string& strValue);
        static float ParserFloat(const std::string& strValue);
        static double ParserDouble(const std::string& strValue);
        static glm::vec4 ParserColor(const std::string& strValue);
        static glm::vec2 ParserSize(const std::string& strValue);
        static glm::vec2 ParserPoint(const std::string& strValue);
        static glm::vec2 ParserVector2(const std::string& strValue);
        static glm::vec3 ParserVector3(const std::string& strValue);
        static glm::vec4 ParserVector4(const std::string& strValue);
        static glm::quat ParserQuaternion(const std::string& strValue);
        static glm::mat3 ParserMatrix3(const std::string& strValue);
        static glm::mat4 ParserMatrix4(const std::string& strValue);

        static void	SaveBool(std::string& strValue, bool bValue);
        static void	SaveInt(std::string& strValue, int32 nValue);
        static void	SaveUInt(std::string& strValue, uint32 nValue);
        static void	SaveSizeT(std::string& strValue, size_t nValue);
        static void	SaveFloat(std::string& strValue, float fValue);
        static void	SaveDouble(std::string& strValue, double dValue);
        static void	SaveColor(std::string& strValue, const glm::vec4& clValue);
        static void	SaveSize(std::string& strValue, const glm::vec2& size);
        static void	SavePoint(std::string& strValue, const glm::vec2& point);
        static void	SaveVector2(std::string& strValue, const glm::vec2& v2Value);
        static void	SaveVector3(std::string& strValue, const glm::vec3& v3Value);
        static void	SaveVector4(std::string& strValue, const glm::vec4& v4Value);
        static void	SaveQuaternion(std::string& strValue, const glm::quat& qValue);
        static void	SaveMatrix3(std::string& strValue, const glm::mat3& mat3Value);
        static void	SaveMatrix4(std::string& strValue, const glm::mat4& mat4Value);

        static std::string SaveBool(bool bValue);
        static std::string SaveInt(int32 nValue);
        static std::string SaveUInt(uint32 nValue);
        static std::string SaveSizeT(size_t nValue);
        static std::string SaveFloat(float fValue);
        static std::string SaveDouble(double dValue);
        static std::string SaveColor(const glm::vec4& clValue);
        static std::string SaveSize(const glm::vec2& size);
        static std::string SavePoint(const glm::vec2& point);
        static std::string SaveVector2(const glm::vec2& v2Value);
        static std::string SaveVector3(const glm::vec3& v3Value);
        static std::string SaveVector4(const glm::vec4& v4Value);
        static std::string SaveQuaternion(const glm::quat& qValue);
        static std::string SaveMatrix3(const glm::mat3& mat3Value);
        static std::string SaveMatrix4(const glm::mat4& mat4Value);
    };

}; //LostPeter

#endif