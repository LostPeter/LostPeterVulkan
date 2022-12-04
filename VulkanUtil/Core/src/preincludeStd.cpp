#include "../include/preincludeStd.h"
#include "../include/vulkanlogmanager.h"
#include "../include/vulkanlog.h"

namespace LostPeter
{
#if UTIL_PLATFORM == UTIL_PLATFORM_ANDROID

#else
	#define buffer_len 2048

	static VulkanLog* s_pLogConsole = nullptr;
	static VulkanLog* s_pLogFile = nullptr;
	

	static void s_EnsureLog()
	{
		if (s_pLogConsole == nullptr)
		{
			s_pLogConsole = VulkanLogManager::GetInstance()->CreateLogPtr(VulkanLogManager::ms_strLogDefault_Console, Vulkan_Log_Console);
			s_pLogConsole->InitLogConsole();
		}
		if (s_pLogFile == nullptr)
		{
			s_pLogFile = VulkanLogManager::GetInstance()->CreateLogPtr(VulkanLogManager::ms_strLogDefault_File, Vulkan_Log_File);
			s_pLogFile->InitLogFile("Log/LostPeter.log", true);
		}
	} 

    void Util_LogError(const char* fmt, ...) 
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

	void Util_LogWarning(const char* fmt, ...)
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

	void Util_LogInfo(const char* fmt, ...) 
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

}; //LostPeter