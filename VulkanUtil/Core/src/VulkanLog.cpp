/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2022-12-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/PreInclude.h"
#include "../include/VulkanLog.h"
#include "../include/VulkanUtil.h"

namespace LostPeter
{
    VulkanLog::VulkanLog(const String& strName)
        : m_strName(strName)
    {

    }

    VulkanLog::~VulkanLog()
    {
        
    }

    void VulkanLog::LogInfoString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "VulkanLog::LogInfoString");
        m_pLogger->info("{0}", szContent);
    }
    void VulkanLog::LogInfo(const char* fmt, ...)
    {
        const int buffer_len = 2048;
        char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';

        LogInfoString(szTemp);
    }

    void VulkanLog::LogWarnString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "VulkanLog::LogWarnString");
        m_pLogger->warn("{0}", szContent);
    }
    void VulkanLog::LogWarn(const char* fmt, ...)
    {
         const int buffer_len = 2048;
        char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';
        
        LogWarnString(szTemp);
    }

    void VulkanLog::LogErrorString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "VulkanLog::LogErrorString");
        m_pLogger->error("{0}", szContent);
    }
    void VulkanLog::LogError(const char* fmt, ...)
    {
        const int buffer_len = 2048;
        char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';
        
        LogErrorString(szTemp);
    }

    VulkanLogConsole::VulkanLogConsole(const String& strName)
        : VulkanLog(strName)
    {

    }

    VulkanLogConsole::~VulkanLogConsole()
    {
        Destroy();
    }

    void VulkanLogConsole::Destroy()
    {

    }

    bool VulkanLogConsole::InitLogConsole()
    {
        m_pLogger = spdlog::stdout_color_mt(m_strName);

        return true;
    }
    bool VulkanLogConsole::InitLogFile(const String& path, bool isTruncate)
    {
        return true;
    }


    VulkanLogFile::VulkanLogFile(const String& strName)
        : VulkanLog(strName)
    {

    }

    VulkanLogFile::~VulkanLogFile()
    {
        Destroy();
    }

    void VulkanLogFile::Destroy()
    {

    }

    bool VulkanLogFile::InitLogConsole()
    {
        return true;
    }
    bool VulkanLogFile::InitLogFile(const String& path, bool isTruncate)
    {
        String strBin = VulkanUtil::GetPathBin();
        String strPath = strBin + "/" + path;
        m_pLogger = spdlog::basic_logger_mt(m_strName, strPath, isTruncate);

        return true;
    }

}; //LostPeter