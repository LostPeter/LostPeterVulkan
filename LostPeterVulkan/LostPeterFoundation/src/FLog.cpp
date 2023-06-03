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

#include "../include/FPreInclude.h"
#include "../include/FLog.h"
#include "../include/FUtil.h"

namespace LostPeterFoundation
{
    FLog::FLog(const String& strName)
        : m_strName(strName)
    {

    }

    FLog::~FLog()
    {
        
    }

    void FLog::LogInfoString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "FLog::LogInfoString");
        m_pLogger->info("{0}", szContent);
    }
    void FLog::LogInfo(const char* fmt, ...)
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

    void FLog::LogWarnString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "FLog::LogWarnString");
        m_pLogger->warn("{0}", szContent);
    }
    void FLog::LogWarn(const char* fmt, ...)
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

    void FLog::LogErrorString(const char* szContent)
    {
        assert(m_pLogger.get() != nullptr && "FLog::LogErrorString");
        m_pLogger->error("{0}", szContent);
    }
    void FLog::LogError(const char* fmt, ...)
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

    FLogConsole::FLogConsole(const String& strName)
        : FLog(strName)
    {

    }

    FLogConsole::~FLogConsole()
    {
        Destroy();
    }

    void FLogConsole::Destroy()
    {

    }

    bool FLogConsole::InitLogConsole()
    {
        m_pLogger = spdlog::stdout_color_mt(m_strName);

        return true;
    }
    bool FLogConsole::InitLogFile(const String& path, bool isTruncate)
    {
        return true;
    }


    FLogFile::FLogFile(const String& strName)
        : FLog(strName)
    {

    }

    FLogFile::~FLogFile()
    {
        Destroy();
    }

    void FLogFile::Destroy()
    {

    }

    bool FLogFile::InitLogConsole()
    {
        return true;
    }
    bool FLogFile::InitLogFile(const String& path, bool isTruncate)
    {
        String strBin = FUtil::GetPathBin();
        String strPath = strBin + "/" + path;
        m_pLogger = spdlog::basic_logger_mt(m_strName, strPath, isTruncate);

        return true;
    }

}; //LostPeterFoundation