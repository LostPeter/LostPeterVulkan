/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-01
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FPreInclude.h"
#include "../include/FLogManager.h"
#include "../include/FLog.h"

namespace LostPeterFoundation
{
#if LP_PLATFORM == LP_PLATFORM_ANDROID

#else
	#define buffer_len 2048

	static FLog* s_pLogConsole = nullptr;
	static FLog* s_pLogFile = nullptr;
	

	static void s_EnsureLog()
	{
		if (s_pLogConsole == nullptr)
		{
			s_pLogConsole = FLogManager::GetInstance()->CreateLogPtr(FLogManager::ms_strLogDefault_Console, F_Log_Console);
			s_pLogConsole->InitLogConsole();
		}
		if (s_pLogFile == nullptr)
		{
			s_pLogFile = FLogManager::GetInstance()->CreateLogPtr(FLogManager::ms_strLogDefault_File, F_Log_File);
			s_pLogFile->InitLogFile("Log/LostPeter.log", true);
		}
	} 

    void F_LogError(const char* fmt, ...) 
	{
		s_EnsureLog();

		char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';

		s_pLogConsole->LogErrorString(szTemp);
		s_pLogFile->LogErrorString(szTemp);
	}

	void F_LogWarning(const char* fmt, ...)
	{
		s_EnsureLog();

		char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';

		s_pLogConsole->LogWarnString(szTemp);
		s_pLogFile->LogWarnString(szTemp);
	}

	void F_LogInfo(const char* fmt, ...) 
	{
		s_EnsureLog();

		char szTemp[buffer_len] = {0};
		va_list ap;
		va_start(ap, fmt);
		vsprintf(szTemp, fmt, ap);
		va_end(ap);

		size_t nLen = strlen(szTemp);
		if (nLen > buffer_len - 1)
            nLen = buffer_len - 1;
        szTemp[nLen+1] = '\0';

		s_pLogConsole->LogInfoString(szTemp);
		s_pLogFile->LogInfoString(szTemp);
	}

#endif
    
}; //LostPeterFoundation