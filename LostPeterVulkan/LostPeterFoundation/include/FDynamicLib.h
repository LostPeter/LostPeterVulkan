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

#ifndef _F_DYNAMIC_LIB_H_
#define _F_DYNAMIC_LIB_H_

#include "FPreDefine.h"

#if LP_PLATFORM == LP_PLATFORM_WIN32
	#define DYNLIB_HANDLE			hInstance
	#define DYNLIB_LOAD(a)			LoadLibraryEx(a, 0, LOAD_WITH_ALTERED_SEARCH_PATH)
	#define DYNLIB_GETSYM(a, b)		GetProcAddress(a, b)
	#define DYNLIB_UNLOAD(a)		!FreeLibrary(a)

	struct HINSTANCE__;
	typedef struct HINSTANCE__* hInstance;

#elif LP_PLATFORM == LP_PLATFORM_MAC || LP_PLATFORM == LP_PLATFORM_IOS 
    #include "FUtilOC.h"
	#define DYNLIB_HANDLE			void*
	#define DYNLIB_LOAD(a)			F_OCMac_LoadExeBundle(a)
	#define FRAMEWORK_LOAD(a) 		F_OCMac_LoadFramework(a)
	#define DYNLIB_GETSYM(a,b)		dlsym(a,b)
	#define DYNLIB_UNLOAD(a)		dlclose(a)

#elif LP_PLATFORM == LP_PLATFORM_LINUX || LP_PLATFORM == LP_PLATFORM_ANDROID 
	#define DYNLIB_HANDLE			void*
	#define DYNLIB_LOAD(a)			dlopen(a,RTLD_LAZY | RTLD_GLOBAL)
	#define DYNLIB_GETSYM(a,b)		dlsym(a,b)
	#define DYNLIB_UNLOAD(a)		dlclose(a)

#else 
	#pragma error "UnKnown platform, FDynamicLib.h Abort! Abort!"
#endif

namespace LostPeterFoundation
{
    class LPF_Export FDynamicLib
	{
	public:
		FDynamicLib();
		FDynamicLib(const String& strName, const String& strPath);
		~FDynamicLib();

	protected:
		String m_strName;		
		String m_strPath;	
		DYNLIB_HANDLE m_hInst;

	public:
		LP_FORCEINLINE const String& GetName() const 
        { 
            return this->m_strName; 
        }
		LP_FORCEINLINE const String& GetPath() const 
        { 
            return this->m_strPath; 
        }

		LP_FORCEINLINE bool IsLoaded() const
		{
			return this->m_hInst != nullptr;
		}
	
	public:
		bool Load();
		bool Unload();

		void* GetSymbol(const String& strName) const throw();

	private:
		String _dynlibError();
	};

}; //LostPeterFoundation

#endif