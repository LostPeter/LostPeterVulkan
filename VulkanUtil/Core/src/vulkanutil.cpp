#include "../include/preinclude.h"
#include "../include/vulkanutil.h"
#include "../include/ocutil.h"

namespace LibUtil
{

#if UTIL_PLATFORM == UTIL_PLATFORM_WIN32
    char* Unicode2Utf8(wchar_t* unicodeStr) {
        int cStrLen = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, NULL, 0, NULL, NULL);
        char* cStr = (char*)malloc(sizeof(char) * (cStrLen + 1));
        WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, cStr, cStrLen + 1, NULL, NULL);
        *(cStr + cStrLen) = '\0';
        return cStr;
    }
#endif

    std::string VulkanUtil::GetAssetsPath()
    {
        std::string path;

        #if UTIL_PLATFORM == UTIL_PLATFORM_WIN32
            wchar_t szBuf[512];
            ::GetModuleFileNameW(NULL, szBuf, 512);
            ::PathRemoveFileSpecW(szBuf);
            char* utf8 = Unicode2Utf8(szBuf);

            path.append(utf8);
            free(utf8); 

            std::replace(path.begin(), path.end(), '\\', '/');

        #elif UTIL_PLATFORM == UTIL_PLATFORM_MAC
            path = OCUtil_GetPathExecute();
           
        #endif
            std::cout << "Path exe: " << path << std::endl; 
            if (path[path.size() - 1] == '.')
            {
                path = path.substr(0, path.size() - 2);
            }
            size_t index = path.find_last_of('/');
            path = path.substr(0, index);
            path += "/";

        return path;
    }

    bool VulkanUtil::GetFileContent(const std::string& pathFile, std::string& contentFile)
    {
        std::ifstream file;
        // ensure ifstream objects can throw exceptions:
        file.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            file.open(pathFile);
            std::stringstream fileStream;
            // read file's buffer contents into streams
            fileStream << file.rdbuf();
            // close file handlers
            file.close();
            // convert stream into string
            contentFile   = fileStream.str();
        }
        catch(std::ifstream::failure e)
        {
            std::cout << "VulkanUtil::GetFileContent: Read file: [" << pathFile << "] error !" << std::endl;
            return false;
        }
        
        return true;
    }

    std::vector<char> VulkanUtil::ReadFile(const std::string& pathFile)
    {
        std::ifstream file(pathFile, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            throw std::runtime_error("VulkanUtil::ReadFile: Failed to open file: " + pathFile);
        }

        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

}; //LibUtil