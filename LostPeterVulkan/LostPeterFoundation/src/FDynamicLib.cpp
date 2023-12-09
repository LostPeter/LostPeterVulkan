/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/FDynamicLib.h"

namespace LostPeterFoundation
{
    FDynamicLib::FDynamicLib()
		: m_strName("")
		, m_strPath("")
		, m_hInst(nullptr)
	{

	}

    FDynamicLib::FDynamicLib(const String& strName, const String& strPath)
		: m_strName(strName)
		, m_strPath(strPath)
		, m_hInst(nullptr)
	{
	
	}

	FDynamicLib::~FDynamicLib()
	{

	}

	bool FDynamicLib::Load()
	{
		F_LogInfo("FDynamicLib::Load: Loading library [%s] ...", this->m_strName.c_str());

		String path = this->m_strPath;
#if F_PLATFORM == F_PLATFORM_WINDOW
        if (path.substr(path.find_last_of(".") + 1) != "dll")
            path += ".dll";
#elif F_PLATFORM == F_PLATFORM_MAC || F_PLATFORM == F_PLATFORM_iOS 
		if (path.substr(path.find_last_of(".") + 1) != String("dylib"))
            path += ".dylib";
#elif F_PLATFORM == F_PLATFORM_LINUX || F_PLATFORM == F_PLATFORM_ANDROID 
		if (path.find(".so") == String::npos)
		{
			path += ".so";
		}
#else 
	#pragma error "UnKnown platform, FDynamicLib.cpp Abort! Abort!"
#endif
		this->m_hInst = (DYNLIB_HANDLE)DYNLIB_LOAD(path.c_str());

#if F_PLATFORM == F_PLATFORM_MAC || F_PLATFORM == F_PLATFORM_iOS 
        if (!this->m_hInst)
        {
            // Try again as a framework
            this->m_hInst = (DYNLIB_HANDLE)FRAMEWORK_LOAD(this->m_strPath);
        }
#endif

		if (!this->m_hInst)
		{
			F_LogError("*********************** FDynamicLib::Load: Load library [%s] failed!  System Error: [%s] !", this->m_strName.c_str(), _dynlibError().c_str());
			return false;
		}

		F_LogInfo("FDynamicLib::Load: Load library [%s] success !", this->m_strName.c_str());
		return true;
	}

	bool FDynamicLib::Unload()
	{
		F_LogInfo("FDynamicLib::Unload: Unloading library [%s] ...", this->m_strName.c_str());
		
		if (this->m_hInst)
		{
			if (DYNLIB_UNLOAD(this->m_hInst))
			{
				F_LogError("*********************** FDynamicLib::Unload: Unload library [%s] failed!  System Error: [%s] !", this->m_strName.c_str(), _dynlibError().c_str());
				return false;
			}
		}

		F_LogInfo("FDynamicLib::Unload: Unload library [%s] success !", this->m_strName.c_str());
		return true;
	}

	void* FDynamicLib::GetSymbol(const String& strName) const throw()
	{
		return (void*)DYNLIB_GETSYM(this->m_hInst, strName.c_str());
	}

	String FDynamicLib::_dynlibError() 
	{
#if F_PLATFORM == F_PLATFORM_WINDOW
		LPVOID lpMsgBuf; 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			0, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			0 
			); 
		String ret = (char*)lpMsgBuf;
		LocalFree(lpMsgBuf);
		return ret;
#elif F_PLATFORM == F_PLATFORM_MAC || F_PLATFORM == F_PLATFORM_iOS || F_PLATFORM == F_PLATFORM_LINUX || F_PLATFORM == F_PLATFORM_ANDROID
	 	const char* errorStr = dlerror();
        if (errorStr)
            return String(errorStr);
        else
            return String("");
#else
		return String("");
#endif
	}
    
}; //LostPeterFoundation