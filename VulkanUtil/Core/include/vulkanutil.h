// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_UTIL_H_
#define _VULKAN_UTIL_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport VulkanUtil
    {
    public:

    public:
        static std::string GetAssetsPath(); 

        static bool GetFileContent(const std::string& pathFile, std::string& contentFile);

        static std::vector<char> ReadFile(const std::string& pathFile);
    };

}; //LibUtil

#endif