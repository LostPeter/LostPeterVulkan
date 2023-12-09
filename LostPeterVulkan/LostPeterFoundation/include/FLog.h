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

#ifndef _F_LOG_H_
#define _F_LOG_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class foundationExport FLog
    {
    public:
        FLog(const String& strName);
        virtual ~FLog();

    public:
    protected:
        String m_strName;
        std::shared_ptr<spdlog::logger> m_pLogger;

    public:
        const String& GetName() const { return m_strName; }
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
        virtual bool InitLogFile(const String& path, bool isTruncate) = 0;  
    };

    class foundationExport FLogConsole : public FLog
    {
    public:
        FLogConsole(const String& strName);
        virtual ~FLogConsole();

    public:
        

    public:
        virtual void Destroy();
        virtual bool InitLogConsole();
        virtual bool InitLogFile(const String& path, bool isTruncate);  
    };

    class foundationExport FLogFile : public FLog
    {
    public:
        FLogFile(const String& strName);
        virtual ~FLogFile();

    public:


    public:
        virtual void Destroy();
        virtual bool InitLogConsole();
        virtual bool InitLogFile(const String& path, bool isTruncate);  
    };

}; //LostPeterFoundation

#endif