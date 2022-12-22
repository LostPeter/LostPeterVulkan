// author: LostPeter
// time:   2022-12-03

#ifndef _VULKAN_LOG_H_
#define _VULKAN_LOG_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanLog
    {
    public:
        VulkanLog(const std::string& strName);
        virtual ~VulkanLog();

    public:
    protected:
        std::string m_strName;
        std::shared_ptr<spdlog::logger> m_pLogger;

    public:
        const std::string& GetName() const { return m_strName; }
        spdlog::logger* GetLogger() const { return m_pLogger.get(); }

    public:
        void LogInfoString(const char* szContent);
        void LogInfo(const char* fmt, ...);

        void LogWarnString(const char* szContent);
        void LogWarn(const char* fmt, ...);

        void LogErrorString(const char* szContent);
        void LogError(const char* fmt, ...);

    public:
        virtual void Destroy() = 0;
        virtual bool InitLogConsole() = 0;
        virtual bool InitLogFile(const std::string& path, bool isTruncate) = 0;  
    };

    class utilExport VulkanLogConsole : public VulkanLog
    {
    public:
        VulkanLogConsole(const std::string& strName);
        virtual ~VulkanLogConsole();

    public:
        

    public:
        virtual void Destroy();
        virtual bool InitLogConsole();
        virtual bool InitLogFile(const std::string& path, bool isTruncate);  
    };

    class utilExport VulkanLogFile : public VulkanLog
    {
    public:
        VulkanLogFile(const std::string& strName);
        virtual ~VulkanLogFile();

    public:


    public:
        virtual void Destroy();
        virtual bool InitLogConsole();
        virtual bool InitLogFile(const std::string& path, bool isTruncate);  
    };

}; //LostPeter

#endif